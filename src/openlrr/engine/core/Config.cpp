// Config.cpp : 
//

#include "../input/Keys.h"
#include "Files.h"
#include "Errors.h"
#include "Maths.h"         // defines
#include "Memory.h"
#include "Utils.h"

#include "Config.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00507098>
Gods98::Config_Globs & Gods98::configGlobs = *(Gods98::Config_Globs*)0x00507098; // = { nullptr };

Gods98::Config_ListSet Gods98::configListSet = Gods98::Config_ListSet(Gods98::configGlobs);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @004790b0>
void __cdecl Gods98::Config_Initialise(void)
{
	log_firstcall();

	configListSet.Initialise();
	configGlobs.flags = Config_GlobFlags::CONFIG_GLOB_FLAG_INITIALISED;

	// Clear this to avoid undefined behaviour with Config_LastStringID().
	std::memset(configGlobs.s_JoinPath_string, 0, sizeof(configGlobs.s_JoinPath_string));
}

// <LegoRR.exe @004790e0>
void __cdecl Gods98::Config_Shutdown(void)
{
	log_firstcall();

	configListSet.Shutdown();
	configGlobs.flags = Config_GlobFlags::CONFIG_GLOB_FLAG_NONE;
}

/*void __cdecl Gods98::Config_SetCharacterTable(const char* fname);
void __cdecl Gods98::Config_SetCharacterConvertFile(const char* fname);
void __cdecl Gods98::Config_ReadCharacterTable(const char* fname);
char __cdecl Gods98::Config_ConvertCharacter(char c);
void __cdecl Gods98::Config_SetLanguageDatabase(const char* langFile);
void __cdecl Gods98::Config_ReadLanguageDatabase(const char* langFile);
char* __cdecl Gods98::Config_ConvertString(const char* s, const char* sectionName, uint32* size, sint32 spaceToUnderscore);
void __cdecl Gods98::Config_DumpUnknownPhrases(const char* fname);
void* __cdecl Gods98::Config_LoadConvertedText(const char* fname, uint32* fileSize);*/


// <LegoRR.exe @00479120>
Gods98::Config* __cdecl Gods98::Config_Load(const char* filename)
{
	return Config_Load2(filename, FileFlags::FILE_FLAGS_DEFAULT);
}

/// CUSTOM: Loads a configuration file, with additional flags specifying where and what checks are used to open it.
Gods98::Config* __cdecl Gods98::Config_Load2(const char* filename, FileFlags fileFlags)
{
	log_firstcall();

	Config* rootConf = nullptr;
	uint32 fileSize, loop;
	char* s;
	char* fdata;

	configGlobs.flags |= Config_GlobFlags::CONFIG_GLOB_FLAG_LOADINGCONFIG;

	/// FIX APPLY: Use extension of File_LoadBinary function to null-terminate buffer.
	if (fdata = static_cast<char*>(File_LoadBinaryString2(filename, &fileSize, fileFlags))) {

		rootConf = Config_Create(nullptr);
		rootConf->fileData = fdata;
		rootConf->fileName = Util_StrCpy(filename);

		std::vector<uint32> lineNumberEnds;
		lineNumberEnds.push_back(0); // Line numbers are 1-indexed, so 0 doesn't exist (ends at index 0).

		// Change any whitespace characters to zero...
		// Clear anything after a semi-colon until the next return character.
		// Also find the boundaries of each line number so we can store those and give clearer error messages.

		const size_t illegalCommentLength = std::strlen(CONFIG_ILLEGALCOMMENT);
		uint32 illegalCommentCount = 0;
		uint32 illegalCommentFirstLineNumber = 0;
		uint32 illegalCommentLastLineNumber  = 0;

		bool commentMode = false;
		for (s = rootConf->fileData, loop = 0; loop < fileSize; loop++) {
			const char c = *s;

			if (c == CONFIG_COMMENTCHAR) {
				commentMode = true;
			}
			else if (c == '\n') {
				lineNumberEnds.push_back(loop);
				commentMode = false;
			}
			else if (!commentMode && ::_strnicmp(s, CONFIG_ILLEGALCOMMENT, illegalCommentLength) == 0) {
				// Count number of illegal comment appearances.
				// Only count illegal comments that are the start of a word.
				if (loop == 0 || s[-1] == '\0') {
					if (illegalCommentCount == 0) {
						illegalCommentFirstLineNumber = static_cast<uint32>(lineNumberEnds.size());
					}
					illegalCommentLastLineNumber = static_cast<uint32>(lineNumberEnds.size());
					illegalCommentCount++;
				}
			}

			if (commentMode || (c == '\t' || c == '\n' || c == '\r' || c == ' ')) *s = '\0';

			s++;
		}
		lineNumberEnds.push_back(fileSize); // End final line.


		Error_WarnF2(illegalCommentCount > 0, "%s (%i-%i): Warning: Found %i illegal \"%s\" comments.\n", filename,
					 illegalCommentFirstLineNumber, illegalCommentLastLineNumber, illegalCommentCount, CONFIG_ILLEGALCOMMENT);


		// Replace the semi-colons that were removed by the language converter...
		//for (loop = 0; loop < fileSize; loop++) {
		//	if (rootConf->fileData[loop] == FONT_LASTCHARACTER + 1)
		//		rootConf->fileData[loop] = CONFIG_COMMENTCHAR;
		//}

		// Run through the file data and point in the config structures.

		uint32 lineNumber = 0;
		Config* conf = rootConf;
		for (s = rootConf->fileData, loop = 0; loop < fileSize; loop++) {

			// Update the current line number we're on.
			while (loop >= lineNumberEnds[lineNumber]) {
				lineNumber++;
			}

			const char c = *s;

			if (c != '\0') {
				const char cnext = s[1];

				if (c == CONFIG_CLOSEBLOCKCHAR && cnext == '\0') {
					// Close block.
					Error_WarnF2((conf->itemName!=nullptr), "%s (%i): Warning: Config close brace \"%s\" used between item name \"%s\" and value.\n",
								filename, lineNumber, CONFIG_CLOSEBLOCK, conf->itemName);
					// Treat as non-fatal for when we introduce hot-reloading settings.
					Error_WarnF2((conf->depth == 0), "%s (%i): Warning: Config close brace \"%s\" used at depth 0.\n",
								 filename, lineNumber, CONFIG_CLOSEBLOCK);
					conf->depth--;
				}
				else if (conf->itemName == nullptr) {
					// Assign item key.
					Error_WarnF2((c==CONFIG_OPENBLOCKCHAR && cnext=='\0'), "%s (%i): Warning: Config open brace \"%s\" used for item name.\n",
								filename, lineNumber, CONFIG_OPENBLOCK);
					conf->itemName = s;
					conf->itemHashCode = Config_HashItemName(s);
					conf->lineNumber = lineNumber;
				}
				else {
					// Assign item value.
					conf->dataString = s;
					conf = Config_Create(conf);
					// Open block.
					if (c==CONFIG_OPENBLOCKCHAR && cnext=='\0') conf->depth++;
					Error_WarnF2((c==CONFIG_CLOSEBLOCKCHAR && cnext=='\0'), "%s (%i): Warning: Config close brace \"%s\" used for item value.\n",
								filename, lineNumber, CONFIG_CLOSEBLOCK);
				}

				// Skip until the end of the string.
				for ( ; loop < fileSize; loop++) if (*s++ == '\0') break;
			}
			else {
				s++;
			}
		}

		if (conf->itemName == nullptr) {
			if (conf == rootConf) {
				/// FIX APPLY: Handle empty config files, which should be considered invalid...?
				///            If we allow reduced strictness, then assign empty string to itemName instead.
				Error_WarnF2(true, "%s (%i): Warning: Config file has no properties.\n", filename, lineNumber);
				Config_Free(rootConf);
				rootConf = nullptr;
			}
			else {
				/// FIX APPLY: Remove empty item from the end of the config.
				///            This issue becomes relevant when fixing the wildcard bug in Config_FindItem when loading PTL files.
				Config* prevConf = conf->linkPrev;
				prevConf->linkNext = nullptr;
				Config_Remove(conf);
				conf = prevConf;
			}
		}
		else if (conf->dataString == nullptr) {
			/// NOTE: This generally won't cause any errors, because most Config functions
			///       consider a null dataString return value as a property that doesn't exist.
			Error_WarnF2(true, "%s (%i): Warning: Last property \"%s\" missing value string.\n",
						filename, conf->lineNumber, conf->itemName);
		}

	}

	configGlobs.flags &= ~Config_GlobFlags::CONFIG_GLOB_FLAG_LOADINGCONFIG;

	return rootConf;
}

// <LegoRR.exe @00479210>
const char* __cdecl Gods98::Config_BuildStringID(const char* s, ...)
{
	log_firstcall();

	std::va_list args;
	const char* curr;

	//static char s_JoinPath_string[CONFIG_MAXSTRINGID];

	// Strcpy for the first part to overwrite the previous string.
	std::strcpy(configGlobs.s_JoinPath_string, s);

	va_start(args, s);
	while ((curr = va_arg(args, const char*))) {
		std::strcat(configGlobs.s_JoinPath_string, CONFIG_SEPARATOR);
		std::strcat(configGlobs.s_JoinPath_string, curr);
	}
	va_end(args);

	return configGlobs.s_JoinPath_string;
}

/// CUSTOM: Returns the last result of Config_BuildStringID or Config_ID.
const char* Gods98::Config_LastStringID()
{
	return configGlobs.s_JoinPath_string;
}

/// CUSTOM: Builds and returns a string ID for the config property. Calling this again will invalidate the previous result.
const char* Gods98::Config_GetStringID(const Config* conf)
{
	static char s_stringID[CONFIG_MAXSTRINGID] = { '\0' };

	const char* hierarchy[CONFIG_MAXDEPTH] = { nullptr };

	for (const Config* parent = conf; parent != nullptr; parent = Config_GetParentItem(parent)) {
		hierarchy[parent->depth] = parent->itemName;
	}
	Error_FatalF(hierarchy[0] == nullptr, "Config_GetStringID: Could not find all parents for config item \"%s\"", conf->itemName);

	// Strcpy for the first part to overwrite the previous string.
	std::strcpy(s_stringID, hierarchy[0]);

	for (uint32 i = 1; i < conf->depth + 1; i++) {
		std::strcat(s_stringID, CONFIG_SEPARATOR);
		std::strcat(s_stringID, hierarchy[i]);
	}

	return s_stringID;
}

/// CUSTOM: Gets the parent property of the config property. Returns null if this is a root property.
const Gods98::Config* Gods98::Config_GetParentItem(const Config* conf)
{
	if (conf->depth > 0) {
		for (const Config* parent = conf; parent != nullptr; parent = parent->linkPrev) {
			if (parent->depth == conf->depth - 1)
				return parent;
		}
	}
	return nullptr;
}

/// CUSTOM: Gets the line number of the config property with the given string ID. Returns 0 on failure.
uint32 Gods98::Config_GetLineNumberOf(const Config* root, const char* stringID)
{
	const Config* conf;
	if (conf = Config_FindItem(root, stringID)) {
		return conf->lineNumber;
	}
	return 0;
}

/// CUSTOM: Gets the line number of the config property.
uint32 Gods98::Config_GetLineNumber(const Config* conf)
{
	return conf->lineNumber;
}

/// CUSTOM: Gets the filename of the config this property with the given string ID was loaded from. Returns "<Config>" on failure.
///         This is useful for merged configs, where a definition may be in a different file from the root config.
const char* Gods98::Config_GetFileNameOf(const Config* root, const char* stringID)
{
	return Config_GetFileName(Config_FindItem(root, stringID));
}

/// CUSTOM: Gets the filename of the config this property was loaded from. Returns "<Config>" on failure.
const char* Gods98::Config_GetFileName(OPTIONAL const Config* conf)
{
	// Walk backwards and find the root config item holding the filename.
	while (conf && conf->fileName == nullptr) {
		conf = conf->linkPrev;
	}

	return (conf && conf->fileName ? conf->fileName : "<Config>");
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

	const Config* conf;
	if (conf = Config_FindItem(root, name)) {
		/// FIX LEGORR: Ensure next linked item is not null before accessing.
		if (conf->linkNext != nullptr && conf->depth < conf->linkNext->depth) {
			return conf->linkNext;
		}
	}
	return nullptr;
}

// <LegoRR.exe @004792e0>
const Gods98::Config* __cdecl Gods98::Config_GetNextItem(const Config* start)
{
	log_firstcall();

	uint32 level = start->depth;
	const Config* conf = start;

	while (conf = conf->linkNext) {
		if (conf->depth < level) return nullptr;
		if (conf->depth == level) return conf;
	}
	return nullptr;
}

// <LegoRR.exe @00479310>
char* Gods98::Config_GetStringValue(const Config* root, const char* stringID)
{
	log_firstcall();

	const Config* conf;
	if (conf = Config_FindItem(root, stringID)) {
		if (conf->dataString) {
			return Util_StrCpy(conf->dataString);
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

	/// FIX LEGORR: Don't bother allocating a new string,
	///              Util_GetBoolFromString won't modify it.
	const char* str;
	if (str = Config_GetTempStringValue(root, stringID)) {
		return Util_GetBoolFromString(str);
	}
	return BoolTri::BOOL3_ERROR;
}

// <LegoRR.exe @004793d0>
real32 __cdecl Gods98::Config_GetAngle(const Config* root, const char* stringID)
{
	log_firstcall();

	real32 degrees;
	if (degrees = Config_GetRealValue(root, stringID)) {
		return (degrees / 360.0f) * (2.0f * M_PI); // degrees to radians
	}
	return 0.0f;
}

// <LegoRR.exe @00479430>
bool32 __cdecl Gods98::Config_GetRGBValue(const Config* root, const char* stringID, OUT real32* r, OUT real32* g, OUT real32* b)
{
	log_firstcall();

	char* argv[3];
	bool res = false;

	char* str;
	if (str = Config_GetStringValue(root, stringID)) {
		if (Util_TokeniseSafe(str, argv, ":", 3) == 3) {
			*r = std::atoi(argv[0]) / 255.0f;
			*g = std::atoi(argv[1]) / 255.0f;
			*b = std::atoi(argv[2]) / 255.0f;

			res = true;
		}
		else {
			Config_WarnLastF(true, root, "Invalid RBG entry \"%s\"", Config_GetTempStringValue(root, stringID));
		}

		Mem_Free(str);
	}

	return res;
}

/// CUSTOM:
bool Gods98::Config_GetColourRGBF(const Config* root, const char* stringID, OUT ColourRGBF* colour)
{
	return Config_GetRGBValue(root, stringID, &colour->r, &colour->g, &colour->b);
}


// <missing>
bool32 __cdecl Gods98::Config_GetCoord(const Config* root, const char* stringID, OUT real32* x, OUT real32* y, OPTIONAL OUT real32* z)
{
	Error_Fatal(!x || !y, "Config_GetCoord: Null passed as x or y");

	char* argv[3];
	bool res = false;

	char* str;
	if (str = Config_GetStringValue(root, stringID)) {
		const uint32 argc = Util_TokeniseSafe(str, argv, ",", 3);

		if (z == nullptr) { // FORMAT: x,y
			if (argc == 2) {
				*x = (real32)std::atof(argv[0]);
				*y = (real32)std::atof(argv[1]);

				res = true;
			}
			else {
				Config_WarnLastF(true, root, "Invalid 2D Coordinate entry \"%s\"", Config_GetTempStringValue(root, stringID));
			}
		}
		else { // FORMAT: x,y,z
			if (argc == 3) {
				*x = (real32)std::atof(argv[0]);
				*y = (real32)std::atof(argv[1]);
				*z = (real32)std::atof(argv[2]);

				res = true;
			}
			else {
				Config_WarnLastF(true, root, "Invalid 3D Coordinate entry \"%s\"", Config_GetTempStringValue(root, stringID));
			}
		}

		Mem_Free(str);
	}

	return res;
}

bool Gods98::Config_GetIntValues(const Config* root, const char* stringID, const char* sep, OUT sint32* values, uint32 count)
{
	bool res = false;

	char* str;
	if (str = Config_GetStringValue(root, stringID)) {
		// Allocate a larger array if count is too high.
		char* argv[128];
		char** argv2 = argv;
		if (count > _countof(argv)) argv2 = new char*[count];

		if (Util_TokeniseSafe(str, argv2, sep, count) == count) {
			for (uint32 i = 0; i < count; i++) {
				values[i] = std::atoi(argv2[i]);
			}
			res = true;
		}

		// Free if we've allocated a larger array, and aren't using the stack.
		if (argv2 != argv) delete[] argv2;
		Mem_Free(str);
	}
	return res;
}

bool Gods98::Config_GetRealValues(const Config* root, const char* stringID, const char* sep, OUT real32* values, uint32 count)
{
	bool res = false;

	char* str;
	if (str = Config_GetStringValue(root, stringID)) {
		// Allocate a larger array if count is too high.
		char* argv[128];
		char** argv2 = argv;
		if (count > _countof(argv)) argv2 = new char*[count];

		if (Util_TokeniseSafe(str, argv2, sep, count) == count) {
			for (uint32 i = 0; i < count; i++) {
				values[i] = static_cast<real32>(std::atof(argv2[i]));
			}
			res = true;
		}

		// Free if we've allocated a larger array, and aren't using the stack.
		if (argv2 != argv) delete[] argv2;
		Mem_Free(str);
	}
	return res;
}

bool Gods98::Config_GetPoint2I(const Config* root, const char* stringID, const char* sep, OUT Point2I* point)
{
	return Config_GetIntValues(root, stringID, sep, point->values.data(), point->values.size());
}

bool Gods98::Config_GetSize2I(const Config* root, const char* stringID, const char* sep, OUT Size2I* size)
{
	return Config_GetIntValues(root, stringID, sep, size->values.data(), size->values.size());
}

bool Gods98::Config_GetRange2I(const Config* root, const char* stringID, const char* sep, OUT Range2I* range)
{
	return Config_GetIntValues(root, stringID, sep, range->values.data(), range->values.size());
}

bool Gods98::Config_GetArea2I(const Config* root, const char* stringID, const char* sep, OUT Area2I* area)
{
	return Config_GetIntValues(root, stringID, sep, area->values.data(), area->values.size());
}

bool Gods98::Config_GetRect2I(const Config* root, const char* stringID, const char* sep, OUT Rect2I* rect)
{
	return Config_GetIntValues(root, stringID, sep, rect->values.data(), rect->values.size());
}

bool Gods98::Config_GetPoint2F(const Config* root, const char* stringID, const char* sep, OUT Point2F* point)
{
	return Config_GetRealValues(root, stringID, sep, point->values.data(), point->values.size());
}

bool Gods98::Config_GetSize2F(const Config* root, const char* stringID, const char* sep, OUT Size2F* size)
{
	return Config_GetRealValues(root, stringID, sep, size->values.data(), size->values.size());
}

bool Gods98::Config_GetRange2F(const Config* root, const char* stringID, const char* sep, OUT Range2F* range)
{
	return Config_GetRealValues(root, stringID, sep, range->values.data(), range->values.size());
}

bool Gods98::Config_GetArea2F(const Config* root, const char* stringID, const char* sep, OUT Area2F* area)
{
	return Config_GetRealValues(root, stringID, sep, area->values.data(), area->values.size());
}

bool Gods98::Config_GetRect2F(const Config* root, const char* stringID, const char* sep, OUT Rect2F* rect)
{
	return Config_GetRealValues(root, stringID, sep, rect->values.data(), rect->values.size());
}

bool Gods98::Config_GetVector3F(const Config* root, const char* stringID, const char* sep, OUT Vector3F* vector)
{
	return Config_GetRealValues(root, stringID, sep, vector->values.data(), vector->values.size());
}

bool Gods98::Config_GetVector4F(const Config* root, const char* stringID, const char* sep, OUT Vector4F* vector)
{
	return Config_GetRealValues(root, stringID, sep, vector->values.data(), vector->values.size());
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


// <LegoRR.exe @00479500>
void __cdecl Gods98::Config_Free(Config* root)
{
	log_firstcall();

	Error_Fatal(root->fileData == nullptr, "Config_Free: Must pass the root (loaded) config structure.");

	while (root) {
		/// CHANGE: Allow other config properties to store string data allocations.
		if (root->fileData) {
			Mem_Free(root->fileData);
			root->fileData = nullptr;
		}
		if (root->fileName) {
			Mem_Free(root->fileName);
			root->fileName = nullptr;
		}

		Config* next = const_cast<Config*>(root->linkNext);
		Config_Remove(root);
		root = next;
	}
}


// Used by Error_TerminateProgram
// <debug>
const char* __cdecl Gods98::Config_Debug_GetLastFind(void)
{
	return "<not implemented>";
}


// <LegoRR.exe @00479530>
Gods98::Config* __cdecl Gods98::Config_Create(Config* prev)
{
	log_firstcall();

	Config_CheckInit();

	Config* newConfig = configListSet.Add();
	ListSet::MemZero(newConfig);

	newConfig->fileData = nullptr;
	newConfig->itemName = nullptr;
	newConfig->dataString = nullptr;
	//newConfig->itemHashCode = 0; // unused field
	newConfig->linkNext = nullptr;

	if (prev != nullptr) { // Non-root config node
		prev->linkNext = newConfig;
		newConfig->linkPrev = prev;
		newConfig->depth = prev->depth;
	}
	else { // Root config node
		newConfig->linkPrev = nullptr;
		newConfig->depth = 0;
	}

//#ifdef _DEBUG_2
//	newConfig->accessed = false;
//#endif // _DEBUG_2

	return newConfig;
}

// <LegoRR.exe @00479580>
void __cdecl Gods98::Config_Remove(Config* dead)
{
	log_firstcall();

	Config_CheckInit();

//#ifdef _DEBUG_2
//	Error_Fatal(!dead, "Config_Remove: NULL passed.");
//	if (configGlobs.debugLastFind == dead) configGlobs.debugLastFind = NULL;
//#endif // _DEBUG_2

	configListSet.Remove(dead);
}

/// CUSTOM:
uint32 Gods98::Config_HashItemName(const char* name)
{
	return Util_HashString(name, false, true);
}

/// CUSTOM: Subfunction of Config_FindItem.
bool Gods98::Config_MatchItemName(const Gods98::Config* conf, const char* name, OPTIONAL uint32 hashCode, OPTIONAL OUT bool* wildcard)
{
	// Then check if this is a full match without wildcards.
	bool fullMatch = false;
	if (hashCode == 0 || conf->itemHashCode == hashCode) {
		fullMatch = (::_stricmp(conf->itemName, name) == 0);
	}

	// Check if we can compare by wildcard (only allowed at depth 0).
	bool wildcardMatch = false;
	if (!fullMatch && conf->depth == 0) {
		const char* s;
		uint32 wildcardLength = 0;
		for (s = conf->itemName; *s != '\0'; s++) {
			if (*s == CONFIG_WILDCARDCHAR) break; // Anything after the wildcard character is ignored.
			wildcardLength++;
		}
		if (*s == CONFIG_WILDCARDCHAR) {
			wildcardMatch = (::_strnicmp(conf->itemName, name, wildcardLength) == 0);
		}
	}

	/// CHANGE: Never treat full match as a wildcard match.
	if (wildcard) *wildcard = wildcardMatch && !fullMatch;
	return (wildcardMatch || fullMatch);
}

// <LegoRR.exe @004795a0>
const Gods98::Config* __cdecl Gods98::Config_FindItem(const Config* conf, const char* stringID)
{
	log_firstcall();

	// Search the list for the given item.

	const Config* foundConf = nullptr;

	char* argv[CONFIG_MAXDEPTH];
	uint32 hashv[CONFIG_MAXDEPTH];
	char* tempstring = Util_StrCpy(stringID);
	const uint32 count = Util_TokeniseSafe(tempstring, argv, CONFIG_SEPARATOR, CONFIG_MAXDEPTH);
	if (count > CONFIG_MAXDEPTH) {
		Mem_Free(tempstring);
		Error_WarnF2(true, "%s: Warning: StringID exceeds max depth of %i \"%s\".\n", Config_GetFileName(conf), CONFIG_MAXDEPTH, stringID);
		return nullptr;
	}

	for (uint32 i = 0; i < count; i++) {
		hashv[i] = Config_HashItemName(argv[i]);
	}

	// First find anything that matches the depth of the request
	// then see if the hierarchy matches the request.

	for (const Config* item = conf; item != nullptr && count > 0; item = item->linkNext) {
		if (item->depth != count - 1)
			continue; // This isn't the depth of the item we're looking for.

		bool wildcard = false;

		// Match the item we're looking for first.
		if (Config_MatchItemName(item, argv[count - 1], hashv[count - 1], &wildcard)) {
			/// FIX APPLY: Don't treat wildcard match as full match for deepest item.
			/// NOTE: This fix requires a fix in Config_Load2, to strip empty properties from the end of the config file.
			///       The error can be experienced when loading PTL files since the root property is searched for as an array.
			//wildcard = false;

			// Then look backwards down the list to check the hierarchy.
			uint32 currDepth = count - 1;
			for (const Config* parent = item; parent != nullptr && currDepth > 0; parent = parent->linkPrev) {
				if (parent->depth < currDepth - 1)
					break; /// SANITY: Protect against malformed config trees.
				if (parent->depth != currDepth - 1)
					continue; // This isn't the depth of the next parent item.

				if (Config_MatchItemName(parent, argv[currDepth - 1], hashv[currDepth - 1], &wildcard)) {
					currDepth--; // Matched a parent, look for the next parent item.
				}
				else {
					break; // This parent in the hierarchy doesn't match.
				}
			}

			// If our depth is zero, then we matched against all parents (or the deepest item was a root item).
			if (currDepth == 0) {
				foundConf = item;

				// If we found a wildcard match, then keep looking for a full match.
				// Otherwise we can exit the loop now and return the found item.
				if (!wildcard) break;
			}
		}
	}

	Mem_Free(tempstring);

//#ifdef _DEBUG_2
//	if (foundConf) {
//		configGlobs.debugLastFind = foundConf;
//		foundConf->accessed = true;
//	}
//	else {
//		foundConf = nullptr;
//	}
//#endif // _DEBUG_2

	return foundConf;
}

// <LegoRR.exe @00479750>
void __cdecl Gods98::Config_AddList(void)
{
	// NOTE: This function is no longer called, configListSet.Add already handles this.
	configListSet.AddList();
}




/// CUSTOM: Count number of items in an array.
uint32 Gods98::Config_CountItems(const Config* arrayItem)
{
	uint32 count = 0;
	while (arrayItem != nullptr) {
		count++;
		arrayItem = Config_GetNextItem(arrayItem);
	}
	return count;
}

/// CUSTOM: Merge configs together by appending one to another.
void Gods98::Config_AppendConfig(Config* root, Config* config)
{
	Error_Fatal((root->depth   != 0), "Config_AppendConfig: Base config does not start at depth 0.");
	Error_Fatal((config->depth != 0), "Config_AppendConfig: Appended config does not start at depth 0.");
	Error_Fatal((config->linkPrev != nullptr), "Config_AppendConfig: Appended config is not the root.");

	Config* next = root;
	do {
		root = next;
		next = const_cast<Config*>(root->linkNext);
	} while (next != nullptr);

	root->linkNext = config;
	config->linkPrev = root;
}

/// CUSTOM: Future replacements for Config_GetIntValue macro.
sint32 Gods98::Config_GetIntValue2(const Config* root, const char* stringID)
{
	const char* str = Config_GetTempStringValue(root, stringID);
	if (str != nullptr) {
		return std::atoi(str);
	}
	return 0;
}

/// CUSTOM: Future replacements for Config_GetRealValue macro.
real32 Gods98::Config_GetRealValue2(const Config* root, const char* stringID)
{
	const char* str = Config_GetTempStringValue(root, stringID);
	if (str != nullptr) {
		return static_cast<real32>(std::atof(str));
	}
	return 0.0f;
}

#pragma endregion
