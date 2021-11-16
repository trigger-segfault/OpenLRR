// Water.cpp : 
//

#include "../../engine/core/Errors.h"
#include "../../engine/core/Maths.h"
#include "../../engine/core/Memory.h"

#include "../effects/Effects.h"
#include "../object/Stats.h"
#include "../object/Weapons.h"

#include "Map3D.h"

#include "Water.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @0054a520>
LegoRR::Water_Globs & LegoRR::waterGlobs = *(LegoRR::Water_Globs*)0x0054a520;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions


// <LegoRR.exe @0046de50>
void __cdecl LegoRR::Water_Initialise(Gods98::Container* contRoot, Lego_Level* level)
{
	waterGlobs.entryCount = 0;
	waterGlobs.pairCount  = 0;
	// Assigned, but never used, level->DigDepth is always used instead
	waterGlobs.digDepth = level->DigDepth;

	for (uint32 y = 0; y < level->height; y++) {
		uint32 water_xMax = 0;
		uint32 water_xAlign = 0;

		for (uint32 x = 0; x < level->width; x++) {
			if (level->blocks[level->width * y + x].terrain == Lego_SurfaceType_Water /*7*/) {
				if (water_xAlign == water_xMax) {
					water_xAlign = x;
				}
				water_xMax = x + 1;
			}
			else {
				if (water_xAlign != water_xMax) {
					WaterEntry* entry = Water_InitSearchAddPairs(y, water_xAlign, water_xMax);
					if (entry == nullptr) {
						Water_InitAddPointFirst(y, water_xAlign, water_xMax);
					}
					else {
						Water_InitAddPoint(entry, y, water_xAlign, water_xMax);
					}
				}
				water_xMax = 0;
				water_xAlign = 0;
			}
		}
	}

	for (uint32 i = 0; i < waterGlobs.pairCount; i++) {
		WaterEntry* second = waterGlobs.pairTable[i].second;
		WaterEntry* first  = waterGlobs.pairTable[i].first;

		if (second->pointCount != 0) {
			// some form of overlapped copying...?
			std::memcpy(&first->points[first->pointCount], &second->points[0], sizeof(Point2F) * second->pointCount);

			first->pointCount += second->pointCount;
			second->pointCount = 0;
		}
	}

	// Sort by pointCount, highest to lowest
	std::qsort(&waterGlobs.entryTable, waterGlobs.entryCount, 0x428, Water_QsortCompare);

	// Then cap entryCount to number of entries with non-zero pointCount's
	for (uint32 i = 0; i < waterGlobs.entryCount; i++) {
		if (waterGlobs.entryTable[i].pointCount == 0) {
			waterGlobs.entryCount = i;
		}
	}

	Water_InitGroups(contRoot, level);
	Water_InitVertices(contRoot, level);
}

// <LegoRR.exe @0046dfd0>
void __cdecl LegoRR::Water_InitGroups(Gods98::Container* contRoot, Lego_Level* level)
{
	const Point2F DIRECTIONS[4] = {
		{  0.0f, -1.0f },
		{  1.0f,  0.0f },
		{  0.0f,  1.0f },
		{ -1.0f,  0.0f },
	};

	for (uint32 i = 0; i < waterGlobs.entryCount; i++) {
		WaterEntry* entry = &waterGlobs.entryTable[i];

		entry->groupCount = 0;

		for (uint32 j = 0; j < entry->pointCount; j++) {

			for (sint32 k = 0; k < 4; k++) {
				// lVar7 = __ftol((float10)dir->x + (float10)local_30->x);
				// x = (int)lVar7;
				// lVar7 = __ftol((float10)local_30->y + (float10)dir->y);
				// y = (int)lVar7;
				sint32 bx = (sint32)(entry->points[j].x + DIRECTIONS[k].x);
				sint32 by = (sint32)(entry->points[j].y + DIRECTIONS[k].y);
				sint32 idx = level->width * by + bx;

				if ((bx >= 0 && bx < (sint32)level->width) && (by >= 0 && by < (sint32)level->height) &&
					(level->blocks[idx].terrain != Lego_SurfaceType_Water /*0x7*/) &&
					(level->blocks[idx].terrain != Lego_SurfaceType_Immovable /*0x1*/))
				{
					entry->groups[entry->groupCount].index = j; // point index
					entry->groups[entry->groupCount].direction = (Direction)k; // direction index
					entry->groupCount++;
					break;
				}
			}
		}
	}
}

// <LegoRR.exe @0046e140>
void __cdecl LegoRR::Water_InitVertices(Gods98::Container* contRoot, Lego_Level* level)
{
	Vector3F vertPoses[4];
	Vertex vertices[4];

	const Point2F DIRECTIONS[4] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
	};
	const uint32 FACEDATA[6] = {
		0, 1, 3, 1, 2, 3,
	};
	const real32 TU_COORDS[4] = {
		0.0f, 1.0f, 1.0f, 0.0f,
	};
	const real32 TV_COORDS[4] = {
		0.0f, 0.0f, 1.0f, 1.0f,
	};

	for (uint32 i = 0; i < waterGlobs.entryCount; i++) {
		WaterEntry* entry = &waterGlobs.entryTable[i];

		entry->dig_z_418 = -10000.0f;

		for (uint32 j = 0; j < entry->pointCount; j++) {
			uint32 bx = (uint32)entry->points[j].x;
			uint32 by = (uint32)entry->points[j].y;
			Map3D_GetBlockVertexPositions(level->map, bx, by, vertPoses);

			for (uint32 k = 0; k < 4; k++) {
				uint32 bxOff = (uint32)(entry->points[j].x + DIRECTIONS[j].x);
				uint32 byOff = (uint32)(entry->points[j].y + DIRECTIONS[j].y);
				uint32 idxOff = level->width * byOff + bxOff;

				if (level->blocks[idxOff].predug == Lego_PredugType_Wall) {
					if (vertPoses[k].z > entry->dig_z_418) {
						entry->dig_z_418 = vertPoses[k].z;
					}
				}
				else {
					entry->flags |= WATER_FLAG_HIDDEN_UNK /*0x1*/;
				}
			}
		}

		entry->dig_z2_41c = entry->dig_z_418;

		entry->contMeshTrans = Gods98::Container_MakeMesh2(contRoot, Gods98::Container_MeshType::Transparent);

		/// NOTE: Original operation used negate on flags before comparing, `~entry->flags & WATER_FLAG_HIDDEN_UNK`.
		///       This would check for "any" flag being set, but `WATER_FLAG_HIDDEN_UNK` is just a single flag `0x1`.
		Gods98::Container_Hide(entry->contMeshTrans, !(entry->flags & WATER_FLAG_HIDDEN_UNK /*0x1*/));
		//Gods98::Container_Hide(entry->contMeshTrans, ~entry->flags & WATER_FLAG_HIDDEN_UNK /*0x1*/);


		for (uint32 j = 0; j < entry->pointCount; j++) {
			uint32 groupID = Gods98::Container_Mesh_AddGroup(entry->contMeshTrans, 4, 2, 3, FACEDATA);
			Gods98::Container_Mesh_SetColourAlpha(entry->contMeshTrans, groupID, 0.0f, 0.3f, 0.8f, 0.6f);

			uint32 bx = (uint32)entry->points[j].x;
			uint32 by = (uint32)entry->points[j].y;
			Map3D_GetBlockVertexPositions(level->map, bx, by, vertPoses);

			for (uint32 k = 0; k < 4; k++) {
				vertices[k].position.x = vertPoses[k].x;
				vertices[k].position.y = vertPoses[k].y;
				vertices[k].position.z = entry->dig_z_418;
				vertices[k].normal.x =  0.0f;
				vertices[k].normal.y =  0.0f;
				vertices[k].normal.z = -1.0f;
				vertices[k].tu = TU_COORDS[k];
				vertices[k].tv = TV_COORDS[k];
				vertices[k].colour = 0;
			}

			Gods98::Container_Mesh_SetVertices(entry->contMeshTrans, j, 0, 4, vertices);
		}
	}
}

// <LegoRR.exe @0046e480>
void __cdecl LegoRR::Water_Debug_LogContainerMesh(bool32 logWithValues)
{
	for (uint32 i = 0; i < waterGlobs.entryCount; i++) {
		real32 arg1, arg2;
		if (!logWithValues) {
			/* contMeshTrans (Container*), 0, 0.0f, 0.0f */
			arg1 = 0.0f;
			arg2 = 0.0f;
		}
		else {
			/* contMeshTrans (Container*), 0, 0.12f, 0.32f */
			arg1 = 0.12f;
			arg2 = 0.32f;
		}

		// Whatever this function originally was, it doesn't truly exist in LegoRR.
		// Beta builds will need to be checked.

		//logf_removed(waterGlobs.entryTable[i].contMeshTrans, 0, uVar2, uVar3);
	}
}

// <LegoRR.exe @0046e4e0>
void __cdecl LegoRR::Water_Block_DestroyWallComplete(Lego_Level* level, uint32 bx, uint32 by)
{
	Vector3F vertPoses[4];

	// I don't think this is actually a Point2I type, just a pair of ints.
	/*const Point2I VERT_OFFSETS[4] = {
		{ 0, 1 },
		{ 1, 2 },
		{ 2, 3 },
		{ 3, 0 },
	};*/
	const uint32 VERT_OFFSETS[4][2] = {
		{ 0, 1 },
		{ 1, 2 },
		{ 2, 3 },
		{ 3, 0 },
	};

	uint32 index;
	WaterEntry* entry = Water_Block_SubDestroyWall_AndDebug_WKey(bx, by, &index);
	if (entry != nullptr) {
		entry->flags &= ~WATER_FLAG_STATE_B /*~0x4*/;
		entry->flags |= WATER_FLAG_HIDDEN_UNK /*0x1*/;

		Gods98::Container_Hide(entry->contMeshTrans, false);

		WaterEntryGroup* group = &entry->groups[index];
		group->bool_14 = true;
		group->elapsedUp_c = 0.0f;
		group->elapsedDown_10 = 0.0f;

		uint32 by = (uint32)entry->points[group->index].y;
		uint32 bx = (uint32)entry->points[group->index].x;
		Map3D_GetBlockVertexPositions(level->map, bx, by, vertPoses);

		group->float_maxDirZ_8 = std::max(vertPoses[VERT_OFFSETS[group->direction][0]].z,
										  vertPoses[VERT_OFFSETS[group->direction][1]].z);
	}
}

// <LegoRR.exe @0046e5f0>
void __cdecl LegoRR::Water_Block_Debug_WKey(uint32 bx, uint32 by)
{
	uint32 groupIndex;
	WaterEntry* entry = Water_Block_SubDestroyWall_AndDebug_WKey(bx, by, &groupIndex);

	if (entry != nullptr) {
		entry->flags &= ~WATER_FLAG_STATE_A /*~0x2*/;
		entry->flags |= WATER_FLAG_HIDDEN_UNK /*0x1*/;

		Gods98::Container_Hide(entry->contMeshTrans, false);
		entry->groups[groupIndex].bool_14 = false;
	}
}

// <LegoRR.exe @0046e650>
void __cdecl LegoRR::Water_Update(Lego_Level* level, real32 elapsedGame)
{
	uint32 unkCount = 0;

	Vertex vertices[4];

	for (uint32 i = 0; i < waterGlobs.entryCount; i++) {
		WaterEntry* entry = &waterGlobs.entryTable[i];

		bool bState_A = false;
		// this isn't a mistake, flags alternate, so 0x4 only works if 0x2 is false
		bool bState_B = !(entry->flags & WATER_FLAG_STATE_A /*0x2*/);
		real32 dig_z2 = entry->dig_z_418;

		for (uint32 j = 0; j < entry->groupCount; j++) {
			WaterEntryGroup* group = &entry->groups[j];

			bool bElapseFloat10 = false; // if true, update float_10 elapsed time
			if (!group->bool_14 || group->float_maxDirZ_8 < entry->dig_z2_41c) {
				// ((ushort)((ushort)((real32)puVar9[2] < *pfVar6) << 8 |
				//         (ushort)((real32)puVar9[2] == *pfVar6) << 0xe) == 0)) {
			//LAB_0046e6eb:
				bElapseFloat10 = true;
			}
			else {
				if (dig_z2 < group->float_maxDirZ_8)
					dig_z2 = group->float_maxDirZ_8;

				unkCount += 1;
				bState_B = false;
				bState_A = true;
				if (group->float_maxDirZ_8 == entry->dig_z2_41c)
					bElapseFloat10 = true;
			}

			group->elapsedUp_c += elapsedGame * 1.5f;
			if (bElapseFloat10)
				group->elapsedDown_10 += elapsedGame * 0.5f;

			if (group->elapsedUp_c  > 240.0f)
				group->elapsedUp_c  = 240.0f;

			if (group->elapsedDown_10 > 240.0f)
				group->elapsedDown_10 = 240.0f;

		}

		if (entry->flags & WATER_FLAG_STATE_B /*0x4*/)
			bState_A = false;


		if (bState_A || bState_B) {

			real32 dig_diff = (entry->dig_z2_41c - entry->dig_z_418) / level->DigDepth;

			if (bState_A) {
				entry->flags &= ~WATER_FLAG_STATE_A /*~0x2*/;
				entry->dig_z2_41c += ((real32)unkCount * elapsedGame * 0.3f) / (dig_diff + 1.0f);
				if (dig_z2 <= entry->dig_z2_41c) {
					entry->dig_z2_41c = dig_z2;
					entry->flags |= WATER_FLAG_STATE_B /*0x4*/;
				}
			}
			else {
				entry->flags &= ~WATER_FLAG_STATE_B /*~0x4*/;
				entry->dig_z2_41c -= elapsedGame * 0.05f;
				if (dig_z2 >= entry->dig_z2_41c) {
					entry->dig_z2_41c = dig_z2;
					entry->flags |= WATER_FLAG_STATE_A /*0x2*/;
				}
			}
			// rgb(  0, 76,204) + alpha change
			Gods98::Container_SetColourAlpha(entry->contMeshTrans, 0.0f, 0.3f, 0.8f, 0.6f - dig_diff * 0.4f);

			for (uint32 j = 0; j < entry->pointCount; j++) {
				//Point2F_t *pPoint = &entry->points[j]; (not used, just the count)

				Gods98::Container_Mesh_GetVertices(entry->contMeshTrans, j /*groupID*/, 0, 4, vertices);

				// copy to vertices Z positions
				for (uint32 k = 0; k < 4; k++) {
					vertices[k].position.z = entry->dig_z2_41c;
				}
				Gods98::Container_Mesh_SetVertices(entry->contMeshTrans, j /*groupID*/, 0, 4, vertices);
			}
		}

	}
	Water_UpdateMap3DBlocks(level);
}

// <LegoRR.exe @0046e8d0>
void __cdecl LegoRR::Water_UpdateMap3DBlocks(Lego_Level* level)
{
	Point2F worldPos;
	Vector3F vertPoses[4];

	const Vector3F DIRECTIONS[4] = {
		{  0.0f,  1.0f,  0.0f },
		{  1.0f,  0.0f,  0.0f },
		{  0.0f, -1.0f,  0.0f },
		{ -1.0f,  0.0f,  0.0f },
	};

	const uint32 VERT_OFFSETS[4][2] = {
	  { 0, 1 },
	  { 1, 2 },
	  { 2, 3 },
	  { 3, 0 },
	};

	for (uint32 i = 0; i < waterGlobs.entryCount; i++) {
		WaterEntry* entry = &waterGlobs.entryTable[i];
		for (uint32 j = 0; j < entry->groupCount; j++) {
			WaterEntryGroup* group = &entry->groups[j];

			uint32 by = (uint32)entry->points[group->index].y;
			uint32 bx = (uint32)entry->points[group->index].x;
			Map3D_GetBlockVertexPositions(level->map, bx, by, vertPoses);

			Direction dir = group->direction;
			real32 dirMult = group->elapsedUp_c;
			worldPos.x = DIRECTIONS[dir].x * dirMult + (vertPoses[VERT_OFFSETS[dir][0]].x + vertPoses[VERT_OFFSETS[dir][1]].x) / 2.0f;
			worldPos.y = DIRECTIONS[dir].y * dirMult + (vertPoses[VERT_OFFSETS[dir][0]].y + vertPoses[VERT_OFFSETS[dir][1]].y) / 2.0f;
			Map3D_WorldToBlockPos_NoZ(level->map, worldPos.x, worldPos.y, (sint32*)&bx, (sint32*)&by);
			Level_Block_SetFlags1_200_AndUpdateSurface_LevelStruct428(level, bx, by, true);

			by = (uint32)entry->points[group->index].y;
			bx = (uint32)entry->points[group->index].x;
			Map3D_GetBlockVertexPositions(level->map, bx, by, vertPoses);

			dir = group->direction;
			dirMult = group->elapsedDown_10;
			worldPos.x = DIRECTIONS[dir].x * dirMult + (vertPoses[VERT_OFFSETS[dir][0]].x + vertPoses[VERT_OFFSETS[dir][1]].x) / 2.0f;
			worldPos.y = DIRECTIONS[dir].y * dirMult + (vertPoses[VERT_OFFSETS[dir][0]].y + vertPoses[VERT_OFFSETS[dir][1]].y) / 2.0f;
			Map3D_WorldToBlockPos_NoZ(level->map, worldPos.x, worldPos.y, (sint32*)&bx, (sint32*)&by);
			Level_Block_SetFlags1_200_AndUpdateSurface_LevelStruct428(level, bx, by, false);
		}
	}
}

// <LegoRR.exe @0046eb60>
LegoRR::WaterEntry* __cdecl LegoRR::Water_Block_SubDestroyWall_AndDebug_WKey(uint32 bx, uint32 by, OPTIONAL OUT uint32* groupIndex)
{
	if (waterGlobs.entryCount == 0)
		return nullptr;

	const Point2F DIRECTIONS[4] = {
		{  0.0f, -1.0f },
		{  1.0f,  0.0f },
		{  0.0f,  1.0f },
		{ -1.0f,  0.0f },
	};

	for (uint32 i = 0; i < waterGlobs.entryCount; i++) {
		WaterEntry* entry = &waterGlobs.entryTable[i];
		for (uint32 j = 0; j < entry->groupCount; j++) {
			WaterEntryGroup* group = &entry->groups[j];

			if ((bx == (uint32)(entry->points[group->index].x + DIRECTIONS[group->direction].x)) &&
				(by == (uint32)(entry->points[group->index].y + DIRECTIONS[group->direction].y)))
			{
				if (groupIndex) *groupIndex = j;
				
				return entry;
			}
		}
	}
	return nullptr;
}

// <LegoRR.exe @0046ec60>
sint32 __cdecl LegoRR::Water_QsortCompare(const void* a, const void* b)
{
	const WaterEntry* wa = (const WaterEntry*)a;
	const WaterEntry* wb = (const WaterEntry*)b;
	if (wa->pointCount > wb->pointCount)
		return -1;
	if (wa->pointCount < wb->pointCount)
		return 1;
	return 0;
}

// <LegoRR.exe @0046ec90>
LegoRR::WaterEntry* __cdecl LegoRR::Water_InitSearchAddPairs(sint32 y, uint32 xAlign, uint32 xMax)
{
	if (y == 0)
		return nullptr;

	WaterEntry* found = nullptr;
	for (uint32 x = xAlign; x < xMax; x++) {

		for (uint32 i = 0; i < waterGlobs.entryCount; i++) {
			WaterEntry* entry = &waterGlobs.entryTable[i];
			WaterEntry* foundLast = found;

			for (uint32 j = 0; j < entry->pointCount; j++) {

				if ((real32)x == entry->points[j].x && (real32)(uint32)(y - 1) == entry->points[j].y) {
					found = (foundLast != nullptr ? foundLast : entry);
					if (foundLast != entry) {
						waterGlobs.pairTable[waterGlobs.pairCount].first = foundLast;
						waterGlobs.pairTable[waterGlobs.pairCount].second = entry;
						waterGlobs.pairCount++;
					}
				}
				foundLast = found;
			}
		}
	}
	return found;
}

// <LegoRR.exe @0046ed90>
void __cdecl LegoRR::Water_InitAddPoint(WaterEntry* entry, uint32 y, uint32 xAlign, uint32 xMax)
{
	for (uint32 x = xAlign; x < xMax; x++) {
		entry->points[entry->pointCount].x = (real32)x;
		entry->points[entry->pointCount].y = (real32)y;

		entry->pointCount++;
	}
}

// <LegoRR.exe @0046edf0>
void __cdecl LegoRR::Water_InitAddPointFirst(uint32 y, uint32 xAlign, uint32 xMax)
{
	WaterEntry* entry = &waterGlobs.entryTable[waterGlobs.entryCount];
	entry->pointCount = 0;
	Water_InitAddPoint(entry, y, xAlign, xMax);

	waterGlobs.entryCount++;
}

#pragma endregion
