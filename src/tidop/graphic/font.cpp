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

#include "tidop/graphic/font.h"

namespace tl
{

namespace graph
{

/* ---------------------------------------------------------------------------------- */
/*                                     Clase Font                                    */
/* ---------------------------------------------------------------------------------- */

Font::Font() 
  : mName("Arial"),
    mSize(10),
    mStyle(Style::normal),
    mUnderline(false),
    mStrikethrough(false)
{
}

Font::Font(const Font &font) 
  : mName(font.mName),
    mSize(font.mSize),
    mStyle(font.mStyle),
    mUnderline(font.mUnderline),
    mStrikethrough(font.mStrikethrough)
{
}

Font::Font(Font &&font) TL_NOEXCEPT
  : mName(std::move(font.mName)),
    mSize(std::move(font.mSize)),
    mStyle(std::move(font.mStyle)),
    mUnderline(std::move(font.mUnderline)),
    mStrikethrough(std::move(font.mStrikethrough))
{
}

Font::Font(const std::string name, int size, Style style, bool underline, bool strikethrough)
  : mName(name),
    mSize(size),
    mStyle(style),
    mUnderline(underline),
    mStrikethrough(strikethrough)
{
}

std::string Font::name() const
{
  return mName;
}

void Font::setName(const std::string &name)
{
  mName = name;
}

int Font::size() const
{
  return mSize;
}

void Font::setSize(int size)
{
  mSize = size;
}

bool Font::isBold() const
{
  return mStyle.isActive(Style::bold);
}

void Font::setBold(bool active)
{
  mStyle.activeFlag(Style::bold, active);
}

bool Font::isItalic() const
{
  return mStyle.isActive(Style::italic);
}

void Font::setItalic(bool active)
{
  mStyle.activeFlag(Style::italic, active);
}

bool Font::isUnderline() const
{
  return mUnderline;
}

void Font::setUnderline(bool active)
{
  mUnderline = active;
}

bool Font::isStrikethrough() const
{
  return mStrikethrough;
}

void Font::setStrikethrough(bool active)
{
  mStrikethrough = active;
}

void Font::setStyle(Style style)
{
  mStyle = style;
}

Font &Font::operator = (const Font &font)
{
  if (this != &font) {
    this->mName = font.mName;
    this->mSize = font.mSize;
    this->mStyle = font.mStyle;
    this->mUnderline = font.mUnderline;
    this->mStrikethrough = font.mStrikethrough;
  }
  return *this;
}

Font &Font::operator = (Font &&font) TL_NOEXCEPT
{
  if (this != &font) {
    this->mName = std::move(font.mName);
    this->mSize = std::move(font.mSize);
    this->mStyle = std::move(font.mStyle);
    this->mUnderline = std::move(font.mUnderline);
    this->mStrikethrough = std::move(font.mStrikethrough);
  }
  return *this;
}

} // End namespace graph

} // End namespace TL
