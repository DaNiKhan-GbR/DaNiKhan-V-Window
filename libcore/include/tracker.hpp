#pragma once

#include <vector>
#include <optional>
#include <opencv2/opencv.hpp>

namespace dnkvw 
{
    class ITracker
    {
        public:
            virtual ~ITracker() {};
            
            virtual bool init() = 0;
            virtual std::optional<cv::Rect> trackFrame(cv::Mat& inputFrame) = 0;

            // TODO Check if really needed!
            virtual void cleanup() = 0;
    };
}
