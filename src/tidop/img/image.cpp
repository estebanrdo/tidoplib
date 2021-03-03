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

#include "image.h"


namespace tl
{

Image::Image()
  : mRows(0),
    mCols(0),
    mType(DataType::TL_8U),
    mChannels(1),
    mData(nullptr)
{
}

Image::Image(int rows, 
             int cols, 
             DataType type, 
             int channels)
  : mRows(rows),
    mCols(cols),
    mType(type),
    mChannels(channels),
    mData(nullptr)
{
  init();
}

Image::Image(const SizeI &size, 
             DataType type, 
             int channels)
  : mRows(size.height),
    mCols(size.width),
    mType(type),
    mChannels(channels),
    mData(nullptr)
{
  init();
}

Image::Image(const Image &image)
  : mRows(image.mRows),
    mCols(image.mCols),
    mType(image.mType),
    mChannels(image.mChannels),
    mData(image.mData)
{
}

Image::~Image()
{
}

Image &Image::operator=(const Image &image)
{
  if (this != &image) {
    mRows = image.mRows;
    mCols = image.mCols;
    mType = image.mType;
    mChannels = image.mChannels;
    mData = image.mData;
  }
  return *this;
}

int Image::rows() const
{
  return mRows;
}

int Image::cols() const
{
  return mCols;
}

DataType Image::type() const
{
  return mType;
}

int Image::channels() const
{
  return mChannels;
}

unsigned char *Image::data()
{
  return mData;
}

bool Image::isEmpty()
{
  return false;
}

void Image::init()
{
  //mData = (unsigned char *)std::malloc(mRows * mCols * mChannels * this->depth());
}

} // End namespace tl
