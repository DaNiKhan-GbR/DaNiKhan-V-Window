//////////////////////////////////////////////////////////////////////////////////////
//  BSD 3-Clause License                                                            //
//                                                                                  //
//  Copyright (c) 2020, Daniel Klintworth and Nicolas Hollmann                      //
//  All rights reserved.                                                            //
//                                                                                  //
//  Redistribution and use in source and binary forms, with or without              //
//  modification, are permitted provided that the following conditions are met:     //
//                                                                                  //
//  * Redistributions of source code must retain the above copyright notice, this   //
//    list of conditions and the following disclaimer.                              //
//                                                                                  //
//  * Redistributions in binary form must reproduce the above copyright notice,     //
//    this list of conditions and the following disclaimer in the documentation     //
//    and/or other materials provided with the distribution.                        //
//                                                                                  //
//  * Neither the name of the copyright holder nor the names of its                 //
//    contributors may be used to endorse or promote products derived from          //
//    this software without specific prior written permission.                      //
//                                                                                  //
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"     //
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE       //
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  //
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE    //
//  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL      //
//  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR      //
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      //
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   //
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   //
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            //
//                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////

/**
 * @file
 * @author Nicolas Hollmann
 * @author Daniel Klintworth
 * @brief Dnkvw Context Implementation.
 */

#pragma once

#include <opencv2/opencv.hpp>
#include "dnkvw/dnkvwInterface.h"

#include "tracker.hpp"
#include "fpsTimer.hpp"
#include "vec3.hpp"
#include "thrdWinCalc.hpp"

namespace dnkvw 
{

    class CDnkvwContext : public IDnkvwContext
    {
        public:
            /**
             * This constructor initializes the context.
             * 
             * Side-effect: The OpenCV Logger and the dnkvw Logger log levels
             * are set to the defaults for the active build configuration.
             */
    	    CDnkvwContext();

            void setLogLevel(int logLevel);
            void setInternalLogLevel(int internalLogLevel);

            bool selectHaarTracker();
            bool selectDnnTracker();

            bool startTracking(int cameraId);
            void stopTracking();
            void stopTrackingAsync();

            void configureFrustum(float aspectRatio, float nearPlane);
            void calibrate();
            void loadFrustum(float *left, float *right, float *top, float *bottom);
            void loadEyeOffset(float *x, float *y, float *z);
            void loadFps(float *fps);

            void debugCameraInput(int cameraId);
            void debugCameraFace(int cameraId);

        private:
            /**
             * The multithreaded window calculator for this context.
             */
            CThreadedWindowCalculator m_winCalc;
    };

}
