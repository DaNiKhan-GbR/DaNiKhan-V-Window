#include "dnkvwContext.hpp"
#include "constants.hpp"
#include "haarTracker.hpp"
#include "dnnTracker.hpp"

#include <cmath>
#include <cstring>

using dnkvw::CDnkvwContext;

void CDnkvwContext::__init()
{
    // TODO not needed??
}

void CDnkvwContext::__cleanup()
{
    // TODO not needed??
}

bool CDnkvwContext::selectHaarTracker()
{
    return m_winCalc.selectTracker(new CHaarTracker);
}

bool CDnkvwContext::selectDnnTracker()
{
    return m_winCalc.selectTracker(new CDnnTracker);
}

bool CDnkvwContext::startTracking(int cameraId)
{
    m_winCalc.start(cameraId);

    return true; // TODO implement a correct success status
}

void CDnkvwContext::stopTracking()
{
    // TODO Async or not? Maybe let the application decide?
    m_winCalc.stop();
}

void CDnkvwContext::configureFrustum(float aspectRatio, float nearPlane)
{
    CWindowSettings settings;
    settings.aspect = aspectRatio;
    settings.near = nearPlane;
    settings.reseved1 = 0.0f;
    m_winCalc.storeSettings(settings);
}

void CDnkvwContext::calibrate()
{
    m_winCalc.signalCalibrate();
}

void CDnkvwContext::loadFrustum(float *left, float *right, float *top, float *bottom, float *fps)
{
    auto result = m_winCalc.obtainLastResult();
    *left = result.left;
    *right = result.right;
    *bottom = result.bottom;
    *top = result.top;

    if (fps)
    {
        *fps = result.fps;
    }
}

void CDnkvwContext::debugCameraInput()
{
    /*if (!m_videoCapture.isOpened())
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
    cv::waitKey(1); // Force closing window on mac*/
}

void CDnkvwContext::debugCameraFace()
{
    /*if (!m_videoCapture.isOpened() || !m_tracker)
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
    cv::waitKey(1); // Force closing window on mac*/
}

