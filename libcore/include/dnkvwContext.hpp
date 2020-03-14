#pragma once

#include "dnkvw/dnkvwInterface.h"
#include <opencv2/opencv.hpp>

namespace dnkvw 
{

    class CDnkvwContext : public IDnkvwContext
    {
        public:
            bool startTracking(int cameraId);
            void stopTracking();

            void debugCameraInput();
            void debugCameraFace();
            void debugCameraFaceDNN();

        private:
            cv::VideoCapture m_videoCapture;
    };

}
