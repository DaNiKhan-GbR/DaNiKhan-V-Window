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
 * @brief DNN Face Tracker.
 */

#include "dnnTracker.hpp"

#include <iostream>

#include "resources.hpp"
#include "logger.hpp"

namespace dnkvw {

    bool CDnnTracker::init()
    {
        auto fs = cmrc::dnkvw_rc::get_filesystem();
        
        auto prototxt = fs.open("data/dnn.prototxt");
        auto caffemodel = fs.open("data/dnn.caffemodel");
        
        try {
            m_dnnNet = cv::dnn::readNetFromCaffe(prototxt.cbegin(), prototxt.size(), caffemodel.cbegin(), caffemodel.size());
        } catch (cv::Exception& e) {
            logger(ELog::ERROR) << "Couldn't load DNN data: ";
            logger(ELog::ERROR) << e.msg;
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

} // namespace dnkvw
