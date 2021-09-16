// Wad.cpp : 
//

#include "Compress.h"
#include "Memory.h"

#include "Wad.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

//extern Wad_FileHandle fileHandles[MAX_OPEN_FILES];
// <LegoRR.exe @005764f4>
Gods98::Wad_Globs & Gods98::wadGlobs = *(Gods98::Wad_Globs*)0x005764f4; // (init: see below)

// // Name of the computer so we can constuct network paths
// char computerName[MAX_COMPUTERNAME_LENGTH + 1];
// uint32 computerNameLength = MAX_COMPUTERNAME_LENGTH + 1;
// bool32 wadLogFileAccess = false;

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
void __cdecl Gods98::Wad_Error(const char* msg, ...)
{
	log_firstcall();

	char buffer[1024];
	std::va_list arg;
	va_start(arg, msg);
	std::vsprintf(buffer, msg, arg);
	va_end(arg);
	/// TEMPORARY: Stop logging all wad filenames
	//::OutputDebugStringA(buffer);
}


// <LegoRR.exe @0048b760>
bool32 __cdecl Gods98::GetFileName(FILE* f, OUT char* buffer)
{
	log_firstcall();

	char c;
	while (c = std::fgetc(f)) {
		*buffer = c;
		buffer++;
	}
	*buffer = '\0';
	return true;
}

////////////////////////////////////////////////////
//////////// The interface for wads ////////////////
////////////////////////////////////////////////////

////////////////////////////////////////////////////
// Open the wad and map in the file data.
// Returns a handle to the wad file.

// <LegoRR.exe @0048b7a0>
Gods98::Wad_HandleValue __cdecl Gods98::Wad_Load(const char* fName)
{
	log_firstcall();

	char header[4];

	Wad_HandleValue newWad = Wad_GetFreeWadSlot();
	if (newWad == WAD_ERROR) return WAD_ERROR;

	// Load the wad
	FILE* f = Wad_Get(newWad)->fWad = std::fopen(fName, "rb");
	if (!f) return WAD_ERROR;

	Wad_Get(newWad)->active = TRUE;

	// Check to see if we actually have a wad file.
	if (fread(header, 1, 4, f) != 4) 
	{
		std::fclose(f);
		return WAD_ERROR;
	}
	if (std::memcmp(header, "WWAD", 4) != 0)
	{
		std::fclose(f);
		return WAD_ERROR;
	}

	// Get the number of files to load
	if (fread(&(Wad_Get(newWad)->numFiles), 1, sizeof(int), f) != sizeof(int))
	{
		std::fclose(f);
		return WAD_ERROR;
	}

	// The names are first
	Wad_Get(newWad)->wadNames =  (char **)std::malloc (sizeof(char *) * Wad_Get(newWad)->numFiles);
	Wad_Get(newWad)->fileNames = (char **)std::malloc (sizeof(char *) * Wad_Get(newWad)->numFiles);
	if (!Wad_Get(newWad)->wadNames || !Wad_Get(newWad)->fileNames) 
	{
		std::fclose(f);
		if (Wad_Get(newWad)->wadNames) std::free(Wad_Get(newWad)->wadNames);
		if (Wad_Get(newWad)->fileNames) std::free(Wad_Get(newWad)->fileNames);
		return -1;
	}
	std::memset(Wad_Get(newWad)->wadNames, 0, sizeof(char *) * Wad_Get(newWad)->numFiles);
	std::memset(Wad_Get(newWad)->fileNames, 0, sizeof(char *) * Wad_Get(newWad)->numFiles);

	// Reads the names of the files in the wad and on disk
	sint32 counter = 0;
	while (counter < Wad_Get(newWad)->numFiles)
	{
		char buffer[4096];		// Nice big buffer to read into likes.
		if (!GetFileName(f, buffer))
		{
			if (Wad_Get(newWad)->wadNames) 
			{
				for (sint32 i=0; i<Wad_Get(newWad)->numFiles; i++) 
					if (Wad_Get(newWad)->wadNames[i]) std::free(Wad_Get(newWad)->wadNames[i]);
				std::free(Wad_Get(newWad)->wadNames);
			}
			if (Wad_Get(newWad)->fileNames) std::free(Wad_Get(newWad)->fileNames);
			std::fclose(f);
			return WAD_ERROR;
		}
		else
		{
			Wad_Get(newWad)->wadNames[counter] = (char *)std::malloc(std::strlen(buffer) + 1);
			std::strcpy(Wad_Get(newWad)->wadNames[counter], buffer);

			Wad_Error("%s\n", Wad_Get(newWad)->wadNames[counter]);
		}
		counter++;
	}
	counter = 0;
	while (counter < Wad_Get(newWad)->numFiles)
	{
		char buffer[4096];		// Nice big buffer to read into likes.
		if (!GetFileName(f, buffer))
		{
			if (Wad_Get(newWad)->fileNames) 
			{
				for (sint32 i=0; i<Wad_Get(newWad)->numFiles; i++)
					if (Wad_Get(newWad)->wadNames[i]) std::free(Wad_Get(newWad)->wadNames[i]);
				std::free(Wad_Get(newWad)->wadNames);
			}
			if (Wad_Get(newWad)->fileNames) 
			{
				for (sint32 i=0; i<Wad_Get(newWad)->numFiles; i++)
					if (Wad_Get(newWad)->fileNames[i]) std::free(Wad_Get(newWad)->fileNames[i]);
				std::free(Wad_Get(newWad)->fileNames);
			}
			std::fclose(f);
			return WAD_ERROR;
		}
		else
		{
			Wad_Get(newWad)->fileNames[counter] = (char *)std::malloc(std::strlen(buffer) + 1);
			std::strcpy(Wad_Get(newWad)->fileNames[counter], buffer);

			Wad_Error("%s\n", Wad_Get(newWad)->fileNames[counter]);
		}
		counter++;
	}


	// Now we get the data.  Address of file, compression
	Wad_Get(newWad)->wadEntries = (WadEntry *)std::malloc(sizeof(WadEntry) * Wad_Get(newWad)->numFiles);
	if (!Wad_Get(newWad)->wadEntries)
	{
		if (Wad_Get(newWad)->fileNames) 
		{
			for (sint32 i=0; i<Wad_Get(newWad)->numFiles; i++)
				if (Wad_Get(newWad)->wadNames[i]) std::free(Wad_Get(newWad)->wadNames[i]);
			std::free(Wad_Get(newWad)->wadNames);
		}
		if (Wad_Get(newWad)->fileNames) 
		{
			for (sint32 i=0; i<Wad_Get(newWad)->numFiles; i++)
				if (Wad_Get(newWad)->fileNames[i]) std::free(Wad_Get(newWad)->fileNames[i]);
			std::free(Wad_Get(newWad)->fileNames);
		}
		std::fclose(f);
		return WAD_ERROR;
	}

	if (std::fread(Wad_Get(newWad)->wadEntries, 1, sizeof(WadEntry) * Wad_Get(newWad)->numFiles, f) != (sint32)(sizeof(WadEntry) * Wad_Get(newWad)->numFiles))
	{
		if (Wad_Get(newWad)->fileNames) 
		{
			for (sint32 i=0; i<Wad_Get(newWad)->numFiles; i++)
				if (Wad_Get(newWad)->wadNames[i]) std::free(Wad_Get(newWad)->wadNames[i]);
			std::free(Wad_Get(newWad)->wadNames);
		}
		if (Wad_Get(newWad)->fileNames) 
		{
			for (sint32 i=0; i<Wad_Get(newWad)->numFiles; i++)
				if (Wad_Get(newWad)->fileNames[i]) std::free(Wad_Get(newWad)->fileNames[i]);
			std::free(Wad_Get(newWad)->fileNames);
		}
		if (Wad_Get(newWad)->wadEntries) std::free(Wad_Get(newWad)->wadEntries);
		std::fclose(f);
		return WAD_ERROR;
	}

	return newWad;
}

// <LegoRR.exe @0048bfa0>
Gods98::Wad* __cdecl Gods98::Wad_Get(Wad_HandleValue wadNo)
{
	log_firstcall();

	return &(wadGlobs.wads[wadNo]);
}

////////////////////////////////////////////////////
// Close an open wad

// <unused>
void __cdecl Gods98::Wad_Close(Wad_HandleValue wadNo)
{
	log_firstcall();

	if (wadNo != WAD_ERROR)
	{
		if (Wad_Get(wadNo)->fileNames)
		{
			for (sint32 i = 0; i < Wad_Get(wadNo)->numFiles; i++)
				if (Wad_Get(wadNo)->wadNames[i]) std::free(Wad_Get(wadNo)->wadNames[i]);
			std::free(Wad_Get(wadNo)->wadNames);
		}
		if (Wad_Get(wadNo)->fileNames)
		{
			for (sint32 i = 0; i < Wad_Get(wadNo)->numFiles; i++)
				if (Wad_Get(wadNo)->fileNames[i]) std::free(Wad_Get(wadNo)->fileNames[i]);
			std::free(Wad_Get(wadNo)->fileNames);
		}
		if (Wad_Get(wadNo)->wadEntries) std::free(Wad_Get(wadNo)->wadEntries);
		std::fclose(Wad_Get(wadNo)->fWad);
	}
}

////////////////////////////////////////////////////
// Save a wad file to the disk.
// Returns WAD_ERROR on failure.

/*int __cdecl Gods98::Wad_Save(const char* wadName,		// Wad to write to
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

//void __cdecl Gods98::Wad_LogFileAccess(bool32 mode);

////////////////////////////////////////////////////
// Data access in wads

////////////////////////////////////////////////////
// Wad information fucntions

// <missing>
bool32 __cdecl Gods98::Wad_FileCompressed(Wad_HandleValue wadNo, sint32 fileNo)
{
	return (Wad_Get(wadNo)->wadEntries[fileNo].compression != Wad_EntryFlags::WAD_FILE_UNCOMPRESSED);
}

// <LegoRR.exe @0048bfb0>
sint32 __cdecl Gods98::Wad_FileLength(Wad_HandleValue wadNo, sint32 fileNo)
{
	log_firstcall();

	return Wad_Get(wadNo)->wadEntries[fileNo].decompressedLength;
}

// <LegoRR.exe @0048bfd0>
bool32 __cdecl Gods98::Wad_FileCompressedLength(Wad_HandleValue wadNo, sint32 fileNo)
{
	log_firstcall();

	return Wad_Get(wadNo)->wadEntries[fileNo].fileLength;
}

// <LegoRR.exe @0048bff0>
sint32 __cdecl Gods98::Wad_FindFreeFileHandle(void)
{
	log_firstcall();

	for (sint32 i = 0; i < MAX_OPEN_FILES; i++) if (!wadGlobs.fileHandles[i].active) return i;
	return WAD_ERROR;
}

// <LegoRR.exe @0048c010>
sint32 __cdecl Gods98::Wad_IsFileInWad(const char* fName, Wad_HandleValue wadNo)
{
	log_firstcall();

	if (wadNo == WAD_ERROR) {
		for (sint32 i = MAX_WADS - 1; i >= 0; i--) {
			if (Wad_Get(i)->active) {
				sint32 res = _Wad_IsFileInWad(fName, i);
				if (res != WAD_ERROR) return res;
			}
		}
		return WAD_ERROR;
	}
	else return _Wad_IsFileInWad(fName, wadNo);
}

// <LegoRR.exe @0048c060>
sint32 __cdecl Gods98::_Wad_IsFileInWad(const char* fName, Wad_HandleValue wadNo)
{
	log_firstcall();

	for (sint32 i = 0; i < wadGlobs.wads[wadNo].numFiles; i++)
	{
		if (!::_stricmp(wadGlobs.wads[wadNo].wadNames[i], fName)) return i;
	}
	return WAD_ERROR;
}

// <LegoRR.exe @0048c0c0>
Gods98::Wad_HandleValue __cdecl Gods98::Wad_GetFreeWadSlot(void)
{
	log_firstcall();

	for (sint32 i = 0; i < MAX_WADS; i++)
	{
		if (!WAD(i).active)
		{
			// Zero out the slot
			std::memset(&WAD(i), 0, sizeof(Wad));
			return (Wad_HandleValue)i;
		}
	}
	return WAD_ERROR;
}

////////////////////////////////////////////////////
// Wad operations

// <LegoRR.exe @0048c100>
sint32 __cdecl Gods98::_Wad_FileOpen(const char* fName, Wad_HandleValue wadNo)
{
	log_firstcall();

	sint32 indexOfFileInWad, fileHandle;

	// Search the file handles for a free one.
	if ((fileHandle = Wad_FindFreeFileHandle()) == WAD_ERROR) return WAD_ERROR;

	// Search the wad for the file and get its index
	if ((indexOfFileInWad = Wad_IsFileInWad(fName, wadNo)) == WAD_ERROR) return WAD_ERROR;

	// Allocate a block of memory for the file and read it from the disk
	void* ptr = std::malloc(Wad_FileCompressedLength(wadNo, indexOfFileInWad));
	if (!ptr) {
		return WAD_ERROR;
	}

	std::fseek(Wad_Get(wadNo)->fWad, Wad_Get(wadNo)->wadEntries[indexOfFileInWad].addr, SEEK_SET);
	if (std::fread(ptr, Wad_FileCompressedLength(wadNo, indexOfFileInWad), 1, Wad_Get(wadNo)->fWad) != 1)
	{
		std::free(ptr);
		return WAD_ERROR;
	}

	// If the file is compressed then it must be decompressed first
	if (Wad_Get(wadNo)->wadEntries[indexOfFileInWad].compression & Wad_EntryFlags::WAD_FILE_RNCOMPRESSED)
	{
		void *newBuffer = nullptr;
		RNC_Uncompress(ptr, &newBuffer);
		std::free(ptr);
		ptr = newBuffer;
	}

	// Fill out the file handle data
	wadGlobs.fileHandles[fileHandle].data = ptr;
	wadGlobs.fileHandles[fileHandle].active = true;
	wadGlobs.fileHandles[fileHandle].wadFile = wadNo;
	wadGlobs.fileHandles[fileHandle].indexOfFileInWad = indexOfFileInWad;

	return fileHandle;
}

// <LegoRR.exe @0048c230>
sint32 __cdecl Gods98::Wad_FileOpen(const char* fName, Wad_HandleValue wadNo)
{
	log_firstcall();

	if (wadNo == WAD_ERROR) {
		// Look in all wads (backwards)
		for (sint32 i = MAX_WADS - 1; i >= 0; i--) {
			if (Wad_Get(i)->active) {
				sint32 res = _Wad_FileOpen(fName, (Wad_HandleValue)i);
				if (res != WAD_ERROR) {
					return res;
				}
			}
		}
		return WAD_ERROR;
	}
	else return _Wad_FileOpen(fName, wadNo);
}

// <LegoRR.exe @0048c280>
void __cdecl Gods98::Wad_FileClose(sint32 handle)
{
	log_firstcall();

	if (wadGlobs.fileHandles[handle].active) {
		std::free(wadGlobs.fileHandles[handle].data);
		wadGlobs.fileHandles[handle].active = false;
	}
}

// <LegoRR.exe @0048c2b0>
void* __cdecl Gods98::Wad_FileGetPointer(sint32 handle)
{
	log_firstcall();

	if (wadGlobs.fileHandles[handle].active) {
		return wadGlobs.fileHandles[handle].data;
	}
	return nullptr;
}

////////////////////////////////////////////////////
// Wad dialogs

// <LegoRR.exe @0048c2d0>
sint32 __cdecl Gods98::Wad_hLength(sint32 handle)
{
	log_firstcall();

	return Wad_FileLength(wadGlobs.fileHandles[handle].wadFile, wadGlobs.fileHandles[handle].indexOfFileInWad);
}

// <LegoRR.exe @0048c2f0>
void* __cdecl Gods98::Wad_hData(sint32 handle)
{
	log_firstcall();

	return Wad_FileGetPointer(handle);
}

/*void __cdecl Wad_Error(const char* msg, ...);
extern char computerName[];
extern DWORD computerNameLength;
extern bool32 wadLogFileAccess;
bool32 __cdecl wad_fcheck(const char*);
sint32 __cdecl wad_flength(FILE*);*/

#pragma endregion
