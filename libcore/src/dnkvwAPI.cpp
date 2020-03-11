#include "dnkvw/dnkvw.h"
#include <opencv2/opencv.hpp>

cv::VideoCapture cap;

extern "C" {

    DNKVW_API void openVideo()
    {
        cap.open(0);
        cv::namedWindow("test",cv::WINDOW_AUTOSIZE);  
    }

    DNKVW_API void captureVideoFrame()
    {
        cv::Mat frame;
        cap >> frame;
        cv::imshow("test", frame);
    }

    DNKVW_API void stopVideo()
    {
        cap.release();
    }
}
