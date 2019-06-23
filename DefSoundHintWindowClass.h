// ----------------------------------------------------------------------------
// DefSoundHintWindowClass.h
// DefSound Hint Window Class
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

class CHintWindowClass
{
public:
    CHintWindowClass(HINSTANCE hInstance, WNDPROC WndProc, int nWndExtra);
    ~CHintWindowClass();

    static PCWSTR GetName();
    static UINT GetNameLength();

private:
    static WCHAR g_wszName[];

    HINSTANCE m_hInstance;
};

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------


