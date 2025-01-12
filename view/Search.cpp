#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include "Search.h"

#include <format>
#include <string>

void Search::SetParentHWND(HWND hwnd)
{
    parent_hwnd = hwnd;
}


void Search::Init()
{
    search_field = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 
        m_rcClient.left,
        m_rcClient.top,
        width * 4 / 5,
        EDIT_HEIGHT,
        parent_hwnd,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
    if (search_field == nullptr)
    {
        MessageBox(nullptr, L"CreateWindowEx failed!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    search_button = CreateWindowEx(
        0,
        L"BUTTON",
        L"Search",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        m_rcClient.right - (width / 5),
        m_rcClient.top,
        width / 5,
        EDIT_HEIGHT,
        parent_hwnd,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
    if (search_button == nullptr)
    {
        MessageBox(nullptr, L"CreateWindowEx failed!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    tag_field = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
        m_rcClient.left,
        m_rcClient.top + EDIT_HEIGHT + 10,
        width * 4 / 5,
        EDIT_HEIGHT,
        parent_hwnd,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
    if (tag_field == nullptr)
    {
        MessageBox(nullptr, L"CreateWindowEx failed!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    tag_button = CreateWindowEx(
        0,
        L"BUTTON",
        L"Tag",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        m_rcClient.right - (width / 5),
        m_rcClient.top + EDIT_HEIGHT + 10,
        width / 5,
        EDIT_HEIGHT,
        parent_hwnd,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
    if (tag_button == nullptr)
    {
        MessageBox(nullptr, L"CreateWindowEx failed!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }
}

void Search::OnPaint()
{
}

void Search::OnResize()
{
}
