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
 * @brief Constant definitions.
 */

#pragma once

namespace dnkvw
{
    namespace constant
    {
        /**
         * Constants specific to the image capturing.
         */
        namespace capture
        {
            /**
             * This sets the target fps for the capture device.
             * 
             * If the device can't fullfill this request a warning is
             * generated but the library will mostly work as expected.
             */
            constexpr double targetFps = 30;

            /**
             * This sets the target image width for the capture device.
             * 
             * If the device can't fullfill this request a warning is
             * generated but the library will mostly work as expected.
             */
            constexpr double targetWidth = 640;

            /**
             * This sets the target image height for the capture device.
             * 
             * If the device can't fullfill this request a warning is
             * generated but the library will mostly work as expected.
             */
            constexpr double targetHeight = 480;
        }

        /**
         * Constants specific to the frustum calculation.
         */
        namespace frustum
        {
            /**
             * The time smoothing factor for the eye position to reduce
             * jiggle. A value of 1.0 disables the smoothing, a lower
             * value increases the smoothing.
             * 
             * The value should always be greater than 0.0 and equal or less than 1.0
             */
            constexpr float eyeAvgFactor = 0.25f;
        }

        /**
         * Constants specific to the calibration routine.
         */
        namespace calib
        {
            /**
             * Defines how many faces should be collected for the average 
             * base position of the face.
             * 
             * This value must be smaller than maxFaceTries!
             */
            constexpr int targetFaceCount = 10;

            /**
             * If no face is found while calibration, the target face count may not be reached.
             * In that case this constant defines after how many face collection tries the algorithm
             * should stop. If it stops, it will use all faces it could collect.
             * 
             * This value must be greater than targetFaceCount!
             */
            constexpr int maxFaceTries = 50;
        }
    }
}
