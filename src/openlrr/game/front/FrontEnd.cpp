// FrontEnd.cpp : 
//

#pragma once

#include "../../engine/audio/3DSound.h"
#include "../../engine/core/Config.h"
#include "../../engine/core/Files.h"
#include "../../engine/core/Maths.h"
#include "../../engine/core/Memory.h"
#include "../../engine/core/Utils.h"
#include "../../engine/drawing/Images.h"
#include "../../engine/input/Input.h"
#include "../../engine/video/Movie.h"
#include "../../engine/Main.h"

#include "../Game.h"

#include "Credits.h"
#include "FrontEnd.h"
#include "../audio/SFX.h"
#include "../interface/HelpWindow.h"
#include "../interface/InfoMessages.h"
#include "../interface/Pointers.h"
#include "../interface/ToolTip.h"
#include "../object/ObjectRecall.h"
#include "../mission/Objective.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004a2f48>
sint32 & LegoRR::g_SaveMenu_INDEX_004a2f48 = *(sint32*)0x004a2f48; // = -1;

// <LegoRR.exe @004a2f4c>
sint32 & LegoRR::g_SaveMenu_INT_004a2f4c = *(sint32*)0x004a2f4c; // = -1;

// <LegoRR.exe @004a2f50>
sint32 & LegoRR::g_SaveMenu_INDEX_004a2f50 = *(sint32*)0x004a2f50; // = -1;

// <LegoRR.exe @004a2f54>
sint32 & LegoRR::g_SaveMenu_INDEX_004a2f54 = *(sint32*)0x004a2f54; // = -1;

// <LegoRR.exe @004a2f58>
sint32 & LegoRR::g_SaveMenu_INT_004a2f58 = *(sint32*)0x004a2f58; // = -1;

// <LegoRR.exe @004a2f5c>
sint32 & LegoRR::g_SaveMenu_OutNumber = *(sint32*)0x004a2f5c; // = -1;

// As a heads up, this IS NOT the array used in GODS98, and seems to exist solely for Rename Input in LRR.
// <LegoRR.exe @004a2f60>
const uint32 (& LegoRR::c_keyCharMap)[256] = *(uint32(*)[256])0x004a2f60;


// <LegoRR.exe @004d88c8>
sint32 (& LegoRR::s_LevelSelect_NameXs)[16] = *(sint32(*)[16])0x004d88c8;


// <LegoRR.exe @004d8b08>
Gods98::Font* (& LegoRR::s_LevelSelectNameFonts)[16] = *(Gods98::Font*(*)[16])0x004d8b08;

// <LegoRR.exe @004d8b48>
char (& LegoRR::s_FrontLevelSelectNames)[15][1024] = *(char(*)[15][1024])0x004d8b48;

// <LegoRR.exe @004dc748>
char (& LegoRR::s_FrontReplaceSpacesBuff)[256] = *(char(*)[256])0x004dc748;


// <LegoRR.exe @004dc850>
sint32 (& LegoRR::s_LevelSelect_NameYs)[16] = *(sint32(*)[16])0x004dc850;

// <LegoRR.exe @004dc890>
Gods98::Image* (& LegoRR::g_SaveSlotImages_TABLE)[5] = *(Gods98::Image*(*)[5])0x004dc890;

// <LegoRR.exe @004dc8a4>
undefined4 & LegoRR::DAT_004dc8a4 = *(undefined4*)0x004dc8a4;

// <LegoRR.exe @004dc8a8>
uint32 (& LegoRR::g_SaveSlotPtrs_TABLE)[5] = *(uint32(*)[5])0x004dc8a8;

// <LegoRR.exe @004dc8bc>
bool32 & LegoRR::g_FrontBool_004dc8bc = *(bool32*)0x004dc8bc;

// <LegoRR.exe @004dc8c0>
bool32 & LegoRR::g_FrontBool_004dc8c0 = *(bool32*)0x004dc8c0;

// <LegoRR.exe @004dc8c4>
bool32 & LegoRR::g_FrontBool_004dc8c4 = *(bool32*)0x004dc8c4;

// <LegoRR.exe @004dc8c8>
bool32 & LegoRR::g_FrontBool_004dc8c8 = *(bool32*)0x004dc8c8;

// <LegoRR.exe @004dc8cc>
LegoRR::Front_Cache* (& LegoRR::g_ImageCache_NEXT) = *(LegoRR::Front_Cache**)0x004dc8cc;

// <LegoRR.exe @004dc8d0>
uint32 & LegoRR::s_LevelSelectNameCount = *(uint32*)0x004dc8d0;

// <LegoRR.exe @004dc8d4>
bool32 & LegoRR::g_FrontBool_004dc8d4 = *(bool32*)0x004dc8d4;


// <LegoRR.exe @004dc8dc>
sint32 & LegoRR::s_frontTextInputCaretBlinker = *(sint32*)0x004dc8dc;

// <LegoRR.exe @004dc8e0>
LegoRR::Menu* (& LegoRR::s_FrontOptionsMenu) = *(LegoRR::Menu**)0x004dc8e0;

// <LegoRR.exe @004dc8e4>
LegoRR::MenuSet* (& LegoRR::s_FrontOptionsMenuSet) = *(LegoRR::MenuSet**)0x004dc8e4;


// <LegoRR.exe @00557fc0>
LegoRR::Front_Globs & LegoRR::frontGlobs = *(LegoRR::Front_Globs*)0x00557fc0;

// Replacement for frontGlobs.rockWipLastUpdateTime, because it stores Main_GetTime() as a float.
static uint32 _rockWipeLastUpdateTimeFixed = 0;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

 // Returns a temporary string buffer. Or str if the temporary buffer is too small.
// <LegoRR.exe @004101e0>
const char* __cdecl LegoRR::Front_Util_ReplaceTextSpaces(const char* str)
{
	// Can't use temporary buffer if the string is too large.
	// Failure here isn't fatal, since spaces are not relied upon.
	uint32 length = std::strlen(str);
	if (length + 1 > _countof(s_FrontReplaceSpacesBuff)) // Include null terminator in length check.
		return str;

	// Memcpy with length can be used instead.
	std::strcpy(s_FrontReplaceSpacesBuff, str);

	for (uint32 i = 0; i < length; i++) {

		if (s_FrontReplaceSpacesBuff[i] == '_')
			s_FrontReplaceSpacesBuff[i] = ' ';
	}
	return s_FrontReplaceSpacesBuff;
}

// <LegoRR.exe @00410250>
void __cdecl LegoRR::Front_LevelSelect_LevelNamePrintF(Gods98::Font* font, sint32 x, sint32 y, const char* msg, ...)
{
	if (msg == nullptr) {
		for (uint32 i = 0; i < s_LevelSelectNameCount; i++) {
			
			Gods98::Font_PrintF(s_LevelSelectNameFonts[i], s_LevelSelect_NameXs[i], s_LevelSelect_NameYs[i],
								s_FrontLevelSelectNames[i]);
		}
		s_LevelSelectNameCount = 0;
	}
	else if (s_LevelSelectNameCount < 15) {
		std::va_list argptr;
		va_start(argptr, msg);
		std::vsprintf(s_FrontLevelSelectNames[s_LevelSelectNameCount], msg, argptr);
		va_end(argptr);

		s_LevelSelect_NameXs[s_LevelSelectNameCount] = x;
		s_LevelSelect_NameYs[s_LevelSelectNameCount] = y;
		s_LevelSelectNameFonts[s_LevelSelectNameCount] = font;
		s_LevelSelectNameCount++;
	}
}

// <LegoRR.exe @00410300>
void __cdecl LegoRR::Front_Callback_TriggerPlayCredits(void)
{
	Credits_Play(legoGlobs.CreditsTextFile, legoGlobs.fontCredits, legoGlobs.CreditsBackAVI);
}

// <LegoRR.exe @00410320>
void __cdecl LegoRR::Front_RockWipe_Play(void)
{
	if (frontGlobs.rockWipeAnim != nullptr) {
		frontGlobs.rockWipeFlags |= (ROCKWIPE_FLAG_ANIMATING|ROCKWIPE_FLAG_NOINPUT);

		frontGlobs.rockWipeTimer = 0.0f;
		/// FIX APPLY: Store rockWipeLastUpdateTime in uint32 instead of float.
		_rockWipeLastUpdateTimeFixed = Gods98::Main_GetTime();
		// This variable has been replaced with _rockWipeLastUpdateTimeFixed in Front_Menu_Update.
		frontGlobs.rockWipeLastUpdateTime = (real32)_rockWipeLastUpdateTimeFixed;

		SFX_AddToQueue(SFX_RockWipe, Gods98::SoundMode::Once);
	}
}

// <LegoRR.exe @00410370>
void __cdecl LegoRR::Front_RockWipe_Stop(void)
{
	frontGlobs.rockWipeFlags &= ~(ROCKWIPE_FLAG_ANIMATING|ROCKWIPE_FLAG_NOINPUT);
}

// <LegoRR.exe @00410380>
LegoRR::Front_Cache* __cdecl LegoRR::Front_Cache_FindByName(const char* filename)
{
	for (Front_Cache* cache = g_ImageCache_NEXT; cache != nullptr; cache = cache->next) {
		if (::_stricmp(filename, cache->path) == 0) {
			return cache;
		}
	}
	return nullptr;
}

// <LegoRR.exe @004103c0>
LegoRR::Front_Cache* __cdecl LegoRR::Front_Cache_Create(const char* filename)
{
	if (filename != nullptr && *filename != '\0') {
		Front_Cache* cache = Front_Cache_FindByName(filename);
		if (cache == nullptr) {
			cache = (Front_Cache*)Gods98::Mem_Alloc(sizeof(Front_Cache));
			cache->path = Gods98::Util_StrCpy(filename);
			cache->image = nullptr;
			cache->font = nullptr;
			cache->next = g_ImageCache_NEXT;
			g_ImageCache_NEXT = cache;
		}
		return cache;
	}
	return nullptr;
}

// <LegoRR.exe @00410450>
Gods98::Image* __cdecl LegoRR::Front_Cache_LoadImage(const char* filename)
{
	if (filename != nullptr && *filename != '\0') {
		Front_Cache* cache = Front_Cache_Create(filename);
		if (cache->image == nullptr) {
			cache->image = Gods98::Image_LoadBMP(filename);
		}
		return cache->image;
	}
	return nullptr;
}

// <LegoRR.exe @00410490>
Gods98::Font* __cdecl LegoRR::Front_Cache_LoadFont(const char* filename)
{
	if (filename != nullptr && *filename != '\0') {
		Front_Cache* cache = Front_Cache_Create(filename);
		if (cache->font == nullptr) {
			cache->font = Gods98::Font_Load(filename);
		}
		return cache->font;
	}
	return nullptr;
}

// Same as `Gods98::Util_StrCpy`, but with null checks.
// <LegoRR.exe @004104d0>
char* __cdecl LegoRR::Front_Util_StrCpy(const char* str)
{
	if (str == nullptr)
		return nullptr;

	char* newString = (char*)Gods98::Mem_Alloc(std::strlen(str) + 1);
	if (newString == nullptr)
		return nullptr;

	std::strcpy(newString, str);
	return newString;
}

// <LegoRR.exe @00410520>
LegoRR::MenuItem_SelectData* __cdecl LegoRR::Front_MenuItem_CreateSelect(IN sint32* valuePtr, const char* string1, const char* string2, sint32 x2, sint32 y2,
																		 sint32 selItemHeight, sint32 scrollCount, sint32 xString1, sint32 yString1, sint32 xString2,
																		 sint32 yString2, sint32 field50, MenuItem_SelectCallback callback,
																		 OPTIONAL Menu* nextMenu)
{
	MenuItem_SelectData* selectData = (MenuItem_SelectData*)Gods98::Mem_Alloc(sizeof(MenuItem_SelectData));
	std::memset(selectData, 0, sizeof(MenuItem_SelectData));

	selectData->string1 = Front_Util_StrCpy(string1);
	selectData->string2 = Front_Util_StrCpy(string2);

	selectData->valuePtr = valuePtr;

	selectData->x2 = x2;
	selectData->y2 = y2;
	selectData->xString1 = xString1;
	selectData->yString1 = yString1;
	selectData->xString2 = xString2;
	selectData->yString2 = yString2;

	selectData->selItemHeight = selItemHeight;
	selectData->scrollCount = scrollCount;
	selectData->scrollStart = 0;

	selectData->callback = callback;
	selectData->field_50 = field50;
	selectData->nextMenu = nextMenu;
	return selectData;
}

// <LegoRR.exe @004105c0>
void __cdecl LegoRR::Front_MenuItem_FreeSelect(MenuItem_SelectData* selectData)
{
	if (selectData != nullptr) {
		if (selectData->string1 != nullptr) {
			Gods98::Mem_Free(selectData->string1);
		}

		if (selectData->string2 != nullptr) {
			Gods98::Mem_Free(selectData->string2);
		}

		if (selectData->selItemList != nullptr) {
			for (sint32 i = 0; i < selectData->selItemCount; i++) {

				if (selectData->selItemList[i].flags & SELECTITEM_FLAG_HASBANNER) {

					Gods98::Mem_Free(selectData->selItemList[i].banner);
				}
				else if (selectData->selItemList[i].flags & SELECTITEM_FLAG_HASIMAGE) {

					for (uint32 j = 0; j < 3; j++) {
						Gods98::Image_Remove(selectData->selItemList[i].images[j]);
					}
				}
			}
			Gods98::Mem_Free(selectData->selItemList);
		}
		Gods98::Mem_Free(selectData);
	}
}

// param flag4 is false for LevelSelect items and true for Load/Save items.
// <LegoRR.exe @00410670>
void __cdecl LegoRR::Front_MenuItem_AddSelectItem(MenuItem_SelectData* selectData, char* bannerOrBMPName, bool32 flag4,
												  Gods98::Font* hiFont, sint32 frontEndX, sint32 frontEndY, bool32 frontEndOpen)
{
	/// FIXME: Original malloc-null-checks here are faulty. LRR checks if newSelItems is non-null.
	///        But it doesn't check newWidths/newHeights... and worse, it uses realloc, so failure can be fatal.
	MenuItem_SelectItem* newSelItems;
	uint32* newWidths[MenuItem_SelectImage_Count];
	uint32* newHeights[MenuItem_SelectImage_Count];

	/*if (selectData->selItemList == nullptr) {
		newSelItems = (MenuItem_SelectItem*)Gods98::Mem_Alloc((selectData->selItemCount + 1) * sizeof(MenuItem_SelectItem));
		for (uint32 i = 0; i < MenuItem_SelectImage_Count; i++) {
			newWidths[i]  = (uint32*)Gods98::Mem_Alloc((selectData->selItemCount + 1) * sizeof(uint32));
			newHeights[i] = (uint32*)Gods98::Mem_Alloc((selectData->selItemCount + 1) * sizeof(uint32));
		}
	}
	else {*/
		// realloc has the same behavior as malloc when the input ptr is null, so just use realloc.
		newSelItems = (MenuItem_SelectItem*)Gods98::Mem_ReAlloc(selectData->selItemList, (selectData->selItemCount + 1) * sizeof(MenuItem_SelectItem));
		for (uint32 i = 0; i < MenuItem_SelectImage_Count; i++) {
			newWidths[i]  = (uint32*)Gods98::Mem_ReAlloc(selectData->widths[i], (selectData->selItemCount + 1) * sizeof(uint32));
			newHeights[i] = (uint32*)Gods98::Mem_ReAlloc(selectData->heights[i], (selectData->selItemCount + 1) * sizeof(uint32));
		}
	//}

	if (newSelItems != nullptr) {
		selectData->selItemList = newSelItems;
		for (uint32 i = 0; i < MenuItem_SelectImage_Count; i++) {
			selectData->widths[i]  = newWidths[i];
			selectData->heights[i] = newHeights[i];
		}

		MenuItem_SelectItem* selItem = &selectData->selItemList[selectData->selItemCount];

		// NOTE: images are NOT nulled out when item is treated as banner.
		//std::memset(&selectData->selItemList[selectData->selItemCount], 0, sizeof(MenuItem_SelectItem));

		selItem->banner = nullptr;
		selItem->flags = (flag4 ? SELECTITEM_FLAG_ENABLED : SELECTITEM_FLAG_NONE);
		selItem->frontEndX = frontEndX;
		selItem->frontEndY = frontEndY;
		selItem->frontEndOpen = frontEndOpen;

		// Only the hackiest of the hackiest solutions...
		// If this parameter contains ".bmp" (case-insensitive), then it's an image filename.
		// Otherwise it's a banner string.
		if (bannerOrBMPName != nullptr &&  Gods98::Util_StrIStr(bannerOrBMPName, ".bmp")) {
			// Image select item:
			selItem->flags |= SELECTITEM_FLAG_HASIMAGE;

			char* stringParts[3];
			uint32 numParts = Gods98::Util_Tokenise(bannerOrBMPName, stringParts, ",");
			Error_Fatal(numParts != 3, "Must have exactly 3 comma-separated bmp images for select item");

			for (uint32 i = 0; i < MenuItem_SelectImage_Count; i++) {
				Gods98::Image* image = Front_Cache_LoadImage(stringParts[i]);
				// This null-check here is pointless, we'll still crash on GetWidth,GetHeight
				if (image != nullptr) Gods98::Image_SetPenZeroTrans(image);

				// This field should be changed to an array
				selItem->images[i] = image;

				selectData->widths[i][selectData->selItemCount]  = Gods98::Image_GetWidth(image);
				selectData->heights[i][selectData->selItemCount] = Gods98::Image_GetHeight(image);
			}
		}
		else {
			// Banner select item:
			selItem->flags |= SELECTITEM_FLAG_HASBANNER;

			selItem->banner = Front_Util_StrCpy(bannerOrBMPName);


			/// CUSTOM: Lazy auto-font so we can try this out with LevelSelect for fun :D
			//if (!hiFont) hiFont = Front_Cache_LoadFont("Interface\\Fonts\\MBriefFONT2.bmp");


			selectData->widths[0][selectData->selItemCount]  = Gods98::Font_GetStringWidth(hiFont, bannerOrBMPName);
			selectData->heights[0][selectData->selItemCount] = Gods98::Font_GetHeight(hiFont);
			// Assign the same width/heights to all elements.
			for (uint32 i = 1; i < MenuItem_SelectImage_Count; i++) {
				selectData->widths[i][selectData->selItemCount]  = selectData->widths[i - 1][selectData->selItemCount];
				selectData->heights[i][selectData->selItemCount] = selectData->heights[i - 1][selectData->selItemCount];
			}
		}

		selectData->selItemCount++;
	}
}

// <LegoRR.exe @00410940>
LegoRR::MenuItem_CycleData* __cdecl LegoRR::Front_MenuItem_CreateCycle(uint32 count, IN sint32* valuePtr, sint32 x2, sint32 y2, MenuItem_CycleCallback callback)
{
	MenuItem_CycleData* cycleData = (MenuItem_CycleData*)Gods98::Mem_Alloc(sizeof(MenuItem_CycleData));
	if (cycleData == nullptr)
		return nullptr;

	cycleData->nameList = (char**)Gods98::Mem_Alloc(count * sizeof(char*));
	if (cycleData->nameList == nullptr) {
		Front_MenuItem_FreeCycle(cycleData);
		return nullptr;
	}

	std::memset(cycleData->nameList, 0, count * sizeof(char*));

	cycleData->valuePtr = valuePtr;
	cycleData->callback = callback;
	cycleData->cycleCount = count;
	cycleData->nameCount = 0; // Handled by `Front_MenuItem_AddCycleName`
	cycleData->x2 = x2;
	cycleData->y2 = y2;
	return cycleData;
}

// <LegoRR.exe @004109d0>
void __cdecl LegoRR::Front_MenuItem_AddCycleName(MenuItem_CycleData* cycleData, const char* name)
{
	if (cycleData && cycleData->nameList && (sint32)cycleData->nameCount < (sint32)cycleData->cycleCount) {

		cycleData->nameList[cycleData->nameCount] = Front_Util_StrCpy(name);

		if (cycleData->nameList[cycleData->nameCount] != nullptr) {
			cycleData->nameCount++;
		}
	}
}

// <LegoRR.exe @00410a10>
void __cdecl LegoRR::Front_MenuItem_FreeCycle(MenuItem_CycleData* cycleData)
{
	if (cycleData != nullptr) {
		if (cycleData->nameList != nullptr) {
			for (uint32 i = 0; i < (uint32)cycleData->nameCount; i++) {

				if (cycleData->nameList[i] != nullptr) {
					Gods98::Mem_Free(cycleData->nameList[i]);
				}
			}
			Gods98::Mem_Free(cycleData->nameList);
		}
		Gods98::Mem_Free(cycleData);
	}
}

// <LegoRR.exe @00410a90>
LegoRR::MenuItem_TriggerData* __cdecl LegoRR::Front_MenuItem_CreateTrigger(IN bool32* valuePtr, bool32 end, MenuItem_TriggerCallback callback)
{
	MenuItem_TriggerData* triggerData = (MenuItem_TriggerData*)Gods98::Mem_Alloc(sizeof(MenuItem_TriggerData));
	if (triggerData == nullptr)
		return nullptr;

	triggerData->valuePtr = valuePtr;
	triggerData->end = end;
	triggerData->callback = callback;
	return triggerData;
}

// <merged with freeNonNull @ 0040f6e0>
void __cdecl LegoRR::Front_MenuItem_FreeTrigger(MenuItem_TriggerData* triggerData)
{
	if (triggerData != nullptr) {
		Gods98::Mem_Free(triggerData);
	}
}

// <merged with freeNonNull @ 0040f6e0>
void __cdecl LegoRR::Front_MenuItem_FreeTextInput(MenuItem_TextInputData* textInputData)
{
	if (textInputData != nullptr) {
		Gods98::Mem_Free(textInputData);
	}
}

// <LegoRR.exe @00410ac0>
LegoRR::MenuItem_RealSliderData* __cdecl LegoRR::Front_MenuItem_CreateRealSlider(IN real32* valuePtr, real32 valueMin, real32 valueMax, real32 step,
																				 sint32 x2, sint32 y2, MenuItem_RealSliderCallback callback)
{
	if (valueMax <= valueMin)
		return nullptr;

	MenuItem_RealSliderData* realSliderData = (MenuItem_RealSliderData*)Gods98::Mem_Alloc(sizeof(MenuItem_RealSliderData));
	if (realSliderData == nullptr)
		return nullptr;

	realSliderData->valuePtr = valuePtr;
	realSliderData->valueMin = valueMin;
	realSliderData->valueMax = valueMax;
	realSliderData->valueStep = step;
	realSliderData->x2 = x2;
	realSliderData->y2 = y2;
	realSliderData->callback = callback;
	return realSliderData;
}

// <merged with freeNonNull @ 0040f6e0>
void __cdecl LegoRR::Front_MenuItem_FreeRealSlider(MenuItem_RealSliderData* realSliderData)
{
	if (realSliderData != nullptr) {
		Gods98::Mem_Free(realSliderData);
	}
}

// <LegoRR.exe @00410b20>
LegoRR::MenuItem_SliderData* __cdecl LegoRR::Front_MenuItem_CreateSlider(IN sint32* valuePtr, sint32 valueMin, sint32 valueMax, sint32 x2, sint32 y2,
																		 MenuItem_SliderCallback callback, Gods98::Image* offBarImage, Gods98::Image* onBarImage,
																		 Gods98::Image* leftCapImage, Gods98::Image* rightCapImage, Gods98::Image* loPlusImage,
																		 Gods98::Image* loMinusImage, Gods98::Image* hiPlusImage, Gods98::Image* hiMinusImage)
{
	if (valueMax <= valueMin)
		return nullptr;

	MenuItem_SliderData* sliderData = (MenuItem_SliderData*)Gods98::Mem_Alloc(sizeof(MenuItem_SliderData));
	if (sliderData == nullptr)
		return nullptr;

	sliderData->valuePtr = valuePtr;
	sliderData->x2 = x2;
	sliderData->y2 = y2;
	sliderData->callback = callback;
	sliderData->imageBarOff = offBarImage;
	sliderData->imageBarOn = onBarImage;
	sliderData->imageCapLeft = leftCapImage;
	sliderData->imageCapRight = rightCapImage;
	sliderData->imagePlusLo = loPlusImage;
	sliderData->imageMinusLo = loMinusImage;
	sliderData->valueMin = valueMin;
	sliderData->valueMax = valueMax;
	sliderData->imagePlusHi = hiPlusImage;
	sliderData->imageMinusHi = hiMinusImage;
	return sliderData;
	
}

// <merged with freeNonNull @ 0040f6e0>
void __cdecl LegoRR::Front_MenuItem_FreeSlider(MenuItem_SliderData* sliderData)
{
	if (sliderData != nullptr) {
		Gods98::Mem_Free(sliderData);
	}
}

// <LegoRR.exe @00410ba0>
void __cdecl LegoRR::Front_MenuItem_FreeMenuItem(MenuItem* menuItem)
{
	if (menuItem != nullptr) {
		if (menuItem->banner != nullptr) {
			Gods98::Mem_Free(menuItem->banner);
		}

		// NOTE: All of these functions check for non-null.
		switch (menuItem->itemType) {
		case MenuItem_Type_Cycle:
			Front_MenuItem_FreeCycle(menuItem->itemData.cycle);
			break;
		case MenuItem_Type_Trigger:
			Front_MenuItem_FreeTrigger(menuItem->itemData.trigger);
			break;
		case MenuItem_Type_TextInput:
			Front_MenuItem_FreeTextInput(menuItem->itemData.textInput);
			break;
		case MenuItem_Type_Slider:
			Front_MenuItem_FreeSlider(menuItem->itemData.slider);
			break;
		case MenuItem_Type_RealSlider:
			Front_MenuItem_FreeRealSlider(menuItem->itemData.realSlider);
			break;
		case MenuItem_Type_Next:
			// No itemData allocation for nextData, just a pointer to the menu to go to.
			break;
		case MenuItem_Type_Select:
			Front_MenuItem_FreeSelect(menuItem->itemData.select);
			break;
		}

		/// FIXME: Remove imageLo and imageHi if non-null.

		Gods98::Mem_Free(menuItem);
	}
}

// <LegoRR.exe @00410c80>
LegoRR::MenuItem* __cdecl LegoRR::Front_MenuItem_CreateBannerItem(const char* banner, Gods98::Font* loFont, Gods98::Font* hiFont, sint32 x1, sint32 y1,
																  MenuItem_Type itemType, bool32 centered, void* itemData, bool32 notInTuto)
{
	if (banner == nullptr)
		return nullptr;

	MenuItem* menuItem = (MenuItem*)Gods98::Mem_Alloc(sizeof(MenuItem));
	if (menuItem == nullptr)
		return nullptr;

	std::memset(menuItem, 0, sizeof(MenuItem));

	menuItem->length = std::strlen(banner);
	menuItem->banner = Front_Util_StrCpy(banner);
	menuItem->fontLo = loFont;
	menuItem->fontHi = hiFont;

	menuItem->x1 = x1;
	menuItem->y1 = y1;

	menuItem->itemData.data = itemData; // Typeless void* assignment for union
	menuItem->itemType = itemType;

	menuItem->notInTuto = notInTuto;

	if (centered && loFont != nullptr && hiFont != nullptr) {
		menuItem->centerOffLo = -((sint32)Gods98::Font_GetStringWidth(loFont, banner) / 2);
		menuItem->centerOffHi = -((sint32)Gods98::Font_GetStringWidth(hiFont, banner) / 2);
	}
	else {
		menuItem->centerOffLo = 0;
		menuItem->centerOffHi = 0;
	}
	return menuItem;
}

// <LegoRR.exe @00410d50>
LegoRR::MenuItem* __cdecl LegoRR::Front_MenuItem_CreateImageItem(const char* banner, Gods98::Font* loFont, Gods98::Font* hiFont, char* loImageName, char* hiImageName,
																 sint32 x1, sint32 y1, MenuItem_Type itemType, bool32 centered, char* toolTipName, void* itemData)
{
	if (banner == nullptr)
		return nullptr;

	MenuItem* menuItem = (MenuItem*)Gods98::Mem_Alloc(sizeof(MenuItem));
	if (menuItem == nullptr)
		return nullptr;

	std::memset(menuItem, 0, sizeof(MenuItem));

	menuItem->length = std::strlen(banner);
	menuItem->banner = Front_Util_StrCpy(banner);
	menuItem->fontLo = loFont;
	menuItem->fontHi = hiFont;

	menuItem->x1 = x1;
	menuItem->y1 = y1;

	menuItem->itemData.data = itemData; // Typeless void* assignment for union
	menuItem->itemType = itemType;


	menuItem->imageLo = Gods98::Image_LoadBMP(loImageName);
	if (menuItem->imageLo != nullptr) {

		menuItem->imageHi = Gods98::Image_LoadBMP(hiImageName);
		// Function does not fail when imageHi is not loaded.
		if (menuItem->imageHi != nullptr) {
			menuItem->isImageItem = true;
		}

		if (!ToolTip_GetType(toolTipName, &menuItem->toolTipType)) {
			menuItem->toolTipType = ToolTip_Null;
		}

		if (centered && menuItem->imageLo != nullptr && menuItem->imageHi != nullptr) {
			menuItem->centerOffLo = -((sint32)Gods98::Image_GetWidth(menuItem->imageLo) / 2);
			menuItem->centerOffHi = -((sint32)Gods98::Image_GetWidth(menuItem->imageHi) / 2);
		}
		else {
			menuItem->centerOffLo = 0;
			menuItem->centerOffHi = 0;
		}
		return menuItem;
	}

	/// FIX APPLY: Properly free menuItem if returning null.
	Gods98::Mem_Free(menuItem);
	return nullptr;
}

// <LegoRR.exe @00410e60>
void __cdecl LegoRR::Front_Menu_FreeMenu(Menu* menu)
{
	if (menu != nullptr) {
		if (menu->title != nullptr) {
			Gods98::Mem_Free(menu->title);
		}
		if (menu->fullName != nullptr) {
			Gods98::Mem_Free(menu->fullName);
		}

		for (sint32 i = 0; i < menu->itemCount; i++) {
			if (menu->items[i] != nullptr) {
				Front_MenuItem_FreeMenuItem(menu->items[i]);
			}
		}

		/// FIX APPLY: Move `Mem_Free(menu->items)` after freeing individual item elements.
		if (menu->items != nullptr) {
			Gods98::Mem_Free(menu->items);
		}

		if (menu->menuImage != nullptr) {
			Gods98::Image_Remove(menu->menuImage);
		}

		Gods98::Mem_Free(menu);
	}
}

// <LegoRR.exe @00410ee0>
bool32 __cdecl LegoRR::Front_Menu_LoadMenuImage(Menu* menu, char* filename, bool32 light)
{
	char* stringParts[4] = { nullptr }; // For once, this is NOT a dummy init
	char buff[1024];

	if (filename == nullptr)
		return false;

	std::strcpy(buff, filename);

	
	// cfg: filename.bmp[:xPos:yPos[:trans=0/1]]
	uint32 numParts = Gods98::Util_Tokenise(buff, stringParts, ":");
	
	if (numParts >= 3) {
		menu->flags |= MENU_FLAG_HASPOSITION;
		menu->currPosition.x = (real32)std::atof(stringParts[1]);
		menu->currPosition.y = (real32)std::atof(stringParts[2]);
	}

	Gods98::Image* image = Front_Cache_LoadImage(stringParts[0]);
	bool32 transBool = (image && numParts == 4 && (std::atoi(stringParts[3]) == 1)); // Optional numeric bool '1'

	if (light) {
		menu->menuImage = image;
		if (transBool) {
			Gods98::Image_SetPenZeroTrans(menu->menuImage);
		}
	}
	else {
		menu->menuImageDark = image;
		if (transBool) {
			Gods98::Image_SetupTrans(menu->menuImageDark, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		}
	}
	return (image != nullptr);
}

// <LegoRR.exe @00411030>
//LegoRR::Menu* __cdecl LegoRR::Front_Menu_CreateMenu(char* title, char* fullName, Gods98::Font* menuFont, sint32 positionX, sint32 positionY, bool32 autoCenter, bool32 displayTitle, sint32 param_8, bool32 canScroll, char* anchored_str);

// <LegoRR.exe @00411190>
bool32 __cdecl LegoRR::Front_Menu_AddMenuItem(Menu* menu, MenuItem* menuItem)
{

	if ((menuItem == nullptr) || (menu == nullptr)) {
		return false;
	}

	if (menu->itemCount == menu->itemCapacity) {
		MenuItem** newMenuItems;

		uint32 newCapacity = menu->itemCapacity + 15;
		if (menu->items == nullptr) {
			newMenuItems = (MenuItem**)Gods98::Mem_Alloc(newCapacity * sizeof(MenuItem*));
		}
		else {
			newMenuItems = (MenuItem**)Gods98::Mem_ReAlloc(menu->items, newCapacity * sizeof(MenuItem*));
		}
		if (newMenuItems == nullptr) {
			return false;
		}

		menu->items = newMenuItems;
		menu->itemCapacity = newCapacity;
	}

	/// CUSTOM:
	/*if (menuItem->itemType == MenuItem_Type_Select) {
		if (menuItem->fontHi == nullptr) menuItem->fontHi = Front_Cache_LoadFont("Interface\\Fonts\\MBriefFONT2.bmp");
		if (menuItem->fontLo == nullptr) menuItem->fontLo = Front_Cache_LoadFont("Interface\\Fonts\\MBriefFONT.bmp");
	}*/

	menu->items[menu->itemCount] = menuItem;
	menu->itemCount++;

	return true;
}


// <LegoRR.exe @00411210>
//bool32 __cdecl LegoRR::Front_Maths_IsPointInsideRect(sint32 ptX, sint32 ptY, sint32 rcX, sint32 rcY, sint32 rcWidth, sint32 rcHeight);

// <LegoRR.exe @00411250>
//bool32 __cdecl LegoRR::Front_Maths_IsPointInsideRect_OptCenterX(sint32 ptX, sint32 ptY, sint32 rcX, sint32 rcY, sint32 rcWidth, sint32 rcHeight, bool32 shouldCenterX);

// <LegoRR.exe @00411290>
//sint32 __cdecl LegoRR::Front_MenuItem_Select_CollisionCheck_FUN_00411290(Menu* menu, MenuItem* menuItem, MenuItem_SelectData* selectData);

// <LegoRR.exe @00411420>
//bool32 __cdecl LegoRR::Front_Menu_IsLevelItemUnderMouse(Menu* menu, sint32 itemIndex);

// <LegoRR.exe @00411460>
//bool32 __cdecl LegoRR::Front_Menu_GetItemBounds(Menu* menu, sint32 itemIndex, OUT sint32* rcX, OUT sint32* rcY, OUT sint32* rcWidth, OUT sint32* rcHeight);

// itemIndex is not assigned on failure.
// <LegoRR.exe @004116c0>
//bool32 __cdecl LegoRR::Front_Menu_FindItemUnderMouse(Menu* menu, OUT sint32* itemIndex);

// <LegoRR.exe @00411770>
//bool32 __cdecl LegoRR::Front_GetMousePressedState(void);

// <LegoRR.exe @004117a0>
//bool32 __cdecl LegoRR::Front_MenuItem_SliderHandleInput(Menu* menu, MenuItem* menuItem, MenuItem_SliderData* sliderData);

// <LegoRR.exe @00411900>
//bool32 __cdecl LegoRR::Front_MenuItem_CheckNotInTutoAnyTutorialFlags(MenuItem* menuItem);

// <LegoRR.exe @00411930>
//LegoRR::Menu* __cdecl LegoRR::Front_Menu_UpdateMenuItemsInput(real32 elapsed, Menu* menu);

// See Keys enum
// <LegoRR.exe @00411e30>
//uint32 __cdecl LegoRR::Front_Input_GetKeyCharacter(Keys diKey);

// valueIndex == (value - valueMin);
// valueRange == (valueMax - valueMin);
// <LegoRR.exe @00411e40>
//void __cdecl LegoRR::Front_MenuItem_DrawSlider(MenuItem_SliderData* sliderData, uint32 x, uint32 y, sint32 valueIndex, uint32 valueRange);

// <LegoRR.exe @004120a0>
//uint32 __cdecl LegoRR::Front_Menu_GetOverlayCount(Menu* menu);

// <LegoRR.exe @004120c0>
bool32 __cdecl LegoRR::Front_Menu_ShouldRandomPlay(void)
{
	/// FIX APPLY: Only check for 1-in-400 once every frame.
	///            Making the average overlay appearance once every 16 seconds.
	static real32 frameTimer = 0.0f;
	frameTimer += Gods98::Main_GetDeltaTime();

	while (frameTimer >= 1.0f) {
		frameTimer -= 1.0f;
		if ((Gods98::Maths_Rand() % 400) == 0) {
			// This function is only called while an overlay isn't playing,
			//  so we should reset the timer because any accumulated overtime becomes meaningless.
			frameTimer = 0.0f;
			return true;
		}
	}
	return false;

	/// OLD CODE:
	//return (Gods98::Maths_Rand() % 400) == 0;
}

// <LegoRR.exe @004120e0>
//void __cdecl LegoRR::Front_Menu_UpdateOverlays(Menu* menu);


// <LegoRR.exe @00412380>
void __cdecl LegoRR::Front_MenuItem_DrawSelectItem(sint32 x, sint32 y, Gods98::Font* font, MenuItem_SelectData* selectData, uint32 selIndex, MenuItem_SelectImageType imageType)
{
	// BANNER/IMAGE flags are treated as one or the other.
	if (selectData->selItemList[selIndex].flags & SELECTITEM_FLAG_HASBANNER) {

		/// CUSTOM: Lazy auto-font so we can try this out with levelselect for fun :D
		/*if (!font) {
			font = Front_Cache_LoadFont("Interface\\Fonts\\MBriefFONT2.bmp");// frontGlobs.mainMenuSet->menus[1]->items[0]->fontHi;
		}*/
		Gods98::Font_PrintF(font, x, y, selectData->selItemList[selIndex].banner);
	}
	else {
		Gods98::Image* image = selectData->selItemList[selIndex].images[imageType];
		// HARDCODED SCREEN RESOLUTION!!

		// NOTE: > 0 used here since we're adding width/height (0 would mean everything is off-screen).
		if (image &&
			((x + Gods98::Image_GetWidth(image))  > 0 && x < 640) &&
			((y + Gods98::Image_GetHeight(image)) > 0 && y < 480))
		{
			Point2F destPos = { (real32)x, (real32)y };
			Gods98::Image_DisplayScaled(image, nullptr, &destPos, nullptr);
		}
	}
}

// <LegoRR.exe @00412420>
//void __cdecl LegoRR::Front_MenuItem_DrawSaveImage(Menu* menu, sint32 selIndex, MenuItem_SelectData* selectData, bool32 bigSize);

// <LegoRR.exe @00412680>
//void __cdecl LegoRR::Front_Menu_DrawLoadSaveText(Menu** pMenu, IN OUT bool32* currBool, IN OUT bool32* nextBool);

// <LegoRR.exe @00412900>
//void __cdecl LegoRR::Front_MenuItem_DrawSelectTextWindow(Menu** pMenu);

// <LegoRR.exe @00412a20>
//void __cdecl LegoRR::Front_Menu_DrawMenuImage(Menu* menu, bool32 light);

// <LegoRR.exe @00412b30>
LegoRR::Menu* __cdecl LegoRR::Front_Menu_Update(real32 elapsed, Menu* menu, OUT bool32* optout_bool)
{
	/// REMOVE: This statement has no effect.
	//Front_Save_GetCurrentSaveData();

	const Pointer_Type currPointer = Pointer_GetCurrentType();
	Pointer_SetCurrent_IfTimerFinished(Pointer_Standard);

	Gods98::Viewport* view = legoGlobs.viewMain;
	Gods98::Container* contView = Gods98::Viewport_GetCamera(view);

	g_SaveMenu_INT_004a2f4c = -1;
	menu->itemFocus = -1;

	if (optout_bool != nullptr) {
		*optout_bool = false;
	}

	Menu* currMenu = menu;
	Menu* nextMenu = menu;
	if (!g_FrontBool_004dc8bc && !g_FrontBool_004dc8c8) {
		currMenu = Front_Menu_UpdateMenuItemsInput(elapsed, menu);
	}
	if (menu == frontGlobs.saveMenuSet->menus[0] && (g_SaveMenu_INT_004a2f4c > -1) && (g_SaveMenu_INT_004a2f58 < 0)) {
		menu->closed = false;
		currMenu->closed = false;
	}
	if (g_SaveMenu_INDEX_004a2f50 > -1) {
		currMenu = nextMenu;
	}

	Vector3F origCamUp;
	Vector3F origCamDir;
	Vector3F origCamPos;

	const bool hasRockWipeAnim = (frontGlobs.rockWipeAnim != nullptr && (frontGlobs.rockWipeFlags & ROCKWIPE_FLAG_ANIMATING));

	if (hasRockWipeAnim) {//frontGlobs.rockWipeAnim != nullptr && (frontGlobs.rockWipeFlags & ROCKWIPE_FLAG_ANIMATING)) {
		Gods98::Viewport_Clear(view, true);
		// Setup camera and add rockwipe.
		Gods98::Container_SetParent(frontGlobs.rockWipeAnim, legoGlobs.rootCont);
		Gods98::Container_GetPosition(contView, nullptr, &origCamPos);
		Gods98::Container_GetOrientation(contView, nullptr, &origCamDir, &origCamUp);
		Gods98::Container_SetPosition(contView, nullptr, 0.0f, 0.0f, 5.0f);
		Gods98::Container_SetOrientation(contView, nullptr, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		Gods98::Container_SetAnimationTime(frontGlobs.rockWipeAnim, frontGlobs.rockWipeTimer);
	}

	Front_Menu_DrawMenuImage(menu, true);
	if (menu != frontGlobs.saveMenuSet->menus[0]) {
		Front_Menu_UpdateOverlays(menu);
	}
	// Something tells me these arguments aren't actually bools, and are really menus.
	// These arguments are only checked for zero/non-zero.
	Front_Menu_DrawLoadSaveText(&menu, (bool32*)&currMenu, (bool32*)&nextMenu);

	const sint32 xPos = menu->position.x;
	const sint32 yPos = menu->position.y;

	if (menu->displayTitle) {
		/// REFACTOR: Don't generate the string twice.
		const char* str = Front_Util_ReplaceTextSpaces(menu->fullName);
		const uint32 strWidth = Gods98::Font_GetStringWidth(menu->menuFont, str);
		const sint32 yAnchor = menu->anchoredPosition.y;
		const sint32 screenCenter = Gods98::appWidth() / 2;
		//const char* str = Front_Util_ReplaceTextSpaces(menu->fullName);
		Gods98::Font_PrintF(menu->menuFont, screenCenter - (strWidth / 2), yAnchor + yPos, str);
	}

	// Later changed by MenuItem_Type_Select switch cases.
	g_SaveMenu_INDEX_004a2f48 = -1;

	for (sint32 i = 0; i < menu->itemCount; i++) {
		MenuItem* item = menu->items[i];
		const sint32 y1 = item->y1;
		const sint32 x1 = item->x1;

		if (menu->itemFocus == i && !g_FrontBool_004dc8bc && !Front_MenuItem_CheckNotInTutoAnyTutorialFlags(item)) {
			const sint32 centerOffHi = item->centerOffHi;

			if (item->fontHi != nullptr) {
				const char* str = Front_Util_ReplaceTextSpaces(item->banner);
				Gods98::Font_PrintF(item->fontHi, x1 + xPos + centerOffHi, y1 + yPos, "%s", str);
			}

			switch (item->itemType) {
			case MenuItem_Type_Cycle:
				{
					MenuItem_CycleData* cycle = item->itemData.cycle;
					if (item->fontHi != nullptr) {
						const char* str = Front_Util_ReplaceTextSpaces(cycle->nameList[*cycle->valuePtr]);
						Gods98::Font_PrintF(item->fontHi,
											(cycle->x2 + x1 + xPos + centerOffHi), 
											(cycle->y2 + y1 + yPos), str);
					}
				}
				break;
			case MenuItem_Type_TextInput:
				{
					MenuItem_TextInputData* textInput = item->itemData.textInput;

					/// FIXME: Timer should update based on elapsed time.
					//static sint32 s_frontTextInputCaretBlinker = 0;
					s_frontTextInputCaretBlinker++;
					const bool showCaret = ((s_frontTextInputCaretBlinker / 2) % 2 != 0);

					const char origChar = textInput->valuePtr[textInput->caretPos];

					if (showCaret) {
						/// FIXME: Caret underscore is overwritten by a space in Front_Util_ReplaceTextSpaces.
						textInput->valuePtr[textInput->caretPos] = '_';
					}
					if (item->fontHi != nullptr) {
						const char* str = Front_Util_ReplaceTextSpaces(textInput->valuePtr);
						Gods98::Font_PrintF(item->fontHi,
											(textInput->x2 + x1 + xPos + centerOffHi),
											(textInput->y2 + y1 + yPos), "%s", str);
					}
					if (showCaret) {
						textInput->valuePtr[textInput->caretPos] = origChar;
					}
				}
				break;
			case MenuItem_Type_Slider:
				{
					MenuItem_SliderData* slider = item->itemData.slider;
					Front_MenuItem_DrawSlider(slider,
											  (slider->x2 + x1 + xPos),
											  (slider->y2 + y1 + yPos),
											  *slider->valuePtr - slider->valueMin, slider->valueMax - slider->valueMin);
				}
				break;
			case MenuItem_Type_RealSlider:
				{
					MenuItem_RealSliderData* realSlider = item->itemData.realSlider;
					if (item->fontHi != nullptr) {
						Gods98::Font_PrintF(item->fontHi,
											(realSlider->x2 + x1 + xPos + centerOffHi),
											(realSlider->y2 + y1 + yPos), "%.2f", (double)*realSlider->valuePtr);
					}
				}
				break;
			case MenuItem_Type_Select:
				{
					MenuItem_SelectData* select = item->itemData.select;

					const uint32 selIndex = Front_MenuItem_Select_CollisionCheck_FUN_00411290(menu, item, select);
					g_SaveMenu_INDEX_004a2f48 = selIndex;

					for (sint32 i = 0; i < select->scrollCount; i++) {
						const sint32 currIndex = select->scrollStart + i;
						if ((sint32)currIndex >= (sint32)select->selItemCount) {
							break;
						}

						MenuItem_SelectItem* selectItem = &select->selItemList[currIndex];
						const bool isSelected = (currIndex == selIndex);

						MenuItem_SelectImageType imageType = MenuItem_SelectImage_Light;
						if (!(selectItem->flags & SELECTITEM_FLAG_ENABLED)) {
							imageType = MenuItem_SelectImage_Locked;
						}
						else if (!isSelected) {
							imageType = MenuItem_SelectImage_Dark;
						}

						sint32 centerOff = 0;
						if (menu->autoCenter) {
							centerOff = -((sint32)select->widths[MenuItem_SelectImage_Light][currIndex] / 2);
						}

						const sint32 selItemX = select->x2 + x1 + xPos + centerOff +
							selectItem->frontEndX + frontGlobs.scrollOffset.x;
						const sint32 selItemY = select->y2 + y1 + yPos + (select->selItemHeight * i) +
							selectItem->frontEndY + frontGlobs.scrollOffset.y;

						if (isSelected) {
							Front_MenuItem_DrawSelectItem(selItemX, selItemY, item->fontHi, select, currIndex, imageType);

							if (select->callback != nullptr) {
								select->callback(elapsed, selIndex);
							}
						}
						else {

							Front_MenuItem_DrawSelectItem(selItemX, selItemY, item->fontLo, select, currIndex, imageType);
						}
						Front_MenuItem_DrawSaveImage(menu, currIndex, select, isSelected);
					}

					if (select->scrollStart < (sint32)(select->selItemCount - select->scrollCount)) {

						Gods98::Font* font = ((selIndex == -2) ? item->fontHi : item->fontLo);
						if (font != nullptr) {
							Gods98::Font_PrintF(font,
												(select->xString1 + xPos),
												(select->yString1 + yPos), select->string1);
						}
					}

					if (select->scrollStart > 0) {
						Gods98::Font* font = ((selIndex == -3) ? item->fontHi : item->fontLo);
						if (font != nullptr) {
							Gods98::Font_PrintF(font,
												(select->xString2 + xPos),
												(select->yString2 + yPos), select->string2);
						}
					}
				}
				break;
			}

		}
		else {
			const sint32 centerOffLo = item->centerOffLo;

			if (item->fontLo != nullptr) {
				const char* str = Front_Util_ReplaceTextSpaces(item->banner);
				Gods98::Font_PrintF(item->fontLo, x1 + xPos + centerOffLo, y1 + yPos, "%s", str);
			}

			switch (item->itemType) {
			case MenuItem_Type_Cycle:
				{
					MenuItem_CycleData* cycle = item->itemData.cycle;
					if (item->fontLo != nullptr) {
						const char* str = Front_Util_ReplaceTextSpaces(cycle->nameList[*cycle->valuePtr]);
						Gods98::Font_PrintF(item->fontLo,
											(cycle->x2 + x1 + xPos + centerOffLo),
											(cycle->y2 + y1 + yPos), str);
					}
				}
				break;
			case MenuItem_Type_TextInput:
				{
					MenuItem_TextInputData* textInput = item->itemData.textInput;
					if (item->fontLo != nullptr) {
						const char* str = Front_Util_ReplaceTextSpaces(textInput->valuePtr);
						Gods98::Font_PrintF(item->fontLo,
											(textInput->x2 + x1 + xPos + centerOffLo),
											(textInput->y2 + y1 + yPos), "%s", str);
					}
				}
				break;
			case MenuItem_Type_Slider:
				{
					MenuItem_SliderData* slider = item->itemData.slider;
					Front_MenuItem_DrawSlider(slider,
											  (slider->x2 + x1 + xPos),
											  (slider->y2 + y1 + yPos),
											  *slider->valuePtr - slider->valueMin, slider->valueMax - slider->valueMin);
				}
				break;
			case MenuItem_Type_RealSlider:
				{
					MenuItem_RealSliderData* realSlider = item->itemData.realSlider;
					if (item->fontLo != nullptr) {
						Gods98::Font_PrintF(item->fontLo,
											(realSlider->x2 + x1 + xPos + centerOffLo),
											(realSlider->y2 + y1 + yPos), "%.2f", (double)*realSlider->valuePtr);
					}
				}
				break;
			case MenuItem_Type_Select:
				{
					MenuItem_SelectData* select = item->itemData.select;

					for (sint32 i = 0; i < select->scrollCount; i++) {
						const sint32 currIndex = select->scrollStart + i;
						if ((sint32)currIndex >= (sint32)select->selItemCount) {
							break;
						}

						MenuItem_SelectItem* selectItem = &select->selItemList[currIndex];

						MenuItem_SelectImageType imageType = MenuItem_SelectImage_Dark;
						if (!(selectItem->flags & SELECTITEM_FLAG_ENABLED)) {
							imageType = MenuItem_SelectImage_Locked;
						}

						sint32 centerOff = 0;
						if (menu->autoCenter) {
							centerOff = -((sint32)select->widths[MenuItem_SelectImage_Light][currIndex] / 2);
						}

						const sint32 selItemX = select->x2 + x1 + xPos + centerOff +
							selectItem->frontEndX + frontGlobs.scrollOffset.x;
						const sint32 selItemY = select->y2 + y1 + yPos + (select->selItemHeight * i) +
							selectItem->frontEndY + frontGlobs.scrollOffset.y;
						
						Front_MenuItem_DrawSelectItem(selItemX, selItemY, item->fontLo, select, currIndex, imageType);

						Front_MenuItem_DrawSaveImage(menu, currIndex, select, false);
					}

					if (select->scrollStart < (sint32)(select->selItemCount - select->scrollCount)) {
						if (item->fontLo != nullptr) {
							Gods98::Font_PrintF(item->fontLo,
												(select->xString1 + xPos),
												(select->yString1 + yPos), select->string1);
						}
					}

					if (select->scrollStart > 0) {
						if (item->fontLo != nullptr) {
							Gods98::Font_PrintF(item->fontLo,
												(select->xString2 + xPos),
												(select->yString2 + yPos), select->string2);
						}
					}
				}
				break;
			}

		}
	}


	Front_MenuItem_DrawSelectTextWindow(&menu);

	for (sint32 i = 0; i < menu->itemCount; i++) {
		const MenuItem* item = menu->items[i];
		if (item->isImageItem) {
			if (menu->itemFocus == i) {
				if (item->imageHi != nullptr) {
					const Point2F destPos = {
						static_cast<real32>(item->x1 + menu->position.x),
						static_cast<real32>(item->y1 + menu->position.y),
					};
					Gods98::Image_DisplayScaled(item->imageHi, nullptr, &destPos, nullptr);
				}
				ToolTip_Activate(item->toolTipType);
			}
			else {
				if (item->imageLo != nullptr) {
					const Point2F destPos = {
						static_cast<real32>(item->x1 + menu->position.x),
						static_cast<real32>(item->y1 + menu->position.y),
					};
					Gods98::Image_DisplayScaled(item->imageLo, nullptr, &destPos, nullptr);
				}
			}
		}
	}

	if ((Gods98::Main_GetFlags() & Gods98::MainFlags::MAIN_FLAG_SHOWVERSION) && (Gods98::Main_ProgrammerMode() != 0) &&
		frontGlobs.versionFont != nullptr && frontGlobs.versionString != nullptr)
	{
		Gods98::Font_PrintF(frontGlobs.versionFont, 545, 450, frontGlobs.versionString);
	}

	Front_LevelSelect_LevelNamePrintF(nullptr, 0, 0, nullptr);

	if (menu == frontGlobs.saveMenuSet->menus[0] && g_FrontBool_004dc8bc) {
		char buff[2048];
		std::sprintf(buff, frontGlobs.langOverwriteMessage, (g_SaveMenu_OutNumber + 1)); // +1 since save numbers aren't 0-indexed.

		g_SaveMenu_INT_004a2f58 = Lego_SaveMenu_ConfirmMessage_FUN_004354f0(frontGlobs.langOverwriteTitle, buff, frontGlobs.langOverwriteOK, frontGlobs.langOverwriteCancel);
		if (g_SaveMenu_INT_004a2f58 == 0) {
			menu->closed = true;
			frontGlobs.saveBool_548 = true;
			g_FrontBool_004dc8bc = false;
			g_SaveMenu_OutNumber = -1;
		}
		else if (g_SaveMenu_INT_004a2f58 == 1) {
			menu->closed = true;
			frontGlobs.saveBool_548 = false;
			g_FrontBool_004dc8bc = false;
		}
	}

	/// REFACTOR: Only update rockWipe anim if we previously setup the camera (add hasRockWipeAnim check).
	if (hasRockWipeAnim && frontGlobs.rockWipeAnim != nullptr && (frontGlobs.rockWipeFlags & ROCKWIPE_FLAG_ANIMATING)) {
		Gods98::Container_Hide(frontGlobs.rockWipeLight, false);
		Gods98::Viewport_Render(view, legoGlobs.rootCont, 0.0f);
		Gods98::Container_Hide(frontGlobs.rockWipeLight, true);
		// Restore camera and remove rockwipe.
		Gods98::Container_SetPosition(contView, nullptr, origCamPos.x, origCamPos.y, origCamPos.z);
		Gods98::Container_SetOrientation(contView, nullptr, origCamDir.x, origCamDir.y, origCamDir.z, origCamUp.x, origCamUp.y, origCamUp.z);
		Gods98::Container_SetParent(frontGlobs.rockWipeAnim, nullptr);

		// Update rockwipe animation, and handle transition logic.
		const uint32 animFrames = Gods98::Container_GetAnimationFrames(frontGlobs.rockWipeAnim);
		
		const uint32 time = Gods98::Main_GetTime();
		
		/// FIX APPLY: Store rockWipeLastUpdateTime in uint32 instead of float.
		real32 delta = (real32)(time - _rockWipeLastUpdateTimeFixed);
		delta *= 0.001f * STANDARD_FRAMERATE; // Milliseconds to standard units.
		delta *= 2.0f; // Animation runs at 2 frames per standard unit.
		// Cap animation speed to at most 3 frames per tick.
		if (delta > 3.0f) delta = 3.0f;

		// When half of the animation is finished, switch to the next menu screen.
		const real32 halfAnimFrames = (real32)animFrames * 0.5f;
		if (frontGlobs.rockWipeTimer <= halfAnimFrames && frontGlobs.rockWipeTimer + delta >= halfAnimFrames) {
			/// FIXME: It seems menu transition logic requires the rockWipe animation to exist.
			///        This should be changed so transitions are instant otherwise.
			if (optout_bool != nullptr) {
				*optout_bool = true;
			}
			g_SaveMenu_INDEX_004a2f50 = -1;
			g_SaveMenu_INT_004a2f4c = -1;
			g_FrontBool_004dc8c8 = 0;
			g_FrontBool_004dc8c0 = 0;
			if (frontGlobs.overlayImageOrFlic.data != nullptr) {
				frontGlobs.overlayStartTime = 0;
				frontGlobs.overlayCurrTime = 0;
			}
		}
		else if (frontGlobs.rockWipeTimer > (real32)animFrames) {
			// This condition could be refactored to before Main_GetTime(),
			//  and remaining logic could be skipped in that case.
			Front_RockWipe_Stop();
		}

		frontGlobs.rockWipeTimer += delta;
		/// FIX APPLY: Store rockWipeLastUpdateTime in uint32 instead of float.
		_rockWipeLastUpdateTimeFixed = time;
		// This variable has been replaced by _rockWipeLastUpdateTimeFixed.
		frontGlobs.rockWipeLastUpdateTime = (real32)_rockWipeLastUpdateTimeFixed;
	}

	Pointer_DrawPointer(Gods98::msx(), Gods98::msy());
	Pointer_SetCurrent_IfTimerFinished(currPointer);

	return currMenu;
}

// <LegoRR.exe @004138a0>
void __cdecl LegoRR::Front_Menu_UpdateMousePosition(Menu* menu)
{
	
	if (menu->menuImage == nullptr || !(menu->flags & MENU_FLAG_CANSCROLL)) {
		frontGlobs.scrollOffset.x = 0;
		frontGlobs.scrollOffset.y = 0;
	}
	else {
		// HARDCODED SCREEN RESOLUTION!!
		real32 mouseY = (real32)Gods98::msy();
		// If mouseY is not between 200.0f and 280.0f.
		if (mouseY < 200.0f || mouseY > 280.0f) {

			sint32 imageHeight = Gods98::Image_GetHeight(menu->menuImage);
			if (mouseY < 480.0f && !Gods98::mslb() && !Gods98::msrb()) {
				/// FIXME: Fixed timing for scroll speed.
				//real32 fVar3 = (real32)Gods98::msy() * 0.2083333f - 50.0f;
				real32 fVar3 = (real32)Gods98::msy() * (100.0f / 480.0f) - 50.0f;
				fVar3 *= fVar3 * 0.02f; // squared * 0.02f
				if (mouseY < (480.0f / 2.0f)) {
					fVar3 = -fVar3;
				}
				fVar3 *= -(STANDARD_FRAMERATE / 100.0f); // * -0.25f
				frontGlobs.scrollOffset.y += (sint32)fVar3;
			}

			if (frontGlobs.scrollOffset.y > 0) {
				frontGlobs.scrollOffset.y = 0;
			}
			if (frontGlobs.scrollOffset.y < (480 - imageHeight)) {
				frontGlobs.scrollOffset.y = (480 - imageHeight);
			}
		}
	}
}

// <LegoRR.exe @00413990>
void __cdecl LegoRR::Front_LoadSaveSlotImages(void)
{
	// Max valid size of SaveImage Path in game is 184, but only due to field reuse optimization, other areas use 128 buffer sizes.
	char buff[128];

	for (uint32 i = 0; i < _countof(g_SaveSlotImages_TABLE); i++) {
		/// FIX APPLY: Remove yet another memory leak from using Config_GetStringValue and no Mem_Free...
		const char* saveImagePath = Gods98::Config_GetTempStringValue(legoGlobs.config, Config_ID(legoGlobs.gameName, "Menu::SaveImage", "Path"));
		std::sprintf(buff, "%s\\%d.dat", saveImagePath, i);
		g_SaveSlotImages_TABLE[i] = Gods98::Image_LoadBMP(buff);
	}


	// Functionally this entire block does nothing, it was likely some debug check at one point.
	for (uint32 i = 0; i < _countof(g_SaveSlotPtrs_TABLE); i++) {

		// For some reason the first field is zeroed out manually, this may actally be a substruct of size 0xb4...
		SaveData saveData;
		saveData.field_0x0 = 0;
		std::memset((uint32*)&saveData + 1, 0, sizeof(saveData) - sizeof(uint32));

		// If this isn't the case, just use this memset for the entire struct.
		//std::memset(&saveData, 0, sizeof(saveData));


		Front_Save_ReadSaveFile(i, &saveData, true);

		g_SaveSlotPtrs_TABLE[i] = 0;
		// Hardcoded tutorial count (8).
		for (uint32 j = 0; j < (saveData.missionsCount - 8); j++) {
			real32 scorePercent = saveData.missionsTable[8 + j].reward.items[Reward_Score].percentFloat;
			g_SaveSlotPtrs_TABLE[i] += (scorePercent != 0.0f ? 4 : 0); // Yeah... no clue what's with the 4 -or- 0.
		}

		// Free the missions table, as we only needed to load it for this loop.
		Gods98::Mem_Free(saveData.missionsTable);
	}
}

// <LegoRR.exe @00413a80>
void __cdecl LegoRR::Front_FreeSaveSlotImages(void)
{
	for (uint32 i = 0; i < _countof(g_SaveSlotImages_TABLE); i++) {
		if (g_SaveSlotImages_TABLE[i] != nullptr) {
			Gods98::Image_Remove(g_SaveSlotImages_TABLE[i]);
			/// FIX APPLY: Set to null.
			g_SaveSlotImages_TABLE[i] = nullptr;
		}
	}
}

// <LegoRR.exe @00413ab0>
//void __cdecl LegoRR::Front_ScreenMenuLoop(Menu* menu);

// <LegoRR.exe @00413d50>
//void __cdecl LegoRR::Front_RunScreenMenu(MenuSet* menuSet, uint32 menuIndex);

// <LegoRR.exe @00413d90>
LegoRR::MenuItem_Type __cdecl LegoRR::Front_MenuItem_ParseTypeString(const char* itemTypeName)
{
	if (::_stricmp(itemTypeName, "cycle") == 0)
		return MenuItem_Type_Cycle;

	else if (::_stricmp(itemTypeName, "trigger") == 0)
		return MenuItem_Type_Trigger;

	else if (::_stricmp(itemTypeName, "textinput") == 0)
		return MenuItem_Type_TextInput;

	else if (::_stricmp(itemTypeName, "slider") == 0)
		return MenuItem_Type_Slider;

	else if (::_stricmp(itemTypeName, "realslider") == 0)
		return MenuItem_Type_RealSlider;

	else if (::_stricmp(itemTypeName, "next") == 0)
		return MenuItem_Type_Next;

	else
		return MenuItem_Type_Invalid;
}

// Returns str.
// <LegoRR.exe @00413e30>
char* __cdecl LegoRR::Front_Util_StringReplaceChar(char* str, char origChar, char newChar)
{
	for (char* s = str; *s != '\0'; s++) {

		if (*s == origChar) *s = newChar;
	}
	return str;
}

// <LegoRR.exe @00413e60>
LegoRR::MenuOverlay_Type __cdecl LegoRR::Front_Menu_GetOverlayType(MenuOverlay* menuOverlay)
{
	// Find extension in filename.
	/// FIX APPLY: Memory access violation if there's no '.' extension.
	char* ext = menuOverlay->filename;
	while (*ext != '.' && *ext != '\0') ext++;
	//while (*ext != '.') ext++;

	if (::_stricmp(ext, ".avi") == 0)
		return MenuOverlay_Type_Animation;

	else if (::_stricmp(ext, ".bmp") == 0)
		return MenuOverlay_Type_Image;

	else if (::_stricmp(ext, ".lws") == 0)
		return MenuOverlay_Type_Lws;

	else
		return MenuOverlay_Type_Flic; // Default, assumed extension is ".flh"(?)
}

// <LegoRR.exe @00413ec0>
LegoRR::MenuOverlay* __cdecl LegoRR::Front_Menu_CreateOverlay(const char* filename, IN OUT MenuOverlay** linkedOverlay, sint32 positionX, sint32 positionY, SFX_ID sfxType)
{
	MenuOverlay* menuOverlay = (MenuOverlay*)Gods98::Mem_Alloc(sizeof(MenuOverlay));
	std::memset(menuOverlay, 0, sizeof(MenuOverlay));
	
	/// FIXME: Okay... so don't use a filename that STARTS with "null"...?
	if (::_strnicmp(filename, "Null", 4) != 0) {
		if (filename != nullptr) {
			menuOverlay->filename = Front_Util_StrCpy(filename);
		}

		menuOverlay->overlayType = Front_Menu_GetOverlayType(menuOverlay);
		menuOverlay->position.x = positionX;
		menuOverlay->position.y = positionY;
		menuOverlay->sfxType = sfxType;
		menuOverlay->field_1c = 0;

		menuOverlay->previous = *linkedOverlay;
		*linkedOverlay = menuOverlay;
	}
	return menuOverlay;
}

// <LegoRR.exe @00413f40>
void __cdecl LegoRR::Front_Menu_LoadSliderImages(sint32 numParts, IN char** stringParts, OUT Gods98::Image** images)
{
	if (numParts == 16) {
		for (uint32 i = 0; i < 8; i++) {
			images[i] = Front_Cache_LoadImage(stringParts[8 + i]); // Slider images start at index 8.

			if (images[i] != nullptr) {
				Gods98::Image_SetPenZeroTrans(images[i]);
			}
		}
	}
	else {
		for (uint32 i = 0; i < 8; i++) {
			images[i] = nullptr;
		}
	}
}

// <LegoRR.exe @00413fa0>
LegoRR::MenuSet* __cdecl LegoRR::Front_CreateMenuSet(uint32 menuCount)
{
	MenuSet* menuSet = (MenuSet*)Gods98::Mem_Alloc(sizeof(MenuSet));
	//menuSet->menus = nullptr;
	//menuSet->menuCount = 0;

	menuSet->menus = (Menu**)Gods98::Mem_Alloc(menuCount * sizeof(Menu*));
	std::memset(menuSet->menus, 0, (menuCount * sizeof(Menu*)));
	menuSet->menuCount = menuCount;

	return menuSet;
}

// <LegoRR.exe @00413ff0>
//LegoRR::MenuSet* __cdecl LegoRR::Front_LoadMenuSet(const Gods98::Config* unused_config, const char* menuName, void* dst, void* callback, ...);

// <LegoRR.exe @00414bc0>
sint32 __cdecl LegoRR::Front_GetMenuIDByName(MenuSet* menuSet, const char* name)
{
	for (sint32 i = 0; i < (sint32)menuSet->menuCount; i++) {
		if (menuSet->menus[i] != nullptr && ::_stricmp(menuSet->menus[i]->title, name) == 0)
			return i;
	}
	return -1;
}

// <LegoRR.exe @00414c10>
bool32 __cdecl LegoRR::Front_IsIntrosEnabled(void)
{
	bool32 dontPlay = Config_GetBoolOrFalse(legoGlobs.config, Config_ID(legoGlobs.gameName, "Main", "DontPlayAvis"));
	//BoolTri dontPlay = Gods98::Config_GetBoolValue(legoGlobs.config, Config_ID(legoGlobs.gameName, "Main", "DontPlayAvis"));

	/// ADDME: Commandline option to stop intros from playing entirely (like LRR:CE).
	//if (Gods98::mainGlobs2.cmdNoIntro)
	//	return false;

	return (!dontPlay && (Gods98::Main_ProgrammerMode() < 3));
	//return ((dontPlay != BoolTri::BOOL3_TRUE) && (Gods98::Main_ProgrammerMode() < 3));
}

// <LegoRR.exe @00414c60>
void __cdecl LegoRR::Front_Callback_SliderBrightness(sint32 slider_0_10)
{
	real32 colour = (real32)slider_0_10 * 0.1f;
	real32 colourFP = (colour * 0.8f) + 0.2f;

	Gods98::Container_SetColourAlpha(legoGlobs.spotlightTop, colour, colour, colour, 1.0f);
	Gods98::Container_SetColourAlpha(legoGlobs.pointLightFP, colourFP, colourFP, colourFP, 1.0f);
	Gods98::Container_Light_SetSpotPenumbra(legoGlobs.spotlightTop, (colour * 1.5f) + 0.5f);
	Gods98::Container_Light_SetSpotUmbra(legoGlobs.spotlightTop,    (colour * 0.8f) + 0.2f); // same as colourFP
}

// <LegoRR.exe @00414d10>
void __cdecl LegoRR::Front_Callback_SliderSoundVolume(sint32 slider_0_10)
{
	Gods98::Sound3D_SetGlobalVolumePrescaled(slider_0_10);
}

// <LegoRR.exe @00414d20>
void __cdecl LegoRR::Front_Callback_SliderMusicVolume(sint32 slider_0_10)
{
	Gods98::Main_SetCDVolume((real32)slider_0_10 * (1.0f / 9.0f), (real32)slider_0_10 * (1.0f / 9.0f));
}

// <LegoRR.exe @00414d40>
sint32 __cdecl LegoRR::Front_CalcSliderCDVolume(void)
{
	real32 leftVol = 0.0f, rightVol = 0.0f; // dummy inits

	if (Gods98::Main_GetCDVolume(&leftVol, &rightVol)) {
		return (sint32)(((leftVol + rightVol) / 2.0f) * 9.0f);
	}
	return 0;
}

// <LegoRR.exe @00414d80>
void __cdecl LegoRR::Front_Callback_CycleWallDetail(sint32 cycle_High_Low)
{
	switch (cycle_High_Low) {
	case 0: // "High"
		legoGlobs.flags1 |= GAME1_USEDETAIL;
		break;
	case 1: // "Low"
		legoGlobs.flags1 &= ~GAME1_USEDETAIL;
		break;
	}
}

// <LegoRR.exe @00414db0>
void __cdecl LegoRR::Front_Callback_CycleAutoGameSpeed(sint32 cycle_On_Off)
{
	Info_SetAutoGameSpeed((cycle_On_Off == 0));
}

// <LegoRR.exe @00414dd0>
void __cdecl LegoRR::Front_Callback_CycleMusic(sint32 cycle_On_Off)
{
	Lego_SetMusicOn((cycle_On_Off == 0));
	//Lego_SetMusicPlaying((cycle_On_Off == 0));
}

// <LegoRR.exe @00414df0>
void __cdecl LegoRR::Front_Callback_CycleSound(sint32 cycle_On_Off)
{
	Lego_SetSoundOn((cycle_On_Off == 0));
}

// <LegoRR.exe @00414e10>
void __cdecl LegoRR::Front_Callback_CycleHelpWindow(sint32 cycle_Off_On)
{
	// Is this a bug? The opposite is used for other menu cycles...
	HelpWindow_SetEnabled(false, (cycle_Off_On == 0));

	/*switch (cycle_Off_On) {
	case 0: // "Off"
		HelpWindow_SetEnabled(false, true);
		break;
	case 1: // "On"
		HelpWindow_SetEnabled(false, false);
		break;
	}*/
}

// <LegoRR.exe @00414e40>
void __cdecl LegoRR::Front_Callback_TriggerReplayObjective(void)
{
	Objective_SetStatus(LevelStatus::LEVELSTATUS_INCOMPLETE);
}

// <LegoRR.exe @00414e50>
void __cdecl LegoRR::Front_Callback_SliderGameSpeed(sint32 slider_0_5)
{
	switch (slider_0_5) {
	case 0:
		legoGlobs.gameSpeed = (1.0f / 3.0f); // 0.3333333f
		break;
	case 1:
		legoGlobs.gameSpeed = (2.0f / 3.0f); // 0.6666667f
		break;
	default: // 100% is default for invalid value.
	case 2:
		legoGlobs.gameSpeed = (3.0f / 3.0f); // 1.0f
		break;
	case 3:
		legoGlobs.gameSpeed = (4.0f / 3.0f); // 1.333333f
		break;
	case 4:
		legoGlobs.gameSpeed = (5.0f / 3.0f); // 1.666667f
		break;
	case 5:
		legoGlobs.gameSpeed = (6.0f / 3.0f); // 2.0f
		break;
	}
}

// This function can be replaced with:
//  `sliderGameSpeed = Front_CalcSliderGameSpeed();`
// <LegoRR.exe @00414ec0>
void __cdecl LegoRR::Front_UpdateSliderGameSpeed(void)
{
	// Original function just performed same thing as `Front_CalcSliderGameSpeed`
	//  and assigned to `sliderGameSpeed` instead of returning.
	frontGlobs.sliderGameSpeed = Front_CalcSliderGameSpeed();
}

// <LegoRR.exe @00414f60>
sint32 __cdecl LegoRR::Front_CalcSliderGameSpeed(void)
{
	// Game Speed to slider levels is calculated in 3rd's:
	//  0 = below 2/3 (<  66%)
	//  1 = below 3/3 (< 100%)
	//  2 = below 4/3 (< 133%)
	//  3 = below 5/3 (< 166%)
	//  4 = below 6/3 (< 200%)
	//  5 = equal or above (6/3) (>=200%)
	if (legoGlobs.gameSpeed      < (2.0f / 3.0f)) // 0.6666666f  [  0%, 66%)
		return 0;

	else if (legoGlobs.gameSpeed < (3.0f / 3.0f)) // 1.0f        [ 66%,100%)
		return 1;

	else if (legoGlobs.gameSpeed < (4.0f / 3.0f)) // 1.333333f   [100%,133%)
		return 2;

	else if (legoGlobs.gameSpeed < (5.0f / 3.0f)) // 1.666667f   [133%,166%)
		return 3;

	else if (legoGlobs.gameSpeed < (6.0f / 3.0f)) // 2.0f        [166%,200%)
		return 4;

	else                                          //             [200%,...]
		return 5;
}

// <LegoRR.exe @00414fe0>
void __cdecl LegoRR::Front_Callback_SelectLoadSave(real32 elapsedAbs, sint32 selectIndex)
{
	Front_Save_SetSaveNumber(selectIndex);


	bool32 keepLocked;
	SaveReward* rewardsTable;
	if (Front_Save_GetCurrentSaveData() != nullptr) {
		keepLocked = false;
		rewardsTable = Front_Save_GetCurrentSaveData()->missionsTable;
	}
	else {
		keepLocked = true;
		rewardsTable = nullptr;
	}
	MenuItem_SelectData* missionSelectData = frontGlobs.mainMenuSet->menus[1]->items[1]->itemData.select;

	Front_Levels_UpdateAvailable(frontGlobs.startMissionLink, rewardsTable,
								 &frontGlobs.missionLevels, missionSelectData, keepLocked);


	MenuItem_SelectData* tutoSelectData = frontGlobs.mainMenuSet->menus[2]->items[1]->itemData.select;

	Front_Levels_UpdateAvailable(frontGlobs.startTutorialLink, nullptr,
								 &frontGlobs.tutorialLevels, tutoSelectData, false);
}

// <LegoRR.exe @00415080>
void __cdecl LegoRR::Front_UpdateOptionsSliders(void)
{
	sint32 saveIndex = Front_Save_GetSaveNumber();
	SaveData* save = Front_Save_GetSaveDataAt(saveIndex);
	if (save != nullptr) {
		Front_Callback_SliderSoundVolume(save->SliderSoundVolume);
		Front_Callback_SliderBrightness(save->SliderBrightness);
		Front_Callback_SliderMusicVolume(save->SliderMusicVolume);
	}
}

// <LegoRR.exe @004150c0>
void __cdecl LegoRR::Front_Callback_TriggerBackSave(void)
{
	frontGlobs.isLoadModeBool_544 = false;
	frontGlobs.saveBool_548 = false;
	g_SaveMenu_INT_004a2f4c = -1;

	MenuItem_SelectData* selectData = frontGlobs.mainMenuSet->menus[1]->items[1]->itemData.select;

	if (Front_Save_GetCurrentSaveData() != nullptr) {

		SaveData* currSave = Front_Save_GetCurrentSaveData();
		Front_Levels_UpdateAvailable(frontGlobs.startMissionLink, currSave->missionsTable,
									 &frontGlobs.missionLevels, selectData, false);
	}
	else {
		Front_Levels_UpdateAvailable(frontGlobs.startMissionLink, nullptr,
									 &frontGlobs.missionLevels, selectData, true);
	}
}

// <LegoRR.exe @00415150>
void __cdecl LegoRR::Front_RestartLevel(void)
{
	char tempLevelNameBuff[128];
	std::strcpy(tempLevelNameBuff, legoGlobs.currLevel->name);

	Lego_SetPaused(false, false);

	Level_Free();

	// Override Lego_LoadLevel to store SeeThroughWalls property.
	/// CHANGE: Don't allocate a new string, since Lego_LoadLevel2 already expects to receive a temporary string.
	Lego_LoadLevel2(tempLevelNameBuff);
	//Lego_LoadLevel2(Gods98::Util_StrCpy(tempLevelNameBuff));

	// Make sure all menus are closed if we came from Restart Mission in the options menu.
	MenuSet* menuSet = frontGlobs.pausedMenuSet;
	for (sint32 i = 0; i < (sint32)menuSet->menuCount; i++) {

		if (menuSet->menus[i] != nullptr) menuSet->menus[i]->closed = true;
	}

	frontGlobs.triggerRestartMission = false;
	legoGlobs.flags1 |= GAME1_PAUSED;

	ObjectRecall_Save_CreateNewObjectRecall();
}

// <LegoRR.exe @004151f0>
void __cdecl LegoRR::Front_Save_GetLevelCompleteWithPoints(OPTIONAL SaveData* saveData, OUT char* buffer)
{
	if (saveData != nullptr) {
		sint32 totalScore = 0;
		sint32 totalCompleted = 0;

		for (uint32 i = 0; i < saveData->missionsCount; i++) {
			totalScore += Front_Save_GetLevelScore(i, saveData);
			if (saveData->missionsTable[i].flags & SAVEREWARD_FLAG_COMPLETED) {
				totalCompleted++;
			}
		}

		if (totalCompleted != 0) {
			sint32 percentCompleted = (sint32)(((real32)totalCompleted / (real32)saveData->missionsCount) * 100.0f);
			// Some hardcoded language text here...
			std::sprintf(buffer, "Game %i%% complete with %i points scored", percentCompleted, totalScore);
		}
	}
}

// <LegoRR.exe @00415290>
void __cdecl LegoRR::Front_UpdateGameSpeedSliderLevel(void)
{
	frontGlobs.sliderGameSpeed = Front_CalcSliderGameSpeed();
}

// param isOptions: 0 = PausedMenu, 1 = OptionsMenu, 2 = not supported
// <LegoRR.exe @004152a0>
bool32 __cdecl LegoRR::Front_Options_Update(real32 elapsed, Menu_ModalType modalType)
{
	Front_UpdateGameSpeedSliderLevel();

	MenuSet* menuSet;// = frontGlobs.pausedMenuSet;
	switch (modalType) {
	case Menu_Modal_Paused:
		menuSet = frontGlobs.pausedMenuSet;
		break;

	case Menu_Modal_Options:
		menuSet = frontGlobs.optionsMenuSet;
		break;

	default:
		return true; // Return now, no other supported modal types.
	}


	// I think this wonkey if block is used to check both if a modal menu is opened OR closed...

	// `s_FrontOptionsMenu == nullptr` effectively "checks" if this function has never been called before with a valid modal type.
	// `s_FrontOptionsMenuSet != menuSet` effectively checks if the last call to this function was the same modal type.
	if (s_FrontOptionsMenu == nullptr || s_FrontOptionsMenuSet != menuSet ||
		frontGlobs.triggerRestartMission != FALSE || frontGlobs.triggerQuitMission != FALSE ||
		frontGlobs.triggerReplayObjective != FALSE || Objective_IsShowing())
	{
		frontGlobs.triggerQuitMission = false;

		// When opening a modal menu, always open on the first Menu in the set.
		s_FrontOptionsMenu = menuSet->menus[0];
		s_FrontOptionsMenuSet = menuSet;

		if (frontGlobs.triggerReplayObjective != FALSE) {
			// Set menus to closed when pressing this trigger(?)

			if (menuSet->menus[0] != nullptr) menuSet->menus[0]->closed = true;

			// Access violation for OptionsMenu???
			// 
			// I think this is a bug where this assignment was meant to check `Menu_Modal_Paused`,
			// while the assignment below to reset `triggerReplayObjective` should have been for all menu types.
			if (menuSet->menus[2] != nullptr) menuSet->menus[2]->closed = true;

			if (modalType == Menu_Modal_Paused) frontGlobs.triggerReplayObjective = false;

			// POTENTIAL FIX: Only use once testing has been done to confirm this should always be reset.
			/*switch (modalType) {
			case Menu_Modal_Paused:
				if (menuSet->menus[0] != nullptr) menuSet->menus[0]->closed = true;
				if (menuSet->menus[2] != nullptr) menuSet->menus[2]->closed = true;
				//frontGlobs.triggerReplayObjective = false;
				break;

			case Menu_Modal_Options:
				if (menuSet->menus[0] != nullptr) menuSet->menus[0]->closed = true;
				break;
			}
			frontGlobs.triggerReplayObjective = false;*/

			return false;
		}
		else if (frontGlobs.triggerRestartMission == TRUE) {
			return false;
		}

		// Otherwise fallthrough to remainder of function...
	}


	if (!Objective_IsShowing()) {
		// From here, the current modal menu can be changed from menus[0].
		s_FrontOptionsMenu = Front_Menu_Update(elapsed, s_FrontOptionsMenu, nullptr);
	}

	bool32 menuClosed = true; // technically dummy init, non-assignment is unreachable
	switch (modalType) {
	case Menu_Modal_Paused:
		menuClosed = (menuSet->menus[0]->closed == TRUE || menuSet->menus[2]->closed == TRUE);
		menuSet->menus[0]->closed = false;
		menuSet->menus[2]->closed = false;
		break;

	case Menu_Modal_Options:
		menuClosed = (menuSet->menus[0]->closed == TRUE);
		menuSet->menus[0]->closed = false;
		break;
	}

	return menuClosed;
}

// levelKey is either "StartLevel" or "TutorialStartLevel".
// <LegoRR.exe @004153e0>
//bool32 __cdecl LegoRR::Front_LoadLevelSet(const Gods98::Config* config, IN OUT LevelSet* levelSet, const char* levelKey);

// Plays an alread-loaded Movie_t from the G98CMovie C wrapper API.
// Allows terminating the movie playback during runtime with isSkippable.
// <LegoRR.exe @00415630>
void __cdecl LegoRR::Front_PlayMovie(Gods98::Movie_t* mov, bool32 skippable)
{
    /// QOL APPLY: Always skippable movies/splashes (TODO: move this to optional?)
	if (Gods98::Main_IsSkippingEnabled())
		skippable = true;

    // NOTE: Speed is not supported by Movie_Update
    const real32 MOVIE_SPEED = 1.0f;

    // HARDCODED SCREEN RESOLUTION!!
	/// FIX APPLY: Don't use hardcoded resolutions.
    Rect2I destRect = { 0, 0, Gods98::appWidth(), Gods98::appHeight() };
    real32 timer = 0.0;

    while (Gods98::Movie_Update(mov, MOVIE_SPEED, &destRect)) {
        Gods98::Main_LoopUpdate(false);

        timer += MOVIE_SPEED;
        if (timer > (uint32)Gods98::Movie_GetDuration(mov))
            break;

        if (skippable) {
            if (Gods98::Input_AnyKeyPressed() || Gods98::INPUT.msrb || Gods98::INPUT.mslb)
                break;
        }
    }
}

// <LegoRR.exe @004156f0>
void __cdecl LegoRR::Front_PlayIntroSplash(const char* imageKey, bool32 skippable, const char* timeKey)
{
	/// QOL: Option to disable intro startup splashes.
	if (!Gods98::Main_IsIntrosEnabled())
		return;

    /// QOL APPLY: Always skippable movies/splashes (TODO: move this to optional?)
	if (Gods98::Main_IsSkippingEnabled())
		skippable = true;

    real32 seconds = Config_GetRealValue(Lego_Config(), Main_ID(timeKey));
    if (seconds == 0.0f)
        seconds = 3.0; // default time, in seconds

    uint32 startTime = Gods98::Main_GetTime();
    uint32 duration = (uint32)(seconds * 1000.0f);

    if (imageKey != nullptr) {
        Gods98::Image* image;
        const char* fName = Gods98::Config_GetTempStringValue(Lego_Config(), Main_ID(imageKey));

        if (fName && (image = Gods98::Image_LoadBMP(fName))) {
            while (true) {
                Gods98::Image_Display(image, nullptr);
                Gods98::Main_LoopUpdate(false);
                
                Gods98::Main_Sleep(100); // replaces `::Sleep(ms)`

                if (Gods98::Main_GetTime() >= startTime + duration)
                    break;

                if (skippable) {
                    if (Gods98::Input_AnyKeyPressed() || Gods98::INPUT.msrb || Gods98::INPUT.mslb)
                        break;
                }
            }
            Gods98::Image_Remove(image);
        }
    }
}

// <LegoRR.exe @00415840>
void __cdecl LegoRR::Front_PlayIntroMovie(const char* movieKey, bool32 skippable)
{
	/// QOL: Option to disable intro startup videos.
	if (!Gods98::Main_IsIntrosEnabled())
		return;

    if (movieKey != nullptr) {
        /// CHANGE: No need to allocate memory for the string
        const char* fName = Gods98::Config_GetTempStringValue(Lego_Config(), Main_ID(movieKey));
        //char* fName = Gods98::Config_GetStringValue(Lego_Config(), Main_ID(movieKey));

        if (fName && Gods98::File_Exists(fName)) {
            Gods98::Movie_t* mov = Gods98::Movie_Load(fName);
            Front_PlayMovie(mov, skippable);
            Gods98::Movie_Free(mov);

            /// CHANGE: No need to allocate memory for the string
            //Gods98::Mem_Free(fName);
        }
    }
}


// <LegoRR.exe @004158c0>
void __cdecl LegoRR::Front_PlayLevelMovie(const char* levelName, bool32 skippable)
{
	/// QOL: Option to disable level startup videos.
	if (!Gods98::Main_IsVideosEnabled())
		return;

	if (levelName != nullptr) {
		/// CHANGE: No need to allocate memory for the string
		const char* fName = Gods98::Config_GetTempStringValue(Lego_Config(), Lego_ID(levelName, "Video"));

		if (fName && Gods98::File_Exists(fName)) {
			Gods98::Movie_t* mov = Gods98::Movie_Load(fName);
			Front_PlayMovie(mov, skippable);
			Gods98::Movie_Free(mov);
		}
	}
}

// <LegoRR.exe @00415940>
void __cdecl LegoRR::Front_LoadLevels(MenuSet* unused_mainMenuFull)
{

	frontGlobs.startMissionLink = Front_LevelSet_LoadLevelLinks(&frontGlobs.missionLevels, frontGlobs.missionLevels.idNames[0]);
	frontGlobs.startTutorialLink = Front_LevelSet_LoadLevelLinks(&frontGlobs.tutorialLevels, frontGlobs.tutorialLevels.idNames[0]);
	Front_Levels_ResetVisited();

	sint32 menuIDLevels = Front_GetMenuIDByName(frontGlobs.mainMenuSet, "Levels");
	sint32 menuIDTuto = Front_GetMenuIDByName(frontGlobs.mainMenuSet, "Tutorials");
	sint32 menuIDLoadSave = Front_GetMenuIDByName(frontGlobs.mainMenuSet, "Load Level Save");

	// Reference for the fonts used in the save menu.
	MenuItem* itemLoadSave = frontGlobs.mainMenuSet->menus[menuIDLoadSave]->items[0];
	

	// Create menu item for Load Game menu selection.
	MenuItem_SelectData* loadSelectData;
	MenuItem* loadSelectItem;
	loadSelectData = Front_MenuItem_CreateSelect(&frontGlobs.selectLoadSaveIndex, "", "",
												 -200, -100, 60, 5, -20, 200, -20, -40,
												 0, Front_Callback_SelectLoadSave,
												 frontGlobs.mainMenuSet->menus[menuIDLevels]); // Transition to levelselect after loading save.

	loadSelectItem = Front_MenuItem_CreateBannerItem("", itemLoadSave->fontLo, itemLoadSave->fontHi,
													 0, 0, MenuItem_Type_Select,
													 true, loadSelectData, false);
	Front_Menu_AddMenuItem(frontGlobs.mainMenuSet->menus[menuIDLoadSave], loadSelectItem);


	// Create menu item for Save Game menu selection.
	MenuItem_SelectData* saveSelectData;
	MenuItem* saveSelectItem;
	saveSelectData = Front_MenuItem_CreateSelect(&frontGlobs.selectLoadSaveIndex, "", "",
												 -200, -100, 60, 5, -20, 200, -20, -40,
												 0, Front_Callback_SelectLoadSave,
												 nullptr);
	
	saveSelectItem = Front_MenuItem_CreateBannerItem("", itemLoadSave->fontLo, itemLoadSave->fontHi,
													 0, 0, MenuItem_Type_Select,
													 true, saveSelectData, false);
	Front_Menu_AddMenuItem(frontGlobs.saveMenuSet->menus[0], saveSelectItem);

	char buff[100];

	// NOTE: This DOES NOT loop through the last index [5] in the array.
	for (uint32 i = 0; i < 5; i++) {
		const char* saveGameText = Gods98::Config_GetTempStringValue(legoGlobs.config, Config_ID(legoGlobs.gameName, "Menu", "Save_Game"));
		std::sprintf(buff, "%s %i", saveGameText, (i + 1));
		Front_Util_StringReplaceChar(buff, '_', ' ');
		Front_Save_GetLevelCompleteWithPoints(&frontGlobs.saveData[i], buff);

		// Add save item for Load Game menu selection.
		Front_MenuItem_AddSelectItem(loadSelectData, buff, true,
									 frontGlobs.mainMenuSet->menus[menuIDLoadSave]->items[0]->fontHi, 0, 0, false);

		// Add save item for Save Game menu selection.
		Front_MenuItem_AddSelectItem(saveSelectData, buff, true,
									 frontGlobs.mainMenuSet->menus[menuIDLoadSave]->items[0]->fontHi, 0, 0, false);
	}


	Front_Save_LoadAllSaveFiles();

	/* 4 ->&frontGlobs.triggerCredits[4] ->&frontGlobs.selectMissionIndex */
	MainMenuFull_AddMissionsDisplay(4, frontGlobs.startMissionLink, &frontGlobs.missionLevels,
									frontGlobs.mainMenuSet->menus[menuIDLevels],
									Front_Save_GetCurrentSaveData(), nullptr,
									Front_Callback_SelectMissionItem);

	/* 5 ->&frontGlobs.triggerCredits[5] ->&frontGlobs.selectTutorialIndex */
	MainMenuFull_AddMissionsDisplay(5, frontGlobs.startTutorialLink, &frontGlobs.tutorialLevels,
									frontGlobs.mainMenuSet->menus[menuIDTuto],
									Front_Save_GetCurrentSaveData(), nullptr,
									Front_Callback_SelectTutorialItem);
}

// Only called during Lego_Initialise.
// <LegoRR.exe @00415c20>
void __cdecl LegoRR::Front_ResetSaveNumber(void)
{
	frontGlobs.saveNumber = -1;
}

// <LegoRR.exe @00415c30>
//void __cdecl LegoRR::Front_LoadMenuTextWindow(const Gods98::Config* config, const char* gameName, MenuTextWindow* menuWnd);

// <LegoRR.exe @00416080>
bool32 __cdecl LegoRR::Front_LevelSelect_PlayLevelNameSFX(sint32 levelNumber)
{
	char buff[128];
	std::sprintf(buff, "Stream_LevelName_Level%d", levelNumber);

	SFX_ID sfxType = SFX_NULL; // dummy init
	return (SFX_GetType(buff, &sfxType) && SFX_Random_SetAndPlayGlobalSample(sfxType, nullptr));
}

// <LegoRR.exe @004160d0>
bool32 __cdecl LegoRR::Front_LevelSelect_PlayTutoLevelNameSFX(sint32 levelNumber)
{
	char buff[128];
	std::sprintf(buff, "Stream_LevelName_TLevel%d", levelNumber);

	SFX_ID sfxType = SFX_NULL; // dummy init
	return (SFX_GetType(buff, &sfxType) && SFX_Random_SetAndPlayGlobalSample(sfxType, nullptr));
}

// Load MenuSets and LevelSets
// <LegoRR.exe @00416120>
//void __cdecl LegoRR::Front_Initialise(const Gods98::Config* config);

// <LegoRR.exe @00416840>
void __cdecl LegoRR::Front_SaveOptionParameters(void)
{
	SaveData* currSave = Front_Save_GetCurrentSaveData();
	if (currSave != nullptr) {
		currSave->SliderGameSpeed = frontGlobs.sliderGameSpeed;
		currSave->SliderSoundVolume = frontGlobs.sliderSFXVolume;
		currSave->SliderBrightness = frontGlobs.sliderBrightness;
	}
}

// <LegoRR.exe @00416870>
void __cdecl LegoRR::Front_LoadOptionParameters(bool32 loadOptions, bool32 resetFront)
{
	if (resetFront) {
		// Reset frontEnd triggers/selections.
		frontGlobs.triggerQuitApp = false;
		frontGlobs.selectMissionIndex = -1;
		frontGlobs.selectTutorialIndex = -1;
		frontGlobs.triggerContinueMission = false;
	}

	if (loadOptions) {
		// Reset options sliders.
		if (frontGlobs.selectLoadSaveIndex != -1 && Front_Save_GetCurrentSaveData() != nullptr) {

			SaveData* currSave = Front_Save_GetCurrentSaveData();
			frontGlobs.sliderGameSpeed = currSave->SliderGameSpeed;
			frontGlobs.sliderSFXVolume = currSave->SliderSoundVolume;
			frontGlobs.sliderBrightness = currSave->SliderBrightness;
		}
		else {
			// Set defaults
			frontGlobs.sliderGameSpeed = 1; // 100% (out of 200%)
			frontGlobs.sliderSFXVolume = 8; // 80&
			frontGlobs.sliderBrightness = 5; // 100%
		}
	}
}

// <LegoRR.exe @004168f0>
//void __cdecl LegoRR::Front_PrepareScreenMenuType(Menu_ScreenType screenType);

// <LegoRR.exe @00416bb0>
bool32 __cdecl LegoRR::Front_RunScreenMenuType(Menu_ScreenType screenType)
{

	MenuSet* menuSet = frontGlobs.mainMenuSet;
	Front_PrepareScreenMenuType(screenType);
	SFX_AddToQueue(SFX_AmbientMusicLoop, Gods98::SoundMode::Loop);

	switch (screenType) {
	case Menu_Screen_Title:
		// MainMenuFull::Menu1 "Main"
		Front_RunScreenMenu(frontGlobs.mainMenuSet, 0);
		break;
	case Menu_Screen_Missions:
		// MainMenuFull::Menu2 "Missions"
		Front_RunScreenMenu(frontGlobs.mainMenuSet, 1);
		break;
	case Menu_Screen_Training:
		// MainMenuFull::Menu3 "Training_Missions"
		Front_RunScreenMenu(frontGlobs.mainMenuSet, 2);
		break;
	case Menu_Screen_Save:
		// SaveMenu::Menu1 "Load_A_Saved_Game"  (save a game)
		Front_RunScreenMenu(frontGlobs.saveMenuSet, 0);
		break;
	}

	Gods98::Sound3D_Stream_Stop(true);
	Gods98::Sound3D_Stream_Stop(false);
	return false;
}

// <LegoRR.exe @00416c30>
bool32 __cdecl LegoRR::Front_IsFrontEndEnabled(void)
{
	bool32 frontEnd = Config_GetBoolOrFalse(legoGlobs.config, Config_ID(legoGlobs.gameName, "Main", "FrontEnd"));
	//BoolTri frontEnd = Gods98::Config_GetBoolValue(legoGlobs.config, Config_ID(legoGlobs.gameName, "Main", "FrontEnd"));

	return (frontEnd && (Gods98::Main_ProgrammerMode() < 3));
	//return ((frontEnd == BoolTri::BOOL3_TRUE) && (Gods98::Main_ProgrammerMode() < 3));
}

// <LegoRR.exe @00416c80>
bool32 __cdecl LegoRR::Front_IsMissionSelected(void)
{
	return (frontGlobs.selectMissionIndex != -1);
}

// <LegoRR.exe @00416c90>
bool32 __cdecl LegoRR::Front_IsTutorialSelected(void)
{
	return (frontGlobs.selectTutorialIndex != -1);
}

// <LegoRR.exe @00416ca0>
const char* __cdecl LegoRR::Front_GetSelectedLevel(void)
{
	if (Front_IsMissionSelected()) {
		sint32 setIndex = Front_LevelLink_FindSetIndexOf(frontGlobs.startMissionLink, frontGlobs.selectMissionIndex);
		if (setIndex != -1) {
			return frontGlobs.missionLevels.idNames[setIndex];
		}
	}
	else if (Front_IsTutorialSelected()) {
		sint32 setIndex = Front_LevelLink_FindSetIndexOf(frontGlobs.startTutorialLink, frontGlobs.selectTutorialIndex);
		if (setIndex != -1) {
			return frontGlobs.tutorialLevels.idNames[setIndex];
		}
	}
	return nullptr;
}

// <LegoRR.exe @00416d00>
bool32 __cdecl LegoRR::Front_IsTriggerAppQuit(void)
{
	return (bool)frontGlobs.triggerQuitApp;
}

// <LegoRR.exe @00416d10>
bool32 __cdecl LegoRR::Front_IsTriggerMissionQuit(void)
{
	return (bool)frontGlobs.triggerQuitMission;
}

// <LegoRR.exe @00416d20>
bool32 __cdecl LegoRR::Front_IsTriggerMissionRestart(void)
{
	return (bool)frontGlobs.triggerRestartMission;
}

// <LegoRR.exe @00416d30>
LegoRR::LevelSet* __cdecl LegoRR::Front_Levels_GetTutoOrMissions(void)
{
	return (Front_IsTutorialSelected() ? &frontGlobs.tutorialLevels : &frontGlobs.missionLevels);
}

// <LegoRR.exe @00416d50>
sint32 __cdecl LegoRR::Front_LevelSet_IndexOf(LevelSet* levelSet, const char* levelName)
{
	for (sint32 i = 0; i < levelSet->count; i++) {

		/// TODO: Case-insensitive comparison here, but case-sensitive comparisons are used by all other functions.
		if (::_stricmp(levelSet->idNames[i], levelName) == 0)
			return i;
	}
	return -1;
}

// Resets links and recurse callback states. Required after RunThroughRecurse function.
// <LegoRR.exe @00416da0>
void __cdecl LegoRR::Front_Levels_ResetVisited(void)
{
	for (sint32 i = 0; i < frontGlobs.missionLevels.count; i++) {

		if (frontGlobs.missionLevels.levels[i]) {
			frontGlobs.missionLevels.levels[i]->visited = false;
			frontGlobs.missionLevels.visitedList[i] = false;
		}
	}

	for (sint32 i = 0; i < frontGlobs.tutorialLevels.count; i++) {

		if (frontGlobs.tutorialLevels.levels[i]) {
			frontGlobs.tutorialLevels.levels[i]->visited = false;
			frontGlobs.tutorialLevels.visitedList[i] = false;
		}
	}
}

// <LegoRR.exe @00416e00>
void __cdecl LegoRR::Front_LevelSet_SetLinkVisited(LevelSet* levelSet, const char* levelName, bool32 visited)
{
	for (sint32 i = 0; i < levelSet->count; i++) {

		if (std::strcmp(levelSet->idNames[i], levelName) == 0) {
			levelSet->visitedList[i] = visited;
		}
	}
}

// <LegoRR.exe @00416e70>
bool32 __cdecl LegoRR::Front_LevelSet_IsLinkVisited(LevelSet* levelSet, const char* levelName)
{
	for (sint32 i = 0; i < levelSet->count; i++) {

		if (std::strcmp(levelSet->idNames[i], levelName) == 0) {
			return levelSet->visitedList[i];
		}
	}
	return false;
}


// <LegoRR.exe @00416ee0>
void __cdecl LegoRR::Front_LevelSet_SetLevelLink(LevelSet* levelSet, const char* levelName, LevelLink* link)
{
	for (sint32 i = 0; i < levelSet->count; i++) {

		if (std::strcmp(levelSet->idNames[i], levelName) == 0) {
			levelSet->levels[i] = link;
		}
	}
}

// <LegoRR.exe @00416f50>
LegoRR::LevelLink* __cdecl LegoRR::Front_LevelSet_GetLevelLink(LevelSet* levelSet, const char* levelName)
{
	for (sint32 i = 0; i < levelSet->count; i++) {

		if (std::strcmp(levelSet->idNames[i], levelName) == 0) {
			return levelSet->levels[i];
		}
	}
	return nullptr;
}

// <LegoRR.exe @00416fc0>
LegoRR::LevelLink* __cdecl LegoRR::Front_LevelSet_LoadLevelLinks(LevelSet* levelSet, OPTIONAL const char* levelName)
{
	if (levelName == nullptr)
		return nullptr;

	if (Front_LevelSet_IsLinkVisited(levelSet, levelName))
		return Front_LevelSet_GetLevelLink(levelSet, levelName);

	LevelLink* link = (LevelLink*)Gods98::Mem_Alloc(sizeof(LevelLink));
	Front_LevelSet_SetLinkVisited(levelSet, levelName, true);
	Front_LevelSet_SetLevelLink(levelSet, levelName, link); // Null can safely be passed to this function
	if (link == nullptr)
		return nullptr;


	sint32 setIndex = Front_LevelSet_IndexOf(levelSet, levelName);
	if (setIndex != -1) {
		link->setIndex = setIndex;

		uint32 numParts;
		char* linkNames[15]; // Maximum of 15 linked levels
		char* levelLinks = Gods98::Config_GetStringValue(legoGlobs.config, Config_ID(legoGlobs.gameName, levelName, "LevelLinks"));

		if (levelLinks == nullptr) {
			// No links defined for this level, as the property is not included.
			// (this is the only valid way to define no links)
			link->linkLevels = nullptr;
			link->linkCount = 0;
			return link;
		}
		else if ((numParts = Gods98::Util_Tokenise(levelLinks, linkNames, ",")) != 0) {
			// NOTE: 0 is only returned for an empty string, which I guess is considered invalid for this property.
			/// FIXME: Loosen CFG strictness and allow empty LevelLinks to specify no future levels.

			link->linkLevels = (LevelLink**)Gods98::Mem_Alloc(numParts * sizeof(LevelLink*));

			for (uint32 i = 0; i < numParts; i++) {
				link->linkLevels[i] = Front_LevelSet_LoadLevelLinks(levelSet, linkNames[i]);
			}
			link->linkCount = numParts;

			/// FIX APPLY: LegoRR doesn't free this string allocation.
			Gods98::Mem_Free(levelLinks);
			return link;
		}

		// Failure, empty LevelLinks property.
		/// FIX APPLY: LegoRR doesn't free this string allocation.
		Gods98::Mem_Free(levelLinks);
	}
	// Failure, either not in LevelSet, or LevelLinks property is empty string.
	Gods98::Mem_Free(link);

	return nullptr;
}

// <LegoRR.exe @004170f0>
bool32 __cdecl LegoRR::Front_LevelLink_RunThroughLinks(LevelLink* startLink, LevelLink_RunThroughLinksCallback callback, void* data)
{
	if (startLink != nullptr && !startLink->visited) {
		startLink->visited = true;

		if (callback(startLink, data)) {
			return true;
		}

		// linkLevels is null if there are no links.
		if (startLink->linkLevels) {
			for (uint32 i = 0; i < startLink->linkCount; i++) {

				if (Front_LevelLink_RunThroughLinks(startLink->linkLevels[i], callback, data)) {
					return true;
				}
			}
		}
	}
	return false;
}

// <LegoRR.exe @00417170>
bool32 __cdecl LegoRR::Front_LevelLink_Callback_IncCount(LevelLink* link, void* pCount)
{
	(*(sint32*)pCount)++;
	return false;
}

// <LegoRR.exe @00417180>
bool32 __cdecl LegoRR::Front_LevelLink_Callback_FindByLinkIndex(LevelLink* link, void* data)
{
	SearchLevelIdentifier_10* search = (SearchLevelIdentifier_10*)data;
	sint32 index = search->currentIndex;
	if (search->searchIndex == index) {
		search->resultIndex = index;
		search->resultLink = link;
		return true;
	}

	search->currentIndex = index + 1;
	return false;
}

// <LegoRR.exe @004171b0>
LegoRR::LevelLink* __cdecl LegoRR::Front_LevelLink_FindByLinkIndex(LevelLink* startLink, sint32 linkIndex)
{
	SearchLevelIdentifier_10 search = { 0 };
	search.resultLink = nullptr;
	search.currentIndex = 0;
	search.resultIndex = 0;
	search.searchIndex = linkIndex;

	if (Front_LevelLink_RunThroughLinks(startLink, Front_LevelLink_Callback_FindByLinkIndex, &search)) {
		Front_Levels_ResetVisited();
		return search.resultLink;
	}
	else {
		Front_Levels_ResetVisited();
		return nullptr;
	}
}

// <LegoRR.exe @00417200>
sint32 __cdecl LegoRR::Front_LevelLink_FindSetIndexOf(LevelLink* startLink, sint32 linkIndex)
{
	LevelLink* link = Front_LevelLink_FindByLinkIndex(startLink, linkIndex);

	return (link ? link->setIndex : -1);
}

// <LegoRR.exe @00417220>
void __cdecl LegoRR::Front_Levels_UpdateAvailable_Recursive(LevelLink* link, SearchLevelSelectInfo_14* search, bool32 unlocked)
{
	if (link && !link->visited) {
		link->visited = true;

		// ProgrammerMode >=2 has the same effect as `-testlevels`.
		bool32 debugUnlockAll = ((Gods98::Main_ProgrammerMode() >= 2) || Gods98::Main_IsLevelsOpen());
	
		// `unlocked`  - this level is available, but future levels are not affected.
		// `completed` - this level and all its links are available (no relation to actually being completed).
		bool32 completed;
		if (debugUnlockAll) {
			/// REMOVED: Field `keepLocked` had no extra effect since the above condition always forces unlocking.
			///          In the original code, the above condition was was checked again with `!search->saveReward`
			///          to assign `completed = !search->keepLocked;`
			search->keepLocked = false;

			completed = true;
		}
		else if (!search->saveReward) { // When saveReward is not specified, `keepLocked` states how to lock levels
			completed = !search->keepLocked;
		}
		else {
			// Even if a level is not unlocked/available, completing it will force it to become available.
			// Note this uses `link->index`, not `search->index`.
			completed = (search->saveReward[link->setIndex].flags & SAVEREWARD_FLAG_COMPLETED);
		}

		// Note FrontEndOpen does not count as "completed", so that linked levels
		// are not unlocked automatically just from this level existing.
		MenuItem_SelectItem* selItem = &search->selectData->selItemList[search->index];
		if (completed || unlocked || selItem->frontEndOpen) {
			selItem->flags |= SELECTITEM_FLAG_ENABLED; // unlocked
		}
		else {
			selItem->flags &= ~SELECTITEM_FLAG_ENABLED; // locked
		}

		// Increment index before recursive calls to this function.
		search->index++;

		for (uint32 i = 0; i < link->linkCount; i++) {
			// When a level is completed, all its links are unlocked.
			Front_Levels_UpdateAvailable_Recursive(link->linkLevels[i], search, completed);
		}
	}
}

// <LegoRR.exe @00417310>
void __cdecl LegoRR::Front_Levels_UpdateAvailable(LevelLink* startLink, OPTIONAL SaveReward* saveReward,
															 LevelSet* levelSet, MenuItem_SelectData* selectData, bool32 keepLocked)
{
	SearchLevelSelectInfo_14 search;

	search.levelSet = levelSet;
	search.selectData = selectData;
	search.saveReward = saveReward;
	search.index = 0;
	search.keepLocked = keepLocked;
	Front_Levels_UpdateAvailable_Recursive(startLink, &search, true);
	Front_Levels_ResetVisited();
}

// <LegoRR.exe @00417360>
sint32 __cdecl LegoRR::Front_Save_GetLevelScore(uint32 index, const SaveData* saveData)
{
	return (sint32)saveData->missionsTable[index].reward.items[Reward_Score].percentFloat;
}

// <LegoRR.exe @00417390>
void __cdecl LegoRR::Front_Callback_SelectMissionItem(real32 elapsedAbs, sint32 selectIndex)
{
	char buffLevel[64]; // Used to display level number (1-indexed) with `-testercall` cmd option.
	char buffMsg[300];

	MenuTextWindow* menuTextWnd = frontGlobs.saveLevelWnd;
	Gods98::Font* font = (frontGlobs.mainMenuSet)->menus[1]->menuFont;
	LevelLink* link = Front_LevelLink_FindByLinkIndex(frontGlobs.startMissionLink, selectIndex);
	const char* langName = frontGlobs.missionLevels.langNames[link->setIndex];

	std::memset(buffLevel, 0, sizeof(buffLevel));

	SaveData* currSave = Front_Save_GetCurrentSaveData();
	frontGlobs.levelSelectLastNumber = frontGlobs.levelSelectHoverNumber;
	frontGlobs.levelSelectHoverNumber = link->setIndex + 1; // 1-indexed

	if (Gods98::Main_IsTesterCall()) {
		std::sprintf(buffLevel, " (Level %d)", (sint32)(link->setIndex + 1));
	}

	// Fallback to ID name when language name (cfg: FullName) isn't defined.
	if (langName == nullptr || *langName == '\0') {
		langName = frontGlobs.missionLevels.idNames[link->setIndex];
	}

	if (menuTextWnd->textWindow == nullptr) {
		uint32 strWidth = Gods98::Font_GetStringWidth(font, langName);
		sint32 nameX = ((sint32)Gods98::appWidth() / 2) - ((sint32)strWidth / 2);
		// HARDCODED SCREEN RESOLUTION!! (390)
		sint32 nameY = 390;

		Front_LevelSelect_LevelNamePrintF(font, nameX, nameY, langName);
	}
	else if (g_FrontBool_004dc8c4) {
		TextWindow_PrintF(menuTextWnd->textWindow, langName);
		
		if (Gods98::Main_IsTesterCall()) {
			TextWindow_PrintF(menuTextWnd->textWindow, buffLevel);
		}
	}

	if (currSave != nullptr &&
		(currSave->missionsTable[link->setIndex].flags & SAVEREWARD_FLAG_COMPLETED) &&
		!(currSave->missionsTable[link->setIndex].flags & SAVEREWARD_FLAG_TUTORIAL))
	{
		sint32 score = Front_Save_GetLevelScore(link->setIndex, currSave);
		const char* completeText = Gods98::Config_GetTempStringValue(legoGlobs.config, Config_ID(legoGlobs.gameName, "Menu", "Level_Completed"));

		std::sprintf(buffMsg, " %s (%i)", completeText, score);
		Front_Util_StringReplaceChar(buffMsg, '_', ' ');
	}
	else {
		const char* incompleteText = Gods98::Config_GetTempStringValue(legoGlobs.config, Config_ID(legoGlobs.gameName, "Menu", "Level_Incomplete"));

		std::sprintf(buffMsg, " %s", incompleteText);
		Front_Util_StringReplaceChar(buffMsg, '_', ' ');
	}

	if (menuTextWnd->textWindow != nullptr && g_FrontBool_004dc8c4) {
		/* Ahhh, lovely... nested printf calls without sanitization,
			it may be possible to intentionally corrupt memory with this. */
		TextWindow_PrintF(menuTextWnd->textWindow, buffMsg);
		if (frontGlobs.levelSelectHoverNumber != frontGlobs.levelSelectLastNumber) {
			frontGlobs.levelSelectSFXStopped = true;
			frontGlobs.levelSelectSFXTimer = 0.0f;
		}

		if (frontGlobs.levelSelectSFXStopped) {
			frontGlobs.levelSelectSFXTimer += elapsedAbs;

			if ((frontGlobs.levelSelectSFXTimer * STANDARD_FRAMERATE) > 500.0f) {
				bool32 soundResult;

				// Uhhh, well then...
				// tutorial levels are kind of hardcoded to max of 9...???
				if ((sint32)frontGlobs.levelSelectHoverNumber <= 8) {
					soundResult = Front_LevelSelect_PlayTutoLevelNameSFX(frontGlobs.levelSelectHoverNumber);
				}
				else {
					/* levelSelectHoverNumber - NUM_TUTORIALS */
					soundResult = Front_LevelSelect_PlayLevelNameSFX(frontGlobs.levelSelectHoverNumber - 8);
				}

				if (soundResult) {
					frontGlobs.levelSelectSFXStopped = false;
				}
			}
		}
	}

	g_FrontBool_004dc8c4 = false;
}

// <LegoRR.exe @00417630>
//void __cdecl LegoRR::Front_Callback_SelectTutorialItem(real32 elapsedAbs, sint32 selectIndex);

// <LegoRR.exe @00417710>
bool32 __cdecl LegoRR::Front_LevelInfo_Callback_AddItem(LevelLink* link, void* data)
{
	SearchLevelSelectAdd* search = (SearchLevelSelectAdd*)data;

	const char* levelName = search->levelSet->idNames[link->setIndex];

	sint32 frontEndX = Config_GetIntValue(legoGlobs.config, Config_ID(legoGlobs.gameName, levelName, "FrontEndX"));
	sint32 frontEndY = Config_GetIntValue(legoGlobs.config, Config_ID(legoGlobs.gameName, levelName, "FrontEndY"));
	bool32 frontEndOpen = Config_GetBoolOrFalse(legoGlobs.config, Config_ID(legoGlobs.gameName, levelName, "FrontEndOpen"));


	/// CUSTOM: Playing around to show fonts for level select
	/*{
		Gods98::Font* font = Front_Cache_LoadFont("Interface\\Fonts\\MBriefFONT2.bmp");
		const char* langName = search->levelSet->langNames[link->setIndex];
		if (!langName) langName = levelName;
		char* banner = Gods98::Util_StrCpy(langName);
		Front_MenuItem_AddSelectItem(search->itemData, banner, false, nullptr, frontEndX, frontEndY, frontEndOpen);
		Gods98::Mem_Free(banner);
		return false;
	}*/



	char* menuBMP = Gods98::Config_GetStringValue(legoGlobs.config, Config_ID(legoGlobs.gameName, levelName, "MenuBMP"));

	if (menuBMP != nullptr) {
		Front_MenuItem_AddSelectItem(search->itemData, menuBMP, false, nullptr, frontEndX, frontEndY, frontEndOpen);
		Gods98::Mem_Free(menuBMP);
	}
	else {
		char buff[1024];
		std::strcpy(buff, frontGlobs.strDefaultLevelBMPS);
		Front_MenuItem_AddSelectItem(search->itemData, buff, false, nullptr, frontEndX, frontEndY, frontEndOpen);
	}
	return false;
}


// <LegoRR.exe @004178e0>
//void __cdecl LegoRR::MainMenuFull_AddMissionsDisplay(sint32 valueOffset, LevelLink* startLink, LevelSet* levelSet, Menu* menu,
//													 SaveData* saveData, OPTIONAL Menu* menu58, void* callback);

// <LegoRR.exe @004179c0>
//bool32 __cdecl LegoRR::Front_Save_ReadSaveFile(uint32 saveIndex, OUT SaveData* saveData, bool32 readOnly);

// <LegoRR.exe @00417b00>
//bool32 __cdecl LegoRR::Front_Save_WriteSaveFiles(uint32 saveNumber, OPTIONAL const SaveData* saveData);

// <LegoRR.exe @00417d20>
//void __cdecl LegoRR::Front_Save_LoadAllSaveFiles(void);

// <LegoRR.exe @00417d80>
LegoRR::SaveData* __cdecl LegoRR::Front_Save_GetSaveDataAt(sint32 saveIndex)
{
	if (saveIndex >= 0 && saveIndex < 6) {
		return &frontGlobs.saveData[saveIndex];
	}
	return nullptr;
}

// <LegoRR.exe @00417da0>
LegoRR::SaveData* __cdecl LegoRR::Front_Save_GetCurrentSaveData(void)
{
	if (frontGlobs.saveNumber != -1) {
		return Front_Save_GetSaveDataAt(frontGlobs.saveNumber);
	}
	return nullptr;
}

// <LegoRR.exe @00417dc0>
sint32 __cdecl LegoRR::Front_Save_GetSaveNumber(void)
{
	return frontGlobs.saveNumber;
}

// <LegoRR.exe @00417dd0>
void __cdecl LegoRR::Front_Save_SetSaveNumber(sint32 saveNumber)
{
	frontGlobs.saveNumber = saveNumber;
}

// <LegoRR.exe @00417de0>
//void __cdecl LegoRR::Front_Save_SetLevelCompleted(uint32 levelIndex);

// <LegoRR.exe @00417e50>
//void __cdecl LegoRR::Front_Save_SetSaveStruct18(const SaveStruct_18* savestruct18);

// <LegoRR.exe @00417e70>
//bool32 __cdecl LegoRR::Front_Save_SetRewardLevel(sint32 levelIndex, const RewardLevel* rewardLevel);

// <LegoRR.exe @00417ec0>
LegoRR::RewardLevel* __cdecl LegoRR::Front_Save_GetRewardLevel(sint32 levelIndex)
{
	SaveData* currSave = Front_Save_GetCurrentSaveData();
	if (currSave != nullptr && levelIndex < (sint32)currSave->missionsCount) {
		return &currSave->missionsTable[levelIndex].reward;
	}
	return nullptr;
}

// <LegoRR.exe @00417ef0>
//bool32 __cdecl LegoRR::Front_Save_WriteCurrentSaveFiles(void);

// <LegoRR.exe @00417f10>
bool32 __cdecl LegoRR::Front_Save_GetBool_540(void)
{
	return frontGlobs.saveBool_540;
}

// <LegoRR.exe @00417f20>
void __cdecl LegoRR::Front_Save_SetBool_540(bool32 state)
{
	frontGlobs.saveBool_540 = state;
}

// <LegoRR.exe @00417f30>
//void __cdecl LegoRR::Front_Save_Write_FUN_00417f30(void);

// <LegoRR.exe @00417f70>
//void __cdecl LegoRR::Front_Save_CopySaveData(OUT SaveData* saveData);

// <LegoRR.exe @00417ff0>
//void __cdecl LegoRR::Front_Save_SetSaveData(const SaveData* saveData);

// <LegoRR.exe @00418040>
void __cdecl LegoRR::Front_Save_SetBool_85c(bool32 state)
{
	frontGlobs.saveBool_85c = state;
}

// <LegoRR.exe @00418050>
//bool32 __cdecl LegoRR::Front_Save_FUN_00418050(void);

#pragma endregion
