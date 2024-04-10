// SpiderWeb.cpp : 
//

#include "SpiderWeb.h"
#include "Map3D.h"
#include "../object/Object.h"
#include "../Game.h"
#include "../../engine/gfx/Containers.h"
#include "../../engine/core/Maths.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

 // <LegoRR.exe @005530e8>
LegoRR::SpiderWeb_Globs& LegoRR::spiderwebGlobs = *(LegoRR::SpiderWeb_Globs*)0x005530e8;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

 // <LegoRR.exe @00466480>
void __cdecl LegoRR::SpiderWeb_Initialise(Lego_Level* level)
{
	uint32 const gridSize = level->width * level->height * sizeof(SpiderWeb_Block);

	spiderwebGlobs.webBlocks = reinterpret_cast<SpiderWeb_Block *>(Gods98::Mem_Alloc(gridSize));
	if (spiderwebGlobs.webBlocks != nullptr)
	{
		std::memset(spiderwebGlobs.webBlocks, 0, gridSize);
	}

	spiderwebGlobs.level = level;
}

// <LegoRR.exe @004664d0>
void __cdecl LegoRR::SpiderWeb_Shutdown(void)
{
	if (spiderwebGlobs.webBlocks != nullptr)
	{
		Gods98::Mem_Free(spiderwebGlobs.webBlocks);
		spiderwebGlobs.webBlocks = nullptr;
	}
}

// <LegoRR.exe @004664f0>
void __cdecl LegoRR::SpiderWeb_Restart(Lego_Level* level)
{
	SpiderWeb_Shutdown();
	SpiderWeb_Initialise(level);
}

// <LegoRR.exe @00466510>
bool32 __cdecl LegoRR::SpiderWeb_SpawnAt(uint32 bx, uint32 by)
{
	// Check if there is already an web block active on this location, if so immediately return false
	SpiderWeb_Block& webBlock = spiderwebBlockValue(bx, by);
	if (hasFlagValue(webBlock.flags, SpiderWeb_BlockFlags::BLOCKWEB_FLAG_ACTIVE))
	{
		return FALSE;
	}

	// Get the angle of the spiderweb, if no success return false
	real32 theta {};
	if (!SpiderWeb_GetAngle(bx, by, &theta))
	{
		return FALSE;
	}

	// Get the world position
	real32 xPos {}, yPos {};
	Map3D_BlockToWorldPos(spiderwebGlobs.level->map, bx, by, &xPos, &yPos);

	// Create the spiderweb object and set its properties
	LegoObject* webObj = LegoObject_CreateInWorld(legoGlobs.contSpiderWeb, LegoObject_Type::LegoObject_SpiderWeb, static_cast<LegoObject_ID>(0), 0, xPos, yPos, theta);
	SpiderWeb_Add(bx, by, webObj);
	setFlagValue(webBlock.object->flags1, LiveFlags1::LIVEOBJ1_EXPANDING, true);
	Gods98::Container_SetActivity(webBlock.object->other, "Expand");
	Gods98::Container_SetAnimationTime(webBlock.object->other, 0.0f);
	return TRUE;
}

// <LegoRR.exe @00466640>
void __cdecl LegoRR::SpiderWeb_Add(sint32 bx, sint32 by, LegoObject* webObj)
{
	SpiderWeb_Block& webBlock = spiderwebBlockValue(bx, by);
	webBlock.object = webObj;
	setFlagValue(webBlock.flags, SpiderWeb_BlockFlags::BLOCKWEB_FLAG_ACTIVE, true);
	webBlock.health = 100.0;
}

// <LegoRR.exe @004666b0>
bool32 __cdecl LegoRR::SpiderWeb_GetAngle(sint32 bx, sint32 by, OUT real32* theta)
{
	// If the block is not floor, then immediatly return false
	if (!hasFlagValue(blockValue(spiderwebGlobs.level, bx, by).flags1, BlockFlags1::BLOCK1_FLOOR))
	{
		return FALSE;
	}

	// If there are walls in both x direction, return 90 degrees rotation (PI/2)
	if (hasFlagValue(blockValue(spiderwebGlobs.level, bx + 1, by).flags1, BlockFlags1::BLOCK1_WALL) &&
		hasFlagValue(blockValue(spiderwebGlobs.level, bx - 1, by).flags1, BlockFlags1::BLOCK1_WALL))
	{
		if (theta != nullptr)
		{
			*theta = M_PI/2;
		}
		return TRUE;
	}

	// If there are walls in both y direction, return 0 degrees rotation
	if (hasFlagValue(blockValue(spiderwebGlobs.level, bx, by + 1).flags1, BlockFlags1::BLOCK1_WALL) &&
		hasFlagValue(blockValue(spiderwebGlobs.level, bx, by - 1).flags1, BlockFlags1::BLOCK1_WALL))
	{
		if (theta != nullptr)
		{
			*theta = 0.0f;
		}
		return TRUE;
	}

	// No adjecent walls, so cannot determine angle
	return FALSE;
}

// <LegoRR.exe @00466750>
bool32 __cdecl LegoRR::SpiderWeb_CheckCollision(LegoObject* liveObj)
{
	// Get the block position if not possible return false immediately
	sint32 bx {}, by {};
	if (!LegoObject_GetBlockPos(liveObj, &bx, &by))
	{
		return FALSE;
	}

	// Get the webBlock and check if it is active
	SpiderWeb_Block& webBlock = spiderwebBlockValue(bx, by);
	if (!hasFlagValue(webBlock.flags, SpiderWeb_BlockFlags::BLOCKWEB_FLAG_ACTIVE))
	{
		return FALSE;
	}

	// Get the world position
	Point2F position {};
	Map3D_BlockToWorldPos(spiderwebGlobs.level->map, bx, by, &position.x, &position.y);

	// Get the faced direction
	Point2F direction {};
	LegoObject_GetFaceDirection(webBlock.object, &direction);

	// Check for collission and apply flags
	if (Weapon_LegoObject_Collision_FUN_00470520(liveObj, &position, &direction, FALSE))
	{
		setFlagValue(liveObj->flags1, LiveFlags1::LIVEOBJ1_CAUGHTINWEB, true);
		setFlagValue(webBlock.object->flags1, LiveFlags1::LIVEOBJ1_CAUGHTINWEB, true);
		return TRUE;
	}

	// No collision
	return FALSE;
}

// <LegoRR.exe @00466880>
bool32 __cdecl LegoRR::SpiderWeb_Update(real32 elapsedGame)
{
	return LegoObject_RunThroughListsSkipUpgradeParts(reinterpret_cast<LegoObject_RunThroughListsCallback>(SpiderWeb_LiveObjectCallback), &elapsedGame);
}

// <LegoRR.exe @004668a0>
bool32 __cdecl LegoRR::SpiderWeb_LiveObjectCallback(LegoObject* liveObj, real32* pElapsedGame)
{
	// Process SpiderWeb objects
	if (liveObj->type == LegoObject_Type::LegoObject_SpiderWeb)
	{
		// Remove active SpiderWebs that have no more walls next to them (SpiderWeb_GetAndle returns false)
		sint32 bx {}, by {};
		LegoObject_GetBlockPos(liveObj, &bx, &by);
		if ((spiderwebBlockValue(bx, by).flags & SpiderWeb_BlockFlags::BLOCKWEB_FLAG_ACTIVE)
			&& !SpiderWeb_GetAngle(bx, by, nullptr))
		{
			SpiderWeb_Remove(bx, by);
		}
		return FALSE;
	}

	// Process objects already caught in web
	if (hasFlagValue(liveObj->flags1, LiveFlags1::LIVEOBJ1_CAUGHTINWEB))
	{
		// If the SpiderWeb is active, we damage it
		sint32 bx {}, by {};
		LegoObject_GetBlockPos(liveObj, &bx, &by);
		SpiderWeb_Block& webBlock = spiderwebBlockValue(bx, by);
		if (!hasFlagValue(webBlock.flags, SpiderWeb_BlockFlags::BLOCKWEB_FLAG_ACTIVE))
		{
			return FALSE;
		}

		uint32 const weaponID = Weapon_GetWeaponIDByName("SpiderWeb");
		real32 const damage = Weapon_GetDamageForObject(weaponID, liveObj);
		webBlock.health -= (damage * *pElapsedGame);

		// If the SpiderWeb has negative health, we remove it and the object is no longer caught in a web
		if (webBlock.health < 0.0f)
		{
			SpiderWeb_Remove(bx, by);
			setFlagValue(liveObj->flags1, LiveFlags1::LIVEOBJ1_CAUGHTINWEB, false);
		}
		return FALSE;
	}

	// Check whether the object collides with a spider web and make it caught
	SpiderWeb_CheckCollision(liveObj);

	return FALSE;
}

// <LegoRR.exe @00466a10>
void __cdecl LegoRR::SpiderWeb_Remove(sint32 bx, sint32 by)
{
	SpiderWeb_Block& webBlock = spiderwebBlockValue(bx, by);
	setFlagValue(webBlock.object->flags3, LiveFlags3::LIVEOBJ3_REMOVING, true);
	setFlagValue(webBlock.object->flags1, LiveFlags1::LIVEOBJ1_CAUGHTINWEB, false);
	setFlagValue(webBlock.flags, SpiderWeb_BlockFlags::BLOCKWEB_FLAG_ACTIVE, false);
}

#pragma endregion
