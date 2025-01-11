#pragma once
#include <d2d1.h>
#include <vector>
#include <array>

#include "View.h"
#include "controller/Book.h"

#define WM_SELECTBOOK 0x401

class Table :
    public View
{
public:
    void Init() override;
    void OnPaint() override;
    void OnResize() override;

    void SetParentHWND(HWND hwnd);

    Table(RECT rc)
        : dpiScaleX(1.0f),
          dpiScaleY(1.0f),
          parent_hwnd(nullptr),
          m_hwnd(nullptr),
          render_target(nullptr),
          text_brush(nullptr),
          text_format_10pt(nullptr)
    {
        m_rcClient = rc;

        WNDCLASSEX wc = {};

        wc.cbSize = sizeof(wc);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wc.lpszClassName = L"TableWindowClass";

        RegisterClassEx(&wc);
    }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Table* pThis = nullptr;

        if (uMsg == WM_CREATE)
        {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = static_cast<Table*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = reinterpret_cast<Table*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
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

private:
    void DrawRow(int index);
    void DrawTable();
    void CreateDeviceResources();

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void UpdateScrollInfo() const;

    float dpiScaleX;
    float dpiScaleY;

    float scrolled = 0.0f;
    float all_height = 0.0f;

    std::vector<Book> book_table;

    HWND parent_hwnd;
    HWND m_hwnd;
    ID2D1HwndRenderTarget* render_target;
    ID2D1SolidColorBrush* text_brush;
    IDWriteTextFormat* text_format_10pt;

    static constexpr std::array<int, 7> column_size = {
        8, 2, 1, 2, 4, 1, 2
    };
    static constexpr std::array<int, 8> column_left = {
        0, 8, 10, 11, 13, 17, 18, 20
    };
    static constexpr int ROW_HEIGHT = 20;
    static constexpr int COLUMN_SIZE_SUM = 20;
    static constexpr int MARGIN = 10;

    static constexpr int DEFAULT_SCROLL_HEIGHT = 100;
};

