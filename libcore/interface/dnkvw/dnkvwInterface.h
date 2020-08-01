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
    /**
     * The destructor deinitializes a context.
     * NEVER call this directly by using 'delete'.
     * ALWAYS use dnkvw_freeContext to free a context.
     */
    virtual ~IDnkvwContext() {}

    /**
     * Set the log level.
     * 
     * @param logLevel the log level
     */
    virtual void setLogLevel(int logLevel) = 0;
    
    /**
     * Set the internal log level.
     * Not intended for production use.
     * 
     * @param internalLogLevel the log level
     */
    virtual void setInternalLogLevel(int internalLogLevel) = 0;

    /**
     * Use Haar tracking. 
     * Overwrites previously selected tracker.
     * 
     * @return true if the selection was successful
     */
    virtual bool selectHaarTracker() = 0;

    /**
     * Use DNN tracking. 
     * Overwrites previously selected tracker.
     * 
     * @return true if the selection was successful
     */
    virtual bool selectDnnTracker() = 0;

    /**
     * Starts the tracking.
     * 
     * @param cameraId the Id of the camera to be uses for tracking
     * 
     * @return true if the tracking could be started successfully. 
     *         A false value may indicate a problem with the camera.
     */
    virtual bool startTracking(int cameraId) = 0;

    /**
     * Stops the tracking.
     * This function may block.
     */
    virtual void stopTracking() = 0;

    /**
     * Stops the tracking as soon as possible without blocking.
     */
    virtual void stopTrackingAsync() = 0;

    /**
     * Configures the viewing frustum according to the parameters.
     * 
     * @param aspectRatio the aspect ratio
     * @param nearPlane the near plane
     */
    virtual void configureFrustum(float aspectRatio, float nearPlane) = 0;

    /**
     * Runs a short calibration routine.
     * The user should sit or stand as centered as possible before this
     * function is called. The position of the user will be used as the new
     * center until a new calibration is started.
     * 
     * This method should be called shortly after the start of the tracking 
     * to correct perspective problems caused by a wrong starting center.
     */
    virtual void calibrate() = 0;
    
    /**
     * Get the last frustum.
     * The viewing frustum is the volume that may appear on the screen.
     * It is described by a near plane, a far plane and the distances of the
     * left, right, top and bottom plane based on the near plane.
     * This function returns only the left, right, top and bottom values.
     * The near and far plane can be set independently.
     * 
     * These values can be used to calculate the projection matrix. This is normaly
     * done by your 3D graphics framework. Look at our demos for some case specific
     * examples.
     * 
     * For more information see https://en.wikipedia.org/wiki/Viewing_frustum
     * 
     * @param left a pointer to a float where the left result can be saved
     * @param right a pointer to a float where the right result can be saved
     * @param top a pointer to a float where the top result can be saved
     * @param bottom a pointer to a float where the bottom result can be saved
     */
    virtual void loadFrustum(float *left, float *right, float *top, float *bottom) = 0;

    /**
     * Get the last calculated eye offset.
     * 
     * @param x a pointer to a float where the X result can be saved
     * @param y a pointer to a float where the Y result can be saved
     * @param z a pointer to a float where the Z result can be saved
     */
    virtual void loadEyeOffset(float *x, float *y, float *z) = 0;
    
    /**
     * Get the last FPS value.
     * 
     * @param fps a pointer to a float where the result can be saved
     */
    virtual void loadFps(float *fps) = 0;

    /**
     * Debugging function to test the camera input. 
     * Should not be called while the tracking thread is running.
     * This function may block.
     * Don't use this in production code.
     * 
     * @param cameraId Id of the camera to use for tracking
     */
    virtual void debugCameraInput(int cameraId) = 0;

    /**
     * Debugging function to test the face detection. 
     * Should not be called while the tracking thread is running.
     * This function may block.
     * Don't use this in production code.
     * 
     * @param cameraId Id of the camera to use for tracking
     */
    virtual void debugCameraFace(int cameraId) = 0;
};

typedef IDnkvwContext* IDnkvwHandle;

#else

typedef void* IDnkvwHandle;

#endif

#endif
