#include "dnkvwContext.hpp"
#include "constants.hpp"
#include "haarTracker.hpp"
#include "dnnTracker.hpp"

#include <cmath>
#include <cstring>

using dnkvw::CDnkvwContext;

void CDnkvwContext::__init()
{
    m_fpsTimer = new CFpsTimer(0.5f);
    m_tracker = nullptr;
    m_eyeOffset = Vec3();
}

void CDnkvwContext::__cleanup()
{
    delete m_fpsTimer;

    if (m_tracker)
    {
        m_tracker->cleanup();
        delete m_tracker;
    }
}

bool CDnkvwContext::selectHaarTracker()
{
    return this->selectTracker(new CHaarTracker);
}

bool CDnkvwContext::selectDnnTracker()
{
    return this->selectTracker(new CDnnTracker);
}

bool CDnkvwContext::selectTracker(dnkvw::ITracker* tracker)
{
    if (tracker->init())
    {
        if (m_tracker)
        {
            m_tracker->cleanup();
            delete m_tracker;
        }

        m_tracker = tracker;
        return true;
    }
    else
    {
        delete tracker;
        return false;
    }
}


bool CDnkvwContext::startTracking(int cameraId)
{
    bool success;
#ifdef _WIN32
    success = m_videoCapture.open(cameraId, cv::CAP_MSMF);
#else
    success = m_videoCapture.open(cameraId);
#endif

    if (success)
    {
        m_videoCapture.set(cv::CAP_PROP_FRAME_WIDTH, dnkvw::constant::targetWidth);
        m_videoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, dnkvw::constant::targetHeight);
        m_videoCapture.set(cv::CAP_PROP_FPS, dnkvw::constant::targetFps);

        if (::fabs(m_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH) - dnkvw::constant::targetWidth) > 0.1f || 
            ::fabs(m_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT) - dnkvw::constant::targetHeight) > 0.1f) 
        {
            std::cout << "WARNING: Camera resolution doesn't match target resolution of " << dnkvw::constant::targetWidth 
                    << "x" << dnkvw::constant::targetHeight << ". Tracking results may be wrong." << std::endl;
        }

        if (::fabs(m_videoCapture.get(cv::CAP_PROP_FPS) - dnkvw::constant::targetFps) > 0.1f) 
        {
            std::cout << "WARNING: Camera framerate doesn't match target framerate of " << dnkvw::constant::targetFps 
                    << "fps. Tracking results may be wrong." << std::endl;
        }
    }

    return success;
}

void CDnkvwContext::stopTracking()
{
    m_videoCapture.release();
}

void CDnkvwContext::calibrate()
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

void CDnkvwContext::loadFrustum(float near, float *left, float *right, float *top, float *bottom)
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
        float nearOverD = near / d;

        float l = (vr * va) * nearOverD;
        float r = (vr * vb) * nearOverD;
        float b = (vu * va) * nearOverD;
        float t = (vu * vc) * nearOverD;

        *left = l;
        *right = r;
        *bottom = b;
        *top = t;
    }
}

void CDnkvwContext::debugCameraInput()
{
    if (!m_videoCapture.isOpened())
    {
        return;
    }

    const std::string WINDOW_NAME = "Dnkvw Debug Input";

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
    cv::Mat frame;

    while (true)
    {
        m_fpsTimer->start();

        m_videoCapture >> frame;

        cv::flip(frame, frame, 1);

        float fps = m_fpsTimer->stop();

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
}

void CDnkvwContext::debugCameraFace()
{
    if (!m_videoCapture.isOpened() || !m_tracker)
    {
        return;
    }
    
    const std::string WINDOW_NAME = "Dnkvw Debug Input";

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);

    cv::Mat frame;

    while (true)
    {
        m_fpsTimer->start();

        m_videoCapture >> frame;

        std::optional<cv::Rect> optFace = m_tracker->trackFrame(frame);

        if (optFace)
        {
            cv::rectangle(frame, optFace.value(), cv::Scalar(0, 255, 0), 2);
        }

        cv::flip(frame, frame, 1);

        float fps = m_fpsTimer->stop();

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
}

