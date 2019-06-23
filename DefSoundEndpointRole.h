// ----------------------------------------------------------------------------
// DefSoundEndpointRole.h
// Audio endpoint roles
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

struct CEndpointRole
{
    enum
    {
        Console,
        Multimedia,
        Communications,
        All,

        CountOf
    };

    PCWSTR m_wszShortName;
    PCWSTR m_wszFullName;
    PCWSTR m_wszDescription;
    ::ERole m_RoleValue;
};

// ----------------------------------------------------------------------------

static_assert(CEndpointRole::Console == ::eConsole, "CEndpointRole::Console == ::eConsole");
static_assert(CEndpointRole::Multimedia == ::eMultimedia, "CEndpointRole::Multimedia == ::eMultimedia");
static_assert(CEndpointRole::Communications == ::eCommunications, "CEndpointRole::Communications == ::eCommunications");

// ----------------------------------------------------------------------------

typedef std::array<CEndpointRole, ERole_enum_count > CEndpointRoleArray;

// ----------------------------------------------------------------------------

const CEndpointRoleArray &GetEndpointRoleArray();
const CEndpointRole &GetEndpointAllRoles();

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
