#include "dnkvwContext.hpp"
#include "constants.hpp"
#include "haarTracker.hpp"
#include "dnnTracker.hpp"
#include "vec3.hpp"

#include <cmath>
#include <cstring>

// TODO: Remove
#include <iostream>
#include <filesystem>

using dnkvw::CDnkvwContext;

void CDnkvwContext::__init()
{
    m_fpsTimer = new CFpsTimer(0.5f);
    m_tracker = nullptr;
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

    return success;
}

void CDnkvwContext::stopTracking()
{
    m_videoCapture.release();
}

void CDnkvwContext::loadFrustum(float near, float *left, float *right, float *top, float *bottom)
{
    if (!m_videoCapture.isOpened())
    {
        return;
    }

    cv::Mat frame;
    m_videoCapture >> frame;
    std::vector<cv::Rect> faces = m_tracker->trackFrame(frame);

    if (faces.size() > 0)
    {
        cv::Rect face = faces[0];

        // TODO too hard for an good approx.
        float eyeX = ((float)(face.x + face.width / 2.0f)) / ((float)frame.cols) * 2.0f - 1.0f;
        float eyeY = ((float)(face.y + face.width / 4.0f)) / ((float)frame.rows) * 2.0f - 1.0f; 
        float eyeZ = 100.0f / (float)face.width;

        Vec3 pa(-1, -1, 0);
        Vec3 pb( 1, -1, 0);
        Vec3 pc(-1,  1, 0);
        Vec3 pe(eyeX, -eyeY, eyeZ);

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
        sprintf_s(buffer, "FPS: %.1f", fps);
        cv::putText(frame, buffer, cv::Point(20, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));

        cv::imshow(WINDOW_NAME, frame);

        if (cv::waitKey(1) >= 0)
        {
            break;
        }
    }

    cv::destroyWindow(WINDOW_NAME);
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

        std::vector<cv::Rect> faces = m_tracker->trackFrame(frame);

        for (size_t i = 0; i < faces.size(); i++)
        {
            cv::rectangle(frame, faces[i], cv::Scalar(0, 255, 0), 2);
        }

        cv::flip(frame, frame, 1);

        float fps = m_fpsTimer->stop();

        char buffer[40] = { 0 };
        sprintf_s(buffer, "FPS: %.1f", fps);
        cv::putText(frame, buffer, cv::Point(20, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));

        cv::imshow(WINDOW_NAME, frame);

        if (cv::waitKey(1) >= 0)
        {
            break;
        }
    }

    cv::destroyWindow(WINDOW_NAME);
}

