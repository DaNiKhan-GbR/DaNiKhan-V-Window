#include "thrdWinCalc.hpp"

using dnkvw::CThreadedWindowCalculator;

CThreadedWindowCalculator::~CThreadedWindowCalculator()
{
    if (m_threadRunning.load())
    {
        this->stop();
    }
}

void CThreadedWindowCalculator::start()
{
    m_threadRunning = true;
    m_thread = std::thread([this]{ this->processingLoop(); });
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
    m_signal_stop = true;
}

bool CThreadedWindowCalculator::isRunning()
{
    return m_threadRunning.load();
}

dnkvw::CWindowResult CThreadedWindowCalculator::obtainLastResult()
{
    return m_lastResult.load();
}

void CThreadedWindowCalculator::storeSettings(dnkvw::CWindowSettings settings)
{
    m_settings = settings;
}

void CThreadedWindowCalculator::signalCalibrate()
{
    m_signal_calibrate = true;
}

void CThreadedWindowCalculator::processingLoop()
{
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

    m_threadRunning = false;
}

void CThreadedWindowCalculator::calibrate()
{
    // TODO as a real constant
    constexpr int targetFaceCount = 10;
    constexpr int maxFaceTries = 50;

    if (!m_videoCapture.isOpened())
    {
        return;
    }

    cv::Mat frame;
    std::vector<cv::Rect> faces;

    int tryCount = 0;
    while (faces.size() < targetFaceCount && tryCount < maxFaceTries)
    {
        tryCount++;
        m_videoCapture >> frame;
        std::optional<cv::Rect> optFace = m_tracker->trackFrame(frame);

        if (optFace)
        {
            faces.push_back(optFace.value());
        }
    }

    if (faces.size() >= targetFaceCount / 2)
    {
        Vec3 eyeAvg;

        for (auto const& face : faces)
        {
            // TODO Avoid Copy & Paste
            float eyeX = ((float)(face.x + face.width / 2.0f)) / ((float)frame.cols) * 2.0f - 1.0f;
            float eyeY = ((float)(face.y + face.width / 4.0f)) / ((float)frame.rows) * 2.0f - 1.0f; 
            float eyeZ = 100.0f / (float)face.width;

            eyeAvg[0] += eyeX;
            eyeAvg[1] += eyeY;
            eyeAvg[2] += eyeZ;
        }

        eyeAvg /= (float) faces.size();
        
        m_eyeOffset = -eyeAvg;
        m_eyeOffset[2] = eyeAvg[2]; // TODO Mal schauen, aber richtig hart
    }
}

void CThreadedWindowCalculator::calcWindow()
{
    if (!m_videoCapture.isOpened())
    {
        return;
    }

    cv::Mat frame;
    m_videoCapture >> frame;
    std::optional<cv::Rect> optFace = m_tracker->trackFrame(frame);

    if (optFace)
    {
        cv::Rect face = optFace.value();

        // TODO Avoid Copy & Paste
        // TODO too hard for an good approx.
        float eyeX = ((float)(face.x + face.width / 2.0f)) / ((float)frame.cols) * 2.0f - 1.0f;
        float eyeY = ((float)(face.y + face.width / 4.0f)) / ((float)frame.rows) * 2.0f - 1.0f; 
        float eyeZ = 100.0f / (float)face.width;

        eyeX += m_eyeOffset[0];
        eyeY += m_eyeOffset[1];
        eyeZ += m_eyeOffset[2];

        Vec3 pa(-1, -1, 0);
        Vec3 pb( 1, -1, 0);
        Vec3 pc(-1,  1, 0);
        Vec3 pe(-eyeX, -eyeY, eyeZ);

        Vec3 vr = (pb - pa).norm();
        Vec3 vu = (pc - pa).norm();
        Vec3 vn = vr.cross(vu).norm();

        Vec3 va = pa - pe;
        Vec3 vb = pb - pe;
        Vec3 vc = pc - pe;

        float d = -(vn * va);
        float nearOverD = m_settings.load().near / d;

        float l = (vr * va) * nearOverD;
        float r = (vr * vb) * nearOverD;
        float b = (vu * va) * nearOverD;
        float t = (vu * vc) * nearOverD;

        // Todo set result
    }
}
