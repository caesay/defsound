// ----------------------------------------------------------------------------
// DefSoundTrayWindow.cpp
// Invisible window for system tray icon
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "DefSoundTrayWindow.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

namespace {

// ----------------------------------------------------------------------------

const PCWSTR g_wszWindowClassName = L"{566116C6-5126-4E68-A3AE-71E132450148}";

// ----------------------------------------------------------------------------

}   // namespace <nameless>

// ----------------------------------------------------------------------------

CTrayWindow::CTrayWindow()
    : m_hWindow(nullptr)
{
}

// ----------------------------------------------------------------------------

bool CTrayWindow::IsAlreadyExist(__in PCWSTR wszApplicationName)
{
    return !!::FindWindow(g_wszWindowClassName, wszApplicationName);
}

// ----------------------------------------------------------------------------

void CTrayWindow::Create(
    __in PCWSTR wszApplicationName,
    __in HINSTANCE hInstance,
    __in WNDPROC pWndProc
)
{
    _ASSERT(!m_hWindow);

    ::WNDCLASS WindowClass = {0};
    WindowClass.style = CS_GLOBALCLASS;
    WindowClass.lpfnWndProc = pWndProc;
    WindowClass.hInstance = hInstance;
    WindowClass.lpszClassName = g_wszWindowClassName;
    if (!::RegisterClass(&WindowClass))
        throw CError( MakeDefaultErrorDescription(L"::RegisterClass") );

    m_hWindow = 
        ::CreateWindow(
            g_wszWindowClassName,
            wszApplicationName,
            0,
            0,
            0,
            0,
            0,
            NULL,
            NULL,
            NULL,
            this);
    if (!m_hWindow)
        throw CError( MakeDefaultErrorDescription(L"::CreateWindow") );
}

// ----------------------------------------------------------------------------

void CTrayWindow::RunMessageLoop()
{
    _ASSERT(m_hWindow);

    ::MSG Msg;
    while (::GetMessage(&Msg, NULL, 0, 0))
    {
        if (Msg.message == WM_TIMER) 
            Msg.hwnd = *this;

        ::TranslateMessage(&Msg);
        ::DispatchMessage(&Msg);
    }
}

// ----------------------------------------------------------------------------

void CTrayWindow::TrackPopupMenu(HMENU hMenu) const
{
    ::POINT CursorPosition;
    if (!::GetCursorPos(&CursorPosition))
        throw CError( MakeDefaultErrorDescription(L"::GetCursorPos") );

    ::SetForegroundWindow(*this);

    if (!::TrackPopupMenu(hMenu, 0, CursorPosition.x, CursorPosition.y, 0, *this, nullptr))
        throw CError( MakeDefaultErrorDescription(L"::TrackPopupMenu") );

    ::PostMessage(*this, WM_NULL, 0, 0);
}

// ----------------------------------------------------------------------------

void CTrayWindow::Close()
{
    ::SendMessage(*this, WM_CLOSE, 0, 0);
    ::DestroyWindow(*this);
}

// ----------------------------------------------------------------------------

CTrayWindow::operator HWND() const
{
    _ASSERT(m_hWindow);
    return m_hWindow;
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
