#ifndef DNKVW_INTERFACE_HEADER
#define DNKVW_INTERFACE_HEADER

#ifdef __cplusplus

struct IDnkvwContext 
{
    virtual bool startTracking(int cameraId) = 0;
    virtual void stopTracking() = 0;

    virtual void debugCameraInput() = 0;
    virtual void debugCameraFace() = 0;
};

typedef IDnkvwContext* IDnkvwHandle;

#else

typedef void* IDnkvwHandle;

#endif

#endif
