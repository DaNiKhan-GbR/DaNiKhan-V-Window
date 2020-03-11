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

    DNKVW_API int dnkvw_startTracking(IDnkvwHandle context, int cameraId)
    {
        return context->startTracking(cameraId) ? 1 : 0;
    }

    DNKVW_API void dnkvw_stopTracking(IDnkvwHandle context)
    {
        context->stopTracking();
    }

    DNKVW_API void dnkvw_debugCameraInput(IDnkvwHandle context)
    {
        context->debugCameraInput();
    }
}
