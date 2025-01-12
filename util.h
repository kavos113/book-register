#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <string>
#include <windows.h>

template<class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = nullptr;
    }
}

inline std::wstring GetWString(const char* str)
{
    int num = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);
    std::wstring wstr(num, 0);
    MultiByteToWideChar(CP_ACP, 0, str, -1, &wstr[0], num);
    return wstr;
}

inline std::string GetCString(const std::wstring& wstr)
{
    int num = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(num, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], num, nullptr, nullptr);
    return str;
}