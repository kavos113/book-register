#ifndef UNICODE
#define UNICODE
#endif

#include "Detail.h"

#include <sstream>

#include "DXFactory.h"
#include "../util.h"

Detail::~Detail()
{
    SafeRelease(&render_target);
    SafeRelease(&text_brush);
    SafeRelease(&text_format_16pt);
    SafeRelease(&text_format_40pt);
}

Detail::Detail(RECT rc)
    : dpiScaleX(1.0f),
      dpiScaleY(1.0f),
      parent_hwnd(nullptr),
      m_hwnd(nullptr),
      render_target(nullptr),
      text_brush(nullptr),
      text_format_16pt(nullptr),
      text_format_40pt(nullptr),
      book(),
      title_layout(nullptr),
      alt_title_layout(nullptr),
      series_layout(nullptr),
      creators_layout(nullptr),
      publisher_layout(nullptr),
      date_layout(nullptr),
      price_layout(nullptr),
      pages_layout(nullptr),
      ndc_layout(nullptr),
      location1_layout(nullptr),
      location2_layout(nullptr),
      tag_layout(nullptr),
      isbn_layout(nullptr),
      url_layout(nullptr),
      series_rect(),
      creators_rect(),
      publisher_rect(),
      date_rect(),
      price_rect(),
      pages_rect(),
      ndc_rect(),
      location1_rect(),
      location2_rect(),
      tag_rect(),
      isbn_rect(),
      url_rect()
{
    m_rcClient = rc;

    WNDCLASSEX wc = {};

    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = L"DetailWindowClass";

    RegisterClassEx(&wc);

    float now_height = 20.0f;

    title_rect = D2D1_RECT_F{
        20.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left) - 20.0f,
        now_height += 160.0f
    };

    alt_title_rect = D2D1_RECT_F{
        20.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        now_height += 20.0f
    };

    tag_rect = D2D1_RECT_F{
        20.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        now_height += 40.0f
    };

    location1_rect = D2D1_RECT_F{
        20.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left) / 2.0f,
        now_height + 60.0f
    };

    location2_rect = D2D1_RECT_F{
        static_cast<float>(m_rcClient.right - m_rcClient.left) / 2.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        now_height += 60.0f
    };

    creators_rect = D2D1_RECT_F{
        20.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        now_height += 40.0f
    };

    publisher_rect = D2D1_RECT_F{
        20.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        now_height += 40.0f
    };

    date_rect = D2D1_RECT_F{
        20.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        now_height += 40.0f
    };

    price_rect = D2D1_RECT_F{
        20.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left) / 2.0f,
        now_height + 40.0f
    };

    pages_rect = D2D1_RECT_F{
        static_cast<float>(m_rcClient.right - m_rcClient.left) / 2.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        now_height += 40.0f
    };

    isbn_rect = D2D1_RECT_F{
        20.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left) / 2.0f,
        now_height + 40.0f
    };

    ndc_rect = D2D1_RECT_F{
        static_cast<float>(m_rcClient.right - m_rcClient.left) / 2.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        now_height += 40.0f
    };

    url_rect = D2D1_RECT_F{
        20.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        now_height += 40.0f
    };

    series_rect = D2D1_RECT_F{
        20.0f,
        now_height,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        now_height + 40.0f
    };
}

void Detail::Init()
{
    m_hwnd = CreateWindowEx(
        0,
        L"DetailWindowClass",
        L"Detail",
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
    if (m_hwnd == nullptr)
    {
        MessageBox(nullptr, L"CreateWindowEx failed!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    HDC screen = GetDC(parent_hwnd);
    dpiScaleX = static_cast<float>(GetDeviceCaps(screen, LOGPIXELSX)) / 96.0f;
    dpiScaleY = static_cast<float>(GetDeviceCaps(screen, LOGPIXELSY)) / 96.0f;
    ReleaseDC(parent_hwnd, screen);

    HRESULT hr = DXFactory::GetDWriteFactory()->CreateTextFormat(
        L"Meiryo UI",
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        16.0f,
        L"ja-jp",
        &text_format_16pt
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text format\n");
        return;
    }

    hr = text_format_16pt->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set text alignment\n");
        return;
    }

    hr = text_format_16pt->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set paragraph alignment\n");
        return;
    }

    hr = DXFactory::GetDWriteFactory()->CreateTextFormat(
        L"Meiryo UI",
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        40.0f,
        L"ja-jp",
        &text_format_40pt
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text format\n");
        return;
    }

    hr = text_format_40pt->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set text alignment\n");
        return;
    }

    hr = text_format_40pt->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set paragraph alignment\n");
        return;
    }
}

void Detail::OnPaint()
{
    Draw();
}

void Detail::OnResize()
{
    if (render_target != nullptr)
    {
        D2D1_SIZE_U size = D2D1::SizeU(m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top);
        render_target->Resize(size);
    }

    InvalidateRect(m_hwnd, nullptr, FALSE);
}

void Detail::SetParentHWND(HWND hwnd)
{
    parent_hwnd = hwnd;
}

void Detail::SetBook(Book b)
{
    book = b;

    std::wstringstream ss;

    HRESULT hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        book.title.c_str(),
        static_cast<UINT32>(book.title.size()),
        text_format_40pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &title_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        book.altTitle.c_str(),
        static_cast<UINT32>(book.altTitle.size()),
        text_format_16pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &alt_title_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    ss << L"シリーズ: " << book.series;
    hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        ss.str().c_str(),
        static_cast<UINT32>(ss.str().size()),
        text_format_16pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &series_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    DWRITE_TEXT_RANGE range = { .startPosition= 0, .length = 5};
    hr = series_layout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set font weight\n");
        return;
    }

    ss.str(L"");
    ss << L"著者: " << book.creators;
    hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        ss.str().c_str(),
        static_cast<UINT32>(ss.str().size()),
        text_format_16pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &creators_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    range = { .startPosition= 0, .length = 3};
    hr = creators_layout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set font weight\n");
        return;
    }

    ss.str(L"");
    ss << L"出版社: " << book.publisher;
    hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        ss.str().c_str(),
        static_cast<UINT32>(ss.str().size()),
        text_format_16pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &publisher_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    range = { .startPosition= 0, .length = 4};
    hr = publisher_layout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set font weight\n");
        return;
    }

    ss.str(L"");
    ss << L"発売日: " << book.date;
    hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        ss.str().c_str(),
        static_cast<UINT32>(ss.str().size()),
        text_format_16pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &date_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    range = { .startPosition= 0, .length = 4};
    hr = date_layout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set font weight\n");
        return;
    }

    ss.str(L"");
    ss << L"価格: " << book.price;
    hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        ss.str().c_str(),
        static_cast<UINT32>(ss.str().size()),
        text_format_16pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &price_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    range = { .startPosition= 0, .length = 3};
    hr = price_layout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set font weight\n");
        return;
    }

    ss.str(L"");
    ss << L"ページ数: " << book.pages;
    hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        ss.str().c_str(),
        static_cast<UINT32>(ss.str().size()),
        text_format_16pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &pages_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    range = { .startPosition= 0, .length = 5};
    hr = pages_layout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set font weight\n");
        return;
    }

    ss.str(L"");
    ss << L"ISBN: " << book.isbn;
    hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        ss.str().c_str(),
        static_cast<UINT32>(ss.str().size()),
        text_format_16pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &isbn_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    range = { .startPosition= 0, .length = 5};
    hr = isbn_layout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set font weight\n");
        return;
    }

    ss.str(L"");
    ss << L"NDC: " << book.ndc;
    hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        ss.str().c_str(),
        static_cast<UINT32>(ss.str().size()),
        text_format_16pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &ndc_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    range = { .startPosition= 0, .length = 4};
    hr = ndc_layout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set font weight\n");
        return;
    }

    ss.str(L"");
    ss << L"URL: " << book.url;
    hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        ss.str().c_str(),
        static_cast<UINT32>(ss.str().size()),
        text_format_16pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &url_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    range = { .startPosition= 0, .length = 4};
    hr = url_layout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set font weight\n");
        return;
    }

    ss.str(L"");
    for (auto& tag : book.tags)
    {
        ss << tag.name << L" ";
    }
    hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        ss.str().c_str(),
        static_cast<UINT32>(ss.str().size()),
        text_format_16pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left),
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &tag_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        book.location1.c_str(),
        static_cast<UINT32>(book.location1.size()),
        text_format_16pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left) / 2.0f,
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &location1_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    range = { .startPosition= 0, .length = static_cast<UINT32>(book.location1.size()) };
    hr = location1_layout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set font weight\n");
        return;
    }

    hr = location1_layout->SetFontSize(22.0f, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set font size\n");
        return;
    }

    hr = location1_layout->SetUnderline(true, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set underline\n");
        return;
    }

    hr = DXFactory::GetDWriteFactory()->CreateTextLayout(
        book.location2.c_str(),
        static_cast<UINT32>(book.location2.size()),
        text_format_16pt,
        static_cast<float>(m_rcClient.right - m_rcClient.left) / 2.0f,
        static_cast<float>(m_rcClient.bottom - m_rcClient.top),
        &location2_layout
    );
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create text layout\n");
        return;
    }

    range = { .startPosition= 0, .length = static_cast<UINT32>(book.location2.size()) };
    hr = location2_layout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set font weight\n");
        return;
    }

    hr = location2_layout->SetFontSize(22.0f, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set font size\n");
        return;
    }

    hr = location2_layout->SetUnderline(true, range);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set underline\n");
        return;
    }

    SendMessage(m_hwnd, WM_PAINT, 0, 0);
}

void Detail::Draw()
{
    CreateDeviceResources();

    render_target->BeginDraw();
    render_target->Clear(D2D1::ColorF(D2D1::ColorF::White));

    DrawDetail();

    HRESULT hr = render_target->EndDraw();
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to draw detail\n");
        return;
    }
}

void Detail::DrawDetail() const
{
    if (book.title.empty() || title_layout == nullptr)
    {
        return;
    }

    D2D1_POINT_2F origin = D2D1::Point2F(title_rect.left, title_rect.top);
    render_target->DrawTextLayout(
        origin,
        title_layout,
        text_brush
    );

    origin = D2D1::Point2F(alt_title_rect.left, alt_title_rect.top);
    render_target->DrawTextLayout(
        origin,
        alt_title_layout,
        text_brush
    );

    origin = D2D1::Point2F(series_rect.left, series_rect.top);
    render_target->DrawTextLayout(
        origin,
        series_layout,
        text_brush
    );

    origin = D2D1::Point2F(creators_rect.left, creators_rect.top);
    render_target->DrawTextLayout(
        origin,
        creators_layout,
        text_brush
    );

    origin = D2D1::Point2F(publisher_rect.left, publisher_rect.top);
    render_target->DrawTextLayout(
        origin,
        publisher_layout,
        text_brush
    );

    origin = D2D1::Point2F(date_rect.left, date_rect.top);
    render_target->DrawTextLayout(
        origin,
        date_layout,
        text_brush
    );

    origin = D2D1::Point2F(price_rect.left, price_rect.top);
    render_target->DrawTextLayout(
        origin,
        price_layout,
        text_brush
    );

    origin = D2D1::Point2F(pages_rect.left, pages_rect.top);
    render_target->DrawTextLayout(
        origin,
        pages_layout,
        text_brush
    );

    origin = D2D1::Point2F(ndc_rect.left, ndc_rect.top);
    render_target->DrawTextLayout(
        origin,
        ndc_layout,
        text_brush
    );

    origin = D2D1::Point2F(location1_rect.left, location1_rect.top);
    render_target->DrawTextLayout(
        origin,
        location1_layout,
        text_brush
    );

    origin = D2D1::Point2F(location2_rect.left, location2_rect.top);
    render_target->DrawTextLayout(
        origin,
        location2_layout,
        text_brush
    );

    origin = D2D1::Point2F(tag_rect.left, tag_rect.top);
    render_target->DrawTextLayout(
        origin,
        tag_layout,
        text_brush
    );

    origin = D2D1::Point2F(isbn_rect.left, isbn_rect.top);
    render_target->DrawTextLayout(
        origin,
        isbn_layout,
        text_brush
    );

    origin = D2D1::Point2F(url_rect.left, url_rect.top);
    render_target->DrawTextLayout(
        origin,
        url_layout,
        text_brush
    );
}

void Detail::CreateDeviceResources()
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

LRESULT Detail::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
