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

#include "tidop/imgprocess/morphologicaloper.h"

#include "tidop/core/messages.h"

#ifdef HAVE_OPENCV
#include <opencv2/highgui.hpp>

#include <cstdarg>
#include <cstdio>


namespace tl
{

MorphologicalOperation::MorphologicalOperation(ImageProcess::ProcessType type)
  : ImageProcess(type)
{
}

void MorphologicalOperation::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image")

  cv::Mat element = getStructuringElement(mShapes, 
                                          cv::Size(2 * mSize + 1, 2 * mSize + 1), 
                                          cv::Point(mSize, mSize));
  switch (this->type())
  {
  case ImageProcess::ProcessType::morph_dilation:
    cv::dilate(matIn, matOut, element, mAnchor, mIterations, mBorderType);
    break;
  case ImageProcess::ProcessType::morph_erotion:
    cv::erode(matIn, matOut, element, mAnchor, mIterations, mBorderType);
    break;
  case ImageProcess::ProcessType::morph_opening:
    cv::morphologyEx(matIn, matOut, cv::MORPH_OPEN, element);
    break;
  case ImageProcess::ProcessType::morph_closing:
    cv::morphologyEx(matIn, matOut, cv::MORPH_CLOSE, element);
    break;
  case ImageProcess::ProcessType::morph_gradient:
    cv::morphologyEx(matIn, matOut, cv::MORPH_GRADIENT, element);
    break;
  case ImageProcess::ProcessType::morph_tophat:
    cv::morphologyEx(matIn, matOut, cv::MORPH_TOPHAT, element);
    break;
  case ImageProcess::ProcessType::morph_blackhat:
    cv::morphologyEx(matIn, matOut, cv::MORPH_BLACKHAT, element);
    break;
  default:
    throw "Incorrect image process";
  }
}

void MorphologicalOperation::setParameters(int size, 
                                           cv::MorphShapes shapes, 
                                           const cv::Point &anchor, 
                                           int iterations, 
                                           int borderType, 
                                           const cv::Scalar &borderValue)
{
  mSize = size;
  mShapes = shapes;
  mAnchor = anchor;
  mIterations = iterations;
  mBorderType = borderType;
  mBorderValue = borderValue;
}



Dilate::Dilate(int size, 
               cv::MorphShapes type, 
               const cv::Point &anchor,
               int iterations, 
               int borderType, 
               const cv::Scalar &borderValue)
  : MorphologicalOperation(ImageProcess::ProcessType::morph_dilation)
{
  setParameters(size, type, anchor, iterations, borderType, borderValue);
}





Erotion::Erotion(int size, 
                 cv::MorphShapes type, 
                 const cv::Point &anchor,
                 int iterations, 
                 int borderType, 
                 const cv::Scalar & borderValue)
  : MorphologicalOperation(ImageProcess::ProcessType::morph_erotion)
{
  setParameters(size, type, anchor, iterations, borderType, borderValue);
}



Opening::Opening(int size, 
                 cv::MorphShapes type, 
                 const cv::Point &anchor, 
                 int iterations, 
                 int borderType, 
                 const cv::Scalar &borderValue)
  : MorphologicalOperation(ImageProcess::ProcessType::morph_opening)
{
  setParameters(size, type, anchor, iterations, borderType, borderValue);
}



Closing::Closing(int size, 
                 cv::MorphShapes type, 
                 const cv::Point &anchor, 
                 int iterations, 
                 int borderType, 
                 const cv::Scalar & borderValue)
  : MorphologicalOperation(ImageProcess::ProcessType::morph_closing)
{
  setParameters(size, type, anchor, iterations, borderType, borderValue);
}



Gradient::Gradient(int size, 
                   cv::MorphShapes type, 
                   const cv::Point &anchor, 
                   int iterations, 
                   int borderType, 
                   const cv::Scalar & borderValue)
  : MorphologicalOperation(ImageProcess::ProcessType::morph_gradient)
{
  setParameters(size, type, anchor, iterations, borderType, borderValue);
}



TopHat::TopHat(int size, 
               cv::MorphShapes type, 
               const cv::Point &anchor, 
               int iterations, 
               int borderType, 
               const cv::Scalar &borderValue)
  : MorphologicalOperation(ImageProcess::ProcessType::morph_tophat)
{
  setParameters(size, type, anchor, iterations, borderType, borderValue);
}




BlackHat::BlackHat(int size, 
                   cv::MorphShapes shapes, 
                   const cv::Point &anchor, 
                   int iterations, 
                   int borderType, 
                   const cv::Scalar &borderValue)
  : MorphologicalOperation(ImageProcess::ProcessType::morph_blackhat)
{
  setParameters(size, shapes, anchor, iterations, borderType, borderValue);
}

} // End namespace tl

#endif // HAVE_OPENCV
