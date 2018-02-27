#ifndef I3D_CURVES_H
#define I3D_CURVES_H

#include <limits>
#include <numeric>

#include "core/defs.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/types.hpp"
#endif // HAVE_OPENCV

#include "geometric_entities/entity.h"
#include "geometric_entities/point.h"

namespace I3D
{

/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */


namespace geometry
{

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase circulo
 *
 */
template<typename T>
class Circle : public Entity
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

  /*!
   * \brief Centro
   */
  Point<T> center;

  /*!
   * \brief Radio
   */
  T radius;

public:

  /*!
   * \brief Constructora por defecto
   */
  Circle();

  /*!
   * \brief Constructor
   * \param[in] center Centro
   * \param[in] radius Radio
   */
  Circle(const Point<T> &center, T radius);

  /*!
   * \brief Constructor de copia
   * \param[in] circle Objeto circulo que se copia
   */
  Circle(const Circle<T> &circle);

  /*!
   * \brief Operador de asignación
   * \param[in] circle Objeto Point que se copia
   */
  Circle<T>& operator = (const Circle<T>& circle);

  /*!
   * \brief Conversión de tipo
   */
  template<typename T2> operator Circle<T2>() const;

  /*!
   * \brief Area de un circulo
   * \return Area
   */
  double area() const;

  /*!
   * \brief Longitud o circunferencia del circulo
   * \return longitud
   */
  double length() const;

};

typedef Circle<int> CircleI;
typedef Circle<double> CircleD;
typedef Circle<float> CircleF;


template<typename T> inline
Circle<T>::Circle()
  : Entity(Entity::type::CIRCLE), 
    center(Point<T>()), 
    radius(static_cast<T>(1)) 
{
}

template<typename T> inline
Circle<T>::Circle(const Point<T> &center, T radius)
  : Entity(Entity::type::CIRCLE), 
    center(center), 
    radius(radius)
{
}

template<typename T> inline
Circle<T>::Circle(const Circle<T> &circle)
  : Entity(Entity::type::CIRCLE), 
    center(circle.center), 
    radius(circle.radius)
{
}

template<typename T> inline
Circle<T>& Circle<T>::operator = (const Circle& circle)
{
  this->center = circle.center;
  this->radius = circle.radius;
  return *this;
}

template<typename T> template<typename T2> inline
Circle<T>::operator Circle<T2>() const
{
  if (typeid(T2) == typeid(int)) {
    return Circle<T2>(Point<T2>(this->center), I3D_ROUND_TO_INT(this->radius));
  } else {
    return Circle<T2>(Point<T2>(center), static_cast<T2>(this->radius));
  }
}

template<typename T> inline
double Circle<T>::area() const 
{
  return I3D_2PI * radius * radius;
}

template<typename T> inline
double Circle<T>::length() const
{
  return 2. * I3D_PI * radius;
}


/*!
 * \brief Clase Ellipse
 *
 */
template<typename T>
class Ellipse : public Entity
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

  /*!
   * \brief Centro
   */
  Point<T> center;

  /*!
   * \brief Semieje mayor
   */
  T a;

  /*!
   * \brief Semieje menor
   */
  T b;

public:

  /*!
   * \brief Constructora por defecto
   */
  Ellipse();

  /*!
   * \brief Constructor
   * \param[in] a Semieje mayor
   * \param[in] b Semieje menor
   */
  Ellipse(const Point<T> &center, T a, T b);

  /*!
   * \brief Constructor de copia
   * \param[in] ellipse Objeto circulo que se copia
   */
  Ellipse(const Ellipse<T> &ellipse);

  /*!
   * \brief Operador de asignación
   * \param[in] ellipse Objeto Elipse que se copia
   */
  Ellipse<T>& operator = (const Ellipse<T>& ellipse);

  /*!
   * \brief Conversión de tipo
   */
  template<typename T2> operator ellipse<T2>() const;

  /*!
   * \brief Area de una elipse
   * \return Area
   */
  double area() const;

  /*!
   * \brief Longitud o circunferencia de la elipse
   * \return longitud
   */
  double length() const;

};

typedef Ellipse<int> EllipseI;
typedef Ellipse<double> EllipseD;
typedef Ellipse<float> EllipseF;


template<typename T> inline
Ellipse<T>::Ellipse()
  : Entity(Entity::type::ELLIPSE), 
    center(Point<T>()), 
    a(static_cast<T>(1)),
    b(static_cast<T>(1))
{
}

template<typename T> inline
Ellipse<T>::Ellipse(const Point<T> &center, T a, T b)
  : Entity(Entity::type::ELLIPSE), 
    center(center), 
    a(a),
    b(b)
{
}

template<typename T> inline
Ellipse<T>::Ellipse(const Ellipse<T> &ellipse)
  : Entity(Entity::type::ELLIPSE), 
    center(ellipse.center), 
    a(ellipse.a),
    b(ellipse.b)
{
}

template<typename T> inline
Ellipse<T>& Ellipse<T>::operator = (const Ellipse& ellipse)
{
  this->center = ellipse.center;
  this->a = ellipse.a;
  this->b = ellipse.b;
  return *this;
}

template<typename T> template<typename T2> inline
Ellipse<T>::operator Ellipse<T2>() const
{
  if (typeid(T2) == typeid(int)) {
    return Ellipse<T2>(Point<T2>(this->center), I3D_ROUND_TO_INT(this->a), I3D_ROUND_TO_INT(this->b));
  } else {
    return Ellipse<T2>(Point<T2>(center), static_cast<T2>(this->a), static_cast<T2>(this->b));
  }
}

template<typename T> inline
double Ellipse<T>::area() const 
{
  return I3D_2PI * a * b;
}

template<typename T> inline
double Ellipse<T>::length() const
{
  return I3D_PI * ( 3*(a+b) - sqrt((3*a+b)*(a+3*b));
}

/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_POINT_H