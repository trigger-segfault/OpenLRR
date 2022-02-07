// Camera.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Images.h"

#include "../object/Object.h"

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum LegoCamera_Type : sint32 // [LegoRR/LegoCamera.c|enum:0x4|type:int]
{
	LegoCamera_None  = 0,
	LegoCamera_Top   = 1,
	LegoCamera_Radar = 2,
	LegoCamera_FP    = 3,

	LegoCamera_Type_Count,
};
assert_sizeof(LegoCamera_Type, 0x4);


flags_scoped(CameraFlags) : uint32 // [LegoRR/LegoCamera.c|flags:0x4|type:uint]
{
	CAMERA_FLAG_NONE          = 0,

	CAMERA_FLAG_TILTRANGE     = 0x1,
	CAMERA_FLAG_ROTATIONRANGE = 0x2,
	CAMERA_FLAG_ZOOMRANGE     = 0x4,
	CAMERA_FLAG_FREEMOVEMENT  = 0x8,
	CAMERA_FLAG_FPSETUP       = 0x10, // Camera_Update has been called once for LegoCamera_FP type
};
flags_scoped_end(CameraFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct LegoCamera // [LegoRR/LegoCamera.c|struct:0xbc] May be camera data (which is related to- but not the same as viewports)
{
	/*00,4*/    LegoCamera_Type type;
	/*04,4*/    LegoObject* trackObj;
	/*08,4*/    real32 trackRadarZoomSpeed;
	/*0c,4*/    real32 trackRadarZoom;
	/*10,4*/    real32 trackRadarRotationSpeed; // A constant yaw H rotation speed applied to tracked radar objects
	/*14,4*/    sint32 trackFPCameraFrame; // CameraFrameIndex for FP object SwapPolyFP
	/*18,c*/    Vector3F moveVector; // current 3D (really 2D) movement and directional speed
	/*24,4*/    Gods98::Container* contCam; // only cont created for FP type (child of contRoot if FP)
	/*28,4*/    Gods98::Container* cont2; // child of contRoot
	/*2c,4*/    Gods98::Container* cont3; // child of contRoot
	/*30,4*/    Gods98::Container* cont4; // child of cont2
	/*34,4*/    Gods98::Container* contListener; // child of cont4 (topdown type only)
	/*38,40*/   uint8 reserved1[64]; // unused region of struct
	/*78,4*/    real32 tilt; // (init: 0.0f) current tilt (Pitch, V Rotation)
	/*7c,8*/    Range2F tiltRange; // min/max tilt (Pitch, V Rotation)
	/*84,4*/    real32 rotation; // (init: 0.0f) current yaw (Angle, H Rotation)
	/*88,8*/    Range2F rotationRange; // min/max yaw (Angle, H Rotation)
	/*90,4*/    real32 zoom; // (init: 200.0f) current dist (Zoom)
	/*94,8*/    Range2F zoomRange; // min/max dist (Zoom)
	/*9c,4*/    real32 moveSpeed; // current 3D (really 2D) movement speed
	/*a0,4*/    real32 shakeIntensity;
	/*a4,4*/    real32 shakeDuration;
	/*a8,4*/    real32 shakeTimer; // count-up timer until shakeDuration
	/*ac,c*/    Vector3F shakeVector;
	/*b8,4*/    CameraFlags flags;
	/*bc*/
};
assert_sizeof(LegoCamera, 0xbc);


struct Camera_Globs // [LegoRR/LegoCamera.c|struct:0x10|tags:GLOBS]
{
	/*00,4*/	real32 maxSpeed;			// (cfg: CameraSpeed) maximum move speed of camera
	/*04,4*/	real32 deceleration;		// (cfg: CameraDropOff) movement deceleration
	/*08,4*/	real32 acceleration;		// (cfg: CameraAcceleration) movement acceleration
	/*0c,4*/	sint32 mouseScrollIndent;	// (cfg: MouseScrollIndent) area around window borders for mouse movement
	/*10*/
};
assert_sizeof(Camera_Globs, 0x10);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// Internal access only
// <LegoRR.exe @005570a0>
extern Camera_Globs & cameraGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <unused>
__inline bool32 Camera_IsFreeMovement(const LegoCamera* cam) { return (cam->flags & CameraFlags::CAMERA_FLAG_FREEMOVEMENT); }



// <LegoRR.exe @00435a50>
LegoCamera* __cdecl Camera_Create(Gods98::Container* contRoot, LegoCamera_Type camType);


/// CUSTOM: Modified function to replace interop hook of `Camera_EnableFreeMovement`.
///         This allows toggling Camera free movement with Numpad 0 instead of only being able to "turn it off".
// <LegoRR.exe @00435cc1>
void __cdecl Camera_ToggleFreeMovement(LegoCamera* cam, bool32 unused);

// NO LONGER USED FOR INTEROP HOOK.
// <LegoRR.exe @00435cc1>
void __cdecl Camera_EnableFreeMovement(LegoCamera* cam, bool32 on);


// <LegoRR.exe @00435cf8>
void __cdecl Camera_Free(LegoCamera* cam);

// <LegoRR.exe @00435d3e>
void __cdecl Camera_InitCameraMovements(real32 maxSpeed, real32 deceleration, real32 acceleration, sint32 mouseScrollIndent);

// <LegoRR.exe @00435d65>
void __cdecl Camera_CopyFPPositionOrientation(LegoCamera* dstCamera, LegoCamera* srcCamera);

// Set liveObj to NULL to disable.
// <LegoRR.exe @00435deb>
void __cdecl Camera_TrackObject(LegoCamera* cam, LegoObject* liveObj, real32 trackZoomSpeed, real32 trackZoom, real32 trackTilt, real32 trackRotationSpeed);

// <LegoRR.exe @00435e24>
void __cdecl Camera_SetFPObject(LegoCamera* cam, LegoObject* liveObj, sint32 cameraFrame);

// <LegoRR.exe @00435e3b>
sint32 __cdecl Camera_GetFPCameraFrame(LegoCamera* cam);

// <LegoRR.exe @00435e46>
void __cdecl Camera_StopMovement(LegoCamera* cam);

// <LegoRR.exe @00435e58>
sint32 __cdecl Camera_GetMouseScrollIndent(void);

// <LegoRR.exe @00435e62>
void __cdecl Camera_Shake(LegoCamera* cam, real32 intensity, real32 duration);

// <LegoRR.exe @00435e8c>
void __cdecl Camera_Update(LegoCamera* cam, Lego_Level* level, real32 elapsedAbs, real32 elapsedGame);

// <LegoRR.exe @00436a53>
void __cdecl Camera_SetTiltRange(LegoCamera* cam, real32 minTilt, real32 maxTilt);

// <LegoRR.exe @00436a82>
void __cdecl Camera_SetTilt(LegoCamera* cam, real32 tilt);

// <LegoRR.exe @00436b22>
void __cdecl Camera_AddTilt(LegoCamera* cam, real32 tiltAmount);

// <LegoRR.exe @00436b43>
void __cdecl Camera_SetRotationRange(LegoCamera* cam, real32 minRotation, real32 maxRotation);

// <LegoRR.exe @00436b75>
void __cdecl Camera_SetRotation(LegoCamera* cam, real32 rotation);

// <LegoRR.exe @00436c16>
void __cdecl Camera_AddRotation(LegoCamera* cam, real32 rotationAmount);

// <LegoRR.exe @00436c3a>
void __cdecl Camera_SetZoomRange(LegoCamera* cam, real32 minZoom, real32 maxZoom);

// <LegoRR.exe @00436c6c>
void __cdecl Camera_SetZoom(LegoCamera* cam, real32 zoom);

// <LegoRR.exe @00436cc7>
void __cdecl Camera_AddZoom(LegoCamera* cam, real32 zoomAmount);

// <LegoRR.exe @00436ceb>
void __cdecl Camera_AddTranslation2D(LegoCamera* cam, real32 translateX, real32 translateY);

// <LegoRR.exe @00436d0b>
void __cdecl Camera_GetTopdownPosition(LegoCamera* cam, Vector3F* out_position);

// <LegoRR.exe @00436d2d>
void __cdecl Camera_SetTopdownPosition(LegoCamera* cam, real32 xPos, real32 yPos);

// <LegoRR.exe @00436d55>
bool32 __cdecl Camera_GetTopdownWorldPos(LegoCamera* cam, Map3D* map, Vector3F* out_worldPos);

// <LegoRR.exe @00436d9b>
real32 __cdecl Camera_GetRotation(LegoCamera* cam);

// <LegoRR.exe @00436da9>
void __cdecl Camera_Move(LegoCamera* cam, IN OUT Vector3F* dir, real32 elapsedAbs);

#pragma endregion

}
