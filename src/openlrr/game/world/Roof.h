// Roof.h : 
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

enum RoofFlags : uint32 // [LegoRR/Roof.c|flags:0x4|type:uint]
{
	ROOF_FLAG_NONE          = 0,
	ROOF_FLAG_HIDDEN        = 0x1,
	ROOF_FLAG_NEEDUPDATE    = 0x2,
	ROOF_FLAG_SHIFTVERTICES = 0x4,
};
flags_end(RoofFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct RoofBlock // [LegoRR/Roof.c|struct:0x8] Mesh group for a single block in the world's roof. (only used for FP)
{
	/*0,4*/	uint32 groupID;
	/*4,4*/	RoofFlags flags; // (0x1 = hidde, 0x2 = needsUpdate, 0x4 = shiftVertices)
	/*8*/
};
assert_sizeof(RoofBlock, 0x8);

struct Roof_Globs // [LegoRR/Roof.c|struct:0x7f0|tags:GLOBS] A level's roof mesh displayed in the world (only when in FP)
{
	/*000,4*/	Gods98::Container* contMesh; // MakeMesh2:IMMEDIATE
	/*004,8*/	Size2I dimensions;
	/*00c,4*/	bool32 hidden;
	/*010,4*/	bool32 needsUpdate; // Similar to the ROOF_NEEDSUPDATE flag, but is true if any blocks have to be updated
	/*014,4*/	Gods98::Container_Texture* texture;
	/*018,4*/	RoofBlock* grid; // Allocation size of 8, may be same as struct type below
	/*01c,7d0*/	RoofBlock* visibleTable[500]; // items are either null, or point to a block in grid
	/*7ec,4*/	uint32 visibleCount; // (not sure if "visible" is correct)
	/*7f0*/
};
assert_sizeof(Roof_Globs, 0x7f0);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00553180>
extern Roof_Globs& roofGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00463060>
bool32 __cdecl Roof_Initialise(Gods98::Container* contRoot, sint32 width, sint32 height);

// <LegoRR.exe @00463120>
bool32 __cdecl Roof_SetTexture(const char* filename);

// <LegoRR.exe @00463190>
void __cdecl Roof_Shutdown(void);

// <LegoRR.exe @004631e0>
void __cdecl Roof_SetBlockRoofVertices(uint32 bx, uint32 by, const Vector3F* vertPos0, const Vector3F* vertPos1, const Vector3F* vertPos2, const Vector3F* vertPos3);

// <LegoRR.exe @004634f0>
void __cdecl Roof_Update(void);

// <LegoRR.exe @00463770>
void __cdecl Roof_Hide(bool32 hide);

// I'm not actually sure if this is supposed to be "visible" blocks. But it's a
// list of blocks that are assigned and cleared, all in a single FP update loop.
// <LegoRR.exe @004637b0>
void __cdecl Roof_AddVisibleBlock(uint32 bx, uint32 by);

// <LegoRR.exe @00463800>
void __cdecl Roof_HideAllVisibleBlocks(void);

// <LegoRR.exe @00463850>
bool32 __cdecl Roof_GetBlockPlaneNormal(uint32 bx, uint32 by, OUT Vector3F* normal);

#pragma endregion

}
