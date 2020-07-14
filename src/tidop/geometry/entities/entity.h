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


#ifndef TL_GEOMETRY_ENTITY_H
#define TL_GEOMETRY_ENTITY_H

#include "config_tl.h"

#include <iterator>
#include <limits>
#include <numeric>
#include <vector>

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include "tidop/core/defs.h"
#include "tidop/core/flags.h"

namespace tl
{


/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */


// forward declaration

template<typename T> class Point;
template<typename T> class Point3;
template<typename T> class Window;
template<typename T> class BoundingBox;


/*!
 * \brief Clase base para entidades geometricas
 */
class TL_EXPORT Entity
{

public:

  /*!
   * \brief tipos de entidades geométricas
   */
  enum class Type
  {
    /* Dimensión */
    geom2d    = (0 << 0),                        /*!< Geometría 2D */
    geom3d    = (1 << 0),                        /*!< Geometría 3D */
    geom4d    = (1 << 1),                        /*!< Geometría 4D */

    /* multientidad */
    multi_entity = (1 << 2),                     /*!< Multientidad */

    /* Entidades 2D */
    point2d      = (1 << 3),                     /*!< Punto */
    linestring2d = (1 << 4),                     /*!< Polilinea */
    polygon2d    = (1 << 5),                     /*!< Poligono */
    segment2d    = (1 << 6),                     /*!< Segmento */
    circle       = (1 << 7),                     /*!< Circulo */
    ellipse      = (1 << 8),                     /*!< Elipse */
    triangle     = (1 << 9),                     /*!< Triangle */

    /* Entidades 3D*/
    point3d      = point2d | geom3d,             /*!< Punto 3D */
    linestring3d = linestring2d | geom3d,        /*!< Polilinea 3D */
    polygon3d    = polygon2d | geom3d,           /*!< Poligono 3D */
    segment3d    = segment2d | geom3d,           /*!< Segmento 3D */

    /* multientidades */
    multipoint2d   = point2d | multi_entity,      /*!< Multipunto 2D */
    multipoint3d   = point3d | multi_entity,      /*!< Multipunto 3D */
    multiline2d    = linestring2d | multi_entity, /*!< Multi-línea 2D */
    multiline3d    = linestring3d | multi_entity, /*!< Multi-línea 3D */
    multipolygon2d = polygon2d | multi_entity,    /*!< Multi-polígono 2D */
    multipoygon3d  = polygon3d | multi_entity,    /*!< Multi-polígono 3D */

    /* Tipos especiales */
    envelope = (1 << 20),                         /*!< Envolvente */
    window   = envelope,                          /*!< Ventana */
    bounding_box = envelope | geom3d              /*!< Cuadro delimitador */
  };

public:

  /*!
   * \brief Constructora
   * \param[in] type Tipo de entidad
   */
  Entity(Type type)
    : mEntityType(type)
  {}
   
  /*!
   * \brief Constructor de copia
   * \param[in] entity Objeto que se copia
   */
  Entity(const Entity &entity) 
    : mEntityType(entity.mEntityType)
  {}
  
  /*!
   * \brief Constructor de movimiento
   * \param[in] entity Objeto que se copia
   */
  Entity(Entity &&entity) TL_NOEXCEPT 
    : mEntityType(std::move(entity.mEntityType))
  {}

  /*!
   * \brief Destructora
   */
  virtual ~Entity() = default;

  /*!
   * \brief Devuelve el tipo de entidad 
   */
  Type type() const { return mEntityType.flags(); }

  /*!
   * \brief Operador de asignación
   */
  Entity &operator = (const Entity &entity) 
  {
    if (this != &entity) {
      mEntityType = entity.mEntityType;
    }
    return *this;
  }

  /*!
   * \brief Operador de asignación de movimiento
   */
  Entity &operator = (Entity &&entity) TL_NOEXCEPT
  {
    if (this != &entity) {
      mEntityType = std::move(entity.mEntityType);
    }
    return *this;
  }

  /*!
   * \brief La dimensión inherente del objeto geométrico
   * Debe ser menor o igual a la dimensión de coordenadas.
   */
  //virtual size_t dimension() const = 0;

  //virtual size_t coordinateDimension() const = 0;
  //virtual size_t spatialDimension() const = 0;

  /*!
   * \brief Comprueba si una entidad geométrica esta vacía.
   * \return Devuelve true si la entidad geométrica esta vacía.
   */
  //virtual bool isEmpty() const = 0;

  /*!
   * \brief Devuelve verdadero si la entidad geométrica no presenta una geométria anómala 
   * como auto-intersección.
   */
  //virtual bool isSimple() const = 0;

  /*!
   * \brief Comprueba si una entidad es 3D
   * \return Verdadero si es una entidad 3D
   */
  bool is3D() const
  {
    return mEntityType.isActive(Type::geom3d);
  }

protected:

  /*!
   * \brief Tipo de entidad
   * \see type
   */
  EnumFlags<Type> mEntityType;

};
ALLOW_BITWISE_FLAG_OPERATIONS(Entity::Type)

/* ---------------------------------------------------------------------------------- */

//TODO: Añadir constructor a partir de iteradores first end

/*!
 * \brief Contenedor para entidades geométricas
 * 
 * Es un simple wrapper de std::vector para que las clases de geometrías 
 * se puedan utilizar como contenedores.
 */
template<typename Entity_t>
class EntityContainer
{

public:

  /*!
   * \brief Allocator
   */
  typedef typename std::vector<Entity_t>::allocator_type allocator_type;

  /*!
   * \brief value_type
   */
  typedef typename std::vector<Entity_t>::value_type value_type;

  /*!
   * \brief Tipo entero sin signo (por lo general size_t) 
   */
  typedef typename std::vector<Entity_t>::size_type size_type;

  /*!
   * \brief Tipo entero con signo (por lo general ptrdiff_t)
   */
  typedef typename std::vector<Entity_t>::difference_type difference_type;

  /*!
   * \brief std::allocator_traits<Allocator>::pointer
   */
  typedef typename std::vector<Entity_t>::pointer pointer;

  /*!
   * \brief std::allocator_traits<Allocator>::const_pointer 
   */
  typedef typename std::vector<Entity_t>::const_pointer const_pointer;

  /*!
   * \brief value_type&
   */
  typedef typename std::vector<Entity_t>::reference reference;

  /*!
   * \brief const value_type&
   */
  typedef typename std::vector<Entity_t>::const_reference const_reference;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  typedef typename std::vector<Entity_t>::iterator iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  typedef typename std::vector<Entity_t>::const_iterator const_iterator;

public:
  
  /*!
   * \brief Constructora por defecto
   */
  EntityContainer();

  /*!
   * \brief Constructora
   * \param[in] size Tamaño que se reserva para el contenedor
   */
  EntityContainer(size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] entity Objeto que se copia
   */
  EntityContainer(const EntityContainer &entity);

  /*!
   * \brief Constructor de movimiento
   * \param[in] entity Objeto que se mueve
   */
  EntityContainer(EntityContainer &&entity) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] entities
   */
  EntityContainer(const std::vector<Entity_t> &entities);

  /*!
   * \brief Constructora de lista
   * \param[in] entities listado de entidades
   */
  EntityContainer(std::initializer_list<Entity_t> entities);

  /*!
   * \brief Destructora
   */
  ~EntityContainer() = default;


  /*!
   * \brief Devuelve un iterador al inicio del contenedor
   * \return Iterador al primer elemento
   */
  virtual iterator begin();

  /*!
   * \brief Devuelve un iterador constante al inicio del contenedor
   * \return Iterador al primer elemento
   */
  virtual const_iterator begin() const;

  /*!
   * \brief Devuelve un iterador al siguiente elemento después del final del contenedor
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del final del contenedor
   */
  virtual iterator end();

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento después del final del contenedor
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido 
   * \return Iterador al siguiente elemento después del final del contenedor
   */
  virtual const_iterator end() const;

  /*!
   * \brief Agrega un elemento al final del contenedor
   * \param[in] entity Entidad que se añade
   */
  void push_back(const Entity_t &entity);

  /*!
   * \brief Agrega un elemento mediante movimiento al final del contenedor
   * \param[in] entity Entidad que se añade
   */
  void push_back(Entity_t &&entity) TL_NOEXCEPT;

  /*!
   * \brief Devuelve una referencia constante al elemento de la posición indicada
   * return Referencia constante al elemento
   */
  const_reference at(size_type position) const;

  /*!
   * \brief Devuelve una referencia al elemento de la posición indicada
   * return Referencia al elemento
   */
  reference at(size_type position);

  /*!
   * \brief Elimina los elementos del recipiente
   */
  void clear();
    
  /*!
   * \brief Comprueba si el contenedor esta vacio
   * \return true si el contenedor está vacío y false en caso contrario
   */
  bool empty() const;

  /*!
   * \brief Establece el tamaño del contenedor
   * \param[in] size
   */
  void reserve(size_type size);

  /*!
   * \brief Modifica el tamaño del contenedor
   * Si el tamaño actual es menor que count, se añaden elementos adicionales. Si el tamaño actual 
   * es mayor que count el contenedor se trunca al número de elementos indicados.
   * \param[in] count Nuevo tamaño del contenedor
   */
  void resize(size_type count);

  /*!
   * \brief Modifica el tamaño del contenedor
   * Si el tamaño actual es menor que count, se añaden elementos adicionales y se inicializan con value.
   * Si el tamaño actual es mayor que count el contenedor se trunca al número de elementos indicados.
   * \param[in] count Nuevo tamaño del contenedor
   * \param[in] value Valor que se asigna a los nuevos elementos
   */
  void resize(size_type count, const Entity_t &value);

  /*!
   * \brief Devuelve el tamaño del contenedor
   * \return Tamaño
   */
  size_type size() const;

  /*!
   * \brief Devuelve una referencia al elemento de la posición indicada
   * No se comprueba si el elemento al que se quiere acceder esta dentro de los limites
   * return Referencia constante al elemento
   */
  const_reference operator[](size_type position) const;

  /*!
   * \brief Devuelve una referencia al elemento de la posición indicada
   * No se comprueba si el elemento al que se quiere acceder esta dentro de los limites
   * return Referencia al elemento
   */
  reference operator[](size_type position);

  /*!
   * \brief Asignación de copia
   */
  EntityContainer<Entity_t>& operator=(const EntityContainer<Entity_t> &entity);

  /*!
   * \brief Asignación de movimiento
   */
  EntityContainer<Entity_t>& operator=(EntityContainer<Entity_t> &&entity) TL_NOEXCEPT;

  /*!
   * \brief Elimina el intervalo
   */
  iterator erase(const_iterator first, const_iterator last);

  //TODO: insert (para insertar nuevos valores en una posición)

protected:

  /*!
   * \brief Conjunto de puntos
   */
  std::vector<Entity_t> mEntities;

};

// Implementación EntityContainer

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer() 
  : mEntities(0)
{
}

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer(size_type size)
  : mEntities(size)
{
}

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer(const EntityContainer &entity)
  : mEntities(entity.mEntities)
{
}

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer(EntityContainer &&entity) TL_NOEXCEPT
  : mEntities(std::move(entity.mEntities))
{
}

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer(const std::vector<Entity_t> &entities)
  : mEntities(entities)
{
}

template<typename Entity_t> inline
EntityContainer<Entity_t>::EntityContainer(std::initializer_list<Entity_t> entities)
  : mEntities(entities)
{
}

template<typename Entity_t> inline 
typename EntityContainer<Entity_t>::iterator EntityContainer<Entity_t>::begin() 
{
  return mEntities.begin();
}

template<typename Entity_t> inline 
typename EntityContainer<Entity_t>::const_iterator EntityContainer<Entity_t>::begin() const 
{
  return mEntities.cbegin();
}

template<typename Entity_t> inline 
typename EntityContainer<Entity_t>::iterator EntityContainer<Entity_t>::end()
{
  return mEntities.end();
}

template<typename Entity_t> inline 
typename EntityContainer<Entity_t>::const_iterator EntityContainer<Entity_t>::end() const 
{
  return mEntities.cend();
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::push_back(const Entity_t &entity)
{
  mEntities.push_back(entity);
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::push_back(Entity_t &&entity) TL_NOEXCEPT
{
  mEntities.push_back(std::forward<Entity_t>(entity));
}

template<typename Entity_t> inline
typename std::vector<Entity_t>::const_reference EntityContainer<Entity_t>::at(size_type position) const
{
  return mEntities.at(position);
}

template<typename Entity_t> inline
typename std::vector<Entity_t>::reference EntityContainer<Entity_t>::at(size_type position)
{
  return mEntities.at(position);
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::clear() 
{ 
  mEntities.clear();
}

template<typename Entity_t> inline
bool EntityContainer<Entity_t>::empty() const
{
  return mEntities.empty();
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::reserve(size_type size)
{
  mEntities.reserve(size);
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::resize(size_type count)
{
  mEntities.resize(count);
}

template<typename Entity_t> inline
void EntityContainer<Entity_t>::resize(size_type count, const Entity_t &value)
{
  mEntities.resize(count, value);
}

template<typename Entity_t> inline
typename EntityContainer<Entity_t>::size_type EntityContainer<Entity_t>::size() const
{ 
  return mEntities.size();
}

template<typename Entity_t> inline
typename EntityContainer<Entity_t>::const_reference EntityContainer<Entity_t>::operator[](size_type position) const
{
  return mEntities[position];
}
  
template<typename Entity_t> inline
typename EntityContainer<Entity_t>::reference EntityContainer<Entity_t>::operator[](size_type position)
{
  return mEntities[position];
}

template<typename Entity_t> inline
EntityContainer<Entity_t> &EntityContainer<Entity_t>::operator=(const EntityContainer<Entity_t> &entity) 
{
  if (this != &entity) {
    this->mEntities = entity.mEntities;
  }
  return (*this);
}

template<typename Entity_t> inline
EntityContainer<Entity_t> &EntityContainer<Entity_t>::operator=(EntityContainer<Entity_t> &&entity) TL_NOEXCEPT
{
  if (this != &entity) {
    this->mEntities.clear();
    this->mEntities = std::move(entity.mEntities);
    entity = 0; ///TODO: Si hago move esto debería ser nulo
  }
  return (*this);
}

template<typename Entity_t> inline
typename EntityContainer<Entity_t>::iterator EntityContainer<Entity_t>::erase(const_iterator first, const_iterator last)
{
  return mEntities.erase(first, last);
}

/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Entidades 2D
 */
template<typename Entity_t>
class Entities2D
  : public EntityContainer<Entity_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Entities2D();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   * \param[in] size Tamaños que se reserva
   */
  Entities2D(typename Entities2D<Entity_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] entity Objeto Entities2D que se copia
   */
  Entities2D(const Entities2D &entity);

  /*!
   * \brief Constructor de movimiento
   * \param[in] entity Objeto Entities2D que se copia
   */
  Entities2D(Entities2D &&entity) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] entities vector de entidades
   */
  Entities2D(const std::vector<Entity_t> &entities);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] entities Inicializador de lista con las entidades
   */
  Entities2D(std::initializer_list<Entity_t> entities);

  /*!
   * \brief Devuelve las entidades que están dentro de una ventana
   * \param[in] window Ventana
   * \return Entidades seleccionadas
   */
  template<typename Window_t>
  std::vector<Entity_t> entitiesInWindow(const Window_t &window) const;

  /*!
   * \brief Asignación de copia
   */
  Entities2D<Entity_t>& operator=(const Entities2D<Entity_t> &entity);

  /*!
   * \brief Asignación de movimiento
   */
  Entities2D<Entity_t>& operator=(Entities2D<Entity_t> &&entity) TL_NOEXCEPT;

};


template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D() 
  : EntityContainer<Entity_t>(0)
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(typename Entities2D<Entity_t>::size_type size)
  : EntityContainer<Entity_t>(size) 
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(const Entities2D &entities) 
  : EntityContainer<Entity_t>(entities) 
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(Entities2D &&entities) TL_NOEXCEPT
  : EntityContainer<Entity_t>(std::forward<EntityContainer<Entity_t>>(entities))
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(const std::vector<Entity_t> &entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(std::initializer_list<Entity_t> entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t> template<typename Window_t> inline
std::vector<Entity_t> Entities2D<Entity_t>::entitiesInWindow(const Window_t &window) const
{
  std::vector<Entity_t> r_points(this->mEntities.size());
  size_t j = 0;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    if (window.containsPoint(this->mEntities[i])) {
      r_points[i] = this->mEntities[i];
      j++;
    }
  }
  r_points.resize(j);
  return r_points;
}

template<typename Entity_t> inline
Entities2D<Entity_t> &Entities2D<Entity_t>::operator=(const Entities2D<Entity_t> &entity)
{
  if (this != &entity) {
    EntityContainer<Entity_t>::operator=(entity);
  }
  return (*this);
}

template<typename Entity_t> inline
Entities2D<Entity_t> &Entities2D<Entity_t>::operator=(Entities2D<Entity_t> &&entity) TL_NOEXCEPT
{
  if (this != &entity) {
    EntityContainer<Entity_t>::operator =(std::forward<EntityContainer<Entity_t>>(entity));
  }
  return (*this);
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Entidades 3D
 */
template<typename Entity_t>
class Entities3D
  : public EntityContainer<Entity_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Entities3D();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   * \param[in] size Tamaño que se reserva
   * \see entity_type
   */
  Entities3D(typename EntityContainer<Entity_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] entity Entidad que se copia
   * \see entity_type
   */
  Entities3D(const Entities3D &entity);
  
  /*!
   * \brief Constructor de movimiento
   * \param[in] entity Entidad que se copia
   * \see entity_type
   */
  Entities3D(Entities3D &&entity) TL_NOEXCEPT;

  /*!
   * \brief Constructor a partir de un vector de entidades
   * \param[in] entities Vector de entidades
   */
  Entities3D(const std::vector<Entity_t> &entities);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] entities Entidades
   */
  Entities3D(std::initializer_list<Entity_t> entities);
  
  /*!
   * \brief Devuelve los puntos que esta dentro de un cuadro delimitador
   * \param[in] bbox Cuadro delimitador
   * \return Puntos que entran dentro de la caja
   */
  std::vector<Entity_t> entitiesInBoundingBox(const BoundingBox<Entity_t> &bbox) const;

  /*!
   * \brief Caja envolvente
   * \return Caja envolvente de los puntos
   */
  //virtual BoundingBox<Entity_t> boundingBox() const;

  /*!
   * \brief Asignación de copia
   */
  Entities3D<Entity_t>& operator=(const Entities3D<Entity_t> &entity);

  /*!
   * \brief Asignación de movimiento
   */
  Entities3D<Entity_t>& operator=(Entities3D<Entity_t> &&entity) TL_NOEXCEPT;

};


template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D() 
  : EntityContainer<Entity_t>() 
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(typename EntityContainer<Entity_t>::size_type size)
  : EntityContainer<Entity_t>(size) 
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(const Entities3D &entities) 
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(Entities3D &&entities) TL_NOEXCEPT
  : EntityContainer<Entity_t>(std::forward<EntityContainer<Entity_t>>(entities))
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(const std::vector<Entity_t> &entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(std::initializer_list<Entity_t> entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t> inline
std::vector<Entity_t> Entities3D<Entity_t>::entitiesInBoundingBox(const BoundingBox<Entity_t> &bbox) const
{
  std::vector<Entity_t> r_points(this->mEntities.size());
  size_t j = 0;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    if (bbox.containsPoint(this->mEntities[i])) {
      r_points[i] = this->mEntities[i];
      j++;
    }
  }
  r_points.resize(j);
  return r_points;
}

//template<typename Entity_t> inline
//BoundingBox<Entity_t> Entities3D<Entity_t>::boundingBox() const
//{
//  BoundingBox<Entity_t> bounding_box;
//  for (size_t i = 0; i < this->mEntities.size(); i++) {
//    if (bounding_box.pt1.x > this->mEntities[i].x) bounding_box.pt1.x = this->mEntities[i].x;
//    if (bounding_box.pt1.y > this->mEntities[i].y) bounding_box.pt1.y = this->mEntities[i].y;
//    if (bounding_box.pt1.z > this->mEntities[i].z) bounding_box.pt1.z = this->mEntities[i].z;
//    if (bounding_box.pt2.x < this->mEntities[i].x) bounding_box.pt2.x = this->mEntities[i].x;
//    if (bounding_box.pt2.y < this->mEntities[i].y) bounding_box.pt2.y = this->mEntities[i].y;
//    if (bounding_box.pt2.z < this->mEntities[i].z) bounding_box.pt2.z = this->mEntities[i].z;
//  }
//  return bounding_box;
//}

template<typename Entity_t> inline
Entities3D<Entity_t> &Entities3D<Entity_t>::operator=(const Entities3D<Entity_t> &entity)
{
  if (this != &entity) {
    EntityContainer<Entity_t>::operator=(entity);
  }
  return (*this);
}

template<typename Entity_t> inline
Entities3D<Entity_t> &Entities3D<Entity_t>::operator=(Entities3D<Entity_t> &&entity) TL_NOEXCEPT
{
  if (this != &entity) {
    EntityContainer<Entity_t>::operator =(std::forward<EntityContainer<Entity_t>>(entity));
  }
  return (*this);
}


/*! \} */ // end of GeometricEntities

} // End namespace tl

#endif // TL_GEOMETRY_ENTITY_H
