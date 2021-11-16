// MeshLOD.cpp : 
//

#include "../../engine/core/Errors.h"
#include "../../engine/core/Memory.h"
#include "../../engine/core/Utils.h"
#include "../../engine/gfx/Containers.h"

#include "MeshLOD.h"


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00451c70>
LegoRR::MeshLOD* __cdecl LegoRR::MeshLOD_Create(OPTIONAL MeshLOD* prevMeshLOD, const char* partName, const char* dirname, const char* meshName, uint32 setID)
{
	char fname[256];

	std::sprintf(fname, "%s\\%s", dirname, meshName);
	Gods98::Container* cont;
	if ((cont = Gods98::Container_Load(nullptr, fname, "LWO", true)) == nullptr) {
		if ((cont = Gods98::Container_Load(nullptr, fname, "MESH", true)) == nullptr) {
			Error_Fatal(true, Gods98::Error_Format("Failed to load MeshLOD part: %s", fname));
			return nullptr; // Failed to load meshLOD part resource.
		}
	}

	MeshLOD* newMeshLOD = (MeshLOD*)Gods98::Mem_Alloc(sizeof(MeshLOD));

	if (prevMeshLOD != nullptr) prevMeshLOD->next = newMeshLOD;

	newMeshLOD->contMeshOrigin = cont;
	newMeshLOD->contMeshTarget = nullptr;
	newMeshLOD->partName = Gods98::Util_StrCpy(partName);
	newMeshLOD->setID = setID;
	newMeshLOD->flags = MeshLODFlags::MESHLOD_FLAG_MEMBLOCK; // Strangely not related to passing non-null prevMeshLOD.
	newMeshLOD->next = nullptr;

	return newMeshLOD;
}

// Creates a MeshLOD struct pointing to a non-existant Mesh.
// AKA, when a part is configured as NULL in the.ae file.
// <LegoRR.exe @00451d70>
LegoRR::MeshLOD* __cdecl LegoRR::MeshLOD_CreateEmpty(OPTIONAL MeshLOD* prevMeshLOD, const char* partName, uint32 setID)
{
	MeshLOD* newMeshLOD = (MeshLOD*)Gods98::Mem_Alloc(sizeof(MeshLOD));

	if (prevMeshLOD != nullptr) prevMeshLOD->next = newMeshLOD;

	newMeshLOD->contMeshOrigin = nullptr;
	newMeshLOD->contMeshTarget = nullptr;
	newMeshLOD->partName = Gods98::Util_StrCpy(partName);
	newMeshLOD->setID = setID;
	newMeshLOD->flags = MeshLODFlags::MESHLOD_FLAG_MEMBLOCK; // Strangely not related to passing non-null prevMeshLOD.
	newMeshLOD->next = nullptr;

	return newMeshLOD;
}

// <LegoRR.exe @00451df0>
LegoRR::MeshLOD* __cdecl LegoRR::MeshLOD_Clone(IN MeshLOD* srcMeshLOD)
{
	uint32 count = 0;
	for (MeshLOD* m = srcMeshLOD; m != nullptr; m = m->next) {
		count++;
	}

	if (count == 0)
		return nullptr; // srcMeshLOD is null.

	// Unlike MeshLOD's created with Create, a cloned MeshLOD is created as contiguous memory.
	MeshLOD* clonedMeshLOD = (MeshLOD*)Gods98::Mem_Alloc(count * sizeof(MeshLOD));

	uint32 i = 0;
	for (MeshLOD* m = srcMeshLOD; m != nullptr; m = m->next) {
		clonedMeshLOD[i] = *m; // Memcpy

		clonedMeshLOD[i].contMeshTarget = nullptr;

		// This is an array of contiguous memory, we only want to free the start of an array.
		// The first item in the array will have this flag set after the loop.
		clonedMeshLOD[i].flags &= ~MeshLODFlags::MESHLOD_FLAG_MEMBLOCK;
		clonedMeshLOD[i].flags |= MeshLODFlags::MESHLOD_FLAG_CLONED;

		// No memory access issues since the pointer is not resolved.
		// The last `next` field will be cleared after the loop.
		// NOTE: DO NOT use `++i` in this expression, as compilers can never agree how to handle it.
		clonedMeshLOD[i].next = &clonedMeshLOD[i + 1];

		i++;
	}
	clonedMeshLOD[0].flags |= MeshLODFlags::MESHLOD_FLAG_MEMBLOCK;
	clonedMeshLOD[i - 1].next = nullptr;
	return clonedMeshLOD;
}

// <LegoRR.exe @00451e80>
void __cdecl LegoRR::MeshLOD_SwapTarget(MeshLOD* meshLOD, Gods98::Container* contActTarget, bool32 restore, uint32 setID)
{
	for (MeshLOD* m = meshLOD; m != nullptr; m = m->next) {
		if (m->setID == setID) {
			if (m->contMeshTarget == nullptr) {
				const char* name = Gods98::Container_FormatPartName(contActTarget, meshLOD->partName, nullptr);
				m->contMeshTarget = Container_SearchTree(contActTarget, name, Gods98::Container_SearchMode::FirstMatch, nullptr);
			}

			if (m->contMeshTarget != nullptr) {
				Gods98::Container_Mesh_Swap(m->contMeshTarget, m->contMeshOrigin, restore);
			}
		}
	}
}

// <LegoRR.exe @00451ef0>
void __cdecl LegoRR::MeshLOD_RemoveTargets(MeshLOD* meshLOD)
{
	for (MeshLOD* m = meshLOD; m != nullptr; m = m->next) {
		m->contMeshTarget = nullptr;
	}
}

// <LegoRR.exe @00451f10>
void __cdecl LegoRR::MeshLOD_Free(MeshLOD* meshLOD)
{
	//MeshLOD* nextAlloc = nullptr;
	MeshLOD* prevAlloc = nullptr; // Track the last MeshLOD that is the start of a memory block to free.

	for (MeshLOD* m = meshLOD; m != nullptr; m = m->next) {
		// Check if we should free partName and contMeshOrigin.
		if (!(m->flags & MeshLODFlags::MESHLOD_FLAG_CLONED)) {

			if (m->partName) Gods98::Mem_Free(m->partName);

			if (m->contMeshOrigin) Gods98::Container_Remove(m->contMeshOrigin);
		}

		// Check how to handle freeing the memory this MeshLOD item.
		if (!(m->flags & MeshLODFlags::MESHLOD_FLAG_MEMBLOCK)) {
			// Splice out items that are not the start of a memory block.
			if (prevAlloc != nullptr) prevAlloc->next = m->next;
		}
		else {
			prevAlloc = m;
		}
	}

	// prevAlloc is non-null when at least one MeshLOD is the start of a memory block.
	if (prevAlloc != nullptr) {
		for (MeshLOD* m = meshLOD; m != nullptr; m = m->next) {
			Gods98::Mem_Free(m);
		}
	}
}

#pragma endregion
