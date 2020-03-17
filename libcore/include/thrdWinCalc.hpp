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
            CThreadedWindowCalculator() : m_thread(), m_fpsTimer(0.5f) {}
            ~CThreadedWindowCalculator();

            void start();
            void stop();
            void stopAsync();
            bool isRunning();

            CWindowResult obtainLastResult();
            void storeSettings(CWindowSettings settings);
            void signalCalibrate();

        private:
            void processingLoop();
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
