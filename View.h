#pragma once

#include <Windows.h>

class View
{
public:
    virtual ~View() = default;
    View() : m_rcClient({}) {}

    virtual void Init() = 0;
    virtual void OnPaint() = 0;
    virtual void OnResize() = 0;

protected:
    RECT m_rcClient;
};

