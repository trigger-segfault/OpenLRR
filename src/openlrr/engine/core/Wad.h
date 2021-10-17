// Wad.h : 
//
/// APIS: -
/// DEPENDENCIES: Compress, (Memory)
/// DEPENDENTS: Files

#pragma once

#include "../../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

// percent    = integer [0,100]
// debug      = TRUE for debug-specific messages (mostly compression-related)
// currentMsg = progress message for current file. (not passed when stateMsg is passed)
// stateMsg   = message for current progress state/pass etc. (only passed when changed)
typedef void (__cdecl* WadProgressUpdate)(sint32 percent, bool32 debug, const char* currentMsg, const char* stateMsg);

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define MAX_WADS 10

// Number of WADS you can have at one time
#define MAX_OPEN_FILES 100
#define MAX_FILE_COMPRESSION_SIZE (1024*1024)

#ifndef WAD_ERROR
#define WAD_ERROR ((Wad_HandleValue) -1)
#endif

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

flags_scoped(Wad_EntryFlags) : uint32
{
	WADENTRY_FLAG_NONE         = 0,
	WADENTRY_FLAG_UNCOMPRESSED = (1 << 0),
	WADENTRY_FLAG_RNCOMPRESSED = (1 << 1),
	WADENTRY_FLAG_IS_IN_WAD    = (1 << 2),
};
flags_scoped_end(Wad_EntryFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Typedefs
 **********************************************************************************/

#pragma region Typedefs

#ifndef _WAD_HANDLEVALUE_TYPEDEF_
#define _WAD_HANDLEVALUE_TYPEDEF_
typedef sint32 Wad_HandleValue;
#endif

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct WadEntry
{
	/*00,4*/ Wad_EntryFlags compression;		// Is the file compressed
	/*04,4*/ sint32 fileLength;			// Length of the file in wad
	/*08,4*/ sint32 decompressedLength;	// Decompressed length of file
	/*0c,4*/ union {
		sint32 addr;
		void* ptr;
	};
	/*10*/
};
assert_sizeof(WadEntry, 0x10);


struct Wad
{
	/*00,4*/ char* fName;
	/*04,4*/ bool32 active;
	/*08,4*/ void* hFile;			// (unused) HANDLE
	/*0c,4*/ void* hFileMapping;	// (unused) HANDLE
	/*10,4*/ FILE* fWad;					// File handle of the wad
										// Each of the file names in the wad
	/*14,4*/ char** fileNames;			// Names of actual files
	/*18,4*/ char** wadNames;			// Names within wad
	/*1c,4*/ WadEntry* wadEntries;
	/*20,4*/ sint32 numFiles;
	/*24*/
};
assert_sizeof(Wad, 0x24);


struct Wad_FileHandle
{
	/*00,4*/ void* data;					// Pointer to the file data
	/*04,4*/ bool32 active;				// Is this handle active already
	/*08,4*/ Wad_HandleValue wadFile;				// Wad file this handle uses
	/*0c,4*/ sint32 indexOfFileInWad;		// Index of the file in the wad structure
	/*10*/
};
assert_sizeof(Wad_FileHandle, 0x10);


struct Wad_Globs
{
	/*000,4*/ uint32 computerNameLength; // (address not known)
	/*004,4*/ bool32 wadLogFileAccess;    // (address not known)
	/*008,4*/ FILE* s_ErrorFile_f;      // (address not known)
	/*00c,168*/ Wad wads[MAX_WADS];				// Wad structures
	/*174,28*/ sint32 references[MAX_WADS];		// Current count of references to the wad file
	/*19c,10*/ char computerName[16];    // (address not known)
	/*1ac,640*/ Wad_FileHandle fileHandles[MAX_OPEN_FILES];
	/*7ec*/
};
assert_sizeof(Wad_Globs, 0x7ec);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

//extern Wad_FileHandle fileHandles[MAX_OPEN_FILES];
// <LegoRR.exe @005764f4>
extern Wad_Globs & wadGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define WAD(which) wadGlobs.wads[which]
#define CURWAD wadGlobs.wads[wadGlobs.activeWad]

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// multiple-uses
// <LegoRR.exe @0047f8c0>
void __cdecl Wad_Error(const char* msg, ...);


// <LegoRR.exe @0048b760>
bool32 __cdecl GetFileName(FILE* f, OUT char* buffer);

////////////////////////////////////////////////////
//////////// The interface for wads ////////////////
////////////////////////////////////////////////////

////////////////////////////////////////////////////
// Open the wad and map in the file data.
// Returns a handle to the wad file.

// <LegoRR.exe @0048b7a0>
Wad_HandleValue __cdecl Wad_Load(const char* fName);

// <LegoRR.exe @0048bfa0>
Wad* __cdecl Wad_Get(Wad_HandleValue wadNo);

////////////////////////////////////////////////////
// Close an open wad

// <missing>
void __cdecl Wad_Close(Wad_HandleValue wadNo);

////////////////////////////////////////////////////
// Save a wad file to the disk.
// Returns WAD_ERROR on failure.

/*int __cdecl Wad_Save(const char* wadName,		// Wad to write to
			 char** fNames,		// List of files to compress
			 char** wNames,		// names of files within wad
			 unsigned* flags,	// Flags for each file
			 sint32 numFiles,
			 sint32 wadHandle,
			 WadProgressUpdate ProgressUpdate
			 );*/

////////////////////////////////////////////////////
// If you pass TRUE to this then all file access
// will be written out to file.log.

//void __cdecl Wad_LogFileAccess(bool32 mode);

////////////////////////////////////////////////////
// Data access in wads

////////////////////////////////////////////////////
// Wad information fucntions

// <missing>
bool32 __cdecl Wad_FileCompressed(Wad_HandleValue wadNo, sint32 fileNo);

// <LegoRR.exe @0048bfb0>
sint32 __cdecl Wad_FileLength(Wad_HandleValue wadNo, sint32 fileNo);

// <LegoRR.exe @0048bfd0>
bool32 __cdecl Wad_FileCompressedLength(Wad_HandleValue wadNo, sint32 fileNo);

// <LegoRR.exe @0048bff0>
sint32 __cdecl Wad_FindFreeFileHandle(void);

// <LegoRR.exe @0048c010>
sint32 __cdecl Wad_IsFileInWad(const char* fName, Wad_HandleValue wadNo);

// <LegoRR.exe @0048c060>
sint32 __cdecl _Wad_IsFileInWad(const char* fName, Wad_HandleValue wadNo);

// <LegoRR.exe @0048c0c0>
Wad_HandleValue __cdecl Wad_GetFreeWadSlot(void);

////////////////////////////////////////////////////
// Wad operations

// <LegoRR.exe @0048c100>
sint32 __cdecl _Wad_FileOpen(const char* fName, Wad_HandleValue wadNo);

// <LegoRR.exe @0048c230>
sint32 __cdecl Wad_FileOpen(const char* fName, Wad_HandleValue wadNo);

// <LegoRR.exe @0048c280>
void __cdecl Wad_FileClose(sint32 handle);

// <LegoRR.exe @0048c2b0>
void* __cdecl Wad_FileGetPointer(sint32 handle);

////////////////////////////////////////////////////
// Wad dialogs

// <LegoRR.exe @0048c2d0>
sint32 __cdecl Wad_hLength(sint32 handle);
// <LegoRR.exe @0048c2f0>
void* __cdecl Wad_hData(sint32 handle);

/*void __cdecl Wad_Error(const char* msg, ...);
extern char computerName[];
extern DWORD computerNameLength;
extern bool32 wadLogFileAccess;
bool32 __cdecl wad_fcheck(const char*);
sint32 __cdecl wad_flength(FILE*);*/

#pragma endregion

}
