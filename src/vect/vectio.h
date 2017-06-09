#ifndef I3D_VECT_IO_H
#define I3D_VECT_IO_H

#include <memory>
#include <list>

#include "core/config.h"
#include "core/defs.h"
//#ifdef HAVE_OPENCV
//#include "opencv2/core/core.hpp"
//#endif // HAVE_OPENCV

#ifdef HAVE_GDAL
I3D_SUPPRESS_WARNINGS
#include "gdal_priv.h"
#include "cpl_conv.h"
I3D_DEFAULT_WARNINGS
#endif // HAVE_GDAL

namespace I3D
{

class VectorGraphics;

namespace graph
{
  class StylePen;
  class StyleBrush;
  class StyleSymbol;
  class StyleLabel;
  class GLayer;
  class GraphicEntity;
  class GPoint;
  class GLineString;
  class GPolygon;
  class GMultiPoint;
  class GMultiLineString;
  class GMultiPolygon;
  class GraphicStyle;
  class GData;
}

}

class OGRFeature;
class OGRPoint;
class OGRLineString;
class OGRPolygon;
class OGRMultiPoint;
class OGRMultiLineString;
class OGRMultiPolygon;
class OGRStyleMgr;
class OGRStylePen;
class OGRStyleBrush;
class OGRStyleSymbol;
class OGRStyleLabel;

namespace I3D
{

enum class Mode
{
  Read,
  Update,
  Create
};

enum class Status
{
  OPEN_OK,
  OPEN_FAIL,
  SAVE_OK,
  SUCCESS,
  FAILURE
};

class VrtVector
{

protected:

  /*!
   * \brief Nombre del fichero
   */
  std::string mName;

  /*!
   * \brief Número de capas
   */
  int mLayersCount;

public:

  VrtVector() : mName(""), mLayersCount(0) {}
  ~VrtVector() {}

  /*!
   * \brief Cierra el fichero imagen
   */
  virtual void close() = 0;
  virtual int open(const char *file, Mode mode = Mode::Read) = 0;
  virtual int create() = 0;
  virtual void read(VectorGraphics *vector) = 0;
  virtual void read(int id, graph::GLayer *layer) = 0;
  virtual void read(const char *name, graph::GLayer *layer) = 0;
  virtual int write(VectorGraphics *vector) = 0;
  int getLayersCount() const;

private:

};


#ifdef HAVE_GDAL

class GdalVector : public VrtVector
{

protected:
    
  /*!
   * \brief Dataset de GDAL
   */
  GDALDataset *pDataset;

  OGRFeature *pFeature;

  /*!
   * \brief Driver GDAL
   */
  GDALDriver *pDriver;

  const char *mDriverName;

public:

  GdalVector();
  virtual ~GdalVector() {}

  /*!
   * \brief Cierra el fichero vectorial
   */
  void close() override;

  /*!
   * \brief Abre un fichero vectorial
   * \param[in] file Nombre del fichero
   * \param[in] mode Modo de apertura
   * \return
   * \see Mode
   */
  int open(const char *file, Mode mode = Mode::Read) override;

  /*!
   * \brief Crea una fichero vectorial
   * \return
   */
  int create() override;

  void read(VectorGraphics *vector) override;
  void read(int id, graph::GLayer *layer) override;
  void read(const char *name, graph::GLayer *layer) override;
  int write(VectorGraphics *vector) override;

  /*!
   * \brief Devuelve el nombre del driver de GDAL correspondiente a una extensión de archivo
   * Si la extensión no se correspondo con un driver disponible devuelve nulo.
   * \param ext Extensión del archivo
   * \return Nombre del Driver de GDAL
   */
  static const char *getDriverFromExt(const char *ext);

private:

  void read(OGRLayer *pLayer, graph::GLayer *layer);
  void readEntity(OGRGeometry *ogrGeometry, std::shared_ptr<graph::GraphicEntity> gEntity);
  void readPoint(OGRPoint *ogrPoint, graph::GPoint *gPoint);
  void readLineString(OGRLineString *ogrLineString, graph::GLineString *gLineString);
  void readPolygon(OGRPolygon *ogrPolygon, graph::GPolygon *gPolygon);
  void readMultiPoint(OGRMultiPoint *ogrMultiPoint, graph::GMultiPoint *gMultiPoint);
  void readMultiLineString(OGRMultiLineString *ogrMultiLineString, graph::GMultiLineString *gMultiLineString);
  void readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon, graph::GMultiPolygon *gMultiPolygon);
  void readStyles(OGRStyleMgr *ogrStyle, graph::GraphicStyle *gStyle);
  void readStylePen(OGRStylePen *ogrStylePen, graph::GraphicStyle *gStyle);
  void readStyleBrush(OGRStyleBrush *ogrStyleBrush, graph::GraphicStyle *gStyle);
  void readStyleSymbol(OGRStyleSymbol *ogrStyleSymbol, graph::GraphicStyle *gStyle);
  void readStyleLabel(OGRStyleLabel *ogrStyleLabel, graph::GraphicStyle *gStyle);
  //void readData();
  void update();

};

#endif // HAVE_GDAL

/*!
 * \brief Clase contenedor y de gestión de gráficos vectoriales
 *
 * Esta clase permite crear, abrir y guardar un fichero vectorial.
 */
class VectorGraphics
{
protected:

  /*!
   * \brief Nombre del fichero
   */
  std::string mName;
  
  std::list<std::shared_ptr<graph::GLayer>> mLayers;

  std::unique_ptr<VrtVector> mVectorFormat;

public:
  VectorGraphics();
  ~VectorGraphics();

  /*!
   * \brief Cierra el archivo
   */
  void close();

  /*!
   * \brief Abre un archivo vectorial
   * \param file Nombre del fichero
   * \param mode Modo de apertura del fichero
   * \return Status
   * \see Mode
   */
  Status open(const char *file, Mode mode = Mode::Read);

  Status create();
  Status createCopy();
  Status read();
  Status write();
  int getLayersCount() const;

private:

  void update();
};







//
//class VectFile
//{
//public:
//  enum class Status
//  {
//    OPEN_OK,
//    OPEN_ERROR
//  };
//
//private:
//  std::string mFile;
//  GDALDataset  *poDataset;
//
//public:
//
//  VectFile(const char *file) : mFile(file) { }
//
//  ~VectFile();
//
//  Status open();
//  
//  bool read( );
//  bool write( );
//
//  WindowI getWindow();
//
//private:
//  const char* GetOGRDriverName(const char *ext ) {
//    const char *format;
//    if      ( strcmpi( ext, ".dxf" ) == 0 )  format = "DXF";
//    else if ( strcmpi( ext, ".dwg" ) == 0 )  format = "DWG";
//    else if ( strcmpi( ext, ".dgn" ) == 0 )  format = "DGN";
//    else if ( strcmpi( ext, ".shp" ) == 0 )  format = "ESRI Shapefile";
//    else if ( strcmpi( ext, ".gml" ) == 0 )  format = "GML";
//    else if ( strcmpi( ext, ".kml" ) == 0 )  format = "LIBKML";
//    else if ( strcmpi( ext, ".kmz" ) == 0 )  format = "LIBKML";
//    else if ( strcmpi( ext, ".json") == 0 )  format = "GeoJSON";
//    else if ( strcmpi( ext, ".osm" ) == 0 )  format = "OSM";
//    else                                     format = 0;
//    return( format );
//  }
//};

} // End namespace I3D



#endif // I3D_VECT_IO_H
