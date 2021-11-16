// MeshLOD.h : 
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

enum MeshLODFlags : uint32
{
	MESHLOD_FLAG_NONE     = 0,
	MESHLOD_FLAG_CLONED   = 0x1, // Don't free part names or origin containers.
	MESHLOD_FLAG_MEMBLOCK = 0x2, // This MeshLOD is the start of a memory allocation.
	                             // This is unset for non-first items in cloned MeshLOD sets, where the
	                             // entire structure is allocated as an array of contiguous memory.
};
flags_end(MeshLODFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct MeshLOD // [LegoRR/MeshPoly.c|struct:0x18]
{
	/*00,4*/	Gods98::Container* contMeshOrigin; // (LWO|MESH, true)
	/*04,4*/	Gods98::Container* contMeshTarget;
	/*08,4*/	char* partName; // name of LoadObject file.lwo
	/*0c,4*/	uint32 setID;	// MeshLOD's may contain multiple sets of the same parts, this specifies which set its from.
	/*10,4*/	MeshLODFlags flags;
	/*14,4*/	MeshLOD* next;
	/*18*/
};
assert_sizeof(MeshLOD, 0x18);

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

// <LegoRR.exe @00451c70>
MeshLOD* __cdecl MeshLOD_Create(OPTIONAL MeshLOD* prevMeshLOD, const char* partName, const char* dirname, const char* meshName, uint32 setID);
//#define MeshLOD_Create ((MeshLOD* (__cdecl* )(OPTIONAL MeshLOD* prevMeshLOD, const char* partName, const char* dirname, const char* meshName, uint32 setID))0x00451c70)

// Creates a MeshLOD struct pointing to a non-existant Mesh.
// AKA, when a part is configured as NULL in the.ae file.
// <LegoRR.exe @00451d70>
MeshLOD* __cdecl MeshLOD_CreateEmpty(OPTIONAL MeshLOD* prevMeshLOD, const char* partName, uint32 setID);
//#define MeshLOD_CreateEmpty ((MeshLOD* (__cdecl* )(OPTIONAL MeshLOD* prevMeshLOD, const char* partName, uint32 setID))0x00451d70)

// <LegoRR.exe @00451df0>
MeshLOD* __cdecl MeshLOD_Clone(IN MeshLOD* srcMeshLOD);
//#define MeshLOD_Clone ((MeshLOD* (__cdecl* )(IN MeshLOD* srcMeshLOD))0x00451df0)

// <LegoRR.exe @00451e80>
void __cdecl MeshLOD_SwapTarget(MeshLOD* meshLOD, Gods98::Container* contActTarget, bool32 restore, uint32 setID);
//#define MeshLOD_SwapTarget ((void (__cdecl* )(MeshLOD* meshLOD, Gods98::Container* contActTarget, bool32 restore, uint32 setID))0x00451e80)

// <LegoRR.exe @00451ef0>
void __cdecl MeshLOD_RemoveTargets(MeshLOD* meshLOD);
//#define MeshLOD_RemoveTargets ((void (__cdecl* )(MeshLOD* meshLOD))0x00451ef0)

// <LegoRR.exe @00451f10>
void __cdecl MeshLOD_Free(MeshLOD* meshLOD);
//#define MeshLOD_Free ((void (__cdecl* )(MeshLOD* meshLOD))0x00451f10)

#pragma endregion

}
