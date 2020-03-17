#pragma once

#include "dnkvw/dnkvwInterface.h"
#include <opencv2/opencv.hpp>

#include "tracker.hpp"
#include "fpsTimer.hpp"
#include "vec3.hpp"

namespace dnkvw 
{

    class CDnkvwContext : public IDnkvwContext
    {
        public:
            void __init();
            void __cleanup();

            bool selectHaarTracker();
            bool selectDnnTracker();

            bool startTracking(int cameraId);
            void stopTracking();

            void calibrate();
            void loadFrustum(float near, float *left, float *right, float *top, float *bottom);

            void debugCameraInput();
            void debugCameraFace();

        private:
            bool selectTracker(ITracker *tracker);

            cv::VideoCapture m_videoCapture;
            ITracker *m_tracker;
            CFpsTimer *m_fpsTimer;

            Vec3 m_eyeOffset;
    };

}
