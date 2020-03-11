#include "dnkvwContext.hpp"

using dnkvw::CDnkvwContext;

bool CDnkvwContext::startTracking(int cameraId)
{
    return m_videoCapture.open(cameraId);
}

void CDnkvwContext::stopTracking()
{
    m_videoCapture.release();
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
        m_videoCapture >> frame;

        cv::imshow(WINDOW_NAME, frame);

        if (cv::waitKey(5) >= 0)
        {
            break;
        }
    }

    cv::destroyWindow(WINDOW_NAME);
}
