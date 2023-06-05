// Advisor.cpp : 
//

#include "../../engine/audio/3DSound.h"
#include "../../engine/core/Maths.h"
#include "../../engine/core/Memory.h"
#include "../../engine/core/Utils.h"
#include "../../engine/gfx/Containers.h"
#include "../../engine/Graphics.h"
#include "../../engine/Main.h"
#include "../../legacy.h"

#include "../Game.h"
#include "../audio/SFX.h"
#include "../interface/Panels.h"
#include "../interface/TextMessages.h"
#include "Advisor.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004b3db0>
LegoRR::Advisor_Globs & LegoRR::advisorGlobs = *(LegoRR::Advisor_Globs*)0x004b3db0;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00401000>
void __cdecl LegoRR::Advisor_Initialise(const char* gameName, LegoCamera* cameraMain, Gods98::Viewport* viewMain)
{
	Advisor_RegisterPositionName(Advisor_Objective);
	Advisor_RegisterPositionName(Advisor_ObjectiveAcheived);
	Advisor_RegisterPositionName(Advisor_ObjectiveFailed);
	Advisor_RegisterPositionName(Advisor_IconPoint_Normal);
	Advisor_RegisterPositionName(Advisor_IconPoint_BackButton);
	Advisor_RegisterPositionName(Advisor_IconPoint_TopButtons);
	Advisor_RegisterPositionName(Advisor_PanelPoint_RadarToggle);
	Advisor_RegisterPositionName(Advisor_PanelPoint_RadarTaggedObjectView);
	Advisor_RegisterPositionName(Advisor_PanelPoint_RadarZoomIn);
	Advisor_RegisterPositionName(Advisor_PanelPoint_RadarZoomOut);
	Advisor_RegisterPositionName(Advisor_PanelPoint_RadarMapView);
	Advisor_RegisterPositionName(Advisor_PanelPoint_InfoDockGoto);
	Advisor_RegisterPositionName(Advisor_PanelPoint_InfoDockClose);
	Advisor_RegisterPositionName(Advisor_PanelPoint_TopPanelInfo);
	Advisor_RegisterPositionName(Advisor_PanelPoint_TopPanelOptions);
	Advisor_RegisterPositionName(Advisor_PanelPoint_TopPanelPriorities);
	Advisor_RegisterPositionName(Advisor_PanelPoint_TopPanelCallToArms);
	Advisor_RegisterPositionName(Advisor_PanelPoint_TopPanelGoBack);
	Advisor_RegisterPositionName(Advisor_PanelPoint_ControlZoomIn);
	Advisor_RegisterPositionName(Advisor_PanelPoint_ControlZoomOut);
	Advisor_RegisterPositionName(Advisor_TalkInGame);

	Advisor_RegisterAnimName(Advisor_Anim_PointToMap);
	Advisor_RegisterAnimName(Advisor_Anim_Point_N);
	Advisor_RegisterAnimName(Advisor_Anim_Point_NE);
	Advisor_RegisterAnimName(Advisor_Anim_Point_E);
	Advisor_RegisterAnimName(Advisor_Anim_Point_SE);
	Advisor_RegisterAnimName(Advisor_Anim_Point_S);
	Advisor_RegisterAnimName(Advisor_Anim_Point_SW);
	Advisor_RegisterAnimName(Advisor_Anim_Point_W);
	Advisor_RegisterAnimName(Advisor_Anim_Point_NW);
	Advisor_RegisterAnimName(Advisor_Anim_Point_Up);
	Advisor_RegisterAnimName(Advisor_Anim_Talk_Top);

	advisorGlobs.cameraCont = cameraMain->contCam;
	advisorGlobs.viewMain = viewMain;

	advisorGlobs.lightCont = Gods98::Container_MakeLight(cameraMain->contCam, Gods98::Container_Light::Directional, 1.0f, 1.0f, 1.0f);
	Gods98::Container_SetOrientation(advisorGlobs.lightCont, cameraMain->contCam, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	Gods98::Container_Hide(advisorGlobs.lightCont, true);

	advisorGlobs.flags = ADVISOR_GLOB_FLAG_NONE;
}

// <LegoRR.exe @004011c0>
void __cdecl LegoRR::Advisor_Stop(void)
{
	Advisor_Anim animType = advisorGlobs.positions[advisorGlobs.currType].animType;
	Gods98::Container* animCont = advisorGlobs.anims[animType].cont;

	if (animCont != nullptr) {
		Gods98::Container_Hide(animCont, true);
	}
	Gods98::Container_Hide(advisorGlobs.lightCont, true);

	advisorGlobs.flags &= ~ADVISOR_GLOB_FLAG_ANIMATING;
}

// <LegoRR.exe @00401210>
void __cdecl LegoRR::Advisor_InitViewport(real32 param_unused)
{
	// It seems there used to be more functionality here, but it's been gutted.
	//Gods98::Viewport_GetBackClip(advisorGlobs.viewMain);  // unused accessor (returns real32)
	//Gods98::Viewport_GetFrontClip(advisorGlobs.viewMain); // unused accessor (returns real32)
	advisorGlobs.viewZ = 0.96f;
}

/// USAGE: <advisorType>  <animFileName>,<loopStart(seconds)>,<loopEnd(seconds)>
/// NOTE: last two parameters can be set to any value if the anim is not made to loop e.g. 0,0
///
/// EXONE: Advisor_Anim_Point_Up  Mini-Figures\Captain\NEW_Captain_Point_CALL_T_ARMS,1.04,4.0
/// EXTWO: Advisor_Anim_Talk_Top  Mini-Figures\Captain\NEW_Captain_Point_QTalk,1.04,2.4
// <LegoRR.exe @00401270>
void __cdecl LegoRR::Advisor_LoadAnims(const Gods98::Config* config, const char* gameName)
{
	char reduceName[64];
	char* parts[3];

	for (uint32 i = 0; i < Advisor_Anim_Count; i++) {
		const char* animName = advisorGlobs.animName[i];
		AdvisorAnimData* animData = &advisorGlobs.anims[i];

		/// FIX APPLY: Create new allocation for string value used by Util_Tokenise.
		char* value = Gods98::Config_GetStringValue(config, Config_ID(gameName, "Advisor", animName));

		if (value == nullptr) {
			// If the property isn't found, check if its listed with the reduce prefix: '!'
			if (!Gods98::Graphics_IsReduceAnimation()) {
				/// FIX APPLY: Correctly check reduce prefix by passing reduceName
				///            into Config_ID instead of animName again.
				std::sprintf(reduceName, "!%s", animName);
				value = Gods98::Config_GetStringValue(config, Config_ID(gameName, "Advisor", reduceName));
			}
		}

		if (value != nullptr) {

			// BAD BAD BAD BAD! Using Util_Tokenise on TempStringValue (should not be modified)
			Gods98::Util_Tokenise(value, parts, ",");

			animData->cont = Gods98::Container_Load(advisorGlobs.cameraCont, parts[0], CONTAINER_LWSSTRING, true);
			if (animData->cont != nullptr) {
				Gods98::Container_SetOrientation(animData->cont, advisorGlobs.cameraCont, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
				Gods98::Container_Hide(animData->cont, true);
			}
			animData->loopStartTime = (real32)(std::atof(parts[1]) * STANDARD_FRAMERATE);
			animData->loopEndTime   = (real32)(std::atof(parts[2]) * STANDARD_FRAMERATE);

			/// FIX APPLY: Cleanup new allocation for string value used by Util_Tokenise.
			Gods98::Mem_Free(value);
		}
	}
}

// <LegoRR.exe @004013a0>
bool32 __cdecl LegoRR::Advisor_GetAdvisorType(const char* advisorName, OUT Advisor_Type* advisorType)
{
	for (uint32 i = 0; i < Advisor_Type_Count; i++) {
		if (::_stricmp(advisorGlobs.positionName[i], advisorName) == 0) {
			*advisorType = (Advisor_Type)i;
			return true;
		}
	}
	return false;
}

// <LegoRR.exe @00401430>
bool32 __cdecl LegoRR::Advisor_GetAnimType(const char* animName, OUT Advisor_Anim* animType)
{
	for (uint32 i = 0; i < Advisor_Anim_Count; i++) {
		if (::_stricmp(advisorGlobs.animName[i], animName) == 0) {
			*animType = (Advisor_Anim)i;
			return true;
		}
	}
	return false;
}

/// USAGE: <positionType>  <animName>,<textName|NULL>,<sfxName|SFX_NULL>,<x(float)>,<y(float)>,<panelName|NULL>
///
/// EXONE: Advisor_IconPoint_TopButtons    Advisor_Anim_Point_NE,NULL,SFX_NULL,-25,25,NULL
/// EXTWO: Advisor_PanelPoint_RadarToggle  Advisor_Anim_Point_N,NULL,SFX_NULL,220,65,Panel_Radar
// <LegoRR.exe @004014a0>
void __cdecl LegoRR::Advisor_LoadPositions(const Gods98::Config* config, const char* gameName, sint32 screenWidth, sint32 screenHeight)
{
	char arrayName[128];
	char* parts[6];

	std::sprintf(arrayName, "AdvisorPositions%ix%i", screenWidth, screenHeight);
	const char* arrayID = Config_ID(gameName, arrayName);
	for (const Gods98::Config* prop = Gods98::Config_FindArray(config, arrayID); prop != nullptr; prop = Gods98::Config_GetNextItem(prop)) {
		
		Text_Type textType   = Text_Type_Count;  // treated as "NULL"
		Panel_Type panelType = Panel_Type_Count; // treated as "NULL"

		Advisor_Type advisorType;
		Advisor_Anim animType;
		SFX_ID sfxID;

		if (Advisor_GetAdvisorType(Gods98::Config_GetItemName(prop), &advisorType)) {
			// BAD BAD BAD! Even more using Util_Tokenise with an immutable string!!

			/// FIX APPLY: Create new allocation for string value used by Util_Tokenise.
			char* value = Gods98::Util_StrCpy(Gods98::Config_GetDataString(prop));

			Gods98::Util_Tokenise(value, parts, ",");

			if (Advisor_GetAnimType(parts[0], &animType)) {
				if (::_stricmp("NULL", parts[1]) == 0 || Text_GetTextType(parts[1], &textType)) {
					if (SFX_GetType(parts[2], &sfxID)) {

						real32 x = (real32)std::atof(parts[3]);
						real32 y = (real32)std::atof(parts[4]);

						if (::_stricmp("NULL", parts[5]) == 0 || Panel_GetPanelType(parts[5], &panelType)) {
							Advisor_AddPosition(advisorType, animType, textType, sfxID, panelType, x, y);
						}
					}
				}
			}

			/// FIX APPLY: Cleanup new allocation for string value used by Util_Tokenise.
			Gods98::Mem_Free(value);
		}
	}
}

// <LegoRR.exe @004016f0>
void __cdecl LegoRR::Advisor_AddPosition(Advisor_Type advisorType, Advisor_Anim animType, Text_Type textType, SFX_ID sfxID, Panel_Type panelType, real32 x, real32 y)
{
	advisorGlobs.positions[advisorType].flags = ADVISOR_FLAG_USED;
	advisorGlobs.positions[advisorType].animType = animType;
	advisorGlobs.positions[advisorType].textType = textType;
	advisorGlobs.positions[advisorType].sfxID = sfxID;
	advisorGlobs.positions[advisorType].x = x;
	advisorGlobs.positions[advisorType].y = y;
	advisorGlobs.positions[advisorType].origX = x;
	advisorGlobs.positions[advisorType].origY = y;
	advisorGlobs.positions[advisorType].panelType = panelType;

	if (panelType == Panel_Type_Count) { // NULL panel
		advisorGlobs.positions[advisorType].flags |= ADVISOR_FLAG_NOPANEL;
	}

	if (textType != Text_Type_Count) { // non-NULL text
		advisorGlobs.positions[advisorType].flags |= ADVISOR_FLAG_HASTEXT;
	}
}

// <LegoRR.exe @00401780>
void __cdecl LegoRR::Advisor_SetParameters(Advisor_Type advisorType, Panel_Type panelType, real32 x, real32 y)
{
	advisorGlobs.positions[advisorType].x = x;
	advisorGlobs.positions[advisorType].y = y;
	advisorGlobs.positions[advisorType].panelType = panelType;

	if (panelType == Panel_Type_Count) // NULL panel
		advisorGlobs.positions[advisorType].flags |= ADVISOR_FLAG_NOPANEL;
	else
		advisorGlobs.positions[advisorType].flags &= ~ADVISOR_FLAG_NOPANEL;
}

// <LegoRR.exe @004017d0>
void __cdecl LegoRR::Advisor_GetOrigPos(Advisor_Type advisorType, OUT real32* origX, OUT real32* origY)
{
	*origX = advisorGlobs.positions[advisorType].origX;
	*origY = advisorGlobs.positions[advisorType].origY;
}

// <LegoRR.exe @00401800>
bool32 __cdecl LegoRR::Advisor_Start(Advisor_Type advisorType, bool32 loop)
{
	if (advisorGlobs.positions[advisorType].flags & ADVISOR_FLAG_USED) {

		if (!(advisorGlobs.flags & ADVISOR_GLOB_FLAG_ANIMATING) && Lego_GetViewMode() != ViewMode_FP) {
			advisorGlobs.currType = advisorType;
			Advisor_PlaySFX(advisorType);

			advisorGlobs.flags |= ADVISOR_GLOB_FLAG_ANIMATING;
			if (loop) advisorGlobs.flags |= ADVISOR_GLOB_FLAG_LOOPING;
			else      advisorGlobs.flags &= ~ADVISOR_GLOB_FLAG_LOOPING;
				
			return true;
		}
	}
	return false;
}

// <LegoRR.exe @00401870>
void __cdecl LegoRR::Advisor_PlaySFX(Advisor_Type advisorType)
{
	const Advisor_Anim animType = advisorGlobs.positions[advisorType].animType;
	Gods98::Container* animCont = advisorGlobs.anims[animType].cont;
	if (animCont != nullptr) {
		Gods98::Container_Hide(animCont, false);
		Gods98::Container_SetAnimationTime(animCont, 0.0f);

		SFX_Random_PlaySoundNormal(advisorGlobs.positions[advisorType].sfxID, false);
		SFX_AddToQueue(SFX_CaptainSlide, Gods98::SoundMode::Once);
	}
}

// Called by Main_GameUpdate
// <LegoRR.exe @004018d0>
void __cdecl LegoRR::Advisor_Update(real32 elapsed)
{
	const Advisor_Type advisorType = advisorGlobs.currType;
	const Advisor_Anim animType = advisorGlobs.positions[advisorType].animType;
	Gods98::Container* animCont = advisorGlobs.anims[animType].cont;

	if (animCont != nullptr) {
		if ((advisorGlobs.flags & ADVISOR_GLOB_FLAG_ANIMATING) && Lego_GetViewMode() != ViewMode_FP) {

			if (advisorGlobs.positions[advisorType].flags & ADVISOR_FLAG_HASTEXT) {
				Text_DisplayMessage(advisorGlobs.positions[advisorType].textType, true, false);
			}

			Gods98::Container_Hide(animCont, false);
			Gods98::Container_Hide(advisorGlobs.lightCont, false);
			Gods98::Container_Light_SetEnableContainer(advisorGlobs.lightCont, animCont);

			Advisor_ViewportTransform(advisorType);

			if (Advisor_MoveAnimation(advisorGlobs.positions[advisorType].animType, elapsed)) {
				return; // Still animating, no cleanup.
			}

			// Animation has ended.

			advisorGlobs.flags &= ~ADVISOR_GLOB_FLAG_ANIMATING;
		}
		Gods98::Container_Hide(animCont, true);
	}
	Gods98::Container_Hide(advisorGlobs.lightCont, true);
}

// <LegoRR.exe @004019b0>
bool32 __cdecl LegoRR::Advisor_MoveAnimation(Advisor_Anim animType, real32 elapsed)
{
	AdvisorAnimData* animData = &advisorGlobs.anims[animType];
	Gods98::Container* animCont = animData->cont;

	real32 overrun = Gods98::Container_MoveAnimation(animCont, elapsed);

	if (advisorGlobs.flags & ADVISOR_GLOB_FLAG_LOOPING) {
		// While looping, wrap the animation if we've finished, or if we overrun our loopEndTime.
		if (overrun > 0.0f) {
			Gods98::Container_SetAnimationTime(animCont, (animData->loopStartTime + overrun));
		}
		else {
			real32 time = Gods98::Container_GetAnimationTime(animCont);
			if (time >= animData->loopEndTime) {
				overrun = (time - animData->loopEndTime);
				Gods98::Container_SetAnimationTime(animCont, (animData->loopStartTime + overrun));
			}
		}
	}
	else if (overrun > 0.0f) {
		return false; // Not looping, end of animation.
	}

	return true;
}

// <LegoRR.exe @00401a60>
void __cdecl LegoRR::Advisor_End(void)
{
	advisorGlobs.flags &= ~ADVISOR_GLOB_FLAG_LOOPING;
}

// Called by Advisor_Update
// <LegoRR.exe @00401a70>
void __cdecl LegoRR::Advisor_ViewportTransform(Advisor_Type advisorType)
{
	const Advisor_Anim animType = advisorGlobs.positions[advisorType].animType;
	Gods98::Container* cont = advisorGlobs.anims[animType].cont;

	Point2F panelPos;

	if (!(advisorGlobs.positions[advisorType].flags & ADVISOR_FLAG_NOPANEL)) {
		// output -> x, y (get panel position?)
		Panel_GetPosition(advisorGlobs.positions[advisorType].panelType, &panelPos.x, &panelPos.y);
	}
	else {
		panelPos.x = 0.0f;
		panelPos.y = 0.0f;
	}

	Vector3F vector;
	const Vector4F transform4d = {
		(advisorGlobs.positions[advisorType].x + panelPos.x),
		(advisorGlobs.positions[advisorType].y + panelPos.y),
		advisorGlobs.viewZ,
		1.0f,
	};
	Gods98::Viewport_InverseTransform(advisorGlobs.viewMain, &vector, &transform4d);
	Gods98::Container_SetPosition(cont, nullptr, vector.x, vector.y, vector.z);
}

// <LegoRR.exe @00401b60>
bool32 __cdecl LegoRR::Advisor_IsAnimating(void)
{
	return (advisorGlobs.flags & ADVISOR_GLOB_FLAG_ANIMATING);
}

#pragma endregion
