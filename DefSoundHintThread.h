// ----------------------------------------------------------------------------
// DefSoundHintThread.h
// DefSound Hint Thread (show window)
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

#include "DefSoundHintWindow.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

class CHintThread
{
public:
    CHintThread(
        const std::wstring &Icon,
        const std::wstring &Text,
        HINSTANCE hInstance,
        std::unique_ptr< CHintWindowClass > &pHintWindowClass
    );
    ~CHintThread();

    void Start();

protected:
    void ThreadProc();
    static unsigned __stdcall ThreadProc(void *pThis);

private:    
    std::unique_ptr<void, decltype(&::CloseHandle)> m_pEventWindowCreated;
    std::unique_ptr<void, decltype(&::CloseHandle)> m_pThread;

    CHintWindow m_HintWindow;
};

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
