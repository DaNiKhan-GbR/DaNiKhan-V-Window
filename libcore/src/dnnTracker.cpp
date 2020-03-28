#include "dnnTracker.hpp"
#include "resources.hpp"
#include "logger.hpp"

#include <iostream>

using dnkvw::CDnnTracker;

bool CDnnTracker::init()
{
    auto fs = cmrc::dnkvw_rc::get_filesystem();
    
    auto prototxt = fs.open("data/dnn.prototxt");
    auto caffemodel = fs.open("data/dnn.caffemodel");
    
    try {
        m_dnnNet = cv::dnn::readNetFromCaffe(prototxt.cbegin(), prototxt.size(), caffemodel.cbegin(), caffemodel.size());
    } catch (cv::Exception& e) {
        logger(ELog::ERROR) << "Couldn't load DNN data: " << e.msg;
        return false;
    }

    return true;
}

std::optional<cv::Rect> CDnnTracker::trackFrame(cv::Mat& inputFrame)
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

    // TODO: Select most likely face
    if (faces.size() > 0)
    {
        return std::optional<cv::Rect>(faces[0]);
    }

    return std::nullopt;
}
