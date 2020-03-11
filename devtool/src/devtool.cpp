#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <dnkvw/dnkvw.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    const wchar_t szWindowClass[] = L"DNKVW Devtool Mainwin";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szWindowClass;
    wc.hIcon = LoadIconW(NULL, IDI_INFORMATION);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        szWindowClass,
        L"DaNiKhan V-Window Devtool",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, // Parent
        NULL, // Menu
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        return -1;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            openVideo();
            return 0;

        case WM_DESTROY:
            stopVideo();
            PostQuitMessage(0);
            return 0;
        
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

                EndPaint(hwnd, &ps);
                captureVideoFrame();
            }
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
