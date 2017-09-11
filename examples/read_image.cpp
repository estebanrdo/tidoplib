#include <stdio.h>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "img/imgio.h"

#include "geometric_entities/point.h"
#include "transform.h" 

#include "experimental/experimental.h"


#include "opencv2/core/core.hpp"

using namespace I3D;
using namespace I3D::geometry;

#ifdef VISUAL_LEAK_DETECTOR
#include <vld.h>
#endif

/*!
 * read_image: 
 *
 *
 * uso:
 */
int main(int argc, char** argv)
{

  //HANDLE hStdout;
  //CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
  // hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 

  //  if (! GetConsoleScreenBufferInfo(hStdout, &csbiInfo)) 
  //  {
  //      MessageBox(NULL, TEXT("GetConsoleScreenBufferInfo"), 
  //          TEXT("Console Error"), MB_OK); 
  //      return 1;
  //  }

  char name[I3D_MAX_FNAME];
  getFileName(getRunfile(), name, I3D_MAX_FNAME);

  CmdParser cmdParser(name, "");
  cmdParser.addParameter("img", "Imagen");
  cmdParser.addParameter("img_out", "Imagen de salida");
  CmdParser::Status status = cmdParser.parse(argc, argv);
  if (status == CmdParser::Status::PARSE_ERROR ) {
    //exit(EXIT_FAILURE);
    return 1;
  } else if (status == CmdParser::Status::PARSE_HELP) {
    //exit(EXIT_SUCCESS);
    return 0;
  }

  std::string img = cmdParser.getValue<Path>("img").toString();
  std::string img_out = cmdParser.getValue<Path>("img_out").toString();


  // Fichero de log
  Log &log = Log::getInstance();
  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  if (changeFileExtension(getRunfile(), "log", logfile, I3D_MAX_PATH) == 0) {
    log.setLogFile(logfile);
  }
  log.setLogLevel(MessageLevel::MSG_VERBOSE);

  // Consola
  Console console;
  console.setTitle(name);
  console.setLogLevel(MessageLevel::MSG_VERBOSE);
  console.setConsoleUnicode();
  //console.setFontBold(true);
  console.setFontHeight(24);
  
  //Configuración de mensajes
  MessageManager &msg_h = MessageManager::getInstance();
  msg_h.addListener(&log);
  msg_h.addListener(&console);
  
  msgInfo("prueba");


  ////CrsTransform<Point3D> crs("EPSG:25830", "EPSG:4326");
  ////Point3D pt_utm(350000., 4800000., 0.);
  ////Point3D pt_geo;
  ////crs.transform(pt_utm, &pt_geo);

  ////RasterGraphics image;
  ////Helmert2D<PointI> trf;
  ////cv::Mat mat_out;
  ////double scale = 4.;   // 25% de escala de la resolución original 

  ////if (image.open(img.c_str()) == RasterGraphics::Status::OPEN_FAIL) exit(EXIT_FAILURE);

  ////// saveAs() tiene que crear una copia con la libreria adecuada si tienen modo de copia
  ////image.createCopy(img_out.c_str());

  ////try {    
  ////  WindowI w(PointI(-100, -100), PointI(3900, 3900)); // Ventana de 4000x4000                          
  ////  image.read(&mat_out, w, scale, &trf);

  ////} catch (I3D::Exception &e) {
  ////  printError(e.what());
  ////  exit(EXIT_FAILURE);
  ////}

  ////// Imagen que se guarda
  ////RasterGraphics imageOut;
  ////imageOut.open(img_out.c_str(), Mode::Create);
  ////imageOut.create(1000, 1000, 3, 0);
  ////imageOut.write(mat_out, &trf);


  ////GeoRasterGraphics geoRaster;
  ////geoRaster.open("D://Desarrollo//datos//mtn25_epsg25830_0033-3.tif", Mode::Read);
  ////WindowD w_g(PointD(377386.335, 4801010.256), PointD(380324.165, 4803176.748));
  ////geoRaster.read(&mat_out, w_g, scale);

  //TODO: Ojo con salir con exit!!! No llama a las destructoras y con VLD me estaba
  //dando fugas de memoria
  //exit(EXIT_SUCCESS); 
  return 0;
}
