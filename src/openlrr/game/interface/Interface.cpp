// Interface.cpp : 
//

#include "../../engine/drawing/Draw.h"
#include "../../engine/input/Input.h"
#include "../../engine/input/Keys.h"

#include "../Game.h"
#include "../Shortcuts.hpp"

#include "Interface.h"


using Gods98::Keys;
using Shortcuts::ShortcutID;


/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define INTERFACE_PRERENDER_HOVEROUTLINES		true

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004a3b58>
real32 & LegoRR::g_Interface_TIMER_004a3b58 = *(real32*)0x004a3b58;

// <LegoRR.exe @004a3b5c>
uint32 & LegoRR::g_Interface_UINT_004a3b5c = *(uint32*)0x004a3b5c;

// <LegoRR.exe @004ddd58>
LegoRR::Interface_Globs & LegoRR::interfaceGlobs = *(LegoRR::Interface_Globs*)0x004ddd58;


/// CUSTOM: Avoid excess surface locking by pre-rendering highlight outlines on demand.
static std::map<std::pair<uint32, uint32>, Gods98::Image*> _interfaceHoverOutlinesCache;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0041c370>
void __cdecl LegoRR::Interface_DoF2InterfaceKeyAction(void)
{
	/// KEYBIND: [F2]+(any)  "F2 configurable interface actions."
	if ((interfaceGlobs.flags & INTERFACE_GLOB_FLAG_UNK_4) &&
		(interfaceGlobs.flags & INTERFACE_GLOB_FLAG_UNK_80) &&
		Shortcut_IsDown(ShortcutID::InterfaceActionModifier))
	{
		Interface_FUN_0041b5b0(interfaceGlobs.currMenuType, Interface_CallbackDoMenuIconKeyAction, nullptr);
	}
}

// <LegoRR.exe @0041c3a0>
bool32 __cdecl LegoRR::Interface_CallbackDoMenuIconKeyAction(Interface_MenuItemType menuIcon, LegoObject_Type objType, LegoObject_ID objID)
{
	Keys keyID = Keys::KEY_NONE;
	if (menuIcon == Interface_MenuItem_Build) {
		if (objType == LegoObject_Vehicle) {
			keyID = interfaceGlobs.vehicleItemF2Keys[objID];
		}
		else if (objType == LegoObject_Building) {
			keyID = interfaceGlobs.buildingItemF2Keys[objID];
		}
	}
	else {
		keyID = interfaceGlobs.menuItemF2keys[menuIcon];
	}

	if (Input_IsKeyPressed(keyID)) {
		Interface_SetDat_004decd8_004decdc(menuIcon, objType, objID);
		Interface_DoAction_FUN_0041dbd0(menuIcon);

		// Prevent other debug shortcuts from triggering.
		Shortcuts::shortcutManager.Disable(true);
		// Re-activate the interface modifier shortcut.
		Shortcut_Activate(ShortcutID::InterfaceActionModifier, true);
	}
	return false;
}


// <LegoRR.exe @0041cc60>
void __cdecl LegoRR::Interface_DrawHoverOutline(const Area2F* area)
{
	if (!(legoGlobs.flags1 & GAME1_FREEZEINTERFACE)) {
		/// OPTIMISE: Draw pre-rendered hover outlines to avoid locking the back surface every frame.
		static constexpr const uint32 THICKNESS = 2;

		const ColourRGBF colour = { 0.0f, 1.0f, 0.0f };

		const Point2F destPos = {
			area->x - THICKNESS,
			area->y - THICKNESS,
		};

		Gods98::Image* prerendered = nullptr;

		// Note: Change define to false to disable pre-rendered drawing.
		#if INTERFACE_PRERENDER_HOVEROUTLINES
		/// NOTE: +1 is added because of how the line drawing was setup, the area dimensions are actually 1 less than the intended hover outline dimensions.
		const auto sizePair = std::make_pair(static_cast<uint32>(std::max(0.0f, area->width))  + THICKNESS * 2 + 1,
											 static_cast<uint32>(std::max(0.0f, area->height)) + THICKNESS * 2 + 1);

		auto it = _interfaceHoverOutlinesCache.find(sizePair);
		if (it != _interfaceHoverOutlinesCache.end()) {
			// We've already pre-rendered an image for this size, use that to avoid needlessly locking a surface.
			prerendered = it->second;
			Gods98::Image_Display(prerendered, &destPos);
			return;
		}

		// We haven't pre-rendered an outline for this size yet, generate it now.
		prerendered = Gods98::Image_CreateNew(sizePair.first, sizePair.second);
		ColourRGBF trans;
		Gods98::Image_FindTransColour(prerendered, &colour, 1, &trans);
		Gods98::Image_SetupTrans2(prerendered, trans.red, trans.green, trans.blue,
									trans.red, trans.green, trans.blue, false); // Don't truncate to 16-bit colour.

		_interfaceHoverOutlinesCache[sizePair] = prerendered;
		#endif

		Gods98::Draw_Begin(prerendered);
		Error_Warn(!Gods98::Draw_IsLocked(), "Interface_DrawHoverOutline: Draw_Begin() failed to lock the surface");
		if (prerendered != nullptr) {
			Gods98::Draw_SetTranslation(Point2F { -destPos.x, -destPos.y }); // Translate to 0,0.
		}

		Point2F start = {
			area->x,
			area->y,
		};
		Point2F end = {
			(area->x + area->width),
			(area->y + area->height),
		};

		// Draw for rect expanded by 1 pixel.
		// Then draw for rect expanded by 2 pixels, for a 2-pixel-width rectangle outline.

		#if true
		/// REFACTOR: Draw all sets of lines in one Draw call instead of two.

		Point2F rectLinesFrom[THICKNESS*4] = { 0.0f }; // dummy inits
		Point2F rectLinesTo[THICKNESS*4] = { 0.0f };
		
		for (uint32 i = 0; i < THICKNESS; i++) {
			start.x -= 1.0f;
			start.y -= 1.0f;
			end.x   += 1.0f;
			end.y   += 1.0f;

			// 0 ____ 1
			//  |    |
			//  |____|
			// 3      2

			rectLinesFrom[(i*4) + 0] = Point2F { start.x, start.y };
			rectLinesFrom[(i*4) + 1] = Point2F {   end.x, start.y };
			rectLinesFrom[(i*4) + 2] = Point2F {   end.x,   end.y };
			rectLinesFrom[(i*4) + 3] = Point2F { start.x,   end.y };
			for (uint32 j = 0; j < 4; j++) {
				rectLinesTo[(i*4) + j] = rectLinesFrom[(i*4) + ((j+1)%4)];
			}
		}
		Gods98::Draw_LineListEx(rectLinesFrom, rectLinesTo, THICKNESS*4, colour.red, colour.green, colour.blue, Gods98::DrawEffect::None);

		#else
		Point2F rectLines[5] = { 0.0f }; // dummy init

		for (uint32 i = 0; i < 2; i++) {
			start.x -= 1.0f;
			start.y -= 1.0f;
			end.x   += 2.0f;
			end.y   += 2.0f;

			// 0 ____ 1
			//  |    |
			//  |____|
			// 3      2

			rectLines[0] = Point2F { start.x, start.y };
			rectLines[1] = Point2F {   end.x, start.y };
			rectLines[2] = Point2F {   end.x,   end.y };
			rectLines[3] = Point2F { start.x,   end.y };
			rectLines[4] = rectLines[0];

			Gods98::Draw_LineListEx(rectLines, rectLines + 1, 4, colour.red, colour.green, colour.blue, Gods98::DrawEffect::None);
		}
		#endif

		Gods98::Draw_End();

		if (prerendered != nullptr) {
			Gods98::Image_Display(prerendered, &destPos);
		}
	}
}

#pragma endregion
