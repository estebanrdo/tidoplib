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

#include "robustmatch.h"

#include "tidop/core/messages.h"
#include "tidop/core/exception.h"

#ifdef HAVE_OPENCV_XFEATURES2D
#include <opencv2/xfeatures2d.hpp>
#endif // HAVE_OPENCV_XFEATURES2D
#include <opencv2/calib3d.hpp>

namespace tl
{

RobustMatchingProperties::RobustMatchingProperties()
  : mRatio(0.8),
    mCrossCheck(true),
    mGeometricTest(GeometricTest::fundamental),
    mHomographyComputeMethod(HomographyComputeMethod::ransac),
    mFundamentalComputeMethod(FundamentalComputeMethod::ransac),
    mEssentialComputeMethod(EssentialComputeMethod::ransac),
    mDistance(0.7),
    mConfidence(0.999),
    mMaxIters(2000)
{
}

double RobustMatchingProperties::ratio() const
{
  return mRatio;
}

void RobustMatchingProperties::setRatio(double ratio)
{
  mRatio = ratio;
}

bool RobustMatchingProperties::crossCheck() const
{
  return mCrossCheck;
}

void RobustMatchingProperties::setCrossCheck(bool crossCheck)
{
  mCrossCheck = crossCheck;
}

RobustMatcher::GeometricTest RobustMatchingProperties::geometricTest() const
{
  return mGeometricTest;
}

void RobustMatchingProperties::setGeometricTest(RobustMatcher::GeometricTest geometricTest)
{
  mGeometricTest = geometricTest;
}

RobustMatcher::HomographyComputeMethod RobustMatchingProperties::homographyComputeMethod() const
{
  return mHomographyComputeMethod;
}

void RobustMatchingProperties::setHomographyComputeMethod(RobustMatcher::HomographyComputeMethod computeMethod)
{
  mHomographyComputeMethod = computeMethod;
}

RobustMatcher::FundamentalComputeMethod RobustMatchingProperties::fundamentalComputeMethod() const
{
  return mFundamentalComputeMethod;
}

void RobustMatchingProperties::setFundamentalComputeMethod(RobustMatcher::FundamentalComputeMethod computeMethod)
{
  mFundamentalComputeMethod = computeMethod;
}

RobustMatcher::EssentialComputeMethod RobustMatchingProperties::essentialComputeMethod() const
{
  return mEssentialComputeMethod;
}

void RobustMatchingProperties::setEssentialComputeMethod(RobustMatcher::EssentialComputeMethod computeMethod)
{
  mEssentialComputeMethod = computeMethod;
}

double RobustMatchingProperties::distance() const
{
  return mDistance;
}

void RobustMatchingProperties::setDistance(double distance)
{
  mDistance = distance;
}

double RobustMatchingProperties::confidence() const
{
  return mConfidence;
}

void RobustMatchingProperties::setConfidence(double confidence)
{
  mConfidence = confidence;
}

int RobustMatchingProperties::maxIter() const
{
  return mMaxIters;
}

void RobustMatchingProperties::setMaxIters(int maxIter)
{
  mMaxIters = maxIter;
}

void RobustMatchingProperties::reset()
{
  mRatio = 0.8;
  mCrossCheck = true;
  mGeometricTest = GeometricTest::fundamental;
  mHomographyComputeMethod = HomographyComputeMethod::ransac;
  mFundamentalComputeMethod = FundamentalComputeMethod::ransac;
  mEssentialComputeMethod = EssentialComputeMethod::ransac;
  mDistance = 0.7;
  mConfidence = 0.999;
  mMaxIters = 2000;
}

std::string RobustMatchingProperties::name() const
{
  return std::string("Robust Matcher");
}



/*----------------------------------------------------------------*/



RobustMatchingImp::RobustMatchingImp(std::shared_ptr<DescriptorMatcher> descriptorMatcher)
  : mDescriptorMatcher(descriptorMatcher)
{

}

RobustMatchingImp::RobustMatchingImp(std::shared_ptr<DescriptorMatcher> descriptorMatcher,
                                     double ratio,
                                     bool crossCheck,
                                     GeometricTest geometricTest,
                                     HomographyComputeMethod homographyComputeMethod,
                                     FundamentalComputeMethod fundamentalComputeMethod,
                                     EssentialComputeMethod essentialComputeMethod,
                                     double distance,
                                     double confidence,
                                     int maxIter)
  : mDescriptorMatcher(descriptorMatcher)
{
  this->setRatio(ratio);
  this->setCrossCheck(crossCheck);
  this->setGeometricTest(geometricTest);
  this->setHomographyComputeMethod(homographyComputeMethod);
  this->setFundamentalComputeMethod(fundamentalComputeMethod);
  this->setEssentialComputeMethod(essentialComputeMethod);
  this->setDistance(distance);
  this->setConfidence(confidence);
  this->setMaxIters(maxIter);
}

void RobustMatchingImp::setDescriptorMatcher(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher)
{
  mDescriptorMatcher = descriptorMatcher;
}

std::vector<cv::DMatch> RobustMatchingImp::geometricFilter(const std::vector<cv::DMatch> &matches, const std::vector<cv::KeyPoint> &keypoints1, const std::vector<cv::KeyPoint> &keypoints2, std::vector<cv::DMatch> *wrongMatches)
{
  std::vector<cv::DMatch> filter_matches;

  // Convert keypoints into Point2f
  size_t nPoints = matches.size();
  std::vector<cv::Point2f> pts1(nPoints);
  std::vector<cv::Point2f> pts2(nPoints);
  for (size_t igm = 0; igm < nPoints; igm++) {
    pts1[igm] = keypoints1[static_cast<size_t>(matches[igm].queryIdx)].pt;
    pts2[igm] = keypoints2[static_cast<size_t>(matches[igm].trainIdx)].pt;
  }

  TL_TODO("Sería mejor 3 clases GeometricTest (GeometricTestHomography, GeometricTestFundamental y GeometricTestEssential)")
  RobustMatcher::GeometricTest geometric_test = RobustMatchingProperties::geometricTest();
  if (geometric_test == RobustMatcher::GeometricTest::essential) {


  } else if (geometric_test == RobustMatcher::GeometricTest::homography){

    filter_matches = filterByHomographyMatrix(matches, pts1, pts2, wrongMatches);

  } else if (geometric_test == RobustMatcher::GeometricTest::fundamental){

    filter_matches = filterByFundamentalMatrix(matches, pts1, pts2, wrongMatches);

  }

  return filter_matches;
}

std::vector<cv::DMatch> RobustMatchingImp::filterByHomographyMatrix(const std::vector<cv::DMatch> &matches, const std::vector<cv::Point2f> &points1, const std::vector<cv::Point2f> &points2, std::vector<cv::DMatch> *wrongMatches)
{
  std::vector<cv::DMatch> filter_matches;

  int hcm = cv::RANSAC;
  RobustMatcher::HomographyComputeMethod homographyComputeMethod = this->homographyComputeMethod();
  if (homographyComputeMethod == RobustMatcher::HomographyComputeMethod::all_points){
    hcm = 0;
  } else if (homographyComputeMethod == RobustMatcher::HomographyComputeMethod::ransac){
    hcm = cv::RANSAC;
  } else if (homographyComputeMethod == RobustMatcher::HomographyComputeMethod::lmeds){
    hcm = cv::LMEDS;
  } else if (homographyComputeMethod == RobustMatcher::HomographyComputeMethod::rho){
    hcm = cv::RHO;
  }

  size_t nPoints = matches.size();
  std::vector<uchar> inliers(nPoints, 0);
  cv::Mat H = cv::findHomography(cv::Mat(points1), cv::Mat(points2), hcm, this->distance(), inliers, this->maxIter(), this->confidence());

  // extract the surviving (inliers) matches
  std::vector<uchar>::const_iterator itIn = inliers.begin();
  std::vector<cv::DMatch>::const_iterator itM = matches.begin();
  // for all matches
  for (; itIn != inliers.end(); ++itIn, ++itM) {

    if (*itIn) {
      filter_matches.push_back(*itM);
    } else {
      if (wrongMatches) wrongMatches->push_back(*itM);
    }
  }


  return filter_matches;
}

std::vector<cv::DMatch> RobustMatchingImp::filterByEssentialMatrix(const std::vector<cv::DMatch> &matches, const std::vector<cv::Point2f> &points1, const std::vector<cv::Point2f> &points2, std::vector<cv::DMatch> *wrongMatches)
{
  std::vector<cv::DMatch> filter_matches;

  int fm = cv::RANSAC;
  RobustMatcher::EssentialComputeMethod essentialComputeMethod = this->essentialComputeMethod();
  if (essentialComputeMethod == RobustMatcher::EssentialComputeMethod::ransac){
    fm = cv::RANSAC;
  } else if (essentialComputeMethod == RobustMatcher::EssentialComputeMethod::lmeds){
    fm = cv::LMEDS;
  }

  // Convert keypoints into Point2f
  size_t nPoints = matches.size();

  TL_TODO("Se necesita la calibración de la cámara (focal y pp) con lo cual no se si es interesante")

//  std::vector<uchar> inliers(nPoints, 0);
//  cv::findEssentialMat(	InputArray 	points1,
//  InputArray 	points2,
//  InputArray 	cameraMatrix,
//  int 	method = RANSAC,
//  double 	prob = 0.999,
//  double 	threshold = 1.0,
//  OutputArray 	mask = noArray()
//  )

      return filter_matches;
}

std::vector<cv::DMatch> RobustMatchingImp::filterByFundamentalMatrix(const std::vector<cv::DMatch> &matches, const std::vector<cv::Point2f> &points1, const std::vector<cv::Point2f> &points2, std::vector<cv::DMatch> *wrongMatches)
{
  int fm_method = cv::FM_RANSAC;
  RobustMatcher::FundamentalComputeMethod fundamentalComputeMethod = this->fundamentalComputeMethod();
  if (fundamentalComputeMethod == RobustMatcher::FundamentalComputeMethod::algorithm_7_point){
    fm_method = cv::FM_7POINT;
  } else if (fundamentalComputeMethod == RobustMatcher::FundamentalComputeMethod::algorithm_8_point){
    fm_method = cv::FM_8POINT;
  } else if (fundamentalComputeMethod == RobustMatcher::FundamentalComputeMethod::ransac){
    fm_method = cv::FM_RANSAC;
  } else if (fundamentalComputeMethod == RobustMatcher::FundamentalComputeMethod::lmeds){
    fm_method = cv::FM_LMEDS;
  }

  // Convert keypoints into Point2f
  size_t nPoints = matches.size();
  std::vector<uchar> inliers(nPoints, 0);
  cv::Mat fundamental = cv::findFundamentalMat(cv::Mat(points1), cv::Mat(points2), inliers,
                                               fm_method, this->distance(), this->confidence());

  std::vector<cv::DMatch> filter_matches;
  // extract the surviving (inliers) matches
  std::vector<uchar>::const_iterator itIn = inliers.begin();
  std::vector<cv::DMatch>::const_iterator itM = matches.begin();
  // for all matches
  for (; itIn != inliers.end(); ++itIn, ++itM) {

    if (*itIn) { // it is a valid match
      filter_matches.push_back(*itM);
    } else {
      if (wrongMatches) wrongMatches->push_back(*itM);
    }
  }

  return filter_matches;
}

std::vector<cv::DMatch> RobustMatchingImp::match(const cv::Mat &queryDescriptor, const cv::Mat &trainDescriptor, std::vector<cv::DMatch> *wrongMatches)
{
  if (this->crossCheck()){
    return this->robustMatch(queryDescriptor, trainDescriptor, wrongMatches);
  } else {
    return this->fastRobustMatch(queryDescriptor, trainDescriptor, wrongMatches);
  }
}

std::vector<cv::DMatch> RobustMatchingImp::robustMatch(const cv::Mat &queryDescriptor, const cv::Mat &trainDescriptor, std::vector<cv::DMatch> *wrongMatches)
{
  std::vector<cv::DMatch> goodMatches;

  try {
    std::vector<std::vector<cv::DMatch>> matches12;
    std::vector<std::vector<cv::DMatch>> matches21;

    mDescriptorMatcher->match(queryDescriptor, trainDescriptor, matches12);

    mDescriptorMatcher->match(trainDescriptor, queryDescriptor, matches21);

    std::vector<std::vector<cv::DMatch>> wrong_matches12;
    std::vector<std::vector<cv::DMatch>> wrong_matches21;
    std::vector<std::vector<cv::DMatch>> good_matches12 = RobustMatchingImp::ratioTest(matches12, this->ratio(), &wrong_matches12);
    std::vector<std::vector<cv::DMatch>> good_matches21 = RobustMatchingImp::ratioTest(matches21, this->ratio(), &wrong_matches21);

    matches12.clear();
    matches21.clear();

    if (wrongMatches) {
      for (auto &wrong_match : wrong_matches12) {
        wrongMatches->push_back(wrong_match[0]);
      }
    }

    goodMatches = RobustMatchingImp::crossCheckTest(good_matches12, good_matches21, wrongMatches);

  } catch (...) {
    std::throw_with_nested(std::runtime_error("RobustMatchingImp::robustMatch() failed"));
  }

  return goodMatches;
}

std::vector<cv::DMatch> RobustMatchingImp::fastRobustMatch(const cv::Mat &queryDescriptor,
                                                           const cv::Mat &trainDescriptor,
                                                           std::vector<cv::DMatch> *wrongMatches)
{
  std::vector<cv::DMatch> goodMatches;

  try {

    std::vector<std::vector<cv::DMatch>> matches;
    mDescriptorMatcher->match(queryDescriptor, trainDescriptor, matches);

    std::vector<std::vector<cv::DMatch>> ratio_test_wrong_matches;
    std::vector<std::vector<cv::DMatch>> ratio_test_matches = RobustMatchingImp::ratioTest(matches, this->ratio(), &ratio_test_wrong_matches);

    for (auto &match : ratio_test_matches){
      goodMatches.push_back(match[0]);
    }

    if (wrongMatches) {
      for (auto &wrong_match : ratio_test_wrong_matches){
        wrongMatches->push_back(wrong_match[0]);
      }
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
  }

  return goodMatches;
}

bool RobustMatchingImp::compute(const cv::Mat &queryDescriptor,
                                const cv::Mat &trainDescriptor,
                                const std::vector<cv::KeyPoint> &keypoints1,
                                const std::vector<cv::KeyPoint> &keypoints2,
                                std::vector<cv::DMatch> *goodMatches,
                                std::vector<cv::DMatch> *wrongMatches,
                                const cv::Size &queryImageSize,
                                const cv::Size &trainImageSize)
{
  try {
    *goodMatches = this->match(queryDescriptor, trainDescriptor, wrongMatches);
    *goodMatches = this->geometricFilter(*goodMatches, keypoints1, keypoints2, wrongMatches);
    return false;
  } catch(std::exception &e){
    msgError(e.what());
    return true;
  }
}

} // namespace tl
