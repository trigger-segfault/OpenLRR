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
	char* stringParts[10];
	char buff[256];
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
	//localLoaderSec = loaderGlobs.sectionList;
	for(int i = 0; i < LOADER_MAXSECTIONS; i++){
		//Check if file/string is null or if we have a string that isnt XXX=YYY
		if ( (f == nullptr) || (Gods98::File_GetS(buff, sizeof(buff), f) == nullptr) || (Gods98::Util_Tokenise(buff, stringParts, "=") != 2) )
		{
			loaderGlobs.sectionList[i].name = nullptr;
		}
		else {

			//Copy data over to string
			loaderGlobs.sectionList[i].name = Gods98::Util_StrCpy(stringParts[0]);

			//Grab integer size from stringparts and store
			loaderGlobs.sectionList[i].totalSize = std::atoi(stringParts[1]);
			loaderGlobs.sectionList[i].currentSize = 0;
		}
	}

	//if we are at the end of the struct and into the game data done loading files
	if (f != nullptr) {
		Gods98::File_Close(f);
	}
	Gods98::File_SetLoadCallback(Loader_FileLoadCallback, nullptr);
	return;
}

// <LegoRR.exe @0044e000>
void __cdecl LegoRR::Loader_Shutdown(const char* filename)
{
	Gods98::File* f;

	//Check if file is null to setup nullptr
	if (filename == nullptr) {
		f = nullptr;
	}
	else {
		f = Gods98::File_Open(filename, "w");
	}

	//Clear out all memory
	for(int i = 0; i < LOADER_MAXSECTIONS; i++){
		//If the section isnt null print the section to the file and clear the memory
		if (loaderGlobs.sectionList[i].name != nullptr) {
			if (loaderGlobs.sectionList[i].currentSize == 0) {
				loaderGlobs.sectionList[i].currentSize = loaderGlobs.sectionList[i].totalSize;
			}
			if (f != nullptr) {
				Gods98::File_PrintF(f, "%s%s%i\n", loaderGlobs.sectionList[i].name, "=", loaderGlobs.sectionList[i].currentSize);
			}
			Gods98::Mem_Free(loaderGlobs.sectionList[i].name);
		}
	}

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
	int secCntr;
	//If section is null clear all flags and bails
	if (section == nullptr) {
		loaderGlobs.flags &= ~LOADER_GLOB_FLAG_ENABLED;
		return;
	}

	secCntr = 0; 
	for(int i = 0; i < LOADER_MAXSECTIONS; i++){
		//Check for empty section to determine if we're done
		if ((loaderGlobs.sectionList[i].name != nullptr) &&  (_stricmp(loaderGlobs.sectionList[i].name, section) == 0)) {
			loaderGlobs.current = loaderGlobs.sectionList + secCntr;
			loaderGlobs.sectionList[secCntr].currentSize = 0;
			break;
		}
		secCntr = secCntr + 1;
	//Leave if we pass the end of the current loader struct
	}

	//Check if we're in the gamedata section and copy over
	if ((secCntr == LOADER_MAXSECTIONS) && (&loaderGlobs.sectionList[LOADER_MAXSECTIONS-1] != nullptr)) {
		loaderGlobs.sectionList[LOADER_MAXSECTIONS-1].name = Gods98::Util_StrCpy(section);

		loaderGlobs.sectionList[LOADER_MAXSECTIONS-1].currentSize = 0;
		loaderGlobs.sectionList[LOADER_MAXSECTIONS-1].totalSize = 0;
		loaderGlobs.current = &loaderGlobs.sectionList[LOADER_MAXSECTIONS-1];
	}

	//Setup flags and create callback
	loaderGlobs.flags |= LOADER_GLOB_FLAG_ENABLED;
	loaderGlobs.progressLast = -1.0f;
	Loader_FileLoadCallback(nullptr, 0, nullptr);
	return;
}

// <LegoRR.exe @0044e180>
void __cdecl LegoRR::Loader_FileLoadCallback(const char* filename, uint32 fileSize, void* data)
{
	int x,y;
	Size2F barSize;
	Point2F barPosition;

	float localFloat = fileSize;

	if (loaderGlobs.flags & LOADER_GLOB_FLAG_ENABLED) {
		if (loaderGlobs.current == nullptr) {
			localFloat = 0.0f;
		}
		else {
			loaderGlobs.current->currentSize += localFloat;
			if (loaderGlobs.current->totalSize == 0) {
				localFloat = 0.0f;
			}
			else {
				//Get percentage here
				float num = loaderGlobs.current->currentSize;
				float deom = loaderGlobs.current->totalSize;
				localFloat = num/deom;
			}
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
				x = (Gods98::appWidth()/2) - (loaderGlobs.LoadingWidth/2);
				y = (int)loaderGlobs.ProgressWindow.y - 1;
				Gods98::Font_PrintF	(loaderGlobs.font, x, y, "%s", loaderGlobs.LoadingText);
			}
			Gods98::Main_LoopUpdate(false);
			loaderGlobs.progressLast = localFloat;
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
