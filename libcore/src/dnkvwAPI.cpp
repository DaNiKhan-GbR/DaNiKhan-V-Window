#include "dnkvw/dnkvw.h"
#include <opencv2/opencv.hpp>

cv::VideoCapture cap;

extern "C" {

    void openVideo()
    {
        cap.open(0);
        cv::namedWindow("test",cv::WINDOW_AUTOSIZE);  
    }

    void captureVideoFrame()
    {
        cv::Mat frame;
        cap >> frame;
        cv::imshow("test", frame);
    }

    void stopVideo()
    {
        cap.release();
    }
}
