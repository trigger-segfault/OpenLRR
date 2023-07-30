// ObjectRecall.cpp : 
//

#include "ObjectRecall.h"
#include "..\..\game\mission\Objective.h"
#include "..\..\game\object\Object.h"
#include "..\..\game\object\Stats.h"
#include "..\..\engine\core\Files.h"

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00500e50>
LegoRR::ObjectRecall_Globs & LegoRR::objectRecallGlobs = *(LegoRR::ObjectRecall_Globs*)0x00500e50;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00459450>
void __cdecl LegoRR::ObjectRecall_StoreMiniFigure(const LegoObject* liveObj)
{

	//Check type is correct, ability flags are set, and name is custom
	if (liveObj->type == LegoObject_MiniFigure && (liveObj->abilityFlags != ABILITY_FLAG_NONE || liveObj->objLevel != 0 || liveObj->customName != nullptr)) {
		if (objectRecallGlobs.recallUsed == objectRecallGlobs.recallCapacity) {
			/* More space is needed, allocate 10 extra slots */
			objectRecallGlobs.recallCapacity = objectRecallGlobs.recallCapacity + 10;
			objectRecallGlobs.recallList = (ObjectRecallEntry*) Gods98::Mem_ReAlloc(objectRecallGlobs.recallList,objectRecallGlobs.recallCapacity * sizeof(ObjectRecallEntry));
		}
		//Grab latest object in list, set flags and strings
		objectRecallGlobs.recallList[objectRecallGlobs.recallUsed].abilityFlags = liveObj->abilityFlags;
		objectRecallGlobs.recallList[objectRecallGlobs.recallUsed].level = liveObj->objLevel;
		std::memset(objectRecallGlobs.recallList[objectRecallGlobs.recallUsed].customName, 0, OBJECT_CUSTOMNAMELENGTH);
		if (liveObj->customName != nullptr) {
			std::strncpy(objectRecallGlobs.recallList[objectRecallGlobs.recallUsed].customName, liveObj->customName, OBJECT_MAXCUSTOMNAMECHARS);
		}

		//Move to next used object
		objectRecallGlobs.recallUsed++;
	}
}

// <LegoRR.exe @00459500>
bool32 __cdecl LegoRR::ObjectRecall_RecallMiniFigure(LegoObject* liveObj)
{
	if ((liveObj->type == LegoObject_MiniFigure) && (objectRecallGlobs.recallUsed != 0)) {
		//Go down a index since we recalled
		objectRecallGlobs.recallUsed--;
		/* Trained ability flags */
		liveObj->abilityFlags = objectRecallGlobs.recallList[objectRecallGlobs.recallUsed].abilityFlags;
		StatsObject_SetObjectLevel(liveObj, objectRecallGlobs.recallList[objectRecallGlobs.recallUsed].level);
		LegoObject_SetCustomName(liveObj, objectRecallGlobs.recallList[objectRecallGlobs.recallUsed].customName);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// <LegoRR.exe @00459560>
void __cdecl LegoRR::ObjectRecall_Save_FreeObjectRecall(void)
{
	//Clear recalled objects and free memory
	objectRecallGlobs.recallUsed = 0;
	objectRecallGlobs.recallCapacity = 0;
	if (objectRecallGlobs.recallList != nullptr) {
		Gods98::Mem_Free(objectRecallGlobs.recallList);
	}
	//Wipe list
	objectRecallGlobs.recallList = nullptr;
}

// <LegoRR.exe @004595a0>
void __cdecl LegoRR::ObjectRecall_Save_CopyToNewObjectRecallData(void)
{
	//If recalling and we have info in our list
	if ((objectRecallGlobs.recallUsed != 0) && (objectRecallGlobs.recallList != nullptr)) {
		//Free list if it was alloced from a previous call
		if (objectRecallGlobs.recallNewList != nullptr) {
			Gods98::Mem_Free(objectRecallGlobs.recallNewList);
		}
		//Alloc memory to new list
		objectRecallGlobs.recallNewList = (ObjectRecallEntry*)Gods98::Mem_Alloc(objectRecallGlobs.recallUsed * sizeof(ObjectRecallEntry));
		//Copy that data to the new list
		std::memcpy(objectRecallGlobs.recallNewList, objectRecallGlobs.recallList, objectRecallGlobs.recallUsed * sizeof(ObjectRecallEntry));
		//Update current recall count to latest used object, then set loaded flag
		objectRecallGlobs.recallNewCount = objectRecallGlobs.recallUsed;
		objectRecallGlobs.loaded = TRUE;
	}
}

// <LegoRR.exe @00459620>
bool32 __cdecl LegoRR::ObjectRecall_Save_CreateNewObjectRecall(void)
{
	//Save current objects
	ObjectRecall_Save_FreeObjectRecall();
	//If new recall list exists and we have things to recall
	if (objectRecallGlobs.recallNewList != nullptr && objectRecallGlobs.recallNewCount != 0) {
		//Setup memory for new list and assign
		objectRecallGlobs.recallList = (ObjectRecallEntry*)Gods98::Mem_Alloc(objectRecallGlobs.recallNewCount * sizeof(ObjectRecallEntry));
		std::memcpy(objectRecallGlobs.recallList, objectRecallGlobs.recallNewList, objectRecallGlobs.recallNewCount * sizeof(ObjectRecallEntry));
		objectRecallGlobs.recallUsed = objectRecallGlobs.recallNewCount;
		objectRecallGlobs.recallCapacity = objectRecallGlobs.recallNewCount;
		return TRUE;
	}
	else {
		return FALSE;
	}
}
// <LegoRR.exe @00459690>
bool32 __cdecl LegoRR::ObjectRecall_IsLoaded(void)
{
	return objectRecallGlobs.loaded;
}

// <LegoRR.exe @004596a0>
bool32 __cdecl LegoRR::ObjectRecall_SaveRROSFile(const char* filename)
{
	Gods98::File* f;

	//Attempt to open file for writing
	f =  Gods98::File_Open(filename, "wb");
	if (f != nullptr) {
		//Create file header and copy over info
		RROSFileHeader hdr;
		std::strncpy(hdr.signature, "RROS", 4);
		hdr.count = objectRecallGlobs.recallUsed;
		/* Write Header:
		   char[4] "RROS"
		   uint    count */
		Gods98::File_Write(&hdr, 4, 2, f);
		/* Write Payload:
		   RROSStruct14[count] data */
		Gods98::File_Write(objectRecallGlobs.recallList, sizeof(ObjectRecallEntry), objectRecallGlobs.recallUsed, f);
		Gods98::File_Close(f);
		ObjectRecall_Save_CopyToNewObjectRecallData();
		objectRecallGlobs.loaded = FALSE;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// <LegoRR.exe @00459730>
bool32 __cdecl LegoRR::ObjectRecall_LoadRROSFile(const char* filename)
{
	Gods98::File* f;

	ObjectRecall_Save_FreeObjectRecall();
	f = Gods98::File_Open(filename, "rb");
	if (f != nullptr) {

		//Read in file header
		RROSFileHeader hdr;
		Gods98::File_Read(&hdr, 4, 2, f);
		//See if RROS string there and then process according to:
		/* Read Header:
		char[4] "RROS"
		uint    count */
		if (std::strncmp(hdr.signature, "RROS", 4) == 0){
			if (Gods98::File_Length(f) - 8U == hdr.count * sizeof(ObjectRecallEntry)) {
				//Copy data from header into recall object
				objectRecallGlobs.recallUsed = hdr.count;
				objectRecallGlobs.recallCapacity = hdr.count;
				objectRecallGlobs.recallList = (ObjectRecallEntry*)Gods98::Mem_Alloc(hdr.count * sizeof(ObjectRecallEntry));
				Gods98::File_Read(objectRecallGlobs.recallList, sizeof(ObjectRecallEntry), objectRecallGlobs.recallUsed, f);
				//Save data to a new objet and indicate the object needs loading
				ObjectRecall_Save_CopyToNewObjectRecallData();
				objectRecallGlobs.loaded = FALSE;
			}
		}
		//Close file and return passing
		Gods98::File_Close(f);
		return TRUE;
	}
	else {
		return FALSE;
	}
}


#pragma endregion
