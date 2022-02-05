// Map3D.h : 
//

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
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef real32 (__cdecl* GetWorldZCallback)(real32 xPos, real32 yPos, Map3D* map);

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
	/*0018,8*/		Size2F worldDimensions_fnegx; // (-blockWidth, +blockHeight) * BlockSize / 2.0f  (blockDimensions)
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
	/*73cc,4*/		Gods98::Container* transMesh; // Mesh for Map3D_TransitionBlocks
	/*73d0,4*/		Map3DFlags flagsMap;
	/*73d4*/
};
assert_sizeof(Map3D, 0x73d4);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004ebed0>
extern Point2I (& s_SurfaceMap_Points10)[10];

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

/**
 * @brief Gets the 1D index of a map's block3D at the specified 2D coordinates.
 */
#define mapBlockIndex(m, bx, by)		(((m)->gridWidth*(uint32)(by))+(uint32)(bx))
/**
 * @brief Gets the map's block3D at the specified 2D coordinates.
 */
#define mapBlockValue(m, bx, by)		((m)->blocks3D[mapBlockIndex((m),(bx),(by))])
//#define mapBlockValue(m, bx, by)		((m)->blocks3D[((m)->gridWidth * (uint32)(by)) + (uint32)(bx)])

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

 // <LegoRR.exe @0044e380>
#define Map3D_Create ((Map3D* (__cdecl* )(Gods98::Container* root, const char* filename, real32 blockSize, real32 roughLevel))0x0044e380)

// <LegoRR.exe @0044e790>
#define Map3D_InitRoughness ((void (__cdecl* )(Map3D* map))0x0044e790)

// <LegoRR.exe @0044e930>
#define Map3D_Remove ((void (__cdecl* )(Map3D* map))0x0044e930)

// <LegoRR.exe @0044e970>
#define Map3D_SetTextureNoFade ((void (__cdecl* )(Map3D* map, SurfaceTexture texture))0x0044e970)

// <LegoRR.exe @0044e990>
#define Map3D_SetBlockFadeInTexture ((void (__cdecl* )(Map3D* map, sint32 bx, sint32 by, SurfaceTexture newTexture, uint8 direction))0x0044e990)

// <LegoRR.exe @0044eb20>
#define Map3D_IsBlockMeshHidden ((bool32 (__cdecl* )(Map3D* map, sint32 bx, sint32 by))0x0044eb20)

// <LegoRR.exe @0044eb40>
#define Map3D_UpdateAllBlockNormals ((void (__cdecl* )(Map3D* map))0x0044eb40)

// <LegoRR.exe @0044eb80>
#define Map3D_CheckBuildingTolerance ((bool32 (__cdecl* )(Map3D* map, Point2I* shapePoints, uint32 shapeCount, real32 buildTolerance, real32 buildMaxVariation))0x0044eb80)

// <LegoRR.exe @0044ed90>
#define Map3D_FlattenShapeVertices ((void (__cdecl* )(Map3D* map, Point2I* shapePoints, uint32 shapeCount, real32 mult_4_0))0x0044ed90)

// <LegoRR.exe @0044f0b0>
#define Map3D_SetBlockRotated ((void (__cdecl* )(Map3D* map, uint32 bx, uint32 by, bool32 on))0x0044f0b0)

// <LegoRR.exe @0044f270>
#define Map3D_SetBlockVertexModified ((void (__cdecl* )(Map3D* map, uint32 vx, uint32 vy))0x0044f270)

// <LegoRR.exe @0044f2b0>
#define Map3D_Update ((void (__cdecl* )(Map3D* map, real32 elapsedGame))0x0044f2b0)

// <LegoRR.exe @0044f350>
#define Map3D_UpdateFadeInTransitions ((void (__cdecl* )(Map3D* map, real32 elapsedGame))0x0044f350)

// <LegoRR.exe @0044f460>
#define Map3D_AddTextureMapping ((void (__cdecl* )(Map3D* map, SurfaceTexture texA, SurfaceTexture texB))0x0044f460)

// <LegoRR.exe @0044f4e0>
#define Map3D_SetTextureSet ((void (__cdecl* )(Map3D* map, Detail_TextureSet* tset))0x0044f4e0)

// <LegoRR.exe @0044f4f0>
#define Map3D_SetBlockTexture ((void (__cdecl* )(Map3D* map, uint32 bx, uint32 by, SurfaceTexture newTexture, uint8 direction))0x0044f4f0)

// <LegoRR.exe @0044f640>
#define Map3D_MoveBlockVertices ((void (__cdecl* )(Map3D* map, uint32 bx, uint32 by, real32 zDist))0x0044f640)

// <LegoRR.exe @0044f750>
#define Map3D_SetPerspectiveCorrectionAll ((void (__cdecl* )(Map3D* map, bool32 on))0x0044f750)

// <LegoRR.exe @0044f7a0>
#define Map3D_SetBlockHighlight ((WallHighlightType (__cdecl* )(Map3D* map, sint32 bx, sint32 by, WallHighlightType highlightType))0x0044f7a0)

// <LegoRR.exe @0044f800>
#define Map3D_GetBlockHighlight ((WallHighlightType (__cdecl* )(Map3D* map, sint32 bx, sint32 by))0x0044f800)

// <LegoRR.exe @0044f830>
#define Map3D_ClearBlockHighlight ((void (__cdecl* )(Map3D* map, sint32 bx, sint32 by))0x0044f830)

// <LegoRR.exe @0044f880>
#define Map3D_Block_SetColour ((void (__cdecl* )(Map3D* map, sint32 bx, sint32 by, bool32 setColour, real32 r, real32 g, real32 b))0x0044f880)

// <LegoRR.exe @0044f900>
#define Map3D_BlockToWorldPos ((bool32 (__cdecl* )(Map3D* map, uint32 bx, uint32 by, OUT real32* xPos, OUT real32* yPos))0x0044f900)

// <LegoRR.exe @0044f990>
#define Map3D_WorldToBlockPos_NoZ ((bool32 (__cdecl* )(Map3D* map, real32 xPos, real32 yPos, OUT sint32* bx, OUT sint32* by))0x0044f990)

// <LegoRR.exe @0044f9c0>
#define Map3D_WorldToBlockPos ((bool32 (__cdecl* )(Map3D* map, real32 xPos, real32 yPos, OUT sint32* bx, OUT sint32* by, OUT real32* unk_zPos))0x0044f9c0)

// <LegoRR.exe @0044fad0>
#define Map3D_FUN_0044fad0 ((void (__cdecl* )(Map3D* map, real32 xPos, real32 yPos, OUT sint32* bx, OUT sint32* by))0x0044fad0)

// Always returns false.
// <LegoRR.exe @0044fb30>
#define Map3D_FUN_0044fb30 ((bool32 (__cdecl* )(Map3D* map, const Point2F* wPos2D, OPTIONAL OUT Point2F* blockPosf, OPTIONAL OUT Point2F* centerDir))0x0044fb30)

// <LegoRR.exe @0044fc00>
#define Map3D_GetWorldZ ((real32 (__cdecl* )(LegoRR::Map3D* map, real32 xPos, real32 yPos))0x0044fc00)

// <LegoRR.exe @0044fd70>
#define Map3D_FUN_0044fd70 ((void (__cdecl* )(Map3D* map, real32 xPos, real32 yPos, OUT Vector3F* vector))0x0044fd70)

// <LegoRR.exe @0044fe50>
#define Map3D_FUN_0044fe50 ((bool32 (__cdecl* )(Map3D* map, real32 xPos, real32 yPos, bool32 diagonal, real32 unkMultiplier, OUT real32* xOut, OUT real32* yOut))0x0044fe50)

// <LegoRR.exe @00450130>
#define Map3D_UnkCameraXYFunc_RetZunk ((real32 (__cdecl* )(Map3D* map, real32 xPos, real32 yPos))0x00450130)

// <LegoRR.exe @00450320>
#define Map3D_GetBlockFirstVertexPosition ((void (__cdecl* )(Map3D* map, sint32 vx, sint32 vy, OUT Vector3F* vector))0x00450320)

// <LegoRR.exe @00450390>
#define Map3D_GetBlockVertexPositions ((bool32 (__cdecl* )(Map3D* map, uint32 bx, uint32 by, OUT Vector3F* vertPoses))0x00450390)

// <LegoRR.exe @004504e0>
#define Map3D_GetBlockVertexPositions_NoRot ((void (__cdecl* )(Map3D* map, uint32 bx, uint32 by, OUT Vector3F* vertPoses))0x004504e0)

// <LegoRR.exe @00450580>
#define Map3D_IsInsideDimensions ((bool32 (__cdecl* )(Map3D* map, uint32 bx, uint32 by))0x00450580)

// <LegoRR.exe @004505a0>
#define Map3D_GetIntersections ((bool32 (__cdecl* )(Map3D* map, Gods98::Viewport* view, uint32 mouseX, uint32 mouseY, OUT uint32* bx, OUT uint32* by, OUT Vector3F* vector))0x004505a0)

// <LegoRR.exe @00450820>
#define Map3D_Intersections_Sub1_FUN_00450820 ((bool32 (__cdecl* )(Map3D* map, const Vector3F* rayOrigin, const Vector3F* ray, OUT Vector3F* endPoint, OUT Point2I* blockPos, sint32 unkCount))0x00450820)

// <LegoRR.exe @004508c0>
#define Map3D_AddVisibleBlocksInRadius_AndDoCallbacks ((void (__cdecl* )(Map3D* map, sint32 bx, sint32 by, sint32 radius, OPTIONAL XYCallback callback))0x004508c0)

// <LegoRR.exe @004509c0>
#define Map3D_HideBlock ((void (__cdecl* )(Map3D* map, sint32 bx, sint32 by, bool32 hide))0x004509c0)

// <LegoRR.exe @004509f0>
#define Map3D_AddVisibleBlock ((void (__cdecl* )(Map3D* map, sint32 bx, sint32 by))0x004509f0)

// <LegoRR.exe @00450a40>
#define Map3D_HideVisibleBlocksList ((void (__cdecl* )(Map3D* map))0x00450a40)

// <LegoRR.exe @00450a90>
#define Map3D_BlockVertexToWorldPos ((void (__cdecl* )(Map3D* map, uint32 bx, uint32 by, OUT real32* xPos, OUT real32* yPos, OUT real32* zPos))0x00450a90)

// <LegoRR.exe @00450b50>
#define Map3D_BlockSize ((real32 (__cdecl* )(Map3D* map))0x00450b50)

// <LegoRR.exe @00450b60>
#define Map3D_CheckRoutingComparison_FUN_00450b60 ((sint32 (__cdecl* )(sint32 param_1, sint32 param_2, sint32 param_3, sint32 param_4))0x00450b60)

// <LegoRR.exe @00450c20>
#define Map3D_SetBlockUVWobbles ((void (__cdecl* )(LegoRR::Map3D* map, uint32 bx, uint32 by, bool32 on))0x00450c20)

// <LegoRR.exe @00450d40>
#define Map3D_SetEmissive ((void (__cdecl* )(Map3D* map, bool32 on))0x00450d40)

// <LegoRR.exe @00450e20>
#define Map3D_UpdateTextureUVs ((void (__cdecl* )(Map3D* map, real32 elapsedGame))0x00450e20)

// <LegoRR.exe @004511f0>
#define Map3D_UpdateBlockNormals ((void (__cdecl* )(Map3D* map, uint32 bx, uint32 by))0x004511f0)

// <LegoRR.exe @00451440>
#define Map3D_BlockPairHasTextureMatch ((bool32 (__cdecl* )(Map3D* map, uint32 bx1, uint32 by1, uint32 bx2, uint32 by2))0x00451440)

// <LegoRR.exe @004514f0>
#define Map3D_SetBlockDirectionNormal ((void (__cdecl* )(Map3D* map, uint32 bx, uint32 by, Direction direction, const Vector3F* normal))0x004514f0)

// <LegoRR.exe @00451590>
#define Map3D_GetBlockDirectionNormal ((bool32 (__cdecl* )(Map3D* map, uint32 bx, uint32 by, Direction direction, OUT Vector3F* normal))0x00451590)

// <LegoRR.exe @00451710>
#define Map3D_MoveBlockDirectionVertex ((void (__cdecl* )(Map3D* map, uint32 bx, uint32 by, Direction direction, const Vector3F* vertDist))0x00451710)

// <LegoRR.exe @004517b0>
#define Map3D_GenerateBlockPlaneNormals ((void (__cdecl* )(Map3D* map, uint32 bx, uint32 by))0x004517b0)

// <LegoRR.exe @00451860>
#define Map3D_MapFileGetSpecs ((void (__cdecl* )(const MapFileInfo* mapFileInfo, OUT uint32* width, OUT uint32* height))0x00451860)

// <LegoRR.exe @00451880>
#define Map3D_MapFileBlockValue ((uint16 (__cdecl* )(const MapFileInfo* mapFile, uint32 bx, uint32 by, uint32 gridWidth))0x00451880)

// <LegoRR.exe @004518a0>
#define Map3D_Intersections_Sub2_FUN_004518a0 ((bool32 (__cdecl* )(Map3D* map, uint32 bx, uint32 by, const Vector3F* rayOrigin, const Vector3F* ray, OUT Vector3F* vector))0x004518a0)


#pragma endregion

}
