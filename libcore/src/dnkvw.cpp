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

    DNKVW_API void dnkvw_selectHaarTracker(IDnkvwHandle context)
    {
        context->selectHaarTracker();
    }

    DNKVW_API void dnkvw_selectDnnTracker(IDnkvwHandle context)
    {
        context->selectDnnTracker();
    }

    DNKVW_API int dnkvw_startTracking(IDnkvwHandle context, int cameraId)
    {
        return context->startTracking(cameraId) ? 1 : 0;
    }

    DNKVW_API void dnkvw_stopTracking(IDnkvwHandle context)
    {
        context->stopTracking();
    }

    DNKVW_API void dnkvw_configureFrustum(IDnkvwHandle context, float aspectRatio, float nearPlane)
    {
        context->configureFrustum(aspectRatio, nearPlane);
    }

    DNKVW_API void dnkvw_calibrate(IDnkvwHandle context)
    {
        context->calibrate();
    }

    DNKVW_API void dnkvw_loadFrustum(IDnkvwHandle context, float* left, float* right, float* top, float* bottom, float* fps)
    {
        context->loadFrustum(left, right, top, bottom, fps);
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
