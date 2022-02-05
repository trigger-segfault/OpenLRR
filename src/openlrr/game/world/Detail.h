// Detail.h : 
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

enum Detail_TransformFlags : uint8 // [LegoRR/Detail.c|flags:0x1|type:byte]
{
	DETAIL_TRANSFORM_FLAG_NONE   = 0,
	DETAIL_ROTATE_FLAG_X1_DEG90  = 0x1,
	DETAIL_ROTATE_FLAG_X1_DEGM90 = 0x2,
	DETAIL_ROTATE_FLAG_Y1_DEG90  = 0x4,
	DETAIL_ROTATE_FLAG_Y1_DEGM90 = 0x8,
	DETAIL_ROTATE_FLAG_Z1_DEG90  = 0x10,
	DETAIL_ROTATE_FLAG_Z1_DEGM90 = 0x20,
	DETAIL_TRANSLATE_FLAG_Y10    = 0x40,
	DETAIL_TRANSLATE_FLAG_YM10   = 0x80,
};
flags_end(Detail_TransformFlags, 0x1);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Detail_Mesh // [LegoRR/Detail.c|struct:0x340]
{
	/*000,8*/	Gods98::Container* promesh_ab[2];
	/*008,320*/	Gods98::Container* clones_ab[2][100];
	/*328,4*/	real32 BlockSize;
	/*32c,4*/	uint32 cloneCount;
	/*330,c*/	undefined field_0x330_0x33b[12];
	/*33c,4*/	Detail_TextureSet* textureSet;
	/*340*/
};
assert_sizeof(Detail_Mesh, 0x340);


struct Detail_TextureSet // [LegoRR/Detail.c|struct:0xc]
{
	/*0,8*/	Size2I gridSize;
	/*8,4*/	Gods98::Container_Texture** gridSurfaces;
	/*c*/
};
assert_sizeof(Detail_TextureSet, 0xc);

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

// <LegoRR.exe @0040b240>
#define Detail_LoadTextureSet ((Detail_TextureSet* (__cdecl* )(const char* textureBaseName, uint32 width, uint32 height))0x0040b240)
//Detail_TextureSet* __cdecl Detail_LoadTextureSet(const char* textureBaseName, uint32 width, uint32 height);

// <LegoRR.exe @0040b340>
#define Detail_FreeTextureSet ((void (__cdecl* )(Detail_TextureSet* textureSet))0x0040b340)
//void __cdecl Detail_FreeTextureSet(Detail_TextureSet* textureSet);

// <LegoRR.exe @0040b3a0>
#define Detail_FUN_0040b3a0 ((real32 (__cdecl* )(const Vector3F* vertPoses, Gods98::Viewport* view, const Point2F* viewportSize, bool32 param_4))0x0040b3a0)
//real32 __cdecl Detail_FUN_0040b3a0(const Vector3F* vertPoses, Gods98::Viewport* view, const Point2F* viewportSize, bool32 param_4);

// <LegoRR.exe @0040b520>
#define Detail_LoadMeshes ((Detail_Mesh* (__cdecl* )(Gods98::Container* cont, const char* meshName_a, const char* meshName_b, real32 blockSize, Detail_TextureSet* textureSet))0x0040b520)
//Detail_Mesh* __cdecl Detail_LoadMeshes(Gods98::Container* cont, const char* meshName_a, const char* meshName_b, real32 blockSize, Detail_TextureSet* textureSet);

// <LegoRR.exe @0040b700>
#define Detail_FreeMesh ((void (__cdecl* )(Detail_Mesh* detailMesh))0x0040b700)
//void __cdecl Detail_FreeMesh(Detail_Mesh* detailMesh);

// <LegoRR.exe @0040b740>
#define Detail_RemoveMesh_FUN_0040b740 ((void (__cdecl* )(Detail_Mesh* detailMesh))0x0040b740)
//void __cdecl Detail_RemoveMesh_FUN_0040b740(Detail_Mesh* detailMesh);

// <LegoRR.exe @0040b780>
#define Detail_GetTexture ((Gods98::Container_Texture* (__cdecl* )(Detail_TextureSet* textureSet, SurfaceTexture texture))0x0040b780)
//Gods98::Container_Texture* __cdecl Detail_GetTexture(Detail_TextureSet* textureSet, SurfaceTexture texture);

// <LegoRR.exe @0040b7b0>
#define Detail_FUN_0040b7b0 ((void (__cdecl* )(LegoRR::Detail_Mesh* detailMesh, const Vector3F* vectPoses4, LegoRR::SurfaceTexture texture, uint8 direction, real32 scaleZ, real32 brightness))0x0040b7b0)
//void __cdecl Detail_FUN_0040b7b0(Detail_Mesh* detailMesh, const Vector3F* vectPoses4, SurfaceTexture texture, uint8 direction, real32 scaleZ, real32 brightness);

// <LegoRR.exe @0040b930>
#define Detail_Sub1_Transform ((void (__cdecl* )(Gods98::Container* cont, real32 blockSize, const Vector3F* p1, const Vector3F* p2, const Vector3F* p3, real32 scaleZ, real32 brightness, Gods98::Container_Texture* contTexture, Detail_TransformFlags transformFlags))0x0040b930)
//void __cdecl Detail_Sub1_Transform(Gods98::Container* cont, real32 blockSize, const Vector3F* p1, const Vector3F* p2, const Vector3F* p3,
//								   real32 scaleZ, real32 brightness, Gods98::Container_Texture* contTexture, Detail_TransformFlags transformFlags);

// <LegoRR.exe @0040bac0>
#define Detail_Sub2_FUN_0040bac0 ((void (__cdecl* )(Gods98::Container* cont, real32 blockSize, const Vector3F* p1, const Vector3F* p2, const Vector3F* p3))0x0040bac0)
//void __cdecl Detail_Sub2_FUN_0040bac0(Gods98::Container* cont, real32 blockSize, const Vector3F* p1, const Vector3F* p2, const Vector3F* p3);

// <LegoRR.exe @0040bc90>
#define Detail_Matrix_FUN_0040bc90 ((void (__cdecl* )(OUT Matrix4F* m, real32 blockWidth, real32 blockHeight, real32 invxP2_y, real32 invxP3_x, real32 invxP3_y))0x0040bc90)
//void __cdecl Detail_Matrix_FUN_0040bc90(OUT Matrix4F* m, real32 blockWidth, real32 blockHeight, real32 invxP2_y, real32 invxP3_x, real32 invxP3_y);

#pragma endregion

}
