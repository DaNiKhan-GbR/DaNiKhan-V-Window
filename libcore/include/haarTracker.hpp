#pragma once

#include "tracker.hpp"

namespace dnkvw 
{

    class CHaarTracker : public ITracker
    {
        public:
            bool init();
            std::optional<cv::Rect> trackFrame(cv::Mat& inputFrame);

        private:
            cv::CascadeClassifier m_faceCascade;
    };

}

