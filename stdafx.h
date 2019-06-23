// [pch]

#pragma once

#include <Windows.h>
#include <strsafe.h>

#include <MmDeviceApi.h>

#include <memory>
#include <sstream>
#include <array>
#include <list>
#include <vector>
#include <functional>

#include <process.h>

#pragma comment(lib, "comsuppw.lib")
#include <comip.h>
#include <comutil.h>

#include <Gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

#ifdef  _DEBUG
#define VERIFY(x)   _ASSERT(x)
#else   // _DEBUG
#define VERIFY(x)   (x)
#endif  // _DEBUG

#include "DefSoundError.h"