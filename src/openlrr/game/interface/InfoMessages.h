// InfoMessages.h : 
//

#pragma once

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct InfoMessageInstance;

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#pragma endregion

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef bool32 (__cdecl* InfoEnumerateCallback)(InfoMessageInstance* instance, void* data);

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum InfoDataFlags : uint32 // [LegoRR/Info.c|flags:0x4|type:uint]
{
	INFOTYPE_FLAG_NONE            = 0,
	INFOTYPE_FLAG_CHANGEGAMESPEED = 0x10000,
	INFOTYPE_FLAG_UNK_20000       = 0x20000,
};
flags_end(InfoDataFlags, 0x4);


enum Info_GlobFlags : uint32 // [LegoRR/Info.c|flags:0x4|type:uint]
{
	INFO_GLOB_FLAG_NONE          = 0,
	INFO_GLOB_FLAG_UNK_1         = 0x1,
	INFO_GLOB_FLAG_UNK_2         = 0x2,
	INFO_GLOB_FLAG_UNK_4         = 0x4,
	INFO_GLOB_FLAG_UNK_8         = 0x8,
	INFO_GLOB_FLAG_AUTOGAMESPEED = 0x10,
};
flags_end(Info_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct InfoData // [LegoRR/Info.c|struct:0x14]
{
	/*00,4*/	char* text;
	/*04,4*/	void* ptr_4; // struct size of >= 0xc (int field_4, inf field_8)
	/*08,4*/	SFX_ID sfxType;
	/*0c,4*/	real32 float_c;
	/*10,4*/	InfoDataFlags flags;
	/*14*/
};
assert_sizeof(InfoData, 0x14);


struct InfoMessageInstance // [LegoRR/Info.c|struct:0x14]
{
	/*00,8*/	Point2I blockPos; // (-1, -1) for NULL blockPos
	/*08,4*/	LegoObject* object;
	/*0c,4*/	char* text;
	/*10,4*/	InfoMessageInstance* next;
	/*14*/
};
assert_sizeof(InfoMessageInstance, 0x14);


struct InfoMessage // [LegoRR/Info.c|struct:0x14]
{
	/*00,4*/	InfoMessageInstance* instance;
	/*04,4*/	uint32 instanceCount;
	/*08,4*/	Info_Type infoType;
	/*0c,4*/	real32 float_c;
	/*10,4*/	real32 next;
	/*14*/
};
assert_sizeof(InfoMessage, 0x14);


struct Info_Globs // [LegoRR/Info.c|struct:0x6f4|tags:GLOBS]
{
	/*000,a0*/	const char* infoName[Info_Type_Count];
	/*0a0,320*/	InfoData infoDataTable[Info_Type_Count];
	/*3c0,320*/	InfoMessage infoMessageTable[Info_Type_Count];
	/*6e0,4*/	uint32 infoMessageCount;
	/*6e4,4*/	Gods98::Font* font;
	/*6e8,4*/	Gods98::Image* OverFlowImage;
	/*6ec,4*/	sint32 int_6ec; // To do with the scroll position of the info message text
	/*6f0,4*/	Info_GlobFlags flags;
	/*6f4*/
};
assert_sizeof(Info_Globs, 0x6f4);


struct SearchInfoObject_8 // [LegoRR/search.c|struct:0x8]
{
	/*0,4*/	LegoObject* object;
	/*4,4*/	uint32 index;
	/*8*/
};
assert_sizeof(SearchInfoObject_8, 0x8);


struct SearchInfoBlockPos_8 // [LegoRR/search.c|struct:0x8]
{
	/*0,4*/	const Point2I* pBlockPos;
	/*4,4*/	uint32 index;
	/*8*/
};
assert_sizeof(SearchInfoBlockPos_8, 0x8);


struct SearchInfoObjectBlockPos_8 // [LegoRR/search.c|struct:0x8]
{
	/*0,4*/	LegoObject* optObject;
	/*4,4*/	const Point2I* optBlockPos;
	/*8*/
};
assert_sizeof(SearchInfoObjectBlockPos_8, 0x8);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004dd658>
extern Info_Globs & infoGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define Info_RegisterName(n)		(infoGlobs.infoName[n]=#n)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <inlined, unused>
__inline bool32 Info_IsAutoGameSpeed(void) { return (infoGlobs.flags & INFO_GLOB_FLAG_AUTOGAMESPEED); }



// <LegoRR.exe @00419160>
//#define Info_Initialise ((void (__cdecl* )(Gods98::Font* font))0x00419160)
void __cdecl Info_Initialise(Gods98::Font* font);

// <LegoRR.exe @00419310>
//#define Info_GetInfoType ((bool32 (__cdecl* )(const char* infoName, OUT Info_Type* infoType))0x00419310)
bool32 __cdecl Info_GetInfoType(const char* infoName, OUT Info_Type* infoType);

// <LegoRR.exe @00419350>
//#define Info_GetTypePtr4 ((void* (__cdecl* )(Info_Type infoType))0x00419350)
void* __cdecl Info_GetTypePtr4(Info_Type infoType);

// <LegoRR.exe @00419360>
//#define Info_SetOverFlowImageFile ((void (__cdecl* )(const char* filename))0x00419360)
void __cdecl Info_SetOverFlowImageFile(const char* filename);

// <LegoRR.exe @00419380>
//#define Info_SetAutoGameSpeed ((void (__cdecl* )(bool32 autoOn))0x00419380)
void __cdecl Info_SetAutoGameSpeed(bool32 autoOn);

// <LegoRR.exe @004193a0>
//#define Info_SetTypeChangeGameSpeed ((void (__cdecl* )(Info_Type infoType, bool32 changeSpeedOn))0x004193a0)
void __cdecl Info_SetTypeChangeGameSpeed(Info_Type infoType, bool32 changeSpeedOn);

// <LegoRR.exe @004193e0>
//#define Info_SetTypeText ((void (__cdecl* )(Info_Type infoType, const char* text))0x004193e0)
void __cdecl Info_SetTypeText(Info_Type infoType, const char* text);

// <LegoRR.exe @00419420>
//#define Info_SetTypeFlag_20000 ((void (__cdecl* )(Info_Type infoType, bool32 setFlag20000))0x00419420)
void __cdecl Info_SetTypeFlag_20000(Info_Type infoType, bool32 setFlag20000);

// <LegoRR.exe @00419460>
//#define Info_SetText_internal ((void (__cdecl* )(const char* text, char** pInfoText))0x00419460)
void __cdecl Info_SetText_internal(const char* text, char** pInfoText);

// <LegoRR.exe @00419580>
//#define Info_SetTypeImageFile ((void (__cdecl* )(Info_Type infoType, const char* filename))0x00419580)
void __cdecl Info_SetTypeImageFile(Info_Type infoType, const char* filename);

// <LegoRR.exe @004195b0>
//#define Info_SetTypeSFX ((void (__cdecl* )(Info_Type infoType, SFX_ID sfxID))0x004195b0)
void __cdecl Info_SetTypeSFX(Info_Type infoType, SFX_ID sfxID);

// <LegoRR.exe @004195d0>
//#define Info_EnumerateMessageInstances ((bool32 (__cdecl* )(sint32 handle, InfoEnumerateCallback callback, void* data))0x004195d0)
bool32 __cdecl Info_EnumerateMessageInstances(sint32 handle, InfoEnumerateCallback callback, void* data);

// <LegoRR.exe @00419620>
//#define Info_AddMessageInstance ((void (__cdecl* )(sint32 handle, InfoMessageInstance* instance))0x00419620)
void __cdecl Info_AddMessageInstance(sint32 handle, InfoMessageInstance* instance);

// <LegoRR.exe @004196b0>
//#define Info_GetMessageInstance ((InfoMessageInstance* (__cdecl* )(sint32 handle, sint32 instanceIndex))0x004196b0)
InfoMessageInstance* __cdecl Info_GetMessageInstance(sint32 handle, sint32 instanceIndex);

// <LegoRR.exe @004196e0>
//#define Info_RemoveMessageInstance ((InfoMessageInstance* (__cdecl* )(sint32 handle, sint32 instanceIndex))0x004196e0)
InfoMessageInstance* __cdecl Info_RemoveMessageInstance(sint32 handle, sint32 instanceIndex);

// Finds the index of the InfoMessageInstance that references the specified object.
// DATA: SearchInfoObject_8*
// <LegoRR.exe @00419740>
//#define Info_Callback_FindObjectReference ((bool32 (__cdecl* )(InfoMessageInstance* instance, void* search))0x00419740)
bool32 __cdecl Info_Callback_FindObjectReference(InfoMessageInstance* instance, void* search);

// Removes all info messages referencing the specified object.
// <LegoRR.exe @00419760>
//#define Info_RemoveObjectReferences ((void (__cdecl* )(LegoObject* liveObj))0x00419760)
void __cdecl Info_RemoveObjectReferences(LegoObject* liveObj);

// DATA: SearchInfoBlockPos_8*
// <LegoRR.exe @004197f0>
//#define Info_Callback_FindBlockPos ((bool32 (__cdecl* )(InfoMessageInstance* infoInstance, void* search))0x004197f0)
bool32 __cdecl Info_Callback_FindBlockPos(InfoMessageInstance* infoInstance, void* search);

// <LegoRR.exe @00419820>
//#define Info_RemoveAllAtBlockPos ((void (__cdecl* )(const Point2I* blockPos))0x00419820)
void __cdecl Info_RemoveAllAtBlockPos(const Point2I* blockPos);

// DATA: SearchInfoObjectBlockPos_8*
// <LegoRR.exe @004198d0>
//#define Info_Callback_FindObjectAndBlockPos ((bool32 (__cdecl* )(InfoMessageInstance* instance, void* search))0x004198d0)
bool32 __cdecl Info_Callback_FindObjectAndBlockPos(InfoMessageInstance* instance, void* search);

// <LegoRR.exe @00419920>
//#define Info_HasTypeAtObjectOrBlockPos ((bool32 (__cdecl* )(Info_Type infoType, OPTIONAL LegoObject* liveObj, OPTIONAL const Point2I* blockPos))0x00419920)
bool32 __cdecl Info_HasTypeAtObjectOrBlockPos(Info_Type infoType, OPTIONAL LegoObject* liveObj, OPTIONAL const Point2I* blockPos);

// <LegoRR.exe @004199b0>
//#define Info_CreateInstance ((InfoMessageInstance* (__cdecl* )(OPTIONAL const Point2I* blockPos, OPTIONAL LegoObject* liveObj, OPTIONAL const char* text))0x004199b0)
InfoMessageInstance* __cdecl Info_CreateInstance(OPTIONAL const Point2I* blockPos, OPTIONAL LegoObject* liveObj, OPTIONAL const char* text);

// <LegoRR.exe @00419a10>
//#define Info_SetFlag4 ((void (__cdecl* )(bool32 state))0x00419a10)
void __cdecl Info_SetFlag4(bool32 state);

// <LegoRR.exe @00419a30>
//#define Info_HasTypeText ((bool32 (__cdecl* )(Info_Type infoType))0x00419a30)
bool32 __cdecl Info_HasTypeText(Info_Type infoType);

// <LegoRR.exe @00419a50>
//#define Info_FindExistingMessageType ((sint32 (__cdecl* )(Info_Type infoType))0x00419a50)
sint32 __cdecl Info_FindExistingMessageType(Info_Type infoType);

// <LegoRR.exe @00419a80>
//#define Info_FUN_00419a80 ((real32 (__cdecl* )(void))0x00419a80)
real32 __cdecl Info_FUN_00419a80(void);

// <LegoRR.exe @00419ab0>
//#define Info_Send ((void (__cdecl* )(Info_Type infoType, OPTIONAL const char* opt_text, OPTIONAL LegoObject* liveObj, OPTIONAL const Point2I* blockPos))0x00419ab0)
void __cdecl Info_Send(Info_Type infoType, OPTIONAL const char* opt_text, OPTIONAL LegoObject* liveObj, OPTIONAL const Point2I* blockPos);

// <LegoRR.exe @00419cd0>
//#define Info_GotoFirst ((void (__cdecl* )(void))0x00419cd0)
void __cdecl Info_GotoFirst(void);

// <LegoRR.exe @00419d10>
//#define Info_UpdateMessage ((void (__cdecl* )(uint32 handle))0x00419d10)
void __cdecl Info_UpdateMessage(uint32 handle);

// <LegoRR.exe @00419d90>
//#define Info_PopFirstMessage ((void (__cdecl* )(void))0x00419d90)
void __cdecl Info_PopFirstMessage(void);

// <LegoRR.exe @00419db0>
//#define Info_RemoveMessage ((void (__cdecl* )(uint32 handle))0x00419db0)
void __cdecl Info_RemoveMessage(uint32 handle);

// <LegoRR.exe @00419e40>
//#define Info_ClearAllMessages ((void (__cdecl* )(void))0x00419e40)
void __cdecl Info_ClearAllMessages(void);

// <LegoRR.exe @00419e60>
//#define Info_Draw ((void (__cdecl* )(real32 elapsedAbs))0x00419e60)
void __cdecl Info_Draw(real32 elapsedAbs);

// <LegoRR.exe @00419fb0>
//#define Info_DrawPanel ((void (__cdecl* )(real32 elapsedAbs))0x00419fb0)
void __cdecl Info_DrawPanel(real32 elapsedAbs);

// <LegoRR.exe @0041a0d0>
//#define Info_Update_FUN_0041a0d0 ((bool32 (__cdecl* )(sint32 mouseX, sint32 mouseY, bool32 leftReleased))0x0041a0d0)
bool32 __cdecl Info_Update_FUN_0041a0d0(sint32 mouseX, sint32 mouseY, bool32 leftReleased);

// <LegoRR.exe @0041a180>
//#define Info_FUN_0041a180 ((void (__cdecl* )(void))0x0041a180)
void __cdecl Info_FUN_0041a180(void);

// <LegoRR.exe @0041a1c0>
//#define Info_UpdateInt6EC_FromScrollInfo ((void (__cdecl* )(void))0x0041a1c0)
void __cdecl Info_UpdateInt6EC_FromScrollInfo(void);

// <LegoRR.exe @0041a1f0>
//#define Info_FUN_0041a1f0 ((void (__cdecl* )(real32 elapsed))0x0041a1f0)
void __cdecl Info_FUN_0041a1f0(real32 elapsed);

#pragma endregion

}
