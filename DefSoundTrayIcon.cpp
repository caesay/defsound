// ----------------------------------------------------------------------------
// DefSoundTrayIcon.cpp
// System tray icon implementation
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "DefSoundTrayIcon.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

CTrayIcon::CTrayIcon()
{
    ::memset(&m_TrayData, 0, sizeof(m_TrayData));
    m_TrayData.cbSize = sizeof(m_TrayData);
}

// ----------------------------------------------------------------------------

void CTrayIcon::Create(
    __in HINSTANCE hInstance,
    __in PCWSTR wszApplicationName,
    __in HWND hWindow,
    __in UINT nCallbackMessage
)
{
    const auto hIconSmall = 
        ::LoadImage(
            hInstance,
            L"IDI_ICON_MAIN",
            IMAGE_ICON,
            ::GetSystemMetrics(SM_CXSMICON),
            ::GetSystemMetrics(SM_CYSMICON),
            LR_DEFAULTCOLOR);
    if (!hIconSmall)
        throw CError( MakeDefaultErrorDescription(L"::LoadImage") );
    m_TrayData.hWnd             = hWindow;
    m_TrayData.uFlags           = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    m_TrayData.uCallbackMessage = nCallbackMessage;
    m_TrayData.hIcon            = static_cast<HICON>(hIconSmall);
    static_assert(_countof(m_TrayData.szTip) <= STRSAFE_MAX_CCH, "NOTIFYICONDATA.m_TrayData too long");
    ::StringCchCopy(m_TrayData.szTip, _countof(m_TrayData.szTip), wszApplicationName);

    if (!::Shell_NotifyIcon(NIM_ADD, &m_TrayData))
        throw CError( MakeDefaultErrorDescription(L"::Shell_NotifyIcon") );
}

// ----------------------------------------------------------------------------

void CTrayIcon::SetFocus()
{
    if (!::Shell_NotifyIcon(NIM_SETFOCUS, &m_TrayData))
        throw CError( MakeDefaultErrorDescription(L"::Shell_NotifyIcon") );
}

// ----------------------------------------------------------------------------

void CTrayIcon::Delete()
{
    if (!::Shell_NotifyIcon(NIM_DELETE, &m_TrayData))
        throw CError( MakeDefaultErrorDescription(L"::Shell_NotifyIcon") );
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
