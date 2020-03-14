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

    DNKVW_API void dnkvw_selectHaarTracker(IDnkvwHandle context);
    DNKVW_API void dnkvw_selectDnnTracker(IDnkvwHandle context);

    DNKVW_API int dnkvw_startTracking(IDnkvwHandle context, int cameraId);
    DNKVW_API void dnkvw_stopTracking(IDnkvwHandle context);

    DNKVW_API void dnkvw_debugCameraInput(IDnkvwHandle context);
    DNKVW_API void dnkvw_debugCameraFace(IDnkvwHandle context);

#ifdef __cplusplus
}
#endif

#endif
