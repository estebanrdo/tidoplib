#ifndef I3D_TRANSFORM_H
#define I3D_TRANSFORM_H

#include <list>
#include <vector>
#include <algorithm>
#include <memory>
#include <exception>
#include <array>
#include <thread>

//#include <Eigen/SVD>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d.hpp"

#include "core/defs.h"
#include "core/messages.h"
#include "geometric_entities/entity.h"
#include "geometric_entities/segment.h"
#include "geometric_entities/bbox.h"

namespace I3D
{

/*! \defgroup trfGroup Transformaciones
 *  Transformaciones geométricas
 *  \{
 */

/*!
 * \brief Tipos de transformaciones
 */
enum class transform_type {
  DEFAULT,         /*!<   */
  TRANSLATE,       /*!< Desplazamiento. */
  ROTATION,        /*!< Giro. */
  HELMERT_2D,      /*!< Helmert 2D */
  AFIN,            /*!< Afin */
  PERSPECTIVE,     /*!< Perspectiva */
  PROJECTIVE,      /*!< Projectiva */
  HELMERT_3D,      /*!< Helmert 3D */
  POLYNOMIAL       /*!< Transformación polinómica*/
};

enum class transform_order {
  DIRECT,   /*!< Transformación directa. */
  INVERSE   /*!< Transformación inversa. */
};

/*!
 * \brief Aplica una traslación a un conjunto de segmentos
 * \param[in] lines_in
 * \param[out] lines_out
 * \param[in] dx
 * \param[in] dy
 * \deprecated{ Reemplazada por I3D::Translate::transform }
 */
I3D_DEPRECATED("I3D::Translate::transform(const std::vector<Segment<sub_type>> &in, std::vector<Segment<sub_type>> *out)")
I3D_EXPORT void translate(const std::vector<Line> &lines_in, std::vector<Line> *lines_out, int dx, int dy);

/*!
 * \brief Cálculo de la matriz de rotación
 * \param[in] omega Rotación respecto al eje X en radianes
 * \param[in] phi Rotación respecto al eje Y en radianes
 * \param[in] kappa Rotación respecto al eje Z en radianes
 * \param[out] R Matriz de rotación
 */
I3D_EXPORT void rotationMatrix(double omega, double phi, double kappa, std::array<std::array<double, 3>, 3> *R);

/*!
 * \brief Clase base para transformaciones
 */
template<typename T>
class I3D_EXPORT Transform
{

protected:

  /*!
   * \brief Tipo de transformación
   * \see transform_type
   */
  transform_type mTrfType;

  /*!
   * \brief Número mínimo de puntos necesario para la transformación
   */
  int mMinPoint;

  /*!
   * 2D o 3D
   */
  int mDimensions;

  typedef typename T::value_type sub_type;

public:

  /*!
   * \brief Constructor
   * \param[in] n_min Número mínimo de puntos necesario para la transformación
   */
  Transform(int n_min = 0, transform_type trfType = transform_type::DEFAULT)
    : mTrfType(trfType), mMinPoint(n_min) {}

  /*!
   * \brief Destructora
   */
  virtual ~Transform() {}

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[in] error Conjunto de puntos en el segundo de los sistemas
   * \return RMSE (Root Mean Square Error)
   */
  virtual double compute(const std::vector<T> &v1, const std::vector<T> &v2, std::vector<double> *error = NULL) = 0;

  /*!
   * \brief Determina si el numero de puntos son suficientes para calcular la transformación
   * \param[in] npoints Número de puntos para calcular la transformación
   * \return Verdadero si son puntos suficientes
   */
  bool isNumberOfPointsValid(int npoints) const { return npoints >= mMinPoint; }

  /*!
   * \brief Aplica la transformación a un conjunto de puntos
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  virtual void transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder = transform_order::DIRECT) const = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  virtual void transform(const T &in, T *out, transform_order trfOrder = transform_order::DIRECT) const = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  virtual T transform(const T &in, transform_order trfOrder = transform_order::DIRECT) const = 0;

  /*!
   * \brief Aplica la transformación a una entidad geométrica
   * \param[in] in Entidad de entrada
   * \param[out] out Entidad de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  //virtual void transformEntity(const Entity<sub_type> &in, Entity<sub_type> *out, transform_order trfOrder = transform_order::DIRECT) const = 0;

  /*!
   * \brief Número mínimo de puntos necesario para la transformación
   * \return Número mínimo de puntos
   */
  int minNumberOfPoints() { return mMinPoint; }

  /*!
   * \brief Tipo de transformación
   * \return Tipo de transformación
   * \see transform_type
   */
  transform_type getTransformType() { return mTrfType; }

  /*!
   * \brief root-mean-square error (Raiz cuadrada de error cuadratico medio)
   * \param pts1 Puntos en el sistema de entrada
   * \param pts2 Puntos en el sistema de salida
   * \param error Vector con los errores para cada punto
   * \return RMSE
   */
  double rootMeanSquareError(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error = NULL)
  {
    size_t n = pts1.size();
    std::vector<T> ptsOut(n);
    std::vector<double> err(n);
    double sumErr = 0.;

    //... Sería mejor añadirlo en el propio calculo de los parámetros?
    if (compute(pts1, pts2)) {
      for (size_t i = 0; i < n; i++) {
        transform(pts1[i], &ptsOut[i]);
        ptsOut[i] -= pts2[i];
        //err[i] = static_cast<double>(ptsOut[i].x * ptsOut[i].x + ptsOut[i].y * ptsOut[i].y);
        err[i] = static_cast<double>((ptsOut[i]).dot(ptsOut[i]));
        sumErr += err[i];
      }
      if (error) *error = err;
      return sqrt(sumErr/(mDimensions * (n - mMinPoint)));
    }
    return 0.;
  }

protected:

  /*!
   * \brief Formatea de forma adecuada el vector de salida
   *
   * Prepara el vector de puntos de salida redimensionandolo al 
   * tamaño adecuado y limpiando su contenido si ya tenia datos.
   * En caso de que se utilice el mismo vector de entrada y de salida no
   * hace nada.
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   */
  template<typename T2> 
  void formatVectorOut(const std::vector<T2> &in, std::vector<T2> *out) const {
    if (&in != out && in.size() != out->size()) {
      out->clear();
      out->resize(in.size());
    }
  }

  /*!
   * \brief root-mean-square error
   * \param pts1 Puntos en el sistema de entrada
   * \param pts2 Puntos en el sistema de salida
   * \param error Vector con los errores para cada punto
   * \return RMSE
   */
  double _rootMeanSquareError(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error = NULL)
  {
    size_t n = pts1.size();
    std::vector<T> ptsOut(n);
    std::vector<double> err(n);
    double sumErr = 0.;

    for (int i = 0; i < n; i++) {
      transform(pts1[i], &ptsOut[i]);
      ptsOut[i] -= pts2[i];
      //err[i] = static_cast<double>(ptsOut[i].x * ptsOut[i].x + ptsOut[i].y * ptsOut[i].y);
      err[i] = static_cast<double>(ptsOut[i].dot(ptsOut[i]));
      sumErr += err[i];
    }

    if (error) *error = err;

    return sqrt(sumErr/(mDimensions * (n - mMinPoint)));
  }
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Transformación multiple.
 *
 * Una transformación multiple permite agrupar varias transformaciones de 
 * forma que se ejecutan a la vez (consecutivamente).
 */
template<typename T>
class I3D_EXPORT TrfMultiple : public Transform<T>
{

private:

  /*!
   * \brief Lista de transformaciones
   */
  std::list<std::shared_ptr<Transform<T>>> mTransf;

  typedef typename T::value_type sub_type;

public:

  /*!
   * \brief Constructora
   */
  TrfMultiple() : Transform<T>(0) {}

  /*!
   * \brief Constructor de lista
   * \param[in] transfList listado de transformaciones
   */
  TrfMultiple(std::initializer_list<std::shared_ptr<Transform<T>>> transfList)
    : Transform<T>(), mTransf(transfList) {}

  /*!
   * \brief Destructora
   */
  ~TrfMultiple() {}

public:

  /*!
   * \brief Añade una nueva transformación a la lista
   * \param[in] trf Transformación que se añade
   */
  void add(std::shared_ptr<Transform<T>> trf) { mTransf.push_back(trf); }

  /*!
   * \brief Borra las transformaciones
   */
  void clear() { mTransf.clear(); }

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Aplica la transformación
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  void transform(const T &in, T *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &in, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica la transformación a una entidad geométrica
   * \param[in] in Entidad de entrada
   * \param[out] out Entidad de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  //virtual void transformEntity(const Entity<sub_type> &in, Entity<sub_type> *out, transform_order trfOrder = transform_order::DIRECT) const override;

};

template<typename T> inline
void TrfMultiple<T>::transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder) const
{
  *out = in;
  //... Controlar errores
  for (auto trf : mTransf) {
    trf->transform(*out, out, trfOrder);
  }
}

template<typename T> inline
void TrfMultiple<T>::transform(const T &in, T *out, transform_order trfOrder) const
{
  *out = in;
  for (auto trf : mTransf) {
    trf->transform(*out, out, trfOrder);
  }
}

template<typename T> inline
T TrfMultiple<T>::transform(const T &in, transform_order trfOrder) const
{
  T out = in;
  for (auto trf : mTransf) {
    out = trf->transform(out, trfOrder);
  }
  return out;
}

template<typename T> inline
double TrfMultiple<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error)
{
  printError("'compute' no esta soportado para TrfMultiple");
  I3D_COMPILER_WARNING("'compute' no esta soportado para TrfMultiple");
  return -1.;
}

//template<typename T> inline
//void TrfMultiple<T>::transformEntity(const Entity<sub_type> &in, Entity<sub_type> *out, transform_order trfOrder) const
//{
//  *out = in;
//  for (auto trf : mTransf) {
//    trf->transform(*out, out, trfOrder);
//  }
//}

/* ---------------------------------------------------------------------------------- */

/*! \defgroup trf2DGroup Transformaciones 2D
*  Transformaciones geométricas en el plano
*  \{
*/

template<typename T>
class I3D_EXPORT Transform2D : public Transform<T>
{
private:

  typedef typename T::value_type sub_type;

public:

  /*!
   * \brief Transform2D
   * \param n_min Número mínimo de puntos necesario para la transformación
   */
  Transform2D(int n_min = 0, transform_type trfType = transform_type::DEFAULT)
    : Transform<T>(n_min, trfType)
  {
    this->mDimensions = 2;
  }

  /*!
   * \brief Destructora
   */
  virtual ~Transform2D() {}

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return RMSE (Root Mean Square Error)
   */
  virtual double compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error = NULL) override = 0;

  /*!
   * \brief Aplica la transformación a un conjunto de puntos
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  virtual void transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder = transform_order::DIRECT) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  virtual void transform(const T &in, T *out, transform_order trfOrder = transform_order::DIRECT) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  virtual T transform(const T &in, transform_order trfOrder = transform_order::DIRECT) const override = 0;

  /*!
   * \brief Aplica la transformación a una entidad geométrica
   * \param[in] in Entidad de entrada
   * \param[out] out Entidad de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  //void transformEntity(const Entity<sub_type> &in, Entity<sub_type> *out, transform_order trfOrder = transform_order::DIRECT) const override;
};

//template<typename T> inline
//void Transform2D<T>::transformEntity(const Entity<sub_type> &in, Entity<sub_type> *out, transform_order trfOrder) const
//{
//  if (in.getType() == entity_type::WINDOW) {
//    Window<sub_type> *w = dynamic_cast<Window<sub_type> *>(out);
//    this->transform(dynamic_cast<const Window<sub_type> &>(in).pt1, &w->pt1, trfOrder);
//    this->transform(dynamic_cast<const Window<sub_type> &>(in).pt2, &w->pt2, trfOrder);
//  } else if ( in.getType() == entity_type::SEGMENT_2D) {
//    Segment<sub_type> *s = dynamic_cast<Segment<sub_type> *>(out);
//    this->transform(dynamic_cast<const Segment<sub_type> &>(in).pt1, &s->pt1, trfOrder);
//    this->transform(dynamic_cast<const Segment<sub_type> &>(in).pt2, &s->pt2, trfOrder);
//  } else if (in.getType() == entity_type::LINESTRING_2D ||
//             in.getType() == entity_type::MULTIPOINT_POINT_2D ||
//             in.getType() == entity_type::POLYGON_2D) {
//    const EntityPoints<sub_type> &_in = dynamic_cast<const EntityPoints<sub_type> &>(in);
//    dynamic_cast<EntityPoints<sub_type> *>(out)->resize(_in.getSize());
//    typename std::vector<T>::iterator it_out = dynamic_cast<EntityPoints<sub_type> *>(out)->begin();
//    for (typename std::vector<T>::const_iterator it = _in.begin(); it != _in.end(); it++, it_out++) {
//      this->transform(*it, &(*it_out), trfOrder);
//    }
//  } else {
//    //tipo no soportado
//    return;
//  }
//}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief perspective
 */
template<typename T>
class I3D_EXPORT TrfPerspective : public Transform2D<T>
{
private:

  cv::Mat H;

  typedef typename T::value_type sub_type;

public:

  TrfPerspective()
    : Transform2D<T>(1, transform_type::PERSPECTIVE) {}

  ~TrfPerspective() {}

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Aplica la transformación
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  void transform(const T &in, T *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &in, transform_order trfOrder = transform_order::DIRECT) const override;
};

template<typename T> inline
void TrfPerspective<T>::transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder) const
{
  //formatVectorOut(out, in.size());
  if (trfOrder == transform_order::DIRECT)
    cv::perspectiveTransform(in, *out, H);
  else {
    cv::perspectiveTransform(in, *out, H.inv());
  }
}

template<typename T> inline
void TrfPerspective<T>::transform(const T &in, T *out, transform_order trfOrder) const
{
  std::vector<T> vIn, vOut;
  vIn.push_back(in);
  if (trfOrder == transform_order::DIRECT) {
    cv::perspectiveTransform(vIn, vOut, H);
  } else {
    cv::perspectiveTransform(vIn, vOut, H.inv());
  }
  *out = vOut[0];
}

template<typename T> inline
T TrfPerspective<T>::transform(const T &in, transform_order trfOrder) const
{
  T out = in;
  transform(in, &out, trfOrder);
  return out;
}

template<typename T> inline
double TrfPerspective<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error)
{
  if (error) error = NULL; // Habria que poder calcular el error
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());
  if (n1 != n2) {
    printError("Número de puntos distinto en cada conjunto de puntos"); 
  } else {
    // Controlar estos errores a nivel general
    if (this->isNumberOfPointsValid(n1)) {

      H = cv::findHomography(pts1, pts2, cv::RANSAC);
      //cv::Mat H0 = cv::findHomography(pts1, pts2, cv::RANSAC);
      //cv::Mat H1 = cv::findHomography(pts1, pts2, cv::LMEDS);
      //cv::Mat H2 = cv::findHomography(pts1, pts2);
      //... determinar error
    } else
      printError("Número de puntos no valido. Se necesitan %i puntos para poder calcular los parámetros de la transformación", this->mMinPoint);
  }
  return H.empty() ? -1. : 1.;
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Traslación
 *
 * Transformación que aplica una traslación en el plano a un conjunto de puntos
 */
template<typename T>
class I3D_EXPORT Translate : public Transform2D<T>
{
private:

  /*!
   * \brief Traslación
   */
  T translate;

  typedef typename T::value_type sub_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  Translate()
    : Transform2D<T>(1, transform_type::TRANSLATE), translate(T()) {}

  /*!
   * \brief Constructora
   * \param[in] x0 Traslación en el eje X
   * \param[in] y0 Traslación en el eje Y
   */
  Translate(sub_type x0, sub_type y0)
    : Transform2D<T>(1, transform_type::TRANSLATE), translate(T(x0, y0)) { }

  /*!
   * \brief Calculo de la traslación
   *
   * Calcula la traslación entre dos sistemas diferentes a partir
   * de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Devuelve el desplazamiento en x de la transformación
   * \return Desplazamiento en el eje x
   */
  sub_type getTranslationX() { return translate.x; }

  /*!
   * \brief Devuelve el desplazamiento en y de la transformación
   * \return  Desplazamiento en el eje y
   */
  sub_type getTranslationY() { return translate.y; }

  /*!
   * \brief Establece los valores de desplazamiento
   * \param[in] x0 Desplazamiento en el eje x
   * \param[in] y0 Desplazamiento en el eje y
   */
  void setTranslation(sub_type x0, sub_type y0);

  /*!
   * \brief Establece el desplazamiento en el eje x
   * \param[in] x0 Desplazamiento en el eje x
   */
  void setTranslationX(sub_type x0);

  /*!
   * \brief Establece el desplazamiento en el eje y
   * \param[in] y0 Desplazamiento en el eje y
   */
  void setTranslationY(sub_type y0);

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando una traslación
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Transforma un conjunto de segmentos en otro aplicando una traslación
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const std::vector<Segment<sub_type>> &in, std::vector<Segment<sub_type>> *out, transform_order trfOrder = transform_order::DIRECT) const;

  /*!
   * \brief Aplica una traslación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const T &in, T *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica una traslación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &pt, transform_order trfOrder = transform_order::DIRECT) const override;

};

template<typename T> inline
double Translate<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error)
{
  double ret = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());
  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)){
      int m = n1 * this->mDimensions, n = 4;
      double *a = new double[m*n], *pa = a, *b = new double[m], *pb = b;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = 0;
          *pa++ = 1;
          *pa++ = 0;
          *pb++ = pts2[i].x;
          *pa++ = 0;
          *pa++ = pts1[i].y;
          *pa++ = 0;
          *pa++ = 1;
          *pb++ = pts2[i].y;
        }

        cv::Mat A(m, n, CV_64F, a);
        cv::Mat B(m, 1, CV_64F, b);
        cv::solve(A, B, C, cv::DECOMP_SVD);
        translate.x = static_cast<sub_type>(C.at<double>(2));
        translate.y = static_cast<sub_type>(C.at<double>(3));

        ret = _rootMeanSquareError(pts1, pts2, error);

      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] a;
      delete[] b;
    }
  }
  return ret;
}

template<typename T> inline
void Translate<T>::setTranslation(sub_type x0, sub_type y0)
{
  translate.x = x0;
  translate.y = y0;
}

template<typename T> inline
void Translate<T>::setTranslationX(sub_type x0)
{
  translate.x = x0;
}

template<typename T> inline
void Translate<T>::setTranslationY(sub_type y0)
{
  translate.y = y0;
}

template<typename T> inline
void Translate<T>::transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], trfOrder);
  }
}

template<typename T> inline
void Translate<T>::transform(const std::vector<Segment<sub_type>> &in, std::vector<Segment<sub_type>> *out, transform_order trfOrder) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i].pt1, &(*out)[i].pt1, trfOrder);
    transform(in[i].pt2, &(*out)[i].pt2, trfOrder);
  }
}

template<typename T> inline
void Translate<T>::transform(const T &in, T *out, transform_order trfOrder) const
{
  //... Podria llamar a transform(in, bDirect) pero es mas rapido hacerlo directamente
  *out = (trfOrder == transform_order::DIRECT) ? in + translate : in - translate;
}

template<typename T> inline
T Translate<T>::transform(const T &in, transform_order trfOrder) const
{
  return (trfOrder == transform_order::DIRECT) ? in + translate : in - translate;
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Rotación
 *
 * Transformación que aplica una rotación en el plano a un conjunto de puntos 
 */
template<typename T> 
class I3D_EXPORT Rotation : public Transform2D<T>
{
private:

  /*!
   * \brief Ángulo de rotación
   */
  double angle;

  /*!
   * \brief r1 = cos(angle);
   */
  double r1;

  /*!
   * \brief r2 = sin(angle);
   */
  double r2;

  typedef typename T::value_type sub_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  Rotation() 
    : Transform2D<T>(1, transform_type::ROTATION), angle(0.)
  {
    update();
  }

  /*!
   * \brief Constructora
   * \param[in] angle Ángulo en radianes
   */
  Rotation(double angle) 
    : Transform2D<T>(1, transform_type::ROTATION), angle(angle)
  {
    update();
  }
  
  /*!
   * \brief Calculo del águlo de rotación
   *
   * Calcula la rotación entre dos sistemas de coordenadas a partir de dos 
   * conjuntos de puntos
   * 
   * <H3>Ejemplo:</H3>
   * \code
   * Rotation<cv::Point2d> rot;
   * std::vector<cv::Point2d> pts_in{ cv::Point2d(12.3, 34.3), cv::Point2d(10.6, 34.345), cv::Point2d(34.76, 54.26) };
   * std::vector<cv::Point2d> pts_out{ cv::Point2d(-7.869, 35.578), cv::Point2d(-9.33, 34.71), cv::Point2d(0.499, 64.43) };
   * rot.compute(pts_in, pts_out);
   * \endcode
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Devuelve el ángulo de la rotación
   * \return Ángulo en radianes
   */
  sub_type getAngle() const { return angle; }

  /*!
   * \brief Establece en ángulo de la rotación
   * \param[in] ang Ángulo en radianes
   */
  void setAngle(sub_type ang);

  /*!
   * \brief Aplica una rotación a un conjunto de puntos
   *
   * Transforma un conjunto de puntos en un sistema cartesiano a otro 
   * sistema aplicando un giro.
   *
   * <H3>Ejemplo:</H3>
   * \code
   * Rotation<cv::Point2d> rot(0.562);
   * std::vector<cv::Point2d> pts_in{ cv::Point2d(12.3, 34.3), cv::Point2d(10.6, 34.345), cv::Point2d(34.76, 54.26) };
   * std::vector<cv::Point2d> pts_out;
   * rot.transform(pts_in, &pts_out);
   * \endcode
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica una rotación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const T &in, T *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica una rotación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &in, transform_order trfOrder = transform_order::DIRECT) const override;

private:

  /*!
   * \brief Actualiza los parámetros r1 y r2
   */
  void update();
};

template<typename T> inline
double Rotation<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error)
{
  double rmse = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());
  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)){
      int m = n1 * this->mDimensions, n = 2;
      double *a = new double[m*n], *pa = a, *b = new double[m], *pb = b;
      cv::Mat C(2, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = -pts1[i].y;
          *pb++ = pts2[i].x;
          *pa++ = pts1[i].y;
          *pa++ = pts1[i].x;
          *pb++ = pts2[i].y;
        }
        
        cv::Mat A(m, n, CV_64F, a);
        cv::Mat B(m, 1, CV_64F, b);
        cv::solve(A, B, C, cv::DECOMP_SVD);
        r1 = static_cast<sub_type>(C.at<double>(0));
        r2 = static_cast<sub_type>(C.at<double>(1));
        angle = acos(r1);

        rmse = _rootMeanSquareError(pts1, pts2, error);
      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] a;
      delete[] b;
    }
  }
  return rmse;
}

template<typename T> inline
void Rotation<T>::setAngle(sub_type ang)
{ 
  angle = ang; 
  update(); 
}

template<typename T> inline
void Rotation<T>::transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], trfOrder);
  }
}

template<typename T> inline
void Rotation<T>::transform(const T &in, T *out, transform_order trfOrder) const
{
  sub_type x_aux = in.x;
  if (trfOrder == transform_order::DIRECT) {
    out->x = static_cast<sub_type>(x_aux*r1 - in.y*r2);
    out->y = static_cast<sub_type>(x_aux*r2 + in.y*r1);
  } else {
    out->x = static_cast<sub_type>(x_aux*r1 + in.y*r2);
    out->y = static_cast<sub_type>(in.y*r1 - x_aux*r2);
  }
}

template<typename T> inline
T Rotation<T>::transform(const T &in, transform_order trfOrder) const
{
  T out;
  if (trfOrder == transform_order::DIRECT) {
    out.x = in.x*r1 - in.y*r2;
    out.y = in.x*r2 + in.y*r1;
  } else {
    out.x = in.x*r1 + in.y*r2;
    out.y = in.y*r1 - in.x*r2;
  }
  return out;
}

template<typename T> inline
void Rotation<T>::update()
{
  r1 = cos(angle);
  r2 = sin(angle);
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Tranformación Helmert 2D o transformación de semejanza
 *
 * Esta transformación expresa la relación que existe (o transformación que es 
 * preciso realizar)  entre dos sistemas cartesianos que discrepan en la situación 
 * del origen, en la orientación de los ejes y en la unidad de medida a lo largo 
 * de los mismos pero de manera que dicha variación en unidad de medida es constante 
 * a lo largo de cada eje y entre los dos ejes  
 *
 * \f$ a = scale * cos(rotation)\f$<BR>
 * \f$ b = scale * sin(rotation)\f$<BR>
 *
 * \f$ x' = a * x + b * y + X0\f$<BR>
 * \f$ y' = a * x - b * x + Y0\f$
 */
template<typename T>
class I3D_EXPORT Helmert2D : public Transform2D<T>
{
private:

  typedef typename T::value_type sub_type;

public:

  /*!
   * \brief Traslación en x
   */
  sub_type x0;

  /*!
   * \brief Traslación en y
   */
  sub_type y0;

private:

  /*!
   * \brief Escala
   */
  double scale;

  /*!
   * \brief Ángulo de rotation
   */
  double rotation;

  /*!
   * \brief a = scale * cos(rotation);
   */
  double a;

  /*!
   * \brief b = scale * sin(rotation);
   */
  double b;

public:

  /*!
   * \brief Constructor por defecto
   */
  Helmert2D()
    : Transform2D<T>(2, transform_type::HELMERT_2D), x0(0), y0(0), scale(1.), rotation(0.)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] x0 Traslación en x
   * \param[in] y0 Traslación en y
   * \param[in] scale Escala
   * \param[in] rotation Rotación
   */
  Helmert2D(sub_type x0, sub_type y0, double scale, double rotation) 
    : Transform2D<T>(2, transform_type::HELMERT_2D), x0(x0), y0(y0), scale(scale), rotation(rotation)
  {
    update();
  }

  //~Helmert2D();

  /*!
   * \brief Calcula la transformación Helmert 2D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando un helmert 2D
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   *  
   * <H3>Ejemplo:</H3>
   * \code
   * Helmert2D<cv::Point2d> h2d(x0,y0, scale, rotation);
   * std::vector<cv::Point2d> pts_in{ cv::Point2d(4157222.543, 664789.307),
   *     cv::Point2d(4149043.336, 688836.443), cv::Point2d(4172803.511, 690340.078),
   *     cv::Point2d(4177148.376, 642997.635), cv::Point2d(4137012.190, 671808.029), 
   *     cv::Point2d(4146292.729, 666952.887), cv::Point2d(4138759.902, 702670.738) };
   * std::vector<cv::Point2d> pts_out;
   * h2d.transform(pts_in, &pts_out);
   * \endcode
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
  * \brief Aplica un helmert 2D a un punto
  * \param[in] in Punto de entrada
  * \param[out] out Punto de salida
  * \param[in] bDirect Transformación directa
  */
  void transform(const T &in, T *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
  * \brief Aplica un helmert 2D a un punto
  * \param[in] in Punto de entrada
  * \param[in] bDirect Transformación directa
  * \return Punto de salida
  */
  T transform(const T &in, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Devuelve el giro
   * \return[in] Ángulo de rotación en radianes
   */
  double getRotation() const { return rotation; };

  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala de la transformación
   */
  double getScale() const { return scale;  };

  /*!
   * \brief Establece los parámetros
   * \param[in] x0 Traslación en x
   * \param[in] y0 Traslación en y
   * \param[in] scale Escala
   * \param[in] rotation Rotación
   */
  void setParameters(T x0, T y0, double scale, double rotation);

  /*!
   * \brief Establece la rotación de la transformación
   * \param[in] rotation Ángulo de rotación en radianes
   */
  void setRotation(double rotation);

  /*!
   * \brief Establece la escala de la transformación
   * \param[in] scale Escala de la transformación
   */
  void setScale(double scale);

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

};

template<typename T> inline
double Helmert2D<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error)
{
  double rmse = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)){
      int m = n1 * this->mDimensions, n = 4;
      double *A = new double[m*n], *pa = A, *B = new double[m], *pb = B;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 1;
          *pa++ = 0;
          *pb++ = pts2[i].x;
          *pa++ = pts1[i].y;
          *pa++ = -pts1[i].x;
          *pa++ = 0;
          *pa++ = 1;
          *pb++ = pts2[i].y;
        }

        cv::Mat mA(m, n, CV_64F, A);
        cv::Mat mB(m, 1, CV_64F, B);
        cv::solve(mA, mB, C, cv::DECOMP_SVD);

        a = C.at<double>(0);
        b = C.at<double>(1);
        x0 = static_cast<sub_type>(C.at<double>(2));
        y0 = static_cast<sub_type>(C.at<double>(3));

        rotation = atan2(b, a);
        scale = sqrt(a*a + b*b);

        rmse = _rootMeanSquareError(pts1, pts2, error);
      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] A;
      delete[] B;
    }
  }
  return rmse;
}

template<typename T> inline
void Helmert2D<T>::transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], trfOrder);
  }
}

template<typename T> inline
void Helmert2D<T>::transform(const T &in, T *out, transform_order trfOrder) const
{
  sub_type x_aux = in.x;
  if (trfOrder == transform_order::DIRECT){
    out->x = static_cast<sub_type>(a * x_aux - b * in.y + x0);
    out->y = static_cast<sub_type>(b * x_aux + a * in.y + y0);
  } else {
    double det = a*a + b*b;
    out->x = static_cast<sub_type>((a*(x_aux - x0) + b*(in.y - y0)) / det);
    out->y = static_cast<sub_type>((-b*(x_aux - x0) + a*(in.y - y0)) / det);
  }
  //*out = transform(in, bDirect);
}

template<typename T> inline
T Helmert2D<T>::transform(const T &in, transform_order trfOrder) const
{
  T r_pt;
  if (trfOrder == transform_order::DIRECT){
    r_pt.x = static_cast<sub_type>(a * in.x - b * in.y + x0);
    r_pt.y = static_cast<sub_type>(b * in.x + a * in.y + y0);
  } else {
    double det = a*a + b*b;
    r_pt.x = static_cast<sub_type>((a*(in.x - x0) + b*(in.y - y0)) / det);
    r_pt.y = static_cast<sub_type>((-b*(in.x - x0) + a*(in.y - y0)) / det);
  }
  return r_pt;
}

template<typename T> inline
void Helmert2D<T>::setParameters(T x0, T y0, double scale, double rotation)
{
  this->x0 = x0;
  this->y0 = y0;
  this->scale = scale;
  this->rotation = rotation;
  update();
}

template<typename T> inline
void Helmert2D<T>::setRotation(double rotation) 
{
  this->rotation = rotation;
  update();
}

template<typename T> inline
void Helmert2D<T>::setScale(double scale)
{
  this->scale = scale;
  update();
}

template<typename T> inline
void Helmert2D<T>::update()
{
  a = scale * cos(rotation);
  b = scale * sin(rotation);
}


/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Transformación Afin
 *
 * La Transformación Afín expresa la relación que existe (o la transformación que es 
 * preciso realizar) entre dos sistemas cartesianos que discrepan en la situación del 
 * origen, en la orientación de los ejes y en la unidad de medida a lo largo de los 
 * mismos de manera que dicha variación en unidad de medida es constante a lo largo 
 * de cada eje pero no entre los dos ejes.
 *
 * \f[ a =  scaleX * cos(rotation)\f]
 * \f[ b = -scaleY * sin(rotation)\f]
 * \f[ c =  scaleX * sin(rotation)\f]
 * \f[ d =  scaleY * cos(rotation)\f]
 *
 * \f[ x' = a * x + b * y + x0\f]
 * \f[ y' = c * x + d * y + y0\f]
 */
template<typename T>
class I3D_EXPORT Afin : public Transform2D<T>
{
private:

  typedef typename T::value_type sub_type;

public:

  /*!
   * \brief Traslación en x
   */
  sub_type x0;

  /*!
   * \brief Traslación en y
   */
  sub_type y0;

private:

  /*!
   * \brief Escala en el eje X
   */
  double mScaleX;

  /*!
   * \brief Escala en el eje Y
   */
  double mScaleY;
  /*!
   * \brief Ángulo de rotation
   */
  double mRotation;

  /*!
   * \brief a = scaleX * cos(rotation)
   */
  double a;

  /*!
   * \brief b = -scaleY * sin(rotation)
   */
  double b;

  /*!
   * \brief c = scaleX * sin(rotation)
   */
  double c;

  /*!
   * \brief d = scaleY * cos(rotation)
   */
  double d;

  double ai;

  double bi;

  double ci;

  double di;

  /*!
   * \brief Traslación en x transformación inversa
   */
  sub_type x0i;

  /*!
   * \brief Traslación en y transformación inversa
   */
  sub_type y0i;

public:

  /*!
   * \brief Constructor por defecto
   */
  Afin()
    : Transform2D<T>(3, transform_type::AFIN), x0(0), y0(0), mScaleX(1.), mScaleY(1.), mRotation(0.)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] x0 Traslación en el eje X
   * \param[in] y0 Traslación en el eje Y
   * \param[in] scaleX Escala en el eje X
   * \param[in] scaleY Escala en el eje Y
   * \param[in] rotation Rotación
   */
  Afin(sub_type x0, sub_type y0, double scaleX, double scaleY, double rotation)
    : Transform2D<T>(3, transform_type::AFIN), x0(x0), y0(y0), mScaleX(scaleX), mScaleY(scaleY), mRotation(rotation)
  {
    update();
  }

  //~Afin();

  /*!
   * \brief Calcula la transformación Helmert 2D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando un helmert 2D
   * <H3>Ejemplo:</H3>
   * \code
   * \endcode
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const T &in, T *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &in, transform_order trfOrder = transform_order::DIRECT) const override;

  void getParameters(double *a, double *b, double *c, double *d);

  /*!
   * \brief Devuelve el giro
   * \return Ángulo de rotación en radianes
   */
  double getRotation() const { return mRotation; };

  /*!
   * \brief Devuelve la escala correspondiente al eje X
   * \return Escala eje X
   */
  double getScaleX() const { return mScaleX; };

  /*!
   * \brief Devuelve la escala correspondiente al eje Y
   * \return Escala eje Y
   */
  double getScaleY() const { return mScaleY; };

  /*!
   * \brief Establece los parámetros
   * \param[in] x0 Traslación en x
   * \param[in] y0 Traslación en y
   * \param[in] scaleX Escala en x
   * \param[in] scaleY Escala en y
   * \param[in] rotation Rotación
   */
  void setParameters(T x0, T y0, double scaleX, double scaleY, double rotation);

  /*!
   * \brief Establece los parámetros
   * \param[in] a
   * \param[in] b
   * \param[in] c
   * \param[in] d
   * \param[in] x0 Traslación en x
   * \param[in] y0 Traslación en y
   */
  void setParameters(double a, double b, double c, double d, T x0, T y0);

  /*!
   * \brief Establece la rotación de la transformación
   * \param[in] rotation Ángulo de rotación en radianes
   */
  void setRotation(double rotation);

  /*!
   * \brief Establece la escala de la transformación en el eje X
   * \param[in] scaleX Escala de la transformación
   */
  void setScaleX(double scaleX);

  /*!
   * \brief Establece la escala de la transformación en el eje Y
   * \param[in] scaleY Escala de la transformación
   */
  void setScaleY(double scaleY);

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

};


template<typename T> inline
double Afin<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error)
{
  double rmse = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)){
      int m = n1 * this->mDimensions, n = 6;
      double *A = new double[m*n], *pa = A, *B = new double[m], *pb = B;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = 1;
          *pa++ = 0;
          *pb++ = pts2[i].x;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 0;
          *pa++ = 1;
          *pb++ = pts2[i].y;
        }

        cv::Mat mA(m, n, CV_64F, A);
        cv::Mat mB(m, 1, CV_64F, B);
        cv::solve(mA, mB, C, cv::DECOMP_SVD);

        a = C.at<double>(0);
        b = C.at<double>(1);
        c = C.at<double>(2);
        d = C.at<double>(3);
        x0 = static_cast<sub_type>(C.at<double>(4));
        y0 = static_cast<sub_type>(C.at<double>(5));

        //double f = atan2( c, a );
        //double w = atan2( -b, d );

        mRotation = (atan2(c, a) + atan2(-b, d) ) / 2.; //... Revisar lo de un unico giro
        mScaleX = sqrt(a*a + c*c);
        mScaleY = sqrt(b*b + d*d);

        rmse = _rootMeanSquareError(pts1, pts2, error);
      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] A;
      delete[] B;
    }
  }
  return rmse;
}

template<typename T> inline
void Afin<T>::transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], trfOrder);
  }
}

template<typename T> inline
void Afin<T>::transform(const T &in, T *out, transform_order trfOrder) const
{
  sub_type x_aux = in.x;
  if (trfOrder == transform_order::DIRECT){
    out->x = static_cast<sub_type>(a * x_aux + b * in.y + x0);
    out->y = static_cast<sub_type>(c * x_aux + d * in.y + y0);
  } else {
    out->x = static_cast<sub_type>(ai * x_aux + bi * in.y + x0i);
    out->y = static_cast<sub_type>(ci * x_aux + di * in.y + y0i);
  }
}

template<typename T> inline
T Afin<T>::transform(const T &in, transform_order trfOrder) const
{
  T r_pt;
  if (trfOrder == transform_order::DIRECT){
    r_pt.x = static_cast<sub_type>(a * in.x + b * in.y + x0);
    r_pt.y = static_cast<sub_type>(c * in.x + d * in.y + y0);
  }
  else {
    r_pt.x = static_cast<sub_type>(ai * in.x + bi * in.y + x0i);
    r_pt.y = static_cast<sub_type>(ci * in.x + di * in.y + y0i);
  }
  return r_pt;
}

template<typename T> inline
void Afin<T>::getParameters(double *_a, double *_b, double *_c, double *_d)
{
  *_a = a;
  *_b = b;
  *_c = c;
  *_d = d;
}


template<typename T> inline
void Afin<T>::setParameters(T x0, T y0, double scaleX, double scaleY, double rotation)
{
  this->x0 = x0;
  this->y0 = y0;
  mScaleX = scaleX;
  mScaleY = scaleY;
  mRotation = rotation;
  update();
}

template<typename T> inline
void setParameters(double a, double b, double c, double d, T x0, T y0)
{
  this->a = a;
  this->b = b;
  this->c = c;
  this->d = d;
  this->x0 = x0;
  this->y0 = y0;
  mRotation = (atan2(b, a) + atan2(-c, d) ) / 2.;
  mScaleX = sqrt(a*a + b*b);
  mScaleY = sqrt(c*c + d*d);
}

template<typename T> inline
void Afin<T>::setRotation(double rotation)
{
  mRotation = rotation;
  update();
}

template<typename T> inline
void Afin<T>::setScaleX(double scaleX)
{
  mScaleX = scaleX;
  update();
}

template<typename T> inline
void Afin<T>::setScaleY(double scaleY)
{
  mScaleY = scaleY;
  update();
}

template<typename T> inline
void Afin<T>::update()
{
  a =  mScaleX * cos(mRotation);
  b = -mScaleY * sin(mRotation);
  c =  mScaleX * sin(mRotation);
  d =  mScaleY * cos(mRotation);
  
  // Transformación inversa
  double det = a * d - c * b;
  if (!det) {
    printError("Determinante nulo");
  } else {
    ai = d / det;
    bi = -b / det;
    ci = -c / det;
    di = a / det;
    I3D_DISABLE_WARNING(4244)
    x0i = (-d * x0 + b * y0) / det;
    y0i = (-a * y0 + c * x0) / det;
    I3D_ENABLE_WARNING(4244)
  }
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Transformación Projectiva
 *
 * La Transformación Projectiva expresa la relación que existe entre dos planos.
 *
 * <BLOCKQUOTE>
 *    x' = ( a * x + b * y + c ) / ( g * x + h * y + 1 ) <BR>
 *    y' = ( d * x + e * y + f ) / ( g * x + h * y + 1 )
 * </BLOCKQUOTE>
 */
template<typename T>
class I3D_EXPORT Projective : public Transform2D<T>
{
private:

  /*!
   * \brief Parámetro a
   */
  double a;

  /*!
   * \brief Parámetro b
   */
  double b;

  /*!
   * \brief Parámetro c
   */
  double c;

  /*!
   * \brief Parámetro d
   */
  double d;

  /*!
   * \brief Parámetro e
   */
  double e;

  /*!
   * \brief Parámetro f
   */
  double f;

  /*!
   * \brief Parámetro g
   */
  double g;
  
  /*!
   * \brief Parámetro h
   */
  double h;

  /*!
   * \brief Parámetro 'a' transformación inversa
   */
  double ai;

  /*!
   * \brief Parámetro 'b' transformación inversa
   */
  double bi;

  /*!
   * \brief Parámetro 'c' transformación inversa
   */
  double ci;

  /*!
   * \brief Parámetro 'd' transformación inversa
   */
  double di;

  /*!
   * \brief Parámetro 'e' transformación inversa
   */
  double ei;

  /*!
   * \brief Parámetro 'f' transformación inversa
   */
  double fi;

  /*!
   * \brief Parámetro 'g' transformación inversa
   */
  double gi;
  
  /*!
   * \brief Parámetro 'h' transformación inversa
   */
  double hi;

  typedef typename T::value_type sub_type;

public:

  /*!
   * \brief Constructor por defecto
   */
  Projective()
    : Transform2D<T>(4, transform_type::PROJECTIVE), a(1), b(0), c(0), d(0), e(1), f(0), g(0), h(0)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] a
   * \param[in] b
   * \param[in] c
   * \param[in] d
   * \param[in] e
   * \param[in] f
   * \param[in] g
   * \param[in] h
   */
  Projective(double a, double b, double c, double d, double e, double f, double g, double h)
    : Transform2D<T>(4, transform_type::PROJECTIVE), a(a), b(b), c(c), d(d), e(e), f(f), g(g), h(h)
  {
    update();
  }

  //~Projective();

  /*!
   * \brief Calcula la transformación Helmert 2D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando un helmert 2D
   * <H3>Ejemplo:</H3>
   * \code
   * \endcode
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const T &in, T *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &in, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] a 
   * \param[in] b 
   * \param[in] c
   * \param[in] d
   * \param[in] e
   * \param[in] f
   * \param[in] g
   * \param[in] h
   */
  void setParameters(double a, double b, double c, double d, double e, double f, double g, double h);

  //getParameters();

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

};


template<typename T> inline
double Projective<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error)
{
  double rmse = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)){
      int m = n1 * this->mDimensions, n = 8;
      double *A = new double[m*n], *pa = A, *B = new double[m], *pb = B;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 1;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = -pts1[i].x * pts2[i].x;
          *pa++ = -pts2[i].x * pts1[i].y;
          *pb++ = pts2[i].x;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = pts1[i].x;
          *pa++ = pts1[i].y;
          *pa++ = 1;
          *pa++ = -pts2[i].y * pts1[i].x;
          *pa++ = -pts2[i].y * pts1[i].y;
          *pb++ = pts2[i].y;
        }

        cv::Mat mA(m, n, CV_64F, A);
        cv::Mat mB(m, 1, CV_64F, B);
        cv::solve(mA, mB, C, cv::DECOMP_SVD);

        a = C.at<double>(0);
        b = C.at<double>(1);
        c = C.at<double>(2);
        d = C.at<double>(3);
        e = C.at<double>(4);
        f = C.at<double>(5);
        g = C.at<double>(6);
        h = C.at<double>(7);

        rmse = _rootMeanSquareError(pts1, pts2, error);

      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] A;
      delete[] B;
    }
  }
  return rmse;
}

template<typename T> inline
void Projective<T>::transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], trfOrder);
  }
}

template<typename T> inline
void Projective<T>::transform(const T &in, T *out, transform_order trfOrder) const
{
  T pt_aux = in;
  if ( trfOrder == transform_order::DIRECT ) {
    out->x = static_cast<sub_type>((a * in.x + b * in.y + c) / (g * in.x + h * in.y + 1));
    out->y = static_cast<sub_type>((d * in.x + e * in.y + f) / (g * in.x + h * in.y + 1));
  } else {
    out->x = static_cast<sub_type>((ai * in.x + bi * in.y + ci) / (g * in.x + h * in.y + 1));
    out->y = static_cast<sub_type>((di * in.x + ei * in.y + fi) / (g * in.x + h * in.y + 1));
  }
}

template<typename T> inline
T Projective<T>::transform(const T &in, transform_order trfOrder) const
{
  T r_pt;
  if (trfOrder == transform_order::DIRECT){
    r_pt.x = static_cast<sub_type>((a * in.x + b * in.y + c) / (g * in.x + h * in.y + 1));
    r_pt.y = static_cast<sub_type>((d * in.x + e * in.y + f) / (g * in.x + h * in.y + 1));
  } else {
    r_pt.x = static_cast<sub_type>((ai * in.x + bi * in.y + ci) / (g * in.x + h * in.y + 1));
    r_pt.y = static_cast<sub_type>((di * in.x + ei * in.y + fi) / (g * in.x + h * in.y + 1));
  }
  return r_pt;
}

template<typename T> inline
void Projective<T>::setParameters(double _a, double _b, double _c, double _d, double _e, double _f, double _g, double _h)
{
  a = _a;
  b = _b;
  c = _c;
  d = _d;
  e = _e;
  f = _f;
  g = _g;
  h = _h;
  update();
}

template<typename T> inline
void Projective<T>::update()
{
  // Transformación inversa
  double aux = a * e - b * d;
  if (!aux) {
    printError("División por cero");
  } else {
    ai = (e - f * h) / aux;
    bi = (c * h - b) / aux;
    ci = (b * f - c * e) / aux;
    di = (f * g - d) / aux;
    ei = (a - c * g) / aux;
    fi = (c * d - a * f) / aux;
    gi = (d * h - e * g) / aux;
    hi = (b * g - a * h) / aux;
  }

}




/* ---------------------------------------------------------------------------------- */



/*!
 * \brief Tranformación polinómica
 *
 */
template<typename T>
class I3D_EXPORT polynomialTransform : public Transform2D<T>
{
private:

  typedef typename T::value_type sub_type;

public:

  /*!
   * \brief Constructor por defecto
   */
  polynomialTransform()
    : Transform2D<T>(2, transform_type::POLYNOMIAL)
  {
  }

  //~Helmert2D();

  /*!
   * \brief Calcula la transformación polinómica entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando una transformación polinómica
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
  * \brief Aplica una transformación polinómica a un punto
  * \param[in] in Punto de entrada
  * \param[out] out Punto de salida
  * \param[in] bDirect Transformación directa
  */
  void transform(const T &in, T *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
  * \brief Aplica una transformación polinómica a un punto
  * \param[in] in Punto de entrada
  * \param[in] bDirect Transformación directa
  * \return Punto de salida
  */
  T transform(const T &in, transform_order trfOrder = transform_order::DIRECT) const override;

};

template<typename T> inline
double polynomialTransform<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error)
{
  double rmse = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)) {
      double xmin = I3D_DOUBLE_MAX;
      double ymin = I3D_DOUBLE_MAX;
      double xmax = I3D_DOUBLE_MIN;
      double ymax = I3D_DOUBLE_MIN;
      for( int ipt = 0; ipt < n1; ipt++ ) {
        T &ptoi = pts1[ipt];
        if ( ptoi.x > xmax ) xmax = ptoi.x;
        if ( ptoi.x < xmin ) xmin = ptoi.x;
        if ( ptoi.y > ymax ) ymax = ptoi.y;
        if ( ptoi.y < ymin ) ymin = ptoi.y;
      }
      double xc = (xmax + xmin) / 2;
      double yc = (ymax + ymin) / 2;

      //...
    

    }
  }
  return rmse;
}

template<typename T> inline
void polynomialTransform<T>::transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], trfOrder);
  }
}

template<typename T> inline
void polynomialTransform<T>::transform(const T &in, T *out, transform_order trfOrder) const
{
  sub_type x_aux = in.x;

  //...
}

template<typename T> inline
T polynomialTransform<T>::transform(const T &in, transform_order trfOrder) const
{
  T r_pt;
  
  //...
  
  return r_pt;
}

/*! \} */ // end of trf2DGroup

/* ---------------------------------------------------------------------------------- */

/*! \defgroup trf3DGroup Transformaciones 3D
 *  Transformaciones geométricas en el espacio
 *  \{
 */

/* ---------------------------------------------------------------------------------- */

template<typename T>
class I3D_EXPORT Transform3D : public Transform<T>
{
private:

  typedef typename T::value_type sub_type;

public:

  /*!
   * \brief Transform
   */
  Transform3D(int n_min = 0, transform_type trfType = transform_type::DEFAULT)
    : Transform<T>(n_min, trfType)
  {
    this->mDimensions = 3;
  }

  /*!
   * \brief Destructora
   */
  virtual ~Transform3D() {}

  /*!
   * \brief Calcula los parámetros de transformación
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return RMSE (Root Mean Square Error)
   */
  virtual double compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error = NULL) override = 0;

  /*!
   * \brief Aplica la transformación
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  virtual void transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder = transform_order::DIRECT) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  virtual void transform(const T &in, T *out, transform_order trfOrder = transform_order::DIRECT) const override = 0;

  /*!
   * \brief Aplica la transformación a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  virtual T transform(const T &in, transform_order trfOrder = transform_order::DIRECT) const override = 0;

  /*!
   * \brief Aplica la transformación a una entidad geométrica
   * \param[in] in Entidad de entrada
   * \param[out] out Entidad de salida
   * \param[in] bDirect Transformación directa (por defecto)
   */
  void transformEntity(const Entity<sub_type> &in, Entity<sub_type> *out, transform_order trfOrder = transform_order::DIRECT) const override;

};

template<typename T> inline
void Transform3D<T>::transformEntity(const Entity<sub_type> &in, Entity<sub_type> *out, transform_order trfOrder = transform_order::DIRECT) const
{
  if (in.getType() == entity_type::BBOX) {
    Bbox<sub_type> *bbox = dynamic_cast<const Bbox<sub_type> *>(out);
    this->transform(dynamic_cast<const Bbox<sub_type> &>(in).pt1, &bbox->pt1, trfOrder);
    this->transform(dynamic_cast<const Bbox<sub_type> &>(in).pt2, &bbox->pt2, trfOrder);
  } else if(in.getType() == entity_type::SEGMENT_3D) {
    Segment3D<sub_type> *segment3d = dynamic_cast<Segment3D<sub_type> *>(out);
    this->transform(dynamic_cast<const Segment3D<sub_type> &>(in).pt1, &segment3d->pt1, trfOrder);
    this->transform(dynamic_cast<const Segment3D<sub_type> &>(in).pt2, &segment3d->pt2, trfOrder);
  } else if (in.getType() == entity_type::LINESTRING_3D ||
             in.getType() == entity_type::MULTIPOINT_POINT_3D ||
             in.getType() == entity_type::POLYGON_3D) {
    const EntityPoints<sub_type> &_in = dynamic_cast<const EntityPoints<sub_type> &>(in);
    dynamic_cast<EntityPoints<sub_type> *>(out)->resize(_in.getSize());
    typename std::vector<cv::Point3_<sub_type>>::iterator it_out = dynamic_cast<EntityPoints<sub_type> *>(out)->begin();
    for (typename std::vector<cv::Point3_<sub_type>>::const_iterator it = _in.begin();
      it != _in.end(); it++, it_out++) {
      this->transform(*it, &(*it_out), trfOrder);
    }
  } else {
    //tipo no soportado
    return;
  }
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Helmert 3D
 *
 * El Helmert 3D o Transformación de Semejanza en el espacio expresa la relación 
 * que existe (o la transformación que es preciso realizar) entre dos sistemas 
 * cartesianos que difieren en la situación del origen, en la orientación de los 
 * ejes y en la unidad de medida a lo largo de los mismos (eslala uniforme a lo
 * largo de los tres ejes)  
 *
 * <BLOCKQUOTE>

 * </BLOCKQUOTE>
 */
template<typename T>
class I3D_EXPORT Helmert3D : public Transform3D<T>
{
private:

  typedef typename T::value_type sub_type;

public:

  /*!
   * \brief Traslación en x
   */
  sub_type x0;

  /*!
   * \brief Traslación en y
   */
  sub_type y0;

  /*!
   * \brief Traslación en z
   */
  sub_type z0;

private:

  /*!
   * \brief Escala
   */
  double mScale;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mOmega;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mPhi;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mKappa;

  /*!
   * \brief Matriz de rotación
   */
  std::array<std::array<double, 3>, 3> mR;

  /*!
   * \brief Matriz de rotación inversa
   */
  std::array<std::array<double, 3>, 3> mRinv;

public:

  /*!
   * \brief Constructor por defecto
   */
  Helmert3D()
    : Transform3D<T>(3, transform_type::HELMERT_3D), x0(0), y0(0), z0(0), mScale(1.), mOmega(0.), mPhi(0.), mKappa(0.)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] x0 Traslación en X
   * \param[in] y0 Traslación en Y
   * \param[in] z0 Traslación en Z
   * \param[in] scale Escala
   * \param[in] omega Rotación respecto al eje X
   * \param[in] phi Rotación respecto al eje Y
   * \param[in] kappa Rotación respecto al eje Z
   */
  Helmert3D(sub_type x0, sub_type y0, sub_type z0, double scale, double omega, double phi, double kappa) 
    : Transform3D<T>(3, transform_type::HELMERT_3D), x0(x0), y0(y0), z0(z0), mScale(scale), mOmega(omega), mPhi(phi), mKappa(kappa)
  {
    update();
  }

  /*!
   * \brief Constructor
   * \param[in] x0 Traslación en X
   * \param[in] y0 Traslación en Y
   * \param[in] z0 Traslación en Z
   * \param[in] scale Escala
   * \param[in] rotation Matriz de rotación
   */
  Helmert3D(sub_type x0, sub_type y0, sub_type z0, double scale, const std::array<std::array<double, 3>, 3> &rotation) 
    : Transform3D<T>(3, transform_type::HELMERT_3D), x0(x0), y0(y0), z0(z0), mScale(scale), mR(rotation)
  {
    eulerAngles(mR, &mOmega, &mPhi, &mKappa);
    update();
  }

  //~Helmert3D();

  /*!
   * \brief Calcula la transformación Helmert 2D entre dos sistemas
   * diferentes a partir de dos conjuntos de puntos en cada sistema
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \return RMSE (Root Mean Square Error)
   */
  double compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error = NULL) override;

  /*!
   * \brief Devuelve una referencia a la matriz de rotación
   */
  const std::array<std::array<double, 3>, 3> &getRotationMatrix() const;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando un helmert 2D
   * <H3>Ejemplo:</H3>
   * \code
   * Helmert2D<cv::Point2d> h2d(x0,y0, scale, rotation);
   * std::vector<cv::Point2d> pts_in{ cv::Point2d(4157222.543, 664789.307),
   *     cv::Point2d(4149043.336, 688836.443), cv::Point2d(4172803.511, 690340.078),
   *     cv::Point2d(4177148.376, 642997.635), cv::Point2d(4137012.190, 671808.029), 
   *     cv::Point2d(4146292.729, 666952.887), cv::Point2d(4138759.902, 702670.738) };
   * std::vector<cv::Point2d> pts_out;
   * h2d.transform(pts_in, &pts_out);
   * \endcode
   * \param[in] in Puntos de entrada
   * \param[out] out Puntos de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] in Punto de entrada
   * \param[out] out Punto de salida
   * \param[in] bDirect Transformación directa
   */
  void transform(const T &in, T *out, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Aplica un helmert 2D a un punto
   * \param[in] in Punto de entrada
   * \param[in] bDirect Transformación directa
   * \return Punto de salida
   */
  T transform(const T &in, transform_order trfOrder = transform_order::DIRECT) const override;

  /*!
   * \brief Devuelve el giro
   * \return Ángulo de rotación en radianes
   */
  //double getRotation() const { return rotation; };

  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala de la transformación
   */
  double getScale() const { return mScale; };

  /*!
   * \brief Establece los parámetros
   * \param[in] x0 Traslación en X
   * \param[in] y0 Traslación en Y
   * \param[in] z0 Traslación en Z
   * \param[in] scale Escala
   * \param[in] omega Rotación respecto al eje X
   * \param[in] phi Rotación respecto al eje Y
   * \param[in] kappa Rotación respecto al eje Z
   */
  void setParameters(T x0, T y0, T z0, double scale, double omega, double phi, double kappa);

  /*!
   * \brief Establece la rotación de la transformación
   * \param Ángulo de rotación en radianes
   */
  //void setRotation(double rotation);

  /*!
   * \brief Establece la escala de la transformación
   * \param[in] Escala de la transformación
   */
  void setScale(double scale);

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

};

template<typename T> inline
double Helmert3D<T>::compute(const std::vector<T> &pts1, const std::vector<T> &pts2, std::vector<double> *error)
{
  double rmse = -1.;
  int n1 = static_cast<int>(pts1.size());
  int n2 = static_cast<int>(pts2.size());

  if (n1 != n2) {
    printf("...");
  } else {
    if (this->isNumberOfPointsValid(n1)) {
      int m = n1 * this->mDimensions, n = 7;
      double *A = new double[m*n], *pa = A, *L = new double[m], *pl = L;
      cv::Mat C(n, 1, CV_64F);
      try {
        for (int i = 0; i < n1; i++) {
          *pa++ = pts1[i].x;
          *pa++ = 0;
          *pa++ = -pts1[i].z;
          *pa++ = pts1[i].y;
          *pa++ = 1;
          *pa++ = 0;
          *pa++ = 0;
          *pl++ = pts2[i].x;
          *pa++ = pts1[i].y;
          *pa++ = pts1[i].z;
          *pa++ = 0;
          *pa++ = -pts1[i].x;
          *pa++ = 0;
          *pa++ = 1;
          *pa++ = 0;
          *pl++ = pts2[i].y;
          *pa++ = pts1[i].z;
          *pa++ = -pts1[i].y;
          *pa++ = pts1[i].x;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = 0;
          *pa++ = 1;
          *pl++ = pts2[i].z;
        }

        cv::Mat mA(m, n, CV_64F, A);
        cv::Mat mB(m, 1, CV_64F, L);
        cv::solve(mA, mB, C, cv::DECOMP_SVD);

        mScale = C.at<double>(0);
        mOmega = C.at<double>(1);
        mPhi = C.at<double>(2);
        mKappa = C.at<double>(3);
        x0 = static_cast<sub_type>(C.at<double>(4));
        y0 = static_cast<sub_type>(C.at<double>(5));
        z0 = static_cast<sub_type>(C.at<double>(6));

        update();
        
        rmse = _rootMeanSquareError(pts1, pts2, error);
      } catch (std::exception &e) {
        printError(e.what());
      }
      delete[] A;
      delete[] L;
    }
  }
  return rmse;
}

template<typename T> inline
const std::array<std::array<double, 3>, 3> &Helmert3D<T>::getRotationMatrix() const 
{
  return mR;
}

template<typename T> inline
void Helmert3D<T>::transform(const std::vector<T> &in, std::vector<T> *out, transform_order trfOrder) const
{
  formatVectorOut(in, out);
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], trfOrder);
  }
}

template<typename T> inline
void Helmert3D<T>::transform(const T &in, T *out, transform_order trfOrder) const
{
  T ptAux = in;
  if (trfOrder == transform_order::DIRECT){
    out->x = static_cast<sub_type>( mScale * (ptAux.x*mR[0][0] + ptAux.y*mR[0][1] + ptAux.z*mR[0][2]) + x0 );
    out->y = static_cast<sub_type>( mScale * (ptAux.x*mR[1][0] + ptAux.y*mR[1][1] + ptAux.z*mR[1][2]) + y0 );
    out->z = static_cast<sub_type>( mScale * (ptAux.x*mR[2][0] + ptAux.y*mR[2][1] + ptAux.z*mR[2][2]) + z0 );
  } else {
    sub_type dx = in.x - x0; 
    sub_type dy = in.y - y0; 
    sub_type dz = in.z - z0;
    out->x = static_cast<sub_type>(mScale * (dx*mRinv[0][0] + dy*mRinv[0][1] + dz*mRinv[0][2]));
    out->y = static_cast<sub_type>(mScale * (dx*mRinv[1][0] + dy*mRinv[1][1] + dz*mRinv[1][2]));
    out->z = static_cast<sub_type>(mScale * (dx*mRinv[2][0] + dy*mRinv[2][1] + dz*mRinv[2][2]));
  }
}

template<typename T> inline
T Helmert3D<T>::transform(const T &in, transform_order trfOrder) const
{
  T r_pt;
  if (trfOrder == transform_order::DIRECT){
    r_pt.x = static_cast<sub_type>( mScale * (in.x*mR[0][0] + in.y*mR[0][1] + in.z*mR[0][2]) + x0 );
    r_pt.y = static_cast<sub_type>( mScale * (in.x*mR[1][0] + in.y*mR[1][1] + in.z*mR[1][2]) + y0 );
    r_pt.z = static_cast<sub_type>( mScale * (in.x*mR[2][0] + in.y*mR[2][1] + in.z*mR[2][2]) + z0 );
  } else {
    sub_type dx = in.x - x0; 
    sub_type dy = in.y - y0; 
    sub_type dz = in.z - z0;
    r_pt.x = static_cast<sub_type>(mScale * (dx*mRinv[0][0] + dy*mRinv[0][1] + dz*mRinv[0][2]));
    r_pt.y = static_cast<sub_type>(mScale * (dx*mRinv[1][0] + dy*mRinv[1][1] + dz*mRinv[1][2]));
    r_pt.z = static_cast<sub_type>(mScale * (dx*mRinv[2][0] + dy*mRinv[2][1] + dz*mRinv[2][2]));
  }
  return r_pt;
}

template<typename T> inline
void Helmert3D<T>::setParameters(T x0, T y0, T z0, double scale, double omega, double phi, double kappa)
{
  this->x0 = x0;
  this->y0 = y0;
  this->z0 = z0;
  mScale = scale;
  mOmega = omega;
  mPhi = phi;
  mKappa = kappa;
  update();
}

template<typename T> inline
void Helmert3D<T>::setScale(double scale)
{
  mScale = scale;
}

template<typename T> inline
void Helmert3D<T>::update()
{
  rotationMatrix(mOmega, mPhi, mKappa, &mR);
  cv::Mat inv = cv::Mat(3, 3, CV_64F, mR.data()).inv();
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      mRinv[i][j] = inv.at<double>(i, j);
    }
  }
}

template<typename T> inline
Helmert3D<T> operator*(Helmert3D<T> &trf1, Helmert3D<T> &trf2)
{
  //const std::array<std::array<double, 3>, 3> R1 = trf1.getRotationMatrix();
  //const std::array<std::array<double, 3>, 3> R2 = trf2.getRotationMatrix();

  //cv::Mat _r1 = cv::Mat(3, 3, CV_32F, R1);

  //std::array<std::array<double, 3>, 3> R;
  //cv::Mat matRot1 = cv::Mat(3, 3, CV_64F, R1.data());
  //cv::Mat matRot2 = cv::Mat(3, 3, CV_64F, R2.data());
  //cv::Mat rot = matRot1 * matRot2;

  //T r_pt;
  //r_pt.x = static_cast<sub_type>( trf2.getScale() * (trf2.x*R1[0][0] + trf2.y*R1[0][1] + trf2.z*R1[0][2]) + trf2.x0 );
  //r_pt.y = static_cast<sub_type>( trf2.getScale() * (trf2.x*R1[1][0] + trf2.y*R1[1][1] + trf2.z*R1[1][2]) + trf2.y0 );
  //r_pt.z = static_cast<sub_type>( trf2.getScale() * (trf2.x*R1[2][0] + trf2.y*R1[2][1] + trf2.z*R1[2][2]) + trf2.z0 );

  //Helmert3D<T> trf(r_pt.x, r_pt.y, r_pt.z, trf1.getScale(), rot);

}

/*! \} */ // end of trf3DGroup

/* ---------------------------------------------------------------------------------- */

template<typename T, typename Point_t>
I3D_EXPORT void transform(const Entity<T> &in, Entity<T> *out, Transform<Point_t> *trf, transform_order trfOrder)
{
  if (in.getType() == entity_type::WINDOW) {
    Window<T> *w = dynamic_cast<Window<T> *>(out);
    trf->transform(dynamic_cast<const Window<T> &>(in).pt1, &w->pt1, trfOrder);
    trf->transform(dynamic_cast<const Window<T> &>(in).pt2, &w->pt2, trfOrder);
  } else if ( in.getType() == entity_type::SEGMENT_2D) {
    Segment<T> *s = dynamic_cast<Segment<T> *>(out);
    trf->transform(dynamic_cast<const Segment<T> &>(in).pt1, &s->pt1, trfOrder);
    trf->transform(dynamic_cast<const Segment<T> &>(in).pt2, &s->pt2, trfOrder);
  } else if (in.getType() == entity_type::LINESTRING_2D ||
             in.getType() == entity_type::MULTIPOINT_POINT_2D ||
             in.getType() == entity_type::POLYGON_2D) {
    const EntityPoints<T> &_in = dynamic_cast<const EntityPoints<T> &>(in);
    dynamic_cast<EntityPoints<T> *>(out)->resize(_in.getSize());
    typename std::vector<Point_t>::iterator it_out = dynamic_cast<EntityPoints<T> *>(out)->begin();
    for (typename std::vector<Point_t>::const_iterator it = _in.begin(); it != _in.end(); it++, it_out++) {
      trf->transform(*it, &(*it_out), trfOrder);
    }
  } else {
    //tipo no soportado
    return;
  }
}

template<typename Entity_t, typename Point_t>
I3D_EXPORT void transform(const std::vector<Entity_t> &in, std::vector<Entity_t> *out, Transform<Point_t> trf, transform_order trfOrder)
{
  for (int i = 0; i < in.size(); i++) {
    transform(in[i], &(*out)[i], trf, trfOrder);
  }
}

#ifdef HAVE_OPENCV

// Aplica una transformación a una imagen
template<typename T>
I3D_EXPORT void transform(cv::Mat in, cv::Mat out, Transform<T> trf, transform_order trfOrder)
{
  transform_type type = trf->getTransformType();
  switch (type) {
  case I3D::transform_type::TRANSLATE:
    Translate<T> transTrf = dynamic_cast<Translate<T>>(trf);
    cv::Mat translateMat( 2, 3, CV_32FC1 );
    translateMat.at<float>(0, 0) = 1.f;
    translateMat.at<float>(0, 1) = 0.f;
    translateMat.at<float>(0, 2) = transTrf.getTranslationX();
    translateMat.at<float>(1, 0) = 0.f;
    translateMat.at<float>(1, 1) = 1.f;
    translateMat.at<float>(1, 2) = transTrf.getTranslationY();
    cv::warpAffine(in, out, translateMat);
    break;
  case I3D::transform_type::ROTATION:
    Rotation<T> rotTrf = dynamic_cast<Rotation<T>>(trf);
    cv::Mat rotMat( 2, 3, CV_32FC1 );
    double a = cos(rotTrf.getAngle());
    double b = sin(rotTrf.getAngle());
    rotMat.at<float>(0, 0) = a;
    rotMat.at<float>(0, 1) = b;
    rotMat.at<float>(0, 2) = 0.f;
    rotMat.at<float>(1, 0) = b;
    rotMat.at<float>(1, 1) = a;
    rotMat.at<float>(1, 2) = 0.f;
    cv::warpAffine(in, out, rotMat);
    break;
  case I3D::transform_type::HELMERT_2D:
    Helmert2D<T> h2dTrf = dynamic_cast<Helmert2D<T>>(trf);
    cv::Mat h2DMat( 2, 3, CV_32FC1 );
    double rotation = h2dTrf.getRotation();
    double scale = h2dTrf.getScale();
    double a = scale * cos(rotation);
    double b = scale * sin(rotation);
    h2DMat.at<float>(0, 0) = a;
    h2DMat.at<float>(0, 1) = b;
    h2DMat.at<float>(0, 2) = h2dTrf.x0;
    h2DMat.at<float>(1, 0) = b;
    h2DMat.at<float>(1, 1) = a;
    h2DMat.at<float>(1, 2) = h2dTrf.y0;
    cv::warpAffine(in, out, h2DMat);
    break;
  case I3D::transform_type::AFIN:
    Afin<T> afinTrf = dynamic_cast<Afin<T>>(trf);
    double a, b, c, d;
    afinTrf.getParameters(&a, &b, &c, &d);
    cv::Mat afinMat( 2, 3, CV_32FC1 );
    afinMat.at<float>(0, 0) = a;
    afinMat.at<float>(0, 1) = b;
    afinMat.at<float>(0, 2) = afinTrf.x0;
    afinMat.at<float>(1, 0) = c;
    afinMat.at<float>(1, 1) = d;
    afinMat.at<float>(1, 2) = afinTrf.y0;
    cv::warpAffine(in, out, afinMat);
    break;
  case I3D::transform_type::PERSPECTIVE:

    cv::warpPerspective();
    break;
  case I3D::transform_type::PROJECTIVE:
    break;
  case I3D::transform_type::POLYNOMIAL:
    break;
  default:
    break;
  }
}

#endif // HAVE_OPENCV

/*! \} */ // end of trfGroup

} // End namespace I3D

#endif // I3D_TRANSFORM_H
