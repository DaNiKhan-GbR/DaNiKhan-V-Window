#ifndef DNKVW_INTERFACE_HEADER
#define DNKVW_INTERFACE_HEADER

#ifdef __cplusplus

struct IDnkvwContext 
{
    virtual void __init() = 0;
    virtual void __cleanup() = 0;

    virtual bool selectHaarTracker() = 0;
    virtual bool selectDnnTracker() = 0;

    virtual bool startTracking(int cameraId) = 0;
    virtual void stopTracking() = 0;

    virtual void loadFrustum(float near, float *left, float *right, float *top, float *bottom) = 0;

    virtual void debugCameraInput() = 0;
    virtual void debugCameraFace() = 0;
};

typedef IDnkvwContext* IDnkvwHandle;

#else

typedef void* IDnkvwHandle;

#endif

#endif
