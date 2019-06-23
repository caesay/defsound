// ----------------------------------------------------------------------------
// DefSoundHintThread.cpp
// DefSound Hint Thread (show window)
// ----------------------------------------------------------------------------

#include "stdafx.h"

// ----------------------------------------------------------------------------

#include "DefSoundHintThread.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

CHintThread::CHintThread(
    const std::wstring &Icon,
    const std::wstring &Text,
    HINSTANCE hInstance,
    std::unique_ptr< CHintWindowClass > &pHintWindowClass
)   : m_pEventWindowCreated(nullptr, &::CloseHandle)
    , m_pThread(nullptr, &::CloseHandle)
    , m_HintWindow(Icon, Text, hInstance, pHintWindowClass)
{
    m_pEventWindowCreated.reset( ::CreateEvent(nullptr, TRUE, FALSE, nullptr) );
    if (!m_pEventWindowCreated)
        throw CError( MakeDefaultErrorDescription(L"::CreateEvent") );
}

// ----------------------------------------------------------------------------

CHintThread::~CHintThread()
{
    if (m_pThread)
        VERIFY(::WaitForSingleObject(m_pThread.get(), INFINITE) == WAIT_OBJECT_0);
}

// ----------------------------------------------------------------------------

void CHintThread::Start()
{
    const auto Thread = ::_beginthreadex(nullptr, 0, ThreadProc, this, 0, nullptr);
    if (Thread == 0 || Thread == -1)
        throw CError( MakeDefaultErrorDescription(L"::_beginthreadex"), _doserrno );
    m_pThread.reset(reinterpret_cast<HANDLE>(Thread));

    VERIFY(::WaitForSingleObject(m_pEventWindowCreated.get(), INFINITE) == WAIT_OBJECT_0);
}

// ----------------------------------------------------------------------------

void CHintThread::ThreadProc()
{
    m_HintWindow.Create();

    ::SetEvent(m_pEventWindowCreated.get());

    m_HintWindow.Show();
}

// ----------------------------------------------------------------------------

unsigned __stdcall CHintThread::ThreadProc(void *pThis)
{
    reinterpret_cast<CHintThread *>(pThis)->ThreadProc();
    return 0;
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------


