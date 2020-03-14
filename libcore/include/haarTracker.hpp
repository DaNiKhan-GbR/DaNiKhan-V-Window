#pragma once

#include "tracker.hpp"

namespace dnkvw 
{

    class CHaarTracker : public ITracker
    {
        public:
            bool init();
            std::vector<cv::Rect> trackFrame(cv::Mat& inputFrame);
            void cleanup();

        private:
            cv::CascadeClassifier m_faceCascade;
    };

}

