/**
 * @file dnkvwinterface.h
 * 
 * This is the public C++ API for DaNiKhan Virtual Window (dnkvw).
 */

#ifndef DNKVW_INTERFACE_HEADER
#define DNKVW_INTERFACE_HEADER

#ifdef __cplusplus

struct IDnkvwContext 
{
    virtual ~IDnkvwContext() {}

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
