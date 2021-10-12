// Lws.h : 
//
/// FILEIO: Shared:Data[wad,std,cd]
/// APIS: IDirect3DRM3, IDirect3DRMFrame3, IDirect3DRMUserVisual, IDirect3DRMVisual[*]
/// DEPENDENCIES: 3DSound, Files, Main, Maths, Mesh, Utils, (Errors, Memory)
/// DEPENDENTS: AnimClone, Containers, Lego (initialise/shutdown only)

#pragma once

#include "../../common.h"
#include "../geometry.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct IDirect3DRMFrame3;

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct Mesh;

#pragma endregion

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef bool32 (__cdecl* LwsFindSFXIDFunc)(const char* name, OUT uint32* sfxID);
typedef bool32 (__cdecl* LwsSoundEnabledFunc)(void);
typedef sint32 (__cdecl* LwsPlaySample3DFunc)(void* frame, uint32 type, bool32 loop, bool32 onCont, const Vector3F* wPos);

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define LWS_FILESUFFIX				"lws"
#define LWS_HEADER1					"LWSC"
#define LWS_HEADER2					"1"
#define LWS_SOUNDTRIGGERPREFIX		"SFX"
#define LWS_SOUNDTRIGGERSEPERATOR	","

#define LWS_MAXLINELEN				1024
#define LWS_MAXARGS					20
#define LWS_NODELISTBLOCKSIZE		10
#define LWS_MAXMESHFILES			1000
#define LWS_MAXSTATICDISSOLVES		100
#define LWS_MAXTRIGGERKEYS			25

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

/*
#define LWS_FLAG_LOOPING			0x01

#define LWSNODE_FLAG_NULL			0x01
#define LWSNODE_FLAG_SOUNDTRIGGER	0x02
#define LWSNODE_FLAG_FACECAMERA		0x04
*/

namespace _ns_LwsFlags {
enum LwsFlags : uint8
{
	LWS_FLAG_NONE    = 0,

	LWS_FLAG_LOOPING = 0x1,
};
DEFINE_ENUM_FLAG_OPERATORS(LwsFlags);
static_assert(sizeof(LwsFlags) == 0x1, "");
} using LwsFlags = _ns_LwsFlags::LwsFlags;


namespace _ns_Lws_NodeFlags {
enum Lws_NodeFlags : uint8
{
	LWSNODE_FLAG_NONE         = 0,

	LWSNODE_FLAG_NULL         = 0x1,
	LWSNODE_FLAG_SOUNDTRIGGER = 0x2,
	LWSNODE_FLAG_FACECAMERA   = 0x4,
};
DEFINE_ENUM_FLAG_OPERATORS(Lws_NodeFlags);
static_assert(sizeof(Lws_NodeFlags) == 0x1, "");
} using Lws_NodeFlags = _ns_Lws_NodeFlags::Lws_NodeFlags;


#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Lws_SoundTrigger
{
	//	Sound* sound;
	/*00,4*/ uint32 sfxID;
	/*04,32*/ uint16 frameStartList[LWS_MAXTRIGGERKEYS];
	/*36,32*/ uint16 frameEndList[LWS_MAXTRIGGERKEYS];
	/*68,64*/ uint32 loopUID[LWS_MAXTRIGGERKEYS];
	/*cc,2*/ uint16 count;
	/*ce,2*/ uint16 padding1;
	/*d0*/
};
static_assert(sizeof(Lws_SoundTrigger) == 0xd0, "");


struct Lws_KeyInfo
{
	/*00,c*/ Vector3F position;
	/*0c,c*/ Vector3F hpb;
	/*18,c*/ Vector3F scale;
	/*24,2*/ uint16 frame;
	/*26,2*/ uint16 padding1;
	/*28*/
};
static_assert(sizeof(Lws_KeyInfo) == 0x28, "");


struct Lws_Node
{
	/*00,4*/ char* name;
	/*04,2*/ uint16 reference;
	/*06,2*/ uint16 frameIndex;
	/*08,1*/ uint8 triggerIndex;
	/*09,3*/ uint8 padding1[3];
	/*0c,c*/ Vector3F pivotVector;
	/*18,4*/ Lws_KeyInfo* keyList;
	/*1c,4*/ real32* dissolveLevel;
	/*20,4*/ uint16* dissolveFrame;
	/*24,2*/ uint16 keyCount;
	/*26,2*/ uint16 dissolveCount;
	/*28,1*/ Lws_NodeFlags flags;
	/*29,3*/ uint8 padding2[3];
	/*2c,4*/ Lws_Node* childList;
	/*30,4*/ Lws_Node* next;
	/*34*/
};
static_assert(sizeof(Lws_Node) == 0x34, "");


struct Lws_Info
{
	/*00,2*/ uint16 firstFrame;
	/*02,2*/ uint16 lastFrame;
	/*04,4*/ real32 fps;
	/*08,4*/ char* filePath;
	/*0c,4*/ real32 lastTime;
	/*10,4*/ real32 time;
	/*14,4*/ IDirect3DRMFrame3** frameList;
	/*18,4*/ Lws_SoundTrigger* triggerList;
	/*1c,4*/ Lws_Node* masterNode;
	/*20,4*/ Lws_Node* nodeList;
	/*24,2*/ uint16 nodeCount;
	/*26,2*/ uint16 nodeListSize;
	/*28,2*/ uint16 triggerCount;
	/*2a,2*/ uint16 padding1;
	/*2c,4*/ Lws_Info* clonedFrom;
	/*30,4*/ uint32 referenceCount;
	/*34,1*/ LwsFlags flags;
	/*35,3*/ uint8 padding2[3];
	/*38*/
};
static_assert(sizeof(Lws_Info) == 0x38, "");


struct Lws_MeshPath
{
	/*0,4*/ char* path;
	/*4,4*/ Mesh* mesh;
	/*8*/
};
static_assert(sizeof(Lws_MeshPath) == 0x8, "");


struct Lws_Globs
{
	/*0000,1f40*/ Lws_MeshPath meshPathList[LWS_MAXMESHFILES];			// For full path to files...
	/*1f40,1f40*/ Lws_MeshPath meshPathListShared[LWS_MAXMESHFILES];	// For shared files...
	/*3e80,4*/ char* sharedDir;
	/*3e84,4*/ uint32 meshPathCount;
	/*3e88,4*/ uint32 meshPathCountShared;
	/*3e8c,190*/ real32 staticDissolveLevel[LWS_MAXSTATICDISSOLVES];
	/*401c,4*/ uint32 staticDissolveCount;
	/*4020,4*/ LwsFindSFXIDFunc FindSFXIDFunc;
	/*4024,4*/ LwsSoundEnabledFunc SoundEnabledFunc;
	/*4028,4*/ LwsPlaySample3DFunc PlaySample3DFunc;
	/*402c*/
};
static_assert(sizeof(Lws_Globs) == 0x402c, "");

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00541838>
extern Lws_Globs & lwsGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00486cb0>
Lws_Info* __cdecl Lws_Parse(const char* fname, bool32 looping);

// <LegoRR.exe @00487980>
void __cdecl Lws_Initialise(const char* sharedDir, LwsFindSFXIDFunc FindSFXID,
								LwsPlaySample3DFunc PlaySample3D, LwsSoundEnabledFunc SoundEnabled);

// <LegoRR.exe @00487a20>
void __cdecl Lws_Shutdown(void);

// <LegoRR.exe @00487a90>
uint32 __cdecl Lws_GetFrameCount(Lws_Info* scene);

// <LegoRR.exe @00487aa0>
void __cdecl Lws_SetupSoundTriggers(Lws_Info* scene);

// <LegoRR.exe @00487c50>
void __cdecl Lws_LoadMeshes(Lws_Info* scene, IDirect3DRMFrame3* parent);


/// APIS: IDirect3DRMFrame3, IDirect3DRMUserVisual, IDirect3DRMVisual*
// <inlined>
/*__inline*/ Mesh* __cdecl Lws_GetNodeMesh(Lws_Info* scene, Lws_Node* node);


/// APIS: IDirect3DRMFrame3
// <LegoRR.exe @00487cc0>
Lws_Info* __cdecl Lws_Clone(Lws_Info* scene, IDirect3DRMFrame3* parent);

// <LegoRR.exe @00487e60>
void __cdecl Lws_SetTime(Lws_Info* scene, real32 time);

// <LegoRR.exe @00487f70>
real32 __cdecl Lws_FindPrevKey(Lws_Node* node, real32 time, uint16* prev);

// <LegoRR.exe @00488010>
void __cdecl Lws_AnimateTextures(Lws_Info* scene, Lws_Node* node);

// <LegoRR.exe @004880a0>
void __cdecl Lws_HandleTrigger(Lws_Info* scene, Lws_Node* node);

// <LegoRR.exe @00488190>
bool32 __cdecl Lws_KeyPassed(Lws_Info* scene, uint32 key);

// <LegoRR.exe @00488280>
real32 __cdecl Lws_FindPrevDissolve(Lws_Node* node, real32 time, uint16* prev);

// <LegoRR.exe @00488330>
void __cdecl Lws_InterpolateDissolve(Lws_Info* scene, Lws_Node* node, uint16 prev, real32 delta);

// <LegoRR.exe @00488390>
void __cdecl Lws_SetDissolveLevel(Lws_Info* scene, Lws_Node* node, real32 level);

// <LegoRR.exe @00488430>
void __cdecl Lws_InterpolateKeys(Lws_Info* scene, Lws_Node* node, uint16 key, real32 delta);

/// APIS: IDirect3DRMFrame3
// <LegoRR.exe @004885a0>
void __cdecl Lws_SetupNodeTransform(Lws_Info* scene, Lws_Node* node, const Vector3F* pos, const Vector3F* hpb, const Vector3F* scale);

// <LegoRR.exe @00488880>
Mesh* __cdecl Lws_LoadMesh(const char* baseDir, const char* fname, IDirect3DRMFrame3* frame, bool32 noTextures);

// <LegoRR.exe @004889f0>
Mesh* __cdecl Lws_SearchMeshPathList(Lws_MeshPath* list, uint32 count, const char* path);

// <LegoRR.exe @00488a50>
void __cdecl Lws_AddMeshPathEntry(Lws_MeshPath* list, IN OUT uint32* count, const char* path, Mesh* mesh);

/// APIS: IDirect3DRM3, IDirect3DRMFrame3
// <LegoRR.exe @00488a80>
void __cdecl Lws_CreateFrames(Lws_Info* scene, Lws_Node* node, IDirect3DRMFrame3* parent, IN OUT uint16* frameCount);

// <LegoRR.exe @00488bc0>
void __cdecl Lws_LoadNodes(Lws_Info* scene, Lws_Node* node);

// <LegoRR.exe @00488c60>
void __cdecl Lws_SetAbsoluteKey(Lws_Info* scene, Lws_Node* node, uint16 key);

// <LegoRR.exe @00488c90>
void __cdecl Lws_Free(Lws_Info* scene);

// <LegoRR.exe @00488d30>
void __cdecl Lws_FreeNode(Lws_Info* scene, Lws_Node* node);

#pragma endregion

}
