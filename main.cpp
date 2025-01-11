#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include "network/Client.h"
#include "view/Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    RECT rc = { 200, 200, 1800, 1200 };
    Application app = Application(rc);

    Client::Fetch("9784764904057");

    // app.Init();
    //
    // ShowWindow(app.Window(), nCmdShow);
    // UpdateWindow(app.Window());
    //
    // MSG msg = {};
    // while (GetMessage(&msg, nullptr, 0, 0))
    // {
    //     TranslateMessage(&msg);
    //     DispatchMessage(&msg);
    // }

    return 0;
}
