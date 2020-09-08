#include "skeleton.h"

#include "tidop/core/utils.h"

#ifdef HAVE_OPENCV

#include <opencv2/imgproc.hpp>

#include <thread>

namespace tl
{

//void thinningZhangSuen(cv::Mat &image, cv::Mat &marker, int iter, int ini, int end) {
//  for ( int i = ini; i < end; i++ ) {
//    for ( int j = 1; j < image.cols - 1; j++ ) {
//      uchar p2 = image.at<uchar>(i - 1, j);
//      uchar p3 = image.at<uchar>(i - 1, j + 1);
//      uchar p4 = image.at<uchar>(i, j + 1);
//      uchar p5 = image.at<uchar>(i + 1, j + 1);
//      uchar p6 = image.at<uchar>(i + 1, j);
//      uchar p7 = image.at<uchar>(i + 1, j - 1);
//      uchar p8 = image.at<uchar>(i, j - 1);
//      uchar p9 = image.at<uchar>(i - 1, j - 1);
//
//      int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
//        (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
//        (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
//        (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
//      int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
//      int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
//      int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);
//
//      if ( A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0 )
//        marker.at<uchar>(i, j) = 1;
//    }
//  }
//}
//
//void thinningGuoHall(cv::Mat &image, cv::Mat &marker, int iter, int ini, int end)
//{
//  for (int i = ini; i < end; i++) {
//    for (int j = 1; j < image.cols-1; j++) {
//      uchar p2 = image.at<uchar>(i-1, j);
//      uchar p3 = image.at<uchar>(i-1, j+1);
//      uchar p4 = image.at<uchar>(i, j+1);
//      uchar p5 = image.at<uchar>(i+1, j+1);
//      uchar p6 = image.at<uchar>(i+1, j);
//      uchar p7 = image.at<uchar>(i+1, j-1);
//      uchar p8 = image.at<uchar>(i, j-1); 
//      uchar p9 = image.at<uchar>(i-1, j-1);
//      
//      int C  = (!p2 & (p3 | p4)) + (!p4 & (p5 | p6)) +
//               (!p6 & (p7 | p8)) + (!p8 & (p9 | p2));
//      int N1 = (p9 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
//      int N2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p9);
//      int N  = N1 < N2 ? N1 : N2;
//      int m  = iter == 0 ? ((p6 | p7 | !p9) & p8) : ((p2 | p3 | !p5) & p4);
//      
//      if (C == 1 && (N >= 2 && N <= 3) & (m == 0))
//        marker.at<uchar>(i,j) = 1;
//    }
//  }
//}

//void thinningIteration(cv::Mat &image, int iter, Thinning thin)
//{
//  cv::Mat marker = cv::Mat::zeros(image.size(), CV_8UC1);
//  
//  auto iteration = [&](int ini, int end) {
//    if ( thin == Thinning::GUO_HALL ) {
//      thinningGuoHall(image, marker, iter, ini, end);
//    } else {
//      thinningZhangSuen(image, marker, iter, ini, end);
//    }
//  };
//
//  int num_threads = getOptimalNumberOfThreads();
//  std::vector<std::thread> threads(num_threads);
//
//  int _size = image.rows / num_threads;
//  int ini = 0;
//  int end = 1;
//  for (int i = 0; i < num_threads; i++) {
//    ini = end;
//    end = ini + _size;
//    if (i == (num_threads - 1) && end != image.rows-1 ) end = image.rows - 1;
//    threads[i] = std::thread(iteration, ini, end);
//  }
//
//  for (auto &_thread : threads) _thread.join();
//
//  image &= ~marker;
//}

//void thinning(const cv::Mat &image, cv::Mat *out, Thinning thin)
//{
//  image.copyTo(*out);
//  if (out->channels() != 1) cv::cvtColor(*out, *out, cv::COLOR_BGR2GRAY);
//  (*out) /= 255;
//
//  cv::Mat prev = cv::Mat::zeros(image.size(), CV_8UC1);
//  cv::Mat diff;
//
//  do {
//    thinningIteration(*out, 0, thin);
//    thinningIteration(*out, 1, thin);
//
//    cv::absdiff(*out, prev, diff);
//    out->copyTo(prev);
//  } while ( cv::countNonZero(diff) > 0 );
//
//    (*out) *= 255;
//}


Thinning::Thinning(Type type)
  : ImageProcess(ProcessType::thinning), 
    mType(type)
{
}

void Thinning::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image")

  matIn.copyTo(matOut);

  if (matOut.channels() != 1) cv::cvtColor(matOut, matOut, cv::COLOR_BGR2GRAY);
  matOut /= 255;

  cv::Mat prev = cv::Mat::zeros(matIn.size(), CV_8UC1);
  cv::Mat diff;

  do {
    this->thinningIteration(matOut, 0, mType);
    this->thinningIteration(matOut, 1, mType);

    cv::absdiff(matOut, prev, diff);
    matOut.copyTo(prev);
  } while ( cv::countNonZero(diff) > 0 );

  matOut *= 255;
}

void Thinning::setType(Type type)
{
  mType = type;
}

void Thinning::thinningIteration(cv::Mat &image, int iter, Type type) const
{
  cv::Mat marker = cv::Mat::zeros(image.size(), CV_8UC1);
  
  auto iteration = [&](int ini, int end) {
    if (type == Type::guo_hall ) {
      this->thinningGuoHall(image, marker, iter, ini, end);
    } else {
      this->thinningZhangSuen(image, marker, iter, ini, end);
    }
  };

  int num_threads = getOptimalNumberOfThreads();
  std::vector<std::thread> threads(num_threads);

  int _size = image.rows / num_threads;
  int ini = 0;
  int end = 1;
  for (int i = 0; i < num_threads; i++) {
    ini = end;
    end = ini + _size;
    if (i == (num_threads - 1) && end != image.rows-1 ) end = image.rows - 1;
    threads[i] = std::thread(iteration, ini, end);
  }

  for (auto &_thread : threads) _thread.join();

  image &= ~marker;
}

void Thinning::thinningZhangSuen(cv::Mat &image, 
                                 cv::Mat &marker, 
                                 int iter, 
                                 int ini, 
                                 int end) const
{
  for ( int i = ini; i < end; i++ ) {
    for ( int j = 1; j < image.cols - 1; j++ ) {
      uchar p2 = image.at<uchar>(i - 1, j);
      uchar p3 = image.at<uchar>(i - 1, j + 1);
      uchar p4 = image.at<uchar>(i, j + 1);
      uchar p5 = image.at<uchar>(i + 1, j + 1);
      uchar p6 = image.at<uchar>(i + 1, j);
      uchar p7 = image.at<uchar>(i + 1, j - 1);
      uchar p8 = image.at<uchar>(i, j - 1);
      uchar p9 = image.at<uchar>(i - 1, j - 1);

      int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
        (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
        (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
        (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
      int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
      int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
      int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

      if ( A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0 )
        marker.at<uchar>(i, j) = 1;
    }
  }
}

void Thinning::thinningGuoHall(cv::Mat &image, 
                               cv::Mat &marker,
                               int iter, 
                               int ini, 
                               int end) const
{
  for (int i = ini; i < end; i++) {
    for (int j = 1; j < image.cols-1; j++) {
      uchar p2 = image.at<uchar>(i-1, j);
      uchar p3 = image.at<uchar>(i-1, j+1);
      uchar p4 = image.at<uchar>(i, j+1);
      uchar p5 = image.at<uchar>(i+1, j+1);
      uchar p6 = image.at<uchar>(i+1, j);
      uchar p7 = image.at<uchar>(i+1, j-1);
      uchar p8 = image.at<uchar>(i, j-1); 
      uchar p9 = image.at<uchar>(i-1, j-1);
      
      int C  = (!p2 & (p3 | p4)) + (!p4 & (p5 | p6)) +
               (!p6 & (p7 | p8)) + (!p8 & (p9 | p2));
      int N1 = (p9 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
      int N2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p9);
      int N  = N1 < N2 ? N1 : N2;
      int m  = iter == 0 ? ((p6 | p7 | !p9) & p8) : ((p2 | p3 | !p5) & p4);
      
      if (C == 1 && (N >= 2 && N <= 3) & (m == 0))
        marker.at<uchar>(i,j) = 1;
    }
  }
}

}

#endif