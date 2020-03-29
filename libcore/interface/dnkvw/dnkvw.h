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
 * @brief DaNiKhan V-Window interface.
 * 
 * This is the public C/C++ API for DaNiKhan Virtual Window (dnkvw).
 * Include only this file if you want to use DNKVW for your own projects.
 */

#ifndef DNKVW_HEADER
#define DNKVW_HEADER

#include "dnkvwInterface.h"

/* Windows DLL Export/Import definitions */
#ifdef _WIN32
# ifdef DNKVW_WIN_EXPORT
#   define DNKVW_API  __declspec( dllexport )
# else
#   define DNKVW_API  __declspec( dllimport )
# endif
#else
# define DNKVW_API
#endif

/* Universal dnkvw interface */
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    DNKVW_API IDnkvwHandle dnkvw_createContext();
    DNKVW_API void dnkvw_freeContext(IDnkvwHandle* context);

    DNKVW_API void dnkvw_setLogLevel(IDnkvwHandle context, int logLevel);
    DNKVW_API void dnkvw_setInternalLogLevel(IDnkvwHandle context, int internalLogLevel);

    DNKVW_API void dnkvw_selectHaarTracker(IDnkvwHandle context);
    DNKVW_API void dnkvw_selectDnnTracker(IDnkvwHandle context);

    DNKVW_API int dnkvw_startTracking(IDnkvwHandle context, int cameraId);
    DNKVW_API void dnkvw_stopTracking(IDnkvwHandle context);

    DNKVW_API void dnkvw_configureFrustum(IDnkvwHandle context, float aspectRatio, float nearPlane);
    DNKVW_API void dnkvw_calibrate(IDnkvwHandle context);
    DNKVW_API void dnkvw_loadFrustum(IDnkvwHandle context, float* left, float* right, float* top, float* bottom, float* fps);

    DNKVW_API void dnkvw_debugCameraInput(IDnkvwHandle context, int cameraId);
    DNKVW_API void dnkvw_debugCameraFace(IDnkvwHandle context, int cameraId);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // DNKVW_HEADER
