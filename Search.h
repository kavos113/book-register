#pragma once
#include "View.h"

class Search :
    public View
{
public:
    void Init() override;
    void OnPaint() override;
    void OnResize() override;

    void SetParentHWND(HWND hwnd);

    Search(RECT rc)
        : parent_hwnd(nullptr),
        search_field(nullptr),
        search_button(nullptr),
        tag_field(nullptr),
        tag_button(nullptr)
    {
        m_rcClient = rc;
        width = rc.right - rc.left - 10;
    }

private:

    HWND parent_hwnd;

    HWND search_field;
    HWND search_button;
    HWND tag_field;
    HWND tag_button;

    int width;

    static constexpr int EDIT_HEIGHT = 25;
};

