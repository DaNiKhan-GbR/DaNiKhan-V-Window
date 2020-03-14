#include "haarTracker.hpp"

#include <iostream>

using dnkvw::CHaarTracker;

bool CHaarTracker::init()
{
    // TODO Read from memory instead of file
    if (!m_faceCascade.load("./data/haarcascade_frontalface_default.xml"))
    {
        std::cerr << "Couldn't load haar cascade.\n";
        return false;
    }

    return true;
}

std::vector<cv::Rect> CHaarTracker::trackFrame(cv::Mat& inputFrame)
{
    std::vector<cv::Rect> faces;
    // TODO: Constant values as settings or real constants. No magic numbers.
    m_faceCascade.detectMultiScale(inputFrame, faces, 1.3f, 3, 0, cv::Size(100, 100));

    return faces;
}

void CHaarTracker::cleanup()
{
    // Do nothing
}
