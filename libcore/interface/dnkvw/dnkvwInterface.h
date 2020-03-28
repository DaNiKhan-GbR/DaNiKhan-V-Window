/**
 * @file dnkvwinterface.h
 * 
 * This is the public C++ API for DaNiKhan Virtual Window (dnkvw).
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
    virtual void loadFrustum(float *left, float *right, float *top, float *bottom, float *fps = nullptr) = 0;

    virtual void debugCameraInput(int cameraId) = 0;
    virtual void debugCameraFace(int cameraId) = 0;
};

typedef IDnkvwContext* IDnkvwHandle;

#else

typedef void* IDnkvwHandle;

#endif

#endif
