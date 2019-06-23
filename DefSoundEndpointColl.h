// ----------------------------------------------------------------------------
// DefSoundEndpointColl.h
// Audio endpoint collection
// ----------------------------------------------------------------------------

#pragma once


// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

struct CEndpoint
{
    std::wstring m_DeviceId;
    std::wstring m_DeviceDesc;
    std::wstring m_DeviceClassIconPath;
    std::array< bool, ERole_enum_count > m_IsDefault;
};

// ----------------------------------------------------------------------------

class CEndpointCollection
{
public:
    CEndpointCollection();

    void Refresh();

    typedef std::vector< CEndpoint > CImpl;
    const CImpl &Get() const;

    
    // ERole_enum_count may be used (==CEndpointRole::All)

    bool IsDefault(__in size_t nIndex, __in ERole Role) const;

    void SetDefault(__in size_t nIndex, __in ERole Role) const;
    INT_PTR SetDefaultNext(__in ERole Role) const;   // -1 == no next

private:
    std::unique_ptr< const CImpl >  m_pImpl;
};

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
