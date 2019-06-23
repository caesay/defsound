// ----------------------------------------------------------------------------
// DefSoundHelpMessage.cpp
// Help message box
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "DefSoundHelpMessage.h"
#include "DefSoundEndpointRole.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

void ShowHelpMessage(
    __in PCWSTR wszApplicationName,
    __in HWND hParentWindow,
    __in PCWSTR wszErrorMessage /*= nullptr*/
)
{
    std::wstringstream Stream;

    if (wszErrorMessage && *wszErrorMessage)
    {
        Stream << wszErrorMessage << std::endl << std::endl;
    }

    Stream << L"Usage: DefSound.exe" << std::endl;
    Stream << L"Run and show system tray icon" << std::endl;

    Stream << std::endl << L"Or: DefSound.exe <DEVICE>[,DEVICE...] [ROLE]" << std::endl;
    Stream << L"    DEVICE - device description or zero-based index (-1=next)" << std::endl;
    Stream << L"    ROLE - default is " << GetEndpointAllRoles().m_wszShortName;
    Stream << L". Variants:";

    Stream << std::endl << L"        " << GetEndpointAllRoles().m_wszShortName;
    Stream << L" - " << GetEndpointAllRoles().m_wszDescription;

    for (const auto &Role : GetEndpointRoleArray())
    {
        Stream << std::endl << L"        " << Role.m_wszShortName;
        Stream << L" - " << Role.m_wszDescription;
    }

    ::MessageBox(hParentWindow, Stream.str().c_str(), wszApplicationName, MB_ICONINFORMATION | MB_OK);
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
