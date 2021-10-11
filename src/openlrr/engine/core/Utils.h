// Utils.h : 
//
/// APIS: -
/// DEPENDENCIES: -, (Memory)
/// DEPENDENTS: ...

#pragma once

#include "../../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define UTIL_DEFSTRINGLEN		512
#define UTIL_LARGENUMBER		6293815		// For Util_HashString()
#define UTIL_MAXSTRINGLENGTH	256

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

//typedef char* Util_StringMatrix[4][4];

// <missing>
char* __cdecl Util_GetLine(IN OUT char** buffer, const char* bufferEnd);

// <CLGen.exe @00401440>
// <LegoRR.exe @00477700>
uint32 __cdecl Util_Tokenise(IN OUT char* str, OUT char** argv, const char* sep);

// <LegoRR.exe @00477770>
uint32 __cdecl Util_WSTokenise(IN OUT char* str, OUT char** argv);

// (char* ::_strdup(const char*))
// <LegoRR.exe @00477810>
char* __cdecl Util_StrCpy(const char* str);

// <LegoRR.exe @00477850>
char* __cdecl Util_RemoveUnderscores(const char* str, ...);

// <LegoRR.exe @004778d0>
char* __cdecl Util_StrIStr(char* str1, const char* str2);
// const implementation
// <LegoRR.exe @004778d0>
inline const char* __cdecl Util_StrIStr(const char* str1, const char* str2)
{
	return Util_StrIStr(const_cast<char*>(str1), str2);
}

// <LegoRR.exe @00477930>
uint32 __cdecl Util_HashString(const char* str, bool32 bIgnoreBlanks, bool32 upperCase);

// <missing>
bool32 __cdecl Util_IsNumber(const char* str);

// <LegoRR.exe @004779d0>
BoolTri __cdecl Util_GetBoolFromString(const char* str);

/*VOID __cdecl Util_MultiplyStringMatrix(Util_StringMatrix r, Util_StringMatrix a, Util_StringMatrix b);
VOID __cdecl Util_PrintStringMatrix(Util_StringMatrix r);
VOID __cdecl Util_TransposeStringMatrix(Util_StringMatrix m);
VOID __cdecl Util_FreeStringMatrix(Util_StringMatrix m);*/

#pragma endregion

}

