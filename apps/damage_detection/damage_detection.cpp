#include <windows.h>
#include <memory>

// Cabeceras de OpenCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/calib3d.hpp"

// Cabeceras propias
#include "core/utils.h"
#include "core/console.h"
#include "core/messages.h"
#include "feature_detection/linedetector.h"
#include "VideoStream.h"
#include "matching.h"
#include "fourier.h"
#include "img_processing.h"
#include "transform.h"

using namespace I3D;
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;


/*!
 * Detección de daños en conductores de líneas eléctricas:
 *
 * img:         Imagen de los conductores
 * out:         Directorio de salida donde se guarda el log y toda la información generada
 * l_detect:    Detector de lineas. Puede ser HOUGH, HOUGHP, HOUGH_FAST o LSD
 */
int main(int argc, char *argv[])
{

  // Barra de progreso
  ProgressBar progress_bar;

  char name[I3D_MAX_FNAME];
  getFileName(getRunfile(), name, I3D_MAX_FNAME);
  char dir[I3D_MAX_DIR];
  getFileDriveDir(getRunfile(), dir, I3D_MAX_DIR);

  // Parser del comando 
  CmdParser cmdParser(name, "Detección de daños en conductores de líneas eléctricas");
  cmdParser.addParameter("img", "Imagen de los conductores");
  cmdParser.addParameter("out", "Directorio de salida donde se guarda el log y toda la información generada", true);
  cmdParser.addParameterOption("l_detect", "HOUGH,HOUGHP,HOUGH_FAST,LSD", "Detector de lineas", true, "HOUGHP");
  if ( cmdParser.parse(argc, argv) == CmdParser::MSG::PARSE_ERROR ) {
    cmdParser.printHelp();
    exit(EXIT_FAILURE);
  }

  std::string img = cmdParser.getValue<std::string>("img");
  std::string out_path = cmdParser.getValue<std::string>("out");
  LD_TYPE ls = cmdParser.getParameterOptionIndex<LD_TYPE>("l_detect");

  if (createDir(out_path.c_str()) == -1) { 
    consolePrintError("No se ha podido crear el directorio: %s", out_path.c_str()); 
    exit(EXIT_FAILURE);
  }

  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  sprintf(logfile, "%s\\%s.log", out_path.c_str(), name );
  Message::setMessageLogFile(logfile);
  Message::setMessageLevel(MessageLevel::MSG_INFO);

  // Procesos que se aplican a las imagenes
  // Hacemos un remuestreo al 25% para que tarde menos en determinar los buffer de lineas
  std::shared_ptr<I3D::Resize> resize = std::make_shared<I3D::Resize>(25.);

  //std::shared_ptr<FunctionProcess> fProcess1 = std::make_shared<FunctionProcess>(
  //  [](const cv::Mat &in, cv::Mat *out) {
  //    in.convertTo(*out, CV_32F);
  //});
  //int kernel_size = 31;
  //double sig = 1., th = I3D_PI / 2., lm = 1.0, gm = 0.02/*, ps = 0.*/;
  //cv::Mat kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sig, th, lm, gm);
  //// Se inicializa Filter2D. Posteriormente se modifica según el angulo obtenido por fourier
  //std::shared_ptr<I3D::Filter2D> filter2d = std::make_shared<I3D::Filter2D>(CV_32F, kernel);
  //std::shared_ptr<FunctionProcess> fProcess2 = std::make_shared<FunctionProcess> (
  //  [&](const cv::Mat &in, cv::Mat *out) {
  //    cv::normalize(in, *out, 0, 255, CV_MINMAX); 
  //    out->convertTo(*out, CV_8U); 
  //    out->colRange(kernel_size, out->cols - kernel_size).rowRange(31, out->rows - kernel_size).copyTo(*out); 
  //});

  std::shared_ptr<I3D::BilateralFilter> bilateralFilter = std::make_shared<I3D::BilateralFilter>(5, 50., 50.);
  std::shared_ptr<I3D::Erotion> erotion = std::make_shared<I3D::Erotion>(1);
  std::shared_ptr<I3D::Dilate> dilate = std::make_shared<I3D::Dilate>(1);

  // Listado de procesos
  I3D::ImgProcessingList imgprolist { 
    /*fProcess1, 
    filter2d,
    fProcess2,*/
    resize,
    bilateralFilter,
    erotion,
    dilate
  };

  if (ls == LD_TYPE::LSD) {
    imgprolist.add(std::make_shared<I3D::Sobel>(1, 0, 3, 1., 0.));
  } else {
    imgprolist.add(std::make_shared<I3D::Canny>());
  }

  // Detector de líneas
  std::unique_ptr<LineDetector> pLineDetector;
  cv::Scalar ang_tol(CV_PI / 2, 0.25);
  
  switch ( ls ) {
    case I3D::LD_TYPE::HOUGH:
      pLineDetector = std::make_unique<ldHouh>(150, ang_tol);
      break;
    case I3D::LD_TYPE::HOUGHP:
      pLineDetector = std::make_unique<ldHouhP>(100, ang_tol, 60., 30.);
      break;
    case I3D::LD_TYPE::HOUGH_FAST:
      pLineDetector = std::make_unique<ldHouhFast>();
      break;
    case I3D::LD_TYPE::LSD:
      pLineDetector = std::make_unique<ldLSD>(ang_tol);
      break;
    default:
      printError("No se ha seleccionado ningún detector de lineas.");
      exit(EXIT_FAILURE);
      break;
  }

  printInfo("Leyendo imagen %s", img.c_str());
  cv::Mat full_image = cv::imread(img.c_str());
  if (full_image.empty()) exit(EXIT_FAILURE);

  cv::Mat image;
  full_image.colRange(1000, full_image.cols - 1000).copyTo(image);
  full_image.release();
  cv::Mat image_gray;
  cvtColor(image, image_gray, CV_BGR2GRAY);
  // Determinar angulo con Fourier
  std::vector<int> colFourier;
  colFourier.push_back(image_gray.cols / 2);
  std::vector<std::vector<cv::Point>> ptsFourier;
  double angleFourier = fourierLinesDetection(image_gray, colFourier, &ptsFourier);

  // Se aplica el procesado previo a la imagen
  // kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sig, angleFourier, lm, gm);
  // filter2d->setParameters(CV_32F, kernel);
  cv::Mat image_pro;
  if ( imgprolist.execute(image_gray, &image_pro) == ProcessExit::FAILURE) 
    exit(EXIT_FAILURE);

  // Detección de lineas

  if (pLineDetector->run(image_pro, cv::Scalar(angleFourier, 0.15)) == LineDetector::Exit::FAILURE) 
    exit(EXIT_FAILURE);

  pLineDetector->drawLines(image_gray);
  const std::vector<Line> &detect_lines = pLineDetector->getLines();
  if ( detect_lines.empty() ) { 
    printError("No se han detectando lineas");
    exit(EXIT_FAILURE);
  }
  printInfo("Número de lineas detectada %i", detect_lines.size());

  std::vector<ldGroupLines> linesGroups;
  groupLinesByDist(detect_lines, &linesGroups, 20);
  std::vector<Line> linesJoin;
  for (size_t ig = 0; ig < linesGroups.size(); ig++) {
    joinLinesByDist(linesGroups[ig].getLines(), &linesJoin, 40);
  }

  cv::Mat gap = cv::Mat::zeros(image_gray.size(), CV_8U);

  Helmert2D<cv::Point> trf(0, 0, 4., 0.0);

  //std::vector<std::vector<cv::Point>> buffer;
  cv::Scalar m, stdv;

  bilateralFilter->execute(image_gray, &image_gray);

  int widthBuffer = 80;

  for (auto &line : linesJoin) {
    cv::Mat gap = cv::Mat::zeros(image_gray.size(), CV_8U);
    trf.transformEntity(line, &line, transform_order::DIRECT);
    //Para comprobar...
    cv::line(image_gray, line.pt1, line.pt2, cv::Scalar(255, 0, 0));
    std::vector<cv::Point> buff;
    lineBuffer(line, widthBuffer, &buff);

    // Se aplica la mascara a la imagen y obtenemos la zona de estudio.
    cv::Mat mask = cv::Mat::zeros(image_gray.size(), CV_8U);
    cv::Mat aux(buff);
    const cv::Point *pts = (const cv::Point*) aux.data;
    int npts = aux.rows;
    cv::fillPoly(mask, &pts, &npts, 1, cv::Scalar(1, 1, 1) );
    cv::Mat searchArea;
    cv::bitwise_and(image_gray, image_gray, searchArea, mask);
    //imshow("Fourier", searchArea);
    //cv::waitKey();

    // binarización de la imagen
    cv::meanStdDev(image_gray, m, stdv, mask);
    cv::Mat imgBN;
    cv::threshold(searchArea, imgBN, 120/*m[0] + stdv[0]*/, 255, cv::THRESH_BINARY);

    
    //Mirar algoritmos para ver la anchura de la linea.

    cv::LineIterator li(imgBN, line.pt1, line.pt2, 4, false);

    //progress_bar.init(0, li.count, "Obteniendo anchura a lo largo de la línea");
    
    std::vector<int> v_with(li.count, 0);
    cv::Point axis;
    for ( int i = 0; i < li.count; i++, ++li ) {
      axis = li.pos();
      // Tenemos las coordenadas del eje de la línea. A partir de esto buscar la anchura para 
      // cada punto
      // Tendria que estimar la anchura del aislador antes. Así podría servirme para el
      // buffer y para determinar esta linea auxiliar
      Line laux(axis, line.angleOX(), widthBuffer, true);
      // Ahora con un LineIterator recorro la linea y busco el cambio de b/n
      cv::LineIterator li2(imgBN, laux.pt1, laux.pt2, 4, false);
      int width = 0;  //Anchura a lo largo de la línea
      int ini = -1;
      int end = 0;
      std::vector<int> v_aux(li2.count, 0);
      std::vector<cv::Point> v_pts_aux(li2.count);
      int value_prev = -1;
      for ( int j = 0; j < li2.count; j++, ++li2 ) {
        cv::Point pt_aux = li2.pos();
        int value = (int)li2.ptr[0];
        if ( value_prev == -1 ) {
          if ( value == 255 )
            v_aux[j] = 1;
        } else {
          int dif = value - value_prev;
          if ( dif < 0 ) {
            v_aux[j] = -1;
          } else if ( dif > 0 ) {
            v_aux[j] = 1;
          } else {
            v_aux[j] = v_aux[j - 1];
          }
        }
        value_prev = value;

        if ( value == 255 ) {
          if (ini == -1) ini = j;
          end = j;
          width++;
        }
        v_pts_aux[j] = li2.pos();
      }

      // Anchura teniendo en cuenta los huecos
      v_with[i] = end - ini + 1; // width;
      //v_with[axis.x] = width;
      // Busqueda del primer punto, último punto y huecos en la linea
      //std::vector<int>::iterator it_first, it_last;
      //int nb[] = { 1, -1 };
      //it_first = std::find_first_of(v_aux.begin(), v_aux.end(), nb, nb);
      //it_last = std::find_end(v_aux.begin(), v_aux.end(), nb+1, nb+1);

      std::vector<int> v_(li2.count);
      for ( int k = ini; k < end; k++ ) {
        if ( v_aux[k] == -1 ) {
          cv::line(gap, v_pts_aux[k], v_pts_aux[k], cv::Scalar(255));
        }
      }


      //if ( width != end - ini ) 
      //  printWarning("Posible daño");
      //logPrintInfo("Ancho línea %i", width);
      //progress_bar();
    }

    // Buscar máxima y mínima anchura
    cv::Mat mat_aux(v_with);
    //cv::Mat mat_aux = cv::Mat(v_with.size(), 1, CV_64F, v_with.data());
    cv::Scalar m, stdv;
    cv::meanStdDev(mat_aux, m, stdv);
    double th1 = m[0] - stdv[0] - 1; // Un pixel mas de margen
    double th2 = m[0] + stdv[0] + 1;
    
    cv::LineIterator li3(imgBN, line.pt1, line.pt2, 4, false);
    // Ahora buscar si quedan pixeles fuera
    
    // Arrastramos una media de 25 posiciones para ver la variación
    //std::vector<int> accumulated;

    cv::Point iniDamage = cv::Point(0, 0);
    cv::Point endDamage = cv::Point(0, 0);
    std::vector<SegmentI> segmentDamage;
    printInfo("Ancho línea: Mínimo: %f. Máximo: %f", th1, th2);
    progress_bar.init(0, v_with.size()-50, "Busqueda de zonas dañadas");
    for (int is = 25; is < v_with.size() - 25; is++, ++li3) {
      double sum = std::accumulate(v_with.begin() + is - 25, v_with.begin() + is + 25, 0);
      int accumul = I3D_ROUND_TO_INT(sum / 50.);
      if ( accumul < I3D_ROUND_TO_INT(th1) || accumul > I3D_ROUND_TO_INT(th2) ) {
        // Pixel fuera de rango.
        axis = li3.pos();
        if (iniDamage == cv::Point(0, 0)) iniDamage = axis;
        endDamage = axis; 
        logPrintWarning("Posible daño (%i, %i). Ancho línea: %i. Mínimo: %f. Máximo: %f", axis.x, axis.y, accumul, th1, th2);
        // Ir acumulando la zona total del daño
      } else {
        if (iniDamage != cv::Point(0, 0)) {
          //cv::Mat imageRGB = cv::imread(img.c_str());
          //Line line(ini_, end_);
          //std::vector<cv::Point> buff;
          //lineBuffer(line, th2 + 10, &buff);
          
          //cv::Mat aux(buff);
          //const cv::Point *pts = (const cv::Point*) aux.data;
          //int npts = aux.rows;
          //cv::polylines(imageRGB, &pts, &npts, 1, true, Color::randomColor().get<cv::Scalar>() );
          LineIterator it3 = li3;
          for ( int k = 0; k < 24; k++ ) ++it3; // Un poco ñapas pero para salir del paso. LineIterator esta poco documentada. 
          endDamage = it3.pos();
          segmentDamage.push_back(SegmentI(iniDamage, endDamage));
          
          iniDamage = cv::Point(0, 0);
          endDamage = cv::Point(0, 0);
        }
      }

      // Buscar pixel a pixel no parece lo mas correcto
      //if ( v_with[is] < th1 || v_with[is] > th2 ) {
      //  // Pixel fuera de rango.
      //  axis = li3.pos();
      //  logPrintWarning("Posible daño (%i, %i)", axis.x, axis.y);
      //}
      progress_bar();
    }

    // Se muestran las zonas encontradas
    for ( int isd = 0; isd < segmentDamage.size(); isd++ ) {
      SegmentI line = segmentDamage[isd];
      if ( line.length() > 10. ) {
        WindowI w_aux = expandWindow(segmentDamage[isd].getWindow(), 100);
        w_aux = windowIntersection(w_aux, WindowI(cv::Point(0, 0), cv::Point(image.cols, image.rows)));
        
        cv::Mat m_aux;
        image.rowRange(w_aux.pt1.y, w_aux.pt2.y).colRange(w_aux.pt1.x, w_aux.pt2.x).copyTo(m_aux);
        
        std::vector<cv::Point> buff;
        I3D::Translate<cv::Point> trf(w_aux.pt1.x, w_aux.pt1.y);
        trf.transformEntity(line, &line, transform_order::DIRECT);
        lineBuffer(line, th2 + 10, &buff);
        cv::Mat aux(buff);
        const cv::Point *pts = (const cv::Point*) aux.data;
        int npts = aux.rows;
        cv::polylines(m_aux, &pts, &npts, 1, true, Color::randomColor().get<cv::Scalar>() );
        
        cv::imshow("Warning", m_aux);
        cv::waitKey();
      }

    }

    cv::Mat canny_output;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours( gap, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    //cv::Mat imageRGB = cv::imread(img.c_str());
    for( size_t i = 0; i< contours.size(); i++ ) {
      if (contours[i].size() < 3) continue;
      cv::Scalar color = Color::randomColor().get<cv::Scalar>();
      //cv::Rect boundRect = boundingRect( contours[i] );
      WindowI w_aux = cvRectToWindow(boundingRect( contours[i] ));
      w_aux = expandWindow(w_aux, 10);
      cv::rectangle( image, w_aux.pt1, w_aux.pt2, color, 2, 8, 0 );
      //cv::drawContours( imageRGB, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
      w_aux = expandWindow(w_aux, 100);
      w_aux = windowIntersection(w_aux, WindowI(cv::Point(0, 0), cv::Point(image.cols, image.rows)));
      cv::Mat m_aux;
      image.rowRange(w_aux.pt1.y, w_aux.pt2.y).colRange(w_aux.pt1.x, w_aux.pt2.x).copyTo(m_aux);
      cv::imshow("Warning", m_aux);
      cv::waitKey();
    }
    //image.release();
    // La busqueda tiene que hacerse contando con los pixeles vecinos.
    // ¿Aplicar un suavizado para eliminar picos?

  }
  
  
  
  return 0;
}