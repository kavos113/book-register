#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <fcntl.h>
#include <io.h>

#include "db/Database.h"
#include "network/Client.h"
#include "view/Application.h"
#include "db/Parser.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    RECT rc = { 200, 200, 1800, 1200 };
    Application app = Application(rc);

    Parser::Initialize();
    Database::Connect();

    app.Init();

    ShowWindow(app.Window(), nCmdShow);
    UpdateWindow(app.Window());

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(app.Window(), app.Accel(), &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    Parser::Terminate();
    Database::Disconnect();

    return 0;
}
