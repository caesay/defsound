// ----------------------------------------------------------------------------
// DefSoundTrayEndpointMenu.h
// Popup menu
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

#include "DefSoundEndpointColl.h"
#include "DefSoundHint.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

class CEndpointMenu
{
public:
    CEndpointMenu(__in HINSTANCE hInstance);

    typedef std::unique_ptr<::HMENU__, decltype(&::DestroyMenu)> CMenuHolder;

    void BuildPopupMenu(
        __in HMENU hMainMenu,
        __in UINT_PTR nFirstEndpointId,
        __out std::list< CMenuHolder > &EndpointMenuHolder
    );

    bool OnCommandEndpoint(WORD nMenuItemNumber);

    void OnCommandNextEndpoint();

private:
    CEndpointCollection m_EndpointCollection;
    CHint m_Hint;
};

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------

