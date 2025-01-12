#pragma once

#include <d2d1.h>
#include <dwrite.h>

#include "../util.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


class DXFactory
{
public:
    static void ReleaseD2DFactory()
    {
        SafeRelease(&pD2DFactory);
    }

    static ID2D1Factory* GetD2DFactory()
    {
        if (pD2DFactory == nullptr)
        {
            if (CreateD2DFactory() < 0)
            {
                return nullptr;
            }
        }
        return pD2DFactory;
    }

    static IDWriteFactory* GetDWriteFactory()
    {
        if (pDWriteFactory == nullptr)
        {
            if (CreateDWriteFactory() < 0)
            {
                return nullptr;
            }
        }
        return pDWriteFactory;
    }

    static void ReleaseDWriteFactory()
    {
        SafeRelease(&pDWriteFactory);
    }
private:

    static ID2D1Factory* pD2DFactory;
    static IDWriteFactory* pDWriteFactory;

    static int CreateD2DFactory()
    {
        HRESULT hr = D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, 
            &pD2DFactory
        );
        if (FAILED(hr))
        {
            return -1;
        }
        return 0;
    }

    static int CreateDWriteFactory()
    {
        HRESULT hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory), 
            reinterpret_cast<IUnknown**>(&pDWriteFactory)
        );
        if (FAILED(hr))
        {
            return -1;
        }
        return 0;
    }
};

