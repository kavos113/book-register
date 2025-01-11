#pragma once
#include <d2d1.h>
#include <iostream>
#include <ostream>
#include <print>

#include "Book.h"
#include "View.h"

class Detail : public View
{
public:
    ~Detail() override;
    void Init() override;
    void OnPaint() override;
    void OnResize() override;

    void SetParentHWND(HWND hwnd);
    void SetBook(Book b);

    Detail(RECT rc);

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Detail* pThis;

        if (uMsg == WM_CREATE)
        {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = static_cast<Detail*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = reinterpret_cast<Detail*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
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
    void Draw();
    void DrawDetail() const;
    void CreateDeviceResources();

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    float dpiScaleX;
    float dpiScaleY;

    HWND parent_hwnd;
    HWND m_hwnd;
    ID2D1HwndRenderTarget* render_target;
    ID2D1SolidColorBrush* text_brush;
    IDWriteTextFormat* text_format_16pt;
    IDWriteTextFormat* text_format_40pt;

    Book book;

    IDWriteTextLayout* title_layout;
    IDWriteTextLayout* alt_title_layout;
    IDWriteTextLayout* series_layout;
    IDWriteTextLayout* creators_layout;
    IDWriteTextLayout* publisher_layout;
    IDWriteTextLayout* date_layout;
    IDWriteTextLayout* price_layout;
    IDWriteTextLayout* pages_layout;
    IDWriteTextLayout* ndc_layout;
    IDWriteTextLayout* location1_layout;
    IDWriteTextLayout* location2_layout;
    IDWriteTextLayout* tag_layout;
    IDWriteTextLayout* isbn_layout;
    IDWriteTextLayout* url_layout;

    D2D1_RECT_F title_rect;
    D2D1_RECT_F alt_title_rect;
    D2D1_RECT_F series_rect;
    D2D1_RECT_F creators_rect;
    D2D1_RECT_F publisher_rect;
    D2D1_RECT_F date_rect;
    D2D1_RECT_F price_rect;
    D2D1_RECT_F pages_rect;
    D2D1_RECT_F ndc_rect;
    D2D1_RECT_F location1_rect;
    D2D1_RECT_F location2_rect;
    D2D1_RECT_F tag_rect;
    D2D1_RECT_F isbn_rect;
    D2D1_RECT_F url_rect;
};
