//--img=D:\Desarrollo\datos\elementos_a_detectar\2K7A7854.jpg --out=D:\Desarrollo\datos\elementos_a_detectar\2K7A7854  // Encuentra baliza con medianBlur
//--img=D:\Desarrollo\datos\elementos_a_detectar\2K7A7973.jpg --out=D:\Desarrollo\datos\elementos_a_detectar\2K7A7973  // Encuentra baliza con medianBlur
//--img=D:\Desarrollo\datos\elementos_a_detectar\2K7A8288.jpg --out=D:\Desarrollo\datos\elementos_a_detectar\2K7A8288  // Encuentra baliza con medianBlur
//--img=D:\Desarrollo\datos\elementos_a_detectar\2K7A8308.jpg --out=D:\Desarrollo\datos\elementos_a_detectar\2K7A8308  // Encuentra baliza con medianBlur

//--img=D:\Desarrollo\datos\elementos_a_detectar\2K7A0728.jpg --out=D:\Desarrollo\datos\elementos_a_detectar\2K7A0728
//--img=D:\Desarrollo\datos\elementos_a_detectar\2K7A0736.jpg --out=D:\Desarrollo\datos\elementos_a_detectar\2K7A0736  // Encuentra baliza con gaussianBlur
//--img=D:\Desarrollo\datos\elementos_a_detectar\2K7A7949.jpg --out=D:\Desarrollo\datos\elementos_a_detectar\2K7A7949
//--img=D:\Desarrollo\datos\elementos_a_detectar\2K7A7954.jpg --out=D:\Desarrollo\datos\elementos_a_detectar\2K7A7954
//--img=D:\Desarrollo\datos\elementos_a_detectar\2K7A8066.jpg --out=D:\Desarrollo\datos\elementos_a_detectar\2K7A8066
//--img=D:\Desarrollo\datos\elementos_a_detectar\2K7A8187.jpg --out=D:\Desarrollo\datos\elementos_a_detectar\2K7A8187


//--img=D:\Desarrollo\datos\elementos_a_detectar\2K7A8424.jpg --out=D:\Desarrollo\datos\elementos_a_detectar\2K7A8424
//--img=D:\Desarrollo\datos\elementos_a_detectar\2K7A0699.jpg --out=D:\Desarrollo\datos\elementos_a_detectar\2K7A0699  // No hay baliza y saca circulos en zonas que no hay

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
#include "LineDetector.h"
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

  ProgressBar progress_bar;

  char name[I3D_MAX_FNAME];
  getFileName(getRunfile(), name, I3D_MAX_FNAME);
  char dir[I3D_MAX_DRIVE + I3D_MAX_DIR];
  getFileDriveDir(getRunfile(), dir, I3D_MAX_DRIVE + I3D_MAX_DIR);

  CmdParser cmdParser(name, "Detección de daños en conductores de líneas eléctricas");
  cmdParser.addParameter("img", "Imagen de los conductores");
  cmdParser.addParameter("out", "Directorio de salida donde se guarda el log y toda la información generada", true);
  if ( cmdParser.parse(argc, argv) == CmdParser::MSG::PARSE_ERROR ) {
    cmdParser.printHelp();
    exit(EXIT_FAILURE);
  }

  std::string img = cmdParser.getValue<std::string>("img");
  std::string out_path = cmdParser.getValue<std::string>("out");

  if (createDir(out_path.c_str()) == -1) { 
    consolePrintError("No se ha podido crear el directorio: %s", out_path.c_str()); 
    exit(EXIT_FAILURE);
  }

  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  sprintf(logfile, "%s\\%s.log", out_path.c_str(), name );
  Message::setMessageLogFile(logfile);
  Message::setMessageLevel(MessageLevel::MSG_INFO);

  //std::shared_ptr<I3D::Normalize> normalize = std::make_shared<I3D::Normalize>(255, 0);
  std::shared_ptr<I3D::BilateralFilter> bilateralFilter = std::make_shared<I3D::BilateralFilter>(5, 50., 50.);
  std::shared_ptr<I3D::Erotion> erotion = std::make_shared<I3D::Erotion>(1);
  std::shared_ptr<I3D::Dilate> dilate = std::make_shared<I3D::Dilate>(1);
  std::shared_ptr<I3D::Canny> canny = std::make_shared<I3D::Canny>();
  std::shared_ptr<I3D::GaussianBlur> gaussianBlur = std::make_shared<I3D::GaussianBlur>(cv::Size(9, 9), 2., 2.);
  std::shared_ptr<I3D::MedianBlur> medianBlur = std::make_shared<I3D::MedianBlur>(5);
  std::shared_ptr<I3D::EqualizeHistogram> equalizeHistogram = std::make_shared<I3D::EqualizeHistogram>();

  cv::Mat image = cv::imread(img.c_str());
  //I3D::Resize res(50.);
  //res.execute(image, &image);
  
  // Ventana de la zona baja de la torre
  //WindowI w(cv::Point(1290, 6190), cv::Point(3402, 7698));
  // Busqueda de la señal de peligro
  //cv::Mat image_crop1, image_crop2;
  //image.rowRange(500, 3400).colRange(1300, 3400).copyTo(image_crop1);
  //image.rowRange(6193, 7698).colRange(1300, 3400).copyTo(image_crop2);
  //image.release();

  cv::Mat channels[3];
  cv::split(image, channels);
  cv::Mat red = channels[2];
  cv::Mat blue = channels[0];

  // Intento de quitar la sombra de la baliza
  //cv::Mat image_cmyk;
  //rgbToCmyk(image, &image_cmyk);
  //cv::Mat channels_cmyk[4];
  //cv::split(image_cmyk, channels_cmyk);
  //channels_cmyk[3] = 0.f;
  //merge(channels_cmyk, 4, image_cmyk);
  //cv::Mat rgb;
  //cmykToRgb(image_cmyk, &rgb);
  //cv::Mat key = channels_cmyk[3];
  //cv::normalize(key, key, 255, 0, CV_MINMAX);
  //key.convertTo(key, CV_8U);

  //cv::Mat channels[3];
  //cv::split(rgb, channels);
  //cv::Mat red = channels[2];

  //cv::Mat image_gray;
  //cvtColor(image, image_gray, CV_RGB2GRAY);
  //bilateralFilter->execute(image_gray, &image_gray);
  //equalizeHistogram->execute(image_gray, &image_gray);
  bilateralFilter->execute(blue, &blue);

 // cv::medianBlur(image, image, 3);

 // cv::Mat image_hsv;
 // cvtColor(image, image_hsv, CV_RGB2HSV);
 // cv::Mat channels_hsv[3];
 // cv::split(image_hsv, channels_hsv);
 // cv::Mat lower_red_hue_range;
 // cv::Mat upper_red_hue_range;
 // cv::inRange(image_hsv, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
 // cv::inRange(image_hsv, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);
 //	// Combine the above two images
	//cv::Mat red_hue_image;
	//cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
 //
 //	cv::GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);





  // binarización de la imagen
  //cv::Mat red_binary;
  //cv::threshold(red, red_binary, 200, 255, cv::THRESH_BINARY);
  //std::vector<std::vector<cv::Point>> contours;
  //std::vector<cv::Vec4i> hierarchy;
  //cv::findContours( red_binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

  ///// Find contours
  //findContours( red_binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  ///// Approximate contours to polygons + get bounding rects and circles
  //vector<vector<Point> > contours_poly( contours.size() );
  //vector<Rect> boundRect( contours.size() );
  //vector<Point2f>center( contours.size() );
  //vector<float>radius( contours.size() );

  //for( int i = 0; i < contours.size(); i++ ) { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
  //  boundRect[i] = boundingRect( Mat(contours_poly[i]) );
  //  minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
  //}


  ///// Draw polygonal contour + bonding rects + circles
  //for( int i = 0; i< contours.size(); i++ ) {
  //  Scalar color = Scalar( 0, 255, 0 );
  //  drawContours( image, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
  //  rectangle( image, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
  //  circle( image, center[i], (int)radius[i], color, 2, 8, 0 );
  //}







  //cv::Mat image_gray;
  //cvtColor(image, image_gray, CV_RGB2GRAY);

  // libero memoria
  //for (int i = 0; i < 3; i++)
  //  channels[i].release();

  // Busqueda baliza
  medianBlur->execute(blue, &blue);
  //gaussianBlur->execute(image_gray, &image_gray);
  //erotion->execute(red, &red);
  //dilate->execute(red, &red);
  std::vector<Vec3f> circles;
  HoughCircles(blue, circles, HOUGH_GRADIENT, 2, blue.rows/4, 150, 100, 25, 150 );
  for (size_t i = 0; i < circles.size(); i++) {
    cv::Point center(I3D_ROUND_TO_INT(circles[i][0]), I3D_ROUND_TO_INT(circles[i][1]));
    int radius = I3D_ROUND_TO_INT(circles[i][2]);
    circle( image, center, 3, Scalar(0,255,0), -1, 8, 0 );
    circle( image, center, radius, Scalar(0,0,255), 3, 8, 0 );
    printInfo("Baliza detectada: Centro (%i, %i). Radio: %i", center.x, center.y, radius);
  }

  // Recortar la imagen para mostrarla
  //cv::imshow("Baliza", image);
  //cv::waitKey();

  return 0;
}


/*

int main(int argc, char *argv[])
{
  ProgressBar progress_bar;

  char name[I3D_MAX_FNAME];
  getFileName(getRunfile(), name, I3D_MAX_FNAME);
  char dir[I3D_MAX_DRIVE + I3D_MAX_DIR];
  getFileDriveDir(getRunfile(), dir, I3D_MAX_DRIVE + I3D_MAX_DIR);

  CmdParser cmdParser(name, "Detección de daños en conductores de líneas eléctricas");
  cmdParser.addParameter("img", "Imagen de los conductores");
  cmdParser.addParameter("out", "Directorio de salida donde se guarda el log y toda la información generada", true);
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

  //std::shared_ptr<I3D::Normalize> normalize = std::make_shared<I3D::Normalize>(255, 0);
  std::shared_ptr<I3D::BilateralFilter> bilateralFilter = std::make_shared<I3D::BilateralFilter>(5, 50., 50.);
  std::shared_ptr<I3D::Erotion> erotion = std::make_shared<I3D::Erotion>(2);
  std::shared_ptr<I3D::Dilate> dilate = std::make_shared<I3D::Dilate>(3);
  std::shared_ptr<I3D::Canny> canny = std::make_shared<I3D::Canny>();
  std::shared_ptr<I3D::GaussianBlur> gaussianBlur = std::make_shared<I3D::GaussianBlur>(cv::Size(9, 9), 2., 2.);
  cv::Mat image = cv::imread(img.c_str());
  //I3D::Resize res(50.);
  //res.execute(image, &image);
  
  // Ventana de la zona baja de la torre
  //WindowI w(cv::Point(1290, 6190), cv::Point(3402, 7698));
  // Busqueda de la señal de peligro
  cv::Mat image_crop1, image_crop2;
  image.rowRange(500, 3400).colRange(1300, 3400).copyTo(image_crop1);
  image.rowRange(6193, 7698).colRange(1300, 3400).copyTo(image_crop2);
  image.release();

  cv::Mat channels[3];
  cv::split(image_crop1, channels);
  cv::Mat red = channels[2];
  cv::Mat image_cmyk;
  rgbToCmyk(image_crop2, &image_cmyk);
  cv::Mat channels_cmyk[4];
  cv::split(image_cmyk, channels_cmyk);
  cv::Mat yellow = channels_cmyk[2];
  cv::normalize(yellow, yellow, 255, 0, CV_MINMAX);
  yellow.convertTo(yellow, CV_8U);

  bilateralFilter->execute(yellow, &yellow);
  // binarización de la imagen
  cv::Mat yellow_binary;
  cv::threshold(yellow, yellow_binary, 100, 255, cv::THRESH_BINARY);
  cv::Mat red_binary;
  cv::threshold(red, red_binary, 190, 255, cv::THRESH_BINARY);

  //cv::Mat image_gray;
  //cvtColor(image, image_gray, CV_RGB2GRAY);

  // libero memoria
  image_cmyk.release();
  for (int i = 0; i < 4; i++)
    channels_cmyk[i].release();
  for (int i = 0; i < 3; i++)
    channels[i].release();

  I3D::ImgProcessingList imgprolist{ 
    dilate
  };

  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;

  imgprolist.execute(yellow_binary, &yellow_binary);
  cv::Mat yellow_area;
  cv::bitwise_and(yellow, yellow, yellow_area, yellow_binary); 
  cv::findContours( yellow_binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

  std::vector<cv::Point> approx;
  for (size_t i = 0; i < contours.size(); i++) {
    approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
    if ( approx.size() == 3 ) {
      //if (fabs(contourArea(Mat(approx))) > 50 &&
      //isContourConvex(Mat(approx))) {
        cv::line(image_crop2, approx[0],approx[1],cv::Scalar(255,0,0));
        cv::line(image_crop2, approx[1],approx[2],cv::Scalar(255,0,0));
        cv::line(image_crop2, approx[2],approx[0],cv::Scalar(255,0,0));
      //}
    }
  //  //if( approx.size() == 4 &&
  //  //                fabs(contourArea(Mat(approx))) > 1000 &&
  //  //                isContourConvex(Mat(approx)) ) {
  //  //}
  }


  // Busqueda baliza
  //imgprolist.clear();
  //imgprolist.add(gaussianBlur);
  //imgprolist.execute(red, &red);
  medianBlur(red, red, 5);
  std::vector<Vec3f> circles;
  HoughCircles(red, circles, HOUGH_GRADIENT, 2, red.rows/4, 200, 100, 25, 100 );
  for( size_t i = 0; i < circles.size(); i++ ) {
    cv::Point center(I3D_ROUND_TO_INT(circles[i][0]), I3D_ROUND_TO_INT(circles[i][1]));
    int radius = I3D_ROUND_TO_INT(circles[i][2]);
    circle( image_crop1, center, 3, Scalar(0,255,0), -1, 8, 0 );
    circle( image_crop1, center, radius, Scalar(0,0,255), 3, 8, 0 );
  }

  return 0;
}


*/