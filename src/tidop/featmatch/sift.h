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

#ifndef TL_FEATMATCH_SIFT_DETECTOR_DESCRIPTOR_H
#define TL_FEATMATCH_SIFT_DETECTOR_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#include <opencv2/xfeatures2d.hpp>

namespace tl
{

/*! \addtogroup Features
 * 
 *  \{
 */

/*! \addtogroup FeatureDetectorAndDescriptor
 * 
 *  \{
 */


class TL_EXPORT SiftProperties
  : public Sift
{

public:

  SiftProperties();
  SiftProperties(const SiftProperties &siftProperties);
  ~SiftProperties() override = default;

// Sift interface

public:

  int featuresNumber() const override;
  int octaveLayers() const override;
  double contrastThreshold() const override;
  double edgeThreshold() const override;
  double sigma() const override;
  void setFeaturesNumber(int featuresNumber) override;
  void setOctaveLayers(int octaveLayers) override;
  void setContrastThreshold(double contrastThreshold) override;
  void setEdgeThreshold(double edgeThreshold) override;
  void setSigma(double sigma) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  int mFeaturesNumber;
  int mOctaveLayers;
  double mContrastThreshold;
  double mEdgeThreshold;
  double mSigma;
};


/*----------------------------------------------------------------*/

#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4)) || defined OPENCV_ENABLE_NONFREE

class TL_EXPORT SiftDetectorDescriptor
  : public SiftProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

public:

  SiftDetectorDescriptor();
  SiftDetectorDescriptor(const SiftDetectorDescriptor &siftDetectorDescriptor);
  SiftDetectorDescriptor(int featuresNumber,
                         int octaveLayers,
                         double contrastThreshold,
                         double edgeThreshold,
                         double sigma);
  ~SiftDetectorDescriptor() override = default;

private:

  void update();

// KeypointDetector interface

public:

  std::vector<cv::KeyPoint> detect(const cv::Mat &img,
                                   cv::InputArray &mask = cv::noArray()) override;

// DescriptorExtractor interface

public:

  cv::Mat extract(const cv::Mat &img,
                  std::vector<cv::KeyPoint> &keyPoints) override;

// Sift interface

public:

  void setFeaturesNumber(int featuresNumber) override;
  void setOctaveLayers(int octaveLayers) override;
  void setContrastThreshold(double contrastThreshold) override;
  void setEdgeThreshold(double edgeThreshold) override;
  void setSigma(double sigma) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::xfeatures2d::SIFT> mSift;
};

#endif

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_FEATMATCH_SIFT_DETECTOR_DESCRIPTOR_H
