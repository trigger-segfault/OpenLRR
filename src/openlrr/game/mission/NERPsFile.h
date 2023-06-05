// NERPsFile.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../object/Stats.h"

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct BlockPointer;

#pragma endregion

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef bool32 (__cdecl* NERPsBlockPointerCallback)(BlockPointer* blockPointer, uint32 bx, uint32 by, void* data);

typedef sint32 (__cdecl* NERPsFunction)(sint32* stack);

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define NERPS_FUNCID_STOP		0

#define NERPS_REGISTERCOUNT		8
#define NERPS_TIMERCOUNT		4

#define NERPS_GETLEVELS_ALL		-1
#define NERPS_GETLEVELS_POWERED	-2

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum TutorialFlags : uint32 // [LegoRR/NERPs.c|flags:0x4|type:uint]
{
	TUTORIAL_FLAG_NONE          = 0,
	TUTORIAL_FLAG_NOICONS       = 0x1,
	TUTORIAL_FLAG_NOBLOCKACTION = 0x2, // Tutorial block flashing?
	TUTORIAL_FLAG_NOMAP         = 0x4, // Click on this block to do something? (arrow?, wall highlight?) Used in Level_BlockPointerCheck.
	TUTORIAL_FLAG_NOOBJECTS     = 0x8, // Allow selection? Click on unit on this block? Used in Level_BlockPointerCheck.
	TUTORIAL_FLAG_NORADAR       = 0x10,
	TUTORIAL_FLAG_NOOPTIONS     = 0x20,
	TUTORIAL_FLAG_NOPRIORITIES  = 0x40,
	TUTORIAL_FLAG_NOCALLTOARMS  = 0x80, // Turn off action stations?
	TUTORIAL_FLAG_NOINFO        = 0x100,
	TUTORIAL_FLAG_NOMULTISELECT = 0x200, // No selection box? (multiselect?)
	TUTORIAL_FLAG_NOCYCLEUNITS  = 0x400,
	TUTORIAL_FLAG_NOHELPWINDOW  = 0x800,
	TUTORIAL_FLAG_NOCAMERA      = 0x1000, // No camera controls?

	TUTORIAL_FLAGS_ALL          = 0x1fff,
};
flags_end(TutorialFlags, 0x4);


enum NERPsFunctionArgs : uint32 // [LegoRR/NERPs.c|enum:0x4|type:uint]
{
	NERPS_ARGS_0          = 0,
	NERPS_ARGS_1          = 1,
	NERPS_ARGS_2          = 2,
	NERPS_ARGS_0_NORETURN = 3,
	NERPS_ARGS_1_NORETURN = 4,
	NERPS_ARGS_2_NORETURN = 5,
	NERPS_ARGS_3_NORETURN = 6,
	NERPS_END_OF_LIST     = 7,
};
assert_sizeof(NERPsFunctionArgs, 0x4);


enum NERPsTutorialAction : uint32 // [LegoRR/NERPs.c|enum:0x4|type:uint]
{
	NERPS_TUTORIAL_NONE                            = 0,
	NERPS_TUTORIAL_GETBLOCKISGROUND                = 1,
	NERPS_TUTORIAL_SETBLOCKISGROUND                = 2,
	NERPS_TUTORIAL_GETBLOCKISPATH                  = 3,
	NERPS_TUTORIAL_SETBLOCKISPATH                  = 4,
	NERPS_TUTORIAL_SETCAMERAGOTOTUTORIAL           = 5,
	NERPS_TUTORIAL_GETCAMERAATTUTORIAL             = 6,
	NERPS_TUTORIAL_SETROCKMONSTERATTUTORIAL        = 7,
	NERPS_TUTORIAL_SETCONGREGATIONATTUTORIAL_START = 8,
	NERPS_TUTORIAL_SETCONGREGATIONATTUTORIAL_STOP  = 9,
	NERPS_TUTORIAL_GETUNITATBLOCK                  = 10,
	NERPS_TUTORIAL_SETTUTORIALPOINTER_UNK          = 11,
	NERPS_TUTORIAL_MAKESOMEONEPICKUP               = 12,
	NERPS_TUTORIAL_GETMONSTERATTUTORIAL            = 13,
};
assert_sizeof(NERPsTutorialAction, 0x4);


// <https://kb.rockraidersunited.com/NERPs_documentation>
// NERPs Opcodes are checked using a bitwise AND comparison.
// Load is only performed when none of the other 4 bits are set (see `Mask`).
//
// Opcodes are checked in the order:
//  1) Function
//  2) Operator
//  3) Label
//  4) Jump
//  5) Load
flags_scoped(NERPsOpcode) : uint16
{
	Load       = 0,   // Load literal integer

	Operator   = 0x1, // Everything else...
	Function   = 0x2, // Call hardcoded NERPs function
	Label      = 0x4, // Define label at instruction index (this serves no purpose during code execution, but is handled)
	Jump       = 0x8, // Unconditional jump to instruction index

	Mask       = (Operator|Function|Label|Jump),
};
flags_scoped_end(NERPsOpcode, 0x2);


//{ "+", "#", "/", "\\", "?", ">", "<", "=", ">=", "<=", "!=" };
// Operands for the NERPsOpcode::Operator instruction.
enum class NERPsOperator : uint16
{
	Plus   = 0,  // "+"  Boolean and "&&"
	Pound  = 1,  // "#"  Boolean not "!"
	FSlash = 2,  // "/"  Boolean or  "||"
	BSlash = 3,  // "\\" Unknown, maybe line continuation (has no effect, single character \ )
	Test   = 4,  // "?"  Test conditional (has no effect) if condition "then" action
	Cgt    = 5,  // ">"  Compare greater than
	Clt    = 6,  // "<"  Compare less than
	Ceq    = 7,  // "="  Compare equal
	Cge    = 8,  // ">=" Compare greater or equal
	Cle    = 9,  // "<=" Compare less or equal
	Cne    = 10, // "!=" Compare not equal
};
assert_sizeof(NERPsOperator, 0x2);


// Intermediate enum used during `NERPsRuntime_Execute`. This is not an instruction operand!
// Note that `Boolean not` is not a NERPsComparison type, allowing it to be used in
// combination with binary comparison operators.
enum class NERPsComparison : uint32
{
	None = 0,
	And  = 1,  // "&&" Boolean and
	Or   = 2,  // "||" Boolean or
	Cgt  = 3,  // ">"  Compare greater than
	Clt  = 4,  // "<"  Compare less than
	Ceq  = 5,  // "="  Compare equal
	Cge  = 6,  // ">=" Compare greater or equal
	Cle  = 7,  // "<=" Compare less or equal
	Cne  = 8,  // "!=" Compare not equal
};
assert_sizeof(NERPsComparison, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Typedefs
 **********************************************************************************/

#pragma region Typedefs

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct BlockPointer // [LegoRR/NERPs.c|struct:0xc]
{
	/*0,8*/	Point2I blockPos;
	/*8,4*/	uint32 id;
	/*c*/
};
assert_sizeof(BlockPointer, 0xc);


struct NERPMessageSound // [LegoRR/NERPs.c|struct:0x8]
{
	/*0,4*/	const char* key;
	/*4,4*/	sint32 sound3DHandle;
	/*8*/
};
assert_sizeof(NERPMessageSound, 0x8);


struct NERPMessageImage // [LegoRR/NERPs.c|struct:0x8]
{
	/*0,4*/	const char* key;
	/*4,4*/	Gods98::Image* image;
	/*8*/
};
assert_sizeof(NERPMessageSound, 0x8);


struct NERPsFunctionSignature // [LegoRR/NERPs.c|struct:0xc]
{
	/*0,4*/	const char* name;
	/*4,4*/	NERPsFunction function;
	/*8,4*/	NERPsFunctionArgs arguments;
	/*c*/
};
assert_sizeof(NERPsFunctionSignature, 0xc);


struct NERPsInstruction
{
	/*0,2*/ uint16 operand;
	/*2,2*/ NERPsOpcode opcode;
	/*4*/
};
assert_sizeof(NERPsInstruction, 0x4);


struct SearchNERPsTutorialAction // [LegoRR/search.c|struct:0x2c]
{
	/*00,4*/	NERPsTutorialAction action;
	/*04,4*/	sint32 blockPointerIdx;
	/*08,4*/	sint32 result;
	/*0c,8*/	undefined field_0xc_0x13[8];
	/*14,8*/	Point2I blockPos;
	/*1c,4*/	undefined4 fieldBool_1c;
	/*20,4*/	undefined4 fieldBool_20;
	/*24,4*/	LegoObject* object_24;
	/*28,4*/	sint32 int_28;
	/*2c*/
};
assert_sizeof(SearchNERPsTutorialAction, 0x2c);


struct SearchNERPsSetObjectHealthPain // [LegoRR/search.c|struct:0xc]
{
	/*0,4*/	enum class Type : uint32 { // (0 = runningAway, 1 = painThreshold, 2 = Health)
		GetRunningAway,
		SetPainThreshold,
		SetHealth,
	} type;
	/*4,4*/	uint32 runningAwayCount; // Only for healthType 0 (incremented)
	/*8,4*/	real32 painHealthValue; // Only for healthTypes 1,2 (assigns)
	/*c*/
};
assert_sizeof(SearchNERPsSetObjectHealthPain, 0xc);


struct SearchNERPsCountRecordObjects // [LegoRR/search.c|struct:0x8]
{
	/*0,4*/	uint32 count;
	/*4,4*/	LegoObject* recordObj;
	/*8*/
};
assert_sizeof(SearchNERPsCountRecordObjects, 0x8);


struct SearchNERPsSetMonsterAttack // [LegoRR/search.c|struct:0xc]
{
	/*0,4*/	StatsFlags2 sflags2Exclude; // Fails the search if the target object has any of these flags (and is not 0xffffffff).
	/*4,4*/	StatsFlags2 sflags2Include; // Fails the search if the target object has none of these flags (and is not 0xffffffff).
	/*8,4*/ bool32 stopAttacking; // EXACT BOOL (0 = start attacking, 1 = stop attacking)
	/*c*/
};
assert_sizeof(SearchNERPsSetMonsterAttack, 0xc);


struct NERPsFile_Globs // [LegoRR/NERPs.c|struct:0xb4|tags:GLOBS]
{
	/*00,4*/	bool32 camIsLockedOn;
	/*04,4*/	uint32 camLockOnRecord; // Record object pointer (0-indexed).
	/*08,8*/	Point2F camLockOnPos;
	/*10,4*/	LegoObject* camLockOnObject; // Assigned based on the lock-on object, but never used.
	/*14,4*/	bool32 camIsZooming;
	/*18,4*/	real32 camZoomTotal; // Total amount to zoom specified by NERPs function.
	/*1c,4*/	real32 camZoomMoved; // Amount of the total that has been zoomed. (amount left == total - moved)
	/*20,4*/	bool32 camIsRotating;
	/*24,4*/	real32 camRotTotal; // Total amount to rotate specified by NERPs function.
	/*28,4*/	real32 camRotMoved; // Amount of the total that has been rotated. (amount left == total - moved)
	/*2c,4*/	NERPsInstruction* instructions; // (script fileData)
	/*30,4*/	uint32 scriptSize; // (script fileSize)
	/*34,2c*/	undefined4 reserved1[11];
	/*60,4*/	char* messageBuffer;
	/*64,4*/	uint32 lineCount;
	/*68,4*/	char** lineList;
	/*6c,4*/	uint32 imageCount;
	/*70,4*/	NERPMessageImage* imageList;
	/*74,4*/	uint32 soundCount;
	/*78,4*/	NERPMessageSound* soundList;
	/*7c,24*/	uint32 lineIndexArray_7c[9];
	/*a0,4*/	uint32 uint_a0;
	/*a4,4*/	sint32 int_a4; // (signedness known due to constant comparison)
	/*a8,4*/	uint32 uint_a8;
	/*ac,4*/	sint32 RecordObjectPointer;
	/*b0,4*/	bool32 AdvisorTalkingMode;
	/*b4*/
};
assert_sizeof(NERPsFile_Globs, 0xb4);


struct NERPsRuntime_Globs // [LegoRR/NERPs.c|struct:0x68|tags:GLOBS]
{
	/*00,20*/	sint32 registers[NERPS_REGISTERCOUNT];
	/*20,4*/	bool32 messagePermit; // allows NERPs messages to display in the TextMessage panel. (see: NERPFunc__SetMessagePermit)
	/*24,4*/	undefined4 reserved1;
	/*28,4*/	TutorialFlags tutorialFlags; // (this is the last field in a structure starting at 0x00)
	/*2c,4*/	bool32 timerbool_2c; // used by UpdateTimers
	/*30,4*/	bool32 nextArrowDisabled; // (see: NERPFunc__SetMessage)
	/*34,4*/	real32 messageTimer; // (see: NERPFunc__GetMessageTimer)
	/*38,4*/	sint32 hiddenObjectsFound; // (see: NERPFunc__SetHiddenObjectsFound)
	/*3c,4*/	bool32 messageWait; // (see: NERPFunc__SetMessageWait)
	/*40,4*/	bool32 logFuncCalls; // (always 0)
	/*44,4*/	undefined4 reserved2;
	/*48,10*/	real32 timers[NERPS_TIMERCOUNT];
	/*58,4*/	bool32 supressArrow; // (see: NERPFunc__SupressArrow)
	/*5c,4*/	bool32 allowCameraMovement; // (see: NERPFunc__AllowCameraMovement)
	/*60,4*/	real32 tutorialIconTimer;
	/*64,4*/	bool32 objectiveSwitch; // (see: NERPFunc__SetObjectiveSwitch)
	/*68*/
};
assert_sizeof(NERPsRuntime_Globs, 0x68);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

 // No const so that we can hook functions.
// <LegoRR.exe @004a6948>
extern /*const*/ NERPsFunctionSignature (& c_nerpsFunctions)[294];

// <LegoRR.exe @004a7710>
extern const char* (& c_nerpsOperators)[11]; // = { "+", "#", "/", "\\", "?", ">", "<", "=", ">=", "<=", "!=" };

// <LegoRR.exe @004a773c>
extern bool32 & nerpsHasNextButton; // = true;

// <LegoRR.exe @004a7740>
extern bool32 & nerpsBOOL_004a7740; // = true;

// Not constant
// <LegoRR.exe @004a7748>
extern real32 (& nerpsMessageTimerValues)[3]; // = { 1000.0f, 500.0f, 4000.0f };

/// HARDCODED SCREEN RESOLUTION:
// <LegoRR.exe @004a7758>
extern Point2F & nerpsIconPos; // = { 260.0f, 386.0f };

// <LegoRR.exe @004a7760>
extern sint32 & nerpsIconWidth; // = 20;

// <LegoRR.exe @004a7764>
extern sint32 & nerpsIconSpace; // = 40;

// <LegoRR.exe @004a7768>
extern sint32 & nerpsUnkSampleIndex; // = -1;

// <LegoRR.exe @004a776c>
extern NERPsBlockPointerCallback & c_NERPsRuntime_TutorialActionCallback; // = LegoRR::NERPsRuntime_TutorialActionCallback; // (0x00456fc0)

// <LegoRR.exe @00500958>
extern NERPsRuntime_Globs & nerpsruntimeGlobs;

// <LegoRR.exe @00556d40>
extern NERPsFile_Globs & nerpsfileGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define NERPs_hook_function(name)	LegoRR::NERPs_HookFunction(nameof(name), LegoRR::NERPFunc__## name)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/// CUSTOM: Interop for hooking NERPs functions without replacing the original calls.
bool NERPs_HookFunction(const char* name, NERPsFunction function);


/// CUSTOM: Shorthand for checking if we're in a tutorial.
inline bool NERPs_AnyTutorialFlags() { return nerpsruntimeGlobs.tutorialFlags != TUTORIAL_FLAG_NONE; }
/// CUSTOM: Replacement for always using NERPFunc_GetTurorialFlags
inline TutorialFlags NERPs_GetTutorialFlags() { return nerpsruntimeGlobs.tutorialFlags; }
/// CUSTOM: Replacement for always using NERPFunc_SetTurorialFlags
inline void NERPs_SetTutorialFlags(TutorialFlags tutFlags) { nerpsruntimeGlobs.tutorialFlags = tutFlags; }


// <LegoRR.exe @004530b0>
bool32 __cdecl NERPsFile_LoadScriptFile(const char* filename);

// <LegoRR.exe @00453130>
bool32 __cdecl NERPsFile_LoadMessageFile(const char* filename);

// <LegoRR.exe @004534c0>
char* __cdecl NERPsFile_GetMessageLine(uint32 lineIndex);

// <LegoRR.exe @004534e0>
bool32 __cdecl NERPsFile_Free(void);

// Helper function that converts an instruction for a call with zero arguments into its literal return value.
// <LegoRR.exe @004535a0>
void __cdecl NERPsRuntime_LoadLiteral(IN OUT NERPsInstruction* instruction);

// <LegoRR.exe @004535e0>
void __cdecl NERPsRuntime_Execute(real32 elapsedAbs);


// [NERPFuncs:start]


// <LegoRR.exe @00453bc0>
//#define NERPs_SetHasNextButton ((void (__cdecl* )(bool32 hasNextButton))0x00453bc0)
void __cdecl NERPs_SetHasNextButton(bool32 hasNextButton);

// <LegoRR.exe @00453bd0>
//#define NERPsRuntime_RepeatMessage ((void (__cdecl* )(void))0x00453bd0)
void __cdecl NERPsRuntime_RepeatMessage(void);

// <LegoRR.exe @00453be0>
//#define NERPsRuntime_AdvanceMessage ((void (__cdecl* )(void))0x00453be0)
void __cdecl NERPsRuntime_AdvanceMessage(void);


// [NERPFuncs...]


// <LegoRR.exe @00453e70>
//#define NERPsRuntime_UpdateTimers ((void (__cdecl* )(real32 elapsed))0x00453e70)
void __cdecl NERPsRuntime_UpdateTimers(real32 elapsed);


// [NERPFuncs...]


// <LegoRR.exe @00454060>
//#define NERPsRuntime_EndExecute ((void (__cdecl*)(real32))0x00454060)
void __cdecl NERPsRuntime_EndExecute(real32 elapsedAbs);


// [NERPFuncs...]


// DATA: OUT LegoObject** out_liveObj
// <LegoRR.exe @004542e0>
#define NERPsRuntime_LiveObject_GetIfRockMonsterAnd_FUN_004542e0 ((bool32 (__cdecl* )(LegoObject* liveObj, void* pOutLiveObj))0x004542e0)


// [NERPFuncs...]


// DATA: SearchNERPsSetMonsterAttack* search
// <LegoRR.exe @004545c0>
//#define NERPs_LegoObject_Callback_SetMonsterAttack ((bool32 (__cdecl* )(LegoObject* liveObj, void* pSearch))0x004545c0)
bool32 __cdecl NERPs_LegoObject_Callback_SetMonsterAttack(LegoObject* liveObj, void* pSearch);

// [NERPFuncs...]


// <LegoRR.exe @004546d0>
#define NERPs_SetObjectsLevel ((void (__cdecl* )(const char* objName, uint32 objLevel))0x004546d0)

// DATA: SearchSetObjectsLevel_8c*
// <LegoRR.exe @00454740>
#define NERPs_LiveObject_Callback_SetLevelOfObjects ((bool32 (__cdecl* )(LegoObject* liveObj, void* pSearch))0x00454740)

// DATA: SearchNERPsCountRecordObjects* search
// <LegoRR.exe @00454780>
#define NERPs_LiveObject_Callback_CountRecordObjectsAtBlock ((bool32 (__cdecl* )(BlockPointer* blockPointer, uint32 bx, uint32 by, void* search))0x00454780)


// [NERPFuncs...]


// <LegoRR.exe @004548c0>
#define NERPsRuntime_DrawTutorialIcon ((void (__cdecl* )(Lego_Level* level, Gods98::Viewport* viewMain, real32 elapsedGame, real32 elapsedAbs))0x004548c0)


// [NERPFuncs...]


// <LegoRR.exe @004549d0>
#define NERPs_GetIconClicked ((sint32 (__cdecl* )(Interface_MenuItemType* pMenuIcon))0x004549d0)


// [NERPFuncs...]


// <LegoRR.exe @00454b40>
#define NERPsRuntime_IncHiddenObjectsFound ((void (__cdecl* )(LegoObject_Type unused_objType))0x00454b40)

// <LegoRR.exe @00454b50>
#define NERPsRuntime_ClearHiddenObjectsFound ((void (__cdecl* )(void))0x00454b50)


// [NERPFuncs...]


// DATA: SearchNERPsSetObjectHealthPain* search
// <LegoRR.exe @00454c70>
//#define NERPs_LiveObject_Callback_SetRockMonsterHealthType ((bool32 (__cdecl* )(LegoObject* liveObj, void* pSearch))0x00454c70)
bool32 __cdecl NERPs_LiveObject_Callback_SetRockMonsterHealthType(LegoObject* liveObj, void* pSearch);


// [NERPFuncs...]


// <LegoRR.exe @00454e90>
#define NERPsRuntime_SetSubmenuIconClicked ((sint32 (__cdecl* )(const char* itemName, sint32 clickedCount))0x00454e90)

// <LegoRR.exe @00454ed0>
#define NERPsRuntime_FlashSubmenuIcon ((sint32 (__cdecl* )(const char* objName, bool32 flash))0x00454ed0)

// DATA: sint32* pClickCount
// <LegoRR.exe @00454f10>
#define NERPsRuntime_Enumerate_SetTutorialBlockClicks ((bool32 (__cdecl* )(BlockPointer* blockPointer, uint32 bx, uint32 by, void* pClickCount))0x00454f10)


// [NERPFuncs...]


// DATA: sint32* pCrystalCount
// <LegoRR.exe @00454f60>
#define NERPsRuntime_Enumerate_SetTutorialCrystals ((bool32 (__cdecl* )(BlockPointer* blockPointer, uint32 bx, uint32 by, void* pCrystalCount))0x00454f60)


// [NERPFuncs...]


// DATA: sint32* pGenerateOre
// <LegoRR.exe @00454ff0>
#define NERPs_SetOreAtBlock ((bool32 (__cdecl* )(BlockPointer* blockPointer, uint32 bx, uint32 by, void* pGenerateOre))0x00454ff0)


// [NERPFuncs...]


// <LegoRR.exe @00455070>
#define NERPsRuntime_CountSelectedUnits_ByObjectName ((sint32 (__cdecl* )(const char* objName))0x00455070)


// [NERPFuncs...]


// <LegoRR.exe @00455320>
#define NERPs_Game_DoCallbacks_ByObjectName ((bool32 (__cdecl* )(const char* objName))0x00455320)

// <LegoRR.exe @00455360>
#define NERPs_LiveObject_Callback_ByObjectTypeIndex_IsDriving ((bool32 (__cdecl* )(LegoObject* liveObj1, LegoObject* liveObj_template))0x00455360)


// [NERPFuncs...]


// DATA: sint32* tutorialCrystals
// <LegoRR.exe @004553f0>
#define NERPsRuntime_Callback_GetTutorialCrystals ((bool32 (__cdecl* )(BlockPointer* blockPointer, uint32 bx, uint32 by, OUT void* pTutorialCrystals))0x004553f0)


// [NERPFuncs...]


// DATA: sint32* clicks
// <LegoRR.exe @00455450>
#define NERPsRuntime_Callback_GetTutorialBlockClicks ((bool32 (__cdecl* )(BlockPointer* blockPointer, uint32 bx, uint32 by, OUT void* pClicks))0x00455450)


// [NERPFuncs...]


// <LegoRR.exe @004554b0>
#define NERPsRuntime_GetLevelObjectsBuilt ((sint32 (__cdecl* )(const char* objName, sint32 objLevel))0x004554b0)

// DATA: SearchObjectTypeCount_c*
// <LegoRR.exe @00455580>
#define NERPsRuntime_LiveObjectCallback_GetPoweredObjectsBuilt ((bool32 (__cdecl* )(LegoObject* liveObj, void* search))0x00455580)

// <LegoRR.exe @004555c0>
#define NERPsRuntime_GetPreviousLevelObjectsBuilt ((uint32 (__cdecl* )(const char* objName, sint32 objLevel))0x004555c0)

// <LegoRR.exe @00455600>
#define NERPs_SubMenu_GetBuildingVehicleIcon_ByObjectName ((sint32 (__cdecl* )(const char* objName))0x00455600)

// <LegoRR.exe @00455630>
#define NERPsRuntime_FlashIcon ((bool32 (__cdecl* )(Interface_MenuItemType menuIcon, bool32 flash))0x00455630)


// [NERPFuncs...]


// <LegoRR.exe @00456900>
#define NERPsRuntime_IsMessagePermit ((bool32 (__cdecl* )(void))0x00456900)

// <LegoRR.exe @00456910>
#define NERPsRuntime_GetMessageWait ((bool32 (__cdecl* )(void))0x00456910)


// [NERPFuncs...]


// <LegoRR.exe @00456ad0>
#define NERPs_RestartMessageSample ((void (__cdecl* )(void))0x00456ad0)

// text is the raw input line (which usually has `#soundName#` and `<imageName>` etc.)
// buffer is the stripped output line without the `#soundName#` or `<imageKey>` markers.
// <LegoRR.exe @00456af0>
//#define NERPs_Level_NERPMessage_Parse ((void (__cdecl* )(const char* text, OPTIONAL OUT char* buffer, bool32 updateTimer))0x00456af0)
void __cdecl NERPs_Level_NERPMessage_Parse(const char* text, OPTIONAL OUT char* buffer, bool32 updateTimer);

// <LegoRR.exe @00456e40>
#define NERPs_InitBlockPointersTable ((void (__cdecl* )(Lego_Level* level))0x00456e40)

// <LegoRR.exe @00456ef0>
#define NERPs_GetBlockPointer ((BlockPointer* (__cdecl* )(Lego_Level* level, sint32 blockPointerID))0x00456ef0)

// <LegoRR.exe @00456f20>
#define NERPs_FreeBlockPointers ((void (__cdecl* )(void))0x00456f20)

// <LegoRR.exe @00456f70>
#define NERPsRuntime_EnumerateBlockPointers ((void (__cdecl* )(sint32 blockPointerID, NERPsBlockPointerCallback callback, void* data))0x00456f70)

// DATA: SearchNERPsTutorialAction* search
// <LegoRR.exe @00456fc0>
//#define NERPsRuntime_TutorialActionCallback ((bool32 (__cdecl* )(BlockPointer* unused, uint32 bx, uint32 by, void* pSearch))0x00456fc0)
bool32 __cdecl NERPsRuntime_TutorialActionCallback(BlockPointer* unused, uint32 bx, uint32 by, void* pSearch);

// DATA: SearchNERPsTutorialAction* search
// <LegoRR.exe @00457320>
#define NERPs_LiveObject_CallbackCheck_FUN_00457320 ((bool32 (__cdecl* )(LegoObject* liveObj, void* pSearch))0x00457320)

// DATA: SearchNERPsTutorialAction* search
// <LegoRR.exe @00457390>
#define NERPs_LiveObject_Callback_SetBool3f8IfAtBlockPos_FUN_00457390 ((bool32 (__cdecl* )(LegoObject* liveObj, void* pSearch))0x00457390)


// [NERPFuncs...]


// <LegoRR.exe @00457430>
#define NERPsRuntime_SetTutorialPointer ((void (__cdecl* )(sint32 blockPointerIdx, sint32 unkMode0_1))0x00457430)


// [NERPFuncs:end] (except for NERPFunc_False/True(?), which is merged everywhere)

#pragma endregion

}
