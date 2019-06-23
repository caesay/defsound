// ----------------------------------------------------------------------------
// DefSoundEndpointRole.cpp
// Audio endpoint roles
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "DefSoundEndpointRole.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

namespace {

// ----------------------------------------------------------------------------

class CEndpointRoleArrayInitializer : public CEndpointRoleArray
{
public:
    CEndpointRoleArrayInitializer();
};

class CEndpointAllRoleInitializer : public CEndpointRole
{
public:
    CEndpointAllRoleInitializer();
};

// ----------------------------------------------------------------------------

CEndpointRoleArrayInitializer::CEndpointRoleArrayInitializer()
{
    at(CEndpointRole::Console).m_wszShortName = L"CON";
    at(CEndpointRole::Console).m_wszFullName = L"Console";
    at(CEndpointRole::Console).m_wszDescription = L"system notification sounds and voice commands";
    at(CEndpointRole::Console).m_RoleValue = eConsole;

    at(CEndpointRole::Multimedia).m_wszShortName = L"MMEDIA";
    at(CEndpointRole::Multimedia).m_wszFullName = L"Multimedia";
    at(CEndpointRole::Multimedia).m_wszDescription = L"multimedia";
    at(CEndpointRole::Multimedia).m_RoleValue = eMultimedia;

    at(CEndpointRole::Communications).m_wszShortName = L"VOICE";
    at(CEndpointRole::Communications).m_wszFullName = L"Communications";
    at(CEndpointRole::Communications).m_wszDescription = L"voice communications";
    at(CEndpointRole::Communications).m_RoleValue = eCommunications;
}

CEndpointAllRoleInitializer::CEndpointAllRoleInitializer()
{
    m_wszShortName = L"ALL";
    m_wszFullName = L"All Roles";
    m_wszDescription = L"all roles";
    m_RoleValue = ERole_enum_count;
}

// ----------------------------------------------------------------------------

CEndpointRoleArrayInitializer g_EndpointRoleArrayInitializer;
CEndpointAllRoleInitializer g_EndpointAllRoleInitializer;

// ----------------------------------------------------------------------------

}   // namespace <nameless>

// ----------------------------------------------------------------------------

const CEndpointRoleArray &GetEndpointRoleArray()
{
    return g_EndpointRoleArrayInitializer;
}

const CEndpointRole &GetEndpointAllRoles()
{
    return g_EndpointAllRoleInitializer;
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
