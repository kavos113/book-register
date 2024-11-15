#pragma once
#include <d2d1.h>
#include <vector>
#include <array>

#include "View.h"
#include "controller/Book.h"

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
        text_format(nullptr)
    {
        m_rcClient = rc;
    }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Table* pThis = nullptr;

        if (uMsg == WM_CREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (Table*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = (Table*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
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

    float dpiScaleX;
    float dpiScaleY;

    std::vector<Book> book_table;

    HWND parent_hwnd;
    HWND m_hwnd;
    ID2D1HwndRenderTarget* render_target;
    ID2D1SolidColorBrush* text_brush;
    IDWriteTextFormat* text_format;

    static constexpr std::array<int, 7> column_size = {
        8, 2, 1, 2, 4, 1, 2
    };
    static constexpr std::array<int, 8> column_left = {
        0, 8, 10, 11, 13, 17, 18, 20
    };
    static constexpr int ROW_HEIGHT = 20;
    static constexpr int COLUMN_SIZE_SUM = 20;
    static constexpr int MARGIN = 10;
};

