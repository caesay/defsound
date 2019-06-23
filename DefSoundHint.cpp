// ----------------------------------------------------------------------------
// DefSoundHint.cpp
// DefSound Hint (balloon)
// ----------------------------------------------------------------------------

#include "stdafx.h"

// ----------------------------------------------------------------------------

#include "DefSoundHint.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

CHint::CHint(HINSTANCE hInstance)
    : m_hInstance(hInstance)
{
}

// ----------------------------------------------------------------------------

void CHint::Show(const CEndpoint &Endpoint)
{
    std::unique_ptr< CHintThread > pHintThread(
        new CHintThread(
            Endpoint.m_DeviceClassIconPath,
            Endpoint.m_DeviceDesc,
            m_hInstance,
            m_pHintWindowClass)
    );

    m_pHintThread.swap(pHintThread);
    m_pHintThread->Start();
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------


