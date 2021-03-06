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
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_CORE_MATH_UTILS_H
#define TL_CORE_MATH_UTILS_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include <vector>
#include <array>

#include "tidop/core/messages.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/svd.h"

namespace tl
{

/*!
 * \addtogroup math
 *
 * \{
 */


/* ---------------------------------------------------------------------------------- */
/*           AJUSTES DE PUNTOS A GEOMETRIAS (LINEAS, PLANOS, ...)                     */
/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Recta de regresión de Y sobre X
 * La recta de regresión de Y sobre X se utiliza para estimar los valores de la Y a partir de los de la X.
 * La pendiente de la recta es el cociente entre la covarianza y la varianza de la variable X.
 *
 * \f$ y = m * x + b\f$<BR>
 *
 * \param[in] pts Puntos
 * \param[out] m Pendiente de la recta
 * \param[out] b Ordenada
 * \return Coeficiente de correlacción. Valor entre -1 (pendiente negativa) y 1 (pendiente positiva). Valores próximos a cero suponen un mal ajuste
 *
 * <h4>Ejemplo</h4>
 * \code
 * double m = 0.;
 * double b = 0.;
 * double corr = regressionLinearYX(pts, &m, &b);
 * \endcode
 */
//TL_EXPORT double regressionLinearYX(const std::vector<cv::Point2i> &pts, double *m, double *b);
template<typename Point_t> inline 
double regressionLinearYX(const std::vector<Point_t> &pts, double *m, double *b)
{
  double corr = 0.0;
  double sx = 0.;
  double sy = 0.;
  double sx2 = 0.;
  double sy2 = 0.;
  double sxy = 0.;
  size_t n = pts.size();
  if (n >= 2) {
    for (size_t i = 0; i < n; i++) {
      sx += pts[i].x;
      sy += pts[i].y;
      sx2 += pts[i].x * pts[i].x;
      sy2 += pts[i].y * pts[i].y;
      sxy += pts[i].x * pts[i].y;
    }
    double den = (n*sx2 - sx*sx);
    if (den != 0.) {
      // Línea no vertical
      *m = (n*sxy - sy*sx) / (n*sx2 - sx*sx);
      *b = (sy - *m*sx) / n;
      corr = sqrt(*m * (sxy - sx*sy / n) / (sy2 - sy*sy / n));
    }
  }
  return(corr);
}

/*!
 * \brief Recta de regresión de X sobre Y
 * La recta de regresión de X sobre Y se utiliza para estimar los valores de la X a partir de los de la Y.
 * La pendiente de la recta es el cociente entre la covarianza y la varianza de la variable Y.
 *
 * \f$ x = m*y + b\f$<BR>
 *
 * \param[in] pts Puntos
 * \param[out] m Pendiente de la recta
 * \param[out] b Ordenada
 * \return Coeficiente de correlacción. Valor entre -1 (pendiente negativa) y 1 (pendiente positiva). Valores próximos a cero suponen un mal ajuste
 *
 * <h4>Ejemplo</h4>
 * \code
 * double m = 0.;
 * double b = 0.;
 * double corr = regressionLinearXY(pts, &m, &b);
 * \endcode
 */
//TL_EXPORT double regressionLinearXY(const std::vector<cv::Point2i> &pts, double *m, double *b);
template<typename Point_t> inline 
double regressionLinearXY(const std::vector<Point_t> &pts, double *m, double *b)
{
  double corr = 0.0;
  double sx = 0.;
  double sy = 0.;
  double sx2 = 0.;
  double sy2 = 0.;
  double sxy = 0.;
  size_t n = pts.size();
  if (n >= 2) {
    for (size_t i = 0; i < n; i++) {
      sx += pts[i].x;
      sy += pts[i].y;
      sx2 += pts[i].x * pts[i].x;
      sy2 += pts[i].y * pts[i].y;
      sxy += pts[i].x * pts[i].y;
    }
    double den = (n*sy2 - sy*sy);
    if (den != 0.) {
      *m = (n*sxy - sy*sx) / (n*sy2 - sy*sy);
      *b = (sx - *m*sy) / n;
      corr = sqrt(*m * (sxy - sx*sy / n) / (sx2 - sx*sx / n));
    }
  }
  return(corr);
}



/*!
 * \brief Ajusta una recta a un conjunto de puntos por mínimos cuadrados
 *
 * La recta de regresión de Y sobre X se utiliza para estimar los valores de la Y a partir de los de la X.
 * La pendiente de la recta es el cociente entre la covarianza y la varianza de la variable X.
 *
 * \f$ y = m * x + b\f$<BR>
 *
 * Si se calcula la recta de regresión de Y sobre X la expresión utilizada es:
 *
 * \f$ x = m*y + b\f$<BR>
 * https://www.geometrictools.com/Documentation/LeastSquaresFitting.pdf
 *
 * \param[in] pts Puntos
 * \param[out] m Pendiente de la recta
 * \param[out] b Ordenada
 * \param[in] YX Si es true (valor por defecto) se cálcula la recta de regresión de Y sobre X. En caso contrario se calcula la recta de X sobre Y.
 * \return Coeficiente de correlacción. Valor entre -1 (pendiente negativa) y 1 (pendiente positiva). Valores próximos a cero suponen un mal ajuste
 *
 * <h4>Ejemplo de determinación de la recta de mejor ajuste</h4>
 * \code
 * double m = 0.;
 * double b = 0.;
 * double corr = linearFittingLS(pts, &m, &b);
 *
 * // Para el caso de la recta de X sobre Y
 * corr = linearFittingLS(pts, &m, &b, false); 
 *
 * \endcode
 */
//template<typename Point_t> inline
//double linearFittingLS(const std::vector<Point_t> &pts, double *m, double *b, bool YX = true)
//{
//  double corr = 0.0;
//  size_t size = pts.size();
//  if (size >= 2) {
//    size_t _m = size * 2;
//    size_t _n = 2;
//    double *_a = new double[_m*_n];
//    double *pa = _a;
//    double *_b = new double[_m];
//    double *pb = _b;
//    double *_c = new double[_n];
//    double sum_sx = 0.;
//    double sum_sy = 0.;
//    double sum_sx2 = 0.;
//    double sum_sy2 = 0.;
//    double sum_sxy = 0.;
//    double sx = 0.;
//    double sy = 0.;
//    double sx2 = 0.;
//    double sy2 = 0.;
//    double sxy = 0.;
//    for (size_t i = 0; i < size; i++) {
//      sum_sx += (sx = pts[i].x);
//      sum_sy += (sy = pts[i].y);
//      sum_sx2 += (sx2 = pts[i].x * pts[i].x);
//      sum_sy2 += (sy2 = pts[i].y * pts[i].y);
//      sum_sxy += (sxy = pts[i].x * pts[i].y );
//      *pa++ = YX ? sx2 : sy2;
//      *pa++ = YX ? sx : sy;
//      *pb++ = sxy;
//      *pa++ = YX ? sx : sy;
//      *pa++ = 1;
//      *pb++ = YX ? sy : sx;
//    }
//
//    solveSVD(_m, _n, _a, _b, _c);
//    *m = _c[0];
//    *b = _c[1];
//    double div = YX ? (sum_sy2 - sum_sy*sum_sy / size) : (sum_sx2 - sum_sx*sum_sx / size);
//    if (div != 0.) {
//      corr = sqrt(*m * (sum_sxy - sum_sx*sum_sy / size) / div);
//    }
//  }
//  return(corr);
//}

/*!
 * \brief Regresión exponencial
 * Ajusta una curva exponencial de regresión a un conjunto de datos según la formula:
 * 
 * \f$ y = A*r^x \f$<BR>
 *
 * A partir de esta expresión tomando logaritmos en ambos lados tenemos que:
 *
 * \f$ log10(y) = log10(A*r^x) \f$<BR>
 * \f$ log10(y) = log10(A) + log10(r^x) \f$<BR>
 * \f$ log10(y) = log10(A) + x * log10(r) \f$<BR>
 *
 * El logaritmo de <b>y</B> se puede expresar ahora como una función lineal y resolver mediante 
 * la recta de regresión donde:
 *
 * \f$ m = log10(r) \f$<BR>
 * \f$ b = log10(A) \f$<BR>
 *
 * y por tanto:
 *
 * \f$ r = 10^m \f$<BR>
 * \f$ A = 10^b \f$<BR>
 *
 * \param[in] pts Puntos
 * \param[out] A
 * \param[out] r
 */
//template<typename Point_t> inline
//void expRegression(const std::vector<Point_t> &pts, double *A, double *r)
//{
//  std::vector<geometry::PointD> ptsLog(pts.size());
//  std::transform(pts.begin(), pts.end(), ptsLog.begin(), 
//                 [](Point_t pt) -> geometry::PointD {
//                    return geometry::PointD(pt.x, log10(pt.y)); 
//                  });
//
//  double m = 0.;
//  double b = 0.;
//  regressionLinearYX<geometry::PointD>(ptsLog, &m, &b);
//  *A = pow(10, b);
//  *r = pow(10, m);
//}


/*!
 * \brief Obtiene la ecuación de un plano que pasa por tres puntos
 *
 * \f$ A*x + B*y + C*z + D = 0\f$
 *
 * \param[in] points Puntos que definen el plano
 * \param[out] plane Parametros de la ecuación general del plano (A, B, C, D)
 * \param[in] normalize Si es verdadero normaliza la ecuación del plano
 * \return Normal al plano
 */
template<typename T> inline 
double threePointsPlane(const std::array<T, 3> &points, std::array<double, 4> &plane, bool normalize = false) 
{
  T v1 = points[1] - points[0];
  T v2 = points[2] - points[0];
  plane[0] = v1.y*v2.z - v1.z*v2.y;
  plane[1] = v1.z*v2.x - v1.x*v2.z;
  plane[2] = v1.x*v2.y - v2.x*v1.y;
  plane[3] = -plane[0] *points[0].x - plane[1]*points[0].y - plane[2]*points[0].z;
  double N = sqrt(plane[0]*plane[0] + plane[1]*plane[1] + plane[2]*plane[2]);
  if ( N != 0. && normalize ) {
    plane[0] /= N;
    plane[1] /= N;
    plane[2] /= N;
    plane[3] /= N;
  }
  return N ;
}

/*!
 * \brief Obtiene la ecuación del plano de mejor ajuste de un conjunto de puntos resolviendo el sistema de ecuaciones por minimos cuadrados
 *
 * \f$ A*x + B*y + C*z + D = 0\f$
 *
 * Referencias:
 *
 * https://www.geometrictools.com/Documentation/LeastSquaresFitting.pdf
 * http://casanchi.com/mat/ajustesuperficies01.pdf
 *
 * \param[in] points Puntos que definen el plano
 * \param[out] plane Parametros de la ecuación general del plano (A, B, C, D)
 * \param[in] normalize Si es verdadero normaliza la ecuación del plano
 * \return Normal al plano
 */
//template<typename Point_t> inline
//double nPointsPlaneLS(const std::vector<Point_t> &points, std::array<double, 4> &plane, bool normalize = false)
//{
//  double N = 0.;
//  size_t size = points.size();
//
//  if (size < 3) {
//    return 0.; // O devolver error
//  }
//
//  if (size == 3) {
//    std::array<Point_t, 3> _points;
//    std::copy_n(points.begin(), 3, _points.begin());
//    threePointsPlane(_points, plane, normalize);
//  } else {
//    size_t m = size * 3;
//    size_t n = 3;
//    double *_a = new double[m*n];
//    double *pa = _a;
//    double *_b = new double[m];
//    double *pb = _b;
//    double *_c = new double[n];
//    try {
//      for (size_t i = 0; i < size; i++) {
//        *pa++ = points[i].x*points[i].x;
//        *pa++ = points[i].x*points[i].y;
//        *pa++ = points[i].x;
//        *pb++ = points[i].x*points[i].z;
//        *pa++ = points[i].x*points[i].y;
//        *pa++ = points[i].y*points[i].y;
//        *pa++ = points[i].y;
//        *pb++ = points[i].z*points[i].y;
//        *pa++ = points[i].x;
//        *pa++ = points[i].y;
//        *pa++ = 1;
//        *pb++ = points[i].z;
//      }
//
//      solveSVD(m, n, _a, _b, _c);
//
//      plane[0] = _c[0];
//      plane[1] = _c[1];
//      plane[2] = -1;
//      plane[3] = _c[2];
//
//      // Por si me interesase devolver el error
//      double dz = 0.;
//      double sumErr = 0.;
//      for (size_t i = 0; i < size; i++) {
//        dz = points[i].z - (plane[0]*points[i].x + plane[1]*points[i].y + plane[3]);
//        sumErr += dz*dz;
//      }
//
//      N = sqrt(plane[0]*plane[0] + plane[1]*plane[1] + 1);
//
//      if ( N != 0. && normalize ) {
//        plane[0] /= N;
//        plane[1] /= N;
//        plane[2] /= N;
//        plane[3] /= N;
//      }
//
//    } catch (std::exception &e) {
//      msgError(e.what());
//    }
//
//  }
//  return N ;
//}

//Lo mismo pero con contenedores
//template<typename it> inline
//double nPointsPlaneLS(it it_begin, it it_end, std::array<double, 4> &plane, bool normalize = false)
//{
//  double N = 0.;
//  typename std::iterator_traits<it>::difference_type size = std::distance(it_begin, it_end);
//  typedef typename std::iterator_traits<it>::value_type Point3_t;

//  if (size < 3) {
//    return 0.; // O devolver error
//  }

//  if (size == 3) {
//    std::array<Point3_t, 3> _points;
//    std::copy_n(it_begin, 3, _points.begin());
//    threePointsPlane(_points, plane, normalize);
//  } else {
//    size_t m = size * 3;
//    size_t n = 3;
//    double *_a = new double[m*n];
//    double *pa = _a;
//    double *_b = new double[m];
//    double *pb = _b;
//    double *_c = new double[n];
//    try {
//      for (int i = 0; i < size; i++, it_begin++) {
//        *pa++ = it_begin->x*it_begin->x;
//        *pa++ = it_begin->x*it_begin->y;
//        *pa++ = it_begin->x;
//        *pb++ = it_begin->x*it_begin->z;
//        *pa++ = it_begin->x*it_begin->y;
//        *pa++ = it_begin->y*it_begin->y;
//        *pa++ = it_begin->y;
//        *pb++ = it_begin->z*it_begin->y;
//        *pa++ = it_begin->x;
//        *pa++ = it_begin->y;
//        *pa++ = 1;
//        *pb++ = it_begin->z;

//      }

//      solveSVD(m, n, _a, _b, _c);
      
//      plane[0] = _c[0];
//      plane[1] = _c[1];
//      plane[2] = -1;
//      plane[3] = _c[2];

//      //// Por si me interesase devolver el error
//      //double dz = 0.;
//      //double sumErr = 0.;
//      //for (int i = 0; i < size; i++) {
//      //  dz = points[i].z - (plane[0]*points[i].x + plane[1]*points[i].y + plane[3]);
//      //  sumErr += dz*dz;
//      //}

//      N = sqrt(plane[0]*plane[0] + plane[1]*plane[1] + 1);

//      if ( N != 0. && normalize ) {
//        plane[0] /= N;
//        plane[1] /= N;
//        plane[2] /= N;
//        plane[3] /= N;
//      }
      
//    } catch (std::exception &e) {
//      msgError(e.what());
//    }

//  }
//  return N ;
//}

/*! \} */ // end of math

} // End namespace tl

#endif // TL_CORE_MATH_UTILS_H
