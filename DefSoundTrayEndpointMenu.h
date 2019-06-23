// ----------------------------------------------------------------------------
// DefSoundTrayEndpointMenu.h
// Popup menu
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

#include "DefSoundEndpointColl.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

class CEndpointMenu
{
public:
    typedef std::unique_ptr<::HMENU__, decltype(&::DestroyMenu)> CMenuHolder;

    void BuildPopupMenu(
        __in HMENU hMainMenu,
        __in UINT_PTR nFirstEndpointId,
        __out std::list< CMenuHolder > &EndpointMenuHolder
    );

    bool OnCommandEndpoint(WORD nMenuItemNumber) const;

    void OnCommandNextEndpoint();

private:
    CEndpointCollection m_EndpointCollection;
};

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------

