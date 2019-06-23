// ----------------------------------------------------------------------------
// DefSoundError.cpp
// DefSound exception
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "DefSoundError.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

CError::CError(const std::wstring &Description, DWORD nErrorCode)
    : m_Description(Description), m_nErrorCode(nErrorCode)
{
}

// ----------------------------------------------------------------------------

std::wstring MakeDefaultErrorDescription(
    __in PCWSTR wszFuctionName, 
    __in PCWSTR wszArguments /*= L"(...)"*/
)
{
    std::wstring Description = L"Call ";
    Description += wszFuctionName;
    Description += wszArguments;
    Description += L" failed";
    return Description;
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
