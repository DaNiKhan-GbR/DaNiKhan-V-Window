#pragma once

#include "tracker.hpp"

namespace dnkvw 
{

    class CDnnTracker : public ITracker
    {
        public:
            bool init();
            std::vector<cv::Rect> trackFrame(cv::Mat& inputFrame);
            void cleanup();

        private:
            cv::dnn::Net m_dnnNet;
    };

}

