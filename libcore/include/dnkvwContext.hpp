#pragma once

#include "dnkvw/dnkvwInterface.h"
#include <opencv2/opencv.hpp>

#include "tracker.hpp"
#include "fpsTimer.hpp"
#include "vec3.hpp"
#include "thrdWinCalc.hpp"

namespace dnkvw 
{

    class CDnkvwContext : public IDnkvwContext
    {
        public:
    	    CDnkvwContext();

            bool selectHaarTracker();
            bool selectDnnTracker();

            bool startTracking(int cameraId);
            void stopTracking();

            void configureFrustum(float aspectRatio, float nearPlane);
            void calibrate();
            void loadFrustum(float *left, float *right, float *top, float *bottom, float *fps = nullptr);

            void debugCameraInput(int cameraId);
            void debugCameraFace(int cameraId);

        private:
            CThreadedWindowCalculator m_winCalc;
    };

}
