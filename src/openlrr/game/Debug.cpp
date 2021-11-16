// Debug.cpp : 
//

#include "Debug.h"

#include "Game.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// (+1 for TVCamera)
static const char* _debugObjectTypeNames[] = {
	/*-1*/  "TVCamera",
	/* 0*/  "None",
	/* 1*/  "Vehicle",
	/* 2*/  "MiniFigure",
	/* 3*/  "RockMonster",
	/* 4*/  "Building",
	/* 5*/  "Boulder",
	/* 6*/  "PowerCrystal",
	/* 7*/  "Ore",
	/* 8*/  "Dynamite",
	/* 9*/  "Barrier",
	/*10*/  "UpgradePart",
	/*11*/  "ElectricFence",
	/*12*/  "SpiderWeb",
	/*13*/  "OohScary",
	/*14*/  "ElectricFenceStud",
	/*15*/  "Path",
	/*16*/  "Pusher",
	/*17*/  "Freezer",
	/*18*/  "IceCube",
	/*19*/  "LaserShot",
};

static const char* _debugRewardTypeNames[] = {
	/*0*/   "Crystals",
	/*1*/   "Ore",
	/*2*/   "Diggable",
	/*3*/   "Constructions",
	/*4*/   "Caverns",
	/*5*/   "Figures",
	/*6*/   "RockMonsters",
	/*7*/   "Oxygen",
	/*8*/   "Timer",
	/*9*/   "Score",
};

static const char* _debugContainerTypeNames[] {
	/*0*/ "Null",
	/*1*/ "Mesh",
	/*2*/ "Frame",
	/*3*/ "Anim",
	/*4*/ "FromActivity",
	/*5*/ "Light",
	/*6*/ "Reference",
	/*7*/ "LWS",
	/*8*/ "LWO",
};


#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

const char* LegoRR::Debug_GetObjectTypeName(LegoObject_Type objType)
{
	return _debugObjectTypeNames[(sint32)objType + 1]; // +1 for TVCamera
}

const char* LegoRR::Debug_GetObjectIDName(LegoObject_Type objType, uint32 objID)
{
	switch (objID) {
	case LegoObject_Type::LegoObject_TVCamera:
	case LegoObject_Type::LegoObject_None:
	case LegoObject_Type::LegoObject_Boulder:
	case LegoObject_Type::LegoObject_ElectricFenceStud:
	case LegoObject_Type::LegoObject_IceCube:
		return Debug_GetObjectTypeName(objType);

	case LegoObject_Type::LegoObject_Ore:
		switch (objID) {
		case LegoObject_IDs::LegoObject_ID_Ore:
			return legoGlobs.langOre_name;
		case LegoObject_IDs::LegoObject_ID_ProcessedOre:
			return legoGlobs.langProcessedOre_name;
		}
		break;

	case LegoObject_Type::LegoObject_Vehicle:
		return legoGlobs.langVehicle_name[objID];
	case LegoObject_Type::LegoObject_MiniFigure:
		return legoGlobs.langMiniFigure_name[objID];
	case LegoObject_Type::LegoObject_RockMonster:
		return legoGlobs.langRockMonster_name[objID];
	case LegoObject_Type::LegoObject_Building:
		return legoGlobs.langBuilding_name[objID];

	case LegoObject_Type::LegoObject_PowerCrystal:
		return legoGlobs.langPowerCrystal_name;
	case LegoObject_Type::LegoObject_Dynamite:
		return legoGlobs.langDynamite_name;
	case LegoObject_Type::LegoObject_Barrier:
		return legoGlobs.langBarrier_name;

	case LegoObject_Type::LegoObject_UpgradePart:
		// Generally this is never defined in Lego.cfg (but check anyway)
		if (legoGlobs.langUpgrade_name && legoGlobs.langUpgrade_name[objID])
			return legoGlobs.langUpgrade_name[objID];
		else
			return legoGlobs.upgradeName[objID];

	case LegoObject_Type::LegoObject_ElectricFence:
		return legoGlobs.langElectricFence_name;
	case LegoObject_Type::LegoObject_SpiderWeb:
		return legoGlobs.langSpiderWeb_name;
	case LegoObject_Type::LegoObject_OohScary:
		return legoGlobs.langOohScary_name;

	case LegoObject_Type::LegoObject_Path:
		return legoGlobs.langPath_name;
	case LegoObject_Type::LegoObject_Pusher:
		return legoGlobs.langTool_name[LegoObject_ToolType::LegoObject_ToolType_PusherGun];
	case LegoObject_Type::LegoObject_Freezer:
		return legoGlobs.langTool_name[LegoObject_ToolType::LegoObject_ToolType_FreezerGun];

	case LegoObject_Type::LegoObject_LaserShot:
		return legoGlobs.langTool_name[LegoObject_ToolType::LegoObject_ToolType_Laser];
	}
	return nullptr;
}

#pragma endregion
