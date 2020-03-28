#include "logger.hpp"

using dnkvw::ELog;
using dnkvw::CLogStream;
using dnkvw::CLogger;

// Global Logger instance.
CLogger dnkvw::logger;

CLogger::CLogger()
{
    m_logLevel = ELog::VERBOSE;

    int logStreamCount = static_cast<int>(ELog::_COUNT);

    for (int i = 0; i < logStreamCount; ++i)
    {
        m_logStreams[i].init(static_cast<ELog>(i));
    }
}

void CLogger::setLogLevel(ELog logLevel)
{
    m_logLevel = logLevel;
}

CLogStream CLogger::operator()(ELog logLevel)
{
    if (logLevel == ELog::SILENT)
    {
        (*this)(ELog::WARNING) << "It's not allowed to output SILENT log messages!";
    }
    else if (logLevel <= m_logLevel)
    {
        int logStreamId = static_cast<int>(logLevel);
        return m_logStreams[logStreamId];
    }

    return m_logStreams[0];
}

void CLogStream::init(ELog logLevel)
{
    m_logLevel = logLevel;

    switch (logLevel)
    {
        case ELog::SILENT:
            m_logLevelName = "[SILENT (YOU SHOULD NEVER SEE THIS)] ";
            break;

        case ELog::ERROR:
            m_logLevelName = "[ERROR] ";
            break;
        
        case ELog::WARNING:
            m_logLevelName = "[WARNING] ";
            break;
        
        case ELog::VERBOSE:
            m_logLevelName = "[VERBOSE] ";
            break;
        
        case ELog::_COUNT:
        default:
            m_logLevelName = "[WTF??] ";
            break;
    }
}

CLogStream& dnkvw::operator<<(CLogStream& logStream, manip manipulator)
{
    if (logStream.m_logLevel != ELog::SILENT)
    {
        std::lock_guard<std::mutex> guard(logger.m_mutex);
        std::cerr << manipulator;
    }
    return logStream;
}
