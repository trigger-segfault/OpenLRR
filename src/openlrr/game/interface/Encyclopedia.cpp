// Encyclopedia.cpp : 
//

#include "../../engine/core/Config.h"
#include "../../engine/core/Files.h"

#include "../Debug.h"
#include "../Game.h"
#include "../object/Object.h"
#include "Panels.h"

#include "Encyclopedia.h"


// Currently there is no native way to enable encyclopedia mode. However this code can be used for testing.
// For example, toggling the mode can be hooked up to the NoNERPs shortcut.
// // Toggle off
// if (panelGlobs.panelTable[Panel_Encyclopedia].flags & PANEL_FLAG_OPEN) {
// 	Panel_ToggleOpenClosed(Panel_Encyclopedia);
// 	Encyclopedia_ClearSelection();
// 	Interface_BackToMain();
// }
// // Toggle on
// if (panelGlobs.panelTable[Panel_Encyclopedia].flags & PANEL_FLAG_CLOSED) {
// 	Interface_OpenMenu(Interface_Menu_Encyclopedia, nullptr);
// }


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004c8e88>
LegoRR::Encyclopedia_Globs & LegoRR::encyclopediaGlobs = *(LegoRR::Encyclopedia_Globs*)0x004c8e88;

/// CUSTOM: Support encyclopedia entries for more object types.
static Gods98::File* _processedOreFile = nullptr;
static Gods98::File* _electricFenceFile = nullptr;
static Gods98::File* _barrierFile = nullptr;
static Gods98::File* _dynamiteFile = nullptr;
static Gods98::File* _oohScaryFile = nullptr;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

#define Encyclopedia_ID(name) Config_ID(gameName, "Encyclopedia", name)

#define Encyclopedia_GetFileName(buffer, name) buffer = Gods98::Config_GetTempStringValue(config, Encyclopedia_ID(name))
// Testing macro to avoid modifying Lego.cfg.
//#define Encyclopedia_GetFileName(buffer, name) std::sprintf(buffer, "Information\\Encyclopaedia\\%s.epb", name)

// <LegoRR.exe @0040e3c0>
void __cdecl LegoRR::Encyclopedia_Initialise(const Gods98::Config* config, const char* gameName)
{
	uint32 count;
	const char* fileName = nullptr;
	//char fileName[1024] = { '\0' };

	count = Lego_GetObjectTypeIDCount(LegoObject_Vehicle);
	encyclopediaGlobs.vehicleFiles = (Gods98::File**)Gods98::Mem_Alloc(count * sizeof(Gods98::File*));

	for (uint32 i = 0; i < count; i++) {
		Encyclopedia_GetFileName(fileName, Object_GetTypeName(LegoObject_Vehicle, (LegoObject_ID)i));
		
		if (fileName != nullptr) {
			encyclopediaGlobs.vehicleFiles[i] = Gods98::File_Open(fileName, "r");
		}
		else {
			encyclopediaGlobs.vehicleFiles[i] = nullptr;
		}
	}


	count = Lego_GetObjectTypeIDCount(LegoObject_MiniFigure);
	encyclopediaGlobs.minifigureFiles = (Gods98::File**)Gods98::Mem_Alloc(count * sizeof(Gods98::File*));

	for (uint32 i = 0; i < count; i++) {
		Encyclopedia_GetFileName(fileName, Object_GetTypeName(LegoObject_MiniFigure, (LegoObject_ID)i));
		
		if (fileName != nullptr) {
			encyclopediaGlobs.minifigureFiles[i] = Gods98::File_Open(fileName, "r");
		}
		else {
			encyclopediaGlobs.minifigureFiles[i] = nullptr;
		}
	}


	count = Lego_GetObjectTypeIDCount(LegoObject_RockMonster);
	encyclopediaGlobs.rockmonsterFiles = (Gods98::File**)Gods98::Mem_Alloc(count * sizeof(Gods98::File*));

	for (uint32 i = 0; i < count; i++) {
		Encyclopedia_GetFileName(fileName, Object_GetTypeName(LegoObject_RockMonster, (LegoObject_ID)i));

		if (fileName != nullptr) {
			encyclopediaGlobs.rockmonsterFiles[i] = Gods98::File_Open(fileName, "r");
		}
		else {
			encyclopediaGlobs.rockmonsterFiles[i] = nullptr;
		}
	}


	count = Lego_GetObjectTypeIDCount(LegoObject_Building);
	encyclopediaGlobs.buildingFiles = (Gods98::File**)Gods98::Mem_Alloc(count * sizeof(Gods98::File*));

	for (uint32 i = 0; i < count; i++) {
		const char* typeName = Object_GetTypeName(LegoObject_Building, (LegoObject_ID)i);
		/// LEGACY: Testing logic to avoid modifying Lego.cfg or renaming files.
		//if (::_stricmp(typeName, "TeleportPad") == 0 && sizeof(fileName) != sizeof(const char*)) {
		//	typeName = "Teleport";
		//}
		Encyclopedia_GetFileName(fileName, typeName);// Object_GetTypeName(LegoObject_Building, (LegoObject_ID)i));
		
		if (fileName != nullptr) {
			encyclopediaGlobs.buildingFiles[i] = Gods98::File_Open(fileName, "r");
		}
		else {
			encyclopediaGlobs.buildingFiles[i] = nullptr;
		}
	}


	Encyclopedia_GetFileName(fileName, Object_GetTypeName(LegoObject_PowerCrystal, (LegoObject_ID)0));
	if (fileName != nullptr) {
		encyclopediaGlobs.powercrystalFile = Gods98::File_Open(fileName, "r");
	}
	else {
		encyclopediaGlobs.powercrystalFile = nullptr;
	}


	Encyclopedia_GetFileName(fileName, Object_GetTypeName(LegoObject_Ore, LegoObject_ID_Ore));
	if (fileName != nullptr) {
		encyclopediaGlobs.oreFile = Gods98::File_Open(fileName, "r");
	}
	else {
		encyclopediaGlobs.oreFile = nullptr;
	}

	/// CUSTOM: Add support for ProcessedOre.
	Encyclopedia_GetFileName(fileName, Object_GetTypeName(LegoObject_Ore, LegoObject_ID_ProcessedOre));
	if (fileName != nullptr) {
		_processedOreFile = Gods98::File_Open(fileName, "r");
	}
	else {
		_processedOreFile = nullptr;
	}

	/// CUSTOM: Add support for ElectricFences.
	Encyclopedia_GetFileName(fileName, Object_GetTypeName(LegoObject_ElectricFence, (LegoObject_ID)0));
	if (fileName != nullptr) {
		_electricFenceFile = Gods98::File_Open(fileName, "r");
	}
	else {
		_electricFenceFile = nullptr;
	}

	/// NOTE: The following objects are not yet selectable in the encyclopedia.

	/// CUSTOM: Add support for Barriers.
	Encyclopedia_GetFileName(fileName, Debug_GetObjectTypeName(LegoObject_Barrier));
	if (fileName != nullptr) {
		_barrierFile = Gods98::File_Open(fileName, "r");
	}
	else {
		_barrierFile = nullptr;
	}

	/// CUSTOM: Add support for Dynamite.
	Encyclopedia_GetFileName(fileName, Debug_GetObjectTypeName(LegoObject_Dynamite));
	if (fileName != nullptr) {
		_dynamiteFile = Gods98::File_Open(fileName, "r");
	}
	else {
		_dynamiteFile = nullptr;
	}

	/// CUSTOM: Add support for SonicBlasters.
	Encyclopedia_GetFileName(fileName, Debug_GetObjectTypeName(LegoObject_OohScary));
	if (fileName != nullptr) {
		_oohScaryFile = Gods98::File_Open(fileName, "r");
	}
	else {
		_oohScaryFile = nullptr;
	}
}

// <LegoRR.exe @0040e630>
void __cdecl LegoRR::Encyclopedia_SelectObject(LegoObject* liveObj)
{
	LegoObject_Type objType;
	LegoObject_ID objID;
	LegoObject_GetTypeAndID(liveObj, &objType, &objID);

	bool supported = true;

	switch (objType) {
	case LegoObject_Vehicle:
		encyclopediaGlobs.currentObjFile = encyclopediaGlobs.vehicleFiles[objID];
		break;
	case LegoObject_MiniFigure:
		encyclopediaGlobs.currentObjFile = encyclopediaGlobs.minifigureFiles[objID];
		break;
	case LegoObject_RockMonster:
		encyclopediaGlobs.currentObjFile = encyclopediaGlobs.rockmonsterFiles[objID];
		break;
	case LegoObject_Building:
		encyclopediaGlobs.currentObjFile = encyclopediaGlobs.buildingFiles[objID];
		break;
	case LegoObject_PowerCrystal:
		encyclopediaGlobs.currentObjFile = encyclopediaGlobs.powercrystalFile;
		break;
	case LegoObject_Ore:
		/// CUSTOM: Add support for ProcessedOre.
		switch (objID) {
		case LegoObject_ID_Ore:
			encyclopediaGlobs.currentObjFile = encyclopediaGlobs.oreFile;
			break;
		case LegoObject_ID_ProcessedOre:
			encyclopediaGlobs.currentObjFile = _processedOreFile;
			break;
		default:
			supported = false;
			break;
		}
		break;
	case LegoObject_ElectricFence:
		/// CUSTOM: Add support for ElectricFences.
		encyclopediaGlobs.currentObjFile = _electricFenceFile;
		break;

	/// NOTE: The following objects are not yet selectable in the encyclopedia.
	case LegoObject_Barrier:
		/// CUSTOM: Add support for Barriers.
		encyclopediaGlobs.currentObjFile = _barrierFile;
		break;
	case LegoObject_Dynamite:
		/// CUSTOM: Add support for Dynamite.
		encyclopediaGlobs.currentObjFile = _dynamiteFile;
		break;
	case LegoObject_OohScary:
		/// CUSTOM: Add support for SonicBlasters.
		encyclopediaGlobs.currentObjFile = _oohScaryFile;
		break;

	default:
		supported = false;
		break;
	}

	if (!supported) {
		encyclopediaGlobs.currentObjFile = nullptr;
		// Or just call Encyclopedia_ClearSelection() for the below statement.
		// Note that ENCYCLOPEDIA_GLOB_FLAG_NEEDSTEXT doesn't need to be cleared
		//  since it's only used when active.
		encyclopediaGlobs.flags &= ~ENCYCLOPEDIA_GLOB_FLAG_ACTIVE;
	}
	else {
		encyclopediaGlobs.flags |= (ENCYCLOPEDIA_GLOB_FLAG_ACTIVE|ENCYCLOPEDIA_GLOB_FLAG_NEEDSTEXT);
		encyclopediaGlobs.currentObj = liveObj;
		Panel_TextWindow_Clear(panelGlobs.encyclopediaTextWnd);
	}
}

// <LegoRR.exe @0040e710>
void __cdecl LegoRR::Encyclopedia_ClearSelection(void)
{
	encyclopediaGlobs.flags &= ~ENCYCLOPEDIA_GLOB_FLAG_ACTIVE;
}

// <LegoRR.exe @0040e720>
void __cdecl LegoRR::Encyclopedia_Update(real32 elapsedAbs)
{
	if ((encyclopediaGlobs.flags & ENCYCLOPEDIA_GLOB_FLAG_ACTIVE) && !Panel_IsFullyClosed(Panel_Encyclopedia)) {

		if (encyclopediaGlobs.flags & ENCYCLOPEDIA_GLOB_FLAG_NEEDSTEXT) {
			if (encyclopediaGlobs.currentObjFile == nullptr) {
				Panel_TextWindow_PrintF(panelGlobs.encyclopediaTextWnd, "Object has no encyclopedia file.");
			}
			else {
				Gods98::File_Seek(encyclopediaGlobs.currentObjFile, 0, Gods98::SeekOrigin::Set);

				/// SANITY: Extra buffer size for encylopedia lines.
				char buff[512*4] = { '\0' }; // dummy init

				char* str = Gods98::File_GetS(buff, sizeof(buff), encyclopediaGlobs.currentObjFile);
				while (str != nullptr) {
					Panel_TextWindow_PrintF(panelGlobs.encyclopediaTextWnd, "%s", buff);
					str = Gods98::File_GetS(buff, sizeof(buff), encyclopediaGlobs.currentObjFile);
				}
			}
			encyclopediaGlobs.flags &= ~ENCYCLOPEDIA_GLOB_FLAG_NEEDSTEXT; // Object text has been assigned.
		}

		Panel_TextWindow_Update(panelGlobs.encyclopediaTextWnd, 0, elapsedAbs);
	}
}

// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @0040e800>
void __cdecl LegoRR::Encyclopedia_DrawSelectBox(Gods98::Viewport* viewMain)
{
	if ((encyclopediaGlobs.flags & ENCYCLOPEDIA_GLOB_FLAG_ACTIVE) &&
		(panelGlobs.panelTable[Panel_Encyclopedia].flags & PANEL_FLAG_OPEN) &&
		encyclopediaGlobs.currentObj != nullptr)
	{
		Lego_DrawObjectSelectionBox(encyclopediaGlobs.currentObj, viewMain, 0.0f, 1.0f, 0.0f); // green
	}
}

/// CUSTOM: Isolate Draw API calls from Encyclopedia_DrawSelectBox.
void __cdecl LegoRR::Encyclopedia_DrawSelectName(Gods98::Viewport* viewMain)
{
	if ((encyclopediaGlobs.flags & ENCYCLOPEDIA_GLOB_FLAG_ACTIVE) &&
		(panelGlobs.panelTable[Panel_Encyclopedia].flags & PANEL_FLAG_OPEN) &&
		encyclopediaGlobs.currentObj != nullptr)
	{
		Lego_DrawObjectName(encyclopediaGlobs.currentObj, viewMain);
	}
}

// Removes the current encyclopedia object if it matches the specified object.
// <LegoRR.exe @0040e840>
void __cdecl LegoRR::Encyclopedia_RemoveCurrentReference(LegoObject* liveObj)
{
	if (encyclopediaGlobs.currentObj == liveObj) {
		encyclopediaGlobs.currentObj = nullptr;
	}
}

#pragma endregion
