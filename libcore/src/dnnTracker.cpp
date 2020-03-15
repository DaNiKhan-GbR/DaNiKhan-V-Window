#include "dnnTracker.hpp"

#include <iostream>

using dnkvw::CDnnTracker;

bool CDnnTracker::init()
{
    // TODO Read from memory instead of file
    try {
        m_dnnNet = cv::dnn::readNetFromCaffe("./data/deploy.prototxt", "./data/res10_300x300_ssd_iter_140000_fp16.caffemodel");
    } catch (cv::Exception& e) {
        std::cerr << "Couldn't load DNN data: " << e.msg << "\n";
        return false;
    }

    return true;
}

std::vector<cv::Rect> CDnnTracker::trackFrame(cv::Mat& inputFrame)
{
    std::vector<cv::Rect> faces;

    // TODO: Constant values as settings or real constants. No magic numbers.
    cv::Mat inputBlob = cv::dnn::blobFromImage(inputFrame, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0), false, false);
    m_dnnNet.setInput(inputBlob, "data");
    cv::Mat detection = m_dnnNet.forward("detection_out");
    cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    for (int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > 0.7f) // TODO: confidence_threshold
        {
            int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * inputFrame.cols);
            int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * inputFrame.rows);
            int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * inputFrame.cols);
            int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * inputFrame.rows);

            faces.push_back(cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)));
        }
    }

    return faces;
}

void CDnnTracker::cleanup()
{
    // Do nothing
}
