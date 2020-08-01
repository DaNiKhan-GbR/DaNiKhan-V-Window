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
 * @brief DaNiKhan V-Window public interface implementation.
 * 
 * This file implements the public interface. It allows the safe creation and
 * destruction of a DnkvwContext (IDnkvwHandle).
 * Also for all methods of the IDnkvwContext there is a C wrapper.
 */

#include "dnkvw/dnkvw.h"
#include "dnkvwContext.hpp"

extern "C" {

    DNKVW_API IDnkvwHandle dnkvw_createContext()
    {
        return new dnkvw::CDnkvwContext;
    }

    DNKVW_API void dnkvw_freeContext(IDnkvwHandle* context)
    {
        if (*context != nullptr)
        {
            delete *context;
            *context = nullptr;
        }
    }

    DNKVW_API void dnkvw_setLogLevel(IDnkvwHandle context, int logLevel)
    {
        context->setLogLevel(logLevel);
    }

    DNKVW_API void dnkvw_setInternalLogLevel(IDnkvwHandle context, int internalLogLevel)
    {
        context->setInternalLogLevel(internalLogLevel);
    }

    DNKVW_API int dnkvw_selectHaarTracker(IDnkvwHandle context)
    {
        return context->selectHaarTracker() ? 1 : 0;
    }

    DNKVW_API int dnkvw_selectDnnTracker(IDnkvwHandle context)
    {
        return context->selectDnnTracker() ? 1 : 0;
    }

    DNKVW_API int dnkvw_startTracking(IDnkvwHandle context, int cameraId)
    {
        return context->startTracking(cameraId) ? 1 : 0;
    }

    DNKVW_API void dnkvw_stopTracking(IDnkvwHandle context)
    {
        context->stopTracking();
    }

    DNKVW_API void dnkvw_stopTrackingAsync(IDnkvwHandle context)
    {
        context->stopTrackingAsync();
    }

    DNKVW_API void dnkvw_configureFrustum(IDnkvwHandle context, float aspectRatio, float nearPlane)
    {
        context->configureFrustum(aspectRatio, nearPlane);
    }

    DNKVW_API void dnkvw_calibrate(IDnkvwHandle context)
    {
        context->calibrate();
    }

    DNKVW_API void dnkvw_loadFrustum(IDnkvwHandle context, float* left, float* right, float* top, float* bottom)
    {
        context->loadFrustum(left, right, top, bottom);
    }

    DNKVW_API void dnkvw_loadEyeOffset(IDnkvwHandle context, float* x, float* y, float* z)
    {
        context->loadEyeOffset(x, y, z);
    }

    DNKVW_API void dnkvw_loadFps(IDnkvwHandle context, float* fps)
    {
        context->loadFps(fps);
    }

    DNKVW_API void dnkvw_debugCameraInput(IDnkvwHandle context, int cameraId)
    {
        context->debugCameraInput(cameraId);
    }

    DNKVW_API void dnkvw_debugCameraFace(IDnkvwHandle context, int cameraId)
    {
        context->debugCameraFace(cameraId);
    }
}
