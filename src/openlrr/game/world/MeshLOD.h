// MeshLOD.h : 
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

enum MeshLODFlags : uint32 // [LegoRR/MeshLOD.c|flags:0x4|type:uint]
{
	MESHLOD_FLAG_NONE     = 0,
	MESHLOD_FLAG_CLONED   = 0x1,
	MESHLOD_FLAG_MEMBLOCK = 0x2,
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
	/*0c,4*/	uint32 setID; // MeshLOD's may contain multiple sets of the same parts, this specifies which set its from.
	/*10,4*/	MeshLODFlags flags; // (1 = dont free partName/cont_0,  2 = unk dtor behavior)
	/*14,4*/	MeshLOD* next;
	/*18*/
};
assert_sizeof(MeshLOD, 0x18);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00451c70>
#define MeshLOD_Create ((MeshLOD* (__cdecl* )(OPTIONAL MeshLOD* prevMeshLOD, const char* partName, const char* dirname, const char* meshName, uint32 setID))0x00451c70)
//MeshLOD* __cdecl MeshLOD_Create(OPTIONAL MeshLOD* prevMeshLOD, const char* partName, const char* dirname, const char* meshName, uint32 setID);

// <LegoRR.exe @00451d70>
#define MeshLOD_CreateEmpty ((MeshLOD* (__cdecl* )(OPTIONAL MeshLOD* prevMeshLOD, const char* partName, uint32 setID))0x00451d70)
//MeshLOD* __cdecl MeshLOD_CreateEmpty(OPTIONAL MeshLOD* prevMeshLOD, const char* partName, uint32 setID);

// <LegoRR.exe @00451df0>
#define MeshLOD_Clone ((MeshLOD* (__cdecl* )(IN MeshLOD* srcMeshLOD))0x00451df0)
//MeshLOD* __cdecl MeshLOD_Clone(IN MeshLOD* srcMeshLOD);

// <LegoRR.exe @00451e80>
#define MeshLOD_SwapTarget ((void (__cdecl* )(MeshLOD* meshLOD, Gods98::Container* contActTarget, bool32 restore, uint32 setID))0x00451e80)
//void __cdecl MeshLOD_SwapTarget(MeshLOD* meshLOD, Gods98::Container* contActTarget, bool32 restore, uint32 setID);

// <LegoRR.exe @00451ef0>
#define MeshLOD_RemoveTargets ((void (__cdecl* )(MeshLOD* meshLOD))0x00451ef0)
//void __cdecl MeshLOD_RemoveTargets(MeshLOD* meshLOD);

// <LegoRR.exe @00451f10>
#define MeshLOD_Free ((void (__cdecl* )(MeshLOD* meshLOD))0x00451f10)
//void __cdecl MeshLOD_Free(MeshLOD* meshLOD);

#pragma endregion

}
