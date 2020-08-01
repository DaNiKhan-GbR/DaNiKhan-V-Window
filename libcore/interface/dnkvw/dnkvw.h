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

    /**
     * This method creates a new dnkvw context.
     * It is needed for all operations in this library.
     * Do not forget to free it after you are done with it.
     * 
     * @return the newly created context
     */
    DNKVW_API IDnkvwHandle dnkvw_createContext();

    /**
     * This method deinitializes and frees a dnkw context.
     * If you use C NEVER use 'free()' to free an existing context.
     * If you use C++ NEVER use 'delete' to free an existing context.
     * Only use this method to free a context.
     * 
     * @param context a pointer to an initialized context
     */
    DNKVW_API void dnkvw_freeContext(IDnkvwHandle* context);

    /**
     * Set the log level.
     * 
     * @param context an initialized dnkvw context
     * @param logLevel the log level
     */
    DNKVW_API void dnkvw_setLogLevel(IDnkvwHandle context, int logLevel);

    /**
     * Set the internal log level.
     * Not intended for production use.
     * 
     * @param context an initialized dnkvw context
     * @param internalLogLevel the log level
     */
    DNKVW_API void dnkvw_setInternalLogLevel(IDnkvwHandle context, int internalLogLevel);

    /**
     * Use Haar tracking. 
     * Overwrites previously selected tracker.
     * 
     * @param context an initialized dnkvw context
     * @return 1 if the selection was successful, 0 if not
     */
    DNKVW_API int dnkvw_selectHaarTracker(IDnkvwHandle context);

    /**
     * Use DNN tracking. 
     * Overwrites previously selected tracker.
     * 
     * @param context an initialized dnkvw context
     * @return 1 if the selection was successful, 0 if not
     */
    DNKVW_API int dnkvw_selectDnnTracker(IDnkvwHandle context);

    /**
     * Starts the tracking.
     * 
     * @param context an initialized dnkvw context
     * @param cameraId the Id of the camera to be uses for tracking
     * 
     * @return 1 if the tracking could be started successfully. 
     *         A 0 value may indicate a problem with the camera.
     */
    DNKVW_API int dnkvw_startTracking(IDnkvwHandle context, int cameraId);

    /**
     * Stops the tracking.
     * This function may block.
     * 
     * @param context an initialized dnkvw context
     */
    DNKVW_API void dnkvw_stopTracking(IDnkvwHandle context);

    /**
     * Stops the tracking as soon as possible without blocking.
     * 
     * @param context an initialized dnkvw context
     */
    DNKVW_API void dnkvw_stopTrackingAsync(IDnkvwHandle context);

    /**
     * Configures the viewing frustum according to the parameters.
     * 
     * @param context an initialized dnkvw context
     * @param aspectRatio the aspect ratio
     * @param nearPlane the near plane
     */
    DNKVW_API void dnkvw_configureFrustum(IDnkvwHandle context, float aspectRatio, float nearPlane);

    /**
     * Runs a short calibration routine.
     * The user should sit or stand as centered as possible before this
     * function is called. The position of the user will be used as the new
     * center until a new calibration is started.
     * 
     * This method should be called shortly after the start of the tracking 
     * to correct perspective problems caused by a wrong starting center.
     * 
     * @param context an initialized dnkvw context
     */
    DNKVW_API void dnkvw_calibrate(IDnkvwHandle context);

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
     * @param context an initialized dnkvw context
     * @param left a pointer to a float where the left result can be saved
     * @param right a pointer to a float where the right result can be saved
     * @param top a pointer to a float where the top result can be saved
     * @param bottom a pointer to a float where the bottom result can be saved
     */
    DNKVW_API void dnkvw_loadFrustum(IDnkvwHandle context, float* left, float* right, float* top, float* bottom);
    
    /**
     * Get the last calculated eye offset.
     * 
     * @param context an initialized dnkvw context
     * @param x a pointer to a float where the X result can be saved
     * @param y a pointer to a float where the Y result can be saved
     * @param z a pointer to a float where the Z result can be saved
     */
    DNKVW_API void dnkvw_loadEyeOffset(IDnkvwHandle context, float* x, float* y, float* z);
    
    /**
     * Get the last FPS value.
     * 
     * @param context an initialized dnkvw context
     * @param fps a pointer to a float where the result can be saved
     */
    DNKVW_API void dnkvw_loadFps(IDnkvwHandle context, float* fps);
    
    /**
     * Debugging function to test the camera input. 
     * Should not be called while the tracking thread is running.
     * This function may block.
     * Don't use this in production code.
     * 
     * @param context an initialized dnkvw context
     * @param cameraId Id of the camera to use for tracking
     */
    DNKVW_API void dnkvw_debugCameraInput(IDnkvwHandle context, int cameraId);

    /**
     * Debugging function to test the face detection. 
     * Should not be called while the tracking thread is running.
     * This function may block.
     * Don't use this in production code.
     * 
     * @param context an initialized dnkvw context
     * @param cameraId Id of the camera to use for tracking
     */
    DNKVW_API void dnkvw_debugCameraFace(IDnkvwHandle context, int cameraId);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // DNKVW_HEADER
