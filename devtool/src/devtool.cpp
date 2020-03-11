#include <iostream>
#include <dnkvw/dnkvw.h>

int main()
{
    openVideo();

    char input = ' ';
    while (input == ' ')
    {
        captureVideoFrame();
        std::cin >> input;
    }
    
    

    stopVideo();
    return EXIT_SUCCESS;
}
