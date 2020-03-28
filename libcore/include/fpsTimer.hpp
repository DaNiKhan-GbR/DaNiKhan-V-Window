#pragma once

#include <chrono>
#include "constants.hpp"

namespace dnkvw 
{
    /**
     * FPS Timer class.
     * Measures the time between two time points and returns the frames per second.
     */
    class CFpsTimer
    {
        public:
            /**
             * Constructor of the FPS Timer.
             * 
             * @param alpha the smoothing factor, 0.0 is no smoothing. 0.0 <= alpha < 1.0 
             */
            CFpsTimer(float alpha = 0.5f) : m_alpha(alpha), m_avgFps(constant::capture::targetFps) {}

            /**
             * Start the timer.
             */
            void start();

            /**
             * Stop the timer and return the measured fps with smoothing.
             * 
             * @return the fps
             */
            float stop();

        private:
            /**
             * The clock for getting the time.
             */
            std::chrono::high_resolution_clock m_timer;

            /**
             * The start point of the active measurment.
             */
            std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

            /**
             * The average fps for the moving average.
             */
            float m_avgFps;

            /**
             * The configured alpha for the smoothing.
             * 0.0 <= alpha < 1.0
             */
            float m_alpha;
    };

}

