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
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

                wchar_t buffer[256] = {0};
                wsprintfW(buffer, L"%d", addNums(3, 4));
                TextOutW(hdc, 20, 20, buffer, 20);

                EndPaint(hwnd, &ps);
            }
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
