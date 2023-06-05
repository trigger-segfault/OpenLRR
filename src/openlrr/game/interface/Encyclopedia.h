// Encyclopedia.h : 
//

#pragma once

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum Encyclopedia_GlobFlags : uint32
{
	ENCYCLOPEDIA_GLOB_FLAG_NONE = 0,

	ENCYCLOPEDIA_GLOB_FLAG_ACTIVE    = 0x1,
	ENCYCLOPEDIA_GLOB_FLAG_NEEDSTEXT = 0x2,
};
flags_end(Encyclopedia_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

/// TODO: Instead of opening a file, just read all the text. This is wasting open wad file entry slots.
struct Encyclopedia_Globs // [LegoRR/Encyclopedia.c|struct:0x24|tags:GLOBS]
{
	/*00,4*/	Gods98::File** vehicleFiles;
	/*04,4*/	Gods98::File** minifigureFiles;
	/*08,4*/	Gods98::File** rockmonsterFiles;
	/*0c,4*/	Gods98::File** buildingFiles;
	/*10,4*/	Gods98::File* powercrystalFile;
	/*14,4*/	Gods98::File* oreFile;
	/*18,4*/	Gods98::File* currentObjFile;
	/*1c,4*/	LegoObject* currentObj;
	/*20,4*/	Encyclopedia_GlobFlags flags;
	/*24*/
};
assert_sizeof(Encyclopedia_Globs, 0x24);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004c8e88>
extern Encyclopedia_Globs & encyclopediaGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0040e3c0>
//#define Encyclopedia_Initialise ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0040e3c0)
void __cdecl Encyclopedia_Initialise(const Gods98::Config* config, const char* gameName);

// <LegoRR.exe @0040e630>
//#define Encyclopedia_SelectObject ((void (__cdecl* )(LegoObject* liveObj))0x0040e630)
void __cdecl Encyclopedia_SelectObject(LegoObject* liveObj);

// <LegoRR.exe @0040e710>
//#define Encyclopedia_ClearSelection ((void (__cdecl* )(void))0x0040e710)
void __cdecl Encyclopedia_ClearSelection(void);

// <LegoRR.exe @0040e720>
//#define Encyclopedia_Update ((void (__cdecl* )(real32 elapsedAbs))0x0040e720)
void __cdecl Encyclopedia_Update(real32 elapsedAbs);

// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @0040e800>
//#define Encyclopedia_DrawSelectBox ((void (__cdecl* )(Gods98::Viewport* viewMain))0x0040e800)
void __cdecl Encyclopedia_DrawSelectBox(Gods98::Viewport* viewMain);

/// CUSTOM: Isolate Draw API calls from Encyclopedia_DrawSelectBox.
void __cdecl Encyclopedia_DrawSelectName(Gods98::Viewport* viewMain);


// Removes the current encyclopedia object if it matches the specified object.
// <LegoRR.exe @0040e840>
//#define Encyclopedia_RemoveCurrentReference ((void (__cdecl* )(LegoObject* liveObj))0x0040e840)
void __cdecl Encyclopedia_RemoveCurrentReference(LegoObject* liveObj);

#pragma endregion

}
