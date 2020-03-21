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
        std::cout << "2) DEBUG: Show face tracking HaarCascade\n";
        std::cout << "3) DEBUG: Show face tracking DNN\n";
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
