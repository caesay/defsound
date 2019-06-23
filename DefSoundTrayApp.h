// ----------------------------------------------------------------------------
// DefSoundTrayApp.h
// Run application as system tray icon
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

#include "DefSoundTrayIcon.h"
#include "DefSoundTrayWindow.h"
#include "DefSoundTrayEndpointMenu.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

class CTrayApp
{
public:
    explicit CTrayApp(PCWSTR wszApplicationName);

    INT Run(__in HINSTANCE hInstance);

protected:
    // Window message IDs
    struct Message
    {
        enum : UINT
        {
            TrayIcon = WM_USER + 1,
        };
    };

    // Menu item IDs
    struct MenuItem
    {
        enum
        {
            SoundOptions    = 10300,
            Help,
            Exit,

            FirstEndpoint   // must be the last!
        };
    };

    static LRESULT CALLBACK WndProc(
        __in HWND hWindow,
        __in UINT nMessage,
        __in WPARAM wParam,
        __in LPARAM lParam
    );
    static CTrayApp &GetInstance(__in HWND hWindow);

    LRESULT OnMessageTrayIconRightClick();
    LRESULT OnMessageTrayIconLeftDoubleClick();

    LRESULT OnMessageCommand(__in WPARAM wParam);

    void OnCommandSoundOptions();
    void OnCommandHelp();
    void OnCommandExit();

private:
    PCWSTR m_wszApplicationName;
    CTrayWindow m_Window;
    CTrayIcon m_Icon;
    CEndpointMenu m_EndpointMenu;
};

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
