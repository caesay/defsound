// ----------------------------------------------------------------------------
// DefSoundTrayWindowApp.h
// Invisible window for system tray icon
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

class CTrayWindow
{
public:
    CTrayWindow();

    static bool IsAlreadyExist(__in PCWSTR wszApplicationName);

    void Create(
        __in PCWSTR wszApplicationName,
        __in HINSTANCE hInstance,
        __in WNDPROC pWndProc
    );

    void RunMessageLoop();

    void TrackPopupMenu(HMENU hMenu) const;

    void Close();

    operator HWND() const;
private:
    HWND m_hWindow;
};

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
