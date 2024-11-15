#include "Table.h"

#include <array>
#include <format>
#include <sstream>

#include "Application.h"
#include "DXFactory.h"

void Table::Init()
{
    Book midashi;
    midashi.id = 0;
    midashi.isbn = 0;
    midashi.title = L"タイトル";
    midashi.titleRuby = L"たいとる";
    midashi.altTitle = L"補足タイトル";
    midashi.altTitleRuby = L"ほそくたいとる";
    midashi.series = L"シリーズ";
    midashi.seriesRuby = L"しりーず";
    midashi.creators = L"著者";
    midashi.publisher = L"出版社";
    midashi.date = L"発行日";
    midashi.price = 0;
    midashi.pages = L"ページ数";
    midashi.ndc = 0;
    midashi.location1 = L"所在1";
    midashi.location2 = L"所在2";

    Book book;
    book.id = 1;
    book.isbn = 9784041021070;
    book.title = L"解析概論";
    book.titleRuby = L"かいせきがいろん";
    book.altTitle = L"Introduction to the Theory of Computation";
    book.altTitleRuby = L"";
    book.series = L"数学シリーズ";
    book.seriesRuby = L"";
    book.creators = L"高木 貞治";
    book.publisher = L"岩波書店";
    book.date = L"2006/12/20";
    book.price = 3000;
    book.pages = L"B5, 200p";
    book.ndc = 511.6;
    book.location1 = L"図書館";
    book.location2 = L"どこかの棚";

    book_table.push_back(midashi);
    book_table.push_back(book);

    WNDCLASSEX wc = {};

    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"TableWindowClass";

    RegisterClassEx(&wc);

    m_hwnd = CreateWindowEx(
        0,
        L"TableWindowClass",
        L"Table",
        WS_CHILD | WS_VISIBLE,
        m_rcClient.left,
        m_rcClient.top,
        m_rcClient.right - m_rcClient.left,
        m_rcClient.bottom - m_rcClient.top,
        parent_hwnd,
        nullptr,
        GetModuleHandle(nullptr),
        this
    );

    HDC screen = GetDC(parent_hwnd);
    dpiScaleX = GetDeviceCaps(screen, LOGPIXELSX) / 96.0f;
    dpiScaleY = GetDeviceCaps(screen, LOGPIXELSY) / 96.0f;
    ReleaseDC(parent_hwnd, screen);

    HRESULT hr = DXFactory::GetDWriteFactory()->CreateTextFormat(
        L"Consolas",
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        10.0f,
        L"ja-jp",
        &text_format
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text format\n");
        return;
    }

    hr = text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set text alignment\n");
        return;
    }

    hr = text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set paragraph alignment\n");
        return;
    }
}

void Table::DrawRow(int index)
{
    for (int i = 0; i < 7; ++i)
    {
    
        FLOAT left = static_cast<FLOAT>(m_rcClient.right - m_rcClient.left) * column_left[i] / COLUMN_SIZE_SUM / dpiScaleX;
        FLOAT top = static_cast<FLOAT>(ROW_HEIGHT * index) / dpiScaleY;
        FLOAT right = static_cast<FLOAT>(m_rcClient.right - m_rcClient.left) * column_left[i + 1] / COLUMN_SIZE_SUM / dpiScaleX;
        FLOAT bottom = static_cast<FLOAT>(ROW_HEIGHT * (index + 1)) / dpiScaleY;

        D2D1_RECT_F rect = D2D1::RectF(left, top, right, bottom);
        D2D1_RECT_F inrect = D2D1::RectF(left + 3, top, right, bottom);

        const wchar_t* text = L"";
        std::wstringstream ss;
        std::wstring ws;

        switch (i)
        {
        case 0:
            text = book_table[index].title.c_str();
            break;
        case 1:
            text = book_table[index].creators.c_str();
            break;
        case 2:
            ss << book_table[index].ndc;
            ws = ss.str();
            text = ws.c_str();
            break;
        case 3:
            text = book_table[index].publisher.c_str();
            break;
        case 4:
            text = L"";
            break;
        case 5:
            text = book_table[index].location1.c_str();
            break;
        case 6:
            text = book_table[index].location2.c_str();
            break;
        }

        size_t length = wcslen(text);

        render_target->DrawText(
            text,
            length,
            text_format,
            inrect,
            text_brush
        );

        render_target->DrawRectangle(rect, text_brush);
    }
    
}

void Table::DrawTable()
{
    CreateDeviceResources();

    render_target->BeginDraw();
    render_target->Clear(D2D1::ColorF(D2D1::ColorF::White));

    for (int i = 0; i < book_table.size(); ++i)
    {
        DrawRow(i);
    }

    HRESULT hr = render_target->EndDraw();
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to draw table\n");
        return;
    }
}

void Table::CreateDeviceResources()
{
    if (render_target == nullptr)
    {
        HRESULT hr = DXFactory::GetD2DFactory()->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
                m_hwnd,
                D2D1::SizeU(m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top)
            ),
            &render_target
        );
        if (FAILED(hr))
        {
            OutputDebugString(L"Failed to create render target\n");
            return;
        }
    }

    if (text_brush == nullptr)
    {
        HRESULT hr = render_target->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &text_brush
        );
        if (FAILED(hr))
        {
            OutputDebugString(L"Failed to create text brush\n");
            return;
        }
    }
}

void Table::OnPaint()
{
    DrawTable();
}

void Table::OnResize()
{
}

void Table::SetParentHWND(HWND hwnd)
{
    parent_hwnd = hwnd;
}

LRESULT Table::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
        OnPaint();
        return 0;
    case WM_SIZE:
        OnResize();
        return 0;
    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
}