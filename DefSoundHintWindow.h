// ----------------------------------------------------------------------------
// DefSoundHintWindow.h
// DefSound Hint Window
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

#include "DefSoundHintWindowClass.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

class CHintWindow
{
public:
    CHintWindow(
        const std::wstring &Icon,
        const std::wstring &Text,
        HINSTANCE hInstance,
        std::unique_ptr< CHintWindowClass > &pHintWindowClass
    );                  // -+
    // ~CHintWindow();  // -+-- Thread A

    void Create();      // -+
    void Show();        // -+-- Thread B

protected:
    static BOOL CALLBACK WndEnumForceQuitProc(HWND hWindow, LPARAM);

    static
    LRESULT CALLBACK WindowProc(
        HWND hWindow,
        UINT nMessage,
        WPARAM wParam,
        LPARAM lParam
    );

    static LRESULT OnMessageQuit(HWND hWindow);

    void Draw();

private:
    std::wstring m_Icon;
    std::wstring m_Text;

    HWND m_hWindow;
};

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
