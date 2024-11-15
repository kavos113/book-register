#include <windows.h>

#include "Application.h"

void Application::Init()
{
    WNDCLASSEX wc = {};

    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = Application::WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"ApplicationWindowClass";

    RegisterClassEx(&wc);

    m_hwnd = CreateWindowEx(
        0,
        L"ApplicationWindowClass",
        L"Hello, Windows!",
        WS_OVERLAPPEDWINDOW,
        rc.top,
        rc.left,
        rc.right - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        this
    );
    if (m_hwnd == nullptr)
    {
        MessageBox(nullptr, L"CreateWindowEx failed!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    search.SetParentHWND(m_hwnd);
    search.Init();

    table.SetParentHWND(m_hwnd);
    table.Init();
}

LRESULT Application::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}
