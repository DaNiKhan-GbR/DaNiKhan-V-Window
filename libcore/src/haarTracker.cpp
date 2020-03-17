#include "haarTracker.hpp"
#include "resources.hpp"

#include <iostream>

using dnkvw::CHaarTracker;

bool CHaarTracker::init()
{
    auto fs = cmrc::dnkvw_rc::get_filesystem();
    
    auto haarCascade = fs.open("data/haarcascade_frontalface_default.xml");
    std::string haarCascadeContent(haarCascade.begin(), haarCascade.end());
    cv::FileStorage cvHaarFs(haarCascadeContent, cv::FileStorage::READ | cv::FileStorage::MEMORY);
    
    if (!m_faceCascade.read(cvHaarFs.getFirstTopLevelNode()))
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
