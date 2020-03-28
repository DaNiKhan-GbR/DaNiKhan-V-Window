/**
 * @file dnkvw.h
 * 
 * This is the public C/C++ API for DaNiKhan Virtual Window (dnkvw).
 */

#ifndef DNKVW_HEADER
#define DNKVW_HEADER

#include "dnkvwInterface.h"

#ifdef _WIN32
# ifdef DNKVW_WIN_EXPORT
#   define DNKVW_API  __declspec( dllexport )
# else
#   define DNKVW_API  __declspec( dllimport )
# endif
#else
# define DNKVW_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

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
#endif

#endif
