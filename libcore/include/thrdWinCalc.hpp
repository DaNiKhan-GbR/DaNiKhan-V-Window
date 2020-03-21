#pragma once

#include <thread>
#include <atomic>
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

            bool selectTracker(ITracker *tracker);

        private:
            void faceToEye(const cv::Rect& face, const float frameWidth, const float frameHeight, Vec3& eye);
            void initVideoCapture(int cameraId);
            void processingLoop(int cameraId);
            void calibrate();
            void calcWindow();

            cv::VideoCapture m_videoCapture;
            ITracker *m_tracker;
            CFpsTimer m_fpsTimer;
            Vec3 m_eyeOffset;

            std::thread m_thread;
            std::atomic<CWindowResult> m_lastResult;
            std::atomic<CWindowSettings> m_settings;
            std::atomic_bool m_threadRunning;
            std::atomic_bool m_signal_stop;
            std::atomic_bool m_signal_calibrate;
    };
}