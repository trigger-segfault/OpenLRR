// Containers.h : 
//
/// FILEIO: (Config,Shared:)Data[wad,std,cd]
/// APIS: IDirect3DRM3, IDirect3DRMDevice3, IDirect3DRMFrame[13|Array], IDirect3DRMLight,
///       IDirect3DRMMesh, IDirect3DRMMeshBuilder2, IDirect3DRMTexture[13], IDirect3DRMVisual,
///       IDirectDraw4, IDirectDrawPalette, IDirectDrawSurface[14]
/// DEPENDENCIES: 3DSound, Activities, AnimClone, Bmp, Config, DirectDraw, Files,
///               Lws, Main, Mesh, Utils, (Errors, Memory)
/// DEPENDENTS: ...

#pragma once

#include "../../common.h"
#include "../geometry.h"

#include "../core/ListSet.hpp"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct IDirectDrawSurface4;
struct IDirect3DRMTexture3;
typedef IDirect3DRMTexture3* LPDIRECT3DRMTEXTURE3;
struct IDirect3DRMObject;
struct IDirect3DRMVisual;
struct IDirect3DRMFrame3;
struct IDirect3DRMLight;
struct IDirect3DRMMesh;
typedef IDirect3DRMObject* LPDIRECT3DRMOBJECT;

/*struct IDirect3DRMFrame3;
typedef IDirect3DRMFrame3* LPDIRECT3DRMFRAME3;
typedef float D3DVALUE;*/

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct Container;
struct Container_CloneData;
struct AnimClone;
struct Mesh;
struct Sound3D_SoundFrameRecord;

#pragma endregion

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef void (__cdecl* ContainerActivityCallback)(Container* cont, void* data);
typedef void (__cdecl* ContainerSoundTriggerCallback)(const char* sampleName, Container* cont, void* data);
typedef void (__cdecl* ContainerTriggerFrameCallback)(Container* cont, void* data);
typedef bool32 (__cdecl* ContainerWalkTreeCallback)(IDirect3DRMFrame3* frame, void* data);

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define CONTAINER_DEBUG_NOTREQUIRED	NULL

#define CONTAINER_DDSFILEMAGIC			0x20534444

#define CONTAINER_FRAMENAMELEN			20
#define CONTAINER_ACTIVITYFRAMEPREFIX	"ActFrame"
#define CONTAINER_SCALESTRING			"SCALE"

#define CONTAINER_MAXLISTS				20			// Maximum of 2^20 - 1 containers
#define CONTAINER_MAXTEXTURES			1000
#define CONTAINER_MESHGROUPBLOCKSIZE	20
//#define CONTAINER_MAXROTATION			100

#define CONTAINER_NULLSTRING			"NULL"
#define CONTAINER_MESHSTRING			"MESH"
#define CONTAINER_MESHNOTEXTURESTRING	"MESH:NOTEXTURE"
#define CONTAINER_FRAMESTRING			"FRAME"
#define CONTAINER_ANIMSTRING			"ANIM"
#define CONTAINER_LWSSTRING				"LWS"
#define CONTAINER_LWOSTRING				"LWO"
#define CONTAINER_LWONOTEXTURESTRING	"LWO:NOTEXTURE"
#define CONTAINER_ACTIVITYSTRING		"ACT"

#define CONTAINER_ULONG_NULL			-1

#define CONTAINER_TRASHVALUE			0xdeaddead

#define CONTAINER_MAXVISUALS			4

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

flags_scoped(ContainerFlags) : uint32
{
	CONTAINER_FLAG_NONE             = 0,

	CONTAINER_FLAG_TRIGGERSAMPLE    = 0x2,
	CONTAINER_FLAG_MESHSWAPPED      = 0x4,
	CONTAINER_FLAG_HIDDEN           = 0x8,
	CONTAINER_FLAG_DEADREFERENCE    = 0x10,
	CONTAINER_FLAG_ANIMATIONSKIPPED = 0x20,

	CONTAINER_FLAG_HIDDEN2          = 0x80,
};
flags_scoped_end(ContainerFlags, 0x4);


flags_scoped(Container_GlobFlags) : uint32
{
	CONTAINER_GLOB_FLAG_NONE           = 0,

	CONTAINER_GLOB_FLAG_INITIALISED    = 0x1,
	CONTAINER_GLOB_FLAG_TRIGGERENABLED = 0x40,
};
flags_scoped_end(Container_GlobFlags, 0x4);


flags_scoped(Container_TextureFlags) : uint32
{
	CONTAINER_TEXTURE_FLAG_NONE   = 0,

	CONTAINER_TEXTURE_FLAG_NOLOAD = 0x1,
};
flags_scoped_end(Container_TextureFlags, 0x4);


enum class Container_Type : sint32
{
	Invalid      = -1,
	Null         = 0,
	Mesh         = 1,
	Frame        = 2,
	Anim         = 3,
	FromActivity = 4,
	Light        = 5,
	Reference    = 6,
	LWS          = 7,
	LWO          = 8,

	TypeCount    = 9,
};
assert_sizeof(Container_Type, 0x4);


enum class Container_Light : uint32
{
	Ambient       = 0,
	Point         = 1,
	Spot          = 2,
	Directional   = 3,
	ParallelPoint = 4,
};
assert_sizeof(Container_Light, 0x4);


enum class Container_Combine : uint32
{
	Replace = 0,
	Before  = 1,
	After   = 2,
};
assert_sizeof(Container_Combine, 0x4);


// same as enum Graphics_Quality (find a way to typedef this in without including Main)
enum class Container_Quality : uint32
{
	Wireframe = 0,
	UnlitFlat = 1,
	Flat      = 2,
	Gouraud   = 3,
	Phong     = 4, // (custom)
};
assert_sizeof(Container_Quality, 0x4);


/// WARNING: Do not use this with SetFogMode, the values are swapped for D3DRM, gahhhh
enum class Container_FogMode : uint32
{
	None               = 0, // D3DFOG_NONE
	Exponential        = 1, // D3DFOG_EXP
	ExponentialSquared = 2, // D3DFOG_EXP2
	Linear             = 3, // D3DFOG_LINEAR
};
assert_sizeof(Container_FogMode, 0x4);


enum class Container_MeshType : uint32
{
	Normal         = 0,
	SeperateMeshes = 1,
	Immediate      = 2,
	Transparent    = 3,
	Additive       = 4,
	Subtractive    = 5,
};
assert_sizeof(Container_MeshType, 0x4);


enum class Container_SearchMode : uint32
{
	FirstMatch = 0,
	MatchCount = 1,
	NthMatch   = 2,
};
assert_sizeof(Container_SearchMode, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define Container_DebugCheckOK(c)			(c)
#define Container_Mesh_DebugCheckOK(c,g)


//#define Container_NoteCreation(o)
#define Container_NoteCreation(f, ...)						Container_Frame_TrackCreation(f, __VA_ARGS__)
#define Container_NoteAddRef(f, ...)						Container_Frame_TrackAddRef(f, __VA_ARGS__)
#define Container_NoteRelease(f, c)							Container_Frame_TrackRelease(f, (sint32)c)


#define CDF(f)	Container_Frame_CastDown(f)
#define CUF(f)	Container_Frame_CastUp(f)

#define Container_SetWorldRotation(c,x,y,z,a)				Container_SetRotation((c),nullptr,(x),(y),(z),(a))
#define Container_SetWorldPosition(c,x,y,z)					Container_SetPosition((c),nullptr,(x),(y),(z))
#define Container_SetWorldOrientation(c,dx,dy,dz,ux,uy,uz)	Container_SetOrientation((c),nullptr,(dx),(dy),(dz),(ux),(uy),(uz))
#define Container_GetWorldRotation(c,ax,a)					Container_GetRotation((c),nullptr,(ax),(a))
#define Container_GetWorldPosition(c,p)						Container_GetPosition((c),nullptr,(p))
#define Container_GetWorldOrientation(c,d,u)				Container_GetOrientation((c),nullptr,(d),(u))

#define Container_MakePointLight(c,r,g,b)					Container_MakeLight((c),Gods98::Container_Light::Point,(r),(g),(b));
#define Container_MakeSpotLight(c,r,g,b)					Container_MakeLight((c),Gods98::Container_Light::Spot,(r),(g),(b));
#define Container_MakeAmbientLight(c,r,g,b)					Container_MakeLight((c),Gods98::Container_Light::Ambient,(r),(g),(b));
#define Container_MakeDirectionalLight(c,r,g,b)				Container_MakeLight((c),Gods98::Container_Light::Directional,(r),(g),(b));
#define Container_MakeParallelPointLight(c,r,g,b)			Container_MakeLight((c),Gods98::Container_Light::ParallelPoint,(r),(g),(b));

#define Container_MakeCamera(p)								Container_Create((p))

#define Container_SetColour(c,r,g,b)						Container_SetColourAlpha((c),(r),(g),(b),1.0f)
#define Container_GetColour(c,r,g,b)						Container_GetColourAlpha((c),(r),(g),(b),nullptr)
#define Container_Mesh_SetColour(c,i,r,g,b)					Container_Mesh_SetColourAlpha((c),(i),(r),(g),(b),1.0f)
#define Container_Mesh_GetColour(c,i,r,g,b)					Container_Mesh_GetColourAlpha((c),(i),(r),(g),(b),nullptr)

#define Container_MakeMesh(r)								Container_MakeMesh2((r),Gods98::Container_MeshType::Normal)
#define Container_MakeMeshSeperateGroups(r)					Container_MakeMesh2((r),Gods98::Container_MeshType::SeperateMeshes)
#define Container_MakeMeshImmediate(r)						Container_MakeMesh2((r),Gods98::Container_MeshType::Immediate)
#define Container_MakeMeshTrans(r)							Container_MakeMesh2((r),Gods98::Container_MeshType::Transparent)
#define Container_MakeMeshAdditive(r)						Container_MakeMesh2((r),Gods98::Container_MeshType::Additive)
#define Container_MakeMeshSubtractive(r)					Container_MakeMesh2((r),Gods98::Container_MeshType::Subtractive)

#define Container_LoadTexture(f,w,h)						Container_LoadTexture2((f),false,(w),(h))
#define Container_LoadTextureImmediate(f,w,h)				Container_LoadTexture2((f),true,(w),(h))

#pragma endregion

/**********************************************************************************
 ******** Structure
 **********************************************************************************/

#pragma region Structs

struct Container_TypeData
{
	/*00,4*/ char* name;
	/*04,4*/ IDirect3DRMLight* light;
	/*08,4*/ IDirect3DRMMesh* mesh;
	/*0c,4*/ Mesh* transMesh;
	/*10*/
};
assert_sizeof(Container_TypeData, 0x10);


//typedef struct Container_CloneData;


//struct Container_RotationData
//{
//	/*00,4*/ Container* container;
//	/*04,4*/ Container_Combine combine;
//	/*08,c*/ Vector3F vector;
//	/*14,4*/ real32 angle;
//	/*18*/
//};
//assert_sizeof(Container_RotationData, 0x18);


struct Container
{
	/*00,4*/ IDirect3DRMFrame3* masterFrame;
	/*04,4*/ IDirect3DRMFrame3* activityFrame;
	/*08,4*/ IDirect3DRMFrame3* hiddenFrame;
	/*0c,4*/ Container_TypeData* typeData;
	/*10,4*/ Container_Type type;
	/*14,4*/ ContainerFlags flags;
	/*18,4*/ ContainerActivityCallback activityCallback;
	/*1c,4*/ void* activityCallbackData;
	/*20,4*/ void* userData;
	/*24,4*/ Container_CloneData* cloneData;			// Only used by animsets.
	/*28,4*/ Container* nextFree;
	/*2c*/
};
assert_sizeof(Container, 0x2c);


struct Container_CloneData
{
	/*00,4*/ Container* clonedFrom;				// NULL if this is the original
	/*04,4*/ Container** cloneTable;			// List of clones (NULL for unused elements)
	/*08,4*/ uint32 cloneCount;
	/*0c,4*/ uint32 cloneNumber;		// Total number of clones made and clone's number in the list.
	/*10,4*/ bool32 used;
	/*14*/
};
assert_sizeof(Container_CloneData, 0x14);


struct Container_AppData
{
	/*00,4*/ Container* ownerContainer;
	//IDirect3DRMAnimationSet2* animSet;
	/*04,4*/ char* animSetFileName;			// For the dodgy Animation Set clone stuff...
	/*08,4*/ const char* frameName;					// For freeing the allocation for SetName...
	/*0c,4*/ uint32 frameCount;
	/*10,4*/ real32 currTime;
	/*14,4*/ real32 transCo;						// Standard translation during amimset loop.
	/*18,4*/ const char* activitySample;				// Sample to play when activity is called...
	/*1c,4*/ AnimClone* animClone;
	/*20,4*/ uint32 trigger;
	/*24,4*/ Sound3D_SoundFrameRecord* soundList;		// For 'Sound3D'
	/*28*/
};
assert_sizeof(Container_AppData, 0x28);


struct Container_MeshAppData
{
	/*00,4*/ IDirect3DRMMesh** meshList;
	/*04,4*/ uint32 usedCount;
	/*08,4*/ uint32 listSize;
	/*0c,4*/ bool32 groupZeroHidden;
	/*10,4*/ bool32 firstAddGroup;
	/*14*/
};
assert_sizeof(Container_MeshAppData, 0x14);


struct Container_SearchData
{
	/*00,4*/ const char* string;
	/*04,4*/ uint32 stringLen;
	/*08,4*/ bool32 caseSensitive;
	/*0c,4*/ IDirect3DRMFrame3* resultFrame;
	/*10,4*/ uint32 count;
	/*14,4*/ Container_SearchMode mode;
	/*18,4*/ uint32 matchNumber;
	/*1c*/
};
assert_sizeof(Container_SearchData, 0x1c);


struct Container_TextureRef
{
	/*0,4*/ char* fileName;
	/*4,4*/ IDirect3DRMTexture3* texture;
	/*8*/
};
assert_sizeof(Container_TextureRef, 0x8);


struct Container_TextureData
{
	/*0,4*/ const char* xFileName;
	/*4,4*/ Container_TextureFlags flags;
	/*8*/
};
assert_sizeof(Container_TextureData, 0x8);


//struct Container_DummyTexture
//{
//	/*0,4*/ uint32 dummy;
//	/*4*/
//};
//assert_sizeof(Container_DummyTexture, 0x4);


struct Container_Texture
{
	/*0,4*/ IDirectDrawSurface4* surface;
	/*4,4*/ IDirect3DRMTexture3* texture;
	/*8,4*/ bool32 colourKey;
	/*c*/
};
assert_sizeof(Container_Texture, 0xc);


//struct Container_DummyMaterial
//{
//	/*0,4*/ uint32 dummy;
//	/*4*/
//};
//assert_sizeof(Container_DummyMaterial, 0x4);


struct Container_Globs
{
	/*0000,50*/ Container* listSet[CONTAINER_MAXLISTS];
	/*0050,4*/ Container* freeList;
	/*0054,4*/ Container* rootContainer;
	/*0058,24*/ const char* typeName[(sint32)Container_Type::TypeCount];
	/*007c,24*/ const char* extensionName[(sint32)Container_Type::TypeCount];
	/*00a0,4*/ const char* gameName;
	/*00a4,10*/ IDirect3DRMVisual* visualArray[CONTAINER_MAXVISUALS];
	/*00b4,1f40*/ Container_TextureRef textureSet[CONTAINER_MAXTEXTURES];
	/*1ff4,4*/ uint32 textureCount;
	/*1ff8,4*/ ContainerSoundTriggerCallback soundTriggerCallback;
	/*1ffc,4*/ void* soundTriggerData;
	/*2000,4*/ ContainerTriggerFrameCallback triggerFrameCallback;
	/*2004,4*/ void* triggerFrameData;
	/*2008,4*/ char* sharedDir;
	/*200c,4*/ uint32 fogColour;
	/*2010,4*/ uint32 listCount;
	/*2014,4*/ Container_GlobFlags flags;
	/*2018*/
};
assert_sizeof(Container_Globs, 0x2018);


using Container_ListSet = ListSet::Collection<Container_Globs>;

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00506400>
extern char (& s_FormatPartName_name)[1024];

// <LegoRR.exe @0076bd80>
extern Container_Globs & containerGlobs;

extern Container_ListSet containerListSet;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions


void __cdecl Container_Frame_TrackCreation(void* frame, sint32 initialRefs = INT_MIN);
void __cdecl Container_Frame_TrackAddRef(void* frame, sint32 curRefs = INT_MIN);
void __cdecl Container_Frame_TrackRelease(void* frame, sint32 curRefs = INT_MIN);
sint32 __cdecl Container_Frame_TrackRefCount(void* frame);
bool32 __cdecl Container_Frame_TrackIsReleased(void* frame);
bool32 __cdecl Container_Frame_TrackIsTracked(void* frame);


// <inlined>
/*__inline*/ uint32 __cdecl Container_GetRGBAColour(real32 r, real32 g, real32 b, real32 a);

// <inlined>
/*__inline*/ uint32 __cdecl Container_GetRGBColour(real32 r, real32 g, real32 b);



// <LegoRR.exe @004729d0>
Container* __cdecl Container_Initialise(const char* gameName);

// <LegoRR.exe @00472ac0>
void __cdecl Container_Shutdown(void);

// <LegoRR.exe @00472b80>
void __cdecl Container_SetSharedTextureDirectory(const char* path);

// <LegoRR.exe @00472ba0>
void __cdecl Container_EnableSoundTriggers(bool32 on);

// <LegoRR.exe @00472bc0>
void __cdecl Container_SetTriggerFrameCallback(ContainerTriggerFrameCallback Callback, void* data);

// <LegoRR.exe @00472be0>
void __cdecl Container_SetSoundTriggerCallback(ContainerSoundTriggerCallback Callback, void* data);

// <LegoRR.exe @00472c00>
Container* __cdecl Container_GetRoot(void);

// <LegoRR.exe @00472c10>
Container* __cdecl Container_Create(Container* parent);

// <LegoRR.exe @00472d00>
void __cdecl Container_Remove(Container* dead);

// <LegoRR.exe @00472d10>
void __cdecl Container_Remove2(Container* dead, bool32 kill);

// <LegoRR.exe @00472f90>
Container* __cdecl Container_Load(Container* parent, const char* filename, const char* typestr, bool32 looping);


// <inlined>
/*__inline*/ Container_Type __cdecl Container_GetType(const Container* cont);

// <inlined>
/*__inline*/ bool32 __cdecl Container_AddActivity(Container* cont, const char* fname, const char* actname, real32 transCo, uint32 trigger, const char* sample, AnimClone* animClone, bool32 lws, bool32 looping);


// <LegoRR.exe @00473600>
bool32 __cdecl Container_IsCurrentActivity(Container* cont, const char* actname);

// <LegoRR.exe @00473630>
bool32 __cdecl Container_SetActivity(Container* cont, const char* actname);

// <LegoRR.exe @00473720>
bool32 __cdecl Container_Light_SetSpotPenumbra(Container* spotlight, real32 angle);

// <LegoRR.exe @00473740>
bool32 __cdecl Container_Light_SetSpotUmbra(Container* spotlight, real32 angle);

// <LegoRR.exe @00473760>
bool32 __cdecl Container_Light_SetSpotRange(Container* spotlight, real32 dist);

// <LegoRR.exe @00473780>
void __cdecl Container_Light_SetEnableContainer(Container* light, Container* enable);

// <LegoRR.exe @004737b0>
Container* __cdecl Container_MakeLight(Container* parent, Container_Light type, real32 r, real32 g, real32 b);

// <LegoRR.exe @00473820>
Container* __cdecl Container_MakeMesh2(Container* parent, Container_MeshType type);

// <LegoRR.exe @00473940>
IDirect3DRMFrame3* __cdecl Container_GetMasterFrame(Container* cont);

// <LegoRR.exe @00473950>
Container* __cdecl Container_Clone(Container* orig);

// <LegoRR.exe @00473de0>
void __cdecl Container_Hide2(Container* cont, bool32 hide);

// <LegoRR.exe @00473e00>
void __cdecl Container_Hide(Container* cont, bool32 hide);

// <LegoRR.exe @00473e60>
bool32 __cdecl Container_IsHidden(Container* cont);

// <LegoRR.exe @00473e80>
Container* __cdecl Container_SearchTree(Container* root, const char* name, Container_SearchMode mode, IN OUT uint32* count);

// Pass NULL as instance for any...
// <LegoRR.exe @00473f20>
const char* __cdecl Container_FormatPartName(Container* cont, const char* partname, OPTIONAL uint32* instance);

// <LegoRR.exe @00474060>
void __cdecl Container_SetUserData(Container* cont, void* data);

// <LegoRR.exe @00474070>
void* __cdecl Container_GetUserData(Container* cont);

// <LegoRR.exe @00474080>
void __cdecl Container_EnableFog(bool32 on);

// <LegoRR.exe @004740d0>
void __cdecl Container_SetFogColour(real32 r, real32 g, real32 b);

// D3DRMFOGMODE mode
// <LegoRR.exe @00474130>
void __cdecl Container_SetFogMode(uint32 mode);

// <LegoRR.exe @00474160>
void __cdecl Container_SetFogParams(real32 start, real32 end, real32 density);

// <LegoRR.exe @00474180>
void __cdecl Container_SetPerspectiveCorrection(Container* cont, bool32 on);

// <LegoRR.exe @00474230>
bool32 __cdecl Container_SetPerspectiveCorrectionCallback(IDirect3DRMFrame3* frame, void* data);

// <LegoRR.exe @00474310>
IDirectDrawSurface4* __cdecl Container_LoadTextureSurface(const char* fname, bool32 managed,
								OUT uint32* width, OUT uint32* height, OUT bool32* trans);

// <LegoRR.exe @004746d0>
bool32 __cdecl Container_GetDecalColour(const char* fname, OUT uint32* colour);

// <LegoRR.exe @004747b0>
Container_Texture* __cdecl Container_LoadTexture2(const char* fname, bool32 immediate,
								OUT uint32* width, OUT uint32* height);

// <LegoRR.exe @004749d0>
void __cdecl Container_FreeTexture(Container_Texture* text);

// <LegoRR.exe @00474a20>
void __cdecl Container_Mesh_Swap(Container* target, Container* origin, bool32 restore);

// <LegoRR.exe @00474bb0>
uint32 __cdecl Container_Mesh_AddGroup(Container* cont, uint32 vertexCount,
								uint32 faceCount, uint32 vPerFace, const uint32* faceData);

// <LegoRR.exe @00474ce0>
uint32 __cdecl Container_Mesh_GetGroupCount(Container* cont);

// <LegoRR.exe @00474d20>
void __cdecl Container_Mesh_SetQuality(Container* cont, uint32 groupID, Container_Quality quality);

// <LegoRR.exe @00474da0>
bool32 __cdecl Container_Mesh_IsGroupHidden(Container* cont, uint32 group);

// <LegoRR.exe @00474df0>
void __cdecl Container_Mesh_HideGroup(Container* cont, uint32 group, bool32 hide);

// <LegoRR.exe @00474ec0>
bool32 __cdecl Container_Mesh_HandleSeperateMeshGroups(IN OUT IDirect3DRMMesh** mesh, IN OUT uint32* group);

// <LegoRR.exe @00474f00>
bool32 __cdecl Container_Mesh_GetGroup(Container* cont, uint32 groupID,
								OUT uint32* vertexCount, OUT uint32* faceCount,
								OUT uint32* vPerFace, OUT uint32* faceDataSize,
								OUT uint32* faceData);

// <LegoRR.exe @00474f80>
uint32 __cdecl Container_Mesh_GetVertices(Container* cont, uint32 groupID, uint32 index,
								uint32 count, OUT Vertex* retArray);

// <LegoRR.exe @00474ff0>
uint32 __cdecl Container_Mesh_SetVertices(Container* cont, uint32 groupID, uint32 index,
								uint32 count, const Vertex* values);

// <LegoRR.exe @00475060>
void __cdecl Container_Mesh_SetTexture(Container* cont, uint32 groupID, Container_Texture* itext);

// <LegoRR.exe @004750f0>
void __cdecl Container_Mesh_SetPerspectiveCorrection(Container* cont, uint32 groupID, bool32 on);

// <LegoRR.exe @00475150>
bool32 __cdecl Container_Mesh_Scale(Container* cont, real32 x, real32 y, real32 z);

// <LegoRR.exe @004751d0>
bool32 __cdecl Container_Mesh_GetBox(Container* cont, OUT Box3F* box);

// <LegoRR.exe @004752b0>
void __cdecl Container_Mesh_SetEmissive(Container* cont, uint32 groupID,
								real32 r, real32 g, real32 b);

// <LegoRR.exe @004752e0>
void __cdecl Container_Mesh_SetColourAlpha(Container* cont, uint32 groupID,
								real32 r, real32 g, real32 b, real32 a);

// <LegoRR.exe @00475330>
void __cdecl Container_Transform(Container* cont, OUT Vector3F* dest, const Vector3F* src);

// <LegoRR.exe @00475350>
void __cdecl Container_InverseTransform(Container* cont, OUT Vector3F* dest, const Vector3F* src);

// <LegoRR.exe @00475370>
void __cdecl Container_SetColourAlpha(Container* cont, real32 r, real32 g, real32 b, real32 a);

// <LegoRR.exe @004753e0>
real32 __cdecl Container_MoveAnimation(Container* cont, real32 delta);

// <LegoRR.exe @00475400>
real32 __cdecl Container_SetAnimationTime(Container* cont, real32 time);

// <LegoRR.exe @004755c0>
void __cdecl Container_ForceAnimationUpdate(Container* cont);

// <LegoRR.exe @00475650>
real32 __cdecl Container_GetAnimationTime(Container* cont);

// <LegoRR.exe @004756b0>
uint32 __cdecl Container_GetAnimationFrames(Container* cont);

// <LegoRR.exe @004756f0>
void __cdecl Container_SetPosition(Container* cont, OPTIONAL Container* ref,
								real32 x, real32 y, real32 z);

// <LegoRR.exe @00475730>
void __cdecl Container_SetOrientation(Container* cont, OPTIONAL Container* ref,
								real32 dirx, real32 diry, real32 dirz, real32 upx, real32 upy, real32 upz);

// <LegoRR.exe @00475780>
void __cdecl Container_GetPosition(Container* cont, OPTIONAL Container* ref, OUT Vector3F* pos);

// <LegoRR.exe @004757c0>
void __cdecl Container_GetOrientation(Container* cont, OPTIONAL Container* ref, OPTIONAL OUT Vector3F* dir, OPTIONAL OUT Vector3F* up);

// <LegoRR.exe @00475840>
void __cdecl Container_AddRotation(Container* cont, Container_Combine combine,
								real32 x, real32 y, real32 z, real32 angle);

// <LegoRR.exe @00475870>
void __cdecl Container_AddScale(Container* cont, Container_Combine combine,
								real32 x, real32 y, real32 z);

// <LegoRR.exe @004758a0>
void __cdecl Container_AddTranslation(Container* cont, Container_Combine combine,
								real32 x, real32 y, real32 z);

// <LegoRR.exe @004758d0>
void __cdecl Container_ClearTransform(Container* cont);

// <LegoRR.exe @00475970>
void __cdecl Container_AddTransform(Container* cont, Container_Combine combine,
								const Matrix4F mat);

// <LegoRR.exe @00475990>
real32 __cdecl Container_GetZXRatio(Container* cont);

// <LegoRR.exe @004759d0>
void __cdecl Container_SetParent(Container* child, OPTIONAL Container* parent);

// <LegoRR.exe @00475a60>
Container* __cdecl Container_GetParent(Container* child);

// <LegoRR.exe @00475ab0>
real32 __cdecl Container_GetTransCoef(Container* cont);

// <LegoRR.exe @00475af0>
Container* __cdecl Container_SearchOwner(IDirect3DRMFrame3* frame);

// <LegoRR.exe @00475b40>
Container* __cdecl Container_Frame_GetContainer(IDirect3DRMFrame3* frame);

// <LegoRR.exe @00475bc0>
void __cdecl Container_GetFrames(Container* cont, OPTIONAL Container* ref, OUT IDirect3DRMFrame3** contFrame, OUT IDirect3DRMFrame3** refFrame);


// <LegoRR.exe @00475bf0>
Container_Type __cdecl Container_ParseTypeString(const char* str, OUT bool32* noTexture);

// <LegoRR.exe @00475cb0>
void __cdecl Container_AddList(void);

// <LegoRR.exe @00475d30>
uint32 __cdecl Container_GetActivities(Container* cont, OUT IDirect3DRMFrame3** frameList, OUT AnimClone** acList, OUT char** nameList);

// <LegoRR.exe @00475ec0>
void __cdecl Container_SetTypeData(Container* cont, OPTIONAL char* name, OPTIONAL IDirect3DRMLight* light, OPTIONAL IDirect3DRMMesh* mesh, OPTIONAL Mesh* transMesh);

// <LegoRR.exe @00475f40>
void __cdecl Container_FreeTypeData(Container* cont);


// <LegoRR.exe @00475fd0>
bool32 __cdecl Container_AddActivity2(Container* cont, const char* filename, const char* actname, real32 transCo, uint32 trigger, const char* sample, AnimClone* origClone, bool32 lws, bool32 looping);

// <LegoRR.exe @004760d0>
void __cdecl Container_Frame_ReferenceDestroyCallback(LPDIRECT3DRMOBJECT lpD3DRMobj, LPVOID lpArg);

// <LegoRR.exe @00476100>
IDirect3DRMFrame3* __cdecl Container_Frame_Find(Container* cont, const char* findName, bool32 /*uint32*/ hidden);

// <LegoRR.exe @00476230>
void __cdecl Container_Frame_SetAppData(IDirect3DRMFrame3* frame, Container* owner,
	OPTIONAL AnimClone* animClone, OPTIONAL const char* asfname, OPTIONAL uint32* frameCount,
	OPTIONAL const char* frameName, OPTIONAL real32* currTime, OPTIONAL real32* transCo,
	OPTIONAL const char* actSample, OPTIONAL void* soundRecord, OPTIONAL uint32* trigger);

// <LegoRR.exe @004763a0>
void __cdecl Container_Frame_RemoveAppData(IDirect3DRMFrame3* frame);

// <LegoRR.exe @004763e0>
Container* __cdecl Container_Frame_GetOwner(IDirect3DRMFrame3* frame);


// <LegoRR.exe @00476400>
const char* __cdecl Container_Frame_GetAnimSetFileName(IDirect3DRMFrame3* frame);

// <LegoRR.exe @00476420>
AnimClone* __cdecl Container_Frame_GetAnimClone(IDirect3DRMFrame3* frame);

// <LegoRR.exe @00476440>
uint32 __cdecl Container_Frame_GetFrameCount(IDirect3DRMFrame3* frame);

// <LegoRR.exe @00476460>
real32 __cdecl Container_Frame_GetCurrTime(IDirect3DRMFrame3* frame);

// <LegoRR.exe @00476480>
real32 __cdecl Container_Frame_GetTransCo(IDirect3DRMFrame3* frame);

// <LegoRR.exe @004764a0>
const char* __cdecl Container_Frame_GetSample(IDirect3DRMFrame3* frame);

// <LegoRR.exe @004764c0>
uint32 __cdecl Container_Frame_GetTrigger(IDirect3DRMFrame3* frame);

// <LegoRR.exe @004764e0>
void __cdecl Container_Frame_SafeAddChild(IDirect3DRMFrame3* parent, IDirect3DRMFrame3* child);

// <LegoRR.exe @00476530>
void __cdecl Container_Frame_FormatName(IDirect3DRMFrame3* frame, const char* msg, ...);

// <LegoRR.exe @004765b0>
void __cdecl Container_Frame_FreeName(IDirect3DRMFrame3* frame);

// <LegoRR.exe @004765d0>
const char* __cdecl Container_Frame_GetName(IDirect3DRMFrame3* frame);

// <LegoRR.exe @004765f0>
bool32 __cdecl Container_Frame_WalkTree(IDirect3DRMFrame3* frame, uint32 level,
									ContainerWalkTreeCallback Callback, void* data);

// <LegoRR.exe @004766d0>
bool32 __cdecl Container_Frame_SearchCallback(IDirect3DRMFrame3* frame, void* data);

// <LegoRR.exe @00476880>
AnimClone* __cdecl Container_LoadAnimSet(const char* fname, IDirect3DRMFrame3* frame, OUT uint32* frameCount, bool32 lws, bool32 looping);

// <LegoRR.exe @00476a30>
uint32 __cdecl Container_GetAnimFileFrameCount(const char* fileData);

// <LegoRR.exe @00476aa0>
bool32 __cdecl Container_FrameLoad(const char* fname, IDirect3DRMFrame3* frame);

// <LegoRR.exe @00476b10>
IDirect3DRMMesh* __cdecl Container_MeshLoad(void* file_data, uint32 file_size, const char* file_name, IDirect3DRMFrame3* frame, bool32 noTexture);

// <LegoRR.exe @00476bc0>
HRESULT __cdecl Container_TextureLoadCallback(char* name, void* data, LPDIRECT3DRMTEXTURE3* texture);

// <LegoRR.exe @00476eb0>
void __cdecl Container_YFlipTexture(IDirect3DRMTexture3* texture);

// std::qsort callback
// <LegoRR.exe @00476fa0>
int __cdecl Container_TextureSetSort(const void* a, const void* b);
//int __cdecl Container_TextureSetSort(Container_TextureRef* a, Container_TextureRef* b);

// <LegoRR.exe @00476fd0>
void __cdecl Container_TextureDestroyCallback(LPDIRECT3DRMOBJECT lpD3DRMobj, LPVOID lpArg);

#pragma endregion

}
