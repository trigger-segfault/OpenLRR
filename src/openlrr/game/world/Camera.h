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


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum_scoped(CameraType) : sint32 // [LegoRR/LegoCamera.c|enum:0x4|type:int]
{
    CAMERATYPE_NONE        = 0,
    CAMERATYPE_TOPDOWN     = 1,
    CAMERATYPE_RADAR       = 2,
    CAMERATYPE_FIRSTPERSON = 3,
};
enum_scoped_end(CameraType, 0x4);


flags_scoped(CameraFlags) : uint32 // [LegoRR/LegoCamera.c|flags:0x4|type:uint]
{
    CAMERA_NONE         = 0,
    CAMERA_TILTRANGE    = 0x1,
    CAMERA_YAWRANGE     = 0x2,
    CAMERA_DISTRANGE    = 0x4,
    CAMERA_FREEMOVEMENT = 0x8,
    CAMERA_UNK_10       = 0x10,
};
flags_scoped_end(CameraFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct LegoCamera // [LegoRR/LegoCamera.c|struct:0xbc] May be camera data (which is related to- but not the same as viewports)
{
    /*00,4*/        CameraType camType;
    /*04,4*/        LegoObject* trackObj;
    /*08,4*/        real32 trackRadarFloat_8;
    /*0c,4*/        real32 trackRadarDist;
    /*10,4*/        real32 trackRadarYawSpeed; // A constant yaw H rotation speed applied to tracked radar objects
    /*14,4*/        undefined4 trackFPCameraFrame; // CameraFrameIndex for FP object SwapPolyFP
    /*18,c*/        Vector3F moveVector; // current 3D (really 2D) movement and directional speed
    /*24,4*/        Gods98::Container* contCam; // only cont created for FP type (child of contRoot if FP)
    /*28,4*/        Gods98::Container* cont2; // child of contRoot
    /*2c,4*/        Gods98::Container* cont3; // child of contRoot
    /*30,4*/        Gods98::Container* cont4; // child of cont2
    /*34,4*/        Gods98::Container* contListener; // child of cont4 (topdown type only)
    /*38,40*/       uint8 reserved1[64]; // unused region of struct
    /*78,4*/        real32 tilt; // (init: 0.0f) current tilt (Pitch, V Rotation)
    /*7c,8*/        Range2F tiltRange; // min/max tilt (Pitch, V Rotation)
    /*84,4*/        real32 yaw; // (init: 0.0f) current yaw (Angle, H Rotation)
    /*88,8*/        Range2F yawRange; // min/max yaw (Angle, H Rotation)
    /*90,4*/        real32 dist; // (init: 200.0f) current dist (Zoom)
    /*94,8*/        Range2F distRange; // min/max dist (Zoom)
    /*9c,4*/        real32 moveSpeed; // current 3D (really 2D) movement speed
    /*a0,4*/        real32 shakeIntensity;
    /*a4,4*/        real32 shakeDuration;
    /*a8,4*/        real32 shakeFloat_a8;
    /*ac,c*/        Vector3F shakeVector;
    /*b8,4*/        CameraFlags flags;
    /*bc*/
};
assert_sizeof(LegoCamera, 0xbc);


struct Camera_Globs // [LegoRR/LegoCamera.c|struct:0x10|tags:GLOBS]
{
	/*00,4*/	real32 CameraSpeed;         // (cfg: CameraSpeed) maximum move speed of camera
	/*04,4*/	real32 CameraDropOff;       // (cfg: CameraDropOff) movement deceleration
	/*08,4*/	real32 CameraAcceleration;  // (cfg: CameraAcceleration) movement acceleration
	/*0c,4*/	sint32 MouseScrollIndent;   // (cfg: MouseScrollIndent) area around window borders for mouse movement
	/*10*/
};
assert_sizeof(Camera_Globs, 0x10);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005570a0>
extern Camera_Globs & cameraGlobs;

#pragma endregion

}
