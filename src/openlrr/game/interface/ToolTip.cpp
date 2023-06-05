// ToolTip.cpp : 
//

#include "../../engine/core/Files.h"
#include "../../engine/drawing/Draw.h"

#include "../audio/SFX.h"
#include "../Game.h"

#include "ToolTip.h"


/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define TOOLTIP_PRERENDER_BOXES		true

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004ab64c>
bool32 & LegoRR::s_ToolTip_sfxStopped = *(bool32*)0x004ab64c; // = true;

// <LegoRR.exe @0054cf20>
LegoRR::ToolTip_Globs & LegoRR::toolTipGlobs = *(LegoRR::ToolTip_Globs*)0x0054cf20;

/// CUSTOM: Cached image for the last-drawn tooltip.
static Gods98::Image* _toolTipImageCache = nullptr;
static Size2I _toolTipImageCacheSize = { 0, 0 };
static ColourRGBF _toolTipImageCacheRc = { 0.0f };
static ColourRGBF _toolTipImageCacheLn1 = { 0.0f };
static ColourRGBF _toolTipImageCacheLn2 = { 0.0f };
static ColourRGBF _toolTipImageCacheTrans = { 0.0f };
static uint32 _toolTipImageCacheThickness = 0;
// We can't cache half-trans drawing.
//static bool _toolTipImageCacheHalfTrans = false;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0046b490>
void __cdecl LegoRR::ToolTip_Initialise(Gods98::Font* font, uint32 borderThickness, uint32 paddingThickness, real32 delaySeconds, uint32 appWidth, uint32 appHeight, sint32 offsetY, real32 red, real32 green, real32 blue)
{
	/// SANITY: Memset globals.
	std::memset(&toolTipGlobs, 0, sizeof(toolTipGlobs));

	/// SANITY: Assign name for ToolTip_Null.
	ToolTip_RegisterName(ToolTip_Null);

	ToolTip_RegisterName(ToolTip_RadarBlock);
	ToolTip_RegisterName(ToolTip_UnitSelect);
	ToolTip_RegisterName(ToolTip_Construction);
	ToolTip_RegisterName(ToolTip_InterfaceMenu);
	ToolTip_RegisterName(ToolTip_InterfaceMenuBackButton);
	ToolTip_RegisterName(ToolTip_MapBlock);
	ToolTip_RegisterName(ToolTip_Priority);
	ToolTip_RegisterName(ToolTip_InfoMenuContinue);
	ToolTip_RegisterName(ToolTip_InfoMenuDisableFuture);
	ToolTip_RegisterName(ToolTip_RadarToggle);
	ToolTip_RegisterName(ToolTip_RadarObjectView);
	ToolTip_RegisterName(ToolTip_RadarZoomIn);
	ToolTip_RegisterName(ToolTip_RadarZoomOut);
	ToolTip_RegisterName(ToolTip_RadarMapView);
	ToolTip_RegisterName(ToolTip_InfoDockGoto);
	ToolTip_RegisterName(ToolTip_InfoDockClose);
	ToolTip_RegisterName(ToolTip_TopPanelCallToArms);
	ToolTip_RegisterName(ToolTip_TopPanelInfo);
	ToolTip_RegisterName(ToolTip_TopPanelOptions);
	ToolTip_RegisterName(ToolTip_TopPanelPriorities);
	ToolTip_RegisterName(ToolTip_PriorityDisable);
	ToolTip_RegisterName(ToolTip_PriorityUpOne);
	ToolTip_RegisterName(ToolTip_PriorityReset);
	ToolTip_RegisterName(ToolTip_CamControlZoomIn);
	ToolTip_RegisterName(ToolTip_CamControlZoomOut);
	ToolTip_RegisterName(ToolTip_CamControlCycle);
	ToolTip_RegisterName(ToolTip_CamControlRotate);
	ToolTip_RegisterName(ToolTip_SideBar_Ore);
	ToolTip_RegisterName(ToolTip_SideBar_Crystals);
	ToolTip_RegisterName(ToolTip_Close);
	ToolTip_RegisterName(ToolTip_PreviousMessage);
	ToolTip_RegisterName(ToolTip_NextMessage);
	ToolTip_RegisterName(ToolTip_More);
	ToolTip_RegisterName(ToolTip_Back);
	ToolTip_RegisterName(ToolTip_CamControlCycleMinifigures);
	ToolTip_RegisterName(ToolTip_Reward_Save);
	ToolTip_RegisterName(ToolTip_Reward_Advance);
	ToolTip_RegisterName(ToolTip_FrontEnd_Back);


	toolTipGlobs.font = font;
	toolTipGlobs.fontHeight = Gods98::Font_GetHeight(font);

	toolTipGlobs.hoverTime = delaySeconds * STANDARD_FRAMERATE; // Seconds to standard units.

	toolTipGlobs.borderThickness  = borderThickness;
	toolTipGlobs.paddingThickness = paddingThickness;
	// Note: We should really also have an offsetX for when using SetRightAlign.
	toolTipGlobs.offsetY   = offsetY; // Offset from bottom of the pointer to where tooltips draw.

	// Bounds where tooltips can be drawn.
	toolTipGlobs.appWidth  = appWidth;
	toolTipGlobs.appHeight = appHeight;

	// The way these are stored is such a headache, since we try to use ColourRGBF structs when possible...
	toolTipGlobs.rgbFloats[0] = red;
	toolTipGlobs.rgbFloats[3] = green;
	toolTipGlobs.rgbFloats[6] = blue;

	toolTipGlobs.rgbFloats[1] = red   + red   * 0.4f;
	toolTipGlobs.rgbFloats[4] = green + green * 0.4f;
	toolTipGlobs.rgbFloats[7] = blue  + blue  * 0.4f;

	toolTipGlobs.rgbFloats[2] = red   - red   * 0.4f;
	toolTipGlobs.rgbFloats[5] = green - green * 0.4f;
	toolTipGlobs.rgbFloats[8] = blue  - blue  * 0.4f;

	// Clamp rgb values to range [0.0,1.0].
	for (uint32 i = 0; i < 3; i++) {
		for (uint32 j = 0; j < 3; j++) {
			toolTipGlobs.rgbFloats[i + (j*3)] = std::clamp(toolTipGlobs.rgbFloats[i + (j*3)], 0.0f, 1.0f);
		}
	}

	/// CHANGE: Not required since we sanity nullify the entire globals structure.
	// Clear tooltip data.
	//std::memset(toolTipGlobs.toolTips, 0, sizeof(toolTipGlobs.toolTips));
}

// <LegoRR.exe @0046b790>
bool32 __cdecl LegoRR::ToolTip_GetType(const char* toolTipName, OUT ToolTip_Type* toolTipType)
{
	// Start at 1 to skip ToolTip_Null. Some callers depend on a false return value to signify 'no tooltip'.
	// toolTipName[0] is also nullptr by default. Though that's been changed for sanity's sake.
	for (uint32 i = 1; i < ToolTip_Type_Count; i++) {
		if (::_stricmp(toolTipGlobs.toolTipName[i], toolTipName) == 0) {
			*toolTipType = static_cast<ToolTip_Type>(i);
			return true;
		}
	}

	/// SANITY: Set to null on failure / no tooltip.
	*toolTipType = ToolTip_Null;
	return false;
}

// <LegoRR.exe @0046b7e0>
void __cdecl LegoRR::ToolTip_SetContent(ToolTip_Type toolTipType, const char* msg, ...)
{
	char buff[TOOLTIP_BUFFERSIZE];
	std::va_list args;

	va_start(args, msg);
	std::vsprintf(buff, msg, args);
	va_end(args);


	ToolTip* toolTip = &toolTipGlobs.toolTips[toolTipType];

	/// FIX APPLY: Dispose of previous image if there was one!!!
	if (toolTip->image != nullptr) {
		Gods98::Image_Remove(toolTip->image);
		toolTip->image = nullptr;
	}

	if (buff[0] == '@') {
		// The following text is a filename to an image, the tooltip will display an image.
		char filename[FILE_MAXPATH];
		std::strcpy(filename, buff + 1);

		toolTip->image = Gods98::Image_LoadBMP(filename);
		toolTip->flags |= TOOLTIP_FLAG_IMAGECONTENT;
		/// SANITY: Clear text buffer.
		toolTip->textBuffer[0] = '\0';
	}
	else {
		// Format text with escapes.
		char* t = toolTip->textBuffer;
		for (const char* s = buff; *s != '\0'; s++, t++) {
			const char c = *s;

			if (c == '\\' && s[1] == 'n') {
				s++; // Skip extra character.
				*t = '\n'; // Newline escape.
			}
			else if (c == '_') {
				*t = ' ';  // Space escape.
			}
			else {
				*t = c;    // Normal character.
			}
		}
		*t = '\0'; // Null-terminate buffer.

		/// FIX APPLY: Don't format buff again!! Use "%s"
		Gods98::Font_GetStringInfo(toolTipGlobs.font, &toolTip->textWidth, &toolTip->textLineCount, "%s", buff);
		// Can't have both text and image content.
		toolTip->flags &= ~TOOLTIP_FLAG_IMAGECONTENT;
	}

	toolTip->iconCount = 0;
	toolTip->iconsX = 0;
	toolTip->iconsY = 0;
	toolTip->iconsWidth  = 0;
	toolTip->iconsHeight = 0;
	toolTip->flags |= (TOOLTIP_FLAG_ENABLED1|TOOLTIP_FLAG_ENABLED2);
}

// <LegoRR.exe @0046b920>
void __cdecl LegoRR::ToolTip_AddIcon(ToolTip_Type toolTipType, OPTIONAL Gods98::Image* image)
{
	ToolTip* toolTip = &toolTipGlobs.toolTips[toolTipType];

	if (image == nullptr) {
		// Start a new row of icons.
		toolTip->iconsX = 0;
		toolTip->iconsY = 0;
	}
	else {
		toolTip->iconsX += Gods98::Image_GetWidth(image);
		if (toolTip->iconsWidth < toolTip->iconsX) {
			toolTip->iconsWidth = toolTip->iconsX;
		}

		if (toolTip->iconsY < Gods98::Image_GetHeight(image)) {
			// Remove old Y from height (if we started a new row, then height is only added).
			toolTip->iconsHeight -= toolTip->iconsY;
			toolTip->iconsY = Gods98::Image_GetHeight(image);
			// Add new Y to height.
			toolTip->iconsHeight += toolTip->iconsY;
		}
	}

	// Note: A null icon in the list is needed to tell ToolTip_Draw to start a new row.
	toolTip->iconList[toolTip->iconCount] = image;
	toolTip->iconCount++;
}

// <LegoRR.exe @0046b9c0>
void __cdecl LegoRR::ToolTip_SetSFX(ToolTip_Type toolTipType, SFX_ID sfxType)
{
	toolTipGlobs.toolTips[toolTipType].sfxType = sfxType;
	toolTipGlobs.toolTips[toolTipType].flags |= (TOOLTIP_FLAG_ENABLED1|TOOLTIP_FLAG_ENABLED2);
}

// <LegoRR.exe @0046b9f0>
void __cdecl LegoRR::ToolTip_SetRightAlign(ToolTip_Type toolTipType, bool32 on)
{
	if (on)
		toolTipGlobs.toolTips[toolTipType].flags |= TOOLTIP_FLAG_RIGHTALIGN;
	else
		toolTipGlobs.toolTips[toolTipType].flags &= ~TOOLTIP_FLAG_RIGHTALIGN;
}

// <LegoRR.exe @0046ba30>
void __cdecl LegoRR::ToolTip_Activate(ToolTip_Type toolTipType)
{
	toolTipGlobs.toolTips[toolTipType].flags |= TOOLTIP_FLAG_ACTIVE;
}

// <LegoRR.exe @0046ba60>
void __cdecl LegoRR::ToolTip_ShowInstant(ToolTip_Type toolTipType)
{
	// Complete timer so the tooltip will show immediately (if activated).
	toolTipGlobs.toolTips[toolTipType].timer = toolTipGlobs.hoverTime;
}

// <LegoRR.exe @0046ba80>
void __cdecl LegoRR::ToolTip_Update(uint32 mouseX, uint32 mouseY, real32 elapsedAbs)
{
	//static bool s_ToolTip_sfxStopped = true;

	bool hasShowing = false;
	bool hasActive  = false;

	// Start at 1 to skip ToolTip_Null.
	for (uint32 i = 1; i < ToolTip_Type_Count; i++) {
		ToolTip* toolTip = &toolTipGlobs.toolTips[i];

		if (!(toolTip->flags & (TOOLTIP_FLAG_ENABLED1|TOOLTIP_FLAG_ENABLED2)))
			continue;

		/// CUSTOM: Allow disabling tooltips altogether.
		if (!(toolTip->flags & TOOLTIP_FLAG_ACTIVE) || hasActive || !Lego_IsShowToolTips()) {
			toolTip->timer = 0.0f;
		}
		else {
			if (toolTip->timer < toolTipGlobs.hoverTime || hasShowing) {
				s_ToolTip_sfxStopped = true;
			}
			else {
				if (toolTip->sfxType != SFX_NULL && s_ToolTip_sfxStopped && !Lego_IsDisableToolTipSound()) {
					s_ToolTip_sfxStopped = !SFX_Random_SetAndPlayGlobalSample(toolTip->sfxType, nullptr);
				}

				/// FIX APPLY: Don't compare the text buffer array address instead of the character!
				/// FIX APPLY: Also check for image content instead of just text buffer.
				//if (toolTip->textBuffer != '\0')
				if (toolTip->textBuffer[0] != '\0' || toolTip->image != nullptr) {
					// Draw the tooltip if we have any text.
					/// TODO: Should we allow drawing tooltips if they only have icons but no text??
					ToolTip_Draw(toolTip, mouseX, mouseY);
				}

				hasShowing = true; // Effectively useless, since hasActive prevents the logic from reaching this variable's usage.
			}
			hasActive = true; // An active tooltip exists. First come first serve I guess...
			toolTip->timer += elapsedAbs;
		}

		// Note: This needs to be outside of the check for ACTIVE, since the check is also affected by hasActive.
		toolTip->flags &= ~TOOLTIP_FLAG_ACTIVE;
	}
}

// For some weird reason, Area2F is passed BY VALUE here.
// The only reason this was even determined was due to the weird compiler behavior when calling this function.
// <LegoRR.exe @0046bb70>
void __cdecl LegoRR::ToolTip_DrawBox(Area2F valueRect, real32 rcRed, real32 rcGreen, real32 rcBlue, real32 ln1Red, real32 ln1Green, real32 ln1Blue, real32 ln2Red, real32 ln2Green, real32 ln2Blue, bool32 halfTrans)
{
	/// OPTIMISE: Draw pre-rendered tooltips on smaller surfaces to avoid locking the back surface every frame.

	const Point2F destPos = valueRect.point;

	Gods98::Image* prerendered = nullptr;

	// Note: Change define to false to disable pre-rendered drawing.
	#if TOOLTIP_PRERENDER_BOXES
	if (!halfTrans) {
		// Can only pre-render when not using halfTrans.
		Size2I size = { static_cast<sint32>(valueRect.width), static_cast<sint32>(valueRect.height) };
		const ColourRGBF rc  = { rcRed,  rcGreen,  rcBlue };
		const ColourRGBF ln1 = { ln1Red, ln1Green, ln1Blue };
		const ColourRGBF ln2 = { ln2Red, ln2Green, ln2Blue };

		if (_toolTipImageCache != nullptr &&
			(size.width == _toolTipImageCacheSize.width && size.height == _toolTipImageCacheSize.height) &&
			(rc.red  == _toolTipImageCacheRc.red  && rc.green  == _toolTipImageCacheRc.green  && rc.blue  == _toolTipImageCacheRc.blue) &&
			(ln1.red == _toolTipImageCacheLn1.red && ln1.green == _toolTipImageCacheLn1.green && ln1.blue == _toolTipImageCacheLn1.blue) &&
			(ln2.red == _toolTipImageCacheLn2.red && ln2.green == _toolTipImageCacheLn2.green && ln2.blue == _toolTipImageCacheLn2.blue) &&
			toolTipGlobs.borderThickness == _toolTipImageCacheThickness)
		{
			// This is the same tooltip image as last time.
			Gods98::Image_Display(_toolTipImageCache, &destPos);
			return;
		}

		// We need to render a new tooltip image.
		bool needsClear = false;

		if (_toolTipImageCache != nullptr &&
			size.width  <= Gods98::Image_GetWidth(_toolTipImageCache) &&
			size.height <= Gods98::Image_GetHeight(_toolTipImageCache))
		{
			// The image size is large enough to fit our tooltip, no need to create a new one.
			prerendered = _toolTipImageCache;

			if (size.width < _toolTipImageCacheSize.width || size.height < _toolTipImageCacheSize.height) {
				// At least one dimension is smaller, so we need to clear the image.
				needsClear = true;
			}
		}
		else {
			needsClear = true;
			if (_toolTipImageCache != nullptr) {
				// The existing image size is too small, dispose of it and make a larger image.
				size.width  = std::max(size.width,  Gods98::Image_GetWidth(_toolTipImageCache));
				size.height = std::max(size.height, Gods98::Image_GetHeight(_toolTipImageCache));
				Gods98::Image_Remove(_toolTipImageCache);
			}
			prerendered = Gods98::Image_CreateNew(size.width, size.height);
		}

		// Pick a transparent colour that isn't rc, ln1, or ln2.
		const ColourRGBF colours[3] = { rc, ln1, ln2 };
		ColourRGBF trans;
		Gods98::Image_FindTransColour(prerendered, colours, _countof(colours), &trans);
		if (needsClear ||
			(trans.red   != _toolTipImageCacheTrans.red) ||
			(trans.green != _toolTipImageCacheTrans.green) ||
			(trans.blue  != _toolTipImageCacheTrans.blue))
		{
			Gods98::Image_ClearRGBF(prerendered, nullptr, trans.red, trans.green, trans.blue);
		}
		Gods98::Image_SetupTrans2(prerendered, trans.red, trans.green, trans.blue,
									trans.red, trans.green, trans.blue, false); // Don't truncate to 16-bit colour.

		_toolTipImageCache = prerendered;
		_toolTipImageCacheSize  = size;
		_toolTipImageCacheRc    = rc;
		_toolTipImageCacheLn1   = ln1;
		_toolTipImageCacheLn2   = ln2;
		_toolTipImageCacheTrans = trans;
		_toolTipImageCacheThickness = toolTipGlobs.borderThickness;
	}
	#endif

	// Setup outline rects.
	uint32 lineCount = 0;
	Point2F ln1ListFrom[10] = { 0.0f }; // dummy inits
	Point2F ln1ListTo[10]   = { 0.0f };
	Point2F ln2ListFrom[10] = { 0.0f };
	Point2F ln2ListTo[10]   = { 0.0f };

	// This uses the same border drawing method as: ObjInfo_DrawHealthBar
	// int2_8 is probably outline thickness.
	for (uint32 i = 0; i < toolTipGlobs.borderThickness; i++) {
		const Point2F start = {
			valueRect.x + static_cast<real32>(i),
			valueRect.y + static_cast<real32>(i),
		};
		const Point2F end = {
			((valueRect.x + valueRect.width)  - 1.0f) - static_cast<real32>(i),
			((valueRect.y + valueRect.height) - 1.0f) - static_cast<real32>(i),
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

	const Gods98::DrawEffect effect = (halfTrans ? Gods98::DrawEffect::HalfTrans : Gods98::DrawEffect::None);

	Gods98::Draw_Begin(prerendered); // Start of only Draw calls for tooltip box.
	Error_Warn(!Gods98::Draw_IsLocked(), "ToolTip_DrawBox: Draw_Begin() failed to lock the surface");

	if (prerendered != nullptr) {
		Gods98::Draw_SetTranslation(Point2F { -destPos.x, -destPos.y });
	}

	// Draw fill.
	Gods98::Draw_RectListEx(&valueRect, 1, rcRed, rcGreen, rcBlue, effect);
	// Draw outline.
	if (lineCount > 0) {
		Gods98::Draw_LineListEx(ln1ListFrom, ln1ListTo, lineCount, ln1Red, ln1Green, ln1Blue, effect);
		Gods98::Draw_LineListEx(ln2ListFrom, ln2ListTo, lineCount, ln2Red, ln2Green, ln2Blue, effect);
	}

	Gods98::Draw_End(); // End of only Draw calls for tooltip box.

	if (prerendered != nullptr) {
		Gods98::Image_Display(prerendered, &destPos);
	}
}

#pragma endregion
