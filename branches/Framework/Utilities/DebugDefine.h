#pragma once
//#define Win32
//#define Android
#define WindowPhone
//#define iOS;

#pragma once

#include "esUtil.h"

#if defined Win32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif
#endif