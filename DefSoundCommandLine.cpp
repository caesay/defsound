// ----------------------------------------------------------------------------
// DefSoundCommandLine.cpp
// Command line parser
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "DefSoundCommandLine.h"
#include "DefSoundEndpointRole.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

CCommandLine::CCommandLine()
    : m_ppArgs(nullptr, &::LocalFree)
{
    m_ppArgs.reset( ::CommandLineToArgvW(::GetCommandLineW(), &m_nNumArgs) );
    if (!m_ppArgs.get())
        throw CError( MakeDefaultErrorDescription(L"::CommandLineToArgvW") );

    if (!m_nNumArgs)
        throw CError( L"Unexpected count of command line arguments", ERROR_SUCCESS );
}

// ----------------------------------------------------------------------------

bool CCommandLine::IsTrayMode() const
{
    return (m_nNumArgs == 1);
}

// ----------------------------------------------------------------------------

bool CCommandLine::Parse(
    __in const CEndpointCollection &EndpointCollection,
    __out CArgumentsPtr &pArguments,
    __out std::wstring &ErrorMessage
) const
{
    _ASSERT(m_nNumArgs > 1);

    if (m_nNumArgs == 2)
    {
        const bool NeedhelpMessage = 
            !_wcsicmp(m_ppArgs.get()[1], L"-h") ||
            !_wcsicmp(m_ppArgs.get()[1], L"/h") ||
            !_wcsicmp(m_ppArgs.get()[1], L"-help") ||
            !_wcsicmp(m_ppArgs.get()[1], L"--help") ||
            !_wcsicmp(m_ppArgs.get()[1], L"/help") ||
            !_wcsicmp(m_ppArgs.get()[1], L"-?") ||
            !_wcsicmp(m_ppArgs.get()[1], L"/?");
        if (NeedhelpMessage)
        {
            ErrorMessage.clear();
            return false;
        }
    }

    if (m_nNumArgs > 3)
    {
        ErrorMessage = L"Wrong number of command line arguments";
        return false;
    }

    CArguments Arguments;

    if (!ParseDevices(EndpointCollection, Arguments, ErrorMessage))
        return false;

    if (!ParseRole(Arguments, ErrorMessage))
        return false;

    MoveDefaultTotail(EndpointCollection, Arguments);

    pArguments.reset( new CArguments(std::move(Arguments)) );
    return true;
}

// ----------------------------------------------------------------------------

bool CCommandLine::ParseDevices(
    __in const CEndpointCollection &EndpointCollection,
    __out CArguments &Arguments,
    __out std::wstring &ErrorMessage
) const
{
    _ASSERT(m_nNumArgs >= 2);

    auto &DeviceIndexes = Arguments.m_DeviceIndexes;

    PCWSTR wszDevice = m_ppArgs.get()[1];
    PCWSTR wszNewDevice = wcschr(wszDevice, L',');
    for (; ; )
    {
        std::wstring Device;
        if (wszNewDevice)
            Device.assign(wszDevice, wszNewDevice);
        else
            Device.assign(wszDevice);

        if (Device == L"-1")
        {
            if (!wszNewDevice && DeviceIndexes.empty())
                break;

            ErrorMessage = L"Special index -1 (next endpoint) must be alone";
            return false;
        }

        unsigned int nDeviceIndex;
        if (1 == swscanf_s(Device.c_str(), L"%i", &nDeviceIndex))
        {
            if (nDeviceIndex >= EndpointCollection.Get().size())
            {
                ErrorMessage = L"Endpoint not found: (by index)";
                return false;
            }

            if (std::find(DeviceIndexes.cbegin(), DeviceIndexes.cend(), nDeviceIndex) == DeviceIndexes.cend())
                DeviceIndexes.push_back( nDeviceIndex );
        }
        else
        {
            auto it = EndpointCollection.Get().cbegin();
            const size_t nEndpointCollectionSize = EndpointCollection.Get().size();
            for (size_t i = 0; i < nEndpointCollectionSize; ++i, ++it)
            {
                if (it->m_DeviceDesc == Device)
                {
                    if (std::find(DeviceIndexes.cbegin(), DeviceIndexes.cend(), i) == DeviceIndexes.cend())
                        DeviceIndexes.push_back( i );
                    break;
                }
            }
            if (it == EndpointCollection.Get().cend())
            {
                ErrorMessage = L"Endpoint not found (by description)";
                return false;
            }
        }

        if (!wszNewDevice)
            break;
        wszDevice = wszNewDevice + 1;
        if (!*wszDevice)
            break;
        wszNewDevice = wcschr(wszDevice, L',');
    }

    return true;
}

// ----------------------------------------------------------------------------

bool CCommandLine::ParseRole(
    __out CArguments &Arguments,
    __out std::wstring &ErrorMessage
) const
{
    if (m_nNumArgs == 2 || !_wcsicmp(GetEndpointAllRoles().m_wszShortName, m_ppArgs.get()[2]))
    {
        Arguments.m_Role = GetEndpointAllRoles().m_RoleValue;
        return true;
    }

    class CPred
    {
    public:
        explicit CPred(__in PCWSTR wszRole)
            : m_wszRole(wszRole)
        {
        }
        bool operator()(const CEndpointRole &EndpointRole) const
        {
            return !_wcsicmp(EndpointRole.m_wszShortName, m_wszRole);
        }
    private:
        PCWSTR m_wszRole;
    };
    const CEndpointRoleArray &EndpointRoleArray = GetEndpointRoleArray();
    CEndpointRoleArray::const_iterator it = 
        std::find_if(
            EndpointRoleArray.cbegin(),
            EndpointRoleArray.cend(),
            CPred(m_ppArgs.get()[2]));
    if (it == EndpointRoleArray.cend())
    {
        ErrorMessage = L"Unknown Role value";
        return false;
    }

    Arguments.m_Role = it->m_RoleValue;
    return true;
}

// ----------------------------------------------------------------------------

void CCommandLine::MoveDefaultTotail(
    __in const CEndpointCollection &EndpointCollection,
    __inout CArguments &Arguments
)
{
    for (const auto nIndex : Arguments.m_DeviceIndexes)
    {
        if (EndpointCollection.IsDefault(nIndex, Arguments.m_Role))
        {
            while (*Arguments.m_DeviceIndexes.rbegin() != nIndex)
            {
                const size_t nMovedIndex = *Arguments.m_DeviceIndexes.rbegin();
                Arguments.m_DeviceIndexes.pop_back();
                Arguments.m_DeviceIndexes.push_front(nMovedIndex);
            }
            break;
        }
    }
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
