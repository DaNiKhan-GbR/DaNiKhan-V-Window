#include "dnkvwContext.hpp"
#include "constants.hpp"

#include <cmath>
#include <chrono>
#include <cstring>

// TODO: Remove
#include <iostream>
#include <filesystem>

using dnkvw::CDnkvwContext;
using ms = std::chrono::duration<float, std::milli>;

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

void CDnkvwContext::debugCameraInput()
{
    if (!m_videoCapture.isOpened())
    {
        return;
    }

    const std::string WINDOW_NAME = "Dnkvw Debug Input";

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
    cv::Mat frame;

    std::chrono::high_resolution_clock timer;
    float prevFPS = 0.0f;

    while (true)
    {
        auto start = timer.now();

        m_videoCapture >> frame;

        cv::flip(frame, frame, 1);

        auto end = timer.now();
        auto deltaTime = std::chrono::duration_cast<ms>(end - start).count();
        float fps = 1000.0f / deltaTime;

        char buffer[40] = { 0 };
        sprintf_s(buffer, "FPS: %.1f", (fps + prevFPS) / 2.0f);
        cv::putText(frame, buffer, cv::Point(20, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));
        prevFPS = fps;

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
    if (!m_videoCapture.isOpened())
    {
        return;
    }
    
    const std::string WINDOW_NAME = "Dnkvw Debug Input";

    std::chrono::high_resolution_clock timer;
    float prevFPS = 0.0f;

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
        auto start = timer.now();

        m_videoCapture >> frame;

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(frame, faces, 1.3f, 3, 0, cv::Size(100, 100));

        for (size_t i = 0; i < faces.size(); i++)
        {
            cv::rectangle(frame, faces[i], cv::Scalar(0, 255, 0), 2);
        }

        cv::flip(frame, frame, 1);

        auto end = timer.now();
        auto deltaTime = std::chrono::duration_cast<ms>(end - start).count();
        float fps = 1000.0f / deltaTime;

        char buffer[40] = { 0 };
        sprintf_s(buffer, "FPS: %.1f", (fps + prevFPS) / 2.0f);
        cv::putText(frame, buffer, cv::Point(20, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));
        prevFPS = fps;

        cv::imshow(WINDOW_NAME, frame);

        if (cv::waitKey(1) >= 0)
        {
            break;
        }
    }

    cv::destroyWindow(WINDOW_NAME);
}

void CDnkvwContext::debugCameraFaceDNN()
{
    if (!m_videoCapture.isOpened())
    {
        return;
    }
    
    const std::string WINDOW_NAME = "Dnkvw Debug Input";

    std::chrono::high_resolution_clock timer;
    float prevFPS = 0.0f;

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
    cv::Mat frame;

    cv::dnn::Net net = cv::dnn::readNetFromCaffe("./data/deploy.prototxt", "./data/res10_300x300_ssd_iter_140000_fp16.caffemodel");

    while (true)
    {
        auto start = timer.now();

        m_videoCapture >> frame;

        cv::Mat inputBlob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0), false, false);
        net.setInput(inputBlob, "data");
        cv::Mat detection = net.forward("detection_out");
        cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

        for (int i = 0; i < detectionMat.rows; i++)
        {
            float confidence = detectionMat.at<float>(i, 2);

            if (confidence > 0.7f)
            {
                int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * dnkvw::constant::targetWidth);
                int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * dnkvw::constant::targetHeight);
                int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * dnkvw::constant::targetWidth);
                int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * dnkvw::constant::targetHeight);

                cv::rectangle(frame, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 2);
            }
        }

        cv::flip(frame, frame, 1);

        auto end = timer.now();
        auto deltaTime = std::chrono::duration_cast<ms>(end - start).count();
        float fps = 1000.0f / deltaTime;

        char buffer[40] = { 0 };
        sprintf_s(buffer, "FPS: %.1f", (fps + prevFPS) / 2.0f);
        cv::putText(frame, buffer, cv::Point(20, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));
        prevFPS = fps;

        cv::imshow(WINDOW_NAME, frame);

        if (cv::waitKey(5) >= 0)
        {
            break;
        }
    }

    cv::destroyWindow(WINDOW_NAME);
}

