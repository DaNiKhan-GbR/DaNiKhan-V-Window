#include "dnkvw/dnkvw.h"
#include "dnkvwContext.hpp"

extern "C" {

    DNKVW_API IDnkvwHandle dnkvw_createContext()
    {
        IDnkvwHandle newHandle = new dnkvw::CDnkvwContext;
        newHandle->__init();
        return newHandle;
    }

    DNKVW_API void dnkvw_freeContext(IDnkvwHandle* context)
    {
        if (*context != nullptr)
        {
            (*context)->__cleanup();
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

    DNKVW_API void dnkvw_calibrate(IDnkvwHandle context)
    {
        context->calibrate();
    }

    DNKVW_API void dnkvw_loadFrustum(IDnkvwHandle context, float near, float *left, float *right, float *top, float *bottom)
    {
        context->loadFrustum(near, left, right, top, bottom);
    }

    DNKVW_API void dnkvw_debugCameraInput(IDnkvwHandle context)
    {
        context->debugCameraInput();
    }

    DNKVW_API void dnkvw_debugCameraFace(IDnkvwHandle context)
    {
        context->debugCameraFace();
    }
}
