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

CTrayWindow::~CTrayWindow()
{
    if (m_hWindow)
    {
        Close();
    }
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
            nullptr,
            nullptr,
            nullptr,
            this);
    if (!m_hWindow)
        throw CError( MakeDefaultErrorDescription(L"::CreateWindow") );

    m_nTaskbarCreatedMessage = ::RegisterWindowMessage(L"TaskbarCreated");
    if (!m_nTaskbarCreatedMessage)
        throw CError( MakeDefaultErrorDescription(L"::RegisterWindowMessage") );
}

// ----------------------------------------------------------------------------

void CTrayWindow::RunMessageLoop()
{
    _ASSERT(m_hWindow);

    ::MSG Msg;
    while (::GetMessage(&Msg, nullptr, 0, 0))
    {
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
    VERIFY(::DestroyWindow(*this));
    m_hWindow = nullptr;
}

// ----------------------------------------------------------------------------

CTrayWindow::operator HWND() const
{
    _ASSERT(m_hWindow);
    return m_hWindow;
}

// ----------------------------------------------------------------------------

UINT CTrayWindow::GetTaskbarCreatedMessage() const
{
    return m_nTaskbarCreatedMessage;
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
