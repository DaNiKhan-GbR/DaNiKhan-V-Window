#include <iostream>
#include <dnkvw/dnkvw.h>

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
        std::cout << "2) DEBUG: Show face tracking\n";
        std::cout << "3) DEBUG: Show face tracking DNN\n";
        std::cout << "q) Quit\n";

        char input;
        std::cin >> input;

        switch (input)
        {
            case '1':
            {
                std::cout << "Starting Tracking...\t\t";
                bool ok = dnkvw->startTracking(0);
                std::cout << (ok ? "OK.\n" : "Error!\n");

                std::cout << "Open DEBUG Window: Camera Input.\nPress any key to exit.\n";
                dnkvw->debugCameraInput();
                std::cout << "Window closed.\n";

                std::cout << "Stopping Tracking...\t\t";
                dnkvw->stopTracking();
                std::cout << "OK.\n";
                break;
            }

            case '2':
            {
                std::cout << "Starting Tracking...\t\t";
                bool ok = dnkvw->startTracking(0);
                std::cout << (ok ? "OK.\n" : "Error!\n");

                std::cout << "Open DEBUG Window: Face Tracking.\nPress any key to exit.\n";
                dnkvw->debugCameraFace();
                std::cout << "Window closed.\n";

                std::cout << "Stopping Tracking...\t\t";
                dnkvw->stopTracking();
                std::cout << "OK.\n";
                break;
            }

            case '3':
            {
                std::cout << "Starting Tracking...\t\t";
                bool ok = dnkvw->startTracking(0);
                std::cout << (ok ? "OK.\n" : "Error!\n");

                std::cout << "Open DEBUG Window: Face Tracking DNN.\nPress any key to exit.\n";
                dnkvw->debugCameraFaceDNN();
                std::cout << "Window closed.\n";

                std::cout << "Stopping Tracking...\t\t";
                dnkvw->stopTracking();
                std::cout << "OK.\n";
                break;
            }
            
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
