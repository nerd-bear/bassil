#define _MAIN_WINDOW_

#ifndef _WINDOWS_
    #include <windows.h>
#endif

#ifndef _IOSTREAM_ 
    #include <iostream>
#endif

#ifndef _STRING_
    #include <string>
#endif

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        // Handle other messages here
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t WINDOW[] = L"Orion Shell TEST GUI";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = WINDOW;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        WINDOW,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    std::string var1;
    std::cin >> var1;

    return 0;
}