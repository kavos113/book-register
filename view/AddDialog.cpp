#include "AddDialog.h"

#include "Book.h"
#include "Client.h"
#include "DXFactory.h"
#include "Table.h"
#include "../db/Database.h"
#include "../db/Parser.h"

void AddDialog::Init()
{
    m_hwnd = CreateWindowEx(
        0,
        L"AddDialogWindowClass",
        L"Add",
        WS_OVERLAPPEDWINDOW,
        rc.left,
        rc.top,
        rc.right - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        this
    );
    if (m_hwnd == nullptr)
    {
        MessageBox(nullptr, L"CreateWindowEx failed!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    location1_rect = {20, 20, 120, 60};
    location2_rect = {(rc.right - rc.left) / 2 + 10, 20, (rc.right - rc.left) / 2 + 110, 60};
    isbn_rect = {20, 100, 120, 140};

    location1_field = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
        location1_rect.right,
        location1_rect.top + 7,
        (rc.right - rc.left) / 2 - location1_rect.right - 30,
        location1_rect.bottom - location1_rect.top - 14,
        m_hwnd,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
    if (location1_field == nullptr)
    {
        MessageBox(nullptr, L"CreateWindowEx failed!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    location2_field = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
        location2_rect.right,
        location2_rect.top + 7,
        (rc.right - rc.left) / 2 - location1_rect.right - 30,
        location2_rect.bottom - location2_rect.top - 14,
        m_hwnd,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
    if (location2_field == nullptr)
    {
        MessageBox(nullptr, L"CreateWindowEx failed!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    isbn_field = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
        isbn_rect.right,
        isbn_rect.top + 7,
        rc.right - rc.left - isbn_rect.right - 40,
        isbn_rect.bottom - isbn_rect.top - 14,
        m_hwnd,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
    if (isbn_field == nullptr)
    {
        MessageBox(nullptr, L"CreateWindowEx failed!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    register_button = CreateWindowEx(
        0,
        L"BUTTON",
        L"Register(Enter)",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        (rc.right - rc.left) / 2 - 75,
        rc.bottom - rc.top - 100,
        150,
        40,
        m_hwnd,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
}

void AddDialog::OnPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hwnd, &ps);

    DrawText(hdc, L"Location1", -1, &location1_rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
    DrawText(hdc, L"Location2", -1, &location2_rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
    DrawText(hdc, L"ISBN", -1, &isbn_rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

    EndPaint(m_hwnd, &ps);
}

void AddDialog::Register()
{
    TCHAR location1[64];
    TCHAR location2[64];
    TCHAR isbn[64];

    GetWindowText(location1_field, location1, 256);
    GetWindowText(location2_field, location2, 256);
    GetWindowText(isbn_field, isbn, 256);

    if (wcslen(location1) == 0 || wcslen(location2) == 0 || wcslen(isbn) == 0)
    {
        MessageBox(m_hwnd, L"Please fill all fields", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    if (wcslen(isbn) != 13)
    {
        MessageBox(m_hwnd, L"ISBN must be 13 digits", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    long long isbn_num = _wtoll(isbn);
    if (isbn_num == 0)
    {
        MessageBox(m_hwnd, L"ISBN must be a number", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    std::string xml_docs = Client::Fetch(std::to_string(isbn_num));
    if (xml_docs.empty())
    {
        MessageBox(m_hwnd, L"Failed to fetch data", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    Book b = Parser::Parse(xml_docs);
    b.location1 = location1;
    b.location2 = location2;
    b.isbn = isbn_num;

    Database::Insert(b);

    SendMessage(parent_hwnd, WM_UPDATEDB, 0, 0);
}

LRESULT AddDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        ShowWindow(m_hwnd, SW_HIDE);
        return 0;

    case WM_PAINT:
        OnPaint();
        return 0;

    case WM_KEYDOWN:
    {
        if (wParam != VK_RETURN)
        {
            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        }

        Register();
    }

    case WM_COMMAND:
    {
        if (LOWORD(wParam) != BN_CLICKED)
        {
            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        }

        if (reinterpret_cast<HWND>(lParam) == register_button)
        {
            Register();
        }
    }

    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}
