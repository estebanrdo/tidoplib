/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/

#ifndef TL_MATH_VECTOR_H
#define TL_MATH_VECTOR_H

#include "config_tl.h"

#include "tidop/core/defs.h"

#include <iterator>
#include <vector>
#include <array>

namespace tl
{

namespace math
{

/*! \addtogroup Math
 *  \{
 */


/*! \addtogroup Algebra
 *  \{
 */

template<size_t _size, typename T = double>
class Vector
{

public:

  /*!
   * \brief value_type
   */
  typedef typename std::array<T, _size>::value_type value_type;

  /*!
   * \brief Tipo entero sin signo (por lo general size_t)
   */
  typedef typename std::array<T, _size>::size_type size_type;

  /*!
   * \brief Tipo entero con signo (por lo general ptrdiff_t)
   */
  typedef typename std::array<T, _size>::difference_type difference_type;

  /*!
   * \brief std::allocator_traits<Allocator>::pointer
   */
  typedef typename std::array<T, _size>::pointer pointer;

  /*!
   * \brief std::allocator_traits<Allocator>::const_pointer 
   */
  typedef typename std::array<T, _size>::const_pointer const_pointer;

  /*!
   * \brief value_type&
   */
  typedef typename std::array<T, _size>::reference reference;

  /*!
   * \brief const value_type&
   */
  typedef typename std::array<T, _size>::const_reference const_reference;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  typedef typename std::array<T, _size>::iterator iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  typedef typename std::array<T, _size>::const_iterator const_iterator;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  typedef typename std::array<T, _size>::reverse_iterator reverse_iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  typedef typename std::array<T, _size>::const_reverse_iterator const_reverse_iterator;

protected:

  std::array<T, _size> mVector;

public:
  
  /*!
   * \brief Constructora por defecto
   */
  Vector();

  /*!
   * \brief Constructor de copia
   * \param[in] vector Objeto que se copia
   */
  Vector(const Vector &vector);

  /*!
   * \brief Constructor de movimiento
   * \param[in] vector Objeto que se mueve
   */
  Vector(Vector &&vector) TL_NOEXCEPT;

  /*!
   * \brief Constructora inicializador de lista
   * \param[in] values Listado de valores del vector
   */
  Vector(std::initializer_list<T> values);

  /*!
   * \brief Destructora
   */
  ~Vector() = default;

  /*!
   * \brief Devuelve el tama�o del vector
   * \return Tama�o del vector
   */
  size_t size() TL_NOEXCEPT;

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  /*!
   * \brief Devuelve un iterador al inicio del vector
   * \return Iterador al primer elemento del vector
   */
  iterator begin() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al inicio del vector
   * \return Iterador al primer elemento del vector
   */
  const_iterator begin() const TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador al siguiente elemento despu�s del final del vector
   * Este elemento act�a como un marcador de posici�n, intentar acceder a �l resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento despu�s del final del vector
   */
  iterator end() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento despu�s del final del contenedor
   * Este elemento act�a como un marcador de posici�n, intentar acceder a �l resulta en un comportamiento no definido 
   * \return Iterador al siguiente elemento despu�s del final del contenedor
   */
  const_iterator end() const TL_NOEXCEPT;

  reverse_iterator rbegin() TL_NOEXCEPT;
  const_reverse_iterator rbegin() const TL_NOEXCEPT;

  reverse_iterator rend() TL_NOEXCEPT;
  const_reverse_iterator rend() const TL_NOEXCEPT;

  /*!
   * \brief Devuelve una referencia constante al elemento de la posici�n indicada
   * return Referencia constante al elemento
   */
  const_reference at(size_type position) const;

  /*!
   * \brief Devuelve una referencia al elemento de la posici�n indicada
   * return Referencia al elemento
   */
  reference at(size_type position);

  /*!
   * \brief Devuelve una referencia al elemento de la posici�n indicada
   * No se comprueba si el elemento al que se quiere acceder esta dentro de los limites
   * return Referencia constante al elemento
   */
  const_reference operator[](size_t position) const;

  /*!
   * \brief Devuelve una referencia al elemento de la posici�n indicada
   * No se comprueba si el elemento al que se quiere acceder esta dentro de los limites
   * return Referencia al elemento
   */
  reference operator[](size_t position);

  /*!
   * \brief Comprueba si el contenedor esta vacio
   * \return true si el contenedor est� vac�o y false en caso contrario
   */
  //bool empty() const;

  void fill(T value);

  /*!
   * \brief Asignaci�n de copia
   */
  Vector &operator=(const Vector &entity);

  /*!
   * \brief Asignaci�n de movimiento
   */
  Vector &operator=(Vector &&entity) TL_NOEXCEPT;

  double module() const;

  void normalize() const;

  bool operator == (const Vector &vector) const;
  bool operator != (const Vector &vector) const;
  bool operator <  (const Vector &vector) const;
  bool operator <= (const Vector &vector) const;
  bool operator >  (const Vector &vector) const;
  bool operator >= (const Vector &vector) const;

  /*!
   * \brief Construye un vector de ceros
   * \f[ V = [ 0 0 0 ] \f]
   * \return
   */
  static Vector zero();

  //static Vector unit();

private:

};



/* Definici�n de alias Vector */


typedef Vector<2, int>    Vector2i;
typedef Vector<2, double> Vector2d;
typedef Vector<2, float>  Vector2f;
typedef Vector<3, int>    Vector3i;
typedef Vector<3, double> Vector3d;
typedef Vector<3, float>  Vector3f;


/* Implementaci�n Vector */

template<size_t _size, typename T> inline 
Vector<_size, T>::Vector()
{
  T ini_val = -std::numeric_limits<T>().max();
  for (size_t i = 0; i < _size; i++) {
    this->mVector[i] = ini_val;
  }
}

template<size_t _size, typename T> inline
Vector<_size, T>::Vector(const Vector &vector)
  : mVector(vector.mVector)
{
}

template<size_t _size, typename T> inline
Vector<_size, T>::Vector(Vector &&vector) TL_NOEXCEPT
  : mVector(std::forward<std::array<T, _size>>(vector.mVector))
{
}

template<size_t _size, typename T> inline
Vector<_size, T>::Vector(std::initializer_list<T> vector)
{
  size_t n = vector.size();
  if (n == _size){
    std::copy(vector.begin(), vector.end(), mVector.begin());
  } else if (n > _size){
    std::copy(vector.begin(), vector.end(), mVector.begin());
    std::fill(mVector.begin() + n, mVector.end(), T{0});
  } else {
    std::copy(vector.begin(), vector.begin() + n, mVector.begin());
  }
}

template<size_t _size, typename T> inline 
size_t Vector<_size, T>::size() TL_NOEXCEPT
{
  return _size;
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::reference Vector<_size, T>::front()
{
  return mVector.front();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_reference Vector<_size, T>::front() const
{
  return mVector.front();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::reference Vector<_size, T>::back()
{
  return mVector.back();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_reference Vector<_size, T>::back() const
{
  return mVector.back();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::iterator Vector<_size, T>::begin() TL_NOEXCEPT
{
  return mVector.begin();
}

template<size_t _size, typename T> inline 
typename Vector<_size, T>::const_iterator Vector<_size, T>::begin() const TL_NOEXCEPT
{
  return mVector.cbegin();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::iterator Vector<_size, T>::end() TL_NOEXCEPT
{
  return mVector.end();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_iterator Vector<_size, T>::end() const TL_NOEXCEPT
{
  return mVector.cend();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::reverse_iterator Vector<_size, T>::rbegin() TL_NOEXCEPT
{
  return mVector.rbegin();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_reverse_iterator Vector<_size, T>::rbegin() const TL_NOEXCEPT
{
  return mVector.rbegin();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::reverse_iterator Vector<_size, T>::rend() TL_NOEXCEPT
{
  return mVector.rend();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_reverse_iterator Vector<_size, T>::rend() const TL_NOEXCEPT
{
  return mVector.rend();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_reference Vector<_size, T>::at(size_type position) const
{
  return mVector.at(position);
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::reference Vector<_size, T>::at(size_type position)
{
  return mVector.at(position);
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_reference Vector<_size, T>::operator[](size_t position) const
{
  return mVector[position];
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::reference Vector<_size, T>::operator[](size_t position)
{
  return mVector[position];
}

//template<size_t _size, typename T> inline
//bool Vector<_size, T>::empty() const
//{
//  return mVector.empty();
//}

template<size_t _size, typename T> inline
void Vector<_size, T>::fill(T value)
{
  return mVector.fill(value);
}

template<size_t _size, typename T> inline
Vector<_size, T> &Vector<_size, T>::operator=(const Vector<_size, T> &vector)
{
  if (this != &vector) {
    this->mVector = vector.mVector;
  }
  return (*this);
}

template<size_t _size, typename T> inline
Vector<_size, T> &Vector<_size, T>::operator=(Vector<_size, T> &&entity) TL_NOEXCEPT
{
  if (this != &vector) {
    this->mVector = std::forward<std::array<T, _size>>(vector.mVector);
  }
  return (*this);
}

template<size_t _size, typename T> inline
double Vector<_size, T>::module() const
{
  return sqrt(dotProduct(v, v));
}

template<size_t _size, typename T> inline
void Vector<_size, T>::normalize() const
{
  T length = static_cast<T>(lenght());
  if (length > static_cast<T>(0)) {
    this /= length;
  } else {
    for (size_t i = 0; i < _size; i++) {
      this->mVector[i] = static_cast<T>(0);
    }
  }
}

template<size_t _size, typename T> inline
bool Vector<_size, T>::operator == (const Vector &vector) const
{
  return this->mVector == vector.mVector;
}

template<size_t _size, typename T> inline
bool Vector<_size, T>::operator != (const Vector &vector) const
{
  return this->mVector != vector.mVector;
}

template<size_t _size, typename T> inline
bool Vector<_size, T>::operator <  (const Vector &vector) const
{
  return this->mVector < vector.mVector;
}

template<size_t _size, typename T> inline
bool Vector<_size, T>::operator <= (const Vector &vector) const
{
  return this->mVector <= vector.mVector;
}

template<size_t _size, typename T> inline
bool Vector<_size, T>::operator >  (const Vector &vector) const
{
  return this->mVector > vector.mVector;
}

template<size_t _size, typename T> inline
bool Vector<_size, T>::operator >= (const Vector &vector) const
{
  return this->mVector > vector.mVector;
}

template<size_t _size, typename T> inline
Vector<_size, T> Vector<_size, T>::zero()
{
  Vector<_size, T> vector;
  for (size_t i = 0; i < _size; i++) {
    vector[i] = T{0};
  }
  return vector;
}

/* Operaciones unarias */

template<size_t _size, typename T>  static
Vector<_size, T> operator + (const Vector<_size, T> &vector)
{
  return vector;
}

template<size_t _size, typename T> static
Vector<_size, T> operator - (const Vector<_size, T> &vector)
{
  Vector<_size, T> v;
  for (size_t i = 0; i < _size; i++) {
    v[i] = -vector[i];
  }
  return v;
}

/* Operaciones binarias */

template<size_t _size, typename T>
Vector<_size, T> operator + (const Vector<_size, T> &v0,
                             const Vector<_size, T> &v1)
{
  Vector<_size, T> v = v0;
  return v += v1;
}

template<size_t _size, typename T>
Vector<_size, T> &operator += (Vector<_size, T> &v0, 
                               const Vector<_size, T> &v1)
{
  for (size_t i = 0; i < _size; i++) {
    v0[i] += v1[i];
  }
  return v0;
}

template<size_t _size, typename T>
Vector<_size, T> operator - (const Vector<_size, T> &v0,
                             const Vector<_size, T> &v1)
{
  Vector<_size, T> v = v0;
  return v -= v1;
}

template<size_t _size, typename T>
Vector<_size, T> &operator -= (Vector<_size, T> &v0, 
                               const Vector<_size, T> &v1)
{
  for (size_t i = 0; i < _size; i++) {
    v0[i] -= v1[i];
  }
  return v0;
}


template<size_t _size, typename T>
Vector<_size, T> operator*(Vector<_size, T> const& v0,
                          Vector<_size, T> const& v1)
{
  Vector<_size, T> result = v0;
  return result *= v1;
}

template<size_t _size, typename T>
Vector<_size, T> &operator *= (Vector<_size, T> &v0, 
                               const Vector<_size, T> &v1)
{
  for (size_t i = 0; i < _size; i++) {
    v0[i] *= v1[i];
  }
  return v0;
}

template<size_t _size, typename T>
Vector<_size, T> operator / (const Vector<_size, T> &v0,
                             const Vector<_size, T> &v1)
{
  Vector<_size, T> result = v0;
  return result /= v1;
}

template<size_t _size, typename T>
Vector<_size, T> &operator /= (Vector<_size, T> &v0, 
                               const Vector<_size, T> &v1)
{
  for (size_t i = 0; i < _size; i++) {
    v0[i] /= v1[i];
  }
  return v0;
}

template<size_t _size, typename T>
Vector<_size, T> operator * (const Vector<_size, T> &vector, 
                             T scalar)
{
  Vector<_size, T> v = vector;
  return v *= scalar;
}

template<size_t _size, typename T>
Vector<_size, T> operator * (T scalar, 
                             const Vector<_size, T> &vector)
{
  Vector<_size, T> v = vector;
  return v *= scalar;
}

template<size_t _size, typename T>
Vector<_size, T> &operator *= (Vector<_size, T> &vector, 
                               T scalar)
{
  for (size_t i = 0; i < _size; i++) {
    vector[i] *= scalar;
  }
  return vector;
}

template<size_t _size, typename T>
Vector<_size, T> operator / (const Vector<_size, T> &vector, 
                             T scalar)
{
  Vector<_size, T> v = vector;
  return v /= scalar;
}

template<size_t _size, typename T>
Vector<_size, T> &operator /= (Vector<_size, T> &vector,
                               T scalar)
{
  if (scalar != static_cast<T>(0)) {
    for (size_t i = 0; i < _size; i++) {
      vector[i] /= scalar;
    }
  } else {
    for (size_t i = 0; i < _size; i++) {
      vector[i] = static_cast<T>(0);
    }
  }
  return vector;
}


template<size_t _size, typename T> inline 
double dotProduct(const Vector<_size, T> &v1,
                  const Vector<_size, T> &v2)
{
  double dot = static_cast<double>(v1.at[0]) * static_cast<double>(v2[0]);
  for (size_t i = 1; i < _size; i++) {
    dot += static_cast<double>(v1[i]) * static_cast<double>(v2[i]);
  }
  return dot;
}

// Producto escalar
// Producto vectorial



/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl

#endif // TL_MATH_VECTOR_H
