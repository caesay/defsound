// ----------------------------------------------------------------------------
// DefSoundHint.h
// DefSound Hint (balloon)
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

#include "DefSoundHintThread.h"
#include "DefSoundEndpointColl.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

struct CEndpoint;

// ----------------------------------------------------------------------------

class CHint
{
public:
    CHint(HINSTANCE hInstance);

    void Show(const CEndpoint &Endpoint);

protected:
    HINSTANCE m_hInstance;
    std::unique_ptr< CHintWindowClass > m_pHintWindowClass;
    std::unique_ptr< CHintThread > m_pHintThread;
};

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------


