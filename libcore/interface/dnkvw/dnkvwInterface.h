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
 * @brief DNKVW Public C++ interface.
 * 
 * This is the public C++ API for DaNiKhan Virtual Window (dnkvw).
 * Do not include this file directly! Only import the dnkvw.h file.
 */

#ifndef DNKVW_INTERFACE_HEADER
#define DNKVW_INTERFACE_HEADER

#define DNKVW_LOG_LEVEL_SILENT  0
#define DNKVW_LOG_LEVEL_WARNING 1
#define DNKVW_LOG_LEVEL_ERROR   2
#define DNKVW_LOG_LEVEL_VERBOSE 3

#define DNKVW_INTERNAL_LOG_LEVEL_SILENT    0
#define DNKVW_INTERNAL_LOG_LEVEL_IMPORTANT 1
#define DNKVW_INTERNAL_LOG_LEVEL_VERBOSE   2

#ifdef __cplusplus

struct IDnkvwContext 
{
    virtual ~IDnkvwContext() {}

    virtual void setLogLevel(int logLevel) = 0;
    virtual void setInternalLogLevel(int internalLogLevel) = 0;

    virtual bool selectHaarTracker() = 0;
    virtual bool selectDnnTracker() = 0;

    virtual bool startTracking(int cameraId) = 0;
    virtual void stopTracking() = 0;

    virtual void configureFrustum(float aspectRatio, float nearPlane) = 0;
    virtual void calibrate() = 0;
    virtual void loadFrustum(float *left, float *right, float *top, float *bottom) = 0;
    virtual void loadEyeOffset(float *x, float *y, float *z) = 0;
    virtual void loadFps(float *fps) = 0;

    virtual void debugCameraInput(int cameraId) = 0;
    virtual void debugCameraFace(int cameraId) = 0;
};

typedef IDnkvwContext* IDnkvwHandle;

#else

typedef void* IDnkvwHandle;

#endif

#endif
