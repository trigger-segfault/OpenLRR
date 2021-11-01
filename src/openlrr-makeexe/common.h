// common.h : Include file for standard C runtime include files,
//            or project specific include files.
//

#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <tchar.h>
#include <windows.h>
#include <algorithm>
#include <string>
#include <vector>
#include <memory>


/////////////////////////////////////////////////////////////////////
// Undefines

// Undefined windows.h macros that would replace std:: functions.
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


/////////////////////////////////////////////////////////////////////
// Missing defines

// Define if window.h is not included.
#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif


/////////////////////////////////////////////////////////////////////
// Macro functions

// (secondary macro needed for macro expansion)
#define nameof_(symbol) #symbol
#define nameof(symbol) nameof_(symbol)

#define tstrsizeof(s) ((_tcslen((s)) + 1) * sizeof(TCHAR))


/////////////////////////////////////////////////////////////////////
// Typedefs

using tstring = std::basic_string<TCHAR>;

typedef char ascii;

