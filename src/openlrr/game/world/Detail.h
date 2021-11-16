// Detail.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Images.h"

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

struct Detail_TextureSet // [LegoRR/???|struct:0xc]
{
	/*0,8*/	Size2I gridSize;
	/*8,4*/	Gods98::Container_Texture** gridSurfaces;
	/*c*/
};
assert_sizeof(Detail_TextureSet, 0xc);


struct Detail_Mesh // [LegoRR/???|struct:0x340]
{
	/*000,8*/	Gods98::Container* promesh_ab[2];
	/*008,320*/	undefined4 table_ab[2][100];
	/*328,4*/	real32 BlockSize;
	/*32c,4*/	undefined4 field_32c;
	/*330,c*/	undefined field_0x330_0x33b[12];
	/*33c,4*/	Detail_TextureSet* textureSet;
	/*340*/
};
assert_sizeof(Detail_Mesh, 0x340);

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

// <LegoRR.exe @0040b240>
#define Detail_LoadTextureSet ((Detail_TextureSet* (__cdecl* )(const char* textureBaseName, uint32 width, uint32 height))0x0040b240)

// <LegoRR.exe @0040b340>
#define Detail_FreeTextureSet ((void (__cdecl* )(Detail_TextureSet* textureSet))0x0040b340)

// <LegoRR.exe @0040b3a0>
#define Detail_FUN_0040b3a0 ((real32 (__cdecl* )(sint32 param_1, Gods98::Viewport* view, const Point2F* param_3, bool32 param_4))0x0040b3a0)

// <LegoRR.exe @0040b520>
#define Detail_LoadMeshes ((Detail_Mesh* (__cdecl* )(Gods98::Container* cont, const char* meshName_a, const char* meshName_b, real32 blockSize, Detail_TextureSet* textureSet))0x0040b520)

// <LegoRR.exe @0040b700>
#define Detail_FreeMesh ((void (__cdecl* )(Detail_Mesh* detailMesh))0x0040b700)

// <LegoRR.exe @0040b740>
#define Detail_RemoveMesh_FUN_0040b740 ((void (__cdecl* )(Detail_Mesh* detailMesh))0x0040b740)

// <LegoRR.exe @0040b780>
#define Detail_GetTexture ((Gods98::Container_Texture* (__cdecl* )(Detail_TextureSet* textureSet, SurfaceTexture coord))0x0040b780)

// <LegoRR.exe @0040b7b0>
#define Detail_FUN_0040b7b0 ((void (__cdecl* )(Detail_Mesh* detailMesh, const Vector3F* vectPoses4, SurfaceTexture texture, uint8 param_4, undefined4 param_5, undefined4 param_6))0x0040b7b0)

// <LegoRR.exe @0040b930>
#define Detail_Sub1_Transform ((void (__cdecl* )(Gods98::Container* cont, real32 blockSize, const Vector3F* position, const Vector3F* dir, const Vector3F* up, real32 scaleZ, real32 brightness, Gods98::Container_Texture* contTexture, Detail_TransformFlags transformFlags))0x0040b930)

// <LegoRR.exe @0040bac0>
#define Detail_Sub2_FUN_0040bac0 ((void (__cdecl* )(Gods98::Container* cont, real32 blockSize, const Vector3F* position, const Vector3F* dir, const Vector3F* up))0x0040bac0)

// <LegoRR.exe @0040bc90>
#define Detail_Matrix_FUN_0040bc90 ((void (__cdecl* )(OUT Matrix4F* m, real32 blockWidth, real32 blockHeight, real32 yDir, real32 xUp, real32 yUp))0x0040bc90)

#pragma endregion

}
