// Priorities.cpp : 
//

#include "Priorities.h"
#include "..\..\game\audio\SFX.h"
#include "..\..\engine\core\Utils.h"
#include "..\..\engine\input\Input.h"
#include "..\..\game\object\AITask.h"
#include "..\..\game\interface\Panels.h"
#include "..\..\game\interface\ToolTip.h"
#include "..\..\game\interface\TextMessages.h"
#include "..\..\game\interface\Interface.h"
#include "..\..\game\Game.h"



/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals


// <LegoRR.exe @004a9ee8>
uint32 & LegoRR::s_PriorityList_Index = *(uint32*)0x004a9ee8;

// <LegoRR.exe @004a9eec>
LegoRR::AI_Priority (& LegoRR::s_PriorityList_Type) = *(LegoRR::AI_Priority*)0x004a9eec;

// <LegoRR.exe @00501f00>
LegoRR::Priorities_Globs & LegoRR::prioritiesGlobs = *(LegoRR::Priorities_Globs*)0x00501f00;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0045d080>
/* <PriorityType> <LangName>[|LangSFX]:<NormalImage>:<Pressedmage>:<OffImage> */
void __cdecl LegoRR::Priorities_LoadImages(const Gods98::Config* config, const char* gameName)
{
	AI_Priority priorityType;
	char* stringParts[10];
	char* stringPartsSub[10];
	char buff[512];
	
	char* arrayID = (char*)Config_ID(gameName, "PriorityImages", 0);
	for (Gods98::Config* prop = (Gods98::Config*)Config_FindArray(config, arrayID); prop != nullptr;  prop = (Gods98::Config*)Config_GetNextItem(prop)) {
		if (AIPriority_GetType(prop->itemName, &priorityType)) {
			// -FC, grab strings and assign images for press, unpress, and disabled
			std::sprintf(buff, "%s", prop->dataString);
			Gods98::Util_Tokenise(buff, stringParts, ":");
			int numSubParts = Gods98::Util_Tokenise(stringParts[0], stringPartsSub, "|");
			AI_Priority priorityTypeLoc = priorityType;
			prioritiesGlobs.langPriorityName[priorityTypeLoc] = Gods98::Util_StrCpy(stringPartsSub[0]);

			if (numSubParts == 2) {
				SFX_GetType(stringPartsSub[1], prioritiesGlobs.sfxPriorityName + priorityTypeLoc);
			}

			prioritiesGlobs.priorityImage[priorityType] =      Gods98::Image_LoadBMPScaled(stringParts[1], 0, 0);
			prioritiesGlobs.priorityPressImage[priorityType] = Gods98::Image_LoadBMPScaled(stringParts[2], 0, 0);
			prioritiesGlobs.priorityOffImage[priorityType] =   Gods98::Image_LoadBMPScaled(stringParts[3], 0, 0);
		}
	}
}

// <LegoRR.exe @0045d1c0>
void __cdecl LegoRR::Priorities_Reset(void)
{
	for (uint32 i = 0; i < prioritiesGlobs.count; i++) {
		prioritiesGlobs.buttonTypes[i] = prioritiesGlobs.initialTypes[i];
		AITask_Game_SetPriorityValue(prioritiesGlobs.buttonTypes[i], prioritiesGlobs.initialValues[i]);
		AITask_Game_SetPriorityOff(prioritiesGlobs.buttonTypes[i], prioritiesGlobs.initialOff[i]);
	}
}

// <LegoRR.exe @0045d210>
bool32 __cdecl LegoRR::Priorities_LoadLevel(const Gods98::Config* config, const char* gameName, const char* levelName)
{
	char* arrayID;
	Gods98::Config* prop;
	prioritiesGlobs.count = 0;
	arrayID = (char*)Config_ID(gameName, levelName, "Priorities", 0);
	// -FC, create this instead of reusing levelName pointer 10 times
	AI_Priority * locAiPro = (AI_Priority*)&levelName;
	for (prop = (Gods98::Config*)Config_FindArray(config, arrayID); prop != nullptr; prop = (Gods98::Config*)Config_GetNextItem(prop)) {
		if (AIPriority_GetType(prop->itemName, locAiPro)) {
			prioritiesGlobs.buttonTypes[prioritiesGlobs.count] = *locAiPro;
			prioritiesGlobs.initialTypes[prioritiesGlobs.count] = *locAiPro;
			// -FC, prios start at 80 and dec by 5 each
			prioritiesGlobs.initialValues[prioritiesGlobs.count] =	(prioritiesGlobs.count * -PRIO_INC) + MAX_PRIO;
			AITask_Game_SetPriorityValue(*locAiPro,prioritiesGlobs.initialValues[prioritiesGlobs.count]);
			if (Gods98::Util_GetBoolFromString(prop->dataString) == BOOL3_TRUE) {
				/* Priority needs to be explicitly defined as ON in Lego.cfg */
				AITask_Game_SetPriorityOff(*locAiPro, FALSE);
				prioritiesGlobs.initialOff[prioritiesGlobs.count] = FALSE;
			}
			else {
				AITask_Game_SetPriorityOff(*locAiPro, TRUE);
				prioritiesGlobs.initialOff[prioritiesGlobs.count] = TRUE;
			}
			prioritiesGlobs.count++;
		}
	}
	return TRUE;
}
// <LegoRR.exe @0045d320>
void __cdecl LegoRR::Priorities_LoadPositions(const Gods98::Config* config, const char* gameName)
{
	char* keyPath;
	char* tempPositionValue;
	int orderIndex;
	char* parts[16];
	char* startGameName = (char*)gameName;
	orderIndex = 0;

	for(uint32 i = 0; i < AI_Priority_Count; i++){
		orderIndex++;
		_itoa(orderIndex, (char*)&gameName, 10);
		keyPath = (char*)Config_ID(startGameName, "PrioritiesImagePositions", &gameName, 0);
		tempPositionValue = (char*)Config_GetTempStringValue(config, keyPath);
		if (tempPositionValue != nullptr) {
			/* NO! BAD BAD BAD! Util_Tokenise changes the passed input value!! */
			Gods98::Util_Tokenise(tempPositionValue, parts, ",");
			prioritiesGlobs.buttonPoints[i].x = (float)std::atoi(parts[0]);
			prioritiesGlobs.buttonPoints[i].y = std::atoi(parts[1]);
		}
	}
}

// <LegoRR.exe @0045d3d0>
bool32 __cdecl LegoRR::Priorities_HandleInput(uint32 mouseX, uint32 mouseY, bool32 leftButton, bool32 leftButtonLast, bool32 leftReleased)
{
	Point2F hoverPt;
	float x3;
	float y3;
	uint32 hoverHeight;
	uint32 hoverWidth;
	Gods98::Image* image;
	// -FC, if panel is closed and not sliding we can activate it
	if (Panel_IsFullyClosed(Panel_PriorityList) == 0 && Panel_IsSliding(Panel_PriorityList) == 0) {
		Panel_GetPosition(Panel_PriorityList, &x3, &y3);
		for(uint32 i = 0; i < prioritiesGlobs.count; i++){
			image = prioritiesGlobs.priorityImage[prioritiesGlobs.buttonTypes[i]];
			if (image != nullptr) {
				hoverPt.x = prioritiesGlobs.buttonPoints[i].x + x3;
				hoverPt.y = y3 + prioritiesGlobs.buttonPoints[i].y;
				hoverHeight = image->height;
				hoverWidth = image->width;
				/* if (mouseX >= hoverPt.x && mouseX < (hoverPt.x + hoverHeight)) */
				if (hoverPt.x <= mouseX &&	mouseX <= hoverWidth + hoverPt.x) {
					/* if (mouseY >= hoverPt.y && mouseY < (hoverPt.y + hoverHeight)) */
					if (hoverPt.y <= mouseY && mouseY <= hoverHeight + hoverPt.y) {
						prioritiesGlobs.flags |= PRIORITIES_GLOB_FLAG_HOVER;
						Priorities_SetHoverArea(&hoverPt, hoverWidth - 1, hoverHeight - 1);
						if (leftButton == 0) {
							if (s_PriorityList_Index == i && s_PriorityList_Type == prioritiesGlobs.buttonTypes[i]) {
								ToolTip_SetContent(ToolTip_Priority, prioritiesGlobs.langPriorityName[prioritiesGlobs.buttonTypes[i]]);
								ToolTip_Activate(ToolTip_Priority);
								Interface_SetSFX_004df1f4(prioritiesGlobs.sfxPriorityName[prioritiesGlobs.buttonTypes[i]]);
							}
							else {
								ToolTip_Activate(ToolTip_Null);
								s_PriorityList_Index = i;
								s_PriorityList_Type = prioritiesGlobs.buttonTypes[i];
							}
						}
						if (Priorities_ChangePriorityPressed(i, leftButton, leftButtonLast) && leftReleased) {
							Priorities_MovePriorityUpOrTop(i);
						}
						return TRUE;
					}
				}
			}
		}
		// -FC, indicate that the prioties have changed
		Priorities_ChangePriorityPressed(AI_Priority_Count, leftButton, leftButtonLast);
	}
	return FALSE;
}

// <LegoRR.exe @0045d5c0>
bool32 __cdecl LegoRR::Priorities_ChangePriorityPressed(uint32 orderIndex, bool32 leftButton, bool32 leftButtonLast)
{
	uint32 origPressIndex = prioritiesGlobs.pressIndex;
	if (leftButton == 0) {
		prioritiesGlobs.pressIndex = AI_Priority_Count;
	}
	else if (prioritiesGlobs.pressIndex == AI_Priority_Count) {
		if (leftButtonLast == 0) {
			prioritiesGlobs.pressIndex = orderIndex;
			prioritiesGlobs.flags |= PRIORITIES_GLOB_FLAG_PRESSED;
			return orderIndex == AI_Priority_Count;
		}
	}
	else if (prioritiesGlobs.pressIndex == orderIndex) {
		prioritiesGlobs.flags |= PRIORITIES_GLOB_FLAG_PRESSED;
		return prioritiesGlobs.pressIndex == orderIndex;
	}
	return origPressIndex == orderIndex;
}

// <LegoRR.exe @0045d630>
/* Sets the area used to draw a green rectangle around the current moused-over priority.
   Hover is only drawn when PRIORITIES_GLOB_FLAG_HOVER flag is set. */
void __cdecl LegoRR::Priorities_SetHoverArea(const Point2F* point, uint32 widthM1, uint32 heightM1)
{
	prioritiesGlobs.hoverArea.x = point->x;
	prioritiesGlobs.hoverArea.y = point->y;
	prioritiesGlobs.hoverArea.width = widthM1;
	prioritiesGlobs.hoverArea.height = heightM1;
}

// <LegoRR.exe @0045d680>
void __cdecl LegoRR::Priorities_MovePriorityUpOrTop(uint32 orderIndex)
{
	if (panelGlobs.flags & PANEL_GLOB_FLAG_PRIORITIES_TOP) {
		Priorities_MovePriorityTop(orderIndex);
		return;
	}
	if (panelGlobs.flags & PANEL_GLOB_FLAG_PRIORITIES_UP) {
		Priorities_MovePriorityUp(orderIndex);
	}
}

// <LegoRR.exe @0045d6b0>
void __cdecl LegoRR::Priorities_MovePriorityTop(uint32 orderIndex)
{
	AI_Priority priorityType;
	uint32 index;
	int priorityValue;
	AI_Priority currPriorityType;

	if ( orderIndex && orderIndex < prioritiesGlobs.count) {
		currPriorityType = prioritiesGlobs.buttonTypes[orderIndex];
		priorityValue = MAX_PRIO;
		for(index = orderIndex; index > 0; index--){
				/* globs::prioritiesGlobs.priorityTypeTable_1[index - 1] */
				//priorityType = (AI_Priority)prioritiesGlobs.sfxPriorityName[index + 0x1a];
				priorityType = (AI_Priority)prioritiesGlobs.buttonTypes[index-1];
				prioritiesGlobs.buttonTypes[index] = priorityType;
				AITask_Game_SetPriorityValue(priorityType, priorityValue + (orderIndex * -PRIO_INC));
				priorityValue = priorityValue + PRIO_INC;
		}
		prioritiesGlobs.buttonTypes[0] = currPriorityType;
		AITask_Game_SetPriorityValue(currPriorityType, MAX_PRIO);
	}
	Lego_SetPointerSFX(PointerSFX_Okay_TopPriority);
}

// <LegoRR.exe @0045d730>
void __cdecl LegoRR::Priorities_MovePriorityUp(uint32 orderIndex)
{
	AI_Priority currPriorityType;
	AI_Priority prevPriorityType;
	if (orderIndex && orderIndex < prioritiesGlobs.count) {
		currPriorityType = prioritiesGlobs.buttonTypes[orderIndex];
		prevPriorityType = prioritiesGlobs.buttonTypes[orderIndex-1];
		/* prioritiesGlobs.priorityTypeTable_1[orderIndex - 1] */
		// -FC, swap prev and current priority
		prioritiesGlobs.buttonTypes[orderIndex] = prevPriorityType;
		prioritiesGlobs.buttonTypes[orderIndex-1] = currPriorityType;
		AITask_Game_SetPriorityValue (prioritiesGlobs.buttonTypes[orderIndex], (orderIndex * -PRIO_INC) + MAX_PRIO);
		AITask_Game_SetPriorityValue((AI_Priority)currPriorityType, (orderIndex * -PRIO_INC) + (MAX_PRIO+PRIO_INC));
		Priorities_MoveCursorToPriorityUpButton(orderIndex - 1);
	}
	Lego_SetPointerSFX(PointerSFX_Okay);
}

// <LegoRR.exe @0045d7b0>
void __cdecl LegoRR::Priorities_TurnPriorityOff(uint32 orderIndex)
{
	BOOL isOff;

	if (orderIndex < prioritiesGlobs.count) {
		isOff = AITask_Game_IsPriorityOff(prioritiesGlobs.buttonTypes[orderIndex]);
		AITask_Game_SetPriorityOff(prioritiesGlobs.buttonTypes[orderIndex], isOff == 0);
		/* Hardcoded handling to warn player about turning off things they don't know
		   anything about.

		   if (isOff && priorityType == AI_Priority_Crystal || priorityType ==
		   AI_Priority_Ore) */
		// -FC, default is to tell the user this when its off but in future maybe better to tell user before they turn off
		if (isOff && (prioritiesGlobs.buttonTypes[orderIndex] == AI_Priority_Crystal || prioritiesGlobs.buttonTypes[orderIndex] == AI_Priority_Ore )) {
			Text_DisplayMessage(Text_CrystalOreDisabled, TRUE, FALSE);
		}
	}
}

// <LegoRR.exe @0045d810>
void __cdecl LegoRR::Priorities_MoveCursorToPriorityUpButton(uint32 orderIndex)
{
	PanelButton_Type buttonType;
	float x3;
	float y3;
	Area2F buttonArea;

	switch (orderIndex) {
		case 0:
			/* There isn't any "Move Up" button for the highest priority. */
			Priorities_MoveCursorToPriorityImage(0);
			return;
		case 1:
			buttonType = PanelButton_PriorityList_UpOne1;
			break;
		case 2:
			buttonType = PanelButton_PriorityList_UpOne2;
			break;
		case 3:
			buttonType = PanelButton_PriorityList_UpOne3;
			break;
		case 4:
			buttonType = PanelButton_PriorityList_UpOne4;
			break;
		case 5:
			buttonType = PanelButton_PriorityList_UpOne5;
			break;
		case 6:
			buttonType = PanelButton_PriorityList_UpOne6;
			break;
		case 7:
			buttonType = PanelButton_PriorityList_UpOne7;
			break;
		case 8:
			buttonType = PanelButton_PriorityList_UpOne8;
			break;
		default:
			return;
	}
	// -FC, grab the button area and force the cursor to the next button img
	Panel_GetPosition(Panel_PriorityList, &x3, &y3);
	Panel_Button_GetArea(Panel_PriorityList, buttonType, &buttonArea);
	Gods98::Input_SetCursorPos(buttonArea.width * 0.5f + buttonArea.x + x3, buttonArea.height * 0.5f + buttonArea.y + y3);
}

// <LegoRR.exe @0045d900>
void __cdecl LegoRR::Priorities_MoveCursorToPriorityImage(uint32 orderIndex)
{
	int y;
	int x;
	float x3;
	float y3;
	uint32 centerSize;
	Gods98::Image* priorityImage;
	AI_Priority priorityType;

	priorityType = prioritiesGlobs.buttonTypes[orderIndex];
	if (prioritiesGlobs.priorityImage[priorityType] != nullptr) {
		Panel_GetPosition(Panel_PriorityList, &x3, &y3);
		priorityImage = prioritiesGlobs.priorityImage[priorityType];
		centerSize = priorityImage->height >> 1;
		y = prioritiesGlobs.buttonPoints[orderIndex].y + y3 + (float)centerSize;
		centerSize = priorityImage->width >> 1;
		x = prioritiesGlobs.buttonPoints[orderIndex].x + x3 +(float)centerSize;
		Gods98::Input_SetCursorPos(x, y);
	}
}

// <LegoRR.exe @0045d990>
void __cdecl LegoRR::Priorities_Draw(void)
{
	Gods98::Image* image;
	Point2F destPos;
	float x3;
	float y3;
	AI_Priority priorityType;

	if (Panel_IsFullyClosed(Panel_PriorityList) == 0) {
		Panel_GetPosition(Panel_PriorityList, &x3, &y3);
		for(uint32 i = 0; i < prioritiesGlobs.count; i++){
				destPos.x = prioritiesGlobs.buttonPoints[i].x + x3;
				destPos.y = y3 + prioritiesGlobs.buttonPoints[i].y;
				priorityType = prioritiesGlobs.buttonTypes[i];
				if (AITask_Game_IsPriorityOff(priorityType) == 0) {
					if (((prioritiesGlobs.flags & PRIORITIES_GLOB_FLAG_PRESSED) ==  PRIORITIES_GLOB_FLAG_NONE) || prioritiesGlobs.pressIndex != i) {
						image = prioritiesGlobs.priorityImage[priorityType];
					}
					else {
						image = prioritiesGlobs.priorityPressImage[priorityType];
					}
				}
				else {
					image = prioritiesGlobs.priorityOffImage[priorityType];
				}
				if (image != nullptr) {
					Image_DisplayScaled(image, nullptr, &destPos, nullptr);
				}
			}
		/* Hover and pressed flags are handled/assigned every tick, rather than updated
		   as needed. */
		if (prioritiesGlobs.flags & PRIORITIES_GLOB_FLAG_HOVER) {
			Interface_DrawHoverOutline(&prioritiesGlobs.hoverArea);
			prioritiesGlobs.flags &= ~PRIORITIES_GLOB_FLAG_HOVER;
		}
		prioritiesGlobs.flags &= ~PRIORITIES_GLOB_FLAG_PRESSED;
	}
}

#pragma endregion
