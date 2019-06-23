// ----------------------------------------------------------------------------
// DefSoundTrayIcon.h
// System tray icon implementation
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

class CTrayIcon
{
public:
    CTrayIcon();

    void Create(
        __in HINSTANCE hInstance,
        __in PCWSTR wszApplicationName,
        __in HWND hWindow,
        __in UINT nCallbackMessage
    );

    void SetFocus();

    void Delete();

private:
    ::NOTIFYICONDATA m_TrayData;
};

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
