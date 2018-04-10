/*
 La ventana o bbox se tendria que recalcular cada vez que se modifica
 para no tener que recorrer todos los puntos si queremos obtener las dimensiones
 del elemento.
*/

#ifndef TL_GEOM_POLYGON_H
#define TL_GEOM_POLYGON_H

#include "config_tl.h"

//#include "opencv2/core/core.hpp"

#include "core/defs.h"
#include "core/utils.h"
#include "core/mathutils.h"
#include "geometry/entities/entity.h"
#include "geometry/entities/window.h"
#include "geometry/entities/point.h"
#include "geometry/operations.h"

namespace TL
{

/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */


namespace geometry
{

/*!
 * \brief Clase poligono 2D
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef Polygon<Point<int>> PolygonI;
 * typedef Polygon<Point<double>> PolygonD;
 * typedef Polygon<Point<float>> PolygonF;
 * \endcode
 */
template<typename Point_t>
class Polygon : public Entity, public Entities2D<Point_t>
{

//public:
//
//  typedef Point_t value_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  Polygon();

  /*!
   * \brief Constructor que establece el tamaño del poligono
   */
  Polygon(size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] polygon
   */
  Polygon(const Polygon &polygon);

  /*!
   * \brief Constructor
   * \param[in] points
   */
  Polygon(const std::vector<Point_t> &points);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  Polygon(std::initializer_list<Point_t> listPoints);

  /*!
   * \brief Destructora
   */
  ~Polygon() {}

  /*!
   * \brief Area de un poligono simple
   * http://mathworld.wolfram.com/PolygonArea.html
   * 
   * \return Area
   */
  double area() const;

  /*!
   * \brief Comprueba si un punto esta dentro del poligono
   * \param[in] point Punto
   */
  bool isInner(const Point_t &point) const;
  
  /*!
   * \brief Perimetro del poligono
   * \return Perimetro del poligono
   */
  //double perimeter() const;
  double length() const;
};

// Definición de métodos

template<typename Point_t> inline
Polygon<Point_t>::Polygon() 
  : Entity(Entity::type::POLYGON_2D), 
    Entities2D<Point_t>() 
{
}

template<typename Point_t> inline
Polygon<Point_t>::Polygon(size_type size) 
  : Entity(Entity::type::POLYGON_2D), 
    Entities2D<Point_t>(size) 
{
}

template<typename Point_t> inline
Polygon<Point_t>::Polygon(const Polygon &polygon) 
  : Entity(Entity::type::POLYGON_2D), 
    Entities2D<Point_t>(polygon) 
{
}

template<typename Point_t> inline
Polygon<Point_t>::Polygon(const std::vector<Point_t> &points) 
  : Entity(Entity::type::POLYGON_2D), 
    Entities2D<Point_t>(points) 
{
}

template<typename Point_t> inline
Polygon<Point_t>::Polygon(std::initializer_list<Point_t> listPoints) 
  : Entity(Entity::type::POLYGON_2D), 
    Entities2D<Point_t>(listPoints) 
{
}

template<typename Point_t> inline
bool Polygon<Point_t>::isInner(const Point_t &point) const
{
  Window<Point_t> w = getWindow();
  // Comprueba si esta dentro de la ventana envolvente.
  if (w.containsPoint(point) == false) return false;

  // Se comprueba si el punto es uno de los vertices
  for (int i = 0; i < mEntities.size(); i++) {
    // Por ahora se devuelve true. Lo suyo sería indicar que es un vertice.
    if (mEntities[i] == point) return true;
  }

  Segment<Point_t> sPointH(point, Point_t(w.pt2.x, point.y));

  // Comprueba si corta una recta que pase por el punto corta en un numero 
  // par al poligono. La recta va desde el punto a la parte inferior, superior,
  // derecha o izquierda de la ventana.
  //... Se suma uno porque Mat comienza en cero. Se podria hacer una conversion a cv::Rect 
  //cv::Mat aux(w.getHeight()+1, w.getWidth()+1, CV_8UC3);
  //cv::line(aux, point-w.pt1, point-w.pt1, Color(Color::NAME::Red).get<cv::Scalar>(), 1);
  int nIntersection = 0;
  bool bVertex = false;
  std::vector<int> vertex_id;

  for (size_t i = 0, j = 1; i < mEntities.size(); i++, j++) {
    if (j == mEntities.size()) j = 0;


    Segment<Point_t> segment(mEntities[i], mEntities[j]);
    
    // El punto es colineal con el segmento y esta dentro del mismo.
    if (distPointToSegment(point, segment) == 0) return true;

    Point_t ptp;
    //if (projectPointInSegment(segment, point, &ptp) == 2) return true; // esta en la linea

      
    if (point.y == segment.pt1.y) {
      vertex_id.push_back(static_cast<int>(i));
      bVertex = true;
    }

    if (point.y == segment.pt1.y && point.y == segment.pt2.y) {
      // los dos segmentos forman parte de la misma linea horizontal
      // como no va a aparecer intersección...
      //nIntersection += 2;
    } else {
      nIntersection += intersectSegments(segment, sPointH, &ptp);
    }

  }

  // hay vertices
  // Si hay vertices hay que estudiar mas en detalle si el punto esta dentro o fuera
  if (bVertex == true) {
    std::vector<int> order;
    int vertex_prev = 0;
    int vertex_next = 0;
    for (int i = 0; i < vertex_id.size(); i++) {
      // Se comprueban los puntos anterior y siguiente
      if (vertex_id[i] == 0) vertex_prev = static_cast<int>(mEntities.size()) - 1;
      else vertex_prev = vertex_id[i] - 1;

      if (vertex_id[i] == mEntities.size() - 1) vertex_next = 0;
      else vertex_next = vertex_id[i] + 1;

      if (mEntities[vertex_prev].y == sPointH.pt1.y) {
        continue;
      } else {
        if (mEntities[vertex_next].y == sPointH.pt1.y) {
          int prev = isLeft(sPointH.pt1, sPointH.pt2, mEntities[vertex_prev]);
          if (vertex_next == mEntities.size() - 1) vertex_next = 0;
          else vertex_next = vertex_next + 1;
          int next = isLeft(sPointH.pt1, sPointH.pt2, mEntities[vertex_next]);
          if (prev == next) nIntersection -= 2;
          else nIntersection--;
        } else {
          int prev = isLeft(sPointH.pt1, sPointH.pt2, mEntities[vertex_prev]);
          int next = isLeft(sPointH.pt1, sPointH.pt2, mEntities[vertex_next]);
          if (prev == next) nIntersection -= 2;
          else nIntersection--;
        }

      }

    }
  }
  if (nIntersection < 0) return false;
  if (nIntersection % 2 == 0) return false;
  else return true;

  //// Probar con OpenCV
  //std::vector<cv::Point2f> points;
  //for (int i = 0; i < mEntities.size(); i++) {
  //  points.push_back(cv::Point2f(mEntities[i].x, mEntities[i].y));
  //}
  //double i_pol_test = cv::pointPolygonTest(mEntities, cv::Point2f(point.x, point.y), false);
  //if (i_pol_test == -1) return false;
  //else return true;

}

template<typename Point_t> inline
double Polygon<Point_t>::length()  const
{
  double perimeter = 0.;
  for (size_t i = 1; i < mEntities.size(); i++) {
    perimeter += distance(mEntities[i-1], mEntities[i]);
  }
  return perimeter;
}

template<typename Point_t> inline
double Polygon<Point_t>::area() const
{
  //TODO: Si el poligono es complejo hay que determinarla de otra forma. Primero hay que ver que sea complejo
  double area = 0.;
  for (size_t i = 1; i < mEntities.size(); i++) {
    area += crossProduct(mEntities[i-1], mEntities[i]);
  }
  return area / 2.;
}

typedef Polygon<Point<int>> PolygonI;
typedef Polygon<Point<double>> PolygonD;
typedef Polygon<Point<float>> PolygonF;



/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Clase poligono 3D
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 *
 * typedef Polygon3D<int> Polygon3dI;
 * typedef Polygon3D<double> Polygon3dD;
 * typedef Polygon3D<float> Polygon3dF;
 *
 */
template<typename Point3_t>
class Polygon3D : public Entity, public Entities3D<Point3_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Polygon3D();

  /*!
   * \brief Constructor que establece el tamaño del poligono
   */
  Polygon3D(size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] polygon
   */
  Polygon3D(const Polygon3D &polygon);

  /*!
   * \brief Constructor
   * \param[in] points
   */
  Polygon3D(const std::vector<Point3_t> &points);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  Polygon3D(std::initializer_list<Point3_t> listPoints);

  /*!
   * \brief Perimetro del poligono
   * \return Perimetro del poligono
   */
  //double perimeter() const;
  double length() const;
};

template<typename Point3_t> inline
Polygon3D<Point3_t>::Polygon3D() 
  : Entity(Entity::type::POLYGON_3D), 
    Entities3D<Point3_t>() 
{
}

template<typename Point3_t> inline
Polygon3D<Point3_t>::Polygon3D(size_type size) 
  : Entity(Entity::type::POLYGON_3D), 
    Entities3D<Point3_t>(size) 
{
}

template<typename Point3_t> inline
Polygon3D<Point3_t>::Polygon3D(const Polygon3D &polygon) 
  : Entity(Entity::type::POLYGON_3D), 
    Entities3D<Point3_t>(polygon.mEntities)
{
}

template<typename Point3_t> inline
Polygon3D<Point3_t>::Polygon3D(const std::vector<Point3_t> &points) 
  : Entity(Entity::type::POLYGON_3D), 
    Entities3D<Point3_t>(vPoint) 
{
}

template<typename Point3_t> inline
Polygon3D<Point3_t>::Polygon3D(std::initializer_list<Point3_t> listPoints) 
  : Entity(Entity::type::POLYGON_3D), 
    Entities3D<Point3_t>(listPoints) 
{
}

template<typename Point3_t> inline
double Polygon3D<Point3_t>::length()  const
{
  double perimeter = 0.;
  for (size_t i = 1; i < mEntities.size(); i++) {
    perimeter += distance(mEntities[i-1], mEntities[i]);
  }
  return perimeter;
}

typedef Polygon3D<Point3<int>> Polygon3dI;
typedef Polygon3D<Point3<double>> Polygon3dD;
typedef Polygon3D<Point3<float>> Polygon3dF;


/* ---------------------------------------------------------------------------------- */

template <typename Point_t>
class MultiPolygon : public Entity, public Entities2D<Polygon<Point_t>>
{

public:

  MultiPolygon();
  ~MultiPolygon() {}


};

template <typename Point_t>
MultiPolygon<Point_t>::MultiPolygon()
  : Entity(Entity::type::MULTIPOLYGON_2D),
    Entities2D<Polygon<Point_t>>()
{
}


/* ---------------------------------------------------------------------------------- */

template <typename Point3_t>
class MultiPolygon3D : public Entity, public Entities3D<Polygon3D<Point3_t>>
{

public:

  MultiPolygon3D();
  ~MultiPolygon3D() {}

};

template <typename Point3_t>
MultiPolygon3D<Point3_t>::MultiPolygon3D()
  : Entity(Entity::type::MULTIPOLYGON_3D),
    Entities3D<Polygon3D<Point3_t>>()
{
}


}

/*! \} */ // end of GeometricEntities

} // End namespace TL

#endif // TL_GEOM_POLYGON_H