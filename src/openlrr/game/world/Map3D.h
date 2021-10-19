#pragma once

#include "../../engine/gfx/Containers.h"
#include "../../engine/gfx/Materials.h"

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define MAP3D_MAXTRANSITIONBLOCKS	10
#define MAP3D_MAXTEXTURES			128

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum Map3DFlags : uint32 // [LegoRR/Map3D.c|flags:0x4|type:uint]
{
	MAP3D_FLAG_NONE           = 0,

	MAP3D_FLAG_VERTEXMODIFIED = 0x2,
	MAP3D_FLAG_EMISSIVE_4     = 0x4,
};
flags_end(Map3DFlags, 0x4);


enum Map3D_TransitionFlags : uint8 // [LegoRR/Map3D.c|flags:0x1|type:byte]
{
	MAP3DTRANS_FLAG_NONE           = 0,

	MAP3DTRANS_FLAG_USED           = 0x1,
};
flags_end(Map3D_TransitionFlags, 0x1);


enum Map3D_BlockFlags : uint8 // [LegoRR/Map3D.c|flags:0x1|type:byte] Flags for SurfaceMapStruct_28->byte_19
{
	MAP3DBLOCK_FLAG_NONE           = 0,

	MAP3DBLOCK_FLAG_ROTATED        = 0x1,

	MAP3DBLOCK_FLAG_VERTEXMODIFIED = 0x4,
	MAP3DBLOCK_FLAG_VERTEXLEVELED  = 0x8,
	MAP3DBLOCK_FLAG_UVWOBBLES      = 0x10, // Camera_Update has been called once for LegoCamera_FP type
	MAP3DBLOCK_FLAG_UNK_20         = 0x20, // set on map load, but never used
	MAP3DBLOCK_FLAG_UNK_40         = 0x40, // set on map load, but never used
};
flags_end(Map3D_BlockFlags, 0x1);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Map3D_TransitionBlock // [LegoRR/Map3D.c|struct:0x38]
{
	/*00,4*/	sint32 groupID; // (init: -1, no group created yet)
	/*04,4*/	real32 timer; // count-up timer [0.05f-1.0f]
	/*08,8*/	Point2I blockPos;
	/*10,20*/	Point2F uvCoords[4];
	/*30,4*/	undefined4 reserved1;
	/*34,1*/	Map3D_TransitionFlags flags; // (0x1 = visible, only flag)
	///*35,3*/	undefined padding1[3];
	/*38*/
};
assert_sizeof(Map3D_TransitionBlock, 0x38);


struct Map3D_Block // [LegoRR/Map3D.c|struct:0x28]
{
	/*00,c*/	Vector3F normalA; // Maths_PlaneNormal of vertex positions 0,1,3
	/*0c,c*/	Vector3F normalB; // Maths_PlaneNormal of vertex positions 1,2,3
	/*18,1*/	SurfaceTexture texture;
	/*19,1*/	Map3D_BlockFlags flags3D;
	/*1a,1*/	uint8 heightValue; // height block value taken from high.map file
	///*1b,1*/	undefined padding1;
	/*1c,8*/	Point2F uvCoord;
	/*24,1*/	uint8 highlight; // (WallHighlightType)
	///*25,3*/	undefined padding2[3];
	/*28*/
};
assert_sizeof(Map3D_Block, 0x28);


struct Map3D // [LegoRR/Map3D.c|struct:0x73d4]
{
	/*0000,4*/		uint32 blockWidth; // gridWidth - 1
	/*0004,4*/		uint32 blockHeight; // gridHeight - 1
	/*0008,8*/		uint32 gridWidth; // full map width
	/*000c,4*/		uint32 gridHeight; // full map height
	/*0010,4*/		real32 BlockSize; // (cfg: BlockSize)
	/*0014,4*/		real32 RoughLevel; // (cfg: RoughLevel)
	/*0018,8*/		Size2F worldDimensions_fnegx; // (-width, +height) * BlockSize / 2.0f  (smallDimensions)
	/*0020,4*/		real32 float_20; // (some sort of maximum)
	/*0024,4*/		Gods98::Container* mesh; // Mesh for Map3D_Blocks
	/*0028,4*/		Map3D_Block* blocks3D; // [*:dimensions]
	/*002c,4*/		Detail_TextureSet* textureSet;
	/*0030,4*/		Gods98::Material* material; // (UNUSED: And never released!!!)
	/*0034,4000*/	uint8 texsGrid[MAP3D_MAXTEXTURES][MAP3D_MAXTEXTURES]; // [MAP3D_MAXTEXTURES][MAP3D_MAXTEXTURES]
	/*4034,80*/		uint8 texsNum[MAP3D_MAXTEXTURES]; // [MAP3D_MAXTEXTURES]
	/*40b4,200*/	bool32 texsNoFade[MAP3D_MAXTEXTURES]; // [MAP3D_MAXTEXTURES] True for textures that cannot fade transition
	/*42b4,7d0*/	Coord2U visibleBlocksTable[500]; // (related to FP detail)
	/*4a84,4*/		uint32 visibleBlocksNum;
	/*4a88,2710*/	Coord2U uvBlocksTable[2500]; // Blocks that require UV updates (EMISSIVE or UVWOBBLES)
	/*7198,4*/		uint32 uvBlocksNum;
	/*719c,230*/	Map3D_TransitionBlock transBlocks[MAP3D_MAXTRANSITIONBLOCKS]; // [BASEMAPTEXTURES??]
	/*73cc,4*/		Gods98::Container* transMesh; // Mesh for Map3D_TransBlocks
	/*73d0,4*/		Map3DFlags flagsMap;
	/*73d4*/
};
assert_sizeof(Map3D, 0x73d4);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals


#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0044f990>
#define Map3D_WorldToBlockPos_NoZ ((bool32(__cdecl *)(LegoRR::Map3D* map,real32 xPos,real32 yPos,OUT sint32* bx,OUT sint32* by))0x0044f990)

// <LegoRR.exe @0044fc00>
#define Map3D_GetWorldZ ((real32(__cdecl *)(LegoRR::Map3D* map,real32 xPos,real32 yPos))0x0044fc00)

// <LegoRR.exe @00450130>
#define Map3D_UnkCameraXYFunc_RetZunk ((real32(__cdecl *)(LegoRR::Map3D* map,real32 xPos,real32 yPos))0x00450130)

#pragma endregion

}
