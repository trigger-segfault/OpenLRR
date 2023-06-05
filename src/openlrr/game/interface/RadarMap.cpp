// RadarMap.cpp : 
//

#include "../../engine/core/Maths.h"
#include "../../engine/core/Memory.h"
#include "../../engine/drawing/DirectDraw.h"
#include "../../engine/drawing/Draw.h"
#include "../../engine/Main.h"

#include "../interface/Interface.h"
#include "../object/Object.h"
#include "../object/Stats.h"
#include "../world/Map3D.h"
#include "../Game.h"

#include "RadarMap.h"


/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum_scoped(Radar_Colour) : uint32
{
	Reinforcement    = 0, // Border drawn around block.
	Cavern           = 1,
	Water            = 2,
	Lava             = 3,
	Soil_Hidden      = 4,
	Loose_Hidden     = 5,
	Medium_Hidden    = 6,
	Hard_Hidden      = 7,
	Immovable_Hidden = 8,
	Soil             = 9,
	Loose            = 10,
	Medium           = 11,
	Hard             = 12,
	Immovable        = 13,
	FriendlyUnit     = 14,
	EnemyUnit        = 15,
	Resource         = 16,
	Soil_Vertex      = 17,
	Loose_Vertex     = 18,
	Medium_Vertex    = 19,
	Hard_Vertex      = 20,
	Immovable_Vertex = 21,
	PoweredPath      = 22,
	UnpoweredPath    = 23,
	OreSeam          = 24,
	CrystalSeam      = 25,
	RechargeSeam     = 26,

	Lake             = 27,
	Foundation       = 28,
	Cavern_Hidden    = 29,

	// Shared:
	Water_Hidden        = Water,
	Lava_Hidden         = Lava,
	Lake_Hidden         = Lake,
	OreSeam_Hidden      = OreSeam,
	CrystalSeam_Hidden  = CrystalSeam,
	RechargeSeam_Hidden = RechargeSeam,

	// Alteration: Add options for colouring these block types.
	//Rubble              = Cavern,
	//Erosion             = Cavern,
};
enum_scoped_end(Radar_Colour, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

// Colour to assign to exposed walls that aren't handled.
static constexpr const ColourRGBF RADARMAP_UNHANDLED_WALLCOLOUR = { 1.0f, 1.0f, 1.0f };

// Colour to assign to hidden walls that aren't handled.
static constexpr const ColourRGBF RADARMAP_UNHANDLED_HIDDENCOLOUR = { 1.0f, 1.0f, 1.0f };

#define RADARMAP_MAXDRAWRECTS		1500

#define RADARMAP_USERENDERTARGET	true

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004a9f28>
LegoRR::RadarMap_Globs & LegoRR::radarmapGlobs = *(LegoRR::RadarMap_Globs*)0x004a9f28;

// Allocated size of drawRects array.
static uint32 _radarMapCurrMaxDrawRects = 0;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/// CUSTOM: Gets the drawing API scale used by the radar map.
sint32 LegoRR::RadarMap_GetDrawScale()
{
	return Gods98::Main_RadarMapScale();
}

/// CUSTOM:
void LegoRR::RadarMap_BeginDraw(RadarMap* radarMap)
{
	Size2I size = {
		static_cast<sint32>(radarMap->screenRect.width  * _RadarMap_GetTransformScale()),
		static_cast<sint32>(radarMap->screenRect.height * _RadarMap_GetTransformScale()),
	};

	// Check if we need to create or resize our render target.
	// Note: Change define to false to disable render target-based drawing.
	#if RADARMAP_USERENDERTARGET
	if (radarMap->renderTarget == nullptr ||
		size.width  != Gods98::Image_GetWidth(radarMap->renderTarget) ||
		size.height != Gods98::Image_GetHeight(radarMap->renderTarget))
	{
		if (radarMap->renderTarget != nullptr) {
			Gods98::Image_Remove(radarMap->renderTarget);
			radarMap->renderTarget = nullptr;
		}

		radarMap->renderTarget = Gods98::Image_CreateNew(size.width, size.height);
	}
	#endif

	// Clear the radar screen to black.
	if (radarMap->renderTarget != nullptr) {
		Gods98::Image_ClearRGBF(radarMap->renderTarget, nullptr, 0.0f, 0.0f, 0.0f);
	}
	else {
		Gods98::DirectDraw_ClearRGBF(&radarMap->screenRect, 0.0f, 0.0f, 0.0f);
	}

	Gods98::Draw_Begin(radarMap->renderTarget);
	Error_Warn(!Gods98::Draw_IsLocked(), "RadarMap_BeginDraw: Draw_Begin() failed to lock the surface");

	if (radarMap->renderTarget != nullptr) {
		Gods98::Draw_SetTranslation(Point2F {
			-radarMap->screenRect.x * _RadarMap_GetTransformScale(),
			-radarMap->screenRect.y * _RadarMap_GetTransformScale(),
		}); // Translate to 0,0.
	}
	else {
		Gods98::Draw_SetScale(RadarMap_GetDrawScale(), false);
	}
}

/// CUSTOM:
void LegoRR::RadarMap_EndDraw(RadarMap* radarMap)
{
	Gods98::Draw_End();

	if (radarMap->renderTarget != nullptr) {
		const Point2F destPos = {
			radarMap->screenRect.x * _RadarMap_GetTransformScale(),
			radarMap->screenRect.y * _RadarMap_GetTransformScale(),
		};
		Gods98::Image_DisplayScaled2(radarMap->renderTarget, nullptr, &destPos, nullptr, RadarMap_GetDrawScale());
	}
}

/// CUSTOM: Gets the scale that radar screen points are transformed by.
real32 LegoRR::_RadarMap_GetTransformScale()
{
	return static_cast<real32>(Gods98::Main_RenderScale()) / RadarMap_GetDrawScale();
}

const Area2F* LegoRR::_RadarMap_ScaledScreenRect(const RadarMap* radarMap)
{
	static Area2F rect = { 0.0f };
	rect = radarMap->screenRect;

	rect.x      *= _RadarMap_GetTransformScale();
	rect.y      *= _RadarMap_GetTransformScale();
	rect.width  *= _RadarMap_GetTransformScale();
	rect.height *= _RadarMap_GetTransformScale();

	return &rect;
}

real32 LegoRR::_RadarMap_ScaledZoom(const RadarMap* radarMap)
{
	return (radarMap->zoom * _RadarMap_GetTransformScale());
}



/// CUSTOM:
void LegoRR::RadarMap_ClearHighlightBlock()
{
	radarmapGlobs.highlightBlockPos = Point2I { -1, -1 };
}

/// CUSTOM:
void LegoRR::RadarMap_SetHighlightBlock(OPTIONAL const Point2I* blockPos)
{
	if (blockPos != nullptr) {
		radarmapGlobs.highlightBlockPos = *blockPos;
	}
	else {
		RadarMap_ClearHighlightBlock();
	}
}

/// CUSTOM:
real32 LegoRR::RadarMap_GetZoom(const RadarMap* radarMap)
{
	return radarMap->zoom;
}



// <LegoRR.exe @0045db60>
void __cdecl LegoRR::RadarMap_SetZoom(RadarMap* radarMap, real32 zoom)
{
	radarMap->zoom = zoom;
}

// <LegoRR.exe @0045db70>
void __cdecl LegoRR::RadarMap_Initialise(void)
{
	// Colours are stored at compile-time in [0.0f-255.0f] real channel values, convert them to [0.0f-1.0f] real values.
	for (uint32 i = 0; i < RADARMAP_MAXTABLECOLOURS; i++) {
		radarmapGlobs.colourTable[i].red   /= 255.0f;
		radarmapGlobs.colourTable[i].green /= 255.0f;
		radarmapGlobs.colourTable[i].blue  /= 255.0f;
	}

	// Do these have something to do with drawing the TV view arrow on the radar map??
	/*for (uint32 i = 0; i < _countof(radarmapGlobs.arrowPointsFrom); i++) {
		radarmapGlobs.arrowPointsFrom[i].w = 1.0f;
		radarmapGlobs.arrowPointsTo[i].w = 1.0f;
	}*/

	/// REFACTOR: Don't manually assign arrowPointsTo, because they're the same (just offset by 1).
	// These are units defined in 640x480, and then transformed into [0,1] range.
	radarmapGlobs.arrowPointsFrom[0] = Vector4F { 320.0f,  40.0f,  0.0f, 1.0f };
	radarmapGlobs.arrowPointsFrom[1] = Vector4F { 420.0f, 180.0f,  0.0f, 1.0f };
	radarmapGlobs.arrowPointsFrom[2] = Vector4F { 360.0f, 180.0f,  0.0f, 1.0f };
	radarmapGlobs.arrowPointsFrom[3] = Vector4F { 360.0f, 400.0f,  0.0f, 1.0f };
	radarmapGlobs.arrowPointsFrom[4] = Vector4F { 280.0f, 400.0f,  0.0f, 1.0f };
	radarmapGlobs.arrowPointsFrom[5] = Vector4F { 280.0f, 180.0f,  0.0f, 1.0f };
	radarmapGlobs.arrowPointsFrom[6] = Vector4F { 220.0f, 180.0f,  0.0f, 1.0f };
	radarmapGlobs.arrowPointCount = 7;

	// Convert 640x480 units to [0,1] range.
	for (uint32 i = 0; i < radarmapGlobs.arrowPointCount; i++) {
		radarmapGlobs.arrowPointsFrom[i].x *= static_cast<real32>(Gods98::appWidth())  / 640.0f;
		radarmapGlobs.arrowPointsFrom[i].y *= static_cast<real32>(Gods98::appHeight()) / 480.0f;
	}
	/// REFACTOR: Assign arrowPointsTo now.
	for (uint32 i = 0; i < radarmapGlobs.arrowPointCount; i++) {
		radarmapGlobs.arrowPointsTo[i] = radarmapGlobs.arrowPointsFrom[(i+1) % radarmapGlobs.arrowPointCount];
	}

	// Never assigned elsewhere, maybe an editor feature?
	RadarMap_ClearHighlightBlock();
	//radarmapGlobs.highlightBlockPos = Point2I { -1, -1 };
}

// <LegoRR.exe @0045dd50>
LegoRR::RadarMap* __cdecl LegoRR::RadarMap_Create(Map3D* map, const Area2F* screenRect, real32 zoom)
{
	RadarMap* radarMap = (RadarMap*)Gods98::Mem_Alloc(sizeof(RadarMap));
	if (radarMap != nullptr) {
		std::memset(radarMap, 0, sizeof(RadarMap));

		radarMap->screenRect = *screenRect;
		radarMap->zoom = zoom;
		radarMap->blockSize = Map3D_BlockSize(map);
		radarMap->map = map;
		return radarMap;
	}
	return nullptr;
}

// <LegoRR.exe @0045ddb0>
void __cdecl LegoRR::RadarMap_Free(RadarMap* radarMap)
{
	if (radarMap->renderTarget != nullptr) {
		Gods98::Image_Remove(radarMap->renderTarget);
		radarMap->renderTarget = nullptr;
	}

	Gods98::Mem_Free(radarMap);
}

// <LegoRR.exe @0045ddc0>
void __cdecl LegoRR::RadarMap_DrawSurveyDotCircle(RadarMap* radarMap, const Point2F* center, real32 radius, real32 brightness)
{
	Area2F oldClipWindow;
	Gods98::Draw_GetClipWindow(&oldClipWindow);

	if (radarMap->renderTarget == nullptr) {
		// We only need to clip if drawing to the back surface.
		Gods98::Draw_SetClipWindow(_RadarMap_ScaledScreenRect(radarMap));
	}

	// This uses TransformRect for a circle, because it gets the same results that are needed.
	Area2F rect = {
		center->x,
		center->y,
		radius,
		0.0f,
	};
	RadarMap_TransformRect(radarMap, &rect);

	const real32 baseChannel = 0.3f + (brightness * 0.7f); // Minimum of 0.3f with range of [0.3,1.0].
	const ColourRGBF rgb = { // Max of rgb(179,230,255).
		baseChannel * 0.7f,
		baseChannel * 0.9f,
		baseChannel * 1.0f,
	};
	Gods98::Draw_DotCircle(&rect.point, static_cast<uint32>(rect.width), (static_cast<uint32>(rect.width) * 2),
						   rgb.red, rgb.green, rgb.blue, Gods98::DrawEffect::None);

	Gods98::Draw_SetClipWindow(&oldClipWindow);
}

// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @0045de80>
void __cdecl LegoRR::RadarMap_Draw(RadarMap* radarMap, const Point2F* centerPos)
{
	Area2F oldClipWindow = { 0.0f }; // dummy init
	Gods98::Draw_GetClipWindow(&oldClipWindow);

	if (radarMap->renderTarget == nullptr) {
		// We only need to clip if drawing to the back surface.
		Gods98::Draw_SetClipWindow(_RadarMap_ScaledScreenRect(radarMap));
	}

	radarMap->centerPos = *centerPos;

	uint32 reinforceLineCount = 0;
	Point2F reinforceListTo[200] = { 0.0f }; // dummy init
	Point2F reinforceListFrom[200] = { 0.0f }; // dummy init

	const Vector3F worldRadius = {
		((radarMap->screenRect.width  * _RadarMap_GetTransformScale() * 0.5f) * (radarMap->blockSize / _RadarMap_ScaledZoom(radarMap))),
		((radarMap->screenRect.height * _RadarMap_GetTransformScale() * 0.5f) * (radarMap->blockSize / _RadarMap_ScaledZoom(radarMap))),
	};
	// Probably function to get block coordinates unbounded.
	Point2I blockStart = { 0 }, blockEnd = { 0 }; // dummy inits
	Map3D_FUN_0044fad0(radarMap->map, centerPos->x - worldRadius.x, centerPos->y + worldRadius.y, &blockStart.x, &blockStart.y);
	Map3D_FUN_0044fad0(radarMap->map, centerPos->x + worldRadius.x, centerPos->y - worldRadius.y, &blockEnd.x, &blockEnd.y);

	/// FIX APPLY: Allocate array that always has room for at least gridSize blocks.
	const sint32 gridWidth  = blockEnd.x - blockStart.x + 1;
	const sint32 gridHeight = blockEnd.y - blockStart.y + 1;
	const uint32 rectCount = static_cast<uint32>(std::max(RADARMAP_MAXDRAWRECTS, (gridWidth + gridHeight)));
	_radarMapCurrMaxDrawRects = rectCount;
	/// CHANGE: Allocate rectList because it takes up way too much stack space.
	uint32 blockCount = 0;
	Gods98::Draw_Rect* rectList = static_cast<Gods98::Draw_Rect*>(Gods98::Mem_Alloc(rectCount * sizeof(Gods98::Draw_Rect)));
	//Gods98::Draw_Rect rectList[RADARMAP_MAXDRAWRECTS] = { 0 }; // dummy init


	for (sint32 by = blockStart.y; by <= blockEnd.y; by++) {
		for (sint32 bx = blockStart.x; bx <= blockEnd.x; bx++) {
			/// SANITY: It's not possible to hit the block limit based on the hardcoded dimensions, but bounds check anyway.
			if (blockCount >= rectCount) //RADARMAP_MAXDRAWRECTS)
				break;

			Gods98::Draw_Rect* drawRect = &rectList[blockCount];
			if (!RadarMap_GetBlockColour(drawRect, bx, by))
				continue;

			blockCount++;

			// This interface boolean is toggled on/off every 15-frames. So this is a flashing block.
			if (Interface_GetFlashingState() &&
				(bx == radarmapGlobs.highlightBlockPos.x && by == radarmapGlobs.highlightBlockPos.y))
			{
				drawRect->colour = ColourRGBF { 1.0f, 0.0f, 0.0f };
			}

			//Map3D* map = radarMap->map;
			drawRect->rect.x = radarMap->map->worldDimensions_fnegx.width  + (static_cast<real32>(bx) * radarMap->blockSize);
			drawRect->rect.y = radarMap->map->worldDimensions_fnegx.height - (static_cast<real32>(by) * radarMap->blockSize);
			drawRect->rect.width  = radarMap->blockSize;
			drawRect->rect.height = radarMap->blockSize;
				
			RadarMap_TransformRect(radarMap, &drawRect->rect);

			// Radar blocks always have a 1 pixel gap between them.
			drawRect->rect.width  -= 1.0f;
			drawRect->rect.height -= 1.0f;

			if (Level_Block_IsReinforced(bx, by) && reinforceLineCount < 200) {
				Point2F* fromPos = &reinforceListFrom[reinforceLineCount];
				Point2F* toPos   = &reinforceListTo[reinforceLineCount];

				reinforceLineCount += 4;

				// 0 ____ 1
				//  |    |
				//  |____|
				// 3      2

				const Point2F endPos = {
					(drawRect->rect.x + (drawRect->rect.width  - 1.0f)),
					(drawRect->rect.y + (drawRect->rect.height - 1.0f)),
				};

				fromPos[0] = Point2F { drawRect->rect.x, drawRect->rect.y };
				fromPos[1] = Point2F {         endPos.x, drawRect->rect.y };
				fromPos[2] = Point2F {         endPos.x,         endPos.y };
				fromPos[3] = Point2F { drawRect->rect.x,         endPos.y };

				for (uint32 i = 0; i < 4; i++) {
					toPos[i] = fromPos[(i+1) % 4];
				}
			}
		}
	}

	/// CHANGE: Moved to RadarMap_ClearScreen, and must be called beforehand.
	// Clear the radar screen to black.
	//Gods98::DirectDraw_Clear(&radarMap->screenRect, 0); // black

	// Draw block squares.
	Gods98::Draw_RectList2Ex(rectList, blockCount, Gods98::DrawEffect::None);

	// Draw reinforcement yellow outlines around blocks.
	const ColourRGBF reinforce = radarmapGlobs.colourTable[Radar_Colour::Reinforcement];
	Gods98::Draw_LineListEx(reinforceListFrom, reinforceListTo, reinforceLineCount, reinforce.red, reinforce.green, reinforce.blue, Gods98::DrawEffect::None);

	/// TODO: Is this addition/subtraction correct??
	radarMap->worldRect.left   = centerPos->x - worldRadius.x;
	radarMap->worldRect.top    = centerPos->y - worldRadius.y;
	radarMap->worldRect.right  = centerPos->x + worldRadius.x;
	radarMap->worldRect.bottom = centerPos->y + worldRadius.y;

	// Draw units.
	radarMap->drawRectList = rectList;
	radarMap->drawRectCount = 0;

	for (LegoObject* obj : objectListSet.EnumerateSkipUpgradeParts()) {
		RadarMap_Callback_AddObjectDrawRect(obj, radarMap);
	}
	//LegoObject_RunThroughListsSkipUpgradeParts(RadarMap_Callback_AddObjectDrawRect, radarMap);
	Gods98::Draw_RectList2Ex(radarMap->drawRectList, radarMap->drawRectCount, Gods98::DrawEffect::None);

	/// SANITY: Nullify pointer that would lead to nowhere after function call.
	radarMap->drawRectList = nullptr;


	/// CHANGE: Done using allocated rectList.
	Gods98::Mem_Free(rectList);
	rectList = nullptr;


	// Draw the TV camera arrow, and box around the arrow.
	if (Lego_GetViewMode() == ViewMode_Top) {

		// Calculate stuff for the surrounding box.
		const real32 width  = static_cast<real32>(Gods98::appWidth());
		const real32 height = static_cast<real32>(Gods98::appHeight());
		const Vector4F TRANSFORM_4D[4*2] = {
			{  0.0f,   0.0f, 0.0f, 1.0f },
			{ width,   0.0f, 0.0f, 1.0f },
			{ width, height, 0.0f, 1.0f },
			{  0.0f, height, 0.0f, 1.0f },

			{  0.0f,   0.0f, 1.0f, 1.0f },
			{ width,   0.0f, 1.0f, 1.0f },
			{ width, height, 1.0f, 1.0f },
			{  0.0f, height, 1.0f, 1.0f },
		};
		Vector3F transform3d[4*2] = { 0.0f }; // dummy init
		for (uint32 i = 0; i < 4*2; i++) {
			Gods98::Viewport_InverseTransform(legoGlobs.viewMain, &transform3d[i], &TRANSFORM_4D[i]);
		}


		/// REFACTOR: Just use the map passed to RadarMap_Create, no need to call Lego_GetMap().
		//Map3D* map = Lego_GetMap();
		const Vector3F planePoint = {
			0.0f,
			0.0f,
			(radarMap->map->float_20 * 0.5f),
		};
		const Vector3F planeNormal = {
			0.0f,
			0.0f,
			-1.0f,
		};


		// Draw the arrow.
		Vector3F transform3dFromTo[2][2][RADARMAP_MAXARROWPOINTS] = { 0.0f }; // dummy init

		for (uint32 i = 0; i < 2; i++) {
			for (uint32 j = 0; j < radarmapGlobs.arrowPointCount; j++) {
				if (i == 0) {
					radarmapGlobs.arrowPointsFrom[j].z = 0.0f;
					radarmapGlobs.arrowPointsTo[j].z   = 0.0f;
				}
				else {
					radarmapGlobs.arrowPointsFrom[j].z = 1.0f;
					radarmapGlobs.arrowPointsTo[j].z   = 1.0f;
				}

				Gods98::Viewport_InverseTransform(legoGlobs.viewMain, &transform3dFromTo[i][0][j], &radarmapGlobs.arrowPointsFrom[j]);
				Gods98::Viewport_InverseTransform(legoGlobs.viewMain, &transform3dFromTo[i][1][j], &radarmapGlobs.arrowPointsTo[j]);
			}
		}

		Point2F arrowListFrom[RADARMAP_MAXARROWPOINTS] = { 0.0f }; // dummy init
		Point2F arrowListTo[RADARMAP_MAXARROWPOINTS] = { 0.0f }; // dummy init

		for (uint32 i = 0; i < 2; i++) {
			for (uint32 j = 0; j < radarmapGlobs.arrowPointCount; j++) {

				/// NOTE: Unlike the above loop, i and 0,1 indexes are swapped.
				Vector3F ray = { 0.0f }; // dummy init
				Gods98::Maths_Vector3DSubtract(&ray, &transform3dFromTo[1][i][j], &transform3dFromTo[0][i][j]);

				Vector3F endPoint = { 0.0f }; // dummy init
				Gods98::Maths_RayPlaneIntersection(&endPoint, &transform3dFromTo[0][i][j], &ray, &planePoint, &planeNormal);

				Area2F endPointRect = { // Rect used solely for transform function.
					endPoint.x,
					endPoint.y,
					0.0f, // Dimensions are not needed.
					0.0f,
				};
				RadarMap_TransformRect(radarMap, &endPointRect);
				if (i == 0)
					arrowListFrom[j] = endPointRect.point;
				else
					arrowListTo[j] = endPointRect.point;
			}
		}

		Gods98::Draw_LineListEx(arrowListFrom, arrowListTo, radarmapGlobs.arrowPointCount, 1.0f, 1.0f, 1.0f, Gods98::DrawEffect::HalfTrans);


		// Draw the box.
		Point2F boxListFromTo[5] = { 0.0f }; // dummy init

		for (uint32 i = 0; i < 4; i++) {

			/// NOTE: Unlike the above loop, i and 0,1 indexes are swapped.
			Vector3F ray = { 0.0f }; // dummy init
			Gods98::Maths_Vector3DSubtract(&ray, &transform3d[i + 4], &transform3d[i + 0]);

			Vector3F endPoint = { 0.0f }; // dummy init
			Gods98::Maths_RayPlaneIntersection(&endPoint, &transform3d[i + 0], &ray, &planePoint, &planeNormal);

			Area2F endPointRect = { // Rect used solely for transform function.
				endPoint.x,
				endPoint.y,
				0.0f, // Dimensions are not needed.
				0.0f,
			};
			RadarMap_TransformRect(radarMap, &endPointRect);

			boxListFromTo[i] = endPointRect.point;
		}
		boxListFromTo[4] = boxListFromTo[0];
		Gods98::Draw_LineListEx(boxListFromTo, (boxListFromTo + 1), 4, 0.7f, 0.7f, 0.7f, Gods98::DrawEffect::HalfTrans);
	}
	Gods98::Draw_SetClipWindow(&oldClipWindow);
}

/// CUSTOM: Isolate Draw API calls from RadarMap_Draw.
void __cdecl LegoRR::RadarMap_ClearScreen(RadarMap* radarMap)
{
	// Clear the radar screen to black.
	Gods98::DirectDraw_ClearRGB(&radarMap->screenRect, 0, 0, 0); // black
}

// <LegoRR.exe @0045e6c0>
bool32 __cdecl LegoRR::RadarMap_CanShowObject(LegoObject* liveObj)
{
	switch (liveObj->type) {
	case LegoObject_Building:
	case LegoObject_Boulder:
	case LegoObject_Dynamite:
	case LegoObject_Barrier:
	case LegoObject_SpiderWeb:
	case LegoObject_OohScary:
	case LegoObject_ElectricFenceStud:
	case LegoObject_Pusher:
	case LegoObject_Freezer:
	case LegoObject_IceCube:
	case LegoObject_LaserShot:
		return false;

	case LegoObject_Vehicle:
	case LegoObject_MiniFigure:
	case LegoObject_RockMonster:
	case LegoObject_PowerCrystal:
	case LegoObject_Ore:
	//case LegoObject_UpgradePart: // Upgrade parts are skipped in enumeration.
	case LegoObject_ElectricFence:
	//case LegoObject_Path: // Paths are not real objects.
	default:
		const StatsFlags2 sflags2 = StatsObject_GetStatsFlags2(liveObj);
		return !(sflags2 & STATS2_DONTSHOWONRADAR);
	}
}

// DATA: RadarMap* radarMap
// <LegoRR.exe @0045e720>
bool32 __cdecl LegoRR::RadarMap_Callback_AddObjectDrawRect(LegoObject* liveObj, void* pRadarMap)
{
	RadarMap* radarMap = static_cast<RadarMap*>(pRadarMap);

	/// FIX APPLY: Bounds check max draw rects.
	if (radarMap->drawRectCount >= _radarMapCurrMaxDrawRects) //RADARMAP_MAXDRAWRECTS)
		return false;

	if (!RadarMap_CanShowObject(liveObj))
		return false;

	Point2F wPos = { 0.0f }; // dummy init
	LegoObject_GetPosition(liveObj, &wPos.x, &wPos.y);

	if (RadarMap_InsideRadarWorld(radarMap, &wPos)) {
		Gods98::Draw_Rect* drawRect = &radarMap->drawRectList[radarMap->drawRectCount];

		real32 size;
		if (liveObj->type == LegoObject_PowerCrystal || liveObj->type == LegoObject_Ore) {
			// Resource.
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Resource];
			size = (1.0f / 4.0f); // Smaller size than friendly/monster units.
		}
		else if (liveObj->type == LegoObject_RockMonster) {
			// Monster unit.
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::EnemyUnit];
			size = (1.0f / 3.0f);
		}
		else {
			// Friendly unit (Vehicle, MiniFigure, ElectricFence).
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::FriendlyUnit];
			size = (1.0f / 3.0f);
		}

		drawRect->rect.x = wPos.x - (size * 0.5f) * radarMap->blockSize;
		drawRect->rect.y = wPos.y + (size * 0.5f) * radarMap->blockSize;
		/// CHANGE: Use minimum size of 1 pixel so that units are always visible.
		drawRect->rect.width  = std::max(1.0f, size * radarMap->blockSize);
		drawRect->rect.height = std::max(1.0f, size * radarMap->blockSize);

		RadarMap_TransformRect(radarMap, &drawRect->rect);

		radarMap->drawRectCount++;
	}
	return false;
}

/// CUSTOM:
bool LegoRR::RadarMap_InsideRadarWorld(const RadarMap* radarMap, const Point2F* wPos)
{
	// Unlike InsideRadarScreen, this doesn't add x,y, and uses `<=` instead of `<`.
	return ((wPos->x >= radarMap->worldRect.left && wPos->x <= radarMap->worldRect.right) &&
			(wPos->y >= radarMap->worldRect.top  && wPos->y <= radarMap->worldRect.bottom));
}

// <LegoRR.exe @0045e920>
bool32 __cdecl LegoRR::RadarMap_InsideRadarScreen(const RadarMap* radarMap, uint32 mouseX, uint32 mouseY)
{
	return ((mouseX >= radarMap->screenRect.x && mouseX < (radarMap->screenRect.x + radarMap->screenRect.width)) &&
			(mouseY >= radarMap->screenRect.y && mouseY < (radarMap->screenRect.y + radarMap->screenRect.height)));
}

// <LegoRR.exe @0045e990>
bool32 __cdecl LegoRR::RadarMap_ScreenToWorldBlockPos(RadarMap* radarMap, uint32 mouseX, uint32 mouseY, OPTIONAL OUT Point2F* worldPos, OUT sint32* bx, OUT sint32* by)
{
	if (RadarMap_InsideRadarScreen(radarMap, mouseX, mouseY)) {

		/// CHANGE: Properly use screenRect.height for other center maybe?? Before only width was used for both.
		const real32 centerX = radarMap->screenRect.width  * 0.5f;
		const real32 centerY = radarMap->screenRect.height * 0.5f;

		Point2F wPos = Point2F { 0.0f, 0.0f };
		if (radarMap->zoom != 0.0f) {
			wPos = Point2F {
				radarMap->centerPos.x - (((radarMap->screenRect.x + centerX) - mouseX) * _RadarMap_GetTransformScale() * (radarMap->blockSize / _RadarMap_ScaledZoom(radarMap))),
				radarMap->centerPos.y + (((radarMap->screenRect.y + centerY) - mouseY) * _RadarMap_GetTransformScale() * (radarMap->blockSize / _RadarMap_ScaledZoom(radarMap))),
			};
		}

		if (Map3D_WorldToBlockPos_NoZ(radarMap->map, wPos.x, wPos.y, bx, by)) {

			if (worldPos) *worldPos = wPos;
			return true;
		}
	}
	return false;
}

// <LegoRR.exe @0045eae0>
bool32 __cdecl LegoRR::RadarMap_TrySelectObject(RadarMap* radarMap, uint32 mouseX, uint32 mouseY, OUT LegoObject** liveObj, OPTIONAL OUT Point2F* objPos)
{
	SearchRadarObjectInArea search = { 0 };
	search.object = nullptr;

	Point2I blockPos = { 0 }; // dummy init
	if (RadarMap_ScreenToWorldBlockPos(radarMap, mouseX, mouseY, &search.worldPos, &blockPos.x, &blockPos.y)) {
		if (!Level_Block_IsSolidBuilding(blockPos.x, blockPos.y, true)) {
			search.radius = radarMap->blockSize / 6.0f; // Other units take up a smaller size.
		}
		else {
			search.radius = radarMap->blockSize; // Building units cover entire tile.
		}

		for (LegoObject* obj : objectListSet.EnumerateSkipUpgradeParts()) {
			if (RadarMap_Callback_FindObjectInClickArea(obj, &search)) {
				*liveObj = search.object;
				if (objPos) {
					*objPos = Point2F { 0.0f, 0.0f }; // Set to zero before calling GetPosition.
					LegoObject_GetPosition(search.object, &objPos->x, &objPos->y);
				}
				return true;
			}
		}
	}
	return false;
}

// DATA: const SearchRadarObjectInArea* search
// <LegoRR.exe @0045eba0>
bool32 __cdecl LegoRR::RadarMap_Callback_FindObjectInClickArea(LegoObject* liveObj, void* pSearch)
{
	SearchRadarObjectInArea* search = static_cast<SearchRadarObjectInArea*>(pSearch);

	Point2F wPos = { 0.0f }; // dummy init
	LegoObject_GetPosition(liveObj, &wPos.x, &wPos.y);

	if ((std::abs(wPos.x - search->worldPos.x) <= search->radius) &&
		(std::abs(wPos.y - search->worldPos.y) <= search->radius))
	{
		search->object = liveObj;
		return true;
	}
	return false;
}

// <LegoRR.exe @0045ec00>
void __cdecl LegoRR::RadarMap_TransformRect(const RadarMap* radarMap, IN OUT Area2F* rect)
{
	// Scale around the center of the radar map view (origin).
	// rect.point -= centerPos;
	Gods98::Maths_Vector2DSubtract(&rect->point, &rect->point, &radarMap->centerPos);

	// rect *= (zoom / blockSize);
	const real32 scalar = (_RadarMap_ScaledZoom(radarMap) / radarMap->blockSize);
	Gods98::Maths_Vector2DScale(&rect->point, &rect->point, scalar);
	Gods98::Maths_Vector2DScale(&rect->size.vec2, &rect->size.vec2, scalar);

	// Move position from transformed origin to relative to the screen center.
	// rect.point = screenRect.point + (screenRect.size * 0.5f) + {rect.x, -rect.y}
	rect->x = (radarMap->screenRect.x + (radarMap->screenRect.width  * 0.5f)) * _RadarMap_GetTransformScale() + rect->x;
	rect->y = (radarMap->screenRect.y + (radarMap->screenRect.height * 0.5f)) * _RadarMap_GetTransformScale() - rect->y;
}

// <LegoRR.exe @0045eca0>
bool32 __cdecl LegoRR::RadarMap_GetBlockColour(IN OUT Gods98::Draw_Rect* drawRect, uint32 bx, uint32 by)
{
	const Point2I blockPos = { static_cast<sint32>(bx), static_cast<sint32>(by) };
	Lego_SurfaceType terrain = (Lego_SurfaceType)0; // dummy init
	Level_Block_GetSurfaceType(bx, by, &terrain); // &bx);

	if (Level_Block_IsWall(bx, by)) {
		// Exposed wall blocks:

		if (Level_Block_IsDestroyedConnection(bx, by)) {
			// Blocks dug using the unused Vertex Mode SingleWidthDig feature.
			switch (terrain) {
			case Lego_SurfaceType_Immovable:
				drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Immovable_Vertex];
				return true;

			case Lego_SurfaceType_Hard:
				drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Hard_Vertex];
				return true;

			case Lego_SurfaceType_Medium:
				drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Medium_Vertex];
				return true;

			case Lego_SurfaceType_Loose:
				drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Loose_Vertex];
				return true;

			case Lego_SurfaceType_Soil:
				drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Soil_Vertex];
				return true;
			}
			/// CHANGE: Fallthrough into normal block colours.
		}

		switch (terrain) {
		case Lego_SurfaceType_Immovable:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Immovable];
			return true;

		case Lego_SurfaceType_Hard:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Hard];
			return true;

		case Lego_SurfaceType_Medium:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Medium];
			return true;

		case Lego_SurfaceType_Loose:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Loose];
			return true;

		case Lego_SurfaceType_Soil:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Soil];
			return true;

		case Lego_SurfaceType_OreSeam:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::OreSeam];
			return true;

		case Lego_SurfaceType_CrystalSeam:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::CrystalSeam];
			return true;

		case Lego_SurfaceType_RechargeSeam:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::RechargeSeam];
			return true;


		/// FIX APPLY: Handle unused water blocks attached to sides of walls(?)
		case Lego_SurfaceType_Water:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Water];
			return true;


		default:
			/// FIX APPLY: Assign default colour for unhandled blocks!
			drawRect->colour = RADARMAP_UNHANDLED_WALLCOLOUR;
			return true;
		}
	}
	else if (Level_Block_IsNotWallOrGround(bx, by) && Level_Block_IsSurveyed(bx, by)) {
		// Hidden blocks: (note hidden blocks do not have floor or wall flags set)

		if (blockValue(Lego_GetLevel(), bx, by).flags1 & BLOCK1_HIDDEN) {
			// Apparently this flag is only used for undiscovered caverns...
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Cavern_Hidden];
			return true;
		}

		switch (terrain) {
		case Lego_SurfaceType_Immovable:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Immovable_Hidden];
			return true;

		case Lego_SurfaceType_Hard:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Hard_Hidden];
			return true;

		case Lego_SurfaceType_Medium:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Medium_Hidden];
			return true;

		case Lego_SurfaceType_Loose:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Loose_Hidden];
			return true;

		case Lego_SurfaceType_Soil:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Soil_Hidden];
			return true;


		case Lego_SurfaceType_OreSeam:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::OreSeam_Hidden];
			return true;

		case Lego_SurfaceType_CrystalSeam:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::CrystalSeam_Hidden];
			return true;

		case Lego_SurfaceType_RechargeSeam:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::RechargeSeam_Hidden];
			return true;


		/// NOTE: These normally won't show because the hidden cavern flag will be set, which is caught before the switch.
		case Lego_SurfaceType_Lava:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Lava_Hidden];
			return true;

		case Lego_SurfaceType_Water:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Water_Hidden];
			return true;

		case Lego_SurfaceType_Lake:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Lake_Hidden];
			return true;


		default:
			/// FIX APPLY: Assign default colour for unhandled blocks!
			drawRect->colour = RADARMAP_UNHANDLED_HIDDENCOLOUR;
			return true;
		}
	}
	else if (Level_Block_IsGround(bx, by)) {
		// Exposed floor blocks:

		switch (terrain) {
		case Lego_SurfaceType_Lava:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Lava];
			return true;

		case Lego_SurfaceType_Water:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Water];
			return true;

		case Lego_SurfaceType_Lake:
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Lake];
			return true;
		}

		if (blockValue(Lego_GetLevel(), bx, by).flags1 & BLOCK1_FOUNDATION) {
			drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Foundation];
			return true;
		}

		if (Level_Block_IsPath(&blockPos)) {
			if (Level_Block_IsPowered(&blockPos)) {
				drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::PoweredPath];
				return true;
			}
			else {
				drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::UnpoweredPath];
				return true;
			}
		}

		// Default to cavern floor if nothing else fits.
		drawRect->colour = radarmapGlobs.colourTable[Radar_Colour::Cavern];
		return true;
	}
	else {
		return false;
	}

	/// CHANGE: Return false if we haven't stopped that from happening in any of the if blocks.
	return false;
}

#pragma endregion
