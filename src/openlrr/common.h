// common.h : include file for standard C runtime include files,
//            or project specific include files.
//

#pragma once


#pragma warning (push)
#pragma warning (disable : 4005)

#pragma region Windows Includes

// I give up on trying to remove windows.h from common.h....
#include "platform/windows.h"

#pragma endregion


#pragma region C/C++ RunTime Includes
// C++ RunTime Header Files
#include <cmath>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cassert>
#include <ctime>
#include <cstdint>

// C++ STL Header Files
#include <string>
#include <array>
#include <vector>
#include <map>
#include <functional>
#include <type_traits>
//#include <typeinfo>

// Windows C RunTime Header Files
#include <direct.h>
#include <io.h>
#pragma endregion

#pragma warning (pop)


// Most of these are from older projects, and will be refactored-away later
#pragma region Global Configuration Settings

// Use `int32_t`, `uint32_t`, etc. over `int`, `unsigned int` for typedefs.
#define USE_STDINT_TYPEDEFS


// Always show cursor? (default is false)
#define SHOWCURSOR true
// Default to windowed mode when opening the Mode Selection Dialog (default is false)
#define DEFAULTWINDOWED true

// On startup, the program will only run if another instance is not already running. (default is true)
#define USEMUTEX true
// Name used for mutex that should be exclusive to this program (name will have " Mutex" appended).
#define MUTEX_NAME "OpenLRR"
// Original mutex name used by LEGO Rock Raiders.
//#define MUTEX_NAME "Lego Rock Raiders"

// Don't read standard command line parameters into our arguments
#define IGNORESTANDARDPARAMETERS true

#pragma endregion


#pragma region Debug Configuration Settings

// Removes sections of code that initialize unused features
#define DEBUG_REMOVEUNUSED
// Set this here to stop CD-ROM drive from spinning up all the time.
#define DEBUG_IGNORECD
// Include 32-bit registered screen mode
#define DEBUG_INCLUDE32BIT
// Ignore whether our desired screen mode is registerd by LRR (such as ignoring bit depth)
//#define DEBUG_IGNOREREGISTERED

#pragma endregion


#pragma region GODS constants

// Standard Gods engine FPS constant. Used everywhere
#define STANDARD_FRAMERATE		25.0f

#pragma endregion


#pragma region Basic type definitions

// Ansi single-byte/multi-byte character strings
typedef char			ansi;
typedef ansi*			ansi_str;
typedef const ansi*		const_ansi_str;

// unsigned ansi for scenarios where GODS uses unsigned character comparisons.
typedef unsigned char	uansi;
typedef uansi*			uansi_str;
typedef const uansi*	const_uansi_str;

// Informative typedefs for single-byte characters that should never be > 0x7f
typedef ansi			ascii;
typedef ascii*			ascii_str;
typedef const ascii*	const_ascii_str;

// Wide 2-byte unicode character strings (Windows specific)
typedef wchar_t			wide;
typedef wide*			wide_str;
typedef const wide*		const_wide_str;

/*typedef wchar_t			utf16;
typedef utf16*			utf16_str;
typedef const utf16*	const_utf16_str;

typedef wchar_t			unicode;
typedef unicode*		unicode_str;
typedef const unicode*	const_unicode_str;*/


/// REMOVEME: Old char typedefs
typedef char			char8;
typedef signed char		schar8;
typedef unsigned char	uchar8;


#ifdef USE_STDINT_TYPEDEFS

typedef int8_t			sint8;
typedef uint8_t			uint8;
typedef int16_t			sint16;
typedef uint16_t		uint16;
typedef int32_t			sint32;
typedef uint32_t		uint32;
typedef int64_t			sint64;
typedef uint64_t		uint64;

#else

typedef signed char			sint8;
typedef unsigned char		uint8;
typedef short				sint16;
typedef unsigned short		uint16;
typedef int					sint32;
typedef unsigned int		uint32;
typedef long long			sint64;
typedef unsigned long long	uint64;

#endif


typedef float			real32;
typedef double			real64;

typedef sint32			bool32; // WIN BOOL

typedef uint32			colour32;

#pragma endregion


#pragma region Macro functions

// Gets the name of a symbol as a C string
#define nameof_(symbol) #symbol
#define nameof(symbol) nameof_(symbol)

#define assert_sizeof(type, size) static_assert(sizeof(type) == (size), "Improper type size for " nameof(type))

// Shorthand cast to unsigned character for GODS string comparisons
#define uchr(c) ((uansi)(c))
#define ustr(c) ((uansi_str)(c))
#define ucstr(c) ((const_uansi_str)(c))


#define noinline(funcname) _noinline_ ##funcname


#define enum_scoped(name) \
	namespace _ns_ ##name { \
	enum name

#define flags_scoped(name) enum_scoped(name)


#define enum_scoped_end(name, size) \
	assert_sizeof(name, size); \
	} using name = _ns_ ##name## :: ##name

#define flags_scoped_end(name, size) \
	DEFINE_ENUM_FLAG_OPERATORS(name); \
	enum_scoped_end(name, size)


#define enum_end(name, size) \
	assert_sizeof(name, size)

#define flags_end(name, size) \
	DEFINE_ENUM_FLAG_OPERATORS(name); \
	assert_sizeof(name, size)


#define log_firstcall() { static bool _log_firstcallbool = false; \
	if (!_log_firstcallbool) {_log_firstcallbool = true; \
		std::printf("%s called\n", __FUNCTION__); } }

#pragma endregion


#pragma region Basic enum types

enum BoolTri : bool32
{
	BOOL3_FALSE = 0,
	BOOL3_TRUE  = 1,
	BOOL3_ERROR = 2, // would `BOOL3_ERR` be better?
};
assert_sizeof(BoolTri, 0x4);

#pragma endregion

