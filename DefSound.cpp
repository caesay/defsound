// ----------------------------------------------------------------------------
// DefSound.cpp
// DefSound Application
// ----------------------------------------------------------------------------

#include "stdafx.h"

// ----------------------------------------------------------------------------

#include "DefSoundTrayApp.h"
#include "DefSoundCommandLine.h"
#include "DefSoundHelpMessage.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

namespace {

// ----------------------------------------------------------------------------

const PCWSTR g_wszApplicationName = L"DefSound: Select Default Sound Render Device";

// ----------------------------------------------------------------------------

INT RunTrayMode(HINSTANCE hInstance)
{
    CTrayApp TrayApp(hInstance, g_wszApplicationName);
    return TrayApp.Run();
}

// ----------------------------------------------------------------------------

INT RunCommandLineMode(HINSTANCE hInstance, const CCommandLine &CommandLine)
{
    CEndpointCollection EndpointCollection;

    CCommandLine::CArgumentsPtr pArguments;
    std::wstring ErrorMessage;
    if (!CommandLine.Parse(EndpointCollection, pArguments, ErrorMessage))
    {
        ShowHelpMessage(g_wszApplicationName, nullptr, ErrorMessage.c_str());
        return STATUS_INVALID_PARAMETER;
    }
    _ASSERT(pArguments.get());

    if (EndpointCollection.Get().empty())
        throw CError( L"There are no audio endpoints", ERROR_SUCCESS );

    CHint Hint(hInstance);

    if (pArguments->m_DeviceIndexes.empty())
    {
        const auto nEndpointIndex = 
            EndpointCollection.SetDefaultNext(pArguments->m_Role);
        if (nEndpointIndex != -1)
            Hint.Show(EndpointCollection.Get()[nEndpointIndex]);
    }
    else
    {
        for (const auto nIndex : pArguments->m_DeviceIndexes)
        {
            try
            {
                EndpointCollection.SetDefault(nIndex, pArguments->m_Role);
                Hint.Show(EndpointCollection.Get()[nIndex]);
                break;
            }
            catch (const CError &Error)
            {
                UNREFERENCED_PARAMETER(Error);
            }
        }
    }

    return ERROR_SUCCESS;
}

// ----------------------------------------------------------------------------

INT ShowError(const CError &Error)
{
    std::wstringstream Stream;
    Stream << Error.m_Description << std::endl;

    static_assert(ERROR_SUCCESS == S_OK, "ERROR_SUCCESS == S_OK");
    static_assert(ERROR_SUCCESS == NO_ERROR, "ERROR_SUCCESS == NO_ERROR");
    if (Error.m_nErrorCode != ERROR_SUCCESS)
    {
        if (static_cast<long>(Error.m_nErrorCode) < 0)
        {
            Stream << L"Error 0x" << std::hex << Error.m_nErrorCode << std::endl;
        }
        else
        {
            Stream << L"Error " << std::dec << Error.m_nErrorCode << std::endl;
        }

        PWSTR wszMessage = NULL;
        const auto FormatResult =
            ::FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                Error.m_nErrorCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                reinterpret_cast<PWSTR>(&wszMessage),
                0,
                NULL);
        if (FormatResult)
        {
            std::unique_ptr<WCHAR, decltype(&::LocalFree)> MessageHolder(wszMessage, &::LocalFree);
            Stream << wszMessage;
        }
    }

    ::MessageBox(nullptr, Stream.str().c_str(), g_wszApplicationName, MB_ICONERROR | MB_OK);
    return (Error.m_nErrorCode != ERROR_SUCCESS) ? Error.m_nErrorCode : ERROR_INTERNAL_ERROR;
}

// ----------------------------------------------------------------------------

INT WinMainImpl(__in HINSTANCE hInstance)
{
    try
    {
        struct CCoInitialize
        {
            CCoInitialize()
            {
                HRESULT Result = ::CoInitialize(nullptr);
                if (S_OK != Result)
                    throw CError( MakeDefaultErrorDescription(L"::CoInitialize"), Result );
            }
            ~CCoInitialize()
            {
                ::CoUninitialize();
            }
        };
        CCoInitialize CoInitializeScoped;

        struct CGdiplusInitialize
        {
            CGdiplusInitialize()
            {
                ::GdiplusStartupInput Input;
                const auto Status = ::GdiplusStartup(&m_Token, &Input, nullptr);
                if (Status != Ok)
                    throw CError( MakeDefaultErrorDescription(L"::CoInitialize"), Status );
            }
            ~CGdiplusInitialize()
            {
                ::GdiplusShutdown(m_Token);
            }
            ULONG_PTR m_Token;
        };
        CGdiplusInitialize GdiplusInitializeScoped;

        CCommandLine CommandLine;
        if (CommandLine.IsTrayMode())
            return RunTrayMode(hInstance);

        return RunCommandLineMode(hInstance, CommandLine);
    }
    catch (const CError &Error)
    {
        return ShowError(Error);
    }
}

// ----------------------------------------------------------------------------

}   // namespace <nameless>

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------

INT WINAPI WinMain(
    __in HINSTANCE hInstance,
    __in_opt HINSTANCE hPrevInstance,
    __in_opt LPSTR lpCmdLine,
    __in int nShowCmd
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    return DefSound::WinMainImpl(hInstance);
}


// ----------------------------------------------------------------------------
