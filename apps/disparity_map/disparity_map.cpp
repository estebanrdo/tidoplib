#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/ximgproc/disparity_filter.hpp"
#include <iostream>
#include <string>


#include "elas.h"
#include "image.h"

using namespace cv;
using namespace cv::ximgproc;
using namespace std;

Rect computeROI(Size2i src_sz, Ptr<StereoMatcher> matcher_instance);

const String keys =
    "{help h usage ? |                  | print this message                                                }"
    "{@left          |../data/aloeL.jpg | left view of the stereopair                                       }"
    "{@right         |../data/aloeR.jpg | right view of the stereopair                                      }"
    "{GT             |../data/aloeGT.png| optional ground-truth disparity (MPI-Sintel or Middlebury format) }"
    "{dst_path       |None              | optional path to save the resulting filtered disparity map        }"
    "{dst_raw_path   |None              | optional path to save raw disparity map before filtering          }"
    "{algorithm      |bm                | stereo matching method (bm, sgbm or elas)                         }"
    "{filter         |wls_conf          | used post-filtering (wls_conf or wls_no_conf)                     }"
    "{no-display     |                  | don't display results                                             }"
    "{no-downscale   |                  | force stereo matching on full-sized views to improve quality      }"
    "{dst_conf_path  |None              | optional path to save the confidence map used in filtering        }"
    "{vis_mult       |1.0               | coefficient used to scale disparity map visualizations            }"
    "{max_disparity  |160               | parameter of stereo matching                                      }"
    "{window_size    |-1                | parameter of stereo matching                                      }"
    "{wls_lambda     |8000.0            | parameter of post-filtering                                       }"
    "{wls_sigma      |1.5               | parameter of post-filtering                                       }"
    ;

//C:\Users\Tido\Pictures\vlcsnap-2016-09-26-11h53m55s447.png C:\Users\Tido\Pictures\vlcsnap-2016-09-26-11h54m49s920.png -algorithm=sgbm -no-downscale -dst_path=C:\Users\Tido\Pictures\vt1.png
//C:\Users\Tido\Pictures\vlcsnap-2016-09-26-11h54m06s036.png C:\Users\Tido\Pictures\vlcsnap-2016-09-26-11h55m03s330.png -algorithm=sgbm -no-downscale -dst_path=C:\Users\Tido\Pictures\vt2.png
//C:\Users\Tido\Pictures\vlcsnap-2016-09-26-11h54m33s634.png C:\Users\Tido\Pictures\vlcsnap-2016-09-26-11h55m03s330.png -algorithm=sgbm -no-downscale -dst_path=C:\Users\Tido\Pictures\vt3.png
//D:\Desarrollo\Libs\opencv\sources\samples\data\aloeL.jpg D:\Desarrollo\Libs\opencv\sources\samples\data\aloeR.jpg -GT=D:\Desarrollo\Libs\opencv\sources\samples\data\aloeGT.png -algorithm=sgbm -no-downscale
static void LoadCameraParams(std::string &file, Size &imageSize, Mat &cameraMatrix, Mat& distCoeffs)
{
  FileStorage fs(file, FileStorage::READ);
  fs["image_width"] >> imageSize.width;
  fs["image_height"] >> imageSize.height;
  fs["camera_matrix"] >> cameraMatrix;
  fs["distortion_coefficients"] >> distCoeffs;
  fs.release();
}

int main(int argc, char** argv)
{
    CommandLineParser parser(argc,argv,keys);
    parser.about("Disparity Filtering Demo");
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    String left_im = parser.get<String>(0);
    String right_im = parser.get<String>(1);
    String GT_path = parser.get<String>("GT");

    String dst_path = parser.get<String>("dst_path");
    String dst_raw_path = parser.get<String>("dst_raw_path");
    String dst_conf_path = parser.get<String>("dst_conf_path");
    String algo = parser.get<String>("algorithm");
    String filter = parser.get<String>("filter");
    bool no_display = parser.has("no-display");
    bool no_downscale = parser.has("no-downscale");
    int max_disp = parser.get<int>("max_disparity");
    double lambda = parser.get<double>("wls_lambda");
    double sigma  = parser.get<double>("wls_sigma");
    double vis_mult = parser.get<double>("vis_mult");

    int wsize;
    if(parser.get<int>("window_size")>=0) //user provided window_size value
        wsize = parser.get<int>("window_size");
    else
    {
        if(algo=="sgbm")
            wsize = 3; //default window size for SGBM
        else if(!no_downscale && algo=="bm" && filter=="wls_conf")
            wsize = 7; //default window size for BM on downscaled views (downscaling is performed only for wls_conf)
        else
            wsize = 15; //default window size for BM on full-sized views
    }

    if (!parser.check())
    {
        parser.printErrors();
        return -1;
    }

    //! [load_views]
    Mat left  = imread(left_im ,IMREAD_COLOR);
    if ( left.empty() )
    {
        cout<<"Cannot read image file: "<<left_im;
        return -1;
    }

    Mat right = imread(right_im,IMREAD_COLOR);
    if ( right.empty() )
    {
        cout<<"Cannot read image file: "<<right_im;
        return -1;
    }
    //! [load_views]

    bool noGT;
    Mat GT_disp;
    if (GT_path=="../data/aloeGT.png" && left_im!="../data/aloeL.jpg")
        noGT=true;
    else
    {
        noGT=false;
        if(readGT(GT_path,GT_disp)!=0)
        {
            cout<<"Cannot read ground truth image file: "<<GT_path<<endl;
            return -1;
        }
    }

    Mat left_for_matcher, right_for_matcher;
    Mat left_disp,right_disp;
    Mat filtered_disp;
    Mat conf_map = Mat(left.rows,left.cols,CV_8U);
    conf_map = Scalar(255);
    Rect ROI;
    Ptr<DisparityWLSFilter> wls_filter;
    double matching_time, filtering_time;
    if(max_disp<=0 || max_disp%16!=0)
    {
        cout<<"Incorrect max_disparity value: it should be positive and divisible by 16";
        return -1;
    }
    if(wsize<=0 || wsize%2!=1)
    {
        cout<<"Incorrect window_size value: it should be positive and odd";
        return -1;
    }
    Size imageSize;
    Mat cameraMatrix, distCoeffs;

    if(filter=="wls_conf") // filtering with confidence (significantly better quality than wls_no_conf)
    {    
      
      std::string file = "D://Esteban//Ingenio3000//Imagenes_Para_Calibracion_GoPro//video_1280x720//out_camera_data.xml";
      LoadCameraParams(file, imageSize, cameraMatrix, distCoeffs);
      Mat rleft, rright, map1, map2;
      Mat optCameraMat = getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0);
      initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), optCameraMat, imageSize, CV_16SC2, map1, map2);
      remap(left, left, map1, map2, INTER_LINEAR);
      remap(right, right, map1, map2, INTER_LINEAR);

        if(!no_downscale)
        {
            // downscale the views to speed-up the matching stage, as we will need to compute both left
            // and right disparity maps for confidence map computation
            //! [downscale]
            max_disp/=2;
            if(max_disp%16!=0)
                max_disp += 16-(max_disp%16);
            resize(left ,left_for_matcher ,Size(),0.5,0.5);
            resize(right,right_for_matcher,Size(),0.5,0.5);
            //! [downscale]
        }
        else
        {
            left_for_matcher  = left.clone();
            right_for_matcher = right.clone();
        }

        if(algo=="bm")
        {
            //! [matching]
            Ptr<StereoBM> left_matcher = StereoBM::create(max_disp,wsize);
            wls_filter = createDisparityWLSFilter(left_matcher);
            Ptr<StereoMatcher> right_matcher = createRightMatcher(left_matcher);

            cvtColor(left_for_matcher,  left_for_matcher,  COLOR_BGR2GRAY);
            cvtColor(right_for_matcher, right_for_matcher, COLOR_BGR2GRAY);

            matching_time = (double)getTickCount();
            left_matcher-> compute(left_for_matcher, right_for_matcher,left_disp);
            right_matcher->compute(right_for_matcher,left_for_matcher, right_disp);
            matching_time = ((double)getTickCount() - matching_time)/getTickFrequency();
            //! [matching]
        }
        else if(algo=="sgbm")
        {
            Ptr<StereoSGBM> left_matcher  = StereoSGBM::create(0,max_disp,wsize);
            left_matcher->setP1(24*wsize*wsize);
            left_matcher->setP2(96*wsize*wsize);
            left_matcher->setPreFilterCap(63);
            left_matcher->setMode(StereoSGBM::MODE_SGBM_3WAY);
            wls_filter = createDisparityWLSFilter(left_matcher);
            Ptr<StereoMatcher> right_matcher = createRightMatcher(left_matcher);

            matching_time = (double)getTickCount();
            left_matcher-> compute(left_for_matcher, right_for_matcher,left_disp);
            right_matcher->compute(right_for_matcher,left_for_matcher, right_disp);
            matching_time = ((double)getTickCount() - matching_time)/getTickFrequency();
        }
        else if (algo == "elas")
        {
          int minDisparity = 0;
          int disparityRange = max_disp;//128;
          int bd = 100;

          Elas::parameters param(Elas::MIDDLEBURY);
          param.disp_min = minDisparity;
          param.disp_max = minDisparity + disparityRange;
          param.postprocess_only_left = true;

          Elas elas(param);

          cvtColor(left_for_matcher, left_for_matcher, COLOR_BGR2GRAY);
          cvtColor(right_for_matcher, right_for_matcher, COLOR_BGR2GRAY);

          //Mat lb, rb;
          //cv::copyMakeBorder(left_for_matcher, lb, 0, 0, bd, bd, cv::BORDER_REPLICATE);
          //cv::copyMakeBorder(right_for_matcher, rb, 0, 0, bd, bd, cv::BORDER_REPLICATE);

          const cv::Size imsize = left_for_matcher.size();
          const int32_t dims[3] = { imsize.width, imsize.height, imsize.width }; // bytes per line = width

          cv::Mat leftdpf = cv::Mat::zeros(imsize, CV_32F);
          cv::Mat rightdpf = cv::Mat::zeros(imsize, CV_32F);

          elas.process(left_for_matcher.data, right_for_matcher.data, leftdpf.ptr<float>(0), rightdpf.ptr<float>(0), dims);

          //Mat disp;
          //Mat(leftdpf(cv::Rect(bd, 0, left_for_matcher.cols, left_for_matcher.rows))).copyTo(disp);
          //disp.convertTo(leftdisp, CV_16S, 16);
          //Mat(rightdpf(cv::Rect(bd, 0, left_for_matcher.cols, left_for_matcher.rows))).copyTo(disp);
          //disp.convertTo(rightdisp, CV_16S, 16);

          leftdpf.convertTo(left_disp, CV_16S, 16);
          rightdpf.convertTo(right_disp, CV_16S, 16);
        }
        else
        {
            cout<<"Unsupported algorithm";
            return -1;
        }

        if (algo != "elas") {
          //! [filtering]
          wls_filter->setLambda(lambda);
          wls_filter->setSigmaColor(sigma);
          filtering_time = (double)getTickCount();
          wls_filter->filter(left_disp, left, filtered_disp, right_disp);
          filtering_time = ((double)getTickCount() - filtering_time) / getTickFrequency();
          //! [filtering]
          conf_map = wls_filter->getConfidenceMap();

          // Get the ROI that was used in the last filter call:
          ROI = wls_filter->getROI();
          if (!no_downscale)
          {
            // upscale raw disparity and ROI back for a proper comparison:
            resize(left_disp, left_disp, Size(), 2.0, 2.0);
            left_disp = left_disp*2.0;
            ROI = Rect(ROI.x * 2, ROI.y * 2, ROI.width * 2, ROI.height * 2);
          }
        }

    }
    else if(filter=="wls_no_conf")
    {
        /* There is no convenience function for the case of filtering with no confidence, so we
        will need to set the ROI and matcher parameters manually */

        left_for_matcher  = left.clone();
        right_for_matcher = right.clone();

        if(algo=="bm")
        {
            Ptr<StereoBM> matcher  = StereoBM::create(max_disp,wsize);
            matcher->setTextureThreshold(0);
            matcher->setUniquenessRatio(0);
            cvtColor(left_for_matcher,  left_for_matcher, COLOR_BGR2GRAY);
            cvtColor(right_for_matcher, right_for_matcher, COLOR_BGR2GRAY);
            ROI = computeROI(left_for_matcher.size(),matcher);
            wls_filter = createDisparityWLSFilterGeneric(false);
            wls_filter->setDepthDiscontinuityRadius((int)ceil(0.33*wsize));

            matching_time = (double)getTickCount();
            matcher->compute(left_for_matcher,right_for_matcher,left_disp);
            matching_time = ((double)getTickCount() - matching_time)/getTickFrequency();
        }
        else if(algo=="sgbm")
        {
            Ptr<StereoSGBM> matcher  = StereoSGBM::create(0,max_disp,wsize);
            matcher->setUniquenessRatio(0);
            matcher->setDisp12MaxDiff(1000000);
            matcher->setSpeckleWindowSize(0);
            matcher->setP1(24*wsize*wsize);
            matcher->setP2(96*wsize*wsize);
            matcher->setMode(StereoSGBM::MODE_SGBM_3WAY);
            ROI = computeROI(left_for_matcher.size(),matcher);
            wls_filter = createDisparityWLSFilterGeneric(false);
            wls_filter->setDepthDiscontinuityRadius((int)ceil(0.5*wsize));

            matching_time = (double)getTickCount();
            matcher->compute(left_for_matcher,right_for_matcher,left_disp);
            matching_time = ((double)getTickCount() - matching_time)/getTickFrequency();
        }
        else
        {
            cout<<"Unsupported algorithm";
            return -1;
        }

        wls_filter->setLambda(lambda);
        wls_filter->setSigmaColor(sigma);
        filtering_time = (double)getTickCount();
        wls_filter->filter(left_disp,left,filtered_disp,Mat(),ROI);
        filtering_time = ((double)getTickCount() - filtering_time)/getTickFrequency();
    }
    else
    {
        cout<<"Unsupported filter";
        return -1;
    }

    if (algo != "elas") {
      //collect and print all the stats:
      cout.precision(2);
      cout << "Matching time:  " << matching_time << "s" << endl;
      cout << "Filtering time: " << filtering_time << "s" << endl;
      cout << endl;
    }

    double MSE_before,percent_bad_before,MSE_after,percent_bad_after;
    if(!noGT)
    {
        MSE_before = computeMSE(GT_disp,left_disp,ROI);
        percent_bad_before = computeBadPixelPercent(GT_disp,left_disp,ROI);
        MSE_after = computeMSE(GT_disp,filtered_disp,ROI);
        percent_bad_after = computeBadPixelPercent(GT_disp,filtered_disp,ROI);

        cout.precision(5);
        cout<<"MSE before filtering: "<<MSE_before<<endl;
        cout<<"MSE after filtering:  "<<MSE_after<<endl;
        cout<<endl;
        cout.precision(3);
        cout<<"Percent of bad pixels before filtering: "<<percent_bad_before<<endl;
        cout<<"Percent of bad pixels after filtering:  "<<percent_bad_after<<endl;
    }

    if(dst_path!="None")
    {
        Mat filtered_disp_vis;
        getDisparityVis(filtered_disp,filtered_disp_vis,vis_mult);
        imwrite(dst_path,filtered_disp_vis);
    }
    if(dst_raw_path!="None")
    {
        Mat raw_disp_vis;
        getDisparityVis(left_disp,raw_disp_vis,vis_mult);
        imwrite(dst_raw_path,raw_disp_vis);
    }
    if(dst_conf_path!="None")
    {
        imwrite(dst_conf_path,conf_map);
    }

    if(!no_display)
    {
        namedWindow("left", WINDOW_AUTOSIZE);
        imshow("left", left);
        namedWindow("right", WINDOW_AUTOSIZE);
        imshow("right", right);

        if(!noGT)
        {
            Mat GT_disp_vis;
            getDisparityVis(GT_disp,GT_disp_vis,vis_mult);
            namedWindow("ground-truth disparity", WINDOW_AUTOSIZE);
            imshow("ground-truth disparity", GT_disp_vis);
        }

        //! [visualization]
        Mat raw_disp_vis;
        getDisparityVis(left_disp,raw_disp_vis,vis_mult);
        namedWindow("raw disparity", WINDOW_AUTOSIZE);
        imshow("raw disparity", raw_disp_vis);
        if (algo != "elas") {
          Mat filtered_disp_vis;
          getDisparityVis(filtered_disp, filtered_disp_vis, vis_mult);
          namedWindow("filtered disparity", WINDOW_AUTOSIZE);
          imshow("filtered disparity", filtered_disp_vis);
        }
        waitKey();
        //! [visualization]
    }

    cv::Mat R, T; //... determinar
    cv::Mat R1, R2, P1, P2, Q;
    cv::stereoRectify(cameraMatrix, distCoeffs, cameraMatrix, distCoeffs, imageSize, R, T, R1, R2, P1, P2, Q);

    cv::Mat_<cv::Vec3f> XYZ(filtered_disp.rows,filtered_disp.cols);
    cv::reprojectImageTo3D(filtered_disp,XYZ,Q);

    return 0;
}

Rect computeROI(Size2i src_sz, Ptr<StereoMatcher> matcher_instance)
{
    int min_disparity = matcher_instance->getMinDisparity();
    int num_disparities = matcher_instance->getNumDisparities();
    int block_size = matcher_instance->getBlockSize();

    int bs2 = block_size/2;
    int minD = min_disparity, maxD = min_disparity + num_disparities - 1;

    int xmin = maxD + bs2;
    int xmax = src_sz.width + minD - bs2;
    int ymin = bs2;
    int ymax = src_sz.height - bs2;

    Rect r(xmin, ymin, xmax - xmin, ymax - ymin);
    return r;
}
