#include "Application.h"

void Application::Init()
{
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
}

LRESULT Application::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        search.SetParentHWND(m_hwnd);
        table.SetParentHWND(m_hwnd);
        detail.SetParentHWND(m_hwnd);
        search.Init();
        table.Init();
        detail.Init();
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_SELECTBOOK:
        Book* book = reinterpret_cast<Book*>(lParam);
        detail.SetBook(*book);
        return 0;
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}
