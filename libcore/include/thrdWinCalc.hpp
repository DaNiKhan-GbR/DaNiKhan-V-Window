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
 * @brief Threaded Virtual Window Calculator.
 */

#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include <opencv2/opencv.hpp>

#include "fpsTimer.hpp"
#include "vec3.hpp"
#include "tracker.hpp"

namespace dnkvw
{
    struct CWindowSettings
    {
        float aspect;
        float near;
        float reseved1; // Dient den umgehen eines pre VS 2015 Update 2 Bugs. :(
    };

    struct CWindowResult 
    {
        float left;
        float right;
        float top;
        float bottom;
        float fps;
    };

    class CThreadedWindowCalculator
    {
        public:
            CThreadedWindowCalculator() : m_thread(), m_fpsTimer(0.5f), m_tracker(nullptr), m_eyeOffset()
            {
                CWindowResult initRes;
                initRes.left    = -1.0f;
                initRes.right   =  1.0f;
                initRes.top     =  1.0f;
                initRes.bottom  = -1.0f;
                initRes.fps     =  1.0f;
                m_lastResult = initRes;

                CWindowSettings initSettings;
                initSettings.aspect = 1.0;
                initSettings.near = 1.0f;
                initSettings.reseved1 = 0.0f;
                m_settings = initSettings;

                m_threadRunning = false;
                m_signal_stop = false;
                m_signal_calibrate = false;
            };
            ~CThreadedWindowCalculator();

            void start(int cameraId);
            void stop();
            void stopAsync();
            bool isRunning();

            CWindowResult obtainLastResult();
            void storeSettings(CWindowSettings settings);
            void signalCalibrate();

            bool selectTracker(std::unique_ptr<ITracker> tracker);

            void debugCameraInput(int cameraId);
            void debugCameraFace(int cameraId);

        private:
            void faceToEye(const cv::Rect& face, const float frameWidth, const float frameHeight, Vec3& eye);
            bool initVideoCapture(int cameraId);
            void processingLoop(int cameraId);
            void calibrate();
            void calcWindow();

            cv::VideoCapture m_videoCapture;
            std::unique_ptr<ITracker> m_tracker;
            CFpsTimer m_fpsTimer;
            Vec3 m_eyeOffset;
            Vec3 m_eyeAvg;

            std::thread m_thread;
            std::atomic<CWindowResult> m_lastResult;
            std::atomic<CWindowSettings> m_settings;
            std::atomic_bool m_threadRunning;
            std::atomic_bool m_signal_stop;
            std::atomic_bool m_signal_calibrate;
    };
}
