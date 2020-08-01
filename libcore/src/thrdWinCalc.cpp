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

#include "thrdWinCalc.hpp"

#include <sstream>

#include "constants.hpp"
#include "logger.hpp"

namespace dnkvw {

    CThreadedWindowCalculator::~CThreadedWindowCalculator()
    {
        if (m_threadRunning.load())
        {
            logger(ELog::VERBOSE) << "Automatically stopping tracking at end of lifetime.";
            this->stop();
        }
    }

    bool CThreadedWindowCalculator::start(int cameraId)
    {
        if (!m_threadRunning.load())
        {
            logger(ELog::VERBOSE) << "Starting tracking...";
            m_threadRunning = true;
            m_thread = std::thread([this, cameraId]{ this->processingLoop(cameraId); });

            {
                std::unique_lock<std::mutex> lock(m_startupMutex);
                m_startupSignal.wait(lock, [this]{ return m_startupDone; });

                return m_threadRunning.load();
            }
        }
        else
        {
            logger(ELog::WARNING) << "Cannot start tracking. Tracking already active.";
            return false;
        }
    }

    void CThreadedWindowCalculator::stop()
    {
        this->stopAsync();
        
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }

    void CThreadedWindowCalculator::stopAsync()
    {
        logger(ELog::VERBOSE) << "Stopping tracking...";
        m_signal_stop = true;
    }

    bool CThreadedWindowCalculator::isRunning()
    {
        return m_threadRunning.load();
    }

    CWindowResult CThreadedWindowCalculator::obtainLastResult()
    {
        return m_lastResult.load();
    }

    void CThreadedWindowCalculator::storeSettings(CWindowSettings settings)
    {
        m_settings = settings;
    }

    void CThreadedWindowCalculator::signalCalibrate()
    {
        logger(ELog::VERBOSE) << "Signal calibration.";
        m_signal_calibrate = true;
    }

    bool CThreadedWindowCalculator::selectTracker(std::unique_ptr<ITracker> tracker)
    {
        if (tracker->init())
        {
            m_tracker = std::move(tracker);
            logger(ELog::VERBOSE) << "Tracker changed.";
            return true;
        }
        else
        {
            logger(ELog::ERROR) << "Couldn't initialise tracker!";
        }
        
        return false;
    }

    void CThreadedWindowCalculator::debugCameraInput(int cameraId)
    {
        if (!initVideoCapture(cameraId))
        {
            return;
        }

        const std::string WINDOW_NAME = "Dnkvw Debug Input";

        cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
        cv::Mat frame;

        while (true)
        {
            m_fpsTimer.start();

            m_videoCapture >> frame;

            cv::flip(frame, frame, 1);

            float fps = m_fpsTimer.stop();

            char buffer[40] = { 0 };
            snprintf(buffer, 40, "FPS: %.1f", fps);
            cv::putText(frame, buffer, cv::Point(20, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));

            cv::imshow(WINDOW_NAME, frame);

            if (cv::waitKey(1) >= 0)
            {
                break;
            }
        }

        cv::destroyWindow(WINDOW_NAME);
        cv::waitKey(1); // Force closing window on mac

        m_videoCapture.release();
    }

    void CThreadedWindowCalculator::debugCameraFace(int cameraId)
    {
        if (!initVideoCapture(cameraId) || !m_tracker)
        {
            return;
        }
        
        const std::string WINDOW_NAME = "Dnkvw Debug Facedetection";

        cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);

        cv::Mat frame;

        while (true)
        {
            m_fpsTimer.start();

            m_videoCapture >> frame;

            std::optional<cv::Rect> optFace = m_tracker->trackFrame(frame);

            if (optFace)
            {
                cv::rectangle(frame, optFace.value(), cv::Scalar(0, 255, 0), 2);
            }

            cv::flip(frame, frame, 1);

            float fps = m_fpsTimer.stop();

            char buffer[40] = { 0 };
            snprintf(buffer, 40, "FPS: %.1f", fps);
            cv::putText(frame, buffer, cv::Point(20, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));

            cv::imshow(WINDOW_NAME, frame);

            if (cv::waitKey(1) >= 0)
            {
                break;
            }
        }

        cv::destroyWindow(WINDOW_NAME);
        cv::waitKey(1); // Force closing window on mac

        m_videoCapture.release();
    }

    void CThreadedWindowCalculator::faceToEye(const cv::Rect& face, const float frameWidth, const float frameHeight, Vec3& eye)
    {
        eye[0] = ((float)(face.x + face.width / 2.0f)) / frameWidth * 2.0f - 1.0f;
        eye[1] = ((float)(face.y + face.height / 4.0f)) / frameHeight * 2.0f - 1.0f; 
        eye[2] = 100.0f / (float)face.width;

        eye[0] *= eye[2];
        eye[1] *= eye[2]; 
    }

    bool CThreadedWindowCalculator::initVideoCapture(int cameraId)
    {
        bool success;
    #ifdef _WIN32
        success = m_videoCapture.open(cameraId, cv::CAP_MSMF);
    #else
        success = m_videoCapture.open(cameraId);
    #endif

        if (success)
        {
            m_videoCapture.set(cv::CAP_PROP_FRAME_WIDTH, constant::capture::targetWidth);
            m_videoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, constant::capture::targetHeight);
            m_videoCapture.set(cv::CAP_PROP_FPS, constant::capture::targetFps);

            if (::fabs(m_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH) - constant::capture::targetWidth) > 0.1f || 
                ::fabs(m_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT) - constant::capture::targetHeight) > 0.1f) 
            {
                std::ostringstream warning;
                warning << "WARNING: Camera resolution doesn't match target resolution of " << constant::capture::targetWidth 
                        << "x" << constant::capture::targetHeight << ". Tracking results may be wrong.";

                logger(ELog::WARNING) << warning.str();
            }

            if (::fabs(m_videoCapture.get(cv::CAP_PROP_FPS) - constant::capture::targetFps) > 0.1f) 
            {
                std::ostringstream warning;
                warning << "WARNING: Camera framerate doesn't match target framerate of " << constant::capture::targetFps 
                        << "fps. Tracking results may be wrong." << std::endl;
                
                logger(ELog::WARNING) << warning.str();
            }
        }
        else
        {
            logger(ELog::ERROR) << "Couldn't open video capture devide!";
        }
        
        return success && m_videoCapture.isOpened();
    }

    void CThreadedWindowCalculator::signalStartupDone()
    {
        {
            std::lock_guard<std::mutex> lock(m_startupMutex);
            m_startupDone = true;
        }
        m_startupSignal.notify_all();
    }

    void CThreadedWindowCalculator::processingLoop(int cameraId)
    {
        // Init
        if (!initVideoCapture(cameraId))
        {
            m_threadRunning = false;
            signalStartupDone();
            return;
        }

        signalStartupDone();

        logger(ELog::VERBOSE) << "Starting tracking... done";

        // Processing Loop
        while (!m_signal_stop)
        {
            if (m_signal_calibrate)
            {
                this->calibrate();
                m_signal_calibrate = false;
            }
            else
            {
                this->calcWindow();
            }
        }

        // Cleanup
        m_videoCapture.release();
        logger(ELog::VERBOSE) << "Stopping tracking... done";
        m_threadRunning = false;
    }

    void CThreadedWindowCalculator::calibrate()
    {
        if (!m_videoCapture.isOpened())
        {
            logger(ELog::ERROR) << "Calibration: Video Capture unexpectedly not open.";
            return;
        }

        logger(ELog::VERBOSE) << "Calibration...";

        cv::Mat frame;
        std::vector<cv::Rect> faces;

        // Collect a few frames with a face for calibration
        int tryCount = 0;
        while (faces.size() < constant::calib::targetFaceCount && tryCount < constant::calib::maxFaceTries)
        {
            tryCount++;
            m_videoCapture >> frame;
            std::optional<cv::Rect> optFace = m_tracker->trackFrame(frame);

            if (optFace)
            {
                faces.push_back(optFace.value());
            }
        }

        // Calculate the average face position and set it as the base/center position
        if (faces.size() >= constant::calib::targetFaceCount / 2)
        {
            Vec3 eyeAvg;

            for (auto const& face : faces)
            {
                Vec3 eye;
                faceToEye(face, (float)frame.cols, (float)frame.rows, eye);
                eyeAvg += eye;
            }

            eyeAvg /= (float) faces.size();
            
            m_eyeOffset = -eyeAvg;
            m_eyeOffset[2] = eyeAvg[2];
        }

        logger(ELog::VERBOSE) << "Calibration... done";
    }

    void CThreadedWindowCalculator::calcWindow()
    {
        m_fpsTimer.start();
        if (!m_videoCapture.isOpened())
        {
            logger(ELog::ERROR) << "Calc Window: Video Capture unexpectedly not open.";
            return;
        }

        cv::Mat frame;
        m_videoCapture >> frame;
        std::optional<cv::Rect> optFace = m_tracker->trackFrame(frame);

        if (optFace)
        {
            CWindowSettings settings = m_settings.load();
            cv::Rect face = optFace.value();

            Vec3 eye;
            faceToEye(face, (float)frame.cols, (float)frame.rows, eye);
            eye += m_eyeOffset;

            constexpr float eyeAvgFactor = constant::frustum::eyeAvgFactor;
            m_eyeAvg = m_eyeAvg * (1.0f - eyeAvgFactor) + eye * eyeAvgFactor;

            // The following algorithm is based on the paper Generalized Perspective 
            // Projection by Robert Kooima
            // https://csc.lsu.edu/~kooima/pdfs/gen-perspective.pdf

            Vec3 pa(-settings.aspect, -1, 0);
            Vec3 pb( settings.aspect, -1, 0);
            Vec3 pc(-settings.aspect,  1, 0);
            Vec3 pe(-m_eyeAvg[0], -m_eyeAvg[1], m_eyeAvg[2]);

            Vec3 vr = (pb - pa).norm();
            Vec3 vu = (pc - pa).norm();
            Vec3 vn = vr.cross(vu).norm();

            Vec3 va = pa - pe;
            Vec3 vb = pb - pe;
            Vec3 vc = pc - pe;

            float d = -(vn * va);
            float nearOverD = settings.near / d;

            float l = (vr * va) * nearOverD;
            float r = (vr * vb) * nearOverD;
            float b = (vu * va) * nearOverD;
            float t = (vu * vc) * nearOverD;

            // This is the end of the algorithm

            // Set result
            CWindowResult newResult;
            newResult.fps = m_fpsTimer.stop();
            newResult.left = l;
            newResult.right = r;
            newResult.bottom = b;
            newResult.top = t;
            newResult.eyeOffsetX = -m_eyeAvg[0];
            newResult.eyeOffsetY = -m_eyeAvg[1];
            newResult.eyeOffsetZ = -m_eyeAvg[2];

            m_lastResult = newResult;
        }
    }

} // namespace dnkvw
