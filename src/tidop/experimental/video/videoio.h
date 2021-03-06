/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#ifndef TL_VIDEO_IO_H
#define TL_VIDEO_IO_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include <memory>
#include <array>
#include <mutex>

#include "tidop/core/utils.h" 
#include "tidop/graphic/color.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/window.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace tl
{
  

class TL_EXPORT VrtVideo
  : public File
{

public:

  //TODO: No puedo llamarlo Status por File...
  /*!
   * \brief Estado del video
   */
  enum class VideoStatus
  {
    start,      /*!< Inicio */
    running,    /*!< Video ejecutandose */
    pause,      /*!< Video en pausa */
    stopped,    /*!< Video detenido */
    finalized   /*!< Video finalizado */
  };

  /*!
   * \brief Tipos salto de frames
   */
  enum class Skip
  {
    not_skip,            /*!< Frame de video uno a uno. */
    skip_frames,         /*!< Carga un frame de cada n. */
    skip_milliseconds    /*!< Carga un frame cada n milisegundos. */
  };

  /*!
   * \brief Tipo de visualización del fotograma de video.
   */
  enum class View
  {
    original_frame,   /*!< Frame del tamaño original del video. */
    resize_frame,     /*!< Redimensiona el frame de video. */
    crop_frame        /*!< Recorta el frame de video. */
  };

  /*!
   * \brief Eventos
   */
  class Listener
  {
  protected:

    VrtVideo *mVideo;

  public:

    Listener() {}

    ~Listener() {}

    virtual void onFinish() = 0;
    virtual void onInitialize() = 0;
    virtual void onPause() = 0;
    virtual void onPositionChange(double position) = 0;
    virtual void onRead(cv::Mat &frame) = 0;
    virtual void onResume() = 0;
    virtual void onShow(cv::Mat &frame) = 0;
    virtual void onStop() = 0;

    void setVideoStream(VrtVideo *video)
    {
      mVideo = video;
    }

  };

protected:

  /*!
   * \brief Resolución horizontal del video
   */
  int mWidth;

  /*!
   * \brief Resolución vertical del video
   */
  int mHeight;

  /*!
   * \brief Número de bandas o canales del video
   */
  int mBands;

  /*!
   * \brief Tamaño de los frames del video despues de recortar
   */
   //cv::Size mFrameSize;

   /*!
    * \brief Región de recorte del video
    */
    //cv::Rect rframe;
  WindowI mWindow;

  /*!
   * \brief Mantiene la proporción al redimensionar una imagen
   */
  bool bKeepRatio;

  /*!
   * \brief Tipo de resolución del frame de video.
   * \see res_frame
   */
  VrtVideo::View mViewMode;

  /*!
   * \brief Tipo de salto de frames aplicado
   * \see skip_video
   */
  VrtVideo::Skip mSkipMode;

  /*!
   * \brief Salto de frames o de milisegundos con que se cargan los frames
   */
  int mSkip;

  double mFramesPerSecond;

  /*!
   * \brief vs Estado de ejecución del video
   * \see video_status
   */
  VrtVideo::VideoStatus mStatus;

  std::list<Listener *> mEvents;

  cv::Mat mFrame;

  int mCodec;

  bool bGray;

public:

  /*!
   * \brief Constructora
   */
  VrtVideo();

  VrtVideo(const std::string &file, Mode mode = Mode::read);

  /*!
   * \brief Constructor de copia
   */
  VrtVideo(const VrtVideo &video);

  /*!
   * \brief Destructora
   */
  virtual ~VrtVideo();

  /*!
   *
   */
  void addListener(Listener *listener);

  /*!
   * \brief Crea un video
   * \return
   */
  virtual Status create(int width, int height, int codec, int fps, bool color = true) = 0;

  /*!
   * \brief Devuelve la resolución horizontal del video
   * \return Número de filas de la imagen
   */
  virtual int width() const;

  /*!
   * \brief Devuelve la resolución Vertical del video
   * \return Número de columnas de la imagen
   */
  virtual int height() const;

  /*!
   * \brief Devuelve el número de canales o bandas de la imagen
   * \return Número de bandas de la imagen
   */
  virtual int bands() const;

  /*!
   * \brief Frames por segundo del video
   * \return Frames por segundo
   */
  virtual double framePerSeconds() const;

  /*!
   * \brief Frame de video actual
   * \return
   */
  virtual double currentFrame() const = 0;

  /*!
   * \brief Número de frames del video
   * \return
   */
  virtual double frameCount() const = 0;

  /*!
   * \brief Comprueba si el video está abierto
   * \return Verdadero si el video está abierto
   */
  virtual bool isOpened() const = 0;

  virtual void setFramePerSeconds(double fps) = 0;

  /*!
   * \brief Arranca el video
   *
   * Ejecuta el video desde el principio. Se puede interactuar con el video
   * mediante las teclas:
   * - Tecla esc: Terminar la ejecución del video.
   * - Tecla +: Aumenta el salto entre frames.
   * - Tecla -: Disminuye el salto entre frames.
   * - Barra espaciadora: Pausa/reanuda la ejecución del video.
   */
  virtual void run();

  /*!
 * \brief lee el siguiente frame
 */
  virtual void read() = 0;

  /*!
   * \brief Detiene la ejecución del video
   */
  virtual void stop() = 0;

  /*!
   * \brief Pausa la ejecución del video
   */
  virtual void pause() = 0;

  /*!
   * \brief Continua la ejecución del video
   */
  virtual void resume() = 0;

  /*!
   * \brief Finaliza la ejecución del video
   */
  virtual void finish() = 0;

protected:

  void onFinish();

  void onInitialize();

  /*!
   * \brief Rausa el video
   */
  void onPause();

  /*!
   * \brief
   */
  void onPositionChange(double position);

  /*!
   * \brief
   */
  void onRead();

  /*!
   * \brief Continua la ejecución del video
   */
  void onResume();

  /*!
   * \brief
   */
  void onShow();

  /*!
   * \brief Detiene la ejecución del video
   */
  void onStop();

};


/*!
 * \brief Ficheros de video que son gestionados por OpenCV
 */
class TL_EXPORT VideoOpenCV 
  : public VrtVideo
{

private:

  /*!
   * \brief Captura de video
   */
  cv::VideoCapture mVideoCapture;

  /*!
   * \brief Escritura de video
   */
  cv::VideoWriter mVideoWriter;

public:

  /*!
   * \brief Constructora
   */
  VideoOpenCV();

  VideoOpenCV(const std::string &file, Mode mode = Mode::read);
  /*!
   * \brief Constructor de copia
   * \param gdalRaster Objeto VideoOpenCV
   */
  VideoOpenCV(const VideoOpenCV &video);

  /*!
   * \brief Destructora
   */
  virtual ~VideoOpenCV();

  /*!
   * \brief Cierra el video
   */
  void close() override;

  /*!
   * \brief Abre un video
   * \param[in] file Nombre del fichero
   * \param[in] mode Modo de apertura
   * \return
   * \see Mode
   */
  Status open(const std::string &file, Mode mode = Mode::read, FileOptions *options = nullptr) override;

  /*!
   * \brief Crea una imagen
   * \param[in] width Ancho de la imagen
   * \param[in] height Alto de la imagen
   * \param[in] codec Codec de video usado
   * \param[in] fps Número de fotogramas por segundo
   * \param[in] color Si es true el video se genera en color. En caso contrario en b/n
   * \return 
   */
  Status create(int width, int height, int codec, int fps, bool color = true) override;

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  Status createCopy(const std::string &fileOut) override;

  /*!
   * \brief Frame de video actual
   * \return
   */
  virtual double currentFrame() const override;

  /*!
   * \brief Número de frames del video
   * \return
   */
  virtual double frameCount() const override;

  /*!
   * \brief Comprueba si el video está abierto
   * \return Verdadero si el video está abierto
   */
  virtual bool isOpened() const;

  virtual void setFramePerSeconds(double fps) override;

  /*!
   * \brief Arranca el video
   *
   * Ejecuta el video desde el principio. Se puede interactuar con el video
   * mediante las teclas:
   * - Tecla esc: Terminar la ejecución del video.
   * - Tecla +: Aumenta el salto entre frames.
   * - Tecla -: Disminuye el salto entre frames.
   * - Barra espaciadora: Pausa/reanuda la ejecución del video.
   */
  //virtual void run() override;

    /*!
   * \brief lee el siguiente frame
   */
  virtual void read() override;

  /*!
   * \brief Detiene la ejecución del video
   */
  virtual void stop() override;

  /*!
   * \brief Pausa la ejecución del video
   */
  virtual void pause() override;

  /*!
   * \brief Continua la ejecución del video
   */
  virtual void resume() override;

  /*!
   * \brief Finaliza la ejecución del video
   */
  virtual void finish() override;

protected:

  virtual void update();

  virtual bool _read();

};



// Clase para la lectura de un listado de imagenes en un archivo. Para reemplazar a ImagesStream

class TL_EXPORT ImagePresentation 
  : public VrtVideo
{
public:

  double mCurrentFrame;

  bool bIsOpened;

  std::vector<std::string> mImages;

public:

  ImagePresentation();
  ImagePresentation(const std::string &file, Mode mode = Mode::read);
  ImagePresentation(const ImagePresentation &imagePresentation);
  ~ImagePresentation();

  /*!
   * \brief Cierra el video
   */
  void close() override;

  /*!
   * \brief Abre un video
   * \param[in] file Nombre del fichero
   * \param[in] mode Modo de apertura
   * \return
   * \see Mode
   */
  Status open(const std::string &file, Mode mode = Mode::update, FileOptions *options = nullptr) override;

  /*!
   * \brief Crea una imagen
   * \param[in] width Ancho de la imagen
   * \param[in] height Alto de la imagen
   * \param[in] codec Codec de video usado
   * \param[in] fps Número de fotogramas por segundo
   * \param[in] color Si es true el video se genera en color. En caso contrario en b/n
   * \return 
   */
  Status create(int width, int height, int codec, int fps, bool color = true) override;

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  Status createCopy(const std::string &fileOut) override;

  /*!
   * \brief Frame de video actual
   * \return
   */
  virtual double currentFrame() const override;

  /*!
   * \brief Número de frames del video
   * \return
   */
  virtual double frameCount() const override;

  /*!
   * \brief Comprueba si el video está abierto
   * \return Verdadero si el video está abierto
   */
  virtual bool isOpened() const;

  virtual void setFramePerSeconds(double fps) override;

  /*!
   * \brief Arranca el video
   *
   * Ejecuta el video desde el principio. Se puede interactuar con el video
   * mediante las teclas:
   * - Tecla esc: Terminar la ejecución del video.
   * - Tecla +: Aumenta el salto entre frames.
   * - Tecla -: Disminuye el salto entre frames.
   * - Barra espaciadora: Pausa/reanuda la ejecución del video.
   */
  //virtual void run() override;

    /*!
   * \brief lee el siguiente frame
   */
  virtual void read() override;

  /*!
   * \brief Detiene la ejecución del video
   */
  virtual void stop() override;

  /*!
   * \brief Pausa la ejecución del video
   */
  virtual void pause() override;

  /*!
   * \brief Continua la ejecución del video
   */
  virtual void resume() override;

  /*!
   * \brief Finaliza la ejecución del video
   */
  virtual void finish() override;

protected:

  virtual void update();

  virtual bool _read();
};


} // End namespace tl

#endif // HAVE_OPENCV

#endif // TL_VIDEO_IO_H
