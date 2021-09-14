#pragma once

#include "../common.h"
#include "Keys.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

//#define CONFIG_MAXCONFIG			100
#define CONFIG_MAXDEPTH				100
#define CONFIG_BINARYFILEID			"GBCF"

#define CONFIG_CONVERTIONINCPC		200

#define CONFIG_MAXLISTS				32			// 2^32 - 1 possible configs...

#define CONFIG_SEPARATOR			"::"

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

//#define CONFIG_FLAG_INITIALISED		0x00000001
//#define CONFIG_FLAG_LOADINGCONFIG	0x00000002

enum Config_GlobFlags : uint32
{
	CONFIG_FLAG_NONE          = 0,

	CONFIG_FLAG_INITIALISED   = 0x1,
	CONFIG_FLAG_LOADINGCONFIG = 0x2,

};
DEFINE_ENUM_FLAG_OPERATORS(Config_GlobFlags);
static_assert(sizeof(Config_GlobFlags) == 0x4, "");


enum Config_ReadStage : sint32
{
	Config_ReadStage_PreName = 0,
	Config_ReadStage_Name,
	Config_ReadStage_PreData,
	Config_ReadStage_Data,

	Config_ReadStage_End,
};
static_assert(sizeof(Config_ReadStage) == 0x4, "");

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

/// TODO: Consider name changes for `itemName`, `dataString` fields to `key`, `value`.
struct Config
{
	/*00,4*/ char* fileData; // held exclusively by root Config node, holds string used for all `itemName`, `dataString` fields.
	/*04,4*/ const char* itemName;   // (key)
	/*08,4*/ const char* dataString; // (value)
	/*0c,4*/ uint32 depth;
	/*10,4*/ uint32 itemHashCode; // (unused field)
	/*14,4*/ Config* linkNext;
	/*18,4*/ Config* linkPrev;
	/*1c,4*/ Config* nextFree; // (listSet field)
	/*20*/
};// Config, * lpConfig;
static_assert(sizeof(Config) == 0x20, "");


struct Config_Globs
{
	/*000,400*/ char s_JoinPath_string[1024]; // (not part of Manager, static array in JoinPath func)
	/*400,80*/ Config* listSet[CONFIG_MAXLISTS];
	/*480,4*/ Config* freeList;
	/*484,4*/ uint32 listCount;
	/*488,4*/ Config_GlobFlags flags;
	/*48c*/
};
static_assert(sizeof(Config_Globs) == 0x48c, "");

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00507098>
extern Config_Globs & configGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

#ifdef DEBUG
	#define Config_CheckInit()			if (!(configGlobs.flags & Config_GlobFlags::CONFIG_FLAG_INITIALISED)) Error_Fatal(TRUE, "Error: Config_Intitialise() Has Not Been Called");
#else
	#define Config_CheckInit()
#endif

// <LegoRR.exe @004790b0>
void __cdecl Config_Initialise(void);

// <LegoRR.exe @004790e0>
void __cdecl Config_Shutdown(void);

/*void Config_SetCharacterTable(const char* fname);
void Config_SetCharacterConvertFile(const char* fname);
void Config_ReadCharacterTable(const char* fname);
char __cdecl Config_ConvertCharacter(char c);
void __cdecl Config_SetLanguageDatabase(const char* langFile);
void __cdecl Config_ReadLanguageDatabase(const char* langFile);
char* __cdecl Config_ConvertString(const char* s, const char* sectionName, uint32* size, sint32 spaceToUnderscore);
void __cdecl Config_DumpUnknownPhrases(const char* fname);
void* __cdecl Config_LoadConvertedText(const char* fname, uint32* fileSize);*/

// <LegoRR.exe @00479120>
Config* __cdecl Config_Load(const char* filename);

// <LegoRR.exe @00479210>
const char* __cdecl Config_BuildStringID(const char* s, ...);

// <inlined>
__inline const char* Config_GetItemName(const Config* conf) { return conf->itemName; }

// <inlined>
__inline const char* Config_GetDataString(const Config* conf) { return conf->dataString; }

// <LegoRR.exe @004792b0>
const Config* __cdecl Config_FindArray(const Config* root, const char* name);

// <LegoRR.exe @004792e0>
const Config* __cdecl Config_GetNextItem(const Config* start);
//char* __cdecl Config_GetStringValue(const Config* root, const char* stringID, unsigned long line, const char* file);

// <LegoRR.exe @00479310>
char* Config_GetStringValue(const Config* root, const char* stringID);

// <LegoRR.exe @00479370>
const char* __cdecl Config_GetTempStringValue(const Config* root, const char* stringID);

// <LegoRR.exe @00479390>
BoolTri __cdecl Config_GetBoolValue(const Config* root, const char* stringID);

// <LegoRR.exe @004793d0>
real32 __cdecl Config_GetAngle(const Config* root, const char* stringID);

// <LegoRR.exe @00479430>
bool32 __cdecl Config_GetRGBValue(const Config* root, const char* stringID, OUT real32* r, OUT real32* g, OUT real32* b);

// <missing>
bool32 __cdecl Config_GetCoord(const Config* root, const char* stringID, OUT real32* x, OUT real32* y, OUT real32* z);

// <missing>
bool32 __cdecl Config_GetKey(const Config* root, const char* stringID, OUT Keys* key);

// <LegoRR.exe @00479500>
void __cdecl Config_Free(Config* root);


// <debug>
const char* __cdecl Config_Debug_GetLastFind(void);


// <LegoRR.exe @00479530>
Config* __cdecl Config_Create(Config* prev);

// <LegoRR.exe @00479580>
void __cdecl Config_Remove(Config* dead);

// <LegoRR.exe @004795a0>
const Config* __cdecl Config_FindItem(const Config* conf, const char* stringID);

//void __cdecl Config_DumpStructure(const Config* conf);

// <LegoRR.exe @00479750>
void __cdecl Config_AddList(void);

//void __cdecl Config_RunThroughLists(void);

//__inline const char* Config_GetItemName(const Config* conf) { return conf->itemName; }
//__inline const char* Config_GetDataString(const Config* conf) { return conf->dataString; }



#define Config_GetIntValue(c,s)		std::atoi(Gods98::Config_GetTempStringValue((c),(s))?Gods98::Config_GetTempStringValue((c),(s)):"")
#define Config_GetRealValue(c,s)	(float)std::atof(Gods98::Config_GetTempStringValue((c),(s))?Gods98::Config_GetTempStringValue((c),(s)):"")
#define Config_Get3DCoord(c,s,v)	Config_GetCoord((c),(s),&((v)->x),&((v)->y),&((v)->z))
#define Config_Get2DCoord(c,s,x,y)	Config_GetCoord((c),(s),(x),(y),nullptr)

/// CUSTOM:
#define Config_ID(s, ...) Config_BuildStringID(s, __VA_ARGS__, nullptr)

#pragma endregion

}
