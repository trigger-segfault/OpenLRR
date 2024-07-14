// InfoMessages.cpp : 
//

#include "InfoMessages.h"

#include "Interface.h"
#include "Panels.h"
#include "ScrollInfo.h"
#include "../Game.h"
#include "../audio/SFX.h"
#include "../interface/Pointers.h"
#include "../mission/NERPsFunctions.h"

#include "../../engine/Main.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004dd658>
LegoRR::Info_Globs & LegoRR::infoGlobs = *(LegoRR::Info_Globs*)0x004dd658;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00419160>
void __cdecl LegoRR::Info_Initialise(Gods98::Font* font)
{
	Info_RegisterName(Info_CrystalFound);
	Info_RegisterName(Info_OreSeamFound);
	Info_RegisterName(Info_RockMonster);
	Info_RegisterName(Info_LavaRockMonster);
	Info_RegisterName(Info_IceRockMonster);
	Info_RegisterName(Info_UnderAttack);
	Info_RegisterName(Info_Landslide);
	Info_RegisterName(Info_CaveIn);
	Info_RegisterName(Info_Constructed);
	Info_RegisterName(Info_CavernLocated);
	Info_RegisterName(Info_LegoManDeath);
	Info_RegisterName(Info_VehicleDeath);
	Info_RegisterName(Info_BuildingDeath);
	Info_RegisterName(Info_DynamitePlaced);
	Info_RegisterName(Info_NoPower);
	Info_RegisterName(Info_PowerDrain);
	Info_RegisterName(Info_AirDepleting);
	Info_RegisterName(Info_AirLow);
	Info_RegisterName(Info_AirOut);
	Info_RegisterName(Info_AirRestored);
	Info_RegisterName(Info_TrainDriver);
	Info_RegisterName(Info_TrainDynamite);
	Info_RegisterName(Info_TrainRepair);
	Info_RegisterName(Info_TrainPilot);
	Info_RegisterName(Info_TrainSailor);
	Info_RegisterName(Info_TrainScanner);
	Info_RegisterName(Info_OreCollected);
	Info_RegisterName(Info_WallDug);
	Info_RegisterName(Info_WallReinforced);
	Info_RegisterName(Info_CrystalPower);
	Info_RegisterName(Info_LavaErode);
	Info_RegisterName(Info_SlugEmerge);
	Info_RegisterName(Info_PathCompleted);
	Info_RegisterName(Info_FoundMinifigure);
	Info_RegisterName(Info_CanUpgradeMinifigure);
	Info_RegisterName(Info_CanTrainMinifigure);
	Info_RegisterName(Info_CrystalSeamFound);
	Info_RegisterName(Info_GenericSeamFound);
	Info_RegisterName(Info_GenericDeath);
	Info_RegisterName(Info_GenericMonster);

	infoGlobs.font = font;
	infoGlobs.flags |= Info_GlobFlags::INFO_GLOB_FLAG_UNK_8 | Info_GlobFlags::INFO_GLOB_FLAG_AUTOGAMESPEED;
}

// <LegoRR.exe @00419310>
bool32 __cdecl LegoRR::Info_GetInfoType(const char* infoName, OUT Info_Type* infoType)
{
	for (int i = 0; i < Info_Type_Count; i++) {
		int cmp = ::_stricmp(infoGlobs.infoName[i], infoName);
		if (cmp == 0) {
			*infoType = (Info_Type)i;
			return true;
		}
	}

	return false;
}

// <LegoRR.exe @00419350>
void* __cdecl LegoRR::Info_GetTypePtr4(Info_Type infoType)
{
	return infoGlobs.infoDataTable[infoType].ptr_4;
}

// <LegoRR.exe @00419360>
void __cdecl LegoRR::Info_SetOverFlowImageFile(const char* filename)
{
	infoGlobs.OverFlowImage = Gods98::Image_LoadBMPScaled(filename, 0, 0);
	Gods98::Image_SetPenZeroTrans(infoGlobs.OverFlowImage);
}

// <LegoRR.exe @00419380>
void __cdecl LegoRR::Info_SetAutoGameSpeed(bool32 autoOn)
{
	if (autoOn) {
		infoGlobs.flags |= Info_GlobFlags::INFO_GLOB_FLAG_AUTOGAMESPEED;
	}
	else {
		infoGlobs.flags &= ~Info_GlobFlags::INFO_GLOB_FLAG_AUTOGAMESPEED;
	}
}

// <LegoRR.exe @004193a0>
void __cdecl LegoRR::Info_SetTypeChangeGameSpeed(Info_Type infoType, bool32 changeSpeedOn)
{
	if (changeSpeedOn) {
		infoGlobs.infoDataTable[infoType].flags |= InfoDataFlags::INFOTYPE_FLAG_CHANGEGAMESPEED;
	}
	else {
		infoGlobs.infoDataTable[infoType].flags &= ~InfoDataFlags::INFOTYPE_FLAG_CHANGEGAMESPEED;
	}
}

// <LegoRR.exe @004193e0>
void __cdecl LegoRR::Info_SetTypeText(Info_Type infoType, const char* text)
{
	char* buffer = infoGlobs.infoDataTable[infoType].text;
	if (buffer != nullptr) {
		Gods98::Mem_Free(buffer);
	}

	Info_SetText_internal(text, &infoGlobs.infoDataTable[infoType].text);
}

// <LegoRR.exe @00419420>
void __cdecl LegoRR::Info_SetTypeFlag_20000(Info_Type infoType, bool32 setFlag20000)
{
	if (setFlag20000) {
		infoGlobs.infoDataTable[infoType].flags |= InfoDataFlags::INFOTYPE_FLAG_UNK_20000;
	}
	else {
		infoGlobs.infoDataTable[infoType].flags &= ~InfoDataFlags::INFOTYPE_FLAG_UNK_20000;
	}
}

// <LegoRR.exe @00419460>
void __cdecl LegoRR::Info_SetText_internal(const char* text, char** pInfoText)
{
	char imagePointers[8];
	uint32 imagePointersCount = 0;

	char* pp = *pInfoText;

	const char* ptr = text;
	uint32 charsLeft = 0xFFFFFFFF;

	while (*ptr != '\0') {
		if (charsLeft == 0) {
			break;
		}

		charsLeft--;
		ptr++;
	}

	char* infoText = (char*)Gods98::Mem_Alloc(~charsLeft + 0xB);
	*pInfoText = infoText;

	char* infoTextPtr = infoText;

	// Converts underlines to spaces, \n to a newline character.
	// Loads a pointer to an image spacified between $, and changes it to %b.
	// Stores the lowest byte of each image pointer at the end (after null byte).

	ptr = text;
	while (*ptr != '\0') {
		char c = *ptr;

		if (c == '_') {
			*infoTextPtr = ' ';
		}
		else if (c == '$') {
			ptr++;

			char infoNameBuffer[32];
			int infoNameSize = 0;

			while (*ptr != '$') {
				infoNameBuffer[infoNameSize] = *ptr;
				infoNameSize++;
				ptr++;
			}

			infoNameBuffer[infoNameSize] = '\0';

			Gods98::Image* image = nullptr;

			Pointer_Type pointerType;
			if (!Pointer_GetType(infoNameBuffer, &pointerType)) {
				Info_Type infoType;
				if (Info_GetInfoType(infoNameBuffer, &infoType)) {
					image = (Gods98::Image*)Info_GetTypePtr4(infoType);
				}
			}
			else {
				image = Pointer_GetImage(pointerType).image;
			}

			if (image != nullptr) {
				/// TODO: Check what this is actually meant to display as %b is not supported
				*infoTextPtr = '%';
				infoTextPtr++;
				*infoTextPtr = 'b';

				imagePointers[imagePointersCount] = (char)image;
				imagePointersCount++;
			}
		}
		else if (c == '\\' && *(ptr + 1) == 'n') {
			*infoTextPtr = '\n';
			ptr++;
		}
		else {
			*infoTextPtr = c;
		}

		infoTextPtr++;
		ptr++;
	}

	*infoTextPtr = '\0';
	infoTextPtr++;

	for (uint32 i = 0; i < 5; i++) {
		if (i < imagePointersCount) {
			*infoTextPtr = imagePointers[i];
		}
		else {
			*infoTextPtr = '\0';
		}

		infoTextPtr++;
	}
}

// <LegoRR.exe @00419580>
void __cdecl LegoRR::Info_SetTypeImageFile(Info_Type infoType, const char* filename)
{
	Gods98::Image* image = Gods98::Image_LoadBMPScaled(filename, 0, 0);
	infoGlobs.infoDataTable[infoType].ptr_4 = image;
	Gods98::Image_SetPenZeroTrans(image);
}

// <LegoRR.exe @004195b0>
void __cdecl LegoRR::Info_SetTypeSFX(Info_Type infoType, SFX_ID sfxID)
{
	infoGlobs.infoDataTable[infoType].sfxType = sfxID;
}

// <LegoRR.exe @004195d0>
bool32 __cdecl LegoRR::Info_EnumerateMessageInstances(sint32 handle, InfoEnumerateCallback callback, void* data)
{
	if (infoGlobs.infoMessageTable[handle].instanceCount == 0) {
		return false;
	}

	InfoMessageInstance* instance = infoGlobs.infoMessageTable[handle].instance;
	if (instance == nullptr) {
		return false;
	}

	while (instance != nullptr) {
		if (callback(instance, data)) {
			return true;
		}

		instance = instance->next;
	}

	return false;
}

// <LegoRR.exe @00419620>
void __cdecl LegoRR::Info_AddMessageInstance(sint32 handle, InfoMessageInstance* instance)
{
	InfoMessageInstance* curr = infoGlobs.infoMessageTable[handle].instance;
	if (curr == nullptr) {
		infoGlobs.infoMessageTable[handle].instance = instance;
		instance->next = nullptr;
		infoGlobs.infoMessageTable[handle].instanceCount++;
		return;
	}

	int count = 1;
	InfoMessageInstance* next = curr->next;

	while (next != nullptr) {
		count++;

		/* Maximum of 9 instances for a single type, pop the oldest one. */
		if (count == 9) {
			Info_RemoveMessageInstance(handle, 0);
		}

		curr = curr->next;
		next = curr->next;
	}

	curr->next = instance;
	instance->next = nullptr;
	infoGlobs.infoMessageTable[handle].instanceCount++;
}

// <LegoRR.exe @004196b0>
LegoRR::InfoMessageInstance* __cdecl LegoRR::Info_GetMessageInstance(sint32 handle, sint32 instanceIndex)
{
	if (instanceIndex >= 0 && instanceIndex < static_cast<sint32>(infoGlobs.infoMessageTable[handle].instanceCount)) {
		InfoMessageInstance* instance = infoGlobs.infoMessageTable[handle].instance;

		while (instanceIndex != 0) {
			instance = instance->next;
			instanceIndex--;
		}

		return instance;
	}
	return nullptr;
}

// <LegoRR.exe @004196e0>
LegoRR::InfoMessageInstance* __cdecl LegoRR::Info_RemoveMessageInstance(sint32 handle, sint32 instanceIndex)
{
	InfoMessageInstance* instance = Info_GetMessageInstance(handle, instanceIndex);
	if (instance == nullptr) {
		return nullptr;
	}

	InfoMessageInstance* prevInstance = Info_GetMessageInstance(handle, instanceIndex - 1);
	if (instanceIndex == 0) {
		infoGlobs.infoMessageTable->instance = instance->next;
	}
	else {
		prevInstance->next = instance->next;
	}

	infoGlobs.infoMessageTable[handle].instanceCount--;
	return instance;
}

// <LegoRR.exe @00419740>
bool32 __cdecl LegoRR::Info_Callback_FindObjectReference(InfoMessageInstance* instance, void* search)
{
	SearchInfoObject_8* searchInfoObject = (SearchInfoObject_8*)search;
	if (instance->object == searchInfoObject->object) {
		return true;
	}
	searchInfoObject->index++;
	return false;
}

// <LegoRR.exe @00419760>
void __cdecl LegoRR::Info_RemoveObjectReferences(LegoObject* liveObj)
{
	if (infoGlobs.infoMessageCount != 0) {
		uint32 handle = 0;
		while (handle < infoGlobs.infoMessageCount) {
			/* Repeatedly search for all info message instances that reference this object until none are left. */
			SearchInfoObject_8 search = { liveObj, 0 };

			while (Info_EnumerateMessageInstances(handle, Info_Callback_FindObjectReference, &search)) {
				/// FIXME: Free the messgae instance
				Info_RemoveMessageInstance(handle, search.index);
				search.index = 0;
			}

			if (infoGlobs.infoMessageTable[handle].instanceCount == 0) {
				Info_RemoveMessage(handle);
			}
			else {
				handle++;
			}
		}
	}
}

// <LegoRR.exe @004197f0>
bool32 __cdecl LegoRR::Info_Callback_FindBlockPos(InfoMessageInstance* infoInstance, void* search)
{
	SearchInfoBlockPos_8* searchInfoBlockPos = (SearchInfoBlockPos_8*)search;

	if (infoInstance->blockPos.x == searchInfoBlockPos->pBlockPos->x &&
		infoInstance->blockPos.y == searchInfoBlockPos->pBlockPos->y) {
		return true;
	}

	searchInfoBlockPos->index++;
	return false;
}

// <LegoRR.exe @00419820>
void __cdecl LegoRR::Info_RemoveAllAtBlockPos(const Point2I* blockPos)
{
	if (infoGlobs.infoMessageCount != 0) {
		uint32 handle = 0;
		while (handle < infoGlobs.infoMessageCount) {
			SearchInfoBlockPos_8 search = { blockPos, 0 };

			if (infoGlobs.infoMessageTable[handle].infoType == Info_Type::Info_Landslide ||
				infoGlobs.infoMessageTable[handle].infoType == Info_Type::Info_CaveIn) {
				while (Info_EnumerateMessageInstances(handle, Info_Callback_FindBlockPos, &search)) {
					/// FIXME: Free the message instance
					Info_RemoveMessageInstance(handle, search.index);
					search.index = 0;
				}

				if (infoGlobs.infoMessageTable[handle].instanceCount == 0) {
					Info_RemoveMessage(handle);
					handle--;
				}
			}

			handle++;
		}
	}
}

// <LegoRR.exe @004198d0>
bool32 __cdecl LegoRR::Info_Callback_FindObjectAndBlockPos(InfoMessageInstance* instance, void* search)
{
	SearchInfoObjectBlockPos_8* searchInfoObjectBlockPos = (SearchInfoObjectBlockPos_8*)search;

	const Point2I* searchBlockPos = searchInfoObjectBlockPos->optBlockPos;
	LegoObject* searchObj = searchInfoObjectBlockPos->optObject;

	if (searchObj != nullptr || searchBlockPos != nullptr) {
		if (searchObj == nullptr || searchObj != instance->object) {
			if (searchBlockPos == nullptr || searchBlockPos->x != instance->blockPos.x ||
				searchBlockPos->y != instance->blockPos.y) {
				return false;
			}
		}

	}

	return true;
}

// <LegoRR.exe @00419920>
bool32 __cdecl LegoRR::Info_HasTypeAtObjectOrBlockPos(Info_Type infoType, OPTIONAL LegoObject* liveObj, OPTIONAL const Point2I* blockPos)
{
	if (infoType != Info_Type::Info_LegoManDeath && infoType != Info_Type::Info_BuildingDeath &&
		infoType != Info_Type::Info_VehicleDeath && infoType != Info_Type::Info_GenericDeath) {
		if (infoGlobs.infoMessageCount != 0) {
			SearchInfoObjectBlockPos_8 search = { liveObj,blockPos };

			for (uint32 handle = 0; handle < infoGlobs.infoMessageCount; handle++) {
				if (infoType == infoGlobs.infoMessageTable[handle].infoType) {
					if (Info_EnumerateMessageInstances(handle, Info_Callback_FindObjectAndBlockPos, &search)) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

// <LegoRR.exe @004199b0>
LegoRR::InfoMessageInstance* __cdecl LegoRR::Info_CreateInstance(OPTIONAL const Point2I* blockPos, OPTIONAL LegoObject* liveObj, OPTIONAL const char* text)
{
	InfoMessageInstance* instance = (InfoMessageInstance*)Gods98::Mem_Alloc(sizeof(InfoMessageInstance));
	if (instance == nullptr) {
		return nullptr;
	}

	if (blockPos == nullptr) {
		instance->blockPos.x = -1;
		instance->blockPos.y = -1;
	}
	else {
		instance->blockPos = *blockPos;
	}

	instance->object = liveObj;

	if (text != nullptr) {
		Info_SetText_internal(text, &instance->text);
	}
	else {
		instance->text = nullptr;
	}

	return instance;
}

// <LegoRR.exe @00419a10>
void __cdecl LegoRR::Info_SetFlag4(bool32 state)
{
	if (state) {
		infoGlobs.flags |= Info_GlobFlags::INFO_GLOB_FLAG_UNK_4;
	}
	else {
		infoGlobs.flags &= ~Info_GlobFlags::INFO_GLOB_FLAG_UNK_4;
	}
}

// <LegoRR.exe @00419a30>
bool32 __cdecl LegoRR::Info_HasTypeText(Info_Type infoType)
{
	return infoGlobs.infoDataTable[infoType].text != nullptr;
}

// <LegoRR.exe @00419a50>
sint32 __cdecl LegoRR::Info_FindExistingMessageType(Info_Type infoType)
{
	for (uint32 i = 0; i < infoGlobs.infoMessageCount; i++) {
		if (infoGlobs.infoMessageTable[i].infoType == infoType) {
			return i;
		}
	}

	return -1;
}

// Calculates the y position of the info message
// <LegoRR.exe @00419a80>
real32 __cdecl LegoRR::Info_FUN_00419a80(void)
{
	real32 value = 330.0;

	if (infoGlobs.infoMessageCount != 0) {
		for (int count = infoGlobs.infoMessageCount; count >= 0; count--) {
			Info_Type infoType = infoGlobs.infoMessageTable[count].infoType;
			value -= infoGlobs.infoDataTable[infoType].float_c;
		}
	}

	return value;
}

// <LegoRR.exe @00419ab0>
void __cdecl LegoRR::Info_Send(Info_Type infoType, OPTIONAL const char* opt_text, OPTIONAL LegoObject* liveObj, OPTIONAL const Point2I* blockPos)
{
	if (infoGlobs.flags & Info_GlobFlags::INFO_GLOB_FLAG_UNK_4) {
		if ((NERPFunc__GetTutorialFlags(nullptr) & TutorialFlags::TUTORIAL_FLAG_NOINFO) == TutorialFlags::TUTORIAL_FLAG_NONE) {
			InfoData* infoData = &infoGlobs.infoDataTable[infoType];

			if (infoGlobs.flags & Info_GlobFlags::INFO_GLOB_FLAG_UNK_8 &&
				infoData->flags != InfoDataFlags::INFOTYPE_FLAG_NONE &&
				(infoData->flags & InfoDataFlags::INFOTYPE_FLAG_UNK_20000) != InfoDataFlags::INFOTYPE_FLAG_NONE &&
				infoData->float_c <= 0.0) {
				infoData->float_c = 25.0;

				if (infoType == Info_Type::Info_LegoManDeath || infoType == Info_Type::Info_BuildingDeath || infoType == Info_Type::Info_VehicleDeath) {
					infoGlobs.infoDataTable[Info_Type::Info_LegoManDeath].float_c = 25.0;
					infoGlobs.infoDataTable[Info_Type::Info_VehicleDeath].float_c = 25.0;
					infoGlobs.infoDataTable[Info_Type::Info_BuildingDeath].float_c = 25.0;
					infoGlobs.infoDataTable[Info_Type::Info_GenericDeath].float_c = 25.0;
				}

				SFX_Random_SetAndPlayGlobalSample(infoData->sfxType, nullptr);
			}

			if (!Info_HasTypeAtObjectOrBlockPos(infoType, liveObj, blockPos)) {
				if (Info_HasTypeText(infoType)) {
					if (infoGlobs.flags & Info_GlobFlags::INFO_GLOB_FLAG_UNK_8 &&
						(infoData->flags & InfoDataFlags::INFOTYPE_FLAG_UNK_20000) == InfoDataFlags::INFOTYPE_FLAG_NONE &&
						infoData->float_c <= 0.0) {
						infoData->float_c = 25.0;

						if (infoType == Info_Type::Info_LegoManDeath || infoType == Info_Type::Info_BuildingDeath ||
							infoType == Info_Type::Info_VehicleDeath || infoType == Info_Type::Info_GenericDeath) {
							infoGlobs.infoDataTable[Info_Type::Info_LegoManDeath].float_c = 25.0;
							infoGlobs.infoDataTable[Info_Type::Info_VehicleDeath].float_c = 25.0;
							infoGlobs.infoDataTable[Info_Type::Info_BuildingDeath].float_c = 25.0;
							infoGlobs.infoDataTable[Info_Type::Info_GenericDeath].float_c = 25.0;
						}

						SFX_Random_SetAndPlayGlobalSample(infoData->sfxType, nullptr);
					}

					if (infoGlobs.infoMessageCount != Info_Type_Count) {
						int handle = Info_FindExistingMessageType(infoType);

						if (handle == -1) {
							InfoMessage* infoMessage = &infoGlobs.infoMessageTable[infoGlobs.infoMessageCount];

							handle = infoGlobs.infoMessageCount;
							infoGlobs.infoMessageCount++;

							infoMessage->float_c = -(float)((Gods98::Image*)(infoData->ptr_4))->width;
							infoGlobs.infoMessageTable[handle].next = Info_FUN_00419a80();
						}

						infoGlobs.infoMessageTable[handle].infoType = infoType;
						InfoMessageInstance* instance = Info_CreateInstance(blockPos, liveObj, opt_text);
						Info_AddMessageInstance(handle, instance);

						uint32 progMode = Gods98::Main_ProgrammerMode();
						if (progMode < 5 && (infoGlobs.flags & Info_GlobFlags::INFO_GLOB_FLAG_AUTOGAMESPEED) &&
							(infoData->flags & InfoDataFlags::INFOTYPE_FLAG_CHANGEGAMESPEED) != InfoDataFlags::INFOTYPE_FLAG_NONE) {
							float gameSpeed = Lego_GetGameSpeed();
							if (gameSpeed > 1.0) {
								Lego_SetGameSpeed(1.0);
							}
						}
					}
				}
			}
		}
	}
}

// <LegoRR.exe @00419cd0>
void __cdecl LegoRR::Info_GotoFirst(void)
{
	if (infoGlobs.infoMessageCount != 0 && infoGlobs.infoMessageTable[0].instanceCount != 0) {
		if (infoGlobs.infoMessageTable[0].instance->object != nullptr &&
			infoGlobs.infoMessageTable[0].instance->blockPos.x > 0 &&
			infoGlobs.infoMessageTable[0].instance->blockPos.y > 0) {
			Lego_Goto(infoGlobs.infoMessageTable[0].instance->object, &infoGlobs.infoMessageTable[0].instance->blockPos, false);
		}
	}
}

// <LegoRR.exe @00419d10>
void __cdecl LegoRR::Info_UpdateMessage(uint32 handle)
{
	if (handle < infoGlobs.infoMessageCount) {
		if (handle != 0) {
			// Copies the selected message to index 0, and shifts the rest up
			// The 'next' member is not swapped, as this causes extra movement, which differs from the original
			InfoMessage infoMessage = infoGlobs.infoMessageTable[handle];

			for (int i = handle; i != 0; i--) {
				infoGlobs.infoMessageTable[i].instance = infoGlobs.infoMessageTable[i - 1].instance;
				infoGlobs.infoMessageTable[i].instanceCount = infoGlobs.infoMessageTable[i - 1].instanceCount;
				infoGlobs.infoMessageTable[i].float_c = infoGlobs.infoMessageTable[i - 1].float_c;
				infoGlobs.infoMessageTable[i].infoType = infoGlobs.infoMessageTable[i - 1].infoType;
				//infoGlobs.infoMessageTable[i].next = infoGlobs.infoMessageTable[i - 1].next;
			}

			float next = infoGlobs.infoMessageTable[0].next;
			infoGlobs.infoMessageTable[0] = infoMessage;
			infoGlobs.infoMessageTable[0].next = next;
		}

		infoGlobs.flags |= (Info_GlobFlags::INFO_GLOB_FLAG_UNK_1 | Info_GlobFlags::INFO_GLOB_FLAG_UNK_2);
	}
}

// <LegoRR.exe @00419d90>
void __cdecl LegoRR::Info_PopFirstMessage(void)
{
	if (infoGlobs.infoMessageCount != 0) {
		Info_RemoveMessage(0);
	}
}

// <LegoRR.exe @00419db0>
void __cdecl LegoRR::Info_RemoveMessage(uint32 handle)
{
	if (handle < infoGlobs.infoMessageCount) {
		/// FIXME: Free the message instance
		InfoMessageInstance* instance = Info_RemoveMessageInstance(handle, 0);
		if (instance != nullptr && instance->text != nullptr) {
			Gods98::Mem_Free(instance->text);
		}

		if (infoGlobs.infoMessageTable[handle].instanceCount == 0) {
			for (uint32 i = 0; i < infoGlobs.infoMessageCount - 1; i++) {
				infoGlobs.infoMessageTable[i].instance = infoGlobs.infoMessageTable[i + 1].instance;
				infoGlobs.infoMessageTable[i].instanceCount = infoGlobs.infoMessageTable[i + 1].instanceCount;
				infoGlobs.infoMessageTable[i].float_c = infoGlobs.infoMessageTable[i + 1].float_c;
				infoGlobs.infoMessageTable[i].infoType = infoGlobs.infoMessageTable[i + 1].infoType;
				infoGlobs.infoMessageTable[i].next = infoGlobs.infoMessageTable[i + 1].next;
			}

			infoGlobs.infoMessageCount--;
		}

		if (handle == 0) {
			infoGlobs.flags &= ~Info_GlobFlags::INFO_GLOB_FLAG_UNK_1;
		}
	}
}

// <LegoRR.exe @00419e40>
void __cdecl LegoRR::Info_ClearAllMessages(void)
{
	while (infoGlobs.infoMessageCount != 0) {
		Info_RemoveMessage(0);
	}
}

// <LegoRR.exe @00419e60>
void __cdecl LegoRR::Info_Draw(real32 elapsedAbs)
{
	if (infoGlobs.infoMessageCount == 0) {
		return;
	}

	Point2F pos = { 0.0,330.0 };

	for (uint32 i = 0; i < infoGlobs.infoMessageCount; i++) {
		InfoMessage* infoMessage = &infoGlobs.infoMessageTable[i];

		Gods98::Image* image = (Gods98::Image*)infoGlobs.infoDataTable[infoMessage->infoType].ptr_4;

		if (infoMessage->next < pos.y) {
			infoMessage->next -= elapsedAbs * -8.0f;
		}
		if (infoMessage->next > pos.y) {
			infoMessage->next = pos.y;
		}

		if (infoMessage->float_c < 0.0) {
			infoMessage->float_c -= elapsedAbs * -3.0f;
		}
		if (infoMessage->float_c > 0.0) {
			infoMessage->float_c = 0.0;
		}

		pos = { infoMessage->float_c, infoMessage->next };
		Gods98::Image_DisplayScaled(image, nullptr, &pos, nullptr);

		if (pos.y < 0.0) {
			pos.y = 0.0;
			Gods98::Image_DisplayScaled(infoGlobs.OverFlowImage, nullptr, &pos, nullptr);
			break;
		}

		sint32 y = static_cast<sint32>(pos.y) + 10;
		sint32 x = static_cast<sint32>(pos.x) + 2;

		Gods98::Font_PrintF(Interface_GetFont(), x, y, "%i", infoMessage->instanceCount);

		pos.y -= image->height;
	}
}

// <LegoRR.exe @00419fb0>
void __cdecl LegoRR::Info_DrawPanel(real32 elapsedAbs)
{
	if ((infoGlobs.flags & Info_GlobFlags::INFO_GLOB_FLAG_UNK_1) == Info_GlobFlags::INFO_GLOB_FLAG_NONE ||
		infoGlobs.infoMessageCount == 0) {
		if ((panelGlobs.panelTable[Panel_Information].flags & PanelDataFlags::PANEL_FLAG_CLOSED) == 0) {
			Panel_ToggleOpenClosed(Panel_Information);
		}
	}
	else {
		if ((panelGlobs.panelTable[Panel_Information].flags & PanelDataFlags::PANEL_FLAG_OPEN) == 0) {
			Panel_ToggleOpenClosed(Panel_Information);
		}

		if ((infoGlobs.flags & Info_GlobFlags::INFO_GLOB_FLAG_UNK_2) != Info_GlobFlags::INFO_GLOB_FLAG_NONE) {
			Panel_TextWindow_Clear(panelGlobs.infoTextWnd);

			const char* message = infoGlobs.infoMessageTable[0].instance->text;
			if (message == nullptr) {
				message = infoGlobs.infoDataTable[infoGlobs.infoMessageTable[0].infoType].text;
			}

			uint32 count = 0xFFFFFFFF;
			const char* ptr = message;
			while (*ptr != '\0') {
				if (count == 0) {
					break;
				}

				count--;
				ptr++;
			}
			count = ~count + 1;

			if (message[count] == '\0') {
				Panel_TextWindow_PrintF(panelGlobs.infoTextWnd, message);
			}
			else {
				Panel_TextWindow_PrintF(panelGlobs.infoTextWnd, message, (uint32)message[count], (uint32)message[count + 1],
										(uint32)message[count + 2], (uint32)message[count + 3], (uint32)message[count + 4]);
			}

			Panel_TextWindow_Update(panelGlobs.infoTextWnd, infoGlobs.int_6ec, elapsedAbs);
			Info_FUN_0041a180();
			infoGlobs.flags |= Info_GlobFlags::INFO_GLOB_FLAG_UNK_2;
		}
	}

	Panel_TextWindow_Update(panelGlobs.infoTextWnd, infoGlobs.int_6ec, elapsedAbs);
}

// Handles left mouse button press
// <LegoRR.exe @0041a0d0>
bool32 __cdecl LegoRR::Info_Update_FUN_0041a0d0(sint32 mouseX, sint32 mouseY, bool32 leftReleased)
{
	if (infoGlobs.infoMessageCount == 0) {
		return false;
	}

	for (uint32 i = 0; i < infoGlobs.infoMessageCount; i++) {
		const InfoMessage* infoMessage = &infoGlobs.infoMessageTable[i];
		const InfoData* infoData = &infoGlobs.infoDataTable[infoMessage->infoType];

		const Gods98::Image* image = (Gods98::Image*)infoData->ptr_4;

		if (0 <= mouseX && mouseX <= static_cast<sint32>(image->width)) {
			if (infoMessage->next <= mouseY && mouseY <= infoMessage->next + image->height) {
				if (leftReleased) {
					if (((infoGlobs.flags & Info_GlobFlags::INFO_GLOB_FLAG_UNK_1) != Info_GlobFlags::INFO_GLOB_FLAG_NONE) && (i == 0)) {
						Panel_ToggleOpenClosed(Panel_Information);
						infoGlobs.flags &= ~Info_GlobFlags::INFO_GLOB_FLAG_UNK_1;
						return true;
					}

					Info_UpdateMessage(i);
				}
				return true;
			}
		}
	}

	return false;
}

// Works updates whether the info is scrollable
// <LegoRR.exe @0041a180>
void __cdecl LegoRR::Info_FUN_0041a180(void)
{
	uint32 lineCount;
	uint32 lineCapacity;
	Panel_TextWindow_GetInfo(panelGlobs.infoTextWnd, &lineCount, &lineCapacity);

	int availableLines = (int)lineCount - (int)lineCapacity;
	if (availableLines < 0) {
		availableLines = 0;
	}

	ScrollInfo_SetSubStruct28_Fields1C_20(0, availableLines, 0);
	Info_UpdateInt6EC_FromScrollInfo();
}

// Sets the scroll line position
// <LegoRR.exe @0041a1c0>
void __cdecl LegoRR::Info_UpdateInt6EC_FromScrollInfo(void)
{
	sint32 field1c;
	sint32 field20;
	ScrollInfo_GetSubStruct28_Fields1C_20(0, &field1c, &field20);

	infoGlobs.int_6ec = field1c - field20;
}

// <LegoRR.exe @0041a1f0>
void __cdecl LegoRR::Info_FUN_0041a1f0(real32 elapsed)
{
	for (int i = 0; i < Info_Type_Count; i++) {
		infoGlobs.infoDataTable[i].float_c -= elapsed;
	}
}

#pragma endregion
