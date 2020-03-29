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
 * @brief FPS Timer Utility.
 */

#pragma once

#include <chrono>
#include "constants.hpp"

namespace dnkvw 
{
    /**
     * FPS Timer class.
     * Measures the time between two time points and returns the frames per second.
     */
    class CFpsTimer
    {
        public:
            /**
             * Constructor of the FPS Timer.
             * 
             * @param alpha the smoothing factor, 0.0 is no smoothing. 0.0 <= alpha < 1.0 
             */
            CFpsTimer(float alpha = 0.5f) : m_alpha(alpha), m_avgFps(constant::capture::targetFps) {}

            /**
             * Start the timer.
             */
            void start();

            /**
             * Stop the timer and return the measured fps with smoothing.
             * 
             * @return the fps
             */
            float stop();

        private:
            /**
             * The clock for getting the time.
             */
            std::chrono::high_resolution_clock m_timer;

            /**
             * The start point of the active measurment.
             */
            std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

            /**
             * The average fps for the moving average.
             */
            float m_avgFps;

            /**
             * The configured alpha for the smoothing.
             * 0.0 <= alpha < 1.0
             */
            float m_alpha;
    };

}

