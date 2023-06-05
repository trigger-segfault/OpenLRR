// ObjInfo.cpp : 
//

#include "../../../engine/core/Maths.h"
#include "../../../engine/drawing/Draw.h"

#include "../../effects/DamageText.h"
#include "../../object/Object.h"

#include "ObjInfo.h"



/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define OBJINFO_PRERENDER_HEALTHBARS		true

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00500e68>
LegoRR::ObjInfo_Globs & LegoRR::objinfoGlobs = *(LegoRR::ObjInfo_Globs*)0x00500e68;

/// CUSTOM: Key represents size of filled health in bar.
static std::map<uint32, Gods98::Image*> _objInfoHealthBarsCache;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/// CUSTOM: Invalidate existing pre-rendered health bars. Use when changing health bar settings at runtime.
void LegoRR::ObjInfo_InvalidateHealthBars()
{
	_objInfoHealthBarsCache.clear();
}

/// CUSTOM:
void LegoRR::ObjInfo_TryBeginDraw()
{
	#if !OBJINFO_PRERENDER_HEALTHBARS
	if (!Gods98::Draw_IsLocked()) {
		Gods98::Draw_Begin();
	}
	#endif
}

/// CUSTOM:
void LegoRR::ObjInfo_TryEndDraw()
{
	#if !OBJINFO_PRERENDER_HEALTHBARS
	if (Gods98::Draw_IsLocked()) {
		Gods98::Draw_End();
	}
	#endif
}


// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @00459dc0>
void __cdecl LegoRR::ObjInfo_DrawHealthBar(LegoObject* liveObj, sint32 screenX, sint32 screenY)
{
	if (!(objinfoGlobs.flags & OBJINFO_GLOB_FLAG_HEALTHBAR))
		return; // No drawing health bars.

	if (!DamageFont_LiveObject_CheckCanShowDamage_Unk(liveObj))
		return; // Don't draw health bars for this unit.

	const ColourRGBF rcfColour = objinfoGlobs.HealthBarRGB;
	const ColourRGBF rcbColour = objinfoGlobs.HealthBarBackgroundRGB;
	const ColourRGBF ln1Colour = {
		objinfoGlobs.HealthBarBorderRGB_r[1],
		objinfoGlobs.HealthBarBorderRGB_g[1],
		objinfoGlobs.HealthBarBorderRGB_b[1],
	};
	const ColourRGBF ln2Colour = {
		objinfoGlobs.HealthBarBorderRGB_r[2],
		objinfoGlobs.HealthBarBorderRGB_g[2],
		objinfoGlobs.HealthBarBorderRGB_b[2],
	};

	const Area2F borderRect = {
		objinfoGlobs.HealthBarPosition.x + static_cast<real32>(screenX),
		objinfoGlobs.HealthBarPosition.y + static_cast<real32>(screenY),
		objinfoGlobs.HealthBarWidthHeight.width  + static_cast<real32>(objinfoGlobs.HealthBarBorderSize * 2),
		objinfoGlobs.HealthBarWidthHeight.height + static_cast<real32>(objinfoGlobs.HealthBarBorderSize * 2),
	};

	/// CHANGE: Shrink bar size back down to avoid drawing over where the border lines will draw.
	const Area2F barRect = {
		borderRect.x + static_cast<real32>(objinfoGlobs.HealthBarBorderSize),
		borderRect.y + static_cast<real32>(objinfoGlobs.HealthBarBorderSize),
		borderRect.width  - static_cast<real32>(objinfoGlobs.HealthBarBorderSize * 2),
		borderRect.height - static_cast<real32>(objinfoGlobs.HealthBarBorderSize * 2),
	};

	const Point2F destPos = borderRect.point;

	const real32 health = std::clamp(liveObj->health, 0.0f, 100.0f);

	/// FIX APPLY: Cast healthSize to uint32 to avoid rounding issues with the end of the red bar cutting off before the border.
	uint32 healthSize;
	if (!(objinfoGlobs.flags & OBJINFO_GLOB_FLAG_HEALTHBAR_VERTICAL))
		healthSize = static_cast<uint32>(barRect.width  * (health / 100.0f));
	else
		healthSize = static_cast<uint32>(barRect.height * (health / 100.0f));
	
	Gods98::Image* prerendered = nullptr;


	// Note: Change define to false to disable pre-rendered drawing.
	#if OBJINFO_PRERENDER_HEALTHBARS
	const uint32 healthKey = static_cast<uint32>(healthSize);
	auto it = _objInfoHealthBarsCache.find(healthKey);
	if (it != _objInfoHealthBarsCache.end()) {
		// We've already pre-rendered an image for this health size, use that to avoid needlessly locking a surface.
		prerendered = it->second;
		Gods98::Image_Display(prerendered, &destPos);
		return;
	}

	// We haven't pre-rendered a health bar for this health size yet, generate it now.
	prerendered = Gods98::Image_CreateNew(static_cast<uint32>(borderRect.width), static_cast<uint32>(borderRect.height));

	const ColourRGBF colours[4] = { rcfColour, rcbColour, ln1Colour, ln2Colour };
	ColourRGBF trans;
	Gods98::Image_FindTransColour(prerendered, colours, _countof(colours), &trans);
	Gods98::Image_SetupTrans2(prerendered, trans.red, trans.green, trans.blue,
							  trans.red, trans.green, trans.blue, false); // Don't truncate to 16-bit colour.

	_objInfoHealthBarsCache[healthKey] = prerendered;
	#endif


	// Setup outline rects.
	uint32 lineCount = 0;
	Point2F ln1ListFrom[10] = { 0.0f }; // dummy inits
	Point2F ln1ListTo[10]   = { 0.0f };
	Point2F ln2ListFrom[10] = { 0.0f };
	Point2F ln2ListTo[10]   = { 0.0f };

	// This uses the same border drawing method as: ToolTip_DrawBox
	for (uint32 i = 0; i < objinfoGlobs.HealthBarBorderSize; i++) {
		const Point2F start = {
			borderRect.x + static_cast<real32>(i),
			borderRect.y + static_cast<real32>(i),
		};
		const Point2F end = {
			((borderRect.x + borderRect.width)  - 1.0f) - static_cast<real32>(i),
			((borderRect.y + borderRect.height) - 1.0f) - static_cast<real32>(i),
		};

		// l10 <----------l11
		//  | 011111111111
		//  | 0          B ^
		//  | 0          B |
		//  v 0          B |
		//    AAAAAAAAAAAB |
		// l20----------> l21

		ln1ListFrom[lineCount + 0] = Point2F { start.x,   start.y };
		ln1ListTo[  lineCount + 0] = Point2F { start.x,   end.y-1 }; // -1 to skip overlapping pixel.
		ln1ListFrom[lineCount + 1] = Point2F {   end.x,   start.y };
		ln1ListTo[  lineCount + 1] = Point2F { start.x+1, start.y }; // +1 to skip overlapping pixel.

		ln2ListFrom[lineCount + 0] = Point2F { start.x,   end.y };
		ln2ListTo[  lineCount + 0] = Point2F { end.x-1,   end.y }; // -1 to skip overlapping pixel.
		ln2ListFrom[lineCount + 1] = Point2F { end.x,     end.y };
		ln2ListTo[  lineCount + 1] = Point2F { end.x, start.y+1 }; // +1 to skip overlapping pixel.

		lineCount += 2;
	}


	// Setup health bar fill rects.
	//const real32 health = std::clamp(liveObj->health, 0.0f, 100.0f);

	/// REFACTOR: Always draw fore/back bar rects in the same order, which changes vertical bar calculation.

	/// REFACTOR: Draw health bar background and fill in single Draw operation using Draw_RectList2Ex.
	Gods98::Draw_Rect healthRects[2] = { 0 }; // 0 = fore, 1 = back
	//Area2F foreHealthRect; // Health the unit has.
	//Area2F backHealthRect; // Health the unit is missing.

	/// CHANGE: Shrink bar size back down to avoid drawing over where the border lines will draw.
	//barRect.x += static_cast<real32>(objinfoGlobs.HealthBarBorderSize);
	//barRect.y += static_cast<real32>(objinfoGlobs.HealthBarBorderSize);
	//barRect.width  -= static_cast<real32>(objinfoGlobs.HealthBarBorderSize * 2);
	//barRect.height -= static_cast<real32>(objinfoGlobs.HealthBarBorderSize * 2);

	if (!(objinfoGlobs.flags & OBJINFO_GLOB_FLAG_HEALTHBAR_VERTICAL)) {
		// Horizontal health bars draw the bar going right as health is filled up.
		//const real32 healthSize = barRect.width * (health / 100.0f);
		healthRects[0].rect = Area2F {
			barRect.x,
			barRect.y,
			static_cast<real32>(healthSize),
			barRect.height,
		};
		healthRects[1].rect = Area2F {
			barRect.x + healthSize,
			barRect.y,
			barRect.width - healthSize,
			barRect.height,
		};
	}
	else {
		// Vertical health bars draw the bar going up as health is filled up.
		//const real32 healthSize = barRect.height * (health / 100.0f);
		healthRects[0].rect = Area2F {
			barRect.x,
			barRect.y + healthSize,
			barRect.width,
			static_cast<real32>(healthSize),
		};
		healthRects[1].rect = Area2F {
			barRect.x,
			barRect.y,
			barRect.width,
			barRect.height - healthSize,
		};
	}
	healthRects[0].colour = rcfColour;
	healthRects[1].colour = rcbColour;

	//const ColourRGBF rcfColour = objinfoGlobs.HealthBarRGB;
	//const ColourRGBF rcbColour = objinfoGlobs.HealthBarBackgroundRGB;
	/*const ColourRGBF ln1Colour = {
		objinfoGlobs.HealthBarBorderRGB_r[1],
		objinfoGlobs.HealthBarBorderRGB_g[1],
		objinfoGlobs.HealthBarBorderRGB_b[1],
	};
	const ColourRGBF ln2Colour = {
		objinfoGlobs.HealthBarBorderRGB_r[2],
		objinfoGlobs.HealthBarBorderRGB_g[2],
		objinfoGlobs.HealthBarBorderRGB_b[2],
	};*/

	if (prerendered != nullptr) {
		Gods98::Draw_Begin(prerendered);
		Error_Warn(!Gods98::Draw_IsLocked(), "ObjInfo_DrawHealthBar: Draw_Begin() failed to lock the surface");
		Gods98::Draw_SetTranslation(Point2F { -destPos.x, -destPos.y });
	}

	// Draw health bar fill rects.
	Gods98::Draw_RectList2Ex(healthRects, _countof(healthRects), Gods98::DrawEffect::None);
	//Gods98::Draw_RectListEx(&healthRects[0].rect, 1, rcfColour.red, rcfColour.green, rcfColour.blue, Gods98::DrawEffect::None);
	//Gods98::Draw_RectListEx(&healthRects[1].rect, 1, rcbColour.red, rcbColour.green, rcbColour.blue, Gods98::DrawEffect::None);

	// Draw health bar border rects.
	if (lineCount > 0) {
		Gods98::Draw_LineListEx(ln1ListFrom, ln1ListTo, lineCount, ln1Colour.red, ln1Colour.green, ln1Colour.blue,
								Gods98::DrawEffect::None);
		Gods98::Draw_LineListEx(ln2ListFrom, ln2ListTo, lineCount, ln2Colour.red, ln2Colour.green, ln2Colour.blue,
								Gods98::DrawEffect::None);
	}

	if (prerendered != nullptr) {
		Gods98::Draw_End();

		Gods98::Image_Display(prerendered, &destPos);
	}
}

// <LegoRR.exe @0045a210>
void __cdecl LegoRR::ObjInfo_DrawHungerImage(LegoObject* liveObj, sint32 screenX, sint32 screenY)
{
	if (objinfoGlobs.flags & OBJINFO_GLOB_FLAG_HUNGERIMAGES) {
		/// FIX APPLY: Clamp to minimum value of 0 too, not just max value.
		sint32 hungerIndex = static_cast<sint32>((liveObj->energy / 100.0f) * OBJINFO_HUNGERIMAGECOUNT);
		hungerIndex = std::clamp(hungerIndex, 0, OBJINFO_HUNGERIMAGECOUNT - 1);

		if (objinfoGlobs.HungerImages[hungerIndex] != nullptr) {
			const Point2F destPos = {
				objinfoGlobs.HungerPosition.x + static_cast<real32>(screenX),
				objinfoGlobs.HungerPosition.y + static_cast<real32>(screenY),
			};
			Gods98::Image_Display(objinfoGlobs.HungerImages[hungerIndex], &destPos);
		}
	}
}

// <LegoRR.exe @0045a290>
void __cdecl LegoRR::ObjInfo_DrawBubbleImage(Gods98::Image* image, sint32 screenX, sint32 screenY)
{
	if (objinfoGlobs.flags & OBJINFO_GLOB_FLAG_BUBBLEIMAGES) {
		const Point2F destPos = {
			objinfoGlobs.BubblePosition.x + static_cast<real32>(screenX),
			objinfoGlobs.BubblePosition.y + static_cast<real32>(screenY),
		};
		Gods98::Image_Display(image, &destPos);
	}
	return;
}

#pragma endregion
