#include "dnkvwContext.hpp"

// TODO: Remove
#include <iostream>
#include <filesystem>

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

void CDnkvwContext::debugCameraFace()
{
    if (!m_videoCapture.isOpened())
    {
        return;
    }
    
    const std::string WINDOW_NAME = "Dnkvw Debug Input";

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
    cv::Mat frame;
    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load("./data/haarcascade_frontalface_default.xml"))
    {
        std::cout << "Couldn't load haar cascade.\nCWD: " << std::filesystem::current_path() << "\n";
        return;
    }

    while (true)
    {
        m_videoCapture >> frame;

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(frame, faces);

        for (size_t i = 0; i < faces.size(); i++)
        {
            cv::rectangle(frame, faces[i], cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow(WINDOW_NAME, frame);

        if (cv::waitKey(5) >= 0)
        {
            break;
        }
    }

    cv::destroyWindow(WINDOW_NAME);
}

