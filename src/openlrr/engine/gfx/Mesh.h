// Mesh.h : 
//
/// FILEIO: Shared:Data[wad,std,cd]
/// APIS: IDirect3DRM3, IDirect3DRMFrame3, IDirect3DRMUserVisual, IDirect3DRMViewport[12],
///       IDirect3D3, IDirect3DDevice[23], IDirect3DTexture2, IDirect3DViewport[13],
///       IDirectDrawSurface4
/// DEPENDENCIES: Containers, Files, Lwt, Main, Maths, Utils, Viewports, (Errors, Memory)
/// DEPENDENTS: 

#pragma once

// no getting around this include without some very ugly work-arounds :(
#include "../../platform/d3drm.h"		// <d3d.h>

#include "../../common.h"
#include "../geometry.h"
#include "../colour.h"
#include "../core/ListSet.hpp"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct IDirect3DRM3;
struct IDirect3DRMDevice3;
struct IDirect3DRMFrame3;
struct IDirect3DRMUserVisual;
struct IDirect3DRMViewport;
struct IDirect3DRMViewport2;
struct IDirect3DTexture2;
struct IDirectDrawSurface4;
enum LightWave_TexFlags : uint32;
enum LightWave_SurfFlags : uint32;
enum _D3DTRANSFORMSTATETYPE;
typedef enum _D3DTRANSFORMSTATETYPE D3DTRANSFORMSTATETYPE;
enum _D3DTEXTURESTAGESTATETYPE;
typedef enum _D3DTEXTURESTAGESTATETYPE D3DTEXTURESTAGESTATETYPE;
//typedef unsigned long DWORD;
//typedef DWORD D3DMATERIALHANDLE;
struct IDirect3DRMDevice;
typedef struct IDirect3DRMDevice* LPDIRECT3DRMDEVICE;
typedef struct IDirect3DRMViewport* LPDIRECT3DRMVIEWPORT;
typedef struct IDirect3DRMUserVisual* LPDIRECT3DRMUSERVISUAL;

enum _D3DRMUSERVISUALREASON;
typedef enum _D3DRMUSERVISUALREASON D3DRMUSERVISUALREASON;

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct Mesh;
struct Container_Texture;
struct APPOBJ;
struct Viewport;

#pragma endregion

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

/// NEW GODS98: Not present in legoRR (included for completeness)
typedef sint32 (__cdecl* MeshTextureRenderCallback)(void* data);

typedef void (__cdecl* MeshRenderCallback)(Mesh* mesh, void* data, Viewport* vp);

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define MESH_MAXTEXTURESEQENCE		100

#define MESH_MAXLISTS				20

#define MESH_TEXTURELISTSIZE		2048

#define MESH_DEFLISTSIZE			10
#define MESH_LISTINCREASE			150			// Increase list size by 150% when it is full...

#define MESH_MAXTEXTURESTAGESTATES	50

#define MESH_UVREALLOCSIZE			10

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

flags_scoped(MeshFlags) : uint32
{
	MESH_FLAG_NONE                 = 0,

	MESH_FLAG_HIDDEN               = 0x1,        // Mesh, Group
	MESH_FLAG_POSTEFFECT           = 0x2,        // Mesh
	MESH_FLAG_LWO                  = 0x4,        // Mesh

	MESH_FLAG_ALPHAENABLE          = 0x80000,    // Group
	MESH_FLAG_TEXTURECOLOURONLY    = 0x100000,   // Group
	MESH_FLAG_HASBEENCLONED        = 0x200000,   // Mesh
	MESH_FLAG_TRANSTEXTURE         = 0x400000,   // Group

	MESH_FLAG_FACECAMERA           = 0x1000000,  // Mesh
	MESH_FLAG_FACECAMERADONE       = 0x2000000,  // Mesh

	MESH_FLAG_ALPHAHIDDEN          = 0x10000000, // Group
};
flags_scoped_end(MeshFlags, 0x4);
//typedef MeshFlags Mesh_GroupFlags;


flags_scoped(Mesh_RenderFlags) : uint32
{
	MESH_RENDER_FLAG_NONE          = 0,

	MESH_RENDER_FLAG_XYZ           = 0x2,   // = D3DFVF_XYZ,
	MESH_RENDER_FLAG_NORMAL        = 0x10,  // = D3DFVF_NORMAL,
	MESH_RENDER_FLAG_TEX1          = 0x100, // = D3DFVF_TEX1,
	MESH_RENDER_FLAG_RESERVED1     = 0x200, // = D3DFVF_RESERVED1, // (unused)

//	MESH_FLAG_ZB_ENABLE            = 0x100, // (commented out define, conflicts with D3DFVF_TEX1)
//	MESH_FLAG_ZB_WRITE             = 0x200, // (commented out define, conflicts with D3DFVF_RESERVED1)

	MESH_RENDER_FLAG_ALPHA11       = 0x400,
	MESH_RENDER_FLAG_ALPHASA1      = 0x800,
	MESH_RENDER_FLAG_ALPHATRANS    = 0x1000,
	MESH_RENDER_FLAG_ALPHASA0      = 0x20000000,
	MESH_RENDER_FLAG_ALLALPHA      = (MESH_RENDER_FLAG_ALPHA11|MESH_RENDER_FLAG_ALPHASA1|MESH_RENDER_FLAG_ALPHATRANS|MESH_RENDER_FLAG_ALPHASA0), // (mask)
	MESH_TRANSFORM_FLAG_PARENTPOS  = 0x2000,
	MESH_TRANSFORM_FLAG_IDENTITY   = 0x4000,
	MESH_RENDER_FLAG_STIPPLE       = 0x8000,  // (unused)
	MESH_RENDER_FLAG_ALPHAMOD      = 0x10000, // (unused)
	MESH_RENDER_FLAG_ALPHATEX      = 0x20000,
	MESH_RENDER_FLAG_ALPHADIFFUSE  = 0x40000,

	MESH_RENDER_FLAG_FILTERNEAREST = 0x800000,

	MESH_RENDER_FLAG_DOUBLESIDED   = 0x4000000,
	MESH_RENDER_FLAG_HIGHZBIAS     = 0x8000000, // (commented out in code)

	MESH_RENDERFLAGS_LWONORM       = (MESH_TRANSFORM_FLAG_PARENTPOS),
	MESH_RENDERFLAGS_LWOALPHA      = (MESH_TRANSFORM_FLAG_PARENTPOS|MESH_RENDER_FLAG_ALPHATRANS),

	//MESH_RENDERFLAGS_DEFAULT       = (MESH_RENDER_FLAG_XYZ|MESH_RENDER_FLAG_NORMAL|MESH_RENDER_FLAG_TEX1|MESH_RENDER_FLAG_RESERVED1), // (commented out define)
	MESH_RENDERFLAGS_DEFAULT       = (MESH_RENDER_FLAG_XYZ|MESH_RENDER_FLAG_NORMAL|MESH_RENDER_FLAG_TEX1), // = D3DFVF_VERTEX,
};
flags_scoped_end(Mesh_RenderFlags, 0x4);


enum class Mesh_Colour : sint32
{
	Diffuse  = 0,
	Ambient  = 1,
	Specular = 2,
	Emissive = 3,

	Alpha    = 4,
	Power    = 5,
};
assert_sizeof(Mesh_Colour, 0x4);


enum class Mesh_Type : sint32
{
	Norm            = 0,
	PostEffect      = 1,
	LightWaveObject = 2,
};
assert_sizeof(Mesh_Type, 0x4);


enum class Mesh_WrapType : sint32
{
	XAxis = 0,
	YAxis = 1,
	ZAxis = 2,
};
assert_sizeof(Mesh_WrapType, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Typedefs
 **********************************************************************************/

#pragma region Typedefs

//struct Container_Texture;
//typedef struct Container_Texture *lpMesh_Texture;

typedef struct Container_Texture Mesh_Texture;

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Mesh_LightWave_Surface
{
	/*00,4*/ Mesh_Texture** textureSeq;
	/*04,4*/ Mesh_Texture* texture;
	/*08,4*/ uint32 numInTexSeq;
	/// NOTE: Changed from Vector4F to ColourRGBAF
	/*0c,10*/ ColourRGBAF colour;
	///*0c,10*/ Vector4F colour;
	/*1c,4*/ real32 diffuse;
	/*20,4*/ real32 emissive;
	/*24,4*/ real32 specular;
	/*28,4*/ real32 power;
	/*2c,4*/ real32 transparency;
	/*30,4*/ LightWave_SurfFlags flags; // lwt.h flags
	/*34,4*/ LightWave_TexFlags texFlags;
	/*38,4*/ sint32 texSeqOffset;
	/*3c*/
};
assert_sizeof(Mesh_LightWave_Surface, 0x3c);


// same struct layout/usage as Graphics_StateChangeData
struct Mesh_TextureStateChangeData
{
	/*0,4*/ uint32 origValue; // type not guaranteed
	/*4,4*/ bool32 changed;
	/*8*/
};
assert_sizeof(Mesh_TextureStateChangeData, 0x8);


struct Mesh_RenderDesc
{
	/*0,4*/ MeshRenderCallback renderCallback;
	/*4,4*/ void* renderCallbackData;
	/*8,4*/ Mesh_RenderFlags renderFlags;
	/*c*/
};
assert_sizeof(Mesh_RenderDesc, 0xc);


struct Mesh_Vertex
{
	/*00,c*/ Vector3F position;
	/*0c,c*/ Vector3F normal;
	//	uint32 dwDiffuseRGBA;
	/*18,4*/ real32 tu;
	/*1c,4*/ real32 tv;
	/*20*/
};
assert_sizeof(Mesh_Vertex, 0x20);


struct Mesh_Group
{
	/*00,4*/ uint32 faceDataSize;
	/*04,4*/ uint32 vertexCount;
	/*08,4*/ uint16* faceData;
	/*0c,4*/ Mesh_Vertex* vertices;
	/*10,50*/ D3DMATERIAL material;
	/*60,4*/ IDirect3DTexture2* imText;
	/*64,4*/ Mesh_RenderFlags renderFlags;
	/*68,4*/ Mesh_LightWave_Surface* lightWaveSurfaceInfo;
	/*6c,4*/ MeshFlags flags; // Group flags share common values, so we'll use the same enum type.
	/*70*/
};
assert_sizeof(Mesh_Group, 0x70);


struct Mesh
{
	/*00,4*/ uint32 groupCount;
	/*04,4*/ uint32 listSize;
	/*08,4*/ Mesh_Group* groupList;
	/*0c,4*/ IDirect3DRMUserVisual* uv;
	/*10,c*/ Mesh_RenderDesc renderDesc;
	/*1c,4*/ Mesh_LightWave_Surface* lightWaveSurf;
	/*20,4*/ uint32 numOfRefs;
	/*24,4*/ Mesh* clonedFrom;
	/*28,4*/ IDirect3DRMFrame3* frameCreatedOn;
	//BOX3D boundingBox;
	/*2c,4*/ MeshFlags flags;
	/*30,4*/ Mesh* nextFree;
	/*34*/
	/// NEW GODS98: Not part of LegoRR
	//MeshTextureRenderCallback textureRenderCallback;
	//void* textureRenderCallbackData;
};
assert_sizeof(Mesh, 0x34);


struct Mesh_PostRenderInfo
{
	/*00,4*/ Mesh* mesh;
	/*04,40*/ D3DMATRIX matWorld;
	/*44,4*/ Mesh_PostRenderInfo* next;
	/*48*/
};
assert_sizeof(Mesh_PostRenderInfo, 0x48);


struct Mesh_TextureReference
{
	/*0,4*/ IDirectDrawSurface4* surface;
	/*4,4*/ char* path;
	/*8,4*/ bool32 trans;
	/*c*/
};
assert_sizeof(Mesh_TextureReference, 0xc);


struct Mesh_Globs
{
	/*0000,4*/ Mesh* postRenderList;
	/*0004,4*/ Mesh_PostRenderInfo* postRenderMeshList;
	/*0008,190*/ Mesh_TextureStateChangeData stateData[MESH_MAXTEXTURESTAGESTATES];
	/*0198,4*/ char* sharedTextureDir;
	/*019c,6000*/ Mesh_TextureReference textureList[MESH_TEXTURELISTSIZE];
	/*619c,6000*/ Mesh_TextureReference textureListShared[MESH_TEXTURELISTSIZE];
	/*c19c,4*/ uint32 textureCount;
	/*c1a0,4*/ uint32 textureCountShared;
	/*c1a4,4*/ D3DTEXTUREHANDLE oldTextureRM;
	/*c1a8,4*/ D3DMATERIALHANDLE oldMatIM;
	/*c1ac,4*/ IDirect3DTexture2* oldTextureIM;
	/*c1b0,4*/ D3DTEXTUREHANDLE currTextureRM;
	/*c1b4,4*/ D3DMATERIALHANDLE currMatIM;
	/*c1b8,4*/ IDirect3DTexture2* currTextureIM;
	/*c1bc,4*/ D3DMATERIALHANDLE matHandle;
	/*c1c0,4*/ IDirect3DMaterial3* imMat;
	//uint32 ambientLight;
	/*c1c4,50*/ Mesh* listSet[MESH_MAXLISTS];
	/*c214,4*/ Mesh* freeList;
	/*c218,4*/ uint32 listCount;
	/*c21c*/
};
assert_sizeof(Mesh_Globs, 0xc21c);


using Mesh_ListSet = ListSet::WrapperCollection<Gods98::Mesh_Globs>;

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005353c0>
extern Mesh_Globs& meshGlobs;

extern Mesh_ListSet meshListSet;

#pragma endregion

/**********************************************************************************
 ******** Macro Functions
 **********************************************************************************/

#pragma region Macros


//#ifdef _DEBUG_2

#define Mesh_Debug_CheckIMDevice_Ptr(PTRTYPE)		{ if (lpIMDevice()==nullptr) return (PTRTYPE) 1; }
//#define Mesh_Debug_CheckIMDevice_Ptr()		{ if (lpIMDevice()==nullptr) return (LPVOID) 1; }
#define Mesh_Debug_CheckIMDevice_Void()		{ if (lpIMDevice()==nullptr) return; }
#define Mesh_Debug_CheckIMDevice_Int()		{ if (lpIMDevice()==nullptr) return 1; }
/*
#else

#define Mesh_Debug_CheckIMDevice_Ptr(PTRTYPE)
//#define Mesh_Debug_CheckIMDevice_Ptr()
#define Mesh_Debug_CheckIMDevice_Void()
#define Mesh_Debug_CheckIMDevice_Int()

#endif // _DEBUG_2
*/


#define Mesh_Create( c, f, rf, d )						Mesh_CreateOnFrame( (c->activityFrame), (f), (rf), (d), Gods98::Mesh_Type::Norm )
#define Mesh_CreatePostEffect( c, f, rf, d )			Mesh_CreateOnFrame( (c->activityFrame), (f), (rf), (d), Gods98::Mesh_Type::PostEffect )
#define Mesh_LoadLightWaveObject( n, c )				Mesh_Load( (n), (c->activityFrame) )
#define Mesh_RemoveFromContainer(m,c)					Mesh_Remove((m),(c)->activityFrame)

#define Mesh_SetWorldTransform( m )						Mesh_SetTransform( TRANSFORMSTATE_WORLD, (m) )

#define Mesh_SetGroupDiffuse( m, n, r, g, b )				Mesh_SetGroupColour( (m), (n), (r), (g), (b), Gods98::Mesh_Colour::Diffuse )
#define Mesh_SetGroupAmbient( m, n, r, g, b )				Mesh_SetGroupColour( (m), (n), (r), (g), (b), Gods98::Mesh_Colour::Ambient )
#define Mesh_SetGroupSpecular( m, n, r, g, b )				Mesh_SetGroupColour( (m), (n), (r), (g), (b), Gods98::Mesh_Colour::Specular )
#define Mesh_SetGroupEmissive( m, n, r, g, b )				Mesh_SetGroupColour( (m), (n), (r), (g), (b), Gods98::Mesh_Colour::Emissive )
#define Mesh_SetGroupPower( m, n, p )						Mesh_SetGroupMaterialValues( (m), (n), (p), Gods98::Mesh_Colour::Power )
#define Mesh_SetGroupAlpha( m, n, a )						Mesh_SetGroupMaterialValues( (m), (n), (a), Gods98::Mesh_Colour::Alpha )

#define Mesh_GetGroupDiffuse( m, n, r, g, b )				Mesh_GetGroupColour( (m), (n), (r), (g), (b), Gods98::Mesh_Colour::Diffuse )
#define Mesh_GetGroupAmbient( m, n, r, g, b )				Mesh_GetGroupColour( (m), (n), (r), (g), (b), Gods98::Mesh_Colour::Ambient )
#define Mesh_GetGroupSpecular( m, n, r, g, b )				Mesh_GetGroupColour( (m), (n), (r), (g), (b), Gods98::Mesh_Colour::Specular )
#define Mesh_GetGroupEmissive( m, n, r, g, b )				Mesh_GetGroupColour( (m), (n), (r), (g), (b), Gods98::Mesh_Colour::Emissive )
#define Mesh_GetGroupPower( m, n, p )						Mesh_GetGroupMaterialValues( (m), (n), (p), Gods98::Mesh_Colour::Power )
#define Mesh_GetGroupAlpha( m, n, a )						Mesh_GetGroupMaterialValues( (m), (n), (a), Gods98::Mesh_Colour::Alpha )

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00480870>
bool32 __cdecl Mesh_Initialise(const char* sharedTextureDir);

// <LegoRR.exe @00480910>
bool32 __cdecl Mesh_CreateGlobalMaterial(void);

// <LegoRR.exe @00480a40>
bool32 __cdecl Mesh_SetMaterial(D3DMATERIAL* newMaterial);

// <LegoRR.exe @00480a60>
Mesh* __cdecl Mesh_ObtainFromList(void);

// <LegoRR.exe @00480a90>
void __cdecl Mesh_ReturnToList(Mesh* dead);

// <LegoRR.exe @00480ab0>
void __cdecl Mesh_AddList(void);

// <LegoRR.exe @00480b30>
Mesh* __cdecl Mesh_CreateOnFrame(IDirect3DRMFrame3* frame, MeshRenderCallback renderFunc,
								 Mesh_RenderFlags renderFlags, void* data, Mesh_Type type);

// <LegoRR.exe @00480bc0>
Mesh* __cdecl Mesh_Clone(Mesh* mesh, IDirect3DRMFrame3* frame);

// <LegoRR.exe @00480ca0>
Mesh* __cdecl Mesh_Load(const char* fname, IDirect3DRMFrame3* frame, bool32 noTextures);

// <LegoRR.exe @00480d80>
bool32 __cdecl Mesh_ParseLWO(const char* basePath, Mesh* mesh, APPOBJ* lightWaveObject, bool32 noTextures);

// <LegoRR.exe @00481ae0>
void __cdecl Mesh_GetSurfInfo(const char* basePath, APPOBJ* lightWaveObject, Mesh_LightWave_Surface* lightWaveSurf/*[]*/, bool32 noTextures);

// <LegoRR.exe @00481d80>
bool32 __cdecl Mesh_GetTextureSeqInfo(const char* tname, OUT char* tfname, OUT uint32* tstart, OUT uint32* tnumlen);

// <LegoRR.exe @00481e40>
void __cdecl Mesh_GetNextInSequence(const char* baseName, OUT char* nextTextName, OUT uint32* texNum, uint32 tnumlen);

// <LegoRR.exe @00481f10>
void __cdecl Mesh_UViewMesh(APPOBJ* lightWaveObject, OUT Point2F* textCoords);

// flags = lwt.h srfTexFlags bit enumeration `TFM_*`
// <LegoRR.exe @00482260>
void __cdecl Mesh_GetTextureUVsWrap(uint32 vertexCount, OUT Vector3F* vertices, OUT Point2F* coords,
									real32 sx, real32 sy, real32 sz, real32 px, real32 py, real32 pz, LightWave_TexFlags flags);


/// NEW GODS98: API that includes mesh->textureRenderCallback
// Only calls Mesh_SetTextureTime2 if mesh->textureRenderCallback == null.
// This function serves no purpose for this version of the engine,
//  but is included regardless.
// <inlined>
bool32 __cdecl Mesh_SetTextureTime(Mesh* mesh, real32 frame);


// <LegoRR.exe @00482300>
bool32 __cdecl Mesh_SetTextureTime2(Mesh* mesh, real32 frame);

// <LegoRR.exe @00482390>
void __cdecl Mesh_Remove(Mesh* mesh, IDirect3DRMFrame3* frame);

// <LegoRR.exe @00482460>
void __cdecl Mesh_GetGroup(const Mesh* mesh, uint32 groupID, OUT uint32* vertexCount,
						   OUT uint32* faceCount, OUT uint32* vPerFace,
						   OUT uint32* faceDataSize, OUT uint32* faceData);

// <LegoRR.exe @004824d0>
uint32 __cdecl Mesh_GetGroupCount(const Mesh* mesh);

// <LegoRR.exe @004824e0>
sint32 __cdecl Mesh_AddGroup(Mesh* mesh, uint32 vertexCount, uint32 faceCount,
							 uint32 vPerFace, const uint32* faceData);

// <LegoRR.exe @00482610>
void __cdecl Mesh_AlterGroupRenderFlags(Mesh* mesh, uint32 groupID, Mesh_RenderFlags newFlags);

// <LegoRR.exe @00482630>
void __cdecl Mesh_Scale(Mesh* mesh, real32 x, real32 y, real32 z);

// <LegoRR.exe @004826a0>
void __cdecl Mesh_SetVertices(Mesh* mesh, uint32 groupID, uint32 index,
							  uint32 count, const Vertex* vertices);

// <LegoRR.exe @00482730>
void __cdecl Mesh_GetVertices(const Mesh* mesh, uint32 groupID, uint32 index,
							  uint32 count, OUT Vertex* vertices);

// <LegoRR.exe @004827c0>
void __cdecl Mesh_SetVertices_PointNormalAt(Mesh* mesh, uint32 groupID, uint32 index,
											uint32 count, const Vector3F* vertices, const Vector3F* position, real32(*textCoords)[2]);

// <LegoRR.exe @004828e0>
void __cdecl Mesh_SetVertices_SameNormal(Mesh* mesh, uint32 groupID, uint32 index,
										 uint32 count, const Vector3F* vertices, const Vector3F* normal, real32(*textCoords)[2]);

// <LegoRR.exe @00482980>
void __cdecl Mesh_SetVertices_VNT(Mesh* mesh, uint32 groupID, uint32 index, uint32 count,
								  const Vector3F* vertices, const Vector3F* const* normal, const Point2F* textCoords);

// <LegoRR.exe @00482a40>
bool32 __cdecl Mesh_IsGroupHidden(const Mesh* mesh, uint32 groupID);

// <LegoRR.exe @00482a60>
void __cdecl Mesh_HideGroup(Mesh* mesh, uint32 groupID, bool32 hide);

// <LegoRR.exe @00482a90>
void __cdecl Mesh_Hide(Mesh* mesh, bool32 hide);

// <LegoRR.exe @00482ab0>
BOOL __cdecl Mesh_RenderCallback(LPDIRECT3DRMUSERVISUAL lpD3DRMUV, LPVOID lpArg, D3DRMUSERVISUALREASON lpD3DRMUVreason, LPDIRECT3DRMDEVICE lpD3DRMDev, LPDIRECT3DRMVIEWPORT lpD3DRMview);

// <LegoRR.exe @00482d80>
void __cdecl Mesh_SetMeshRenderDesc(Mesh* mesh, Viewport* vp, const D3DMATRIX* matWorld, bool32 alphaBlend);

// <LegoRR.exe @00482e10>
void __cdecl Mesh_SetRenderDesc(Mesh_RenderFlags flags, const D3DMATRIX* matWorld, bool32 alphaBlend);

// <LegoRR.exe @00482f70>
void __cdecl Mesh_SetAlphaRender(D3DBLEND src, D3DBLEND dest);

// <LegoRR.exe @00482fa0>
void __cdecl Mesh_AddToPostRenderList(Mesh* mesh, OPTIONAL const D3DMATRIX* matWorld);

// <LegoRR.exe @00482ff0>
void __cdecl Mesh_ClearPostRenderList(void);

// <LegoRR.exe @00483020>
void __cdecl Mesh_PostRenderAll(Viewport* vp);

// <LegoRR.exe @00483130>
Mesh_Texture* __cdecl Mesh_LoadTexture(const char* baseDir, const char* name, OUT uint32* width, OUT uint32* height);

// <LegoRR.exe @004832f0>
Mesh_TextureReference* __cdecl Mesh_SearchTexturePathList(Mesh_TextureReference* list, uint32 count, const char* path);

// <LegoRR.exe @00483340>
void __cdecl Mesh_AddTexturePathEntry(Mesh_TextureReference* list, IN OUT uint32* count, const char* path, IDirectDrawSurface4* surface, bool32 trans);

// <LegoRR.exe @00483380>
void __cdecl Mesh_SetGroupTexture(Mesh* mesh, uint32 groupID, Mesh_Texture* mt);

// <LegoRR.exe @00483400>
void __cdecl Mesh_RemoveGroupTexture(Mesh* mesh, uint32 groupID);

// <LegoRR.exe @00483430>
bool32 __cdecl Mesh_CreateGroupMaterial(Mesh* mesh, uint32 groupID);

// <LegoRR.exe @00483500>
bool32 __cdecl Mesh_SetGroupMaterial(Mesh* mesh, uint32 groupID, const D3DMATERIAL* mat);

// <LegoRR.exe @00483530>
bool32 __cdecl Mesh_SetGroupColour(Mesh* mesh, uint32 groupID, real32 r, real32 g, real32 b, Mesh_Colour type);

// <LegoRR.exe @004836c0>
D3DMATERIAL* __cdecl Mesh_GetGroupMaterial(Mesh* mesh, uint32 groupID);

// <LegoRR.exe @004836e0>
bool32 __cdecl Mesh_SetGroupMaterialValues(Mesh* mesh, uint32 groupID, real32 value, Mesh_Colour type);

// <LegoRR.exe @00483800>
void __cdecl Mesh_SetIdentityMatrix(OUT Matrix4F m);

// <LegoRR.exe @00483840>
bool32 __cdecl Mesh_SetCurrentViewport(IDirect3DRMViewport* view);

// <LegoRR.exe @004838c0>
bool32 __cdecl Mesh_SetCurrentGODSViewport(Viewport* vp);

/// TODO: is this the correct matrix ptr level?
// <LegoRR.exe @00483950>
bool32 __cdecl Mesh_SetTransform(D3DTRANSFORMSTATETYPE state, const Matrix4F* matrix);
//bool32 __cdecl Mesh_SetTransform(D3DTRANSFORMSTATETYPE state, const Matrix4F matrix);

// <LegoRR.exe @00483ad0>
bool32 __cdecl Mesh_ChangeTextureStageState(D3DTEXTURESTAGESTATETYPE dwRenderStateType, uint32 dwRenderState);

// <LegoRR.exe @00483b70>
void __cdecl Mesh_StoreTextureAndMat(void);

// <LegoRR.exe @00483c00>
void __cdecl Mesh_RestoreTextureAndMat(void);

// <LegoRR.exe @00483c80>
bool32 __cdecl Mesh_RenderMesh(Mesh* mesh, const D3DMATRIX* matWorld, bool32 alphaBlend);

// <LegoRR.exe @00483d30>
bool32 __cdecl Mesh_CanRenderGroup(const Mesh_Group* group);

// <LegoRR.exe @00483d50>
bool32 __cdecl Mesh_RenderGroup(Mesh* mesh, Mesh_Group* group, const D3DMATRIX* matWorld, bool32 alphaBlend);

// <LegoRR.exe @00483dc0>
bool32 __cdecl Mesh_SetGroupRenderDesc(Mesh* mesh, Mesh_Group* group, const D3DMATRIX* matWorld, bool32 alphaBlend);

// <LegoRR.exe @00483e30>
bool32 __cdecl Mesh_RenderTriangleList(D3DMATERIALHANDLE matHandle, IDirect3DTexture2* texture, Mesh_RenderFlags renderFlags,
									   Mesh_Vertex* vertices, uint32 vertexCount, uint16* faceData, uint32 indexCount);


/*
/// NEW GODS98: Feature not present in LegoRR
// <unused>
void __cdecl Mesh_SetAmbientLight(real32 r, real32 g, real32 b);
*/

#pragma endregion

}
