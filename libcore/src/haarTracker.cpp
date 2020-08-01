//////////////////////////////////////////////////////////////////////////////////////
//  BSD 3-Clause License                                                            //
//                                                                                  //
//  Copyright (c) 2020, Daniel Klintworth and Nicolas Hollmann                      //
//  All rights reserved.                                                            //
//                                                                                  //
//  Redistribution and use in source and binary forms, with or without              //
//  modification, are permitted provided that the following conditions are met:     //
//                                                                                  //
//  * Redistributions of source code must retain the above copyright notice, this   //
//    list of conditions and the following disclaimer.                              //
//                                                                                  //
//  * Redistributions in binary form must reproduce the above copyright notice,     //
//    this list of conditions and the following disclaimer in the documentation     //
//    and/or other materials provided with the distribution.                        //
//                                                                                  //
//  * Neither the name of the copyright holder nor the names of its                 //
//    contributors may be used to endorse or promote products derived from          //
//    this software without specific prior written permission.                      //
//                                                                                  //
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"     //
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE       //
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  //
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE    //
//  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL      //
//  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR      //
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      //
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   //
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   //
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            //
//                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////

/**
 * @file
 * @author Nicolas Hollmann
 * @author Daniel Klintworth
 * @brief Haar-Cascade Face Tracker.
 */

#include "haarTracker.hpp"

#include <iostream>

#include "resources.hpp"
#include "logger.hpp"
#include "constants.hpp"

namespace dnkvw {

    bool CHaarTracker::init()
    {
        auto fs = cmrc::dnkvw_rc::get_filesystem();
        
        auto haarCascade = fs.open("data/haarcascade_frontalface_default.xml");
        std::string haarCascadeContent(haarCascade.begin(), haarCascade.end());
        cv::FileStorage cvHaarFs(haarCascadeContent, cv::FileStorage::READ | cv::FileStorage::MEMORY);
        
        if (!m_faceCascade.read(cvHaarFs.getFirstTopLevelNode()))
        {
            logger(ELog::ERROR) << "Couldn't load haar cascade.\n";
            return false;
        }

        return true;
    }

    std::optional<cv::Rect> CHaarTracker::trackFrame(cv::Mat& inputFrame)
    {
        std::vector<cv::Rect> faces;
        
        m_faceCascade.detectMultiScale(
            inputFrame, 
            faces, 
            constant::haartracker::scaleFactor, 
            3, // default value for minNeighbours 
            0, // no flags 
            cv::Size(
                constant::haartracker::minSize, 
                constant::haartracker::minSize
            )
        );

        if (faces.size() > 0)
        {
            return std::optional<cv::Rect>(faces[0]);
        }

        return std::nullopt;
    }

} // namespace dnkvw
