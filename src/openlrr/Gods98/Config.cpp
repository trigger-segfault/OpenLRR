
#include "Files.h"
#include "Memory.h"
#include "Errors.h"
#include "Utils.h"
#include "Config.h"
#include "Keys.h"
#include "Maths.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00507098>
Gods98::Config_Globs & Gods98::configGlobs = *(Gods98::Config_Globs*)0x00507098; // = { nullptr };

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @004790b0>
void __cdecl Gods98::Config_Initialise(void)
{
	log_firstcall();

	for (uint32 loop = 0; loop < CONFIG_MAXLISTS; loop++) {
		configGlobs.listSet[loop] = nullptr;
	}

	configGlobs.freeList = nullptr;
	configGlobs.listCount = 0;
	configGlobs.flags = Config_GlobFlags::CONFIG_FLAG_INITIALISED;
}

// <LegoRR.exe @004790e0>
void __cdecl Gods98::Config_Shutdown(void)
{
	log_firstcall();

	for (uint32 loop = 0; loop < CONFIG_MAXLISTS; loop++) {
		if (configGlobs.listSet[loop]) Mem_Free(configGlobs.listSet[loop]);
	}

	configGlobs.freeList = nullptr;
	configGlobs.flags = Config_GlobFlags::CONFIG_FLAG_NONE;
}

/*void Config_SetCharacterTable(const char* fname);
void Config_SetCharacterConvertFile(const char* fname);
void Config_ReadCharacterTable(const char* fname);
char __cdecl Config_ConvertCharacter(char c);
void __cdecl Config_SetLanguageDatabase(const char* langFile);
void __cdecl Config_ReadLanguageDatabase(const char* langFile);
char* __cdecl Config_ConvertString(const char* s, const char* sectionName, uint32* size, sint32 spaceToUnderscore);
void __cdecl Config_DumpUnknownPhrases(const char* fname);
void* __cdecl Config_LoadConvertedText(const char* fname, uint32* fileSize);*/


//#define _ISSPACE(c) (('\t' == (c)) || ('\n' == (c)) || ('\r' == (c)) || (' ' == (c)))

// <LegoRR.exe @00479120>
Gods98::Config* __cdecl Gods98::Config_Load(const char* filename)
{
	log_firstcall();

	Config *conf, *rootConf = nullptr;
	uint32 fileSize, loop;
	char* s;
	char* fdata;

	configGlobs.flags |= Config_GlobFlags::CONFIG_FLAG_LOADINGCONFIG;

	/// FIXME: Load into buffer with one extra byte for null-termination
	if (fdata = (char*)File_LoadBinary(filename, &fileSize)){

		conf = rootConf = Config_Create(nullptr);
		rootConf->fileData = fdata;

		// Change any return/tab/blah/blah characters to zero...
		// Clear anything after a semi-colon until the next return character.

		bool32 commentMode = false;
		for (s=rootConf->fileData,loop=0 ; loop<fileSize ; loop++){

			if (*s == ';') commentMode = true;
			else if (*s == '\n') commentMode = false;

			//if (commentMode || _ISSPACE(*s)) *s = '\0';
			if (commentMode || (*s == '\t' || *s == '\n' || *s == '\r' || *s == ' ')) *s = '\0';

			s++;
		}

		// Replace the semi-colons that were removed by the language converter...
		//for (loop=0 ; loop<fileSize ; loop++) if (FONT_LASTCHARACTER + 1 == rootConf->fileData[loop]) rootConf->fileData[loop] = ';';

		// Run through the file data and point in the config structures

		for (s=rootConf->fileData, loop=0 ; loop<fileSize ; loop++){
			if (*s != '\0'){
				if (*s == '}' && *(s+1) == '\0') conf->depth--;
				else if (conf->itemName == nullptr) {
					Error_Fatal((*s == '{'), "Config");
					conf->itemName = s;
				}
				else {
					conf->dataString = s;
					conf = Config_Create(conf);
					if (*s == '{' && *(s+1) == '\0') conf->depth++;
				}

				for ( ; loop<fileSize ; loop++) if (*(s++) == '\0') break;
			} else s++;
		}

	}

	configGlobs.flags &= ~Config_GlobFlags::CONFIG_FLAG_LOADINGCONFIG;

	return rootConf;
}

// <LegoRR.exe @00479210>
const char* __cdecl Gods98::Config_BuildStringID(const char* s, ...)
{
	log_firstcall();

	std::va_list args;
	//static char str[1024];
	//configGlobs.s_JoinPath_string
	const char* curr;

	std::strcpy(configGlobs.s_JoinPath_string, s);

	va_start(args, s);
	while (curr = va_arg(args, const char*)) {
		std::strcat(configGlobs.s_JoinPath_string, CONFIG_SEPARATOR);
		std::strcat(configGlobs.s_JoinPath_string, curr);
	}
	va_end(s);

	return configGlobs.s_JoinPath_string;
}

/*
// <inlined>
__inline const char* Gods98::Config_GetItemName(const Config* conf)
{
	return conf->itemName;
}

// <inlined>
__inline const char* Gods98::Config_GetDataString(const Config* conf)
{
	return conf->dataString;
}
*/

// <LegoRR.exe @004792b0>
const Gods98::Config* __cdecl Gods98::Config_FindArray(const Config* root, const char* name)
{
	log_firstcall();

	log_firstcall();

	const Config* conf;
	if (conf = Config_FindItem(root, name)) {
		if (conf->depth < conf->linkNext->depth) return conf->linkNext;
	}
	return nullptr;
}

// <LegoRR.exe @004792e0>
const Gods98::Config* __cdecl Gods98::Config_GetNextItem(const Config* start)
{
	log_firstcall();

	uint32 level = start->depth;
	const Config* conf = start;

	while (conf) {
		if (conf->depth < level) return nullptr;
		conf = conf->linkNext;
		if (conf->depth == level) return conf;
	}
	return nullptr;
}

//char* __cdecl Config_GetStringValue(const Config* root, const char* stringID, unsigned long line, const char* file);

// <LegoRR.exe @00479310>
char* Gods98::Config_GetStringValue(const Config* root, const char* stringID)
{
	log_firstcall();

	const Config* conf;
	if (conf = Config_FindItem(root, stringID)) {
		if (conf->dataString) {
			char* str = (char*)Mem_Alloc(std::strlen(conf->dataString) + 1);
			std::strcpy(str, conf->dataString);
			return str;
		}
	}

	return nullptr;
}

// <LegoRR.exe @00479370>
const char* __cdecl Gods98::Config_GetTempStringValue(const Config* root, const char* stringID)
{
	log_firstcall();

	const Config* conf;
	if (conf = Config_FindItem(root, stringID)) {
		return conf->dataString;
	}
	return nullptr;
}

// <LegoRR.exe @00479390>
BoolTri __cdecl Gods98::Config_GetBoolValue(const Config* root, const char* stringID)
{
	log_firstcall();

	BoolTri res = BoolTri::BOOL3_ERROR;

	/// FIX LEGORR: Don't bother allocating a new string,
	///              Util_GetBoolFromString won't modify it.
	const char* str;
	if (str = Config_GetTempStringValue(root, stringID)) {
		res = Util_GetBoolFromString(str);
	}

	/*char* str;
	if (str = Config_GetStringValue(root, stringID)) {

		res = Util_GetBoolFromString(str);
		Mem_Free(str);
	}*/

	return res;
}

// <LegoRR.exe @004793d0>
real32 __cdecl Gods98::Config_GetAngle(const Config* root, const char* stringID)
{
	log_firstcall();

	real32 degrees;

	if (degrees = Config_GetRealValue(root, stringID)) {
		return (degrees / 360.0f) * (2.0f * M_PI);
	}

	return 0.0f;
}

// <LegoRR.exe @00479430>
bool32 __cdecl Gods98::Config_GetRGBValue(const Config* root, const char* stringID, OUT real32* r, OUT real32* g, OUT real32* b)
{
	log_firstcall();

	char* argv[100];
	bool32 res = false;

	char *str;
	if (str = Config_GetStringValue(root, stringID)) {
		if (Util_Tokenise(str, argv, ":") == 3) {
			*r = atoi(argv[0]) / 255.0f;
			*g = atoi(argv[1]) / 255.0f;
			*b = atoi(argv[2]) / 255.0f;

			res = true;

		}
		else Error_Warn(true, Error_Format("Invalid RBG entry '%s'", Config_GetTempStringValue(root, stringID)));

		Mem_Free(str);
	}

	return res;
}


// <missing>
bool32 __cdecl Gods98::Config_GetCoord(const Config* root, const char* stringID, OUT real32* x, OUT real32* y, OUT real32* z)
{
	char* argv[100];
	char *str;
	bool32 res = false;

	Error_Fatal(!x || !y, "Null passed as x or y");

	if (str = Config_GetStringValue(root, stringID)) {
		if (z == nullptr) {
			if (Util_Tokenise(str, argv, ",") != 2) Error_Warn(TRUE, Error_Format("Invalid 2D Coordinate entry '%s'", str));
		}
		else if (Util_Tokenise(str, argv, ",") == 3) *z = (real32)std::atof(argv[2]);
		else {
			Error_Warn(true, Error_Format("Invalid 3D Coordinate entry '%s'", str));
			res = false;
		}
		*x = (real32)std::atof(argv[0]);
		*y = (real32)std::atof(argv[1]);

		Mem_Free(str);
		res = true;
	}

	return res;
}


// <missing>
bool32 __cdecl Gods98::Config_GetKey(const Config* root, const char* stringID, OUT Keys* key)
{
	const char* str;

	if (str = Config_GetTempStringValue(root, stringID)) {
		return Key_Find(str, key);
	}

	return false;
}


// <debug>
const char* __cdecl Gods98::Config_Debug_GetLastFind(void)
{
	return "<not implemented>";
}

// <LegoRR.exe @00479500>
void __cdecl Gods98::Config_Free(Config* root)
{
	log_firstcall();

	Error_Fatal(root->fileData = nullptr, "Only pass the root (loaded) config structure to Config_Free()");

	Mem_Free(root->fileData);
	while (root) {
		Config* next = const_cast<Config*>(root->linkNext);
		Config_Remove(root);
		root = next;
	}
}

//const char* __cdecl Config_Debug_GetLastFind(void);

// <LegoRR.exe @00479530>
Gods98::Config* __cdecl Gods98::Config_Create(Config* prev)
{
	log_firstcall();

	Config_CheckInit();

	if (configGlobs.freeList == nullptr) Config_AddList();

	Config* newConfig = configGlobs.freeList;
	configGlobs.freeList = newConfig->nextFree;
	newConfig->nextFree = newConfig;

	newConfig->itemName = nullptr;
	newConfig->dataString = nullptr;
	newConfig->linkNext = nullptr;

	if (prev != nullptr) {
		prev->linkNext = newConfig;
		newConfig->linkPrev = prev;
		newConfig->depth = prev->depth;
		//		newConfig->fileData = prev->fileData;
		newConfig->fileData = nullptr;
	}
	else {
		newConfig->depth = 0;
		newConfig->linkPrev = nullptr;
	}

//#ifdef _DEBUG
//	newConfig->accessed = false;
//#endif // _DEBUG

	return newConfig;
}

// <LegoRR.exe @00479580>
void __cdecl Gods98::Config_Remove(Config* dead)
{
	log_firstcall();

	Config_CheckInit();
	Error_Fatal(!dead, "NULL passed to Config_Remove()");

//#ifdef _DEBUG
//	if (configGlobs.debugLastFind == dead) configGlobs.debugLastFind = NULL;
//#endif // _DEBUG

	dead->nextFree = configGlobs.freeList;
	configGlobs.freeList = dead;
}

// <LegoRR.exe @004795a0>
const Gods98::Config* __cdecl Gods98::Config_FindItem(const Config* conf, const char* stringID)
{
	log_firstcall();

	// Search the list for the given item.

	char * argv[CONFIG_MAXDEPTH];
	uint32 count, currDepth;
	const Config* backConf;
	const Config* foundConf = nullptr;
	char* tempstring = (char*)Mem_Alloc(std::strlen(stringID) + 1);
	bool32 wildcard;

	std::strcpy(tempstring, stringID);

	count = Util_Tokenise(tempstring, argv, "::");

	// First find anything that matches the depth of the request
	// then see if the hierarchy matches the request.

	while (conf) {
		if (conf->depth == count - 1) {

			wildcard = false;

			if (count == 1) {
				const char* s;
				uint32 index = 0;
				for (s = conf->itemName; *s != '\0'; s++) { if (*s == '*') break; index++; }
				if (*s == '*') {
					wildcard = (::_strnicmp(argv[count - 1], conf->itemName, index) == 0);
				}
			}

			if (wildcard || ::_stricmp(argv[count - 1], conf->itemName) == 0) {

				wildcard = false;

				// Look backwards down the list to check the hierarchy.
				currDepth = count - 1;
				backConf = conf;
				while (backConf) {
					if (backConf->depth == currDepth - 1) {
						if (currDepth == 1) {
							const char* s;
							uint32 index = 0;
							for (s = backConf->itemName; *s != '\0'; s++) { if (*s == '*') break; index++; }
							if (*s == '*') {
								wildcard = !::_strnicmp(argv[currDepth - 1], backConf->itemName, index);
							}
						}

						if (wildcard || ::_stricmp(argv[currDepth - 1], backConf->itemName) == 0) {
							currDepth--;
						}
						else break;
					}
					backConf = backConf->linkPrev;
				}
				if (currDepth == 0) {
					foundConf = conf;
					if (!wildcard) break;
				}
			}
		}
		conf = conf->linkNext;
	}

	Mem_Free(tempstring);

//#ifdef _DEBUG
//	if (foundConf) {
//		configGlobs.debugLastFind = foundConf;
//		foundConf->accessed = true;
//	}
//	else {
//		foundConf = nullptr;
//	}
//#endif // _DEBUG

	return foundConf;
}

//void __cdecl Gods98::Config_DumpStructure(const Config* conf);

// <LegoRR.exe @00479750>
void __cdecl Gods98::Config_AddList(void)
{
	log_firstcall();

	Config_CheckInit();

	Error_Fatal(configGlobs.listCount + 1 >= CONFIG_MAXLISTS, "Run out of lists");

	uint32 count = 0x00000001 << configGlobs.listCount;

	Config* list;
	if (list = configGlobs.listSet[configGlobs.listCount] = (Config*)Mem_Alloc(sizeof(Config) * count)) {

		configGlobs.listCount++;

		for (uint32 loop = 1; loop < count; loop++) {

			list[loop - 1].nextFree = &list[loop];
		}
		list[count - 1].nextFree = configGlobs.freeList;
		configGlobs.freeList = list;

	}
	else Error_Fatal(true, Error_Format("Unable to allocate %d bytes of memory for new list.\n", sizeof(Config) * count));
}

//void __cdecl Gods98::Config_RunThroughLists(void);

//__inline const char* Gods98::Config_GetItemName(const Config* conf) { return conf->itemName; }
//__inline const char* Gods98::Config_GetDataString(const Config* conf) { return conf->dataString; }



#pragma endregion
