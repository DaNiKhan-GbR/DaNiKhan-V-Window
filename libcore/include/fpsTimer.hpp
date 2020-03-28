#pragma once

#include <chrono>
#include "constants.hpp"

namespace dnkvw 
{

    class CFpsTimer
    {
        public:
            CFpsTimer(float alpha = 0.5f) : m_alpha(alpha), m_avgFps(constant::capture::targetFps) {}
            void start();
            float stop();

        private:
            std::chrono::high_resolution_clock m_timer;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
            float m_avgFps;
            float m_alpha;
    };

}

