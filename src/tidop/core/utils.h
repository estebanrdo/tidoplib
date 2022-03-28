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

#ifndef TL_CORE_UTILS_H
#define TL_CORE_UTILS_H

#include "config_tl.h"

#include <vector>
#include <string>
#include <numeric>

#include "tidop/core/defs.h"

namespace tl
{

/*!
 * \addtogroup core
 *
 * \{
 */




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
TL_EXPORT int splitToNumbers(const std::string &cad, std::vector<int> &vOut, const char *chs = ",");

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
TL_EXPORT int splitToNumbers(const std::string &cad, std::vector<double> &vOut, const  char *chs = ",");

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
TL_EXPORT void replaceString(std::string *str, const std::string &str_old, const std::string &str_new);

/*!
 * \brief Separa una cadena
 *
 * \param[in] in Cadena de entrada
 * \param[in] chs cadena de separación. Si se omite toma por defecto ","
 * \return vector con las cadenas resultantes
 * <h4>Ejemplo</h4>
 * \code
 * char *in = "cadena1,cadena2";
 * std::vector<std::string> out;
 * split(in, out, ",")
 * \endcode
 */
TL_EXPORT std::vector<std::string> split(const std::string &in, const std::string &chs = ",");


/*! \} */ // end of stringOper

#ifdef TL_ENABLE_DEPRECATED_METHODS
/*!
 * \brief Convierte un número a una cadena de texto
 * \param[in] number Numero
 * \return Cadena de texto
 */
template <typename T> inline
TL_DEPRECATED("std::to_string()", "2.0")
std::string numberToString(T number)
{
  std::ostringstream ss;
  ss << number;
  return ss.str();
}
#endif // TL_ENABLE_DEPRECATED_METHODS


/*!
 * \brief Convierte una cadena de texto a un número
 * \param[in] text Texto
 * \return número
 */
template <typename T> inline 
T stringToNumber(const std::string &text)
{
  T number{};
  return (std::istringstream(text) >> number) ? number : 0;
}

enum class Base : int8_t
{
  octal       =  8,
  decimal     = 10,
  hexadecimal = 16
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
TL_EXPORT int stringToInteger(const std::string &text, Base base = Base::decimal);


/*!
 * \brief Ordena los indices de un vector de menor a mayor
 * Para un vector [10,20,15,5] devuelve [3,0,2,1]. El elemento mas
 * pequeño esta en la posición 3, el segundo en la posición 0, ...
 * \param[in] v Vector
 * \return Vector con los indices ordenados
 */
template <typename T> inline 
std::vector<int> sortIdx(const std::vector<T> &v)
{
  std::vector<int> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  sort(idx.begin(), idx.end(), [&v](int i1, int i2) {return v[i1] < v[i2]; });

  return idx;
}

TL_EXPORT bool compareInsensitiveCase(const std::string &source,
                                      const std::string &compare);



/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Opciones del formato
 */
class TL_EXPORT FileOptions
{
public:

  FileOptions() = default;
  virtual ~FileOptions() = default;

  virtual const char *options() = 0;
};

/*!
 * \brief Clase base para manejo de ficheros.
 *
 * Esta clase define la interfaz básica para lectura, creación y escritura de ficheros
 */
class TL_EXPORT File
{
public:

  //TODO: Revisar los modos. Igual es mejor utilizar flags
  /*!
   * \brief Modos de apertura de ficheros
   */
  enum class Mode : int8_t
  {
    read,      /*!< Lectura */
    update,    /*!< Lectura y escritura. */
    create     /*!< Creación */
  };

  /*!
   * \brief Estado
   */
  enum class Status : int8_t
  {
    open_ok,
    open_fail,
    save_ok,
    success,
    failure
  };

protected:
  
  /*!
   * \brief Fichero
   */
  std::string mFile;

  Mode mMode;

public:

  File() : mFile(""), mMode(Mode::read){}
  File(std::string file, Mode mode = Mode::update) : mFile(std::move(file)), mMode(mode) { }
  virtual ~File()= default;

  /*!
   * \brief Abre un fichero especificando las opciones del formato
   * \param[in] file Fichero
   * \param[in] mode Modo de apertura
   * \param[in] options Opciones del formato
   * \return
   * \see Mode
   */
  virtual Status open(const std::string &file, Mode mode = Mode::update, FileOptions *options = nullptr) = 0;
 
  /*!
   * \brief Cierra el fichero
   */
  virtual void close() = 0;

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  virtual Status createCopy(const std::string &fileOut) = 0;
};



/*! \} */ // end of core


} // End namespace tl

#endif // TL_CORE_UTILS_H
