// Visualización estereo
//
// Las matrices estan reordenadas para adaptarnos a OpenCV (orden de bandas BGR)
// INFO:
// - Least-Squares Projection Method. Eric Dubois, March 2009 
//  http://www.site.uottawa.ca/~edubois/icassp01/anaglyphdubois.pdf
//  http://www.site.uottawa.ca/~edubois/anaglyph/LeastSquaresHowToPhotoshop.pdf
//  http://www.3dtv.at/knowhow/anaglyphcomparison_en.aspx



#include "anaglyph.h"

#include "opencv2/core.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"

namespace I3D
{

struct MatrixLR {
  cv::Matx33d imgLeft;
  cv::Matx33d imgRight;
};

struct MatrixLR AnaglyphMatrix[] = {   
  // Normal
  {
    cv::Matx33d( 0, 0, 0, 
                 0, 0, 0,
                 0, 0, 1 ),
    cv::Matx33d( 1, 0, 0,
                 0, 1, 0,
                 0, 0, 0 )
  },
  // gray
  {
    cv::Matx33d( 0.0,   0.0,   0.0,
                 0.0,   0.0,   0.0,
                 0.114, 0.587, 0.299 ),
    cv::Matx33d( 0.114, 0.587, 0.299, 
                 0.114, 0.587, 0.299,
                 0.0,   0.0,   0.0 )
  },
  // Dubois
  {
    cv::Matx33d( -0.017, -0.050, -0.048, 
                 -0.024, -0.062, -0.062,
                  0.164,  0.449,  0.437),
    cv::Matx33d( 1.234, -0.093, -0.026,
                 0.009,  0.761,  0.377,
                -0.007, -0.032, -0.011)
  },
  // Half Color
  {
    cv::Matx33d( 0.0,   0.0,   0.0,
                 0.0,   0.0,   0.0,
                 0.114, 0.587, 0.299 ),
    cv::Matx33d( 1, 0, 0,
                 0, 1, 0,
                 0, 0, 0 )
  },
  // Optimized
  {
    cv::Matx33d( 0.0, 0.0, 0.0,
                 0.0, 0.0, 0.0,
                 0.3, 0.7, 0.0 ),
    cv::Matx33d( 1, 0, 0,
                 0, 1, 0,
                 0, 0, 0 )
  },
  //Red/Blue Monochrome
  {
    cv::Matx33d( 0.0,   0.0,   0.0,
                 0.0,   0.0,   0.0,
                 0.114, 0.587, 0.299 ),
    cv::Matx33d( 0.114, 0.587, 0.299, 
                 0.0,   0.0,   0.0,
                 0.0,   0.0,   0.0 )
  }
};

int getStereoImage(const cv::Mat &left, const cv::Mat &right, cv::Mat *stimage, StereoMode mode)
{

  if (left.size() != right.size()) return -1;

  int height = left.rows;
  int width = left.cols;

  if (mode == StereoMode::NORMAL) {
    // Se puede hacer con la matriz Normal
    cv::Mat in[] = { left, right };
    cv::Mat out[] = { *stimage };
    int from_to[] = { 2, 2, left.channels() + 1, 1, left.channels(), 0 };
    cv::mixChannels(in, 2, out, 1, from_to, 3);
    return 1;
  } else if (mode == StereoMode::SIDEBYSIDE) {
      cv::Size sz( width * 2, height );
      cv::resize(*stimage,*stimage,sz);
      stimage->adjustROI(0, 0, 0, -width);
      right.copyTo(*stimage);
      stimage->adjustROI(0, 0, -width, width);
      left.copyTo(*stimage);
      stimage->adjustROI(0, 0, width, 0);
      return 1;
  } else if (mode == StereoMode::NONE) {
    return -1;
  } else {
    cv::Mat imgL;
    cv::Mat imgR;
    //if (left.isContinuous())
    //{
    //    width *= height;
    //    height = 1;
    //}
    //const cv::Vec3b *pl, *pr;
    cv::Vec3b bgrPxL, bgrPxR;
    for ( int r = 0, c = 0; r < height; r++ ) {
      //pl = left.ptr<cv::Vec3b>(r);
      //pr = right.ptr<cv::Vec3b>(r);
      for ( c = 0; c < width; c++ ) {
        //bgrPxL = pl[c]; 
        //bgrPxR = pr[c];
        bgrPxL = left.at<cv::Vec3b>(r, c);
        bgrPxR = right.at<cv::Vec3b>(r, c);
        cv::Vec3b &bgrSt = stimage->at<cv::Vec3b>(r, c);
        int imode = static_cast<int>(mode);
        bgrSt = AnaglyphMatrix[imode].imgRight*bgrPxR + AnaglyphMatrix[imode].imgLeft*bgrPxL;
      }
    }
    return 1;
  }
}

} // End namespace I3D