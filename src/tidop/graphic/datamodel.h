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

#ifndef TL_DATA_MODEL_H
#define TL_DATA_MODEL_H

#include "config_tl.h"

#include <list>
#include <vector>
#include <memory>
#include <string>

#include "tidop/core/defs.h"

namespace tl
{



/*!
 * \brief Campo de la tabla
 *
 */
class TL_EXPORT TableField
{
public:

  enum class Type
  {
    INT,
    INT64,
    DOUBLE,
    FLOAT,
    STRING
    //....
  };

public:

  /*!
   * \brief Constructor
   * \param[in] name Nombre del campo
   * \param[in] type Tipo
   * \param[in] size Tamaño
   */
  TableField(const std::string &name, Type type, int size);

  ~TableField();

  /*!
   * \brief Devuelve el nombre del campo
   */
  std::string name() const;

  /*!
   * \brief Devuelve el tipo del campo
   */
  Type type() const;

  /*!
   * \brief Devuelve el tamaño del campo
   */
  int size() const;

private:

  /*!
   * \brief Nombre del campo
   */
  std::string mName;

  /*!
   * \brief Tipo del campo
   */
  Type mType;

  /*!
   * \brief Tamaño 
   */
  int mSize;
};



class TL_EXPORT RegisterValue
{

public:

  RegisterValue(const std::shared_ptr<TableField> &field);
  RegisterValue(const std::shared_ptr<TableField> &field, 
                const std::string &value);
  ~RegisterValue();

  std::string value() const;
  void setValue(const std::string &value);

private:

  std::shared_ptr<TableField> mField;
  //Por ahora cargo una cadena con el valor
  std::string mValue;
};




/*!
 * \brief Clase que representa un registro de una tabla
 */
class TL_EXPORT TableRegister
{

public:

  /*!
   * \brief Constructora por defecto
   */
  TableRegister(const std::vector<std::shared_ptr<TableField>> &fields);

  /*!
   * \brief Constructor de copia
   * \param[in] _register Objeto que se copia
   */
  TableRegister(const TableRegister &_register);
  
  //TableRegister(std::initializer_list<std::shared_ptr<TableRegisterField>> registerFields);

  /*!
   * \brief Destructora
   */
  ~TableRegister();

  std::string value(int idx) const;
  void setValue(int idx, const std::string &field);

  size_t size() const;

protected:

  /*!
   * \brief Campos de la tabla
   */
  std::vector<RegisterValue> mRegisterValues;

};


/*!
 * \brief Tabla de datos
 */
class TL_EXPORT DataTable
{
public:

  /*!
   * \brief Iterador sobre los registros de la tabla
   */
  typedef std::list<std::shared_ptr<TableRegister>>::iterator iterator;

public:

  DataTable(const std::string &tableName,
            const std::vector<std::shared_ptr<TableField>> &TableField);
  ~DataTable() {}

  /*!
   * \brief Iterador al primer registro
   */
  iterator begin();

  /*!
   * \brief Iterador al último registro
   */
  iterator end();

  ///*!
  // * \brief Añade un registro al final
  // * \param[in] _register Registro que se añade
  // */
  //void addRegister(std::shared_ptr<TableRegister> _register);

  ///*!
  // * \brief Borra un registro
  // * \param[in] index Indice del registro que se borra
  // */
  //void deleteRegister(int index);

  /*!
   * \brief Nombre de la tabla
   * \return Nombre de la tabla
   */
  std::string name() const;


  std::shared_ptr<TableRegister> createRegister(int index);

  std::vector<std::shared_ptr<TableField>> fields() const;
  
  //size_t getFieldCount() const;
  
  /*!
   * \brief Establece el nombre de la tabla
   * \param[in] name Nombre de la tabla
   */
  void setName(const char *name);

  /*!
   * \brief Establece la cabecera de la tabla
   * \param[in] tableHeader Cabecera de la tabla
   */
  //void setTableHeader(std::shared_ptr<TableHeader> tableHeader);

  size_t size();

private:

  /*!
   * \brief Nombre de la tabla
   */
  std::string mTableName;

  /*!
   * \brief Campos de la tabla
   */
  std::vector<std::shared_ptr<TableField>> mTableFields;

  /*!
   * \brief Registros de la tabla
   */
  std::list<std::shared_ptr<TableRegister>> mRegister;
  
  //size_t index;

};


/*!
 * \brief The DataModel class
 */
class TL_EXPORT DataModel
{

public:

  /*!
   * \brief Constructor por defecto
   */
  DataModel();

  /*!
   * \brief Destructor
   */
  ~DataModel();

  /*!
   * \brief create
   */
  //void create();

  /*!
   * \brief Crea una tabla nueva en el modelo de datos
   * \param[in] tableName Nombre de la tabla
   * \param[in] tableHeader Cabecera de la tabla
   * \see TableField
   */
  void createTable(const std::string &tableName,
                   const std::vector<std::shared_ptr<TableField>> &fields);

  /*!
   * \brief Añade una tabla al modelo de datos
   * \param[in] table Tabla que se añade
   */
  void addTable(std::shared_ptr<DataTable> table);

  /*!
   * \brief drop
   */
  //void drop();

  /*!
   * \brief open
   */
  //void open(std::string file);

  /*!
   * \brief read
   */
  //void read();

  /*!
   * \brief write
   */
  //void write();

  /*!
   * \brief save
   */
  //void save();

private:

  std::list<std::shared_ptr<DataTable>> mDataTables;

};


} // End namespace tl

#endif // TL_DATA_MODEL_H
