// Viewports.cpp : 
//

#include "../../platform/d3drm.h"

#include "../drawing/DirectDraw.h"
#include "../core/Errors.h"
#include "../core/Maths.h"
#include "../core/Memory.h"
#include "../util/Dxbug.h"
#include "../Graphics.h"
#include "Containers.h"
#include "Mesh.h"

#include "Viewports.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @0076bce0>
Gods98::Viewport_Globs & Gods98::viewportGlobs = *(Gods98::Viewport_Globs*)0x0076bce0; // (no init)

Gods98::Viewport_ListSet Gods98::viewportListSet = Gods98::Viewport_ListSet(Gods98::viewportGlobs);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00477010>
void __cdecl Gods98::Viewport_Initialise(void)
{
	log_firstcall();

	viewportListSet.Initialise();
	viewportGlobs.flags = Viewport_GlobFlags::VIEWPORT_GLOB_FLAG_INITIALISED;
}

// <LegoRR.exe @00477040>
void __cdecl Gods98::Viewport_Shutdown(void)
{
	log_firstcall();

	Viewport_RemoveAll();

	viewportListSet.Shutdown();
	viewportGlobs.flags = Viewport_GlobFlags::VIEWPORT_GLOB_FLAG_NONE;
}

// <LegoRR.exe @00477080>
Gods98::Viewport* __cdecl Gods98::Viewport_Create(real32 xPos, real32 yPos, real32 width, real32 height, Container* camera)
{
	log_firstcall();

	uint32 devWidth  = lpDevice()->GetWidth();
	uint32 devHeight = lpDevice()->GetHeight();
	
	/// SANITY: Cast to sint32 for xPos,yPos
	sint32 actXPos = (sint32)(xPos * devWidth);
	sint32 actYPos = (sint32)(yPos * devHeight);
	uint32 actWidth  = (uint32)(width  * devWidth);
	uint32 actHeight = (uint32)(height * devHeight);

	return Viewport_CreatePixel(actXPos, actYPos, actWidth, actHeight, camera);
}

// <LegoRR.exe @00477110>
Gods98::Viewport* __cdecl Gods98::Viewport_CreatePixel(sint32 xPos, sint32 yPos, uint32 width, uint32 height, Container* camera)
{
	log_firstcall();

	Viewport_CheckInit();

	uint32 devWidth  = lpDevice()->GetWidth();
	uint32 devHeight = lpDevice()->GetHeight();

	// This is only useful if xPos or yPos is >= negative devWidth, or devHeight.
	// Consider while < 0 or modulus operation instead?
	if (xPos < 0) xPos += devWidth;
	if (yPos < 0) yPos += devHeight;

	if (xPos + width > devWidth || yPos + height > devHeight) {
		Error_Warn(true, "Invalid sizes provided to Viewport_CreatePixel()");
		return nullptr;
	}

	/// CHANGE: newViewport now has its nextFree field assigned to itself, like all other listSets.
	Viewport* newViewport = viewportListSet.Add();
	ListSet::MemZero(newViewport); // Clear memory before returning.

	if (lpD3DRM()->CreateViewport(lpDevice(), camera->masterFrame, xPos, yPos, width, height, &newViewport->lpVP) == D3DRM_OK) {

		/// X86: 32-bit pointer
		newViewport->lpVP->SetAppData((DWORD)newViewport);

		newViewport->smoothFOV = 0.0f;
		return newViewport;
	}
	else Error_Warn(true, "CreateViewport() call failed");

	/// SANITY: Proper cleanup on failure. Don't need to call Viewport_Remove since there's no lpVP to release.
	viewportListSet.Remove(newViewport);

	return nullptr;
}

// <LegoRR.exe @004771d0>
void __cdecl Gods98::Viewport_GetSize(Viewport* vp, OPTIONAL OUT uint32* width, OPTIONAL OUT uint32* height)
{
	log_firstcall();

	Viewport_CheckInit();

	Error_Fatal(!vp, "NULL passed as viewport to Viewport_Configure()");

	if (width)  *width  = vp->lpVP->GetWidth();
	if (height) *height = vp->lpVP->GetHeight();
}

// <LegoRR.exe @00477210>
void __cdecl Gods98::Viewport_SetCamera(Viewport* vp, Container* cont)
{
	log_firstcall();

	Viewport_CheckInit();

	Error_Fatal(!vp||!cont, "NULL passed as viewport or container to Viewport_SetCamera()");

	// Does it matter that a non-camera container can be used as a camera????

	if (vp->lpVP->SetCamera(cont->masterFrame) == D3DRM_OK) {

	}
	else Error_Warn(true, "Cannot set container as camera");
}

// <LegoRR.exe @00477230>
Gods98::Container* __cdecl Gods98::Viewport_GetCamera(Viewport* vp)
{
	log_firstcall();

	Container* camera = nullptr;

	Viewport_CheckInit();

	Error_Fatal(!vp, "NULL passed as viewport or container to Viewport_SetCamera()");

	// Does it matter that a non-camera container can be used as a camera????

	IDirect3DRMFrame3* frame;
	if (vp->lpVP->GetCamera(&frame) == D3DRM_OK) {
		Container_AppData* appData = (Container_AppData*)frame->GetAppData();
		if (appData) camera = appData->ownerContainer;
		frame->Release();
	}
	else Error_Warn(true, "Cannot set container as camera");

	return camera;
}

// <LegoRR.exe @00477270>
void __cdecl Gods98::Viewport_SetBackClip(Viewport* vp, real32 dist)
{
	log_firstcall();

	Viewport_CheckInit();

	if (vp->lpVP->SetBack(dist) == D3DRM_OK) {

	}
	else Error_Warn(true, "Cannot set back clipping plane distance");
}

// <LegoRR.exe @00477290>
real32 __cdecl Gods98::Viewport_GetBackClip(Viewport* vp)
{
	Viewport_CheckInit();

	return vp->lpVP->GetBack();
}

// <LegoRR.exe @004772b0>
real32 __cdecl Gods98::Viewport_GetFrontClip(Viewport* vp)
{
	log_firstcall();

	Viewport_CheckInit();

	return vp->lpVP->GetFront();
}

// <LegoRR.exe @004772d0>
void __cdecl Gods98::Viewport_Clear(Viewport* vp, bool32 full)
{
	log_firstcall();

	Viewport_CheckInit();

	if (full) {
		// Viewport_GetScene doesn't add to RefCount. No need to Release scene afterwards.
		IDirect3DRMFrame3* scene = Viewport_GetScene(vp);
		D3DCOLOR colour = 0;
		/// REFACTOR: Moved this call above area assignment to group with other related calls.
		if (scene) colour = scene->GetSceneBackground();

		Area2F area = {
			(real32)vp->lpVP->GetX(),
			(real32)vp->lpVP->GetY(),
			(real32)vp->lpVP->GetWidth(),
			(real32)vp->lpVP->GetHeight(),
		};

		// Probably an old clear method before being replaced with the code below.
//		DirectDraw_Clear(&area, colour);
//		vp->lpVP->Clear(D3DRMCLEAR_ZBUFFER);

		/// FIXME: Cast from float to unsigned
		D3DRECT rect = { // sint32 casts for width,height to stop compiler from complaining
			(sint32)area.x,
			(sint32)area.y,
			(sint32)(uint32)(area.x + area.width),
			(sint32)(uint32)(area.y + area.height),
		};

		IDirect3DViewport* view1;
		IDirect3DViewport3* view3;
		HRESULT r = vp->lpVP->GetDirect3DViewport(&view1);
		r = view1->QueryInterface(IID_IDirect3DViewport3, (void**)&view3);
		view1->Release();

		// 1    = rectangle in array to clear: &rect
		// 1.0f = depth to fill Z buffer with
		// 0    = value to fill stencil bits with
		r = view3->Clear2(1, &rect, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, colour, 1.0f, 0);
		view3->Release();

	}
	else {
		vp->lpVP->Clear(D3DRMCLEAR_ALL);

	}
}

// <LegoRR.exe @00477410>
void __cdecl Gods98::Viewport_Render(Viewport* vp, Container* root, real32 delta)
{
	log_firstcall();

	Viewport_CheckInit();

	Error_Fatal(!vp || !root, "NULL passed as viewport or container to Viewport_Render()");

	vp->rendering = true;

	/// FIX APPLY: Avoid divide-by-zero during pause by also checking delta.
	if (vp->smoothFOV != 0.0f && delta != 0.0f) {

		real32 fov = vp->lpVP->GetField();

		real32 coef = 4.0f * (1.0f / delta);
		fov *= coef;
		fov += vp->smoothFOV;
		fov *= 1.0f / (1.0f + coef);

		vp->lpVP->SetField(fov);
		if (fov == vp->smoothFOV) vp->smoothFOV = 0.0f;

	}

	HRESULT r;
	if ((r = vp->lpVP->Render(root->masterFrame)) == D3DRM_OK) {

		Mesh_PostRenderAll(vp);

	}
	else {
		Error_Warn(true, "Cannot render viewport");
		CHKRM(r);
	}

	vp->rendering = false;
}

// <LegoRR.exe @004774e0>
void __cdecl Gods98::Viewport_Remove(Viewport* dead)
{
	log_firstcall();

	Viewport_CheckInit();
	Error_Fatal(!dead, "NULL passed to Viewport_Remove()");

	dead->lpVP->Release();

	viewportListSet.Remove(dead);
}

// (Field of View, FOV)
// <LegoRR.exe @00477500>
void __cdecl Gods98::Viewport_SmoothSetField(Viewport* vp, real32 fov)
{
	log_firstcall();

	vp->smoothFOV = fov;
}

// (Field of View, FOV)
// <LegoRR.exe @00477510>
void __cdecl Gods98::Viewport_SetField(Viewport* vp, real32 fov)
{
	log_firstcall();

	Viewport_CheckInit();
	Error_Fatal(!vp, "NULL passed to Viewport_SetField()");

	vp->lpVP->SetField(fov);
	vp->smoothFOV = 0.0f;
}

// <LegoRR.exe @00477530>
real32 __cdecl Gods98::Viewport_GetField(Viewport* vp)
{
	log_firstcall();

	Viewport_CheckInit();
	Error_Fatal(!vp, "NULL passed to Viewport_GetField()");

	return vp->lpVP->GetField();
}

// <LegoRR.exe @00477550>
void __cdecl Gods98::Viewport_InverseTransform(Viewport* vp, OUT Vector3F* dest, const Vector4F* src)
{
	log_firstcall();

	Viewport_CheckInit();
	Error_Fatal(!vp, "NULL passed to Viewport_InverseTransform()");
	
	vp->lpVP->InverseTransform((LPD3DVECTOR)dest, (LPD3DRMVECTOR4D)const_cast<Vector4F*>(src));
}

// <LegoRR.exe @00477570>
void __cdecl Gods98::Viewport_Transform(Viewport* vp, OUT Vector4F* dest, const Vector3F* src)
{
	log_firstcall();

	Viewport_CheckInit();
	Error_Fatal(!vp, "NULL passed to Viewport_InverseTransform()");
	
	vp->lpVP->Transform((LPD3DRMVECTOR4D)dest, (LPD3DVECTOR)const_cast<Vector3F*>(src));
}

// <LegoRR.exe @00477590>
Point2F* __cdecl Gods98::Viewport_WorldToScreen(Viewport* vp, OUT Point2F* screen, const Vector3F* world)
{
	log_firstcall();

	Vector4F v4d;

	Viewport_Transform(vp, &v4d, world);
	screen->x = v4d.x / v4d.w;
	screen->y = v4d.y / v4d.w;

	return screen;
}

// <LegoRR.exe @004775d0>
IDirect3DRMFrame3* __cdecl Gods98::Viewport_GetScene(Viewport* vp)
{
	log_firstcall();

	IDirect3DRMFrame3* camera = nullptr;
	IDirect3DRMFrame3* scene = nullptr;

	vp->lpVP->GetCamera(&camera);
	if (camera) {
		camera->Release();
		camera->GetScene(&scene);
		scene->Release();
	}

	return scene;
}

// <LegoRR.exe @00477630>
void __cdecl Gods98::Viewport_AddList(void)
{
	// NOTE: This function is no longer called, viewportListSet.Add already handles this.
	viewportListSet.AddList();
}

// <LegoRR.exe @004776a0>
void __cdecl Gods98::Viewport_RemoveAll(void)
{
	log_firstcall();

	for (Viewport* viewport : viewportListSet.EnumerateAlive()) {
		Viewport_Remove(viewport);
	}
}

#pragma endregion
