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
 * @brief DaNiKhan V-Window devtool.
 */

#include <iostream>
#include <dnkvw/dnkvw.h>

void logLevelDialog(IDnkvwHandle dnkvw);

int main()
{
    std::cout << "Dnkvw Devtool\n";
    std::cout << "=============\n\n";

    IDnkvwHandle dnkvw = dnkvw_createContext();

    bool running = true;
    while (running)
    {
        std::cout << "\nMenu\n----\n";
        std::cout << "1) DEBUG: Show video stream\n";
        std::cout << "2) DEBUG: Show face tracking HaarCascade\n";
        std::cout << "3) DEBUG: Show face tracking DNN\n";
        std::cout << "9) Set log level\n";
        std::cout << "q) Quit\n";

        char input;
        std::cin >> input;

        switch (input)
        {
            case '1':
            {
                std::cout << "Press any key to exit in open window.\n";
                dnkvw->debugCameraInput(0);
                std::cout << "Window closed.\n";
                break;
            }

            case '2':
            {
                std::cout << "Press any key to exit in open window.\n";
                dnkvw->selectHaarTracker();
                dnkvw->debugCameraFace(0);
                std::cout << "Window closed.\n";
                break;
            }

            case '3':
            {
                std::cout << "Press any key to exit in open window.\n";
                dnkvw->selectDnnTracker();
                dnkvw->debugCameraFace(0);
                std::cout << "Window closed.\n";
                break;
            }

            case '9':
                logLevelDialog(dnkvw);
                break;
            
            case 'q':
                running = false;
                break;
            
            default:
                std::cout << "Unknown operation!\n";
                break;
        }
    }

    dnkvw_freeContext(&dnkvw);

    std::cout << "Bye!\n";
    
    return EXIT_SUCCESS;
}

void logLevelDialog(IDnkvwHandle dnkvw)
{
    bool running = true;
    while (running)
    {
        std::cout << "\nLog Level Submenu\n----\n";
        std::cout << "1) Log Level SILENT\n";
        std::cout << "2) Log Level ERROR\n";
        std::cout << "3) Log Level WARNING\n";
        std::cout << "4) Log Level VERBOSE\n";
        std::cout << "5) Internal Log Level SILENT\n";
        std::cout << "6) Internal Log Level IMPORTANT\n";
        std::cout << "7) Internal Log Level VERBOSE\n";
        std::cout << "q) Exit menu\n";

        char input;
        std::cin >> input;

        switch (input)
        {
            case '1':
                dnkvw->setLogLevel(DNKVW_LOG_LEVEL_SILENT);
                break;
            
            case '2':
                dnkvw->setLogLevel(DNKVW_LOG_LEVEL_ERROR);
                break;
            
            case '3':
                dnkvw->setLogLevel(DNKVW_LOG_LEVEL_WARNING);
                break;
            
            case '4':
                dnkvw->setLogLevel(DNKVW_LOG_LEVEL_VERBOSE);
                break;
            
            case '5':
                dnkvw->setInternalLogLevel(DNKVW_INTERNAL_LOG_LEVEL_SILENT);
                break;
            
            case '6':
                dnkvw->setInternalLogLevel(DNKVW_INTERNAL_LOG_LEVEL_IMPORTANT);
                break;
            
            case '7':
                dnkvw->setInternalLogLevel(DNKVW_INTERNAL_LOG_LEVEL_VERBOSE);
                break;
            
            case 'q':
                running = false;
                break;
            
            default:
                std::cout << "Unknown operation!\n";
                break;
        }
    }
}
