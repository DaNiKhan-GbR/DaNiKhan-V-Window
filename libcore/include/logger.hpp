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
    /**
     * Enum for the different Log Levels.
     */
    enum class ELog : int
    {
        SILENT   = 0,   ///< Output no logging messages
        ERROR    = 1,   ///< Output error messages
        WARNING  = 2,   ///< Output warning messages
        VERBOSE  = 3,   ///< Output a lot of meta information

        _COUNT          ///< Count of Log Levels
    };

    /**
     * Typedef for a stream manipulator.
     */
    typedef std::ios_base& (*manip) (std::ios_base&);

    /* Forward declaration for the CLogger. */
    class CLogger;

    /**
     * Log Output Stream for a single log level.
     */
    class CLogStream
    {
        private:
            CLogStream() = default;
            /**
             * Initializes this output stream.
             * 
             * @param logLevel the log level of this stream
             */
            void init(ELog logLevel);

            /**
             * The log level of this output stream.
             */
            ELog m_logLevel;

            /**
             * The printable name of the log level of this stream.
             */
            char *m_logLevelName;
        
        friend dnkvw::CLogger;
        template <class T> friend void operator<<(CLogStream& logStream, const T& x);
        friend CLogStream& operator<<(CLogStream& logStream, manip manipulator);
    };

    /**
     * Logger control class.
     * Provides access to all log output streams.
     * 
     * You should only use the global instance of this class!
     */
    class CLogger
    {
        public:
            /**
             * Constructor initializes all output streams.
             * 
             * You should only use the global instance of this class!
             */
            CLogger();

            /**
             * Set the maximum log level to output.
             * All messages with a higher log level are omitted.
             * 
             * @param logLevel the maximum log level
             */
            void setLogLevel(ELog logLevel);

            /**
             * Access a log output stream for a specific log level.
             * 
             * @code{.cpp}
             * logger(ELog::VERBOSE) << "<Log Message>";
             * @endcode
             * 
             * @param logLevel the log level of the wanted output stream
             * @return the correct output stream for the requested log level
             */
            CLogStream& operator()(ELog logLevel);

        private:
            /**
             * The maximal log level.
             */
            ELog m_logLevel;

            /**
             * Array of log output streams.
             */
            CLogStream m_logStreams[static_cast<int>(ELog::_COUNT)];

            /**
             * Mutex for the thread-safe access to std::cerr.
             */
            std::mutex m_mutex;
        
        template <class T> friend void operator<<(CLogStream& logStream, const T& x);
        friend CLogStream& operator<<(CLogStream& logStream, manip manipulator); 
    };

    /**
     * Global logger object.
     */
    extern CLogger logger;

    /**
     * Stream operator for writing to a log output stream.
     * Messages only get printed when the log level of this output stream is lower
     * or equal to the log level of the logger.
     * 
     * Messages can't be chained.
     * 
     * @param logStream the stream to write to
     * @param x the object to write
     */
    template <class T> void operator<<(CLogStream& logStream, const T& x) 
    { 
        if (logStream.m_logLevel != ELog::SILENT)
        {
            std::lock_guard<std::mutex> guard(logger.m_mutex);
            std::cerr << logStream.m_logLevelName << x << "\n";
        }
    }

    /**
     * Stream operator for accepting stream manipulators.
     * 
     * Contrary to messages, manipulators can be chained.
     * 
     * @param logStream the stream to write to
     * @param manipulator the manipulator to use
     * @return the logStream parameter for chaining
     */
    CLogStream& operator<<(CLogStream& logStream, manip manipulator);
}
