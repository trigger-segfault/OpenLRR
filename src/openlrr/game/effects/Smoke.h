// Smoke.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Images.h"

#include "../audio/SFX.h"
#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

// |          |cont   |anim  |thickn|wPos  |sfx  |
// |---------:|:------|:-----|:-----|:-----|:----|
// |Looping   |create |animSp|thickn|bx,by |sfxID|
// |Attach    |cont   |animSp|thickn|cont  |NULL |
// |Dissipate |create |2.0   |0.4   |bx,by |sfxID|
enum Smoke_Type : uint32 // [LegoRR/Smoke.c|enum:0x4|type:uint]
{
	Smoke_Type_Looping   = 0, // Standard smoke behaviour for block, will loop until manually removed, and has most configurability.
	Smoke_Type_Attach    = 1, // Attach to existing container's activity frame.
	Smoke_Type_Dissipate = 2, // Use 4 smoke groups and then vanish.
};
assert_sizeof(Smoke_Type, 0x4);


enum SmokeFlags : uint32 // [LegoRR/Smoke.c|flags:0x4|type:uint] SMOKE_FLAG_UNK_4 is hopelessly broken, and impossible to reverse.
{
	SMOKE_FLAG_NONE         = 0,

	SMOKE_FLAG_HIDDEN       = 0x1,
	SMOKE_FLAG_OWNCONTAINER = 0x2,  // (Mode 0,2) cont is self-owned, and must be cleaned up during Smoke_Remove.
	SMOKE_FLAG_BROKEN_UNK_4 = 0x4,  // (Mode 1,Show) Not enough information about this BROKEN flag to determine
									//  its usage. This flag is never checked for, only erroneously assigned.
	SMOKE_FLAG_REPEATCOUNT  = 0x8,  // (Mode 2) Smoke four groups and then dissipate
	SMOKE_FLAG_REPEATEND    = 0x10, // (Mode 2) Set after four smoke groups have been created.

	SMOKE_FLAG_SFXFAILED    = 0x20, // Only set when the sound3DPlayHandle == 0 after trying to play the SFX.
									// The logic for this is jank. It's only used for a condition block to stop
									//  trying to play a failed sound again.
};
flags_end(SmokeFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Smoke_Group // [LegoRR/Smoke.c|struct:0x3c]
{
	/*00,4*/	bool32 used;
	/*04,c*/	Vector3F randVec;	// (init: Smoke_Group_Show, stored randomness applied to dirVec to get moveVec, unused)
	/*10,c*/	Vector3F moveVec;	// (init: Smoke_Group_Show, readonly)
	/*1c,c*/	Vector3F position;	// (init: Container_GetPosition, updated)
	/*28,4*/	real32 float_28;	// (init: random[6.0f,9.0f])
	/*2c,4*/	real32 float_2c;	// (init: float_28 + random[2.0f,5.0f])
	/*30,4*/	real32 timer_30;	// Count-up timer related to alpha (never used for anything).
	/*34,4*/	sint32 int_34;		// (random: [40,69]) Related to constant scalar 0.7f
	/*38,4*/	real32 timerUp_38;	// Count-up timer (used for rendering).
	/*3c*/
};
assert_sizeof(Smoke_Group, 0x3c);


struct Smoke // [LegoRR/Smoke.c|struct:0x2a8]
{
	/*000,258*/	Smoke_Group groupList[10];
	/*258,4*/	real32 newSmokeTimer;	// Countdown timer until another smoke group is added.
	/*25c,c*/	Vector3F sfxOrigin;		// Originating position of SFX when played in world.
	/*268,c*/	Vector3F dirVec;		// Base move direction vector for smoke groups before randomness is applied.
	/*274,c*/	ColourRGBF colour;
	/*280,4*/	uint32 groupCount;
	/*284,4*/	sint32 numRepeats;		// (Dissipate) Number of repeats from 0-4 before vanishing.
	/*288,4*/	real32 animSpeed;		// Speed of creating new groups and applied to movement speed.
	/*28c,4*/	Gods98::Container* cont; // Attached or created container for mesh.
	/*290,4*/	Gods98::Mesh* mesh;
	/*294,4*/	SmokeFlags flags;
	/*298,4*/	Smoke* next;			// Next Smoke in doubly-linked list.
	/*29c,4*/	Smoke* previous;		// Previous Smoke in doubly-linked list.
	/*2a0,4*/	SFX_ID sfxID;
	/*2a4,4*/	sint32 sound3DPlayHandle;
	/*2a8*/
};
assert_sizeof(Smoke, 0x2a8);


struct Smoke_Globs // [LegoRR/Smoke.c|struct:0x34|tags:GLOBS]
{
	/*00,28*/	Gods98::Mesh_Texture* textureList[10];
	/*28,4*/	Smoke* firstSmoke; // A doublely-linked list, each node has next/previous as well
	/*2c,4*/	Smoke* lastSmoke;
	/*30,4*/	uint32 textureCount;
	/*34*/
};
assert_sizeof(Smoke_Globs, 0x34);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// Internal access only
// <LegoRR.exe @00553100>
extern Smoke_Globs & smokeGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00465640>
bool32 __cdecl Smoke_Initialise(void);

// <LegoRR.exe @00465660>
void __cdecl Smoke_LoadTextures(const char* dirname, const char* basename, uint32 textureCount);

// thickness: a range of (0.0f,1.0f], which simply determines the number of smoke groups (clouds?).
//   0 is invalid. 0.1 -> 1 cloud, 0.9 -> 9 clouds, 1.0 -> 10 clouds (max).
// Valid Smoke_Type modes: 0, 1, 2
// 
// <LegoRR.exe @004656f0>
Smoke* __cdecl Smoke_CreateSmokeArea(OPTIONAL IN Gods98::Container* attachedCont, uint32 bx, uint32 by,
									 const Vector3F* dirVec, real32 r, real32 g, real32 b, real32 thickness,
									 real32 animSpeed, Smoke_Type smokeType, uint32 randomness, SFX_ID sfxID);

// <LegoRR.exe @00465c30>
void __cdecl Smoke_RemoveAll(void);

// <LegoRR.exe @00465c70>
void __cdecl Smoke_Remove(Smoke* smoke, Gods98::Container* attachedCont);

// <LegoRR.exe @00465d50>
void __cdecl Smoke_HideAll(bool32 hide);

// <LegoRR.exe @00465d80>
void __cdecl Smoke_Hide(Smoke* smoke, bool32 hide);

// <LegoRR.exe @00465dc0>
void __cdecl Smoke_Update(real32 elapsedGame);

// <LegoRR.exe @00465f10>
void __cdecl Smoke_Group_Show(Smoke* smoke, uint32 groupID);

// <LegoRR.exe @004660c0>
void __cdecl Smoke_Group_Update(Smoke* smoke, uint32 groupID, real32 elapsedGame);

// <LegoRR.exe @004661a0>
void __cdecl Smoke_MeshRenderCallback(Gods98::Mesh* mesh, void* data, Gods98::Viewport* view);

// <LegoRR.exe @00466200>
void __cdecl Smoke_Group_MeshRenderCallback(Smoke* smoke, uint32 groupID, Gods98::Viewport* view, const Vector3F* lightPos);

#pragma endregion

}
