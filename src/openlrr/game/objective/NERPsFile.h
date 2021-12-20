// NERPsFile.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

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

#define NERPS_FUNCID_STOP 0

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum TutorialFlags : uint32 // [LegoRR/NERPs.c|flags:0x4|type:uint]
{
	TUTORIAL_NONE     = 0,
	TUTORIAL_UNK_2    = 0x2,
	TUTORIAL_UNK_4    = 0x4,
	TUTORIAL_UNK_8    = 0x8,
	TUTORIAL_UNK_10   = 0x10,
	TUTORIAL_UNK_20   = 0x20,
	TUTORIAL_UNK_40   = 0x40,
	TUTORIAL_UNK_80   = 0x80,
	TUTORIAL_UNK_100  = 0x100,
	TUTORIAL_UNK_200  = 0x200,
	TUTORIAL_UNK_400  = 0x400,
	TUTORIAL_UNK_800  = 0x800,
	TUTORIAL_UNK_1000 = 0x1000,
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



flags_scoped(NERPsOpcode) : uint16
{
	Load       = 0, // Load literal integer
	Operator   = 1, // Everything else...
	Function   = 2, // Call hardcoded NERPs function
	Label      = 4, // Define label at instruction index (this serves no purpose during code execution, but is handled)
	Jump       = 8, // Unconditional jump to instruction index
	//NERPS_OPCODE_LITERAL = 0,
	//NERPS_OPCODE_COMPARISON = 1,
	//NERPS_OPCODE_CALL = 2,
	//NERPS_OPCODE_LABEL = 4,
	//NERPS_OPCODE_GOTO = 8,
};
flags_scoped_end(NERPsOpcode, 0x2);


//{ "+", "#", "/", "\\", "?", ">", "<", "=", ">=", "<=", "!=" };
enum class NERPsOperator : uint16
{
	Plus   = 0,  // "+"  Addition(???)
	Pound  = 1,  // "#"  (???)
	FSlash = 2,  // "/"  Divide(???)
	BSlash = 3,  // "\\" (1 character)
	Test   = 4,  // "?"  (expression "?" then action)
	Cgt    = 5,  // ">"  Compare greater than
	Clt    = 6,  // "<"  Compare less than
	Ceq    = 7,  // "="  Compare equal
	Cge    = 8,  // ">=" Compare greater or equal
	Cle    = 9,  // "<=" Compare less or equal
	Cne    = 10, // "!=" Compare not equal
};
assert_sizeof(NERPsOperator, 0x2);


// Intermediate enum used during NERPsRuntime_Execute, not in the instruction format.
enum class NERPsComparison : uint32
{
	None = 0,
	And  = 1,  // "&&" Logical AND
	Or   = 2,  // "||" Logical OR
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
	/*0,4*/	char* key;
	/*4,4*/	sint32 sampleIndex;
	/*8*/
};
assert_sizeof(NERPMessageSound, 0x8);


struct NERPsFunctionSignature // [LegoRR/NERPs.c|struct:0xc]
{
	/*0,4*/	char* name;
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


struct NERPsFile_Globs // [LegoRR/NERPs.c|struct:0xb4|tags:GLOBS]
{
	/*00,4*/	bool32 Camera_IsLockedOn;
	/*04,4*/	LegoObject* Camera_LockedOnTarget;
	/*08,8*/	Point2I pointi_8;
	/*10,4*/	LegoObject* object_10;
	/*14,4*/	bool32 bool_14;
	/*18,4*/	real32 float_18;
	/*1c,4*/	real32 float_1c;
	/*20,4*/	bool32 bool_20;
	/*24,4*/	real32 float_24;
	/*28,4*/	real32 float_28;
	/*2c,4*/	NERPsInstruction* instructions; // void* fileData;
	/*30,4*/	uint32 fileSize;
	/*34,2c*/	undefined4 reserved1[11];
	/*60,4*/	char* messageBuffer;
	/*64,4*/	uint32 messageLineCount;
	/*68,4*/	char** messageLineList;
	/*6c,4*/	uint32 messageCount;
	/*70,4*/	void* messageList;
	/*74,4*/	uint32 soundCount;
	/*78,4*/	NERPMessageSound* soundList;
	/*7c,4*/	undefined4 soundsUNKCOUNT;
	/*80,4*/	undefined4 field_80;
	/*84,4*/	undefined4 field_84;
	/*88,4*/	undefined4 field_88;
	/*8c,4*/	undefined4 field_8c;
	/*90,4*/	undefined4 field_90;
	/*94,4*/	undefined4 field_94;
	/*98,4*/	undefined4 field_98;
	/*9c,4*/	undefined4 field_9c;
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
	/*00,20*/	sint32 registers[8];
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
	/*48,10*/	real32 timers[4];
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

// <LegoRR.exe @004a6948>
extern const NERPsFunctionSignature (& c_nerpsFunctions)[294];

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


#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

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

#pragma endregion

}
