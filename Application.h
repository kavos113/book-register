#pragma once
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
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (Application*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = (Application*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
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
        table(Table({ MARGIN, MARGIN * 2 + 60, (rc.right - rc.left - 3 * MARGIN) * 3 / 5 + MARGIN, (rc.bottom - rc.top) - 10}))
    {
    }

    HWND Window() const { return m_hwnd; }

private:
    static constexpr int MARGIN = 10;

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd;
    RECT rc;

    Search search;
    Table table;
};

