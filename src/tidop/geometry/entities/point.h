/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

#ifndef TL_GEOMETRY_POINT_H
#define TL_GEOMETRY_POINT_H

#include "config_tl.h"

//#include <limits>
//#include <numeric>
//#include <array>

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/entity.h"

namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */


/*!
 * \brief Clase punto 2D
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef Point<int> PointI;
 * typedef Point<double> PointD;
 * typedef Point<float> PointF;
 * \endcode
 */
template<typename T>
class Point 
  : public Entity
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  Point();

  /*!
   * \brief Constructor
   * \param[in] x Coordenada x
   * \param[in] y Coordenada y
   */
  Point(T x, T y);

  /*!
   * \brief Constructor de copia
   * \param[in] point Objeto Point que se copia
   */
  Point(const Point<T> &point);

  /*!
   * \brief Constructor de movimiento
   * \param[in] point Objeto Point que se mueve
   */
  Point(Point<T> &&point) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] v Objeto Point que se copia
   */
  Point(const std::array<T, 2> &v);

  ~Point() override = default;

  /*!
   * \brief Operador de asignación
   * \param[in] point Objeto Point que se copia
   */
  Point<T> &operator = (const Point<T>& point);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] point Objeto Point que se mueve
   */
  Point<T> &operator = (Point<T> &&point) TL_NOEXCEPT;

  /*!
   * \brief Conversión de tipo
   */
  template<typename T2> operator Point<T2>() const;

  /*!
   * \brief Conversión a un punto 3D
   */
  template<typename T2> operator Point3<T2>() const;

public:

  /*!
   * \brief Coordenada X
   */
  T x;

  /*!
   * \brief Coordenada Y
   */
  T y;

};


/* Definición de alias Point */

typedef Point<int> PointI;
typedef Point<double> PointD;
typedef Point<float> PointF;


/* Implementación Point */

template<typename T> inline
Point<T>::Point()
  : Entity(Entity::Type::point2d),
    x(static_cast<T>(0)), 
    y(static_cast<T>(0)) 
{
}

template<typename T> inline
Point<T>::Point(T x, T y)
  : Entity(Entity::Type::point2d),
    x(x), 
    y(y) 
{
}

template<typename T> inline
Point<T>::Point(const Point &pt)
  : Entity(Entity::Type::point2d),
    x(pt.x), 
    y(pt.y) 
{
}

template<typename T> inline
Point<T>::Point(Point &&pt) TL_NOEXCEPT
  : Entity(std::forward<Entity>(pt)), 
    x(std::exchange(pt.x, static_cast<T>(0))),
    y(std::exchange(pt.y, static_cast<T>(0)))
{
}

template<typename T> inline
Point<T>::Point(const std::array<T, 2> &v)
  : Entity(Entity::Type::point2d),
    x(v[0]), 
    y(v[1]) 
{
}

template<typename T> inline
Point<T> &Point<T>::operator = (const Point &pt)
{
  if (this != &pt) {
    Entity::operator = (pt);
    this->x = pt.x;
    this->y = pt.y;
  }
  return *this;
}


template<typename T> inline
Point<T> &Point<T>::operator = (Point &&pt) TL_NOEXCEPT
{
  if (this != &pt) {
    Entity::operator = (std::forward<Entity>(pt));
    this->x = std::exchange(pt.x, static_cast<T>(0));
    this->y = std::exchange(pt.y, static_cast<T>(0));
  }
  return *this;
}


TL_DISABLE_WARNING(TL_WARNING_C4244)
template<typename T> template<typename T2> inline
Point<T>::operator Point<T2>() const
{
  if (std::is_integral<T2>::value) {
    return Point<T2>(static_cast<T2>(std::round(this->x)), 
                     static_cast<T2>(std::round(this->y)));
  } else {
    return Point<T2>(static_cast<T2>(this->x), static_cast<T2>(this->y));
  }
}

template<typename T> template<typename T2> inline
Point<T>::operator Point3<T2>() const
{
  if (std::is_integral<T2>::value) {
    return Point3<T2>(static_cast<T2>(std::round(this->x)), 
                      static_cast<T2>(std::round(this->y)),
                      0);
  } else {
    return Point3<T2>(static_cast<T2>(this->x), 
                      static_cast<T2>(this->y), 
                      0);
  }
}

template<typename T> static inline
Point<T>& operator += (Point<T> &pt1, const Point<T> &pt2)
{
  pt1.x += pt2.x;
  pt1.y += pt2.y;
  return pt1;
}

template<typename T1, typename T2> static inline
Point<T1>& operator += (Point<T1> &pt1, const Point<T2> &pt2)
{
  pt1 += static_cast<Point<T1>>(pt2);
  return pt1;
}

template<typename T> static inline
Point<T>& operator -= (Point<T> &pt1, const Point<T> &pt2)
{
  pt1.x -= pt2.x;
  pt1.y -= pt2.y;
  return pt1;
}

template<typename T1, typename T2> static inline
Point<T1>& operator -= (Point<T1> &pt1, const Point<T2> &pt2)
{
  pt1 -= static_cast<Point<T1>>(pt2);
  return pt1;
}

template<typename T1, typename T2> static inline
Point<T1>& operator *= (Point<T1>& pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    pt.x = static_cast<T1>(std::round(pt.x * b));
    pt.y = static_cast<T1>(std::round(pt.y * b));
  } else {
    pt.x = static_cast<T1>(pt.x * b);
    pt.y = static_cast<T1>(pt.y * b);
  }
  return pt;
}

template<typename T1, typename T2> static inline
Point<T1> &operator /= (Point<T1> &pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    pt.x = static_cast<T1>(std::round(pt.x / static_cast<double>(b)));
    pt.y = static_cast<T1>(std::round(pt.y / static_cast<double>(b)));
  } else {
    pt.x = static_cast<T1>(pt.x / b);
    pt.y = static_cast<T1>(pt.y / b);
  }
  return pt;
}

template<typename T> static inline
bool operator == (const Point<T> &pt1, const Point<T> &pt2)
{
  return pt1.x == pt2.x && pt1.y == pt2.y;
}

template<typename T> static inline
bool operator != (const Point<T> &pt1, const Point<T> &pt2)
{
  return pt1.x != pt2.x || pt1.y != pt2.y;
}

template<typename T> static inline
Point<T> operator + (const Point<T> &pt1, const Point<T> &pt2)
{
  return Point<T>(pt1.x + pt2.x, pt1.y + pt2.y);
}

template<typename T> static inline
Point<T> operator - (const Point<T> &pt1, const Point<T> &pt2)
{
  return Point<T>(pt1.x - pt2.x, pt1.y - pt2.y);
}

template<typename T> static inline
Point<T> operator - (const Point<T> &pt1)
{
  return Point<T>(-pt1.x, -pt1.y);
}

template<typename T1, typename T2> static inline
Point<T1> operator * (const Point<T1> &pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    return Point<T1>(static_cast<T1>(std::round(pt.x*b)), 
                     static_cast<T1>(std::round(pt.y*b)));
  } else {
    return Point<T1>(static_cast<T1>(pt.x*b), 
                     static_cast<T1>(pt.y*b));
  }
}

template<typename T1, typename T2> static inline
Point<T2> operator * (T1 a, const Point<T2> &b)
{
  if (std::is_integral<T2>::value) {
    return Point<T2>(static_cast<T2>(std::round(b.x*a)), 
                     static_cast<T2>(std::round(b.y*a)));
  } else {
    return Point<T2>(static_cast<T2>(b.x*a), 
                     static_cast<T2>(b.y*a));
  }
}


template<typename T1, typename T2> static inline
Point<T1> operator / (const Point<T1> &pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    return Point<T1>(static_cast<T1>(std::round(pt.x / static_cast<double>(b))),
                     static_cast<T1>(std::round(pt.y / static_cast<double>(b))));
  } else {
    return Point<T1>(static_cast<T1>(pt.x / b), 
                     static_cast<T1>(pt.y / b));
  }
}


/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Clase punto 3D
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef Point<int> Point3I;
 * typedef Point<double> Point3D;
 * typedef Point<float> Point3F;
 * \endcode
 */
template<typename T>
class Point3 
  : public Entity
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

  /*!
   * \brief Coordenada X
   */
  T x;

  /*!
   * \brief Coordenada Y
   */
  T y;

  /*!
   * \brief Coordenada Z
   */ 
  T z;

public:

  /*!
   * \brief Constructora por defecto
   */
  Point3();

  /*!
   * \brief Constructor
   * \param[in] x Coordenada x
   * \param[in] y Coordenada y
   * \param[in] z Coordenada z
   */
  Point3(T x, T y, T z);

  /*!
   * \brief Constructor de copia
   * \param[in] point Objeto Point3 que se copia
   */
  Point3(const Point3<T> &point);

  /*!
   * \brief Constructor de movimiento
   * \param[in] point Objeto Point que se mueve
   */
  Point3(Point3<T> &&point) TL_NOEXCEPT;

  /*!
   * \brief Constructor vector
   * \param[in] v Vector de coordenadas
   */
  Point3(const std::array<T, 3> &v);

  ~Point3() override = default;

  /*!
   * \brief Operador de asignación
   * \param[in] point Objeto Point3 que se copia
   */
  Point3<T> &operator = (const Point3<T> &point);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] point Objeto Point3 que se mueve
   */
  Point3<T> &operator = (Point3<T> &&point) TL_NOEXCEPT;

  /*!
   * \brief Conversión de tipo
   */
  template<typename T2> operator Point3<T2>() const;
  template<typename T2> operator Point<T2>() const;
};

typedef Point3<int> Point3I;
typedef Point3<double> Point3D;
typedef Point3<float> Point3F;


template<typename T> inline
Point3<T>::Point3()
  : Entity(Entity::Type::point3d),
    x(0), 
    y(0), 
    z(0)
{
}

template<typename T> inline
Point3<T>::Point3(T x, T y, T z)
  : Entity(Entity::Type::point3d),
    x(x), 
    y(y), 
    z(z)
{
}

template<typename T> inline
Point3<T>::Point3(const Point3 &pt)
  : Entity(pt),
    x(pt.x), 
    y(pt.y), 
    z(pt.z)
{
}

template<typename T> inline
Point3<T>::Point3(Point3 &&pt) TL_NOEXCEPT
  : Entity(std::forward<Entity>(pt)),
    x(std::exchange(pt.x, 0)), 
    y(std::exchange(pt.y, 0)), 
    z(std::exchange(pt.z, 0))
{
}

template<typename T> inline
Point3<T>::Point3(const std::array<T, 3> &v)
  : Entity(Entity::Type::point3d),
    x(v[0]), 
    y(v[1]),
    z(v[2])
{
}

template<typename T> inline
Point3<T> &Point3<T>::operator = (const Point3 &pt)
{
  if (this != &pt) {
    Entity::operator = (pt);
    this->x = pt.x;
    this->y = pt.y;
    this->z = pt.z;
  }
  return *this;
}

template<typename T> inline
Point3<T> &Point3<T>::operator = (Point3 &&pt) TL_NOEXCEPT
{
  if (this != &pt) {
    Entity::operator = (std::forward<Entity>(pt));
    this->x = std::exchange(pt.x, 0);
    this->y = std::exchange(pt.y, 0);
    this->z = std::exchange(pt.z, 0);
  }
  return *this;
}
TL_DISABLE_WARNING(TL_WARNING_C4244)
template<typename T> template<typename T2> inline
Point3<T>::operator Point3<T2>() const
{
  if (std::is_integral<T2>::value) {
    return Point3<T2>(static_cast<T2>(std::round(this->x)), 
                      static_cast<T2>(std::round(this->y)), 
                      static_cast<T2>(std::round(this->z)));
  } else {
    return Point3<T2>(static_cast<T2>(this->x), 
                      static_cast<T2>(this->y), 
                      static_cast<T2>(this->z));
  }
}

template<typename T> template<typename T2> inline
Point3<T>::operator Point<T2>() const
{
  if (std::is_integral<T2>::value) {
    return Point<T2>(static_cast<T2>(std::round(this->x)), 
                     static_cast<T2>(std::round(this->y)));
  } else {
    return Point<T2>(static_cast<T2>(this->x), 
                     static_cast<T2>(this->y));
  }
}
TL_ENABLE_WARNING(TL_WARNING_C4244)

template<typename T> static inline
Point3<T>& operator += (Point3<T>& pt1, const Point3<T>& pt2)
{
  pt1.x += pt2.x;
  pt1.y += pt2.y;
  pt1.z += pt2.z;
  return pt1;
}

template<typename T1, typename T2> static inline
Point3<T1>& operator += (Point3<T1>& pt1, const Point3<T2>& pt2)
{
  pt1 += static_cast<Point3<T1>>(pt2);
  return pt1;
}

template<typename T> static inline
Point3<T>& operator -= (Point3<T>& pt1, const Point3<T>& pt2)
{
  pt1.x -= pt2.x;
  pt1.y -= pt2.y;
  pt1.z -= pt2.z;
  return pt1;
}

template<typename T1, typename T2> static inline
Point3<T1>& operator -= (Point3<T1>& pt1, const Point3<T2>& pt2)
{
  pt1 -= static_cast<Point3<T1>>(pt2);
  return pt1;
}

template<typename T1, typename T2> static inline
Point3<T1>& operator *= (Point3<T1>& pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    pt.x = static_cast<T1>(std::round(pt.x * b));
    pt.y = static_cast<T1>(std::round(pt.y * b));
    pt.z = static_cast<T1>(std::round(pt.z * b));
  } else {
    pt.x = static_cast<T1>(pt.x * b);
    pt.y = static_cast<T1>(pt.y * b);
    pt.z = static_cast<T1>(pt.z * b);
  }
  return pt;
}

template<typename T1, typename T2> static inline
Point3<T1>& operator /= (Point3<T1>& pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    pt.x = static_cast<T1>(std::round(pt.x / static_cast<double>(b)));
    pt.y = static_cast<T1>(std::round(pt.y / static_cast<double>(b)));
    pt.z = static_cast<T1>(std::round(pt.z / static_cast<double>(b)));
  } else {
    pt.x = static_cast<T1>(pt.x / static_cast<double>(b));
    pt.y = static_cast<T1>(pt.y / static_cast<double>(b));
    pt.z = static_cast<T1>(pt.z / static_cast<double>(b));
  }
  return pt;
}

template<typename T> static inline
bool operator == (const Point3<T>& pt1, const Point3<T>& pt2)
{
  return pt1.x == pt2.x && pt1.y == pt2.y && pt1.z == pt2.z;
}

template<typename T> static inline
bool operator != (const Point3<T>& pt1, const Point3<T>& pt2)
{
  return pt1.x != pt2.x || pt1.y != pt2.y || pt1.z != pt2.z;
}

template<typename T> static inline
Point3<T> operator + (const Point3<T>& pt1, const Point3<T>& pt2)
{
  return Point3<T>(pt1.x + pt2.x, pt1.y + pt2.y, pt1.z + pt2.z);
}

template<typename T> static inline
Point3<T> operator - (const Point3<T>& pt1, const Point3<T>& pt2)
{
  return Point3<T>(pt1.x - pt2.x, pt1.y - pt2.y, pt1.z - pt2.z);
}

template<typename T> static inline
Point3<T> operator - (const Point3<T>& pt1)
{
  return Point3<T>(-pt1.x, -pt1.y, -pt1.z);
}

template<typename T1, typename T2> static inline
Point3<T1> operator * (const Point3<T1>& pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    return Point3<T1>(static_cast<T1>(std::round(pt.x*b)), 
                      static_cast<T1>(std::round(pt.y*b)), 
                      static_cast<T1>(std::round(pt.z*b)));
  } else {
    return Point3<T1>(static_cast<T1>(pt.x*b), 
                      static_cast<T1>(pt.y*b), 
                      static_cast<T1>(pt.z*b));
  }
}

template<typename T1, typename T2> static inline
Point3<T2> operator * (T1 a, const Point3<T2>& pt)
{
  if (std::is_integral<T2>::value) {
    return Point3<T2>(static_cast<T2>(std::round(pt.x*a)), 
                      static_cast<T2>(std::round(pt.y*a)), 
                      static_cast<T2>(std::round(pt.z*a)));
  } else {
    return Point3<T2>(static_cast<T2>(pt.x*a), 
                      static_cast<T2>(pt.y*a), 
                      static_cast<T2>(pt.z*a));
  }
}

template<typename T1, typename T2> static inline
Point3<T1> operator / (const Point3<T1>& pt, T2 b)
{
  if (std::is_integral<T1>::value) {
    return Point3<T1>(static_cast<T1>(std::round(pt.x/static_cast<double>(b))), 
                      static_cast<T1>(std::round(pt.y/static_cast<double>(b))), 
                      static_cast<T1>(std::round(pt.z/static_cast<double>(b))));
  } else {
    return Point3<T1>(static_cast<T1>(pt.x/b), 
                      static_cast<T1>(pt.y/b), 
                      static_cast<T1>(pt.z/b));
  }
}



/*! \} */ // end of GeometricEntities

} // End namespace tl

#endif // TL_GEOMETRY_POINT_H
