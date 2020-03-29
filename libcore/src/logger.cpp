//////////////////////////////////////////////////////////////////////////////////////
//  BSD 3-Clause License                                                            //
//                                                                                  //
//  Copyright (c) 2020, Daniel Klintworth and Nicolas Hollmann                      //
//  All rights reserved.                                                            //
//                                                                                  //
//  Redistribution and use in source and binary forms, with or without              //
//  modification, are permitted provided that the following conditions are met:     //
//                                                                                  //
//  * Redistributions of source code must retain the above copyright notice, this   //
//    list of conditions and the following disclaimer.                              //
//                                                                                  //
//  * Redistributions in binary form must reproduce the above copyright notice,     //
//    this list of conditions and the following disclaimer in the documentation     //
//    and/or other materials provided with the distribution.                        //
//                                                                                  //
//  * Neither the name of the copyright holder nor the names of its                 //
//    contributors may be used to endorse or promote products derived from          //
//    this software without specific prior written permission.                      //
//                                                                                  //
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"     //
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE       //
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  //
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE    //
//  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL      //
//  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR      //
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      //
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   //
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   //
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            //
//                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////

/**
 * @file
 * @author Nicolas Hollmann
 * @author Daniel Klintworth
 * @brief Logging Utilities.
 */

#include "logger.hpp"

namespace dnkvw {

    /**
     * Global Logger instance.
     */
    CLogger logger;

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

    CLogStream& CLogger::operator()(ELog logLevel)
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

    CLogStream& operator<<(CLogStream& logStream, manip manipulator)
    {
        if (logStream.m_logLevel != ELog::SILENT)
        {
            std::lock_guard<std::mutex> guard(logger.m_mutex);
            std::cerr << manipulator;
        }
        return logStream;
    }

} // namespace dnkvw
