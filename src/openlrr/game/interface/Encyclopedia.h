// Encyclopedia.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/core/Config.h"
#include "../../engine/core/Files.h"

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

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

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
	/*20,4*/	uint32 flags;
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
#define Encyclopedia_Initialise ((void (__cdecl* )(const Gods98::Config* config, const char* keyBasePath))0x0040e3c0)

// <LegoRR.exe @0040e630>
#define Encyclopedia_SetObject ((void (__cdecl* )(LegoObject* liveObj))0x0040e630)

// <LegoRR.exe @0040e710>
#define Encyclopedia_UnsetFlag1 ((void (__cdecl* )(void))0x0040e710)

// <LegoRR.exe @0040e720>
#define Encyclopedia_Update ((void (__cdecl* )(real32 elapsedAbs))0x0040e720)

// <LegoRR.exe @0040e800>
#define Encyclopedia_DrawSelectBox ((void (__cdecl* )(Viewport* viewMain))0x0040e800)

// <LegoRR.exe @0040e840>
#define Encyclopedia_LiveObject_ClearPtr_004c8ea4_IfEqual ((void (__cdecl* )(LegoObject* liveObj))0x0040e840)

#pragma endregion

}
