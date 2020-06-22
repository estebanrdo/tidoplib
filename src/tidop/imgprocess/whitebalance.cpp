#include "whitebalance.h"

#include "tidop/core/messages.h"

#ifdef HAVE_OPENCV
#include <opencv2/highgui.hpp>

#include <cstdarg>
#include <cstdio>
#include <thread>

namespace tl
{


/* ---------------------------------------------------------------------------------- */

#if defined CV_VERSION_MAJOR && CV_VERSION_MAJOR >= 3
#  if defined CV_VERSION_MINOR && CV_VERSION_MINOR >= 2

Grayworld::Grayworld()
  : ImageProcess(ProcessType::grayworld),
    mGrayworld(cv::xphoto::createGrayworldWB())
{
}

void Grayworld::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");
  TL_ASSERT(matIn.channels() != 3, "Invalid image type. Incorrect number of channels")
  
  mGrayworld->balanceWhite(matIn, matOut);
}

#  endif // CV_VERSION_MINOR
#endif // CV_VERSION_MAJOR

/* ---------------------------------------------------------------------------------- */

WhitePatch::WhitePatch(const graph::Color & white)
  : ImageProcess(ProcessType::whitepatch),
    mWhite(white)
{
}

void WhitePatch::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");
  TL_ASSERT(matIn.channels() != 3, "Invalid image type. Incorrect number of channels")

  std::vector<cv::Mat> bgr(3);
  cv::split(matIn, bgr);
  double scale_red = this->scaleRed(bgr[2]);
  double scale_green = this->scaleGreen(bgr[1]);
  double scale_blue = this->scaleBlue(bgr[0]);
  bgr.clear();

  cv::Mat aux(matIn.size(), CV_8UC3);

  parallel_for(static_cast<size_t>(0), static_cast<size_t>(matIn.rows), [&](size_t row) {

    int r = static_cast<int>(row);
    const uchar *rgb_ptr = matIn.ptr<uchar>(r);
    for (int c = 0; c < matIn.cols; c++) {
      aux.at<cv::Vec3b>(r, c)[0] = static_cast<uchar>(rgb_ptr[3*c] * scale_red);
      aux.at<cv::Vec3b>(r, c)[1] = static_cast<uchar>(rgb_ptr[3*c+1] * scale_green);
      aux.at<cv::Vec3b>(r, c)[2] = static_cast<uchar>(rgb_ptr[3*c+2] * scale_blue);
    }
  });

  matOut = aux;
}

void WhitePatch::setWhite(const graph::Color &white)
{
  mWhite = white;
}

double WhitePatch::scaleRed(const cv::Mat &red) const
{
  double max_red;
  cv::minMaxLoc(red, nullptr, &max_red);
  double sr = mWhite.red() / max_red;
  return sr;
}

double WhitePatch::scaleGreen(const cv::Mat &green) const
{
  double max_green;
  cv::minMaxLoc(green, nullptr, &max_green);
  double sg = mWhite.green() / max_green;
  return sg;
}

double WhitePatch::scaleBlue(const cv::Mat &blue) const
{
  double max_blue;
  cv::minMaxLoc(blue, nullptr, &max_blue);
  double sb = mWhite.blue() / max_blue;
  return sb;
}

/* ---------------------------------------------------------------------------------- */


} // End namespace tl

#endif // HAVE_OPENCV
