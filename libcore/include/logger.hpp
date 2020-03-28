/**
 * @file log.hpp
 * 
 * Logging Utilities.
 */

#pragma once

#include <iostream>
#include <mutex>

namespace dnkvw 
{
    enum class ELog : int
    {
        SILENT   = 0,
        ERROR    = 1,
        WARNING  = 2,
        VERBOSE  = 3,

        _COUNT 
    };

    typedef std::ostream& (*manip) (std::ostream&);
    class CLogger;

    class CLogStream
    {
        private:
            void init(ELog logLevel);

            ELog m_logLevel;
            char *m_logLevelName;
        
        friend dnkvw::CLogger;
        template <class T> friend CLogStream& operator<<(CLogStream& logStream, const T& x);
        friend CLogStream& operator<<(CLogStream& logStream, manip manipulator);
    };

    class CLogger
    {
        public:
            CLogger();
            void setLogLevel(ELog logLevel);
            CLogStream operator()(ELog logLevel);

        private:
            ELog m_logLevel;
            CLogStream m_logStreams[static_cast<int>(ELog::_COUNT)];
            std::mutex m_mutex;
        
        template <class T> friend CLogStream& operator<<(CLogStream& logStream, const T& x);
        friend CLogStream& operator<<(CLogStream& logStream, manip manipulator); 
    };

    extern CLogger logger;

    template <class T> CLogStream& operator<<(CLogStream& logStream, const T& x) 
    { 
        if (logStream.m_logLevel != ELog::SILENT)
        {
            std::lock_guard<std::mutex> guard(logger.m_mutex);
            std::cerr << logStream.m_logLevelName << x << "\n";
        }
        return logStream; 
    }

    CLogStream& operator<<(CLogStream& logStream, manip manipulator);
}
