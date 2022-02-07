// PTL.h : 
//

#pragma once

#include "../interface/Messages.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct MessageAction;

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define PTL_MAXPROPERTIES		40

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

struct PTL_Property // [LegoRR/PTL.c|struct:0x8] Property loaded from a level's PTL config file (contains lookup index for actions)
{
	/*0,4*/	Message_Type event;
	/*4,4*/	Message_Type action;
	/*8*/
};
assert_sizeof(PTL_Property, 0x8);


struct PTL_Globs // [LegoRR/PTL.c|struct:0x144|tags:GLOBS]
{
	/*000,140*/	PTL_Property table[PTL_MAXPROPERTIES];
	/*140,4*/	uint32 count;
	/*144*/
};
assert_sizeof(PTL_Globs, 0x144);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00556be0>
extern PTL_Globs & ptlGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0045daa0>
bool32 __cdecl PTL_Initialise(const char* fname, const char* gameName);

// <LegoRR.exe @0045db30>
void __cdecl PTL_EventToAction(IN OUT MessageAction* message);

#pragma endregion

}
