#ifndef I3D_UTILS_H
#define I3D_UTILS_H

#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <functional>
#include <memory>

#include "core/config.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // HAVE_OPENCV

#include "core/defs.h"

namespace I3D
{

/*!
 * \defgroup utilities Utilidades
 *
 * Utilidades de proposito general como manipulación de cadenas, información de la
 * aplicación etc
 * \{
 */

/* ---------------------------------------------------------------------------------- */
/*                           Información de la aplicación                             */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Obtiene el path (directorio + nombre + extensión) de la aplicación
 * que esta corriendo
 * \return path de la aplicación
 */
I3D_EXPORT const char *getRunfile();


//bool getAppVersion(char *libName, std::string *companyName, std::string *productName,
//                   std::string *fileVersion, std::string *copyright, std::string *fileDescription);

/*!
 * \brief Comprueba si existe un directorio
 * \param[in] path Ruta del directorio
 * \return true si existe.
 */
I3D_EXPORT bool isDirectory(const char *path);

/*!
 * \brief Crea un directorio
 * \param[in] path Ruta del directorio
 * \return Error = -1, creado = 0 y existente = 1
 */
I3D_EXPORT int createDir(const char *path);

/*!
 * \brief Crea un directorio
 * \param[in] path Ruta del directorio
 * \param[in] confirm Pide confirmación para borrar el archivo
 * \return Error
 */
I3D_EXPORT int deleteDir(const char *path, bool confirm = false);

/*!
 * \brief Optiene el directorio de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] dir Directorio del archivo
 * \param[in] size Tamaño de dir
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char dir[I3D_MAX_DIR];
 * if (getFileDir("c:\temp\file.txt", dir, I3D_MAX_DIR) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileDir(const char *path, char *dir, int size);

/*!
 * \brief Optiene la unidad de disco de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] drive Unidad de disco
 * \param[in] size Tamaño de drive
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char drive[I3D_MAX_DRIVE];
 * if (getFileDrive("c:\temp\file.txt", drive, I3D_MAX_DRIVE) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileDrive(const char *path, char *drive, int size);

/*!
 * \brief Optiene la extensión de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] ext Extensión del archivo
 * \param[in] size Tamaño de ext
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char ext[I3D_MAX_EXT];
 * if (getFileExtension("c:\temp\file.txt", ext, I3D_MAX_EXT) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileExtension(const char *path, char *ext, int size);

/*!
 * \brief Optiene el nombre de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] name Nombre del archivo
 * \param[in] size Tamaño de name
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char name[I3D_MAX_FNAME];
 * if (getFileName("c:\temp\file.txt", name, I3D_MAX_FNAME) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileName(const char *path, char *name, int size);

/*!
 * \brief Optiene el directorio de un archivo incluyendo la letra de la unidad
 * \param[in] path Ruta del archivo
 * \param[out] driveDir Unidad de disco y directorio del archivo
 * \param[in] size Tamaño de driveDir
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char driveDir[I3D_MAX_PATH];
 * if (getFileDriveDir("c:\temp\file.txt", driveDir, I3D_MAX_PATH) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileDriveDir(const char *path, char *driveDir, int size);

/*!
 * \brief Cambia el nombre de un archivo
 * \param[in] path Ruta del archivo
 * \param[in] newName Nuevo nombre
 * \param[out] pathOut Ruta del archivo modificada
 * \param[in] size Tamaño de pathOut
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char new_name[I3D_MAX_PATH];
 * if (changeFileName("c:\temp\old_name.txt", "new_name", new_name, I3D_MAX_PATH) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int changeFileName(const char *path, const char *newName, char *pathOut, int size);

/*!
 * \brief Cambia la extensión de un archivo
 * \param[in] path Ruta del archivo
 * \param[in] newExt Nueva extensión
 * \param[out] pathOut Ruta del archivo modificada
 * \param[in] size Tamaño de pathOut
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char logfile[I3D_MAX_PATH];
 * if (changeFileExtension(getRunfile(), "log", logfile, I3D_MAX_PATH) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int changeFileExtension(const char *path, const char *newExt, char *pathOut, int size);

/*!
 * \brief Cambia el nombre y la extensión de un archivo
 * \param[in] path Ruta del archivo
 * \param[in] newNameExt Nuevo nombre incluyendo la extensión
 * \param[out] pathOut Ruta del archivo modificada
 * \param[in] size Tamaño de pathOut
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char logfile[I3D_MAX_FNAME + I3D_MAX_EXT];
 * if (changeFileNameAndExtension(getRunfile(), "error.log", logfile, I3D_MAX_FNAME + I3D_MAX_EXT) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int changeFileNameAndExtension(const char *path, const char *newNameExt, char *pathOut, int size);

/*!
 * \brief The Path class
 */
class I3D_EXPORT Path
{
private:

 /*!
  * \brief mPos
  */
  int mPos;

  /*!
   * \brief mPath
   */
  std::vector<std::string> mPath;

  /*!
   * \brief Directorio o fichero
   */
  //bool bDir;

#if defined WIN32

  /*!
   * \brief Unidad de disco
   */
  std::string mDrive;

#endif

public:

  /*!
   * \brief Constructor por defecto
   */
  Path() : mPos(0), mPath(0)
  {
  }

  /*!
   * \brief Constructor
   * \param path Ruta
   */
  Path(const std::string &path) : mPos(0), mPath(0)
  {
    parse(path);
  }

  /*!
   * \brief Constructor de copia
   * \param path Ruta
   */
  Path(const Path &path) : mPos(path.mPos), mPath(path.mPath) { }

  /*!
   * \brief Destructora
   */
  ~Path()
  {
  }

  /*!
   * \brief Parsea una cadena
   * \param path
   */
  void parse(const std::string &path);

  /*!
   * \brief getDrive
   * \return
   */
  std::string getDrive();

  /*!
   * \brief up
   */
  void up();

  /*!
   * \brief down
   */
  void down();
  
  /*!
   * \brief currentPath
   * \return
   */
  std::vector<std::string> currentPath();

  /*!
   * \brief convierte el path a una cadena
   * \return
   */
  std::string toString();
};


class Process
{
public:

  Process() {}
  ~Process() {}

  virtual int run() = 0;

  // pause() ??
  // stop() ??

private:

};


class CmdProcess : public Process
{
protected:

  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  std::string mCmd;

public:

  CmdProcess(const std::string &cmd);
  ~CmdProcess();

  virtual int run() override;

private:

};


class BatchProcess
{
protected:

  /*!
   * \brief Lista de procesos
   */
  std::list<std::shared_ptr<Process>> mProcessList;

public:  

  /*!
   * \brief Constructora por defecto
   */
  BatchProcess();

  /*!
   * \brief Constructor de copia
   * \param[in] batchProcess Procesos que se copia
   */
  BatchProcess(const BatchProcess &batchProcess) : mProcessList(batchProcess.mProcessList) {}

  /*!
   * \brief Constructor de lista
   * \param[in] Listado de procesos
   */
  BatchProcess(std::initializer_list<std::shared_ptr<Process>> procList) : mProcessList(procList) {}

  /*!
   * \brief Destructora
   */
  ~BatchProcess();

  /*!
   * \brief Añade un nuevo proceso a la lista
   * \param[in] process Proceso que se añade
   */
  void add(const std::shared_ptr<Process> &process);

  /*!
   * \brief Limpia la lista de procesos
   */
  void clear();

  /*!
   * \brief Corre los procesos
   */
  int run();
};



/* ---------------------------------------------------------------------------------- */
/*                             Operaciones con cadenas                                */
/* ---------------------------------------------------------------------------------- */

/*! \defgroup stringOper Operaciones con cadenas
 *  
 * \{
 */

/*!
 * \brief Separa una cadena en un array de enteros
 * \param[in] cad Cadena de texto que contiene una lista de numeros
 * \param[out] vOut Vector con los números extraidos
 * \param[in] chs Caracter separador de la cadena. Si se omite toma por defecto ","
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * std::string aux = "1102,3654";
 * std::vector<int> coord;
 * if ( splitToNumbers(aux, coord) == 0 ){
 *   ...
 * }
 * \endcode
 */
I3D_EXPORT int splitToNumbers(const std::string &cad, std::vector<int> &vOut, const char *chs = ",");

/*!
 * \brief Separa una cadena en un array de dobles
 * \param[in] cad Cadena de texto que contiene una lista de numeros
 * \param[out] vOut Vector con los números extraidos
 * \param[in] chs Caracter separador de la cadena. Si se omite toma por defecto ","
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * std::string aux = "1102.52,3654.95";
 * std::vector<double> coord;
 * if ( splitToNumbers(aux, coord) == 0 ){
 *   ...
 * }
 * \endcode
 */
I3D_EXPORT int splitToNumbers(const std::string &cad, std::vector<double> &vOut, const  char *chs = ",");

/*!
 * \brief Reemplaza una cadena por otra en un texto.
 * \param[in] str Cadena original
 * \param[in] str_old Cadena a remplazar
 * \param[out] str_new Nueva cadena
 *
 * <h4>Ejemplo</h4>
 * \code
 * std::string str = "Hola mundo";
 * replaceString(str, " ", "_");
 * \endcode
 */
I3D_EXPORT void replaceString(std::string *str, const std::string &str_old, const std::string &str_new);


/*!
 * \brief Separa una cadena
 *
 * \param[in] in Cadena de entrada
 * \param[out] out vector con las cadenas resultantes
 * \param[in] chs cadena de separación. Si se omite toma por defecto ","
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char *in = "cadena1,cadena2";
 * std::vector<std::string> out;
 *
 * if ( split(in, out, ",") == 0 ){
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int split(const std::string &in, std::vector<std::string> &out, const char *chs = ",");

/*! \} */ // end of stringOper

/*!
 * \brief Convierte un número a una cadena de texto
 * \param[in] number Numero
 * \return Cadena de texto
 */
template <typename T>
I3D_EXPORT inline std::string numberToString(T number)
{
  std::ostringstream ss;
  ss << number;
  return ss.str();
}



/*!
 * \brief Convierte una cadena de texto a un número
 * \param[in] text Texto
 * \return número
 */
template <typename T>
I3D_EXPORT inline T stringToNumber(const std::string &text)
{
  T number;
  return (std::istringstream(text) >> number) ? number : 0;
}

enum class Base : int8_t
{
  OCTAL       =  8,
  DECIMAL     = 10,
  HEXADECIMAL = 16
};

/*!
 * \brief Convierte una cadena a un número entero.
 *
 * La cadena puede tener un número en base octal, decimal o hexadecimal
 * \param text Cadena de texto
 * \param base Base en la cual esta el número
 * \return Número
 * \see Base
 */
I3D_EXPORT int stringToInteger(const std::string &text, I3D::Base base = I3D::Base::DECIMAL);

/* ---------------------------------------------------------------------------------- */
/*                              Operaciones con vectores                              */
/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_OPENCV

/*!
 * \brief Convierte una matriz de OpenCV en un vector
 * \param[in] m Matriz de entrada
 * \param[out] av Vector de salida
 */
template<typename T>
I3D_EXPORT inline void cvMatToVector(const cv::Mat &m, std::vector<T> *av)
{
  av->resize(m.rows*m.cols);
  if (m.isContinuous()) {
    av->assign((T*)m.datastart, (T*)m.dataend);
  } else {
    for (int i = 0; i < m.rows; ++i) {
      av->insert(av->end(), (T*)m.ptr<uchar>(i), (T*)m.ptr<uchar>(i)+m.cols);
    }
  }
}

#endif // HAVE_OPENCV

/*!
 * \brief Ordena un vector de menor a mayor
 * \param[in] v Vector
 */
template<typename T>
I3D_EXPORT inline void sortVector(std::vector<T> *v)
{
  std::sort(v->begin(), v->end());
}

/*!
 * \brief Ordena un vector de mayor a menor
 * \param[in] v Vector
 */
template<typename T>
I3D_EXPORT inline void sortVectorInv(std::vector<T> *v)
{
  std::sort(v->rbegin(),v->rend());
}

/*!
 * \brief Determinar el número de elementos iguales a un número.
 * Sobrecarga del operador == para determinar el número de elementos de un
 * vector que son iguales al valor pasado como parámetro.
 * \param[in] v Vector
 * \param[in] t Valor
 * \return Número de elementos que cumplen la condición
 */
template<typename T>
I3D_EXPORT inline int operator==(const std::vector<T> &v, const T t)
{
  sortVector(v);
  std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> bounds;
  bounds = std::equal_range(v.begin(), v.end(), t);
  int ini = bounds.first - v.begin();
  int end = bounds.second - v.begin();
  return ( end - ini );
}

/*!
 * \brief Determina el número de elementos distintos a un número.
 * Sobrecarga del operador != para determinar el número de elementos de un
 * vector que son distintos al valor pasado como parámetro.
 * \param[in] v Vector
 * \param[in] t Valor
 * \return Número de elementos que cumplen la condición
 */
template<typename T>
I3D_EXPORT inline int operator!=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> bounds;
  bounds = std::equal_range(v.begin(), v.end(), t);
  int r1 = bounds.first - v.begin();
  int r2 = v.end() - bounds.second;
  return (r1 + r2 );
}

/*!
 * \brief operator >=
 * \param[in] v
 * \param[in] t
 * \return
 */
template<typename T>
I3D_EXPORT inline int operator>=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator upOrEq;
  upOrEq = std::lower_bound(v.begin(), v.end(), t);
  return (v.end() - upOrEq);
}

template<typename T>
I3D_EXPORT inline int operator<=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator lowOrEq;
  lowOrEq = std::upper_bound(v.begin(), v.end(), t);
  return (lowOrEq - v.begin());
}

template<typename T>
I3D_EXPORT inline int operator> (const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator up;
  up = std::upper_bound(v.begin(), v.end(), t);
  return (v.end() - up);
}

template<typename T>
I3D_EXPORT inline int operator< (const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator low;
  low = std::lower_bound(v.begin(), v.end(), t);
  return (low - v.begin());
}

/* ---------------------------------------------------------------------------------- */



/*!
 * \brief Ordena los indices de un vector de menor a mayor
 * Para un vector [10,20,15,5] devuelve [3,0,2,1]. El elemento mas
 * pequeño esta en la posición 3, el segundo en la posición 0, ...
 * \param[in] v Vector
 * \return Vector con los indices ordenados
 */
template <typename T>
I3D_EXPORT inline std::vector<int> sortIdx(const std::vector<T> &v)
{
  std::vector<int> idx(v.size());
  iota(idx.begin(), idx.end(), 0);

  sort(idx.begin(), idx.end(), [&v](int i1, int i2) {return v[i1] < v[i2]; });

  return idx;
}

#ifdef HAVE_OPENCV


/* ---------------------------------------------------------------------------------- */
/*                Utilidades de carga y guardado para OpenCV                          */
/* ---------------------------------------------------------------------------------- */

I3D_EXPORT void loadCameraParams(std::string &file, cv::Size &imageSize, cv::Mat &cameraMatrix, cv::Mat& distCoeffs);

I3D_EXPORT int loadBinMat(const char *file, cv::Mat *data);

I3D_EXPORT int saveBinMat(const char *file, cv::Mat &data);

#endif // HAVE_OPENCV


/* ---------------------------------------------------------------------------------- */
/*                         Concurrencia, hilos y multiproceso                         */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief número optimo de hilos
 */
I3D_EXPORT uint32_t getOptimalNumberOfThreads();

/*!
 * \brief Ejecuta una función en paralelo
 * \param[in] ini
 * \param[in] end
 * \param[in] f Función o lambda
 */
I3D_EXPORT void parallel_for(int ini, int end, std::function<void(int)> f);


/* ---------------------------------------------------------------------------------- */
/*                                Medición de tiempo                                  */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief tiempo actual
 *
 * <h4>Ejemplo</h4>
 * \code
 * uint64_t time_ini = getTickCount();
 * ...
 * double time = (getTickCount() - time_ini) / 1000.;
 * consolePrintInfo("Time %f", time);
 * \endcode
 */
I3D_EXPORT uint64_t getTickCount();

/*!
 * \brief Clase para medir tiempos.
 *
 * Su uso esta pensado para medir tiempos en depuración
 */
class I3D_EXPORT Chrono
{
public:

  /*!
   * \brief Posibles estados del cronómetro
   */
  enum class Status {
    START,      /*!< Iniciado */
    RUNNING,    /*!< Corriendo */
    PAUSE,      /*!< Pausado */
    STOPPED,    /*!< Detenido */
    FINALIZED   /*!< Finalizado */
  };

private:
  
  /*!
   * Tiempo de inicio en milisegundos
   */
  uint64_t mTimeIni;

  /*!
   * Tiempo acumulado en milisegundos
   */
  uint64_t mAccumulated;

  /*!
   * \brief Estado del cronómetro
   */
  Status mStatus;

  /*!
   * \brief Mensaje de información opcional
   */
  std::string mMessage;

  /*!
   * \brief Escribe mensajes en log y consola
   */
  bool bWriteMsg;

public:

  /*!
   * \brief Constructor
   */
  Chrono(const char *msg = "", bool writeMsg = true);

  /*!
   * \brief Destructora
   * En la destructora se llama al método stop si este no ha sido llamado
   */
  ~Chrono();

  /*!
   * \brief Pausa el cronometro
   */
  uint64_t pause();

  /*!
   * \brief Reinicio del cronometro
   */
  void reset();

  /*!
   * \brief Continua corriendo el cronometro cuando estaba pausado
   */
  void resume();

  /*!
   * \brief Arranca el cronometro
   */
  uint64_t run();

  /*!
   * \brief Detiene el cronometro
   */
  uint64_t stop();

  void setMessage(const char *msg);

private:

};


/* ---------------------------------------------------------------------------------- */
/*                               Plantillas html y xml                                */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase virtual para la sustitución de etiquetas en textos, ficheros html o xml.
 */
class VrtTemplate
{

private:
  
  /*!
   * \brief Texto con las etiquetas de reemplazo
   */
  std::string mText;

public:

  /*!
   * \brief Constructora por defecto
   */
  VrtTemplate();

  /*!
   * \brief Constructora
   */
  VrtTemplate(const char *file);

  /*!
   * \brief Destructora
   */
  virtual ~VrtTemplate();

  /*!
   * \brief Lee un fichero plantilla
   * \param[in] file Fichero plantilla
   * \return
   */
  virtual int read(const char *file) = 0;

  /*!
   * \brief Establece la plantilla
   * \param[in] templ Texto con la plantilla
   * \return
   */
  virtual int setTemplate(const char *templ) = 0;

  /*!
   * \brief Remplaza todas las ocurrencias de las etiquetas
   * de plantilla
   * \param[out] output Texto con las etiquetas sustituidas
   * \return
   */
  virtual int replace(std::string *output) = 0;

};



/* ---------------------------------------------------------------------------------- */


} // End namespace I3D

#endif // I3D_UTILS_H
