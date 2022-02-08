// Smoke.cpp : 
//

#include "../../engine/core/Errors.h"
#include "../../engine/core/Maths.h"
#include "../../engine/core/Memory.h"
#include "../../engine/gfx/Containers.h"
#include "../../engine/gfx/Mesh.h"
#include "../../engine/gfx/Viewports.h"

#include "../Game.h"
#include "../audio/SFX.h"
#include "../world/Map3D.h"

#include "Smoke.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00553100>
LegoRR::Smoke_Globs & LegoRR::smokeGlobs = *(LegoRR::Smoke_Globs*)0x00553100;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00465640>
bool32 __cdecl LegoRR::Smoke_Initialise(void)
{
	smokeGlobs.firstSmoke = nullptr;
	smokeGlobs.lastSmoke  = nullptr;
	smokeGlobs.textureCount = 0;
	return true;
}

// <LegoRR.exe @00465660>
void __cdecl LegoRR::Smoke_LoadTextures(const char* dirname, const char* basename, uint32 textureCount)
{
	Error_Fatal(textureCount > _countof(smokeGlobs.textureList), "Smoke_LoadTextures: Too many textures for textureCount");
	
	char buffBmp[260];
	char buffDir[260];
	
	smokeGlobs.textureCount = textureCount;
	for (uint32 i = 0; i < textureCount; i++) {
		// Can be refactored into a single sprintf statement.
		std::sprintf(buffDir, "%s\\", dirname); // directory
		std::sprintf(buffBmp, "%s%i.bmp", basename, i); // directory+filename

		smokeGlobs.textureList[i] = Gods98::Mesh_LoadTexture(buffDir, buffBmp, nullptr, nullptr);
	}
}


// thickness: a range of (0.0f,1.0f], which simply determines the number of smoke groups (clouds?).
//   0 is invalid. 0.1 -> 1 cloud, 0.9 -> 9 clouds, 1.0 -> 10 clouds (max).
// Valid Smoke_Type modes: 0, 1, 2
// 
// <LegoRR.exe @004656f0>
LegoRR::Smoke* __cdecl LegoRR::Smoke_CreateSmokeArea(OPTIONAL IN Gods98::Container* attachedCont, uint32 bx, uint32 by, const Vector3F* dirVec, real32 r, real32 g, real32 b, real32 thickness, real32 animSpeed, Smoke_Type smokeType, uint32 randomness, SFX_ID sfxID)
{
	const uint32 faceData[6] = {
		0, 3, 2, 2, 1, 0,
	};


	Smoke* smoke = (Smoke*)Gods98::Mem_Alloc(sizeof(Smoke));
	std::memset(smoke, 0, sizeof(Smoke));

	if (smokeType == Smoke_Type_Looping || smokeType == Smoke_Type_Dissipate) {
		Map3D* map = Lego_GetMap();
		
		Vector3F worldPos; // local_24
		Map3D_BlockToWorldPos(map, bx, by, &worldPos.x, &worldPos.y);
		
		const uint32 blockSizeUnits = (uint32)(sint32)(map->BlockSize * 0.6f); // __ftol
		const real32 fVar2 = (real32)blockSizeUnits * 0.5f;

		// Arbitrary randomness numbers: 4-digits of arbitraryness! :D
		worldPos.x += ((real32)((randomness % 4254) % blockSizeUnits) - fVar2);
		worldPos.y += ((real32)((randomness % 1515) % blockSizeUnits) - fVar2);
		worldPos.z = Map3D_GetWorldZ(map, worldPos.x, worldPos.y);

		smoke->cont = Gods98::Container_Create(Gods98::Container_GetRoot());
		/// MACRO: Container_SetWorldPosition => Container_SetPosition
		Gods98::Container_SetWorldPosition(smoke->cont, worldPos.x, worldPos.y, worldPos.z);

		// We've created our own container, and need to manage disposing of it.
		smoke->flags = SMOKE_FLAG_OWNCONTAINER; // Mode 0,2
		
		// SFX setup:
		smoke->sfxOrigin = worldPos;

		// 1-in-4 chance for SFX: `(randomness % 4) == 0`.
		if ((randomness & 0x3) == 0) {
			smoke->sfxID = sfxID;
		}
	}
	else if (smokeType == Smoke_Type_Attach) {
		smoke->cont = attachedCont; // User has passed in an existing container for Smoke to use.
		smoke->flags = SMOKE_FLAG_BROKEN_UNK_4; // Mode 1
	}
	else {
		Error_Fatal(true, "Smoke_CreateSmokeArea: smokeType must be of value 0, 1, or 2");
	}

	smoke->dirVec = *dirVec;
	Gods98::Maths_Vector3DNormalize(&smoke->dirVec);


	smoke->colour.red   = r;
	smoke->colour.green = g;
	smoke->colour.blue  = b;


	if (smokeType == Smoke_Type_Dissipate) {
		smoke->groupCount = 4;
		smoke->numRepeats = 0;
		smoke->flags |= SMOKE_FLAG_REPEATCOUNT; // Mode 2
		smoke->animSpeed = 2.0f;
	}
	else {
		/// FIX APPLY: Change to && comparison for proper range check.
		if (thickness > 0.0f && thickness < 1.0f) {
			smoke->groupCount = (uint32)(sint32)(thickness * 10.0f); // __ftol
		}
		else { // Invalid argument (or maybe zero is treated as default). Use max groupCount.
			smoke->groupCount = 10;
		}
		smoke->animSpeed = animSpeed;
	}


	/// MACRO: Mesh_Create => Mesh_CreateOnFrame
	Gods98::Mesh_RenderFlags renderFlags = (Gods98::Mesh_RenderFlags::MESH_TRANSFORM_FLAG_IDENTITY|Gods98::Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHASA1) /*0x4800*/;
	smoke->mesh = Gods98::Mesh_Create(smoke->cont, Smoke_MeshRenderCallback, renderFlags, smoke);

	for (uint32 i = 0; i < smoke->groupCount; i++) {
		
		uint32 groupID = Gods98::Mesh_AddGroup(smoke->mesh, 4, 2, 3, faceData);
		Smoke_Group* sgroup = &smoke->groupList[groupID];
		Gods98::Mesh_SetGroupTexture(smoke->mesh, groupID, smokeGlobs.textureList[i % smokeGlobs.textureCount]);
		
		/// MACRO: Mesh_SetGroupDiffuse  => Mesh_SetGroupColour
		///        Mesh_SetGroupEmissive => Mesh_SetGroupColour
		Gods98::Mesh_SetGroupDiffuse(smoke->mesh, groupID, r, g, b);
		Gods98::Mesh_SetGroupEmissive(smoke->mesh, groupID, r * 0.2f, g * 0.2f, b * 0.2f);
		Gods98::Mesh_HideGroup(smoke->mesh, groupID, true);


		sint32 rng1 = std::rand();
		sint32 rng2 = std::rand();
		

		/// ORIGINAL BITFIDDLE MATH:
		// preserve-sign modulus:
		//   -5=-1, -4=0, -3=-3, -2=-2, -1=-1, 0=0, 1=1, 2=2, 3=3, 4=0, 5=1, ...
		// Not that it matters... since std::rand always returns a value in range [0,0x7fff] for LRR.
		// Signed right shift will take the MSB 1's with it: (-1 >> 31 == -1), (-2 >> 31 == -1)
		//  Basically, if negative, then we have a full uint32 mask: 0xffffffff, otherwise 0.
		/*const uint32 sxmsb1 = (sint32)rng1 >> 31;
		const uint32 sxmsb2 = (sint32)rng2 >> 31;
		rng1 = (((rng1 ^ sxmsb1) - sxmsb1 & 3 ^ sxmsb1) - sxmsb1);
		rng2 = (((rng2 ^ sxmsb2) - sxmsb2 & 3 ^ sxmsb2) - sxmsb2);*/


		/// CLEANED-UP "READABLE" MATH: (even though it's impossible to get a negative return)
		// Preserve-sign modulus 4: (abs(rng) % 4) * sign(rng)
		rng1 = (std::abs(rng1) & 0x3) * (rng1>=0 ? 1 : -1);
		rng2 = (std::abs(rng2) & 0x3) * (rng2>=0 ? 1 : -1);

		
		// These fields have to do with scaling, and float_2c also affects newSmokeTimer... it's awkward...
		sgroup->float_28 = (real32)rng1 + 6.0f; // [6,9]
		sgroup->float_2c = (real32)rng2 + sgroup->float_28 + 2.0f; // [8,14] ([6,9] + [2,5])

		if (!(smoke->flags & SMOKE_FLAG_REPEATCOUNT)) {
			sgroup->float_28 *= 0.5; // sgroup->float_28 /= 2.0f;
		}
		else {
			sgroup->float_2c *= 2.0f; // sgroup->float_2c += sgroup->float_2c;
		}
	}

	smoke->newSmokeTimer = 0.0f;

	if (smokeType != Smoke_Type_Dissipate) {
		// We don't create all smoke groups at once, but only as the newSmokeTimer counts down to zero.
		// During the creation/update process, the timer may count-down enough (but really can it?)...
		//  so we loop through the max possible groups and try to add new smoke.
		// Each loop also updates ALL smoke groups, so this may be some form of initial state for the smoke.
		// 
		// Like, you enter a level and it would be weird if the smoke rising effect
		//  just started from nothing when its a constant behaviour.
		for (uint32 i = 0; i < 10; i++) {

			/// WARNING: Smoke_Group_Show changes `smoke->newSmokeTimer`. So we can't be assured that
			//           This condition will always succeed.
			// Condition always true because of the above `smoke->newSmokeTimer = 0.0f;` assignment.
			//  And the below `smoke->newSmokeTimer -= 5.0f;` compound assignment.
			if (smoke->newSmokeTimer <= 0.0f) {

				for (uint32 groupID = 0; groupID < smoke->groupCount; groupID++) {
					if (!smoke->groupList[groupID].used) {
						Smoke_Group_Show(smoke, groupID);
						break; // Unused group found, use this for the outer `FOR 0-9` loop.
					}
				}
			}
			
			// Some kind of change to counteract Smoke_Group_Show or Smoke_Group_Update(?)
			smoke->newSmokeTimer -= 5.0f;

			for (uint32 groupID = 0; groupID < smoke->groupCount; groupID++) {
				if (smoke->groupList[groupID].used) {
					Smoke_Group_Update(smoke, groupID, 5.0f);
				}
			}
		}
	}

	// Insert smoke item into doubly-linked list.
	smoke->previous = smokeGlobs.lastSmoke;
	if (smokeGlobs.firstSmoke == nullptr) {
		smokeGlobs.firstSmoke = smoke;
	}
	else {
		smokeGlobs.lastSmoke->next = smoke;
	}
	smokeGlobs.lastSmoke = smoke;

	return smoke;
}

// <LegoRR.exe @00465c30>
void __cdecl LegoRR::Smoke_RemoveAll(void)
{
	Smoke* smoke = smokeGlobs.firstSmoke;
	while (smoke != nullptr) {
		Smoke* smokeNext = smoke->next; // No guarantee this isn't changed by `Smoke_Remove`
		Smoke_Remove(smoke, smoke->cont);
		smoke = smokeNext;
	}

	/// TODO: Is this FULLY handled by Smoke_Remove or not?
	///       And what about `smokeGlobs.lastSmoke`?
	smokeGlobs.firstSmoke = nullptr;
}

// <LegoRR.exe @00465c70>
void __cdecl LegoRR::Smoke_Remove(Smoke* smoke, Gods98::Container* attachedCont)
{
	if (!(smoke->flags & SMOKE_FLAG_OWNCONTAINER)) {
		// Remove mesh from the passed attached container.
		// NOTE: attachedCont is always passed as `smoke->cont`,
		//       (so its no different from just using smoke->cont here).

		/// MACRO: Mesh_Remove => Mesh_RemoveFromContainer
		Gods98::Mesh_RemoveFromContainer(smoke->mesh, attachedCont);
	}
	else {
		/// MACRO: Mesh_Remove => Mesh_RemoveFromContainer
		Gods98::Mesh_RemoveFromContainer(smoke->mesh, smoke->cont);
		
		Gods98::Container_Remove(smoke->cont);
	}


	/// FIX APPLY: Re-order logic so that SFX is always stopped.
	///  (previously there possibly would have been a 1/4 chance of lava bubbling SFX not stopping at the end of the level).

	// Splice our smoke item out of the doubly-linked list.
	if (smokeGlobs.lastSmoke == smoke && smoke->previous == nullptr) {
		// We're the only item in the doubly-linked list.
		smokeGlobs.lastSmoke = nullptr;
		smokeGlobs.firstSmoke = nullptr; // smokeGlobs.lastSmoke;
	}
	else {
		// Reconnect items in the doubly-linked list.
		if (smoke->next != nullptr) {
			// Reconnect next smoke item.
			smoke->next->previous = smoke->previous;
			if (smoke->next->previous == nullptr) {
				// We're the first item in the doubly-linked list.
				smokeGlobs.firstSmoke = smoke->next;
			}
		}
		if (smoke->previous != nullptr) {
			// Reconnect previous smoke item.
			smoke->previous->next = smoke->next;
			if (smokeGlobs.lastSmoke == smoke) {
				// We're the last item in the doubly-linked list.
				smokeGlobs.lastSmoke = smoke->previous;
			}
		}
	}

	/// OLD: Broken logic forgetting to stop SFX
	/*if (smokeGlobs.lastSmoke == smoke) {
		// We're the last smoke item in the doubly-linked list.
		smokeGlobs.lastSmoke = smoke->previous;
		if (smokeGlobs.lastSmoke == nullptr) {
			// We're the only item in the doubly-linked list.
			smokeGlobs.firstSmoke = nullptr; // smokeGlobs.lastSmoke;

			/// FIXME: SFX_Sound3D_StopSound(smoke->sound3DPlayHandle) isn't called during this return!!!
			Gods98::Mem_Free(smoke);
			return;
		}
	}

	if (smoke->next != nullptr) {
		// Reconnect next smoke item.
		smoke->next->previous = smoke->previous;
		if (smoke->next->previous == nullptr) {
			// We're the first smoke item in the doubly-linked list.
			smokeGlobs.firstSmoke = smoke->next;
		}
	}
	if (smoke->previous != nullptr) {
		// Reconnect previous smoke item.
		smoke->previous->next = smoke->next;
	}*/

	if (smoke->sound3DPlayHandle != 0) {
		SFX_Sound3D_StopSound(smoke->sound3DPlayHandle);
	}

	Gods98::Mem_Free(smoke);
}

// <LegoRR.exe @00465d50>
void __cdecl LegoRR::Smoke_HideAll(bool32 hide)
{
	Smoke* smoke = smokeGlobs.firstSmoke;
	while (smoke != nullptr) {
		// Note: Originally, unlike other functions iterating through linked list,
		//       this function one assigned smoke->next after calling Smoke_Hide.
		Smoke* smokeNext = smoke->next;
		Smoke_Hide(smoke, hide);
		smoke = smokeNext;
	}
}

// <LegoRR.exe @00465d80>
void __cdecl LegoRR::Smoke_Hide(Smoke* smoke, bool32 hide)
{
	Gods98::Mesh_Hide(smoke->mesh, hide);
	if (hide)
		smoke->flags |= SMOKE_FLAG_HIDDEN;
	else
		smoke->flags &= ~SMOKE_FLAG_HIDDEN;
}

// <LegoRR.exe @00465dc0>
void __cdecl LegoRR::Smoke_Update(real32 elapsedGame)
{
	Smoke* smoke = smokeGlobs.firstSmoke;
	while (smoke != nullptr) {
		Smoke* smokeNext = smoke->next;

		if (!(smoke->flags & SMOKE_FLAG_SFXFAILED) && smoke->sound3DPlayHandle == 0) {
			if (smoke->sfxID != SFX_NULL) {
				// Start looping sound, so this will only be called once per smoke until Smoke_Remove.
				smoke->sound3DPlayHandle = SFX_Container_Random_Play_OrInitSoundUnk(nullptr, smoke->sfxID, true, false, &smoke->sfxOrigin);
			}
			if (smoke->sound3DPlayHandle == 0) {
				// SFX failed to play, but we don't want to try playing it again.
				// This flag usage is really awkward, in that its ONLY used to
				//  prevent the outer if statement condition from succeeding.
				smoke->flags |= SMOKE_FLAG_SFXFAILED;
			}
		}

		if (!(smoke->flags & SMOKE_FLAG_HIDDEN)) {
			// If neither of the repeat flags.
			if (!(smoke->flags & (SMOKE_FLAG_REPEATCOUNT|SMOKE_FLAG_REPEATEND))) {

				if (smoke->newSmokeTimer <= 0.0f) {
					for (uint32 groupID = 0; groupID < smoke->groupCount; groupID++) {
						// Find an available smoke group to show.
						if (!smoke->groupList[groupID].used) {
							Smoke_Group_Show(smoke, groupID);
							break;
						}
					}
				}
				smoke->newSmokeTimer -= elapsedGame;
			}

			bool noGroupsUsed = true;

			for (uint32 groupID = 0; groupID < smoke->groupCount; groupID++) {
				if (smoke->groupList[groupID].used) {
					Smoke_Group_Update(smoke, groupID, elapsedGame);
					noGroupsUsed = false;
				}
			}

			// If both of the repeat flags.
			if (noGroupsUsed && (smoke->flags & SMOKE_FLAG_REPEATCOUNT) && (smoke->flags & SMOKE_FLAG_REPEATEND)) {
				Smoke_Remove(smoke, smoke->cont);
			}
		}


		smoke = smokeNext;
	}
}

// <LegoRR.exe @00465f10>
void __cdecl LegoRR::Smoke_Group_Show(Smoke* smoke, uint32 groupID)
{
	Smoke_Group* sgroup = &smoke->groupList[groupID];

	if (smoke->flags & SMOKE_FLAG_REPEATCOUNT) {
		//smoke->numRepeats++;
		if (++smoke->numRepeats == 4) { // Maximum repeat of 4 times.
			// NOTE: This flags operation has no effect because of the `smoke->flags = SMOKE_FLAG_BROKEN_UNK_4;` assignment halfway down the function.
			smoke->flags |= SMOKE_FLAG_REPEATEND;
		}
	}

	sgroup->used = true;
	Gods98::Mesh_HideGroup(smoke->mesh, groupID, false);

	
	Gods98::Maths_Vector3DRandom(&sgroup->randVec);
	// sgroup->randVec = { [ 0.00, 0.17], [ 0.00, 0.17], [-1.00,-0.83] };
	sgroup->randVec.x *= 0.17f;
	sgroup->randVec.y *= 0.17f;
	sgroup->randVec.z = (std::abs(sgroup->randVec.z) * -0.17f) - 0.83f;
	

	Gods98::Maths_Vector3DAdd(&sgroup->moveVec, &smoke->dirVec, &sgroup->randVec);

	const real32 moveSpeed = -smoke->animSpeed / sgroup->moveVec.z; // Use `sgroup->moveVec.z` as-is only after Maths_Vector3DAdd.
	Gods98::Maths_Vector3DScale(&sgroup->moveVec, &sgroup->moveVec, moveSpeed);

	/// FIXME: Was this meant to be an or-equals instead of an assignment???
	///        This breaks SMOKE_FLAG_REPEATEND functionality.
	smoke->flags = SMOKE_FLAG_BROKEN_UNK_4;
	
	/// MACRO: Container_GetWorldPosition => Container_GetPosition
	Gods98::Container_GetWorldPosition(smoke->cont, &sgroup->position);
	
	sgroup->timer_30 = 0.0f;
	sgroup->timerUp_38 = 0.0f;

	smoke->newSmokeTimer = (0.5f / smoke->animSpeed) * sgroup->float_2c;

	//sint32 rng = std::rand();
	sgroup->int_34 = 40 + (std::rand() % 30);

	// Void. Return is never used, and is merely a side effect of
	//  the IDIV instruction and EAX not being used until the end.
	//return rng / 30;
}

// <LegoRR.exe @004660c0>
void __cdecl LegoRR::Smoke_Group_Update(Smoke* smoke, uint32 groupID, real32 elapsedGame)
{
	Smoke_Group* sgroup = &smoke->groupList[groupID];

	// Update smoke position:
	Gods98::Maths_RayEndPoint(&sgroup->position, &sgroup->position, &sgroup->moveVec, elapsedGame);

	sgroup->timerUp_38 += std::abs(sgroup->moveVec.z * elapsedGame);
	if (sgroup->timerUp_38 >= (real32)sgroup->int_34) {
		sgroup->used = false;
		Gods98::Mesh_HideGroup(smoke->mesh, groupID, true);
	}
	else {
		// Fade away as smoke rises.
		sgroup->timer_30 += elapsedGame;
		/// MACRO: Mesh_SetGroupAlpha -> Mesh_SetGroupMaterialValues
		Gods98::Mesh_SetGroupAlpha(smoke->mesh, groupID, (0.7f - (sgroup->timerUp_38 / (real32)sgroup->int_34) * 0.7f));
	}
}

// <LegoRR.exe @004661a0>
void __cdecl LegoRR::Smoke_MeshRenderCallback(Gods98::Mesh* mesh, void* data, Gods98::Viewport* view)
{
	Vector3F lightPos;
	/// MACRO: Container_GetWorldPosition => Container_GetPosition
	Gods98::Container_GetWorldPosition(Lego_GetCurrentViewLight(), &lightPos);

	Smoke* smoke = (Smoke*)data;
	for (uint32 groupID = 0; groupID < smoke->groupCount; groupID++) {
		if (smoke->groupList[groupID].used) {
			Smoke_Group_MeshRenderCallback(smoke, groupID, view, &lightPos);
		}
	}
}

// <LegoRR.exe @00466200>
void __cdecl LegoRR::Smoke_Group_MeshRenderCallback(Smoke* smoke, uint32 groupID, Gods98::Viewport* view, const Vector3F* lightPos)
{
	Vector3F vertPoses[4];
	const Point2F textCoords[4] = {
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
	};

	const Smoke_Group* sgroup = &smoke->groupList[groupID];

	Vector3F viewDir, viewUp;
	/// MACRO: Container_GetWorldOrientation => Container_GetOrientation
	Gods98::Container_GetWorldOrientation(Gods98::Viewport_GetCamera(view), &viewDir, &viewUp);
	
	Vector3F perpendicular = { viewDir.y, -viewDir.x, 0.0f }; // Perpendicular of 2D vector.
	Vector3F local_80 = { 0.0f, 0.0f, -1.0f };

	// 0.3f and 0.7f are lazy radians(?)
	if (viewDir.z > 0.3f) {
		Gods98::Maths_Vector3DRotate(&local_80, &local_80, &perpendicular, 0.7f);
	}

	// Scale from 1.0f to 2.0f.
	const real32 baseScalar = (sgroup->timerUp_38 / (real32)sgroup->int_34) + 1.0f;
	const real32 scalar1 = sgroup->float_28 * baseScalar;
	const real32 scalar2 = sgroup->float_2c * baseScalar;

	Gods98::Maths_Vector3DScale(&perpendicular, &perpendicular, (sgroup->float_28 * baseScalar)); //scalar1);
	Gods98::Maths_Vector3DScale(&local_80, &local_80, (sgroup->float_2c * baseScalar)); //scalar2);

	Gods98::Maths_Vector3DAdd(     &vertPoses[0], &sgroup->position, &perpendicular);
	Gods98::Maths_Vector3DSubtract(&vertPoses[1], &sgroup->position, &perpendicular);
	
	Gods98::Maths_Vector3DSubtract(&vertPoses[2], &vertPoses[1], &local_80);
	Gods98::Maths_Vector3DSubtract(&vertPoses[3], &vertPoses[0], &local_80);

	Gods98::Mesh_SetVertices_PointNormalAt(smoke->mesh, groupID, 0, 4, vertPoses, lightPos,
										   reinterpret_cast<real32(*)[2]>(const_cast<Point2F*>(textCoords)));
}

#pragma endregion
