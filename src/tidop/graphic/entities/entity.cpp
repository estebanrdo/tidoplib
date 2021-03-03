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

#include "tidop/graphic/entities/entity.h"

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "ogr_core.h"
#include "ogr_featurestyle.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#endif


namespace tl
{

namespace graph
{ 

/* ---------------------------------------------------------------------------------- */


GraphicEntity::GraphicEntity(Type type)
  : GraphicStyle(),
    //GData(),
    mEntityType(type)
{
}

GraphicEntity::GraphicEntity(const GraphicEntity &graphicEntity)
  : GraphicStyle(graphicEntity),
    //GData(graphicEntity),
    mEntityType(graphicEntity.mEntityType)
{
}

GraphicEntity::GraphicEntity(GraphicEntity &&graphicEntity) TL_NOEXCEPT
  : GraphicStyle(std::forward<GraphicStyle>(graphicEntity)),
    //GData(std::forward<GData>(graphicEntity)),
    mEntityType(std::move(graphicEntity.mEntityType))
{
}

GraphicEntity::~GraphicEntity()
{
}

GraphicEntity &GraphicEntity::operator = (const GraphicEntity &graphicEntity)
{ 
  if (this != &graphicEntity) {
    GraphicStyle::operator=(graphicEntity);
    //GData::operator=(graphicEntity);
    mEntityType = graphicEntity.mEntityType;
  }
  return *this;
}

GraphicEntity &GraphicEntity::operator = (GraphicEntity &&graphicEntity) TL_NOEXCEPT
{ 
  if (this != &graphicEntity) {
    GraphicStyle::operator=(std::forward<GraphicStyle>(graphicEntity));
    //GData::operator=(std::forward<GData>(graphicEntity));
    mEntityType = std::move(graphicEntity.mEntityType);
  }
  return *this;
}

GraphicEntity::Type GraphicEntity::type() const
{
  return mEntityType;
}

std::shared_ptr<TableRegister> graph::GraphicEntity::data() const
{
  return mData;
}

void GraphicEntity::setData(std::shared_ptr<TableRegister> &data)
{
  mData = data;
}


} // Fin namespace graph

} // End namespace TL
