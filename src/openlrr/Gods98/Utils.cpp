// Utils.cpp : 
//

#include "Memory.h"

#include "Utils.h"


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

//typedef char* Util_StringMatrix[4][4];

// <missing>
char* __cdecl Gods98::Util_GetLine(IN OUT char** buffer, const char* bufferEnd)
{
	bool32 foundEnd = FALSE;
	char* s;
	char* line = *buffer;

	for (s = *buffer; s != bufferEnd; s++) {
		if (*s == '\r' || *s == '\n') {
			*s = '\0';
			foundEnd = true;
		}
		else {
			if (foundEnd) break;
		}
	}

	if (!foundEnd) return nullptr;

	*buffer = s;
	return line;
}

// <LegoRR.exe @00477700>
uint32 __cdecl Gods98::Util_Tokenise(IN OUT char* str, OUT char** argv, const char* sep)
{
	log_firstcall();

	char* s = str;
	uint32 count = 0, sl = std::strlen(sep);

	if (str[0] == '\0') return 0;

	argv[count++] = str;
	while (*s != '\0') {
		if (std::strncmp(sep, s, sl) == 0) {
			*s = '\0';
			argv[count++] = s + sl;
		}
		s++;
	}

	return count;
}

// <LegoRR.exe @00477770>
uint32 __cdecl Gods98::Util_WSTokenise(IN OUT char* str, OUT char** argv)
{
	log_firstcall();

	char* s = str;
	uint32 count = 0;

	if (str[0] == '\0') return 0;

	argv[count++] = str;
	while (*s != '\0'){
		if (std::isspace(*(uchar8*)s)) {
			*s++ = '\0';
			while (std::isspace(*(uchar8*)s)) s++;
			argv[count++] = s;
		} else s++;
	}

	return count;
}

// (char* ::_strdup(const char*))
// <LegoRR.exe @00477810>
char* __cdecl Gods98::Util_StrCpy(const char* str)
{
	log_firstcall();

	char* newString = (char*)Mem_Alloc(std::strlen(str) + 1);
	std::strcpy(newString, str);
	return newString;
}

// <LegoRR.exe @00477850>
char* __cdecl Gods98::Util_RemoveUnderscores(const char* str, ...)
{
	log_firstcall();

	const char* s;
	char* t;
	char completeString[UTIL_MAXSTRINGLENGTH], buffer[UTIL_MAXSTRINGLENGTH];
	uint32 len;
	std::va_list args;
	char* result;


	va_start(args, str);
	len = std::vsprintf(completeString, str, args);
	//Error_Fatal(len > UTIL_MAXSTRINGLENGTH, Error_Format("String too big for 'Util_RemoveUnderscores'.", str));
	va_end(args);

	t = buffer;
	for (s = completeString; '\0' != *s; s++)
	{
		if (('\\' == *s) && ('n' == *(s + 1)))
		{
			*t = '\n';
			s++;
		}
		else if ('_' == *s)
			*t = ' ';
		else
			*t = *s;

		t++;
	}
	*t = '\0';

	//result = /*Util_StrCpy*/((char* (__cdecl*)(const char*))0x00477810)(str);
	result = Util_StrCpy(str);

	return result;
}

// <LegoRR.exe @004778d0>
char* __cdecl Gods98::Util_StrIStr(char* str1, const char* str2)
{
	log_firstcall();

	uint32 len1 = std::strlen(str1);
	uint32 len2 = std::strlen(str2);

	if (!len1 || !len2)
		return NULL;

	for (uint32 i = 0; i < len1; i++)
	{
		char* str = str1 + i;
		if (::_strnicmp(str, str2, len2) == 0)
		{
			//			str+=len2;
			return str;
		}
	}

	return nullptr;
}
// const implementation
// <LegoRR.exe @004778d0>
//const char* __cdecl Gods98::Util_StrIStr(const char* str1, const char* str2)
//{
//	return Util_StrIStr(const_cast<char*>(str1), str2);
//}

// <LegoRR.exe @00477930>
uint32 __cdecl Gods98::Util_HashString(const char* str, bool32 bIgnoreBlanks, bool32 upperCase)
{
	log_firstcall();

	uint32 sum = 0;
	uint32 multiple = UTIL_LARGENUMBER;
	uint32 index = 1;

	while (*str != '\0') {
		if (bIgnoreBlanks) while (std::isspace(*(uchar8*)str)) str++;
		uchar8 c = (*(uchar8*)str++);
		if (upperCase) c = std::toupper(c);
		sum += multiple * index++ * c;
		multiple *= UTIL_LARGENUMBER;
	}
	return sum;
}

// <missing>
bool32 __cdecl Gods98::Util_IsNumber(const char* str)
{
	while (*str != '\0') {
		if (!std::isdigit(*(uchar8*)str) && *str != '.') return false;
	}

	return true;
}

// <LegoRR.exe @004779d0>
BoolTri __cdecl Gods98::Util_GetBoolFromString(const char* str)
{
	log_firstcall();

	if (::_stricmp(str, "YES") == 0 || ::_stricmp(str, "TRUE") == 0 || ::_stricmp(str, "ON") == 0)
		return BoolTri::BOOL3_TRUE;
	else if (::_stricmp(str, "NO") == 0 || ::_stricmp(str, "FALSE") == 0 || ::_stricmp(str, "OFF") == 0)
		return BoolTri::BOOL3_FALSE;

	return BoolTri::BOOL3_ERROR;		// Unknown...
}

/*VOID __cdecl Util_MultiplyStringMatrix(Util_StringMatrix r, Util_StringMatrix a, Util_StringMatrix b);
VOID __cdecl Util_PrintStringMatrix(Util_StringMatrix r);
VOID __cdecl Util_TransposeStringMatrix(Util_StringMatrix m);
VOID __cdecl Util_FreeStringMatrix(Util_StringMatrix m);*/

#pragma endregion
