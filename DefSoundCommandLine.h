// ----------------------------------------------------------------------------
// DefSoundCommandLine.h
// Command line parser
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

#include "DefSoundEndpointColl.h"

// ----------------------------------------------------------------------------

namespace DefSound {
    
// ----------------------------------------------------------------------------

class CCommandLine
{
public:
    CCommandLine();

    bool IsTrayMode() const;

    struct CArguments
    {
        std::list< size_t > m_DeviceIndexes;
        ERole m_Role;
    };
    typedef std::unique_ptr< const CArguments > CArgumentsPtr;
    bool Parse(
        __in const CEndpointCollection &EndpointCollection,
        __out CArgumentsPtr &pArguments,
        __out std::wstring &ErrorMessage
    ) const;

protected:
    bool ParseDevices(
        __in const CEndpointCollection &EndpointCollection,
        __out CArguments &Arguments,
        __out std::wstring &ErrorMessage
    ) const;

    bool ParseRole(
        __out CArguments &Arguments,
        __out std::wstring &ErrorMessage
    ) const;

    static void MoveDefaultTotail(
        __in const CEndpointCollection &EndpointCollection,
        __inout CArguments &Arguments
    );

private:
    std::unique_ptr<WCHAR *, decltype(&::LocalFree)> m_ppArgs;
    INT m_nNumArgs;
};

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
