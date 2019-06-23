// ----------------------------------------------------------------------------
// DefSoundTrayApp.cpp
// Run application as system tray icon
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "DefSoundTrayApp.h"
#include "DefSoundHelpMessage.h"
#include "DefSoundEndpointColl.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

namespace {

// ----------------------------------------------------------------------------

const PCWSTR g_wszSoundOptionsText = L"System Sound Options...";
const PCWSTR g_wszHelpText = L"Using...";
const PCWSTR g_wszExitText = L"Exit";

// ----------------------------------------------------------------------------

}   // namespace <nameless>

// ----------------------------------------------------------------------------

CTrayApp::CTrayApp(PCWSTR wszApplicationName)
    : m_wszApplicationName(wszApplicationName)
{
}

// ----------------------------------------------------------------------------

INT CTrayApp::Run(__in HINSTANCE hInstance)
{
    if (CTrayWindow::IsAlreadyExist(m_wszApplicationName))
        return ERROR_ALREADY_EXISTS;

    m_Window.Create(m_wszApplicationName, hInstance, CTrayApp::WndProc);
    ::SetWindowLongPtr(m_Window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    m_Icon.Create(hInstance, m_wszApplicationName, m_Window, Message::TrayIcon);

    m_Window.RunMessageLoop();

    return ERROR_SUCCESS;
}

// ----------------------------------------------------------------------------

LRESULT CALLBACK CTrayApp::WndProc(
    __in HWND hWindow,
    __in UINT nMessage,
    __in WPARAM wParam,
    __in LPARAM lParam
)
{
    try
    {
        switch (nMessage)
        {

        case WM_DESTROY:
            ::PostQuitMessage(ERROR_SUCCESS);
            break;

        case WM_COMMAND:
            return GetInstance(hWindow).OnMessageCommand(wParam);

        case Message::TrayIcon:
            switch (lParam)
            {
            case WM_RBUTTONUP:
                return GetInstance(hWindow).OnMessageTrayIconRightClick();

            case WM_LBUTTONDBLCLK:
                return GetInstance(hWindow).OnMessageTrayIconLeftDoubleClick();
            }
            break;
        }
    }
    catch (const CError &Error)
    {
        UNREFERENCED_PARAMETER(Error);
#ifdef  DEBUG
        throw;
#endif  // DEBUG
    }
    return ::DefWindowProc(hWindow, nMessage, wParam, lParam);
}

// ----------------------------------------------------------------------------

CTrayApp &CTrayApp::GetInstance(__in HWND hWindow)
{
    const auto ThisPtr = ::GetWindowLongPtr(hWindow, GWLP_USERDATA);
    if (!ThisPtr)
        throw CError( MakeDefaultErrorDescription(L"::GetWindowLongPtr") );
    return *reinterpret_cast<CTrayApp *>(ThisPtr);
}

// ----------------------------------------------------------------------------

LRESULT CTrayApp::OnMessageTrayIconRightClick()
{
    const auto hTrayMenu = ::CreatePopupMenu();
    if (!hTrayMenu)
        throw CError( MakeDefaultErrorDescription(L"::CreatePopupMenu") );
    CEndpointMenu::CMenuHolder TrayMenuHolder(hTrayMenu, &::DestroyMenu);

    if (!::AppendMenu(hTrayMenu, 0, MenuItem::SoundOptions, g_wszSoundOptionsText))
        throw CError( MakeDefaultErrorDescription(L"::AppendMenu") );
    if (!::AppendMenu(hTrayMenu, MF_MENUBREAK, 0, nullptr))
        throw CError( MakeDefaultErrorDescription(L"::AppendMenu") );


    std::list< CEndpointMenu::CMenuHolder > EndpointMenuHolder;
    m_EndpointMenu.BuildPopupMenu(hTrayMenu, MenuItem::FirstEndpoint, EndpointMenuHolder);

    if (!::AppendMenu(hTrayMenu, MF_MENUBREAK, 0, nullptr))
        throw CError( MakeDefaultErrorDescription(L"::AppendMenu") );
    if (!::AppendMenu(hTrayMenu, 0, MenuItem::Help, g_wszHelpText))
        throw CError( MakeDefaultErrorDescription(L"::AppendMenu") );
    if (!::AppendMenu(hTrayMenu, 0, MenuItem::Exit, g_wszExitText))
        throw CError( MakeDefaultErrorDescription(L"::AppendMenu") );

    m_Window.TrackPopupMenu(hTrayMenu);
    m_Icon.SetFocus();
    return 0;
}

// ----------------------------------------------------------------------------

LRESULT CTrayApp::OnMessageTrayIconLeftDoubleClick()
{
    m_EndpointMenu.OnCommandNextEndpoint();
    return 0;
}

// ----------------------------------------------------------------------------

LRESULT CTrayApp::OnMessageCommand(__in WPARAM wParam)
{
    const WORD nMenuItemId = LOWORD(wParam);
    switch (nMenuItemId)
    {
    case MenuItem::SoundOptions:
        OnCommandSoundOptions();
        break;
    
    case MenuItem::Help:
        OnCommandHelp();
        break;
    
    case MenuItem::Exit:
        OnCommandExit();
        break;

    default:
        _ASSERT(nMenuItemId >= MenuItem::FirstEndpoint);
        if (nMenuItemId >= MenuItem::FirstEndpoint)
            m_EndpointMenu.OnCommandEndpoint(nMenuItemId - MenuItem::FirstEndpoint);
        break;
    }
    return 0;
}

// ----------------------------------------------------------------------------

void CTrayApp::OnCommandSoundOptions()
{
    STARTUPINFO StartupInfo = {0};
    StartupInfo.cb =  sizeof(StartupInfo);

    PROCESS_INFORMATION ProcessInfo = {0};
    RtlZeroMemory(&ProcessInfo, sizeof(ProcessInfo));

    WCHAR wszCommandLine[] = L"control.exe mmsys.cpl";
    const auto bProcessCreated = 
        ::CreateProcess(
            NULL,
            wszCommandLine,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &StartupInfo,
            &ProcessInfo);
    if (!bProcessCreated)
        throw CError( MakeDefaultErrorDescription(L"::CreateProcess") );

    ::CloseHandle(ProcessInfo.hProcess);
    ::CloseHandle(ProcessInfo.hThread);
}

// ----------------------------------------------------------------------------

void CTrayApp::OnCommandHelp()
{
    ShowHelpMessage(m_wszApplicationName, m_Window);
}

// ----------------------------------------------------------------------------

void CTrayApp::OnCommandExit()
{
    m_Icon.Delete();
    m_Window.Close();
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
