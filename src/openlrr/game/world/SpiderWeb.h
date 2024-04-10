// SpiderWeb.h : 
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

enum SpiderWeb_BlockFlags : uint32 // [LegoRR/SpiderWeb.c|flags:0x4|type:uint]
{
	BLOCKWEB_FLAG_NONE   = 0,
	BLOCKWEB_FLAG_ACTIVE = 0x100,
};
flags_end(SpiderWeb_BlockFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct SpiderWeb_Block // [LegoRR/SpiderWeb.c|struct:0xc]
{
	/*0,4*/	LegoObject* object; // "SpiderWeb" LiveObject
	/*4,4*/	real32 health; // Assumed as health, init: 100.0f
	/*8,4*/	SpiderWeb_BlockFlags flags;
	/*c*/
};
assert_sizeof(SpiderWeb_Block, 0xc);


struct SpiderWeb_Globs // [LegoRR/SpiderWeb.c|struct:0x8|tags:GLOBS]
{
	/*0,4*/	SpiderWeb_Block* webBlocks;
	/*4,4*/	Lego_Level* level;
	/*8*/
};
assert_sizeof(SpiderWeb_Globs, 0x8);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

 // <LegoRR.exe @005530e8>
extern SpiderWeb_Globs& spiderwebGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

/**
 * @brief Gets the 1D index of a level's spider web grid block at the specified 2D coordinates.
 */
#define spiderwebBlockIndex(bx, by)		(blockIndex(spiderwebGlobs.level, (bx), (by)))
/**
 * @brief Gets the level's spider web grid block at the specified 2D coordinates.
 */
#define spiderwebBlockValue(bx, by)		(spiderwebGlobs.webBlocks[spiderwebBlockIndex((bx),(by))])

#pragma endregion

   /**********************************************************************************
	******** Functions
	**********************************************************************************/

#pragma region Functions

	// <LegoRR.exe @00466480>
	//#define SpiderWeb_Initialise ((void (__cdecl* )(Lego_Level* level))0x00466480)
void __cdecl SpiderWeb_Initialise(Lego_Level* level);

// <LegoRR.exe @004664d0>
//#define SpiderWeb_Shutdown ((void (__cdecl* )(void))0x004664d0)
void __cdecl SpiderWeb_Shutdown(void);

// <LegoRR.exe @004664f0>
//#define SpiderWeb_Restart ((void (__cdecl* )(Lego_Level* level))0x004664f0)
void __cdecl SpiderWeb_Restart(Lego_Level* level);

// <LegoRR.exe @00466510>
//#define SpiderWeb_SpawnAt ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00466510)
bool32 __cdecl SpiderWeb_SpawnAt(uint32 bx, uint32 by);

// <LegoRR.exe @00466640>
//#define SpiderWeb_Add ((void (__cdecl* )(sint32 bx, sint32 by, LegoObject* webObj))0x00466640)
void __cdecl SpiderWeb_Add(sint32 bx, sint32 by, LegoObject* webObj);

// <LegoRR.exe @004666b0>
//#define SpiderWeb_GetAngle ((bool32 (__cdecl* )(sint32 bx, sint32 by, OUT real32* theta))0x004666b0)
bool32 __cdecl SpiderWeb_GetAngle(sint32 bx, sint32 by, OUT real32* theta);

// <LegoRR.exe @00466750>
//#define SpiderWeb_CheckCollision ((bool32 (__cdecl* )(LegoObject* liveObj))0x00466750)
bool32 __cdecl SpiderWeb_CheckCollision(LegoObject* liveObj);

// <LegoRR.exe @00466880>
//#define SpiderWeb_Update ((bool32 (__cdecl* )(real32 elapsedGame))0x00466880)
bool32 __cdecl SpiderWeb_Update(real32 elapsedGame);

// <LegoRR.exe @004668a0>
//#define SpiderWeb_LiveObjectCallback ((bool32 (__cdecl* )(LegoObject* liveObj, real32* pElapsedGame))0x004668a0)
bool32 __cdecl SpiderWeb_LiveObjectCallback(LegoObject* liveObj, real32* pElapsedGame);

// <LegoRR.exe @00466a10>
//#define SpiderWeb_Remove ((void (__cdecl* )(sint32 bx, sint32 by))0x00466a10)
void __cdecl SpiderWeb_Remove(sint32 bx, sint32 by);

#pragma endregion

}
