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
 * @brief Implementation of the IDnkvwContext.
 */

#include "dnkvwContext.hpp"

#include <cmath>
#include <cstring>
#include <opencv2/core/utils/logger.hpp>

#include "constants.hpp"
#include "haarTracker.hpp"
#include "dnnTracker.hpp"
#include "logger.hpp"

namespace dnkvw {

    CDnkvwContext::CDnkvwContext()
    {
        #ifdef NDEBUG // NDEBUG = NOT DEBUG
            cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
            logger.setLogLevel(ELog::WARNING);
        #else
            cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
            logger.setLogLevel(ELog::VERBOSE);
        #endif
    }

    void CDnkvwContext::setLogLevel(int logLevel)
    {
        if (m_winCalc.isRunning())
        {
            logger(ELog::WARNING) << "Cannot set a new log level while tracking is active!";
            return;
        }

        ELog level;

        switch (logLevel)
        {
            case DNKVW_LOG_LEVEL_SILENT:
                level = ELog::SILENT;
                break;
                
            case DNKVW_LOG_LEVEL_ERROR:
                level = ELog::ERROR;
                break;
            
            case DNKVW_LOG_LEVEL_WARNING:
                level = ELog::WARNING;
                break;

            case DNKVW_LOG_LEVEL_VERBOSE:
                level = ELog::VERBOSE;
                break;

            default:
                logger(ELog::WARNING) << "Unkown log level!";
                return;
        }

        logger.setLogLevel(level);
    }

    void CDnkvwContext::setInternalLogLevel(int internalLogLevel)
    {
        cv::utils::logging::LogLevel level;

        switch (internalLogLevel)
        {
            case DNKVW_INTERNAL_LOG_LEVEL_SILENT:
                level = cv::utils::logging::LOG_LEVEL_SILENT;
                break;
                
            case DNKVW_INTERNAL_LOG_LEVEL_IMPORTANT:
                level = cv::utils::logging::LOG_LEVEL_WARNING;
                break;

            case DNKVW_INTERNAL_LOG_LEVEL_VERBOSE:
                level = cv::utils::logging::LOG_LEVEL_VERBOSE;
                break;

            default:
                logger(ELog::WARNING) << "Unkown internal log level!";
                return;
        }

        cv::utils::logging::setLogLevel(level);
    }

    bool CDnkvwContext::selectHaarTracker()
    {
        return m_winCalc.selectTracker(std::make_unique<CHaarTracker>());
    }

    bool CDnkvwContext::selectDnnTracker()
    {
        return m_winCalc.selectTracker(std::make_unique<CDnnTracker>());
    }

    bool CDnkvwContext::startTracking(int cameraId)
    {
        m_winCalc.start(cameraId);

        return true; // TODO implement a correct success status
    }

    void CDnkvwContext::stopTracking()
    {
        m_winCalc.stop();
    }

    void CDnkvwContext::stopTrackingAsync()
    {
        m_winCalc.stopAsync();
    }

    void CDnkvwContext::configureFrustum(float aspectRatio, float nearPlane)
    {
        CWindowSettings settings;
        settings.aspect = aspectRatio;
        settings.near = nearPlane;
        settings.reseved1 = 0.0f;
        m_winCalc.storeSettings(settings);
    }

    void CDnkvwContext::calibrate()
    {
        m_winCalc.signalCalibrate();
    }

    void CDnkvwContext::loadFrustum(float *left, float *right, float *top, float *bottom)
    {
        auto result = m_winCalc.obtainLastResult();
        *left = result.left;
        *right = result.right;
        *bottom = result.bottom;
        *top = result.top;
    }

    void CDnkvwContext::loadEyeOffset(float *x, float *y, float *z)
    {
        auto result = m_winCalc.obtainLastResult();
        *x = result.eyeOffsetX;
        *y = result.eyeOffsetY;
        *z = result.eyeOffsetZ;
    }

    void CDnkvwContext::loadFps(float *fps)
    {
        auto result = m_winCalc.obtainLastResult();
        *fps = result.fps;
    }

    void CDnkvwContext::debugCameraInput(int cameraId)
    {
        logger(ELog::VERBOSE) << "DEBUG Input";
        m_winCalc.debugCameraInput(cameraId);
    }

    void CDnkvwContext::debugCameraFace(int cameraId)
    {
        logger(ELog::VERBOSE) << "DEBUG Tracking";
        m_winCalc.debugCameraFace(cameraId);
    }

} // namespace dnkvw
