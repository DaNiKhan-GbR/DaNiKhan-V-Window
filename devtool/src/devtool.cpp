#include <windows.h>
#include <dnkvw/dnkvw.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    wchar_t buffer[256];
    wsprintfW(buffer, L"%d", addNums(10, 7));
    MessageBoxW(NULL, buffer, L"DaNiKhan V-Window Devtool", 0);

    return 0;
}
