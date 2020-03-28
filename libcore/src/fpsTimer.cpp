#include "fpsTimer.hpp"

using dnkvw::CFpsTimer;
using ms = std::chrono::duration<float, std::milli>;

void CFpsTimer::start()
{
    m_start = m_timer.now();
}

float CFpsTimer::stop()
{
    auto end = m_timer.now();
    auto deltaTime = std::chrono::duration_cast<ms>(end - m_start).count();
    float fps = 1000.0f / deltaTime;

    // Moving average to smooth the output
    m_avgFps = m_alpha * m_avgFps + (1.0f - m_alpha) * fps;

    return m_avgFps;
}
