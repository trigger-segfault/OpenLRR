// NERPsFile.cpp : 
//

#include "../../engine/audio/3DSound.h"
#include "../../engine/core/Files.h"
#include "../../engine/core/Maths.h"
#include "../../engine/core/Memory.h"
#include "../../engine/core/Utils.h"
#include "../../engine/gfx/Containers.h"
#include "../../legacy.h"

#include "NERPsFile.h"


#define Text_Clear ((void (__cdecl*)(void))0x0046ad50)
#define NERPFunc__SetMessagePermit ((sint32 (__cdecl*)(sint32*))0x004568b0)
#define NERPsRuntime_EndExecute ((void (__cdecl*)(real32))0x00454060)


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @>
//LegoRR::_Globs & LegoRR::Globs = *(LegoRR::_Globs*)0x;


// <LegoRR.exe @004a6948>
const LegoRR::NERPsFunctionSignature (& LegoRR::c_nerpsFunctions)[294] = *(LegoRR::NERPsFunctionSignature(*)[294])0x004a6948;

// <LegoRR.exe @004a7710>
const char* (& LegoRR::c_nerpsOperators)[11] = *(const char*(*)[11])0x004a7710; // = { "+", "#", "/", "\\", "?", ">", "<", "=", ">=", "<=", "!=" };

// <LegoRR.exe @004a773c>
bool32 & LegoRR::nerpsHasNextButton = *(bool32*)0x004a773c; // = true;

// <LegoRR.exe @004a7740>
bool32 & LegoRR::nerpsBOOL_004a7740 = *(bool32*)0x004a7740; // = true;

// Not constant
// <LegoRR.exe @004a7748>
real32 (& LegoRR::nerpsMessageTimerValues)[3] = *(real32(*)[3])0x004a7748; // = { 1000.0f, 500.0f, 4000.0f };

/// HARDCODED SCREEN RESOLUTION:
// <LegoRR.exe @004a7758>
Point2F & LegoRR::nerpsIconPos = *(Point2F*)0x004a7758; // = { 260.0f, 386.0f };

// <LegoRR.exe @004a7760>
sint32 & LegoRR::nerpsIconWidth = *(sint32*)0x004a7760; // = 20;

// <LegoRR.exe @004a7764>
sint32 & LegoRR::nerpsIconSpace = *(sint32*)0x004a7764; // = 40;

// <LegoRR.exe @004a7768>
sint32 & LegoRR::nerpsUnkSampleIndex = *(sint32*)0x004a7768; // = -1;

// <LegoRR.exe @004a776c>
LegoRR::NERPsBlockPointerCallback & LegoRR::c_NERPsRuntime_TutorialActionCallback = *(LegoRR::NERPsBlockPointerCallback*)0x004a776c; // = LegoRR::NERPsRuntime_TutorialActionCallback; // (0x00456fc0)

// <LegoRR.exe @00500958>
LegoRR::NERPsRuntime_Globs & LegoRR::nerpsruntimeGlobs = *(LegoRR::NERPsRuntime_Globs*)0x00500958;

// <LegoRR.exe @00556d40>
LegoRR::NERPsFile_Globs & LegoRR::nerpsfileGlobs = *(LegoRR::NERPsFile_Globs*)0x00556d40;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @004530b0>
bool32 __cdecl LegoRR::NERPsFile_LoadScriptFile(const char* filename)
{
	std::memset(&nerpsfileGlobs, 0, sizeof(nerpsfileGlobs));

	Text_Clear();

	nerpsfileGlobs.fileSize = 0;
	std::memset(nerpsruntimeGlobs.registers, 0, sizeof(nerpsruntimeGlobs.registers));

	nerpsfileGlobs.instructions = nullptr;
	nerpsruntimeGlobs.messagePermit = true;

	sint32 setMsgPermit = true;
	NERPFunc__SetMessagePermit(&setMsgPermit);

	nerpsfileGlobs.instructions = (NERPsInstruction*)Gods98::File_LoadBinary(filename, &nerpsfileGlobs.fileSize);
	return (nerpsfileGlobs.instructions != nullptr);
}

// <LegoRR.exe @00453130>
bool32 __cdecl LegoRR::NERPsFile_LoadMessageFile(const char* filename);

// <LegoRR.exe @004534c0>
char* __cdecl LegoRR::NERPsFile_GetMessageLine(uint32 lineIndex)
{
	if (lineIndex < nerpsfileGlobs.messageLineCount) {
		return nerpsfileGlobs.messageLineList[lineIndex];
	}
	return nullptr;
}

// <LegoRR.exe @004534e0>
bool32 __cdecl LegoRR::NERPsFile_Free(void)
{
	if (nerpsfileGlobs.fileSize != 0) {
		Gods98::Mem_Free(nerpsfileGlobs.instructions);
	}

	if (nerpsfileGlobs.messageBuffer != nullptr) {
		Gods98::Mem_Free(nerpsfileGlobs.messageBuffer);
		if (nerpsfileGlobs.messageLineList != nullptr) {
			Gods98::Mem_Free(nerpsfileGlobs.messageLineList);
		}
	}

	nerpsfileGlobs.messageBuffer = nullptr;
	nerpsfileGlobs.messageLineList = nullptr;
	nerpsfileGlobs.messageLineCount = 0;
	nerpsfileGlobs.fileSize = 0;

	for (uint32 i = 0; i < nerpsfileGlobs.soundCount; i++) {
		Gods98::Sound3D_Remove(nerpsfileGlobs.soundList[i].sampleIndex);
	}

	if (nerpsfileGlobs.soundCount != 0) {
		if (nerpsfileGlobs.soundList != nullptr) {
			Gods98::Mem_Free(nerpsfileGlobs.soundList);
			nerpsfileGlobs.soundList = nullptr;
		}
		nerpsfileGlobs.soundCount = 0;
	}


	std::memset(&nerpsfileGlobs, 0, sizeof(nerpsfileGlobs));

	Text_Clear();
	return true;
}

// Helper function that converts an instruction for a call with zero arguments into its literal return value.
// <LegoRR.exe @004535a0>
void __cdecl LegoRR::NERPsRuntime_LoadLiteral(IN OUT NERPsInstruction* instruction)
{
	if (instruction->opcode == NERPsOpcode::Function && c_nerpsFunctions[instruction->operand].arguments == NERPS_ARGS_0) {

		// The wonkey assignment here is because of how NERPs treats instructions as a whole DWORD.
		// Loading integer literals only supports 0x0-0xffff, but functions can potentially
		// return anything from 0x0-0xffffffff.
		*(sint32*)instruction = c_nerpsFunctions[instruction->operand].function(nullptr);
	}
}

// <LegoRR.exe @004535e0>
void __cdecl LegoRR::NERPsRuntime_Execute(real32 elapsedAbs)
{
	if (nerpsfileGlobs.instructions == nullptr)
		return; // No NERPs file loaded.


	char logBuff[512];
	NERPsInstruction argsStack[3];

	const NERPsInstruction* instructions = nerpsfileGlobs.instructions;
	const uint32 instrCount = (nerpsfileGlobs.fileSize / sizeof(NERPsInstruction));

	// Left and right-hand registers during comparisons
	// regA is also used as the result of a conditional expression
	uint32 regA = 0;
	uint32 regB = 0; // dummy init = lastRegB; // Assigning uninitialised memory, oh no...
	bool32 negate = false;
	NERPsComparison currCmp = NERPsComparison::None;
	NERPsComparison nextCmp = currCmp; // dummy init

	for (uint32 instrIdx = 0; instrIdx < instrCount; instrIdx++, currCmp = nextCmp) {

		// Local copy of instruction, no need to worry about values getting modified.
		const NERPsInstruction instr = instructions[instrIdx];

		/// TRY ME: Uncomment to showcase how '?' operator functionally serves no purpose.
		//if (instr.opcode == NERPsOpcode::Operator && instr.operand == (uint16)NERPsOperator::Test) {
		//	nextCmp = currCmp;
		//	continue;
		//}

		// NERPs treats instructions as a single DWORD (loword=operand, hiword=opcode), and operates on them as such.
		// This is probably the reason why we have pointless bitwise AND comparisons
		// for opcode types instead of equality comparisons...
		if (instr.opcode & NERPsOpcode::Function) {
			uint32 funcId = (uint32)instr.operand;
			if (funcId == NERPS_FUNCID_STOP /*0*/ && (regA || currCmp == NERPsComparison::None)) break;

			NERPsFunctionArgs nargs = c_nerpsFunctions[funcId].arguments;

			// Functions have some very awkward behavior here.
			// 
			// A) Function arguments cannot be complex. They must either be an integer literals,
			//    or a 0-argument functions that returns a value (i.e. True, False).
			//    If a function argument is something else, the raw uint32 value of the instruction is used.
			//    `SetR2 SetR2` would result in `SetR2 0x2001b`
			//     (where `0x20000` is the opcode for functions, and 0x1b is the function ID for `SetR2`)
			// 
			// B) Functions that returns a value are ONLY designed to work on as part of an expression.
			//    Basically, they cannot be used as the action after '?', as they do not check the
			//     condition register, and will always execute.
			//    Likewise, functions with a void return always check the conditional register, which means
			//     they can technically be changed after any return-function or literal.
			//
			// C) Based on the above information, and how the Operator opcode works, the Test operator '?'
			//     seems to be COMPLETELY USELESS!!
			//    `TRUE ? Stop` would supposedly be identical to just `TRUE Stop`.
			//    Try it for yourself by uncommenting the code at the beginning of this instruction loop.

			if (nargs >= NERPS_ARGS_0 && nargs <= NERPS_ARGS_2) { // expression function

				if (nerpsruntimeGlobs.logFuncCalls) {
					// Only called for NERPS_ARGS_0_NORETURN and NERPS_ARGS_1_NORETURN in LegoRR.
					// Never called for non-void functions.
					std::sprintf(logBuff, "Func Call %s", c_nerpsFunctions[funcId].name);
				}

				switch (nargs) {
				case NERPS_ARGS_0:
					regB = c_nerpsFunctions[funcId].function(nullptr); // 0 stack arguments
					break;

				case NERPS_ARGS_1:
					argsStack[0] = instructions[instrIdx + 1];
					NERPsRuntime_LoadLiteral(&argsStack[0]);
					regB = c_nerpsFunctions[funcId].function((sint32*)argsStack);
					instrIdx += 1;
					break;

				case NERPS_ARGS_2:
					argsStack[0] = instructions[instrIdx + 1];
					argsStack[1] = instructions[instrIdx + 2];
					NERPsRuntime_LoadLiteral(&argsStack[0]);
					NERPsRuntime_LoadLiteral(&argsStack[1]);
					regB = c_nerpsFunctions[funcId].function((sint32*)argsStack);
					instrIdx += 2;
					break;
				}

				if (negate) regB = (uint32)!regB; // (regB == 0)

				uint32 ret = regA;
				switch (currCmp) {
				case NERPsComparison::None:
					ret = regB;
					break;
				case NERPsComparison::And: // Boolean AND
					ret = (uint32)(regA && regB);
					break;
				case NERPsComparison::Or:  // Boolean OR
					ret = (uint32)(regA || regB);
					break;
				case NERPsComparison::Cgt: // NERPsOperator::Cgt /*5 ">"*/
					ret = (uint32)(regA > regB);
					break;
				case NERPsComparison::Clt: // NERPsOperator::Clt /*6 "<"*/
					ret = (uint32)(regA < regB);
					break;
				case NERPsComparison::Ceq: // NERPsOperator::Ceq /*7 "="*/
					ret = (uint32)(regA == regB);
					break;
				case NERPsComparison::Cge: // NERPsOperator::Cge /*8 ">="*/
					ret = (uint32)(regA >= regB);
					break;
				case NERPsComparison::Cle: // NERPsOperator::Cle /*9 "<="*/
					ret = (uint32)(regA <= regB);
					break;
				case NERPsComparison::Cne: // NERPsOperator::Cne /*10 "!="*/
					ret = (uint32)(regA != regB);
					break;
				}
				regA = ret;
				negate = false;
				nextCmp = NERPsComparison::And;

			}
			else if (nargs >= NERPS_ARGS_0_NORETURN && nargs <= NERPS_ARGS_3_NORETURN) { // action function

				if (nerpsruntimeGlobs.logFuncCalls) {
					// Only called for NERPS_ARGS_0_NORETURN and NERPS_ARGS_1_NORETURN in LegoRR.
					std::sprintf(logBuff, "Func Call %s", c_nerpsFunctions[funcId].name);
				}

				switch (nargs) {
				case NERPS_ARGS_0_NORETURN:
					if (regA) { // Action if expression is true
						c_nerpsFunctions[funcId].function(nullptr); // 0 stack arguments
					}
					break;

				case NERPS_ARGS_1_NORETURN:
					if (regA) { // Action if expression is true
						argsStack[0] = instructions[instrIdx + 1];
						NERPsRuntime_LoadLiteral(&argsStack[0]);
						c_nerpsFunctions[funcId].function((sint32*)argsStack);
					}
					instrIdx += 1;
					break;
				case NERPS_ARGS_2_NORETURN:
					if (regA) { // Action if expression is true
						argsStack[0] = instructions[instrIdx + 1];
						argsStack[1] = instructions[instrIdx + 2];
						NERPsRuntime_LoadLiteral(&argsStack[0]);
						NERPsRuntime_LoadLiteral(&argsStack[1]);
						c_nerpsFunctions[funcId].function((sint32*)argsStack);
					}
					instrIdx += 2;
					break;
				case NERPS_ARGS_3_NORETURN:
					if (regA) { // Action if expression is true
						argsStack[0] = instructions[instrIdx + 1];
						argsStack[1] = instructions[instrIdx + 2];
						argsStack[2] = instructions[instrIdx + 3];
						NERPsRuntime_LoadLiteral(&argsStack[0]);
						NERPsRuntime_LoadLiteral(&argsStack[1]);
						NERPsRuntime_LoadLiteral(&argsStack[2]);
						c_nerpsFunctions[funcId].function((sint32*)argsStack);
					}
					instrIdx += 3;
					break;
				}

				regA = 0;
				negate = false;
				nextCmp = NERPsComparison::None;
			}
			else { // invalid
				nextCmp = currCmp;
			}

		}
		else if (instr.opcode & NERPsOpcode::Operator) {

			NERPsOperator opId = (NERPsOperator)instr.operand;
			switch (opId) {
			case NERPsOperator::Plus:   // Boolean AND
				negate = false;
				nextCmp = NERPsComparison::And;
				break;
			case NERPsOperator::FSlash: // Boolean OR
				negate = false;
				nextCmp = NERPsComparison::Or;
				break;
			case NERPsOperator::Cgt: // NERPsOperator::Cgt /*5 ">"*/
				negate = false;
				nextCmp = NERPsComparison::Cgt;
				break;
			case NERPsOperator::Clt: // NERPsOperator::Clt /*6 "<"*/
				negate = false;
				nextCmp = NERPsComparison::Clt;
				break;
			case NERPsOperator::Ceq: // NERPsOperator::Ceq /*7 "="*/
				negate = false;
				nextCmp = NERPsComparison::Ceq;
				break;
			case NERPsOperator::Cge: // NERPsOperator::Cge /*8 ">="*/
				negate = false;
				nextCmp = NERPsComparison::Cge;
				break;
			case NERPsOperator::Cle: // NERPsOperator::Cle /*9 "<="*/
				negate = false;
				nextCmp = NERPsComparison::Cle;
				break;
			case NERPsOperator::Cne: // NERPsOperator::Cne /*10 "!="*/
				negate = false;
				nextCmp = NERPsComparison::Cne;
				break;
			case NERPsOperator::Pound: // Boolean Not "!" (???)
				negate = true;
				nextCmp = currCmp;
				break;
			//case NERPsOperator::BSlash:
			//case NERPsOperator::Test:
			default: // (implicit)
				negate = false;
				nextCmp = currCmp;
				break;
			}

		}
		else if (instr.opcode & NERPsOpcode::Label) {
			regA = 0;
			negate = false;
			nextCmp = NERPsComparison::None;

		}
		else if (instr.opcode & NERPsOpcode::Jump) {
			if (regA) { // Jump if expression is true
				instrIdx = (uint32)instr.operand;
			}

			regA = 0;
			negate = false;
			nextCmp = NERPsComparison::None;

		}
		else { //if (instr.opcode == NERPsOpcode::Load) {
			regB = (sint32)(sint16)instr.operand; // sint16 -> sint32 (sign extension to preserve negative numbers)

			if (negate) regB = (uint32)!regB; // (regB == 0)

			uint32 ret = regA;
			switch (currCmp) {
			case NERPsComparison::None:
				ret = regB;
				break;
			case NERPsComparison::And: // Boolean AND
				ret = (uint32)(regA && regB);
				break;
			case NERPsComparison::Or:  // Boolean OR
				ret = (uint32)(regA || regB);
				break;
			case NERPsComparison::Cgt: // NERPsOperator::Cgt /*5 ">"*/
				ret = (uint32)(regA > regB);
				break;
			case NERPsComparison::Clt: // NERPsOperator::Clt /*6 "<"*/
				ret = (uint32)(regA < regB);
				break;
			case NERPsComparison::Ceq: // NERPsOperator::Ceq /*7 "="*/
				ret = (uint32)(regA == regB);
				break;
			case NERPsComparison::Cge: // NERPsOperator::Cge /*8 ">="*/
				ret = (uint32)(regA >= regB);
				break;
			case NERPsComparison::Cle: // NERPsOperator::Cle /*9 "<="*/
				ret = (uint32)(regA <= regB);
				break;
			case NERPsComparison::Cne: // NERPsOperator::Cne /*10 "!="*/
				ret = (uint32)(regA != regB);
				break;
			}
			regA = ret;
			negate = false;
			nextCmp = NERPsComparison::And;
		}


		//currCmp = nextCmp; // (moved into for loop increment section)
	}

	NERPsRuntime_EndExecute(elapsedAbs);
}

#pragma endregion
