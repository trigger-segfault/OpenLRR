// Animation.cpp : Definitions file for the C++ G98CAnimation implementation and C wrapper.
//

#include "../platform/timeapi.h"
#include "../platform/vfw.h"

#include "DirectDraw.h"
#include "Files.h"
#include "Main.h"

#include "Animation.hpp"
#include "Animation.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00534998>
Gods98::Animation_Globs & Gods98::animationGlobs = *(Gods98::Animation_Globs*)0x00534998;// = { 0 };

#pragma endregion

/**********************************************************************************
 ******** Class Functions
 **********************************************************************************/

#pragma region G98CSurface class definition

// <LegoRR.exe @0047e720>
// G98CSurface* Gods98::G98CSurface::deletor(uint8 flag)
// {
// 	this->~G98CSurface();
// 	if (flag & 0x1) {
// 		std::free(this);
// 		// (FUN_0045ddb0(void* ptr))
// 		//game::LevelStruct3C_Free_Unwind((LevelStruct_3c *)this) /*<LegoRR.exe @0045ddb0>*/;
// 	}
// 	return this;
// }

// <LegoRR.exe @0047e740>
Gods98::G98CSurface::~G98CSurface()
{
	//this->vftable = &G98CSurface___vftable /*<LegoRR.exe @0049f9a0>*/;

	// Release the surface.
	if (this->m_surf != nullptr) {
		this->m_surf->Release();
		this->m_surf = nullptr;
	}
	if (this->m_palette != nullptr) {
		this->m_palette->Release();
		this->m_palette = nullptr;
	}
	//Release((void*&)m_surf);
	//Release((void*&)m_palette);

	// Release DirectDraw reference
/*	if (ddraw) {
		LPVOID tempdd = ddraw;
		if (0 == (refCount = Release(tempdd))) {
			Info(true, "DirectDraw has been shutdown");
			ddraw = 0;
		}
	}*/
}

// <LegoRR.exe @0047e780>
Gods98::G98CSurface::G98CSurface(sint32 width, sint32 height, sint32 bpp, bool vram, bool fullScreen /*unused*/)
	: m_clipper(nullptr), m_surf(nullptr), m_squashToEdge(false), m_15bit(false)
{
	//this->vftable = &G98CSurface___vftable /*<LegoRR.exe @0049f9a0>*/;
	this->m_surfaceInited = false;
	this->PreInit(fullScreen);
	if (this->Init(width, height, bpp, vram))
		m_surfaceInited = true;
}

// <LegoRR.exe @0047e7d0>
bool Gods98::G98CSurface::Init(sint32 width, sint32 height, sint32 bpp, bool vram)
{
	this->m_desc.dwFlags |=
		(width  == 0 ? 0 : DDSD_WIDTH)  | 
		(height == 0 ? 0 : DDSD_HEIGHT) | 
		 DDSD_CAPS;

	this->m_bpp = bpp;
	if (width  != 0) this->m_desc.dwWidth  = width;
	if (height != 0) this->m_desc.dwHeight = height;
	if (!width && !height) {
		// We want the whole front buffer if no size specced.
		this->m_desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	} else {
		this->m_desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		if (vram) this->m_desc.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
		else this->m_desc.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}

	if (bpp != 0) {
		this->m_desc.dwFlags |= DDSD_PIXELFORMAT;
		this->m_desc.ddpfPixelFormat.dwSize = sizeof(this->m_desc.ddpfPixelFormat);
		this->m_desc.ddpfPixelFormat.dwRGBBitCount = (bpp == 15) ? 16 : bpp;
		if (bpp == 24) { // 24-bpp
			this->m_desc.ddpfPixelFormat.dwRBitMask = 0xff0000;
			this->m_desc.ddpfPixelFormat.dwGBitMask = 0x00ff00;
			this->m_desc.ddpfPixelFormat.dwBBitMask = 0x0000ff;
			this->m_desc.ddpfPixelFormat.dwFlags = DDPF_RGB;
		} else if (bpp == 16) { // 16-bpp (proper)
			this->m_desc.ddpfPixelFormat.dwRBitMask = 0xf800; // 0b1111100000000000
			this->m_desc.ddpfPixelFormat.dwGBitMask = 0x07e0; // 0b0000011111100000
			this->m_desc.ddpfPixelFormat.dwBBitMask = 0x001f; // 0b0000000000011111
			this->m_desc.ddpfPixelFormat.dwFlags = DDPF_RGB;
		} else if (bpp == 15) { // 15-bpp (treated as 16-bpp)
			this->m_desc.ddpfPixelFormat.dwRBitMask = 0x7c00; // 0b0111110000000000
			this->m_desc.ddpfPixelFormat.dwGBitMask = 0x03e0; // 0b0000001111100000
			this->m_desc.ddpfPixelFormat.dwBBitMask = 0x001f; // 0b0000000000011111
			this->m_desc.ddpfPixelFormat.dwFlags = DDPF_RGB;
		} else if (bpp == 8) { // 8-bpp (indexed)
			this->m_desc.ddpfPixelFormat.dwRBitMask = 0xff0000;		// I Doubt that the masks are valid 
			this->m_desc.ddpfPixelFormat.dwGBitMask = 0x00ff00;		// but wont seem to work otherwise - DDI
			this->m_desc.ddpfPixelFormat.dwBBitMask = 0x0000ff;
			this->m_desc.ddpfPixelFormat.dwFlags = DDPF_PALETTEINDEXED8 | DDPF_RGB;
		}
	}

	if (animationGlobs.ddraw->CreateSurface(&m_desc, &m_surf, nullptr) == DD_OK) {
		// Last thing is to lock the surface and get it info.
		this->Lock();
		this->Unlock();

		if (this->m_desc.ddpfPixelFormat.dwRGBBitCount == 8) {
			PALETTEENTRY pal[256];
			for (sint32 i=0; i<256; i++) {
				pal[i].peRed   = 0;
				pal[i].peGreen = 0;
				pal[i].peBlue  = 0;
				pal[i].peFlags = 0;
			}
			if (animationGlobs.ddraw->CreatePalette(DDPCAPS_8BIT, pal, &this->m_palette, nullptr) != DD_OK) {
				return false;
			}
			if (this->m_surf->SetPalette(this->m_palette) != DD_OK) {
				return false;
			}
		}
		else if (this->m_desc.ddpfPixelFormat.dwRGBBitCount == 16) {
			// Count number of bits to check if we're actually using 15-bpp handling.
			//  (when surface considers it as 16-bpp)
			sint32 maskCount;
			if ((maskCount = this->CountMaskBits(this->m_desc.ddpfPixelFormat.dwRBitMask) +
				this->CountMaskBits(this->m_desc.ddpfPixelFormat.dwGBitMask) +
				 this->CountMaskBits(this->m_desc.ddpfPixelFormat.dwBBitMask)) == 15) this->m_15bit = true;
		}
		return true;
	} else {
		return false;
	}
}

// <LegoRR.exe @0047e9d0>
void Gods98::G98CSurface::PreInit(bool fullScreen /*unused*/)
{
	if (!animationGlobs.ddraw) {

		///////////////////////
		// ROB THROW AN ERROR - must have ddraw
		///////////////////////

	}
	else {
		///////////////////////
		//
		// ROB - I AddRef ddraw and remove it in destructor
		//
		///////////////////////

	}

	std::memset(&this->m_desc, 0, sizeof(DDSURFACEDESC2));
	this->m_desc.dwSize = sizeof(DDSURFACEDESC2);
	this->m_surf = nullptr;
	this->m_palette = nullptr;
	this->m_surfaceLocked = false;
	this->m_colourKeyed = false;
}

// <LegoRR.exe @0047ea00>
uint32 Gods98::G98CSurface::Width() const
{
	return this->m_desc.dwWidth;
}

// <LegoRR.exe @0047ea10>
uint32 Gods98::G98CSurface::Height() const
{
	return this->m_desc.dwHeight;
}

// <LegoRR.exe @0047ea20>
bool Gods98::G98CSurface::Lock()
{
	if (!this->m_surfaceLocked) {
		std::memset(&this->m_desc, 0, sizeof(DDSURFACEDESC2));
		this->m_desc.dwSize = sizeof(DDSURFACEDESC2);
		if (this->m_surf->Lock(nullptr, &m_desc, DDLOCK_WAIT|DDLOCK_NOSYSLOCK, nullptr) == DD_OK) {
			this->m_surfaceLocked = true;
			return true;
		} else return false;
	} 
	return false;
}

// <LegoRR.exe @0047ea70>
bool Gods98::G98CSurface::Unlock()
{
	if (this->m_surfaceLocked) {
		if (this->m_surf->Unlock(nullptr) == DD_OK) {
			this->m_surfaceLocked = false;
			return true;
		} return false;
	}
	return false;
}

// <LegoRR.exe @0047eaa0>
sint32 Gods98::G98CSurface::CountMaskBits(uint32 mask) const
{
	uint32 count = 0;

	for (uint32 i = 0; i < (sizeof(mask) * 8); i++) {
		if (0 != (mask & (1 << i))) count++;
	}

	return count;
}

#pragma endregion


#pragma region G98CAnimation class definition

// <LegoRR.exe @0047eac0>
Gods98::G98CAnimation::G98CAnimation(const char* fName)
	: m_colourKeySet(false), m_init(false), m_movieSurf(nullptr)
{
	// /*this->bool_unused_98 = false;
	// this->isOpen = false;
	// this->renderer = nullptr;*/
	// //this->vftable = &G98CAnimation___vftable /*<LegoRR.exe @0049f9b0>*/;
	//const char* datadirFilename = File_VerifyFilename(fName);
	//this->aviFrame  = nullptr;
	//this->aviStream = nullptr;
	//std::memset(&this->streamInfo, 0, sizeof(this->streamInfo) /*0x8c*/);
	//
	//this->rect.top    = 0;
	//this->rect.bottom = 0;
	//this->rect.left   = 0;
	//this->rect.right  = 0;
	//
	//if (!globals::g_AVIPlayer_IsDisabled) {
	//	// fccType "sdiv" (as DWORD = 0x73646976) mmioFOURCC('v','i','d','s')
	//	DWORD fccType = streamtypeVIDEO /*mmioFOURCC('v','i','d','s') "sdiv"*/;
	//	if (legacy::AVIStreamOpenFromFileA(&this->aviStream, datadirFilename, fccType, 0, 0, nullptr) != 0)
	//		return; // failure
	//	
	//	this->aviFrame = legacy::AVIStreamGetFrameOpen(this->aviStream, (BITMAPINFOHEADER*)AVIGETFRAMEF_BESTDISPLAYFMT /*0x1*/);
	//	if (this->aviFrame == nullptr)
	//		return; // failure
	//
	//	if (legacy::AVIStreamInfoA(this->aviStream, &this->streamInfo, sizeof(this->streamInfo) /*0x8c*/) != 0)
	//		return; // failure
	//
	//	DWORD dwTime = legacy::timeGetTime();
	//	this->isOpen = true;
	//	this->position = 0;
	//	this->rect.top    = this->streamInfo.rcFrame.top;
	//	this->rect.bottom = this->streamInfo.rcFrame.bottom;
	//	this->rect.left   = this->streamInfo.rcFrame.left;
	//	this->rect.right  = this->streamInfo.rcFrame.right;
	//	this->startTime = (float)(unsigned long long)dwTime * 0.001f; // milliseconds (uint) -> seconds (float)
	//	this->renderer = new G98CSurface(this->rect.right - this->rect.left,
	//									 this->rect.bottom - this->rect.top,
	//									 0 /*no bpp override*/, false /*!useVideoMemory*/, 0 /*unused*/);
	//}


	//this->vftable = &G98CAnimation___vftable /*<LegoRR.exe @0049f9b0>*/;
	const char* name = File_VerifyFilename(fName);
	this->m_decompressFn = nullptr;
	this->m_aviStream = nullptr;
	std::memset(&this->m_aviStreamInfo, 0, sizeof(this->m_aviStreamInfo));

	this->m_movieRect.top    = 0;
	this->m_movieRect.bottom = 0;
	this->m_movieRect.left   = 0;
	this->m_movieRect.right  = 0;

	if (!animationGlobs.g98NoAvis) {
		if (legacy::AVIStreamOpenFromFileA(&m_aviStream, name, streamtypeVIDEO, 0, OF_READ, nullptr) == S_OK) {
			// Load the video stream
			if (this->m_decompressFn = legacy::AVIStreamGetFrameOpen(m_aviStream,
				(LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT)) {
				if (legacy::AVIStreamInfoA(m_aviStream, &m_aviStreamInfo, sizeof(AVISTREAMINFOA)) == S_OK) {
					// Get info - length, rectangle etc
					this->m_startTime = ((real32)Main_GetTime()) / 1000.0f; // milliseconds (uint) -> seconds (float)
					this->m_init = true;
					this->m_currFrame = 0;

					this->m_movieRect.top    = this->m_aviStreamInfo.rcFrame.top;
					this->m_movieRect.bottom = this->m_aviStreamInfo.rcFrame.bottom;
					this->m_movieRect.left   = this->m_aviStreamInfo.rcFrame.left;
					this->m_movieRect.right  = this->m_aviStreamInfo.rcFrame.right;
					this->m_movieSurf = new G98CSurface(this->m_movieRect.right - this->m_movieRect.left,
														this->m_movieRect.bottom - this->m_movieRect.top,
														0 /*bpp*/);
				}
			}
		}
	}
}

// <LegoRR.exe @0047ec40>
// G98CAnimation* Gods98::G98CAnimation::deletor(uint8 flag)
// {
// 	this->~G98CAnimation();
// 	if (flag & 0x1) {
// 		std::free(this);
// 		// (FUN_0045ddb0(void* ptr))
// 		//game::LevelStruct3C_Free_Unwind((LevelStruct_3c *)this) /*<LegoRR.exe @0045ddb0>*/;
// 	}
// 	return this;
// }

// <LegoRR.exe @0047ec60>
Gods98::G98CAnimation::~G98CAnimation()
{
	//this->vftable = &G98CAnimation___vftable /*<LegoRR.exe @0049f9b0>*/;

	if (!animationGlobs.g98NoAvis) {
		if (m_aviStream) legacy::AVIStreamRelease(m_aviStream);
		if (m_movieSurf) delete m_movieSurf;
	}
}

// <LegoRR.exe @0047eca0>
void Gods98::G98CAnimation::BMICopy(const BITMAPINFO* bmi)
{
	/*bool result = false;
	if (!this->renderer->isLocked && this->renderer->LockSurface())
		result = true;

	sint32 width  = this->rect.right - this->rect.left;
	sint32 height = this->rect.bottom - this->rect.top;
	sint32 stride = (sint32)this->renderer->surfDesc.lPitch;
	// sint32 stride_2 = (sint32)this->renderer->surfDesc.lPitch / 2;

	uint32 depth = 15;
	if (!this->renderer->isBPP15)
		depth = this->renderer->surfDesc.ddpfPixelFormat.dwRGBBitCount;

	// "Corporate needs you to find the differences between these two code blocks."
	// 
	// Originally 2 duplicate code blocks performing IDENTICAL functionality were here.
	//  First was `if (depth == 15)`, then second was check if depth is 15-bpp *again*,
	//  then do THE SAME loop `if (depth == 16)`.
	/// CHANGE: swap out the first `depth == 15` confition for `depth == 15 || depth == 16` in a single block

	/// LIMITATION: Only 16-bpp-type depths are supported here
	if (depth == 15 || depth == 16) { // 15-bpp (treated as 16-bpp) || 16-bpp (proper)
		// pointer to start of last row (we need to copy *and* vertically flip data)
		uint8* srcRow = (uint8*)((uint8*)frameData + *(sint32 *)frameData + (height - 1) * stride);
		uint8* dstRow = (uint8*)this->renderer->surfDesc.lpSurface;

		for (sint32 y = 0; y < height; y++) {
			std::memcpy(dstData, srcData, (size_t)(width * 2));

			srcRow -= stride; // previous row
			dstRow += stride; // next row
		}
	}

	if (result)
		result = this->renderer->UnlockSurface();*/

	//return result;

	G98CSurfaceLock lock(this->m_movieSurf);
	uint16* pSrc = (uint16*)(((uint8*)bmi) + bmi->bmiHeader.biSize);
	uint16* pDest = (uint16*)lock.Data();
	sint32 width, height;

	// Width and height of the bitmap (and indeed the destination surface)
	width = this->m_movieRect.right - this->m_movieRect.left;
	height = this->m_movieRect.bottom - this->m_movieRect.top;

	// Caluclate the dest surface skip
//	sint32 skipSrc  = ((m_movieSurf->Pitch() + 3) & ~3) / 2;
	sint32 skipSrc = this->m_movieSurf->Pitch() / 2;
	sint32 skipDest = this->m_movieSurf->Pitch() / 2;

	// Animations are ONLY gonna be done in 16/15 bit - note m_movieSurf is created in the primary display format (since no BPP was given).
	if (this->m_movieSurf->Bpp() == 15) {

		// Set the source to the last line
		pSrc += (height - 1) * skipSrc;

		for (sint32 i = 0; i < height; i++) {
			std::memcpy(pDest, pSrc, sizeof(uint16) * width);		// NEED LINE CONVERSION HERE I FEAR (BUT AM NOT SURE!)
			pSrc -= skipSrc;
			pDest += skipDest;
		}

	}
	else if (this->m_movieSurf->Bpp() == 16) {

		// Set the source to the last line
		pSrc += (height - 1) * skipSrc;

		for (sint32 i = 0; i < height; i++) {
			std::memcpy(pDest, pSrc, sizeof(uint16) * width);
			pSrc -= skipSrc;
			pDest += skipDest;
		}

	}
}

// <LegoRR.exe @0047ede0>
bool Gods98::G98CAnimation::Update()
{
	/// FIXME: never modified, true is always returned (same in LegoRR and Gods98)
	HRESULT r = S_OK;

	if (!this->m_init) return false;

	if (animationGlobs.g98NoAvis) return false;

	/// Get the pointer to the bitmap info
    BITMAPINFO* pbmi;
    if (pbmi = (BITMAPINFO*)legacy::AVIStreamGetFrame(this->m_decompressFn, this->m_currFrame)) {

		// Copy the frame to the surface
		this->BMICopy(pbmi);
//		G98CRect<sint32> r(destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top);
//		m_dest->Copy(0, m_movieSurf, &r);

		// Update to the next frame - do it here so that we can check the frame number to see if we have overrun
		this->m_currTime		   = ((real32)Main_GetTime()) / 1000.0f; // milliseconds (uint) -> seconds (float)
		real32 fElapsedTime = this->m_currTime - this->m_startTime;
		this->m_aviTimeScale	   = ((real32)this->m_aviStreamInfo.dwRate) / this->m_aviStreamInfo.dwScale;
		this->m_currFrame		   = (uint32) (fElapsedTime * this->m_aviTimeScale);

	}

	return (r == S_OK);// ? true : false;
}

// <LegoRR.exe @0047eea0>
uint32 Gods98::G98CAnimation::Length() const
{
	//sint32 len = 0; // (kept -but commented out- in-case this idiotically breaks things)
	if (animationGlobs.g98NoAvis) return 0;
	return (uint32)this->m_aviStreamInfo.dwLength;
}

// <LegoRR.exe @0047eeb0>
void Gods98::G98CAnimation::SetTime(uint32 time)
{
	if (animationGlobs.g98NoAvis) return;

	// Adjust the curr frame and start time so that the following frames time correctly.
	this->m_currFrame = time;
	this->m_currTime = (real32)Main_GetTime() / 1000.0f; // milliseconds (uint) -> seconds (float)
	this->m_startTime = this->m_currTime - ((real32)this->m_currFrame / this->m_aviTimeScale);
}

// <LegoRR.exe @0047ef20>
uint32 Gods98::G98CAnimation::GetTime() const
{
	sint32 time = this->m_currFrame;
	if (animationGlobs.g98NoAvis) return 0;
	return (uint32)time;
}

#pragma endregion


/**********************************************************************************
 ******** C Functions
 **********************************************************************************/

#pragma region C Functions

// <LegoRR.exe @0047ef40>
void __cdecl Gods98::Animation_Initialise(IDirectDraw4* directDraw)
{
	log_firstcall();

	animationGlobs.ddraw = directDraw;
	legacy::AVIFileInit();
}


// <missing>
void __cdecl Gods98::Animation_ShutDown(void)
{
	log_firstcall();

	::AVIFileExit(); // no legacy for this
}


// <LegoRR.exe @0047ef50>
Gods98::Animation_t* __cdecl Gods98::Animation_Load(const char* fName)
{
	log_firstcall();

	G98CAnimation* animation = new G98CAnimation(fName);

	return (Animation_t*)animation;
}

// <LegoRR.exe @0047efb0>
bool32 __cdecl Gods98::Animation_Update(Animation_t* anim)
{
	log_firstcall();

	G98CAnimation* animation = (G98CAnimation*)anim;

	return animation->Update();
}

// <LegoRR.exe @0047efc0>
void __cdecl Gods98::Animation_BlitToBackBuffer(Animation_t* anim, const Rect2I* destRect)
{
	log_firstcall();

	G98CAnimation* animation = (G98CAnimation*)anim;

	if (animation->IsOk()) {
		DirectDraw_bSurf()->Blt(const_cast<RECT*>((const RECT*)destRect), animation->GetSurface(), nullptr, DDBLT_WAIT, nullptr);
	}

	//if (((G98CAnimation*)anim)->isOpen) {
	//	draw::Draw_GetDrawingSurface()->Blt(rect, ((G98CAnimation*)anim)->renderer->ddSurface4,
	//		nullptr, DDBLT_WAIT /*0x1000000*/, nullptr);
	///}
}

// Sets the animation time in frame units.
// <LegoRR.exe @0047f000>
void __cdecl Gods98::Animation_SetTime(Animation_t* anim, uint32 time)
{
	log_firstcall();

	G98CAnimation* animation = (G98CAnimation*)anim;

	animation->SetTime(time);
}

// Gets the animation time in frame units.
// <LegoRR.exe @0047f010>
uint32 __cdecl Gods98::Animation_GetTime(const Animation_t* anim)
{
	log_firstcall();

	G98CAnimation* animation = (G98CAnimation*)anim;

	return animation->GetTime();
}

// Gets the animation length in frame units.
// <LegoRR.exe @0047f020>
uint32 __cdecl Gods98::Animation_GetLength(const Animation_t* anim)
{
	log_firstcall();

	G98CAnimation* animation = (G98CAnimation*)anim;

	return animation->Length();
}

// <LegoRR.exe @0047f030>
void __cdecl Gods98::Animation_Free(Animation_t* anim)
{
	log_firstcall();

	G98CAnimation* animation = (G98CAnimation*)anim;

	/// DIFFERENCE: Gods98 source does not null-check this, maybe this is a C++ deletor behavior(?)
	if (animation) delete animation;
}

// <LegoRR.exe @0047f040>
bool32 __cdecl Gods98::Animation_IsOk(const Animation_t* anim)
{
	log_firstcall();

	G98CAnimation* animation = (G98CAnimation*)anim;

	return animation->IsOk();
}

#pragma endregion
