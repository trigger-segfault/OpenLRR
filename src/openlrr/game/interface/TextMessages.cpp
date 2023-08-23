// TextMessages.cpp : 
//

#include "string.h"
#include "TextMessages.h"
#include "..\..\engine\drawing\TextWindow.h"
#include "..\..\game\Game.h"
#include "..\..\game\audio\SFX.h"
#include "..\..\game\interface\Interface.h"
#include "..\..\game\interface\Panels.h"
#include "..\mission\NERPsFile.h"
#include "..\mission\NERPsFunctions.h"




/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00504190>
LegoRR::Text_Globs & LegoRR::textGlobs = *(LegoRR::Text_Globs*)0x00504190;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0046aab0>
void __cdecl LegoRR::Text_Load(Gods98::TextWindow* textOnlyWindow, Gods98::TextWindow* textImageWindow, sint32 imageX, sint32 imageY, real32 pauseTime)
{
	// -FC, copy params to global object
	textGlobs.textOnlyWindow = textOnlyWindow;
	textGlobs.textImageWindow = textImageWindow;
	textGlobs.TextImagePosition.x = imageX;
	textGlobs.TextImagePosition.y = imageY;
	textGlobs.textCount = Text_Type_Count;
	textGlobs.currType = Text_Type_Count;
	// -FC, now just copy over the strings
	const char *TextLocs[] = {
		"Text_Walk",
		"Text_Drill",
		"Text_NoDrill",
		"Text_CantDrill",
		"Text_Reinforce",
		"Text_NoReinforce",
		"Text_CantReinforce",
		"Text_PickupCrystal",
		"Text_Encyclopedia",
		"Text_RadarHelp",
		"Text_BuildHelp",
		"Text_SelectionHelp",
		"Text_Drive",
		"Text_CantDrive",
		"Text_MakeTeleporter",
		"Text_CannotPlaceBuilding",
		"Text_CrystalOreDisabled",
		"Text_CrystalFound",
		"Text_CavernDiscovered",
		"Text_OreFound",
		"Text_AirSupplyLow",
		"Text_AirSupplyRunningOut",
		"Text_SpaceToContinue",
		"Text_GameCompleted",
		"Text_ManTrained",
		"Text_UnitUpgraded",
	};
	std::copy(std::begin(TextLocs), std::end(TextLocs), std::begin(textGlobs.textName));

	// -FC, catch if pause time is 0 and reset back to 100
	if (pauseTime == 0.0) {
		textGlobs.TextPauseTime = 100.0f;
		return;
	}
	textGlobs.TextPauseTime = pauseTime;
}


// <LegoRR.exe @0046ac10>
void __cdecl LegoRR::Text_Initialise(const char* filename, uint32 x, uint32 y, uint32 unused_int, const Area2F* msgRect, const Area2F* unused_rect, real32 param_7)
{
	textGlobs.msgYPos = y;
	textGlobs.MsgPanel_Rect1 = *msgRect;
	textGlobs.winLowPos =	(textGlobs.MsgPanel_Rect1.y + textGlobs.MsgPanel_Rect1.height) - (float)(textGlobs.textOnlyWindow->font->fontHeight);
	textGlobs.MsgPanel_Float20 = param_7; // -FC, this seems unused in this code
	textGlobs.msgXPos = x;
	Text_UpdatePositionAndSize();
}

// <LegoRR.exe @0046aca0>
void __cdecl LegoRR::Text_UpdatePositionAndSize(void)
{
	textGlobs.winFontHeightOffs = textGlobs.msgYPos;
	// -FC, modify panel 1 then assign to panel 2
	Gods98::TextWindow_ChangePosition(textGlobs.textOnlyWindow, textGlobs.MsgPanel_Rect1.x, textGlobs.MsgPanel_Rect1.y);
	Gods98::TextWindow_ChangeSize(textGlobs.textOnlyWindow, textGlobs.MsgPanel_Rect1.width, textGlobs.MsgPanel_Rect1.height);
	textGlobs.MsgPanel_Rect2 = textGlobs.MsgPanel_Rect1;
	textGlobs.TextPanelFlags &= ~TEXT_GLOB_FLAG_NERPSMSG; // -FC, inverse of 1 should be 0xFFFFFFFE
}

// <LegoRR.exe @0046ad50>
void __cdecl LegoRR::Text_Clear(void)
{
	Text_UpdatePositionAndSize();
	textGlobs.currType = Text_Type_Count;
	textGlobs.jankCounter = 0;
	textGlobs.currText = nullptr;
	textGlobs.TextPanelFlags = TEXT_GLOB_FLAG_NONE;
	TextWindow_Clear(textGlobs.textOnlyWindow);
	TextWindow_Clear(textGlobs.textImageWindow);
}

// <LegoRR.exe @0046ad90>
bool32 __cdecl LegoRR::Text_GetTextType(const char* textName, OUT Text_Type* textType)
{
	for (int i = 0; i < Text_Type_Count; i++) {
		if (_stricmp(textGlobs.textName[i], textName) == 0) {
			*textType = (Text_Type)i;
			return TRUE;
		}
	}
	return FALSE;
}

// <LegoRR.exe @0046add0>
void __cdecl LegoRR::Text_SetMessage(Text_Type textType, const char* textMessage)
{
	// -FC, free existing memory and allocate to new string
	if (textGlobs.textMessages[textType] != (char*)0x0) {
		Gods98::Mem_Free(textGlobs.textMessages[textType]);
	}
	textGlobs.textMessages[textType] = (char*)Gods98::Mem_Alloc(strlen(textMessage)+1);

	// -FC, loop throguh string removing addl \n's
	char* LocStrPtr = textGlobs.textMessages[textType];
	while (textMessage[0] != '\0') {
		if (textMessage[0] == '_') {
			LocStrPtr[0] = ' ';
		}
		else if ((textMessage[0] == '\\') && (textMessage[1] == 'n')) {
			LocStrPtr[0] = '\n';
			textMessage++;
		}
		else {
			*LocStrPtr = textMessage[0];
		}
		LocStrPtr++;
		textMessage++;
	}
	LocStrPtr[0] = '\0';

	// -FC, finally check if the text image exists and remove it from memory
	if (textGlobs.textImages[textType] !=  nullptr) {
		Gods98::Image_Remove(textGlobs.textImages[textType]);
		textGlobs.textImages[textType] = nullptr;
	}
}

// <LegoRR.exe @0046ae70>
void __cdecl LegoRR::Text_SetMessageWithImage(Text_Type textType, const char* message, const char* filename, const char* sfxName)
{
	Gods98::Image* image;

	// -FC, set text then copy over any nessary image as well
	Text_SetMessage(textType, message);
	std::sprintf((char*)textGlobs.textImagesSFX[textType], "%s", sfxName);
	image = Gods98::Image_LoadBMPScaled(filename, 0, 0);
	textGlobs.textImages[textType] = image;
	if (image != nullptr) {
		Gods98::Image_SetupTrans(image, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}
}

// <LegoRR.exe @0046aee0>
void __cdecl LegoRR::Text_SetNERPsMessage(const char* text, uint32 unusedCounter)
{
	textGlobs.currText = (char*)text;
	if (unusedCounter == 0) {
		unusedCounter = 0x8000;
	}
	textGlobs.jankCounter = unusedCounter | 0x10000;
	NERPs_Level_NERPMessage_Parse(text, nullptr, TRUE);
	textGlobs.TextPanelFlags |= TEXT_GLOB_FLAG_NERPSMSG;
}

// <LegoRR.exe @0046af20>
void __cdecl LegoRR::Text_DisplayMessage(Text_Type textType, bool32 changeTiming, bool32 setFlagFlashing)
{

	if (changeTiming == 0) {
		if (textGlobs.flashTimer <= 0.0) {
			textGlobs.currType = textType;
		}
	}
	else {
		textGlobs.currType = textType;
		textGlobs.flashTimer = textGlobs.TextPauseTime;
	}

	// -FC, if tutorial flags off grab the normal text/image
	if ((TutorialFlags)NERPFunc__GetTutorialFlags(nullptr) == TUTORIAL_FLAG_NONE) {
		if (SFX_GetType((char*)textGlobs.textImagesSFX[textType], (SFX_ID*)&changeTiming) != 0) {
			SFX_Random_SetAndPlayGlobalSample((SFX_ID)changeTiming, nullptr);
		}
	}

	// -FC, if we have a type match and parm says to set flag
	if (textGlobs.currType == textType && setFlagFlashing) {
		textGlobs.TextPanelFlags |= TEXT_GLOB_FLAG_FLASHING;
		return;
	}

	// -FC, no longer flashing so clear
	textGlobs.TextPanelFlags &= ~TEXT_GLOB_FLAG_FLASHING;
}

// <LegoRR.exe @0046afc0>
void __cdecl LegoRR::Text_Update(real32 elapsedAbs)
{
	// -FC, local vars for storing state before its modified by this wierd flashing functiuon
	Text_Type locCurrType = textGlobs.currType;
	Gods98::Image* image = nullptr;
	bool isImageNull = true;
	Point2F locPoint;

	// -FC, commense the jank
	if (textGlobs.jankCounter == 0) {
		if (NERPsRuntime_IsMessagePermit() && textGlobs.currType != Text_Type_Count) {
			image = textGlobs.textImages[textGlobs.currType];
			char* format = textGlobs.textMessages[textGlobs.currType];
			isImageNull = (image == nullptr);
			Gods98::TextWindow* locTextWindow = textGlobs.textOnlyWindow;
			if (!isImageNull) {
				locPoint.x = textGlobs.TextImagePosition.x;
				locPoint.y = textGlobs.TextImagePosition.y;
				locTextWindow = textGlobs.textImageWindow;
			}
			// -FC, check format and see if its flashing
			if (format != nullptr) {
				int isFlashing = 1;
				if ((textGlobs.TextPanelFlags & TEXT_GLOB_FLAG_FLASHING) != 0) {
					isFlashing = interfaceGlobs.flashingState; // -FC this was a function but it just grabs this one var
				}
				if (isFlashing != 0) {
					TextWindow_PrintFOverlay(locTextWindow, TRUE, format);
				}
			}
			// -FC, update flash delta time
			if (textGlobs.flashTimer <= 0.0) {
				textGlobs.currType = Text_Type_Count;
			}
			textGlobs.flashTimer = textGlobs.flashTimer - elapsedAbs;
		}
	}
	else if (textGlobs.currText == nullptr) {
		textGlobs.jankCounter = 0;
	}
	else {
		// -FC, parse new nerps message to the main text box
		char strBuff[512];
		NERPs_Level_NERPMessage_Parse(textGlobs.currText, strBuff, FALSE);
		TextWindow_PrintFOverlay(textGlobs.textOnlyWindow, 0, strBuff);
		if ((textGlobs.jankCounter & 0x8000) == 0) {
			textGlobs.jankCounter = textGlobs.jankCounter - 1;
		}
	}

	if (((legoGlobs.flags1 & GAME1_FREEZEINTERFACE) != GAME1_NONE) && (locCurrType != Text_SpaceToContinue)) {
		Text_UpdatePositionAndSize();
		Panel_SetPosition(Panel_Messages, textGlobs.msgXPos, textGlobs.winFontHeightOffs);
		Panel_Draw(Panel_Messages, 0.0);
		Panel_AirMeter_DrawJuice(Panel_Messages, legoGlobs.currLevel->oxygenLevel);
		Panel_AirMeter_DrawOxygenLow(Panel_Messages);
		return;
	}

	if (!isImageNull) {
		Text_UpdatePositionAndSize();
		Panel_SetPosition(Panel_Messages, textGlobs.msgXPos, textGlobs.winFontHeightOffs);
		Panel_Draw(Panel_Messages, 0.0);
		Panel_AirMeter_DrawJuice(Panel_Messages, (legoGlobs.currLevel)->oxygenLevel);
		Panel_AirMeter_DrawOxygenLow(Panel_Messages);
		TextWindow_Update(textGlobs.textImageWindow, 0, elapsedAbs, nullptr);
		Gods98::Image_DisplayScaled(image, nullptr, &locPoint, nullptr);
		return;
	}

	Panel_SetPosition(Panel_Messages, textGlobs.msgXPos, textGlobs.winFontHeightOffs);
	Panel_Draw(Panel_Messages, 0.0);
	Panel_AirMeter_DrawJuice(Panel_Messages, (legoGlobs.currLevel)->oxygenLevel);
	Panel_AirMeter_DrawOxygenLow(Panel_Messages);

	int locWinLowPoint;
	if (TextWindow_Update(textGlobs.textOnlyWindow, 0, elapsedAbs, &locWinLowPoint) == 0) {
		if ((textGlobs.TextPanelFlags & TEXT_GLOB_FLAG_NERPSMSG) != 0) {
			if (textGlobs.winLowPos <= (float)locWinLowPoint) {
				textGlobs.TextPanelFlags &= ~TEXT_GLOB_FLAG_NERPSMSG;
				return;
			}
			textGlobs.winFontHeightOffs = textGlobs.textOnlyWindow->font->fontHeight + textGlobs.winFontHeightOffs;
			textGlobs.MsgPanel_Rect2.y = textGlobs.textOnlyWindow->font->fontHeight + textGlobs.MsgPanel_Rect2.y;
			textGlobs.MsgPanel_Rect2.height = textGlobs.MsgPanel_Rect2.height - textGlobs.textOnlyWindow->font->fontHeight;
			TextWindow_ChangePosition(textGlobs.textOnlyWindow, textGlobs.MsgPanel_Rect2.x, textGlobs.MsgPanel_Rect2.y);
			TextWindow_ChangeSize(textGlobs.textOnlyWindow, textGlobs.MsgPanel_Rect2.width, textGlobs.MsgPanel_Rect2.height);

			if (textGlobs.msgYPos <= textGlobs.winFontHeightOffs) {
				textGlobs.winFontHeightOffs = textGlobs.msgYPos;
				textGlobs.TextPanelFlags &= ~TEXT_GLOB_FLAG_NERPSMSG;
				textGlobs.MsgPanel_Rect2 = textGlobs.MsgPanel_Rect1;
				TextWindow_ChangePosition(textGlobs.textOnlyWindow, textGlobs.MsgPanel_Rect1.x, textGlobs.MsgPanel_Rect1.y);
				TextWindow_ChangeSize(textGlobs.textOnlyWindow, textGlobs.MsgPanel_Rect2.width, textGlobs.MsgPanel_Rect2.height);
				return;
			}
		}
		return;
	}

	// -FC, if nothing else has returned, clear the nerps flag and update the output window
	textGlobs.winFontHeightOffs = textGlobs.winFontHeightOffs - textGlobs.textOnlyWindow->font->fontHeight;
	textGlobs.MsgPanel_Rect2.y = textGlobs.MsgPanel_Rect2.y - textGlobs.textOnlyWindow->font->fontHeight;
	textGlobs.MsgPanel_Rect2.height = textGlobs.textOnlyWindow->font->fontHeight + textGlobs.MsgPanel_Rect2.height;
	TextWindow_ChangePosition(textGlobs.textOnlyWindow, textGlobs.MsgPanel_Rect2.x, textGlobs.MsgPanel_Rect2.y);
	TextWindow_ChangeSize(textGlobs.textOnlyWindow, textGlobs.MsgPanel_Rect2.width, textGlobs.MsgPanel_Rect2.height);
	textGlobs.TextPanelFlags &= ~TEXT_GLOB_FLAG_NERPSMSG;
}

#pragma endregion
