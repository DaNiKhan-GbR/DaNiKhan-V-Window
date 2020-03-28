#include "dnkvwContext.hpp"
#include "constants.hpp"
#include "haarTracker.hpp"
#include "dnnTracker.hpp"
#include "logger.hpp"

#include <cmath>
#include <cstring>
#include <opencv2/core/utils/logger.hpp>

using dnkvw::CDnkvwContext;

CDnkvwContext::CDnkvwContext()
{
    #ifdef NDEBUG // NDEBUG = NOT DEBUG
        cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
        logger.setLogLevel(ELog::WARNING);
    #else
        cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
        logger.setLogLevel(ELog::VERBOSE);
    #endif
}

void CDnkvwContext::setLogLevel(int logLevel)
{
    if (m_winCalc.isRunning())
    {
        logger(ELog::WARNING) << "Cannot set a new log level while tracking is active!";
        return;
    }

    ELog level;

    switch (logLevel)
    {
        case DNKVW_LOG_LEVEL_SILENT:
            level = ELog::SILENT;
            break;
            
        case DNKVW_LOG_LEVEL_ERROR:
            level = ELog::ERROR;
            break;
        
        case DNKVW_LOG_LEVEL_WARNING:
            level = ELog::WARNING;
            break;

        case DNKVW_LOG_LEVEL_VERBOSE:
            level = ELog::VERBOSE;
            break;

        default:
            logger(ELog::WARNING) << "Unkown log level!";
            return;
    }

    logger.setLogLevel(level);
}

void CDnkvwContext::setInternalLogLevel(int internalLogLevel)
{
    cv::utils::logging::LogLevel level;

    switch (internalLogLevel)
    {
        case DNKVW_INTERNAL_LOG_LEVEL_SILENT:
            level = cv::utils::logging::LOG_LEVEL_SILENT;
            break;
            
        case DNKVW_INTERNAL_LOG_LEVEL_IMPORTANT:
            level = cv::utils::logging::LOG_LEVEL_WARNING;
            break;

        case DNKVW_INTERNAL_LOG_LEVEL_VERBOSE:
            level = cv::utils::logging::LOG_LEVEL_VERBOSE;
            break;

        default:
            logger(ELog::WARNING) << "Unkown internal log level!";
            return;
    }

    cv::utils::logging::setLogLevel(level);
}

bool CDnkvwContext::selectHaarTracker()
{
    return m_winCalc.selectTracker(std::make_unique<CHaarTracker>());
}

bool CDnkvwContext::selectDnnTracker()
{
    return m_winCalc.selectTracker(std::make_unique<CDnnTracker>());
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
    logger(ELog::VERBOSE) << "DEBUG Input";
    m_winCalc.debugCameraInput(cameraId);
}

void CDnkvwContext::debugCameraFace(int cameraId)
{
    logger(ELog::VERBOSE) << "DEBUG Tracking";
    m_winCalc.debugCameraFace(cameraId);
}

