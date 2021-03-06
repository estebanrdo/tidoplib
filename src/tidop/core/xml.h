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

#ifndef TL_CORE_UTILS_XML_H
#define TL_CORE_UTILS_XML_H

#include "config_tl.h"

#ifdef TL_HAVE_GDAL

#include "tidop/core/defs.h"

#include <memory>
#include <mutex>

#include "tidop/core/path.h"

namespace tl
{

namespace internal
{
class XmlNode;
}

/*! \addtogroup core
 *  \{
 */

class TL_EXPORT XmlNodeValue
{

public:

  XmlNodeValue(std::string value);
  ~XmlNodeValue();

  std::string toString();
  int toInteger();
  float toFloat();
  double toDouble();

private:

  std::string mValue;
};




class TL_EXPORT XmlNode
{

public:

	XmlNode();
  XmlNode(const XmlNode &node);
	~XmlNode();

  XmlNode &next();
  XmlNode &child();
  XmlNodeValue value();

private:

  std::shared_ptr<internal::XmlNode> mNode;
};



/*!
 * \brief XMLReader
 *
 */
class TL_EXPORT XMLReader
{

public:
  
  XMLReader();
  ~XMLReader() = default;

  void parse(const tl::Path &file);

private:


};


/*!
 * \brief XMLReader
 *
 */
class TL_EXPORT XMLWriter
{

public:
  
  XMLWriter(const std::string &file);
  ~XMLWriter() = default;

};


/*! \} */ // end of core

} // End namespace tl

#endif //TL_HAVE_GDAL

#endif // TL_CORE_UTILS_GDAL_H
