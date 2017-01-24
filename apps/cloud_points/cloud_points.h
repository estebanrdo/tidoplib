#ifndef I3D_CLOUD_POINTS_H
#define I3D_CLOUD_POINTS_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/optflow.hpp"


#include "img_process/img_processing.h"
#include "img_process/filters.h"
#include "feature_detection/linedetector.h"
#include "videostream.h"

namespace I3D
{

// Meter calibración de la camara y corrección
class I3D_EXPORT DetectTransmissionTower
{
public:

  std::vector<ldGroupLines> prevLinesGrops;

  double prevFrame;

  LineDetector *pLineDetector;

  ImgProcessingList imgprolist;

  cv::Ptr<cv::DenseOpticalFlow> algorithm;

  cv::Scalar c;

  cv::RNG rng;

  bool bDrawRegressionLine;
  
  bool bSaveImages;

public:

  DetectTransmissionTower(LineDetector *lineDetector) 
    : pLineDetector(lineDetector) 
  { 
    imgprolist.add(std::make_shared<I3D::Canny>());
    algorithm = cv::optflow::createOptFlow_Farneback();
    rng(12345);
    prevFrame = 0.;
    bDrawRegressionLine = false;
  }

  ~DetectTransmissionTower()
  {
  }

  void detectGroupLines(const cv::Mat &img, std::vector<ldGroupLines> *linesGroup);

  void computeOpticalFlow(const cv::Mat &img1, const cv::Mat &img2, cv::Mat_<cv::Point2f> *flow);

  bool run(const cv::Mat &img1, const cv::Mat &img2, cv::Mat *imgout, WindowI *wOut);

  bool isTower(cv::Mat *imgout, const ldGroupLines &linesGroup1, const cv::Mat &magnitude);

  void getMagnitude(const cv::Mat_<cv::Point2f> &flow, cv::Mat *magnitude);

};

} // End namespace I3D

#endif // I3D_CLOUD_POINTS_H