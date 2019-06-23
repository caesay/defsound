// ----------------------------------------------------------------------------
// DefSoundTrayEndpointMenu.h
// Popup menu
// ----------------------------------------------------------------------------

#include "stdafx.h"

#include "DefSoundTrayEndpointMenu.h"
#include "DefSoundEndpointRole.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

namespace {

// ----------------------------------------------------------------------------

const PCWSTR g_wszNoAudioEndpointsText = L"No audio endpoints";

// ----------------------------------------------------------------------------

}   // namespace <nameless>

// ----------------------------------------------------------------------------

CEndpointMenu::CEndpointMenu(__in HINSTANCE hInstance)
    : m_Hint(hInstance)
{
}

// ----------------------------------------------------------------------------

void CEndpointMenu::BuildPopupMenu(
    __in HMENU hMainMenu,
    __in UINT_PTR nFirstEndpointId,
    __out std::list< CMenuHolder > &EndpointMenuHolder
)
{
    m_EndpointCollection.Refresh();

    if (!m_EndpointCollection.Get().empty())
    {

        UINT_PTR nEndpointId = nFirstEndpointId;
        for (const auto &Endpoint : m_EndpointCollection.Get())
        {
            CMenuHolder MenuHolder(::CreatePopupMenu(), &::DestroyMenu);

            bool bAllRoles = true;
            for (const auto &Role : GetEndpointRoleArray())
                bAllRoles = bAllRoles && Endpoint.m_IsDefault[Role.m_RoleValue];

            auto Result = 
                ::AppendMenu(
                    MenuHolder.get(),
                    bAllRoles ? MF_CHECKED : MF_UNCHECKED,
                    nEndpointId++,
                    GetEndpointAllRoles().m_wszFullName);
            if (!Result)
                throw CError( MakeDefaultErrorDescription(L"::AppendMenu") );

            for (const auto &Role : GetEndpointRoleArray())
            {
                Result = 
                    ::AppendMenu(
                        MenuHolder.get(),
                        Endpoint.m_IsDefault[Role.m_RoleValue] ? MF_CHECKED : MF_UNCHECKED,
                        nEndpointId++,
                        Role.m_wszFullName);
                if (!Result)
                    throw CError( MakeDefaultErrorDescription(L"::AppendMenu") );
            }

            Result = 
                ::AppendMenu(
                    hMainMenu,
                    MF_POPUP | (bAllRoles ? MF_CHECKED : MF_UNCHECKED),
                    reinterpret_cast<UINT_PTR>(MenuHolder.get()),
                    Endpoint.m_DeviceDesc.c_str());
            if (!Result)
                throw CError( MakeDefaultErrorDescription(L"::AppendMenu") );

            EndpointMenuHolder.push_back( std::move(MenuHolder) );
        }
    }
    else
    {
        if (!::AppendMenu(hMainMenu, MF_GRAYED, 0, g_wszNoAudioEndpointsText))
            throw CError( MakeDefaultErrorDescription(L"::AppendMenu") );
    }
}

// ----------------------------------------------------------------------------

bool CEndpointMenu::OnCommandEndpoint(WORD nMenuItemNumber)
{
    _ASSERT(nMenuItemNumber < m_EndpointCollection.Get().size() * CEndpointRole::CountOf);
    if (nMenuItemNumber >= m_EndpointCollection.Get().size() * CEndpointRole::CountOf)
        return false;

    const size_t &nEndpointIndex = nMenuItemNumber / CEndpointRole::CountOf;
    const size_t nRoleIndex = nMenuItemNumber % CEndpointRole::CountOf;

    if (!nRoleIndex)
        m_EndpointCollection.SetDefault(nEndpointIndex, GetEndpointAllRoles().m_RoleValue);
    else
        m_EndpointCollection.SetDefault(nEndpointIndex, static_cast<::ERole>(nRoleIndex - 1));

    m_Hint.Show(m_EndpointCollection.Get()[nEndpointIndex]);

    return true;
}

// ----------------------------------------------------------------------------

void CEndpointMenu::OnCommandNextEndpoint()
{
    m_EndpointCollection.Refresh();
    const auto nEndpointIndex = 
        m_EndpointCollection.SetDefaultNext(GetEndpointAllRoles().m_RoleValue);
    if (nEndpointIndex != -1)
        m_Hint.Show(m_EndpointCollection.Get()[nEndpointIndex]);
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
