#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include "Detail.h"
#include "Search.h"
#include "Table.h"

class Application
{
public:
    void Init();

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Application* pThis = nullptr;

        if (uMsg == WM_CREATE)
        {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = static_cast<Application*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    Application(RECT rc)
        : m_hwnd(nullptr),
        rc(rc),
        search(Search({ MARGIN, MARGIN, (rc.right - rc.left - 3 * MARGIN) * 3 / 5 + MARGIN, 60 })),
        table(Table({ MARGIN, MARGIN * 2 + 60, (rc.right - rc.left - 3 * MARGIN) * 3 / 5 + MARGIN, (rc.bottom - rc.top)- MARGIN * 5})),
        detail(Detail({MARGIN * 2 + (rc.right - rc.left - 3 * MARGIN) * 3 / 5, MARGIN, rc.right - rc.left - MARGIN, rc.bottom - rc.top - MARGIN * 5}))
    {
        WNDCLASSEX wc = {};

        wc.cbSize = sizeof(wc);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wc.lpszClassName = L"ApplicationWindowClass";

        RegisterClassEx(&wc);
    }

    HWND Window() const { return m_hwnd; }

private:
    static constexpr int MARGIN = 10;

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd;
    RECT rc;

    Search search;
    Table table;
    Detail detail;
};

