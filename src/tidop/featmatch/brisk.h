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

#ifndef TL_FEATMATCH_BRISK_DETECTOR_DESCRIPTOR_H
#define TL_FEATMATCH_BRISK_DETECTOR_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

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


class TL_EXPORT BriskProperties
  : public Brisk
{
public:

  BriskProperties();
  BriskProperties(const BriskProperties &briskProperties);
  ~BriskProperties() override = default;

// Brisk interface

public:

  int threshold() const override;
  int octaves() const override;
  double patternScale() const override;
  void setThreshold(int threshold) override;
  void setOctaves(int octaves) override;
  void setPatternScale(double patternScale) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  int mThreshold;
  int mOctaves;
  double mPatternScale;
};


/*----------------------------------------------------------------*/


class TL_EXPORT BriskDetectorDescriptor
  : public BriskProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

public:

  BriskDetectorDescriptor();
  BriskDetectorDescriptor(const BriskDetectorDescriptor &briskDetectorDescriptor);
  BriskDetectorDescriptor(int threshold,
                          int octaves,
                          double patternScale);
  ~BriskDetectorDescriptor() override = default;

protected:

  void update();


// KeypointDetector interface

public:

  std::vector<cv::KeyPoint> detect(const cv::Mat &img,
                                   cv::InputArray &mask = cv::noArray()) override;

// DescriptorExtractor interface

public:

  cv::Mat extract(const cv::Mat &img,
                  std::vector<cv::KeyPoint> &keyPoints) override;

// Brisk interface

public:

  void setThreshold(int threshold) override;
  void setOctaves(int octaves) override;
  void setPatternScale(double patternScale) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::BRISK> mBrisk;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_FEATMATCH_BRISK_DETECTOR_DESCRIPTOR_H
