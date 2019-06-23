// ----------------------------------------------------------------------------
// DefSoundHintWindowClass.cpp
// DefSound Hint Window Class
// ----------------------------------------------------------------------------

#include "stdafx.h"

// ----------------------------------------------------------------------------

#include "DefSoundHintWindowClass.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

WCHAR CHintWindowClass::g_wszName[] = L"DefSound Hint Class";

// ----------------------------------------------------------------------------

CHintWindowClass::CHintWindowClass(HINSTANCE hInstance, WNDPROC WndProc, int nWndExtra)
    : m_hInstance(hInstance)
{
    WNDCLASSEX WindowClassEx = {0};
    WindowClassEx.cbSize = sizeof(WNDCLASSEX);
    WindowClassEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    WindowClassEx.lpfnWndProc = WndProc;
    WindowClassEx.cbClsExtra = 0;
    WindowClassEx.cbWndExtra = nWndExtra;
    WindowClassEx.hInstance = m_hInstance;
    WindowClassEx.hIcon = nullptr;
    WindowClassEx.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
    if (!WindowClassEx.hCursor)
        throw CError( MakeDefaultErrorDescription(L"::LoadCursor") );
    WindowClassEx.hbrBackground = reinterpret_cast<HBRUSH>(NULL_BRUSH);
    WindowClassEx.lpszMenuName = nullptr;
    WindowClassEx.lpszClassName = GetName();
    WindowClassEx.hIconSm = nullptr;
    if (!::RegisterClassEx(&WindowClassEx))
        throw CError( MakeDefaultErrorDescription(L"::RegisterClassEx") );
}

// ----------------------------------------------------------------------------

CHintWindowClass::~CHintWindowClass()
{
    ::UnregisterClass(GetName(), m_hInstance);
}

// ----------------------------------------------------------------------------

PCWSTR CHintWindowClass::GetName()
{
    return g_wszName;
}

UINT CHintWindowClass::GetNameLength()
{
    return _countof(g_wszName) - 1;
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------

