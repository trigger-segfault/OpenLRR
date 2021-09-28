// platform/targetver.h : 
//

#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
// <https://docs.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt?view=msvc-160>

// Disabled for now. Causing trouble with dsound
#if 0
#pragma warning (push)
#pragma warning (disable : 4005)

#include <WinSDKVer.h>

// Windows NT 4.0
//#define _WIN32_WINNT	0x0400
//#define WINVER			0x0400
//#define NTDDI_VERSION	0x04000000

// Windows 2000
#define _WIN32_WINNT	0x0500
#define WINVER			0x0500
#define NTDDI_VERSION	0x05000000

// Windows XP
//#define _WIN32_WINNT	0x0501
//#define WINVER			0x0501
//#define NTDDI_VERSION	0x05010000

#include <SDKDDKVer.h>

#pragma warning (pop)
#endif
