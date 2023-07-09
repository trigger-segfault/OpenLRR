// Loader.cpp : 
//

#include "Loader.h"
#include "..\..\engine\core\Utils.h"
#include "..\..\engine\core\files.h"
#include "..\..\engine\Main.h"
#include "..\..\engine\drawing\DirectDraw.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00556e00>
LegoRR::Loader_Globs & LegoRR::loaderGlobs = *(LegoRR::Loader_Globs*)0x00556e00;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0044de10>
void __cdecl LegoRR::Loader_Initialise(const char* loadScreenName, const char* shutdownScreenName, Gods98::Font* font5HI, const char* profileName, Direction progressDirection, const char* progressBarName, const Area2F* progressWindow, const char* loadingText)
{
	char* strBuffPtr;
	Gods98::File* f;
	LoaderSection* localLoaderSec;
	char* stringParts[10];
	#define BUFFSIZE 256
	char buff[BUFFSIZE];
	//Setup all data structs
	f = nullptr;
	loaderGlobs.LoadScreen = Gods98::Image_LoadBMPScaled(loadScreenName, 0, 0);
	loaderGlobs.ShutdownScreen = Gods98::Image_LoadBMPScaled(shutdownScreenName, 0, 0);
	loaderGlobs.font = font5HI;
	loaderGlobs.current = nullptr;
	loaderGlobs.ProgressBar = Gods98::Image_LoadBMPScaled(progressBarName, 0, 0);
	loaderGlobs.ProgressWindow.x = progressWindow->x;
	loaderGlobs.ProgressWindow.y = progressWindow->y;
	loaderGlobs.ProgressWindow.width = progressWindow->width;
	loaderGlobs.ProgressWindow.height = progressWindow->height;
	loaderGlobs.ProgressDirection = progressDirection;

	//Grab loading text
	if (loadingText == nullptr) {
		loaderGlobs.LoadingText = nullptr;
	}
	else {
		//Go through string until term char and replace underscore with space
		strBuffPtr = Gods98::Util_StrCpy(loadingText);
		loaderGlobs.LoadingText = strBuffPtr;
		while (*strBuffPtr != '\0') {
			if (*strBuffPtr == '_') {
				*strBuffPtr = ' ';
			}
			strBuffPtr = strBuffPtr + 1;
		}
		Gods98::Font_GetStringInfo(font5HI, &loaderGlobs.LoadingWidth, nullptr, "%s", loadingText);
	}

	//Check if profile is empty and open file
	if (profileName != nullptr) {
		f = Gods98::File_Open(profileName, "r");
	}

	//Iterate through all loader sections and store to loaderStruct
	localLoaderSec = loaderGlobs.sectionList;
	do {
		//Check if file/string is null or if we have a string that isnt XXX=YYY
		if ( (f == nullptr) || (Gods98::File_GetS(buff, BUFFSIZE, f) == nullptr) || (Gods98::Util_Tokenise(buff, stringParts, "=") != 2) )
		{
			localLoaderSec->name = nullptr;
		}
		else {

			//Allocate memory for the string on the struct and copy data over
			uint32* allocPtr = (uint32*)Gods98::Mem_Alloc(strlen(stringParts[0]));
			localLoaderSec->name = (char*)allocPtr;
			strcpy((char*)allocPtr, stringParts[0]);

			//Grab integer size from stringparts and store
			localLoaderSec->totalSize = std::atoi(stringParts[1]);
			localLoaderSec->currentSize = 0;
		}
		//Move up one slot
		localLoaderSec = localLoaderSec + 1;
		//if we are at the end of the struct and into the game data done loading files
		if (&loaderGlobs.sectionList[LOADER_MAXSECTIONS] == localLoaderSec) {
			if (f != nullptr) {
				Gods98::File_Close(f);
			}
			Gods98::File_SetLoadCallback(Loader_FileLoadCallback, (void*)0x0);
			return;
		}
	} while (true);
}

// <LegoRR.exe @0044e000>
void __cdecl LegoRR::Loader_Shutdown(const char* filename)
{
	Gods98::File* f;
	LoaderSection* localLoaderSec;

	//Check if file is null to setup nullptr
	if (filename == nullptr) {
		f = nullptr;
	}
	else {
		f = Gods98::File_Open(filename, "w");
	}

	//Setup localStruct to clear out all memory
	localLoaderSec = loaderGlobs.sectionList;
	do {
		//If the section isnt null print the section to the file and clear the memory
		if (localLoaderSec->name != nullptr) {
			if (localLoaderSec->currentSize == 0) {
				localLoaderSec->currentSize = localLoaderSec->totalSize;
			}
			if (f != nullptr) {
				Gods98::File_PrintF(f, "%s%s%i\n", localLoaderSec->name, "=", localLoaderSec->currentSize);
			}
			Gods98::Mem_Free((void*)localLoaderSec->name);
		}
		//Bump up iterator unitl done
		localLoaderSec = localLoaderSec + 1;
	} while (localLoaderSec < (LoaderSection*)&loaderGlobs.current);

	//if the file isnt null we're done
	if (f != nullptr) {
		Gods98::File_Close(f);
	}
	//Remove and close load/shutdown screens
	Gods98::Image_Remove(loaderGlobs.LoadScreen);
	if (loaderGlobs.ShutdownScreen != nullptr) {
		Gods98::Image_Remove(loaderGlobs.ShutdownScreen);
	}
	return;
}

// <LegoRR.exe @0044e0a0>
void __cdecl LegoRR::Loader_display_loading_bar(const char* section)
{
	LoaderSection* localLoaderSec;
	int secCntr;
	//If section is null clear all flags and bails
	if (section == nullptr) {
		loaderGlobs.flags = loaderGlobs.flags & (~LOADER_GLOB_FLAG_ENABLED);
		return;
	}

	secCntr = 0; 
	localLoaderSec = loaderGlobs.sectionList;
	do {
		//Check for empty section to determine if we're done
		if ((localLoaderSec->name != nullptr) &&  (_stricmp(localLoaderSec->name, section) == 0)) {
			loaderGlobs.current = loaderGlobs.sectionList + secCntr;
			loaderGlobs.sectionList[secCntr].currentSize = 0;
			break;
		}
		localLoaderSec = localLoaderSec + 1;
		secCntr = secCntr + 1;
	//Leave if we pass the end of the current loader struct
	} while (localLoaderSec < (LoaderSection*)&loaderGlobs.current);

	//Check if we're in the gamedata section and copy over
	if ((secCntr == LOADER_MAXSECTIONS) && (localLoaderSec != nullptr)) {
		uint32* allocPtr = (undefined4*)Gods98::Mem_Alloc(strlen(section));
		localLoaderSec->name = (char*)allocPtr;
		strcpy((char*)allocPtr, section);

		localLoaderSec->currentSize = 0;
		localLoaderSec->totalSize = 0;
		loaderGlobs.current = localLoaderSec;
	}

	//Setup flags and create callback
	loaderGlobs.flags = loaderGlobs.flags | LOADER_GLOB_FLAG_ENABLED;
	loaderGlobs.progressLast = -1.0;
	Loader_FileLoadCallback(nullptr, 0, nullptr);
	return;
}

// <LegoRR.exe @0044e180>
void __cdecl LegoRR::Loader_FileLoadCallback(const char* filename, uint32 fileSize, void* data)
{
	int x,y;
	long long lVar2;
	Size2F barSize;
	Point2F barPosition;

	float localFloat = fileSize;

	if (((byte)loaderGlobs.flags & LOADER_GLOB_FLAG_ENABLED) != 0) {
		if (((loaderGlobs.current == nullptr) || ((loaderGlobs.current)->currentSize = (loaderGlobs.current)->currentSize + localFloat, loaderGlobs.current == nullptr))
			|| ((loaderGlobs.current)->totalSize == 0)) {
			localFloat = 0.0f;
		}
		else {
			//Get percentage here
			float num = loaderGlobs.current->currentSize;
			float deom = loaderGlobs.current->totalSize;
			localFloat = num/deom;
		}
		/* // minimum increase since last update of progress bar
		   if (floatVal > (loaderGlobs.progressLast - 0.002f)) */
		if (loaderGlobs.progressLast + 0.002f < localFloat) {
			barPosition.y = loaderGlobs.ProgressWindow.y;
			barPosition.x = loaderGlobs.ProgressWindow.x;
			barSize.width = loaderGlobs.ProgressWindow.width;
			barSize.height =loaderGlobs.ProgressWindow.height;
			Gods98::Image_DisplayScaled(loaderGlobs.LoadScreen, nullptr, nullptr, nullptr);
			/* if (floatVal > 1.0f) floatVal = 1.0f; */
			if (1.0f < localFloat) {
				/* 1.0f */
				localFloat = 1.0f;
			}
			switch (loaderGlobs.ProgressDirection) {
			case DIRECTION_UP:
				barPosition.y = (barSize.height + barPosition.y) - barSize.height * localFloat;
				barSize.height = (barSize.height + barPosition.y) - barPosition.y;
				break;
			case DIRECTION_RIGHT:
				barSize.width = barSize.width * localFloat;
				break;
			case DIRECTION_DOWN:
				barSize.height = barSize.height * localFloat;
				break;
			case DIRECTION_LEFT:
				barPosition.x = (barSize.width + barPosition.x) - barSize.width * localFloat;
				barSize.width = (barSize.width + barPosition.x) - barPosition.x;
			}
			if (loaderGlobs.ProgressBar !=  nullptr) {
				Gods98::Image_DisplayScaled(loaderGlobs.ProgressBar, nullptr, &barPosition, (const Point2F*)&barSize);
			}
			if (loaderGlobs.LoadingText != nullptr) {
				//center text in screen
				x = (Gods98::appWidth()/2) - (loaderGlobs.LoadingWidth >> 1);
				y = (int)loaderGlobs.ProgressWindow.y - 1;
				Gods98::Font_PrintF	(loaderGlobs.font, x, y, "%s", loaderGlobs.LoadingText);
			}
			Gods98::Main_LoopUpdate(0);
			loaderGlobs.progressLast = (float)localFloat;
		}
	}
	return;
}

// <LegoRR.exe @0044e360>
void __cdecl LegoRR::Loader_display_shutdown(void)
{
	//If we have a shutdown screen null it and then bail
	if (loaderGlobs.ShutdownScreen != nullptr) {
		Gods98::Image_DisplayScaled(loaderGlobs.ShutdownScreen, nullptr, nullptr, nullptr);
		Gods98::DirectDraw_Flip();
		return;
	}
	return;
}
#pragma endregion
