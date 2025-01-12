#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include "Detail.h"
#include "Search.h"
#include "Table.h"
#include "AddDialog.h"

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

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    Application(RECT rc)
        : m_hwnd(nullptr),
        m_haccel(nullptr),
        rc(rc),
        search(Search({ MARGIN, MARGIN, (rc.right - rc.left - 3 * MARGIN) * 3 / 5 + MARGIN, 60 })),
        table(Table({ MARGIN, MARGIN * 2 + 60, (rc.right - rc.left - 3 * MARGIN) * 3 / 5 + MARGIN, (rc.bottom - rc.top)- MARGIN * 5})),
        detail(Detail({MARGIN * 2 + (rc.right - rc.left - 3 * MARGIN) * 3 / 5, MARGIN, rc.right - rc.left - MARGIN * 3, rc.bottom - rc.top - MARGIN * 5})),
        addDialog(AddDialog({(rc.right - rc.left) / 2 - 100, (rc.bottom - rc.top) / 2 - 100, (rc.right - rc.left) / 2 + 500, (rc.bottom - rc.top) / 2 + 200}))
    {
        WNDCLASSEX wc = {};

        wc.cbSize = sizeof(wc);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wc.lpszClassName = L"ApplicationWindowClass";
        wc.lpszMenuName = L"MYMENU";

        RegisterClassEx(&wc);
    }

    HWND Window() const { return m_hwnd; }
    HACCEL Accel() const { return m_haccel; }

private:
    static constexpr int MARGIN = 10;

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd;
    HACCEL m_haccel;
    RECT rc;

    Search search;
    Table table;
    Detail detail;
    AddDialog addDialog;
};

