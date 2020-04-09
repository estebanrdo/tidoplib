#ifndef TL_IMAGE_WRITER_H
#define TL_IMAGE_WRITER_H

#include "config_tl.h"

#include <string>
#include <memory>

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // HAVE_OPENCV

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/transform.h"
#include "tidop/img/img.h"

namespace tl
{


/*!
 * \brief Clase para la escritura de distintos formatos imagen
 */
class TL_EXPORT ImageWriter
{

public:

  ImageWriter(const std::string &fileName);
  virtual ~ImageWriter() = default;

  /*!
   * \brief Abre el fichero
   */
  virtual void open() = 0;

  /*!
   * \brief Comprueba si el fichero se ha cargado correctamente
   */
  virtual bool isOpen() = 0;

  /*!
   * \brief Cierra el fichero
   */
  virtual void close() = 0;


  /*!
   * \brief Crea una imagen
   * \param[in] rows N�mero de filas de la imagen
   * \param[in] cols N�mero de columnas de la imagen
   * \param[in] bands N�mero de bandas de la imagen
   * \param[in] type Tipo de dato
   * \see DataType
   */
  virtual void create(int rows, int cols, int bands, DataType type) = 0;

#ifdef HAVE_OPENCV

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe. Por defecto toda la imagen
   */
  virtual void write(const cv::Mat &image, const geometry::WindowI &w = geometry::WindowI()) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformaci�n entre el bloque y la imagen. Si es nula no se aplica transformaci�n
   */
  virtual void write(const cv::Mat &image, const Helmert2D<geometry::PointI> *trf = nullptr) = 0;

#endif

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe
   */
  virtual void write(const unsigned char *buff, const geometry::WindowI &w) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformaci�n entre el bloque y la imagen. Si es nula no se aplica transformaci�n
   */
  //virtual void write(const unsigned char *buff, const Helmert2D<geometry::PointI> *trf = nullptr) = 0;

  /*!
   * \brief Devuelve el n�mero de filas de la imagen
   * \return N�mero de filas de la imagen
   */
  virtual int rows() const = 0;

  /*!
   * \brief Devuelve el n�mero de columnas de la imagen
   * \return N�mero de columnas de la imagen
   */
  virtual int cols() const = 0;

  /*!
   * \brief Devuelve el n�mero de canales o bandas de la imagen
   * \return N�mero de bandas de la imagen
   */
  virtual int channels() const = 0;

  /*!
   * \brief Devuelve el tipo de dato
   * \return 
   */
  virtual DataType dataType() const = 0;

  /*!
   * \brief Devuelve la profundidad de color o bits por pixel de una imagen
   * \return Profundidad de color
   */
  virtual int depth() const = 0;

protected:

  std::string mFileName;

};


/*!
 * \brief Clase factor�a para la escritura de diferentes formatos de imagen
 */
class TL_EXPORT ImageWriterFactory
{
public:

private:

  ImageWriterFactory() {}

public:

  static std::unique_ptr<ImageWriter> createWriter(const std::string &fileName);
};


} // End namespace tl



#endif // TL_IMAGE_WRITER_H
