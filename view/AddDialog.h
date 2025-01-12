#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#ifndef UNICODE
#define UNICODE
#include <d2d1.h>
#endif

#include <windows.h>

class AddDialog
{
public:
    void Init();
    void Show() const
    {
        ShowWindow(m_hwnd, SW_SHOW);
    }

    void Destroy() const
    {
        DestroyWindow(m_hwnd);
    }

    void SetParentHWND(HWND hwnd)
    {
        parent_hwnd = hwnd;
    }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        AddDialog* pThis = nullptr;

        if (uMsg == WM_CREATE)
        {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = static_cast<AddDialog*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = reinterpret_cast<AddDialog*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    AddDialog(RECT rc)
        : parent_hwnd(nullptr)
        , m_hwnd(nullptr)
        , rc(rc)
        , location1_field(nullptr)
        , location2_field(nullptr)
        , isbn_field(nullptr)
        , register_button(nullptr)
        , location1_rect()
        , location2_rect()
        , isbn_rect()
    {
        WNDCLASSEX wc = {};

        wc.cbSize = sizeof(wc);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wc.lpszClassName = L"AddDialogWindowClass";

        RegisterClassEx(&wc);
    }

    HWND Window() const { return m_hwnd; }

private:
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void OnPaint();
    void Register();

    HWND parent_hwnd;
    HWND m_hwnd;
    RECT rc;

    HWND location1_field;
    HWND location2_field;
    HWND isbn_field;
    HWND register_button;
    RECT location1_rect;
    RECT location2_rect;
    RECT isbn_rect;

};



#endif //ADDDIALOG_H
