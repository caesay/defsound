// ----------------------------------------------------------------------------
// DefSoundError.h
// DefSound exception
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

struct CError
{
public:
    explicit CError(const std::wstring &Description, DWORD nErrorCode = ::GetLastError());

    std::wstring m_Description;
    DWORD m_nErrorCode;
};

// ----------------------------------------------------------------------------

std::wstring MakeDefaultErrorDescription(
    __in PCWSTR wszFuctionName, 
    __in PCWSTR wszArguments = L"(...)"
);

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
