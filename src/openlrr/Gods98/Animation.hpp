// Animation.hpp : Header file for the C++ G98CAnimation implementation.
//

#pragma once

#include <windows.h>
#include <ddraw.h>
#include <vfw.h>

#include "../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Classes
 **********************************************************************************/

#pragma region Classes

/// PRIVATE:
class G98CSurface // no CObject inheritance
{
public:
	// /*00,4*/ vftable;
	/*04,4*/ IDirectDrawClipper* m_clipper; // (unused)
	/*08,1*/ bool m_surfaceLocked;
	/*09,3*/ uint8 reserved1[3];
	/*0c,4*/ sint32 m_bpp;
	/*10,4*/ IDirectDrawPalette* m_palette;
	/*14,1*/ bool m_colourKeyed; // (unused)
	/*15,1*/ bool m_squashToEdge; // (unused)
	/*16,1*/ bool m_15bit;
	/*17,1*/ bool m_surfaceInited;
	/*18,4*/ IDirectDrawSurface4* m_surf;
	/*1c,7c*/ DDSURFACEDESC2 m_desc;
	/*98*/

	
	// <LegoRR.exe @0047e720>
	// G98CSurface* deletor(unsigned char flag);
	// <LegoRR.exe @0047e740>
	virtual ~G98CSurface();
	// <LegoRR.exe @0047e780>
	G98CSurface(sint32 width, sint32 height, sint32 bpp, bool vram = false, bool fullScreen = false /*unused*/);

private:
	// <LegoRR.exe @0047e7d0>
	bool Init(sint32 width, sint32 height, sint32 bpp, bool vram);
	// <LegoRR.exe @0047e9d0>
	void PreInit(bool fullScreen = false /*unused*/);
public:
	// Virtual functions, possibly extending from some unseen base class, never used.
	// <LegoRR.exe @0047ea00>
	virtual uint32 Width() const;
	// <LegoRR.exe @0047ea10>
	virtual uint32 Height() const;
//private:
	// <LegoRR.exe @0047ea20>
	bool Lock();
	// <LegoRR.exe @0047ea70>
	bool Unlock();
	// <LegoRR.exe @0047eaa0>
	sint32 CountMaskBits(uint32 mask) const;

public:
	// <inlined>
	void* Data() const { return this->m_desc.lpSurface; }			// Only sutiable when lock taken thus access through G98CSurfaceLock

	// <missing>
	//uint32 CountMaskBitShift(uint32 mask) const;

	// <inlined>
	IDirectDrawSurface4* GetSurface() { return this->m_surf; }


	// <inlined>
	bool IsLocked() const { return this->m_surfaceLocked; }

	// <inlined>
	sint32 Pitch() const { return this->m_desc.lPitch; }

	// <inlined>
	sint32 Bpp() const {
		if (this->m_15bit) return 15;
		else return (sint32)this->m_desc.ddpfPixelFormat.dwRGBBitCount;
	}
};
static_assert(sizeof(G98CSurface) == 0x98, "");


/// PRIVATE:
class G98CSurfaceLock {
//private:
public:

	// (no vftable)
	/*0,4*/ bool m_lockHeld;
	/*4,4*/ G98CSurface* m_surf;
	/*8*/

public:

	// <inlined>
	G98CSurfaceLock(G98CSurface* surf)
	{
		this->m_surf = surf;
		this->m_lockHeld = false;
		if (!this->m_surf->IsLocked())
			if (this->m_surf->Lock()) this->m_lockHeld = true;
	}

	// <inlined>
	~G98CSurfaceLock()
	{
		if (this->m_lockHeld) this->m_surf->Unlock();
	}
	
	// <inlined>
	void* Data() const
	{ 
		return this->m_surf->Data();
	}

	// <inlined>
	bool IsLocked() const { return this->m_lockHeld; }
};
static_assert(sizeof(G98CSurfaceLock) == 0x8, "");


/// PRIVATE:
class G98CAnimation //: public CObject  // there's no need to include this inheritance in decompilation
{
public:
	// /*00,4*/ vftable;
	/*04,4*/ IAVIStream* m_aviStream;
	/*08,4*/ IGetFrame* m_decompressFn;
	/*0c,8c*/ AVISTREAMINFOA m_aviStreamInfo;
	/*98,1*/ bool m_colourKeySet; // (unused)
	/*99,1*/ bool m_init;
	/*9a,2*/ uint16 reserved1;
	/*9c,10*/ RECT m_movieRect;
	/*ac,4*/ real32 m_startTime;
	/*b0,4*/ real32 m_currTime;
	/*b4,4*/ real32 m_aviTimeScale;
	/*b8,4*/ uint32 m_currFrame;
	/*bc,4*/ uint32 m_length;
	/*c0,4*/ G98CSurface* m_movieSurf;
	/*c4*/


	// <LegoRR.exe @0047eac0>
	G98CAnimation(const char* fName);
	// <LegoRR.exe @0047ec40>
	// G98CAnimation* deletor(unsigned char flag);
	// <LegoRR.exe @0047ec60>
	virtual ~G98CAnimation();

private:
	// <LegoRR.exe @0047eca0>
	void BMICopy(const BITMAPINFO* bmi);

public:
	// <LegoRR.exe @0047ede0>
	bool Update();
	// <LegoRR.exe @0047eea0>
	uint32 Length() const;
	// <LegoRR.exe @0047eeb0>
	void SetTime(uint32 time);
	// <LegoRR.exe @0047ef20>
	uint32 GetTime() const;

	// <missing>
	IDirectDrawSurface4* GetSurface() { return this->m_movieSurf->GetSurface(); }

	// <missing>
	const RECT* GetRect() const { return &this->m_movieRect; }

	// <missing>
	bool IsOk() const { return this->m_init; }

	// <missing>
	//void SetColourKey();
};
static_assert(sizeof(G98CAnimation) == 0xc4, "");

#pragma endregion

} /* namespace Gods98 */
