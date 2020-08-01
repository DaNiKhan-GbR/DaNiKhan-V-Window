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
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>

#include "fpsTimer.hpp"
#include "vec3.hpp"
#include "tracker.hpp"

namespace dnkvw
{
    /**
     * Datatype for needed calculator settings. 
     */
    struct CWindowSettings
    {
        float aspect;       /* The aspect ratio of the display window */
        float near;         /* The distance to the nearplane */
        float reseved1;     /* This avoids a pre VS 2015 Update 2 memory alignment bug. :( */
    };

    /**
     * Result datatype for calculated head positions.
     */
    struct CWindowResult 
    {
        float left;         /* The left border of the viewing frustum */
        float right;        /* The right border of the viewing frustum */
        float top;          /* The top border of the viewing frustum */
        float bottom;       /* The bottom border of the viewing frustum */
        float eyeOffsetX;   /* Offset of the eye from the normal camery position on the X axis */
        float eyeOffsetY;   /* Offset of the eye from the normal camery position on the Y axis */
        float eyeOffsetZ;   /* Offset of the eye from the normal camery position on the Z axis */
        float fps;          /* How many frames per second can be archieved with the head tracking */
    };

    /**
     * A multithreaded virtual window calculator.
     * To better performance on a multicore CPU all calculations
     * are done on a second thread which is controlled by a member of this class.
     */
    class CThreadedWindowCalculator
    {
        public:
            /**
             * This constructor initialises a new threaded window calculator.
             * No thead is started until the start method is called.
             */
            CThreadedWindowCalculator() : m_thread(), m_fpsTimer(0.5f), m_tracker(nullptr), m_eyeOffset()
            {
                CWindowResult initRes;
                initRes.left    = -1.0f;
                initRes.right   =  1.0f;
                initRes.top     =  1.0f;
                initRes.bottom  = -1.0f;
                initRes.eyeOffsetX = 0.0f;
                initRes.eyeOffsetY = 0.0f;
                initRes.eyeOffsetZ = 0.0f;
                initRes.fps     =  1.0f;
                m_lastResult = initRes;

                CWindowSettings initSettings;
                initSettings.aspect = 1.0;
                initSettings.near = 1.0f;
                initSettings.reseved1 = 0.0f;
                m_settings = initSettings;

                m_startupDone = false;
                m_threadRunning = false;
                m_signal_stop = false;
                m_signal_calibrate = false;
            };

            /**
             * While not recommended, the deconstructor stops the thread if needed.
             */
            ~CThreadedWindowCalculator();

            /**
             * Start the tracking thread.
             * 
             * @param cameraId Id of the camera to use for tracking
             * @return true if initialization was successful
             */
            bool start(int cameraId);
            
            /**
             * Stop the tracking. This function may block.
             */
            void stop();

            /**
             * Stop the tracking as soon as possible.
             */
            void stopAsync();
            
            /**
             * Determine if the thread is currently running.
             * 
             * @return true if the tracking thread is running
             */
            bool isRunning();

            /**
             * Obtain the last tracking result.
             * 
             * @return the last tracking result
             */
            CWindowResult obtainLastResult();

            /**
             * Stores the new settings.
             * 
             * @param settings the new settings
             */
            void storeSettings(CWindowSettings settings);

            /**
             * Signals to initiate the calibration.
             */
            void signalCalibrate();

            /**
             * Change which tracker is used.
             * 
             * @param tracker the new tracker to use
             * @return true if changing was successful
             */
            bool selectTracker(std::unique_ptr<ITracker> tracker);

            /**
             * Debugging function to test the camera input. 
             * Should not be called while the tracking thread is running.
             * This function may block.
             * 
             * @param cameraId Id of the camera to use for tracking
             */
            void debugCameraInput(int cameraId);

            /**
             * Debugging function to test the face detection. 
             * Should not be called while the tracking thread is running.
             * This function may block.
             * 
             * @param cameraId Id of the camera to use for tracking
             */
            void debugCameraFace(int cameraId);

        private:
            /**
             * Approximates an eye position from a tracked face.
             * 
             * @param face the tracked face
             * @param frameWidth the width of the camera frame
             * @param frameHeight the height of the camera frame
             * @param eye output parameter reference to store the approximated result
             */
            void faceToEye(const cv::Rect& face, const float frameWidth, const float frameHeight, Vec3& eye);

            /**
             * Opens the VideoCapture Device specified by the cameraId.
             * Also some checks are performed.
             * 
             * @param cameraId the id of the camera to read the image data from
             */
            bool initVideoCapture(int cameraId);

            /**
             * Signal to the main thread that the startup is done.
             */
            void signalStartupDone();

            /**
             * Entry point and main method for the tracking thread.
             * Handles the complete face tracking and window calculation process.
             * 
             * @param cameraId the id of the camera to read the image data from
             */
            void processingLoop(int cameraId);

            /**
             * Calibrates by taking some frames and calculating the average
             * eye position. This position is then used as the base/center position.
             */
            void calibrate();

            /**
             * Calculates the virtual window from the face tracking results.
             */
            void calcWindow();

            /**
             * Allows to read the camera with OpenCV.
             */
            cv::VideoCapture m_videoCapture;

            /**
             * A reference to the face tracker of choice.
             */
            std::unique_ptr<ITracker> m_tracker;

            /**
             * FPS Timer object.
             */
            CFpsTimer m_fpsTimer;

            /**
             * The calibration result. 
             * This is a calculated offset for the eye position.
             */
            Vec3 m_eyeOffset;

            /**
             * The running average of the virtual eye position.
             */
            Vec3 m_eyeAvg;
            
            /**
             * The handle for the tracking thread.
             */
            std::thread m_thread;

            /**
             * This is a mutex for the startup variable.
             */
            std::mutex m_startupMutex;

            /**
             * This startup variable signals that the start is completed.
             * It may have failed.
             */
            bool m_startupDone;

            /**
             * This conditional variable is used to check if the startup has finished yet.
             */
            std::condition_variable m_startupSignal;
            
            /**
             * Atomic for the last calculation result.
             * Can be accessed from both threads at any time.
             */
            std::atomic<CWindowResult> m_lastResult;

            /**
             * Atomic for the settings describing the window.
             * Can be accessed from both threads at any time.
             */
            std::atomic<CWindowSettings> m_settings;
            
            /**
             * Atomic status flag that signals if the thread is running or not.
             */
            std::atomic_bool m_threadRunning;
            
            /**
             * This atomic boolean can be used to signal the tracking thread to stop as
             * soon as possible.
             */
            std::atomic_bool m_signal_stop;

            /**
             * This atomic boolean can be used to signal the tracking thread to calibrate
             * the tracking as soon as possible.
             */
            std::atomic_bool m_signal_calibrate;
    };
}
