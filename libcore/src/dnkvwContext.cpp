#include "dnkvwContext.hpp"
#include "constants.hpp"
#include "haarTracker.hpp"
#include "dnnTracker.hpp"

#include <cmath>
#include <cstring>

using dnkvw::CDnkvwContext;

void CDnkvwContext::__init()
{
    // TODO not needed??
}

void CDnkvwContext::__cleanup()
{
    // TODO not needed??
}

bool CDnkvwContext::selectHaarTracker()
{
    return m_winCalc.selectTracker(new CHaarTracker);
}

bool CDnkvwContext::selectDnnTracker()
{
    return m_winCalc.selectTracker(new CDnnTracker);
}

bool CDnkvwContext::startTracking(int cameraId)
{
    m_winCalc.start(cameraId);

    return true; // TODO implement a correct success status
}

void CDnkvwContext::stopTracking()
{
    // TODO Async or not? Maybe let the application decide?
    m_winCalc.stop();
}

void CDnkvwContext::configureFrustum(float aspectRatio, float nearPlane)
{
    CWindowSettings settings;
    settings.aspect = aspectRatio;
    settings.near = nearPlane;
    settings.reseved1 = 0.0f;
    m_winCalc.storeSettings(settings);
}

void CDnkvwContext::calibrate()
{
    m_winCalc.signalCalibrate();
}

void CDnkvwContext::loadFrustum(float *left, float *right, float *top, float *bottom, float *fps)
{
    auto result = m_winCalc.obtainLastResult();
    *left = result.left;
    *right = result.right;
    *bottom = result.bottom;
    *top = result.top;

    if (fps)
    {
        *fps = result.fps;
    }
}

void CDnkvwContext::debugCameraInput(int cameraId)
{
    m_winCalc.debugCameraInput(cameraId);
}

void CDnkvwContext::debugCameraFace(int cameraId)
{
    m_winCalc.debugCameraFace(cameraId);
}

