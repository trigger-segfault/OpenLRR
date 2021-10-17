// Files.cpp : 
//

#include "../../platform/windows.h"

#include "../util/Registry.h"
#include "Errors.h"
#include "Memory.h"
#include "Utils.h"
#include "Wad.h"

#include "Files.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004abf00>
sint32 & Gods98::currWadHandle = *(sint32*)0x004abf00; // = -1; // = WAD_ERROR;

// <LegoRR.exe @005349a0>
Gods98::File_Globs & Gods98::fileGlobs = *(Gods98::File_Globs*)0x005349a0; // = { 0 };

// <LegoRR.exe @005779e0>
Gods98::FileCheck_Globs & Gods98::checkGlobs = *(Gods98::FileCheck_Globs*)0x005779e0; // (no init)

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define StdFile(f) ((FILE*)((f)->std))
#define WadFile(f) ((WADFILE*)((f)->wad))

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// multiple-uses: logf_removed
// <LegoRR.exe @00484f50>
void __cdecl Gods98::File_ErrorFile(const char* msg, ...)
{
	log_firstcall();

}

// <LegoRR.exe @0047f3f0>
void __cdecl Gods98::File_Initialise(const char* programName, bool32 insistOnCD, const char* registryLocation)
{
	log_firstcall();

	sint32 wad = WAD_ERROR;
	bool32 foundCD = false;
	bool32 foundWad = false;
	char cwd[_MAX_PATH];
	FILE* fp;
	char fname[1024];
	bool32 failed = false;

	::_getcwd(cwd, sizeof(cwd));
	if (cwd[std::strlen(cwd) - 1] == '\\') cwd[std::strlen(cwd) - 1] = '\0';
	std::sprintf(fileGlobs.dataDir, "%s\\%s", cwd, FILE_DATADIRNAME);
	//sprintf(fileGlobs.exeDir, "%s", cwd);

//#ifdef _GODS98_USEWAD_
	{
		char wadFile[FILE_MAXPATH];
		char cwdBase[FILE_MAXPATH];
		for (uint32 i = 0; i < MAX_WADS; i++) {
			std::sprintf(wadFile, "%s%i.wad", programName, (int)i);
			wad = File_LoadWad(wadFile);
			if (wad == WAD_ERROR) {
				Error_Warn(true, Error_Format("Cannot load %s", wadFile));
			} else {
//				File_SetActiveWad(wad);
				foundWad = true;
			}
		}
		::_getcwd(cwdBase, sizeof(cwdBase));
		std::strcat(cwdBase, "\\Data");
		File_SetBaseSearchPath(cwdBase);
	}
//#endif // _GODS98_USEWAD_


	while (!(foundCD = File_FindDataCD()) && insistOnCD) {

		char msgNoCD[1024];
		char msgError[1024] = "Error";

		if (Registry_GetValue(registryLocation, "CDMissing", RegistryValue::String, msgNoCD, sizeof(msgNoCD))) {

			Registry_GetValue(registryLocation, "SetupError", RegistryValue::String, msgError, sizeof(msgError));

			if (::MessageBoxA(nullptr, msgNoCD, msgError, MB_OKCANCEL) == IDCANCEL) std::exit(0);

		} else std::exit(0);
	}

	if (!foundCD && !foundWad) {     // Assume that if a wad is found then it is the correct one.

		failed = true;

		_finddata32_t findData;
		intptr_t handle;
		if ((handle = ::_findfirst32("*.*", &findData)) != -1) {
			do {
				if (findData.attrib & _A_SUBDIR) {
					if (::_stricmp(findData.name, FILE_DATADIRNAME) == 0) {
						failed = false;
						break;
					}
				}
			} while (::_findnext32(handle, &findData) == 0);

			::_findclose(handle);
		}
	}

	if (insistOnCD) {
		std::sprintf(fname, "%s\\%s", FILE_DATADIRNAME, FILE_KEYFILENAME);
		if (fp = std::fopen(fname, "r")) {
			std::fclose(fp);
			failed = true;
		}
	}

	if (!failed) {

		// If everything started up ok (CD in or WAD found), then ensure the data directory exists...
		::_mkdir(FILE_DATADIRNAME);

		// Remove the 'delme' file if it exists...
		std::sprintf(fname, "%s\\%s", FILE_DATADIRNAME, FILE_DELMEFILENAME);
		
		if (fp = std::fopen(fname, "r")) {
			std::fclose(fp);
			::_chmod(fname, _S_IWRITE);
			std::remove(fname);
		}

	} else {
		char msgNoData[1024];
		char msgError[1024] = "Error";

		if (Registry_GetValue(registryLocation, "DataMissing", RegistryValue::String, msgNoData, sizeof(msgNoData))) {
			Registry_GetValue(registryLocation, "SetupError", RegistryValue::String, msgError, sizeof(msgError));
			::MessageBoxA(nullptr, msgNoData, msgError, MB_OK);
		}
		std::exit(0);
	}
}

// <LegoRR.exe @0047f7b0>
bool32 __cdecl Gods98::File_FindDataCD(void)
{
	log_firstcall();

	char drive[] = "A:\\";
	//char letter;
	char fname[FILE_MAXPATH];

	for (char letter = 'C'; letter <= 'Z'; letter++) {
		drive[0] = letter;
		if (::GetDriveTypeA(drive) == DRIVE_CDROM) {
			std::sprintf(fname, "%c:\\%s\\%s", letter, FILE_DATADIRNAME, FILE_KEYFILENAME);
			FILE* fp;
			if (fp = std::fopen(fname, "r")) {
				std::fclose(fp);
				fileGlobs.cdLetter = letter;
				return true;
			}
		}
	}
	return false;
}

// This allows the translation from File paths into Wad names.
// <LegoRR.exe @0047f850>
bool32 __cdecl Gods98::File_SetBaseSearchPath(const char* basePath)
{
	log_firstcall();

	sint32 len;
	if (!basePath || (len = (sint32)std::strlen(basePath)) > MAX_WAD_BASE_PATH || !len)
	{
		std::memset(fileGlobs.wadBasePath, 0, MAX_WAD_BASE_PATH);
		fileGlobs.basePathSet = false;
		return false;
	}
	else
	{
		std::strcpy(fileGlobs.wadBasePath, basePath);
		fileGlobs.basePathSet = true;
		return true;
	}
}

// <LegoRR.exe @0047f8c0>
void __cdecl Gods98::File_Error(const char* msg, ...)
{
	log_firstcall();

	std::va_list args;
	char buff[1024];
	va_start(args, msg);
	std::vsprintf(buff, msg, args);
	////////////////////////
	::OutputDebugStringA(buff);//CHANGE TO WHATEVER IS SUITABLE
	////////////////////////
	va_end(args);
}

// Extra functions for using with Wads.
// <LegoRR.exe @0047f900>
sint32 __cdecl Gods98::File_LoadWad(const char* fName)
{
	log_firstcall();

	//File_ErrorFile(0);			// Open the wad error log
	return Wad_Load(fName);
}

// <missing>
void __cdecl Gods98::File_CloseWad(sint32 hWad)
{
	//File_ErrorFile(0);			// Close the wad error log
	//if (hWad == WAD_ERROR) Wad_Close(hWad);
	/// FIX: originally == WAD_ERROR
	if (hWad != WAD_ERROR) Wad_Close(hWad);
}

// <missing>
void __cdecl Gods98::File_SetActiveWad(sint32 hWad)
{
	currWadHandle = hWad;
}

// <LegoRR.exe @0047f920>
bool32 __cdecl Gods98::File_GetCDFilePath(IN OUT char* path, const char* fname)
{
	log_firstcall();

	if (fileGlobs.cdLetter != '\0') {
		std::sprintf(path, "%c:\\%s\\%s", fileGlobs.cdLetter, FILE_DATADIRNAME, fname);
		return true;
	}

	return false;
}

// C library implementation

// <LegoRR.exe @0047f960>
void __cdecl Gods98::File_MakeDir(const char* path)
{
	log_firstcall();

	char name[FILE_MAXPATH];

	std::sprintf(name, "%s\\%s", FILE_DATADIRNAME, path);

	for (char* s = name; *s != '\0'; s++) {
		if (*s == '\\') {
			*s = '\0';
			::_mkdir(name);
			*s = '\\';
		}
	}
}

// <LegoRR.exe @0047f9a0>
Gods98::File* __cdecl Gods98::File_Open(const char* fName, const char* mode)
{
	log_firstcall();

	const char* fullName = File_VerifyFilename(fName);
	FileSys fs = _File_CheckSystem(fullName, mode);
	File* file;

	switch (fs)
	{
	
	case FileSys::Standard: {
		file = _File_Alloc(fs); if (!file) return nullptr;
		file->std = std::fopen(fullName, mode);
		if (file->std)
		{
			File_ErrorFile("STD Load %s\n", fullName);
			return file;
		}
		else
		{
			if (Util_StrIStr(mode, "w") == nullptr) {
				char cdName[FILE_MAXPATH];
				if (File_GetCDFilePath(cdName, fName)) {
					if (file->std = std::fopen(cdName, mode)) return file;
				}
			}

			File_ErrorFile("STD Fail %s\n", fullName);
			_File_Dealloc(file);
			return nullptr;
		}
		break;
	}
	case FileSys::Wad: {
		file = _File_Alloc(fs); if (!file) return nullptr;
		if (_File_OpenWad(file->wad, _File_GetWadName(fullName)))
		{
			File_ErrorFile("WAD Load %s\n", _File_GetWadName(fullName));
			return file;
		}
		else
		{
			File_ErrorFile("WAD Fail %s\n", _File_GetWadName(fullName));
			_File_Dealloc(file);
			return nullptr;
		}
		break;
	}
	case FileSys::Error:
	default:
		File_Error("%s(%i) : Error in call to %s\n", __FILE__, __LINE__, "File_Open");
		break;
	}
	return nullptr;
}

// <LegoRR.exe @0047fb10>
sint32 __cdecl Gods98::File_Seek(File* f, sint32 pos, SeekOrigin mode)
{
	log_firstcall();

	FileSys fs = _File_GetSystem(f);
	switch (fs)
	{
	case FileSys::Standard:
		return (sint32)std::fseek(StdFile(f), pos, (sint32)mode);

	case FileSys::Wad:
		switch (mode)
		{
		case SeekOrigin::Set:
			WadFile(f)->streamPos = pos;
			if (WadFile(f)->streamPos > Wad_hLength(WadFile(f)->hFile))
				WadFile(f)->streamPos = Wad_hLength(WadFile(f)->hFile);
			if (WadFile(f)->streamPos < 0) WadFile(f)->streamPos = 0;
			break;
		case SeekOrigin::Current:
			WadFile(f)->streamPos += pos;
			if (WadFile(f)->streamPos > Wad_hLength(WadFile(f)->hFile))
				WadFile(f)->streamPos = Wad_hLength(WadFile(f)->hFile);
			if (WadFile(f)->streamPos < 0) WadFile(f)->streamPos = 0;
			break;
		case SeekOrigin::End:
			WadFile(f)->streamPos = Wad_hLength(WadFile(f)->hFile) + pos;
			break;
		default:
			File_Error("Uknown seek mode (%i)", (sint32)mode);
		}
		break;

	case FileSys::Error:
	default:
		File_Error("%s(%i) : Unknown file system in call to %s", __FILE__, __LINE__, "File_Seek");
		break;
	}
	return 0;
}

// <LegoRR.exe @0047fc40>
sint32 __cdecl Gods98::File_Read(OUT void* buffer, sint32 size, sint32 count, File* f)
{
	log_firstcall();

	FileSys fs = _File_GetSystem(f);
	switch (fs)
	{
	case FileSys::Standard:
		return (sint32)std::fread(buffer, size, count, StdFile(f));

	case FileSys::Wad: {
		int len = Wad_hLength(WadFile(f)->hFile);
		int amountToCopy;
		if ((WadFile(f)->streamPos + (size * count)) > len) amountToCopy = len - WadFile(f)->streamPos;
		else amountToCopy = size * count;
		std::memcpy(buffer, (char*)Wad_hData(WadFile(f)->hFile) + WadFile(f)->streamPos, amountToCopy);
		WadFile(f)->streamPos += amountToCopy;
		return amountToCopy / size;
	}
	case FileSys::Error:
	default:
		File_Error("%s(%i) : Unknown file system in call to %s", __FILE__, __LINE__, "File_Read");
		break;
	}
	return 0;
}

// <LegoRR.exe @0047fd10>
sint32 __cdecl Gods98::File_Write(const void* buffer, sint32 size, sint32 count, File* f)
{
	log_firstcall();

	FileSys fs = _File_GetSystem(f);
	switch (fs)
	{
	case FileSys::Standard:
		return (sint32)std::fwrite(buffer, size, count, StdFile(f));

	case FileSys::Wad:
		File_Error("Cannot write to a file stored in a Wad!");
		break;
	case FileSys::Error:
	default:
		File_Error("%s(%i) : Unknown file system in call to %s", __FILE__, __LINE__, "File_Write");
		break;
	}
	return 0;
}

// <LegoRR.exe @0047fd80>
sint32 __cdecl Gods98::File_Close(File* f)
{
	log_firstcall();

	FileSys fs = _File_GetSystem(f);
	switch (fs)
	{
	case FileSys::Standard:
	case FileSys::Wad:
		_File_Dealloc(f);
		break;
	case FileSys::Error:
	default:
		File_Error("%s(%i) : Unknown file system in call to %s", __FILE__, __LINE__, "File_Close");
		break;
	}
	return 0;
}

// <missing>
sint32 __cdecl Gods98::File_EOF(File* f)
{
	FileSys fs = _File_GetSystem(f);
	switch (fs)
	{
	case FileSys::Standard:
		return (sint32)std::feof(StdFile(f));

	case FileSys::Wad:
		return WadFile(f)->streamPos >= (Wad_hLength(WadFile(f)->hFile) - 1);

	case FileSys::Error:
	default:
		File_Error("%s(%i) : Unknown file system in call to %s", __FILE__, __LINE__, "File_EOF");
		break;
	}
	return 0;
}

// <LegoRR.exe @0047fdd0>
sint32 __cdecl Gods98::File_Tell(File* f)
{
	log_firstcall();

	FileSys fs = _File_GetSystem(f);
	switch (fs)
	{
	case FileSys::Standard:
		return (sint32)std::ftell(StdFile(f));

	case FileSys::Wad:
		return WadFile(f)->streamPos;

	case FileSys::Error:
	default:
		File_Error("%s(%i) : Unknown file system in call to %s", __FILE__, __LINE__, "File_Tell");
		break;
	}
	return 0;
}

// <missing>
sint32 __cdecl Gods98::File_Flush(File* f)
{
	FileSys fs = _File_GetSystem(f);
	switch (fs)
	{
	case FileSys::Standard:
		return (sint32)std::fflush(StdFile(f));

	case FileSys::Wad:
		return 0;

	case FileSys::Error:
	default:
		File_Error("%s(%i) : Unknown file system in call to %s", __FILE__, __LINE__, "File_Flush");
		break;
	}
	return 0;
}

// <LegoRR.exe @0047fe20>
bool32 __cdecl Gods98::File_Exists(const char* fName)
{
	log_firstcall();

	const char* fullName = File_VerifyFilename(fName);
	FileSys fs = _File_CheckSystem(fullName, "r");
	switch (fs)
	{
	case FileSys::Standard: {
		FILE* f = std::fopen(fullName, "r");
		if (f)
		{
			std::fclose(f);
			return true;
		} else {
			char cdName[FILE_MAXPATH];
			if (File_GetCDFilePath(cdName, fName)) {
				if (f = std::fopen(cdName, "r")) {
					std::fclose(f);
					return true;
				}
			}
		}
		break;
	}
	case FileSys::Wad:
		return Wad_IsFileInWad(fullName, currWadHandle);

	case FileSys::Error:
	default:
		break;
	}
	return false;
}

// <LegoRR.exe @0047fee0>
sint32 __cdecl Gods98::File_GetC(File* f)
{
	log_firstcall();

	FileSys fs = _File_GetSystem(f);
	switch (fs)
	{
	case FileSys::Standard:
		return (sint32)std::fgetc(StdFile(f));

	case FileSys::Wad: {
		sint32 len = Wad_hLength(WadFile(f)->hFile);
		if (WadFile(f)->streamPos >= len - 1)
		{
			return EOF;
		}
		else
		{
			int c = *((int*)((char*)Wad_hData(WadFile(f)->hFile) + WadFile(f)->streamPos));
			WadFile(f)->streamPos++;
			return c;
		}
	}
	case FileSys::Error:
	default:
		File_Error("%s(%i) : Unknown file system in call to %s", __FILE__, __LINE__, "File_GetC");
		break;
	}
	return 0;
}

// <LegoRR.exe @0047ff60>
sint32 __cdecl Gods98::File_Length(File* f)
{
	log_firstcall();

	sint32 pos = File_Tell(f);
	File_Seek(f, 0, SeekOrigin::End);
	sint32 len = File_Tell(f);
	File_Seek(f, pos, SeekOrigin::Set);
	return len;
}

// <LegoRR.exe @0047ffa0>
char* __cdecl Gods98::File_InternalFGetS(OUT char* fgetsBuffer, sint32 num, File* f)
{
	log_firstcall();

	sint32 pos = 0;
	while ((fgetsBuffer[pos] = (char)File_GetC(f)) != '\0' && fgetsBuffer[pos] != '\n' && fgetsBuffer[pos] != EOF && pos != num) {
		pos++;
	}
	fgetsBuffer[pos + 1] = '\0';
	if (pos == 0) return nullptr;
	else return fgetsBuffer;
}

// <LegoRR.exe @00480000>
char* __cdecl Gods98::File_GetS(OUT char* fgetsBuffer, sint32 num, File* f)
{
	log_firstcall();

	FileSys fs = _File_GetSystem(f);
	switch (fs)
	{
	case FileSys::Standard:
		return std::fgets(fgetsBuffer, num, StdFile(f));

	case FileSys::Wad:
		return File_InternalFGetS(fgetsBuffer, num, f);

	case FileSys::Error:
	default:
		File_Error("%s(%i) : Unknown file system in call to %s", __FILE__, __LINE__, "File_GetS");
		break;
	}
	return 0;
}

// <LegoRR.exe @00480070>
sint32 __cdecl Gods98::File_PrintF(File* f, const char* msg, ...)
{
	log_firstcall();

	FileSys fs = _File_GetSystem(f);

	std::va_list arg;
	va_start(arg, msg);
	/// FIXME: other FileSys types do not call va_end(?)

	switch (fs)
	{
	case FileSys::Standard: {
		/// FIXME: ret is not returned
		sint32 ret = std::vfprintf(StdFile(f), msg, arg);
		va_end(arg);
		break;
	}
	case FileSys::Wad:
		File_Error("\"fprintf\" is unsupprted for wad files");
		break;
	case FileSys::Error:
	default:
		File_Error("%s(%i) : Unknown file system in call to %s", __FILE__, __LINE__, "File_PrintF");
		break;
	}
	return 0;
}

// <missing>
uint32 __cdecl Gods98::File_VPrintF(File* f, const char* msg, std::va_list args)
{
	FileSys fs = _File_GetSystem(f);

	if (FileSys::Standard == fs)
	{
		char buffer[1024];

		uint32 len = (uint32)std::vsprintf(buffer, msg, args);
		Error_Fatal(len >= sizeof(buffer), "Buffer too small");
		File_Write(buffer, sizeof(char), len, f);

		return len;
	}

	return 0;
}

// <missing>
sint32 __cdecl Gods98::File_ScanF(File* f, const char* msg, ...)
{
	/// FIXME: not implemented by GODS98
	return 0;
}


// <LegoRR.exe @004800e0>
Gods98::FileSys __cdecl Gods98::_File_GetSystem(File* f)
{
	log_firstcall();

	return f->type;
}

// <LegoRR.exe @004800f0>
Gods98::FileSys __cdecl Gods98::_File_CheckSystem(const char* fName, const char* mode)
{
	log_firstcall();

	if (!fName || !mode || !std::strlen(fName) || !std::strlen(mode)) return FileSys::Error;

	if (mode[0] == 'w' || mode[0] == 'W')
	{
		// File must be opened as stdC
		return FileSys::Standard;
	}
	else
	{
		if (Wad_IsFileInWad(_File_GetWadName(fName), currWadHandle) != WAD_ERROR)
//		if (currWadHandle != WAD_ERROR && Wad_IsFileInWad(_File_GetWadName(fName), currWadHandle) != WAD_ERROR)
		{
			// The file is in the wad so we can use the wad version
			return FileSys::Wad;
		}	// Otherwise we will try the normal file system
		else return FileSys::Standard;
	}
}

// <LegoRR.exe @00480160>
bool32 __cdecl Gods98::_File_OpenWad(WADFILE* wad, const char* fName)
{
	log_firstcall();

	wad->eof = false;
	wad->streamPos = 0;
	wad->hFile = Wad_FileOpen(fName, currWadHandle);
	return (bool32)(wad->hFile == WAD_ERROR ? false : true);
}

// <LegoRR.exe @00480190>
Gods98::File* __cdecl Gods98::_File_Alloc(FileSys fType)
{
	log_firstcall();

	if (fType == FileSys::Standard)
	{
		File* f;
		f = (File*)_File_Malloc(sizeof(File));
		f->type = fType;
		return f;
	}
	else if (fType == FileSys::Wad)
	{
		File* f = (File*)_File_Malloc(sizeof(File));
		if (f)
		{
			f->type = fType;
			f->wad = (WADFILE*)_File_Malloc(sizeof(WADFILE));
			if (f->wad) return f;
			else _File_Free(f);
		}
	}
	return nullptr;
}

// <LegoRR.exe @004801f0>
void* __cdecl Gods98::_File_Malloc(uint32 size)
{
	log_firstcall();

	return Mem_Alloc(size);	// Change to whatever is required
}

// <LegoRR.exe @00480200>
void __cdecl Gods98::_File_Free(void* ptr)
{
	log_firstcall();

	Mem_Free(ptr);				// Change to whatever is required
}

// <LegoRR.exe @00480210>
void __cdecl Gods98::_File_Dealloc(File* file)
{
	log_firstcall();

	if (file)
	{
		if (_File_GetSystem(file) == FileSys::Standard)
		{
			if (file->std) std::fclose(StdFile(file));
			_File_Free(file);
		}
		else if (_File_GetSystem(file) == FileSys::Wad)
		{
			if (file->wad)
			{
				Wad_FileClose(file->wad->hFile);
				_File_Free(file->wad);
			}
			_File_Free(file);
		}
	}
}

// <LegoRR.exe @00480280>
const char* __cdecl Gods98::_File_GetWadName(const char* fName)
{
	log_firstcall();

	//static char wadedName[MAX_WAD_BASE_PATH];
	//fileGlobs.s_GetWadName_wadedName

	if (fileGlobs.basePathSet) {
		int fLen = std::strlen(fName);
		int wLen = std::strlen(fileGlobs.wadBasePath);
		if (fLen <= wLen) {
			return fName;
		}
		else {
			char copy = fName[wLen];
			const_cast<char*>(fName)[wLen] = '\0';
			if (!::_stricmp(fName, fileGlobs.wadBasePath)) {
				const char* ptr = fName + wLen + 1;
				std::sprintf(fileGlobs.s_GetWadName_wadedName, "%s", ptr);
				const_cast<char*>(fName)[wLen] = copy;
				return fileGlobs.s_GetWadName_wadedName;
			}
			else {
				const_cast<char*>(fName)[wLen] = copy;
				return fName;
			}
		}
	}
	else return fName;
}

// <LegoRR.exe @00480310>
char* __cdecl Gods98::File_GetLine(OUT char* buffer, uint32 size, File* file)
{
	log_firstcall();

	// Same as File_GetS() but without the return character.

	char* res = File_GetS(buffer, size, file);
	uint32 length = std::strlen(buffer) - 1;
	if (buffer[length] == '\n') buffer[length] = '\0';
	if (length && buffer[length - 1] == '\r') buffer[length - 1] = '\0';

	return res;
}

// <LegoRR.exe @00480360>
void* __cdecl Gods98::File_LoadBinary(const char* filename, OUT uint32* sizeptr)
{
	log_firstcall();

	return File_Load(filename, sizeptr, true);
}

// <missing>
void* __cdecl Gods98::File_LoadASCII(const char* filename, OUT uint32* sizeptr)
{
	return File_Load(filename, sizeptr, false);
}

// <LegoRR.exe @00480380>
void* __cdecl Gods98::File_Load(const char* filename, OUT uint32* sizeptr, bool32 binary)
{
	log_firstcall();

	File* ifp;
	if (ifp = File_Open(filename, binary ? "rb" : "r")) {
		File_Seek(ifp, 0, SeekOrigin::End);
		uint32 size = File_Tell(ifp);

		if (fileGlobs.loadCallback) fileGlobs.loadCallback(filename, size, fileGlobs.loadCallbackData);

		char* buffer;
		if (buffer = (char*)Mem_Alloc(size)) {

			File_Seek(ifp, 0, SeekOrigin::Set);
			File_Read(buffer, sizeof(char), size, ifp);
			if (sizeptr != nullptr) *sizeptr = size;

			File_Close(ifp);
			return buffer;
		}
		File_Close(ifp);
	}

	return nullptr;
}

// <LegoRR.exe @00480430>
uint32 __cdecl Gods98::File_LoadBinaryHandle(const char* filename, OUT uint32* sizeptr)
{
	log_firstcall();

	File* ifp;
	if (ifp = File_Open(filename, "rb")) {
		File_Seek(ifp, 0, SeekOrigin::End);
		uint32 size = File_Tell(ifp);
		if (fileGlobs.loadCallback) fileGlobs.loadCallback(filename, size, fileGlobs.loadCallbackData);

		uint32 handle;
		if ((handle = Mem_AllocHandle(size)) != -1) {
			char* buffer = (char*) Mem_AddressHandle(handle);
			File_Seek(ifp, 0, SeekOrigin::Set);
			File_Read(buffer, sizeof(char), size, ifp);
			if (sizeptr != nullptr) *sizeptr = size;

			File_Close(ifp);
			return handle;
		}
		File_Close(ifp);
	}

	return (uint32)-1;
}

// <LegoRR.exe @004804e0>
const char* __cdecl Gods98::File_VerifyFilename(const char* filename)
{
	log_firstcall();

	//static char full[_MAX_PATH];
	//fileGlobs.s_VerifyFilename_full

	char part[_MAX_PATH];

	if (filename != nullptr) {
		//REMOVE THE LEADING FORWARD SLASH IF ANY
		const char* temp = filename;
		if (*temp == '\\')
			temp++;

		/*if (*temp == '@')
		{
			temp++;
			std::sprintf(fileGlobs.s_VerifyFilename_full, "%s\\%s", fileGlobs.exeDir, temp);
			return fileGlobs.s_VerifyFilename_full;
		}
		else*/
			std::sprintf(part, "%s\\%s", fileGlobs.dataDir, temp);

		if (::_fullpath(fileGlobs.s_VerifyFilename_full, part, sizeof(fileGlobs.s_VerifyFilename_full)) != nullptr) {
			if (std::strncmp(fileGlobs.s_VerifyFilename_full, fileGlobs.dataDir, std::strlen(fileGlobs.dataDir)) == 0) {
				return fileGlobs.s_VerifyFilename_full;
			}
		}
	}

	Error_Warn(true, Error_Format("Cannot verify file name \"%s\".", filename));
	Error_LogLoadError(true, Error_Format("%d\t%s", (sint32)Error_LoadError::UnableToVerifyName, filename));

	return nullptr;
}

// <LegoRR.exe @00480570>
void __cdecl Gods98::File_SetLoadCallback(FileLoadCallback Callback, void* data)
{
	log_firstcall();

	fileGlobs.loadCallback = Callback;
	fileGlobs.loadCallbackData = data;
}

// <LegoRR.exe @00480590>
void __cdecl Gods98::File_CheckRedundantFiles(const char* logName)
{
	log_firstcall();

	char fileName[1024];


	// Yep... this is as broken as it looks
	FILE* fileList;
	if (fileList = (FILE*)File_Open(logName, "r")) {
		checkGlobs.numInList = 0;

		while (std::fscanf(fileList, "%s", fileName) != EOF) {
			Error_Fatal(checkGlobs.numInList == FILE_DEBUG_RLISTSIZE, "FILE_DEBUG_RLISTSIZE too small");
			std::strcpy(checkGlobs.loadedList[checkGlobs.numInList++], fileName);
		}

		File_CheckDirectory(fileGlobs.dataDir);

		std::fclose(fileList);

	}
}

// <LegoRR.exe @00480650>
void __cdecl Gods98::File_CheckDirectory(const char* dirName)
{
	log_firstcall();

	_finddata32_t c_file;
	intptr_t hFile;
	char name[1024];


	std::strcpy(name, dirName);
	std::strcat(name, "\\*.*");

	if ((hFile = _findfirst32(name, &c_file)) == -1L)
		return;
	else
	{
		do
		{
			if (std::strcmp(c_file.name, ".") == 0 || std::strcmp(c_file.name, "..") == 0)
				continue;

			std::strcpy(name, dirName);
			std::strcat(name, "\\");
			std::strcat(name, c_file.name);

			if (c_file.attrib & _A_SUBDIR)
				File_CheckDirectory(name);
			else
				File_CheckFile(name);

		} while (_findnext32(hFile, &c_file) == 0);

		_findclose(hFile);
	}
}

// <LegoRR.exe @00480830>
void __cdecl Gods98::File_CheckFile(const char* fileName)
{
	log_firstcall();

	for (uint32 loop = 0; loop < checkGlobs.numInList; loop++)
		if (!::_stricmp(fileName, checkGlobs.loadedList[loop]))
			return;

	Error_LogRedundantFile(true, fileName);
}


// <missing>
void __cdecl Gods98::File_SetDataDir(const char* newDataDir)
{
	std::strcpy(fileGlobs.dataDir, newDataDir);
}

#pragma endregion
