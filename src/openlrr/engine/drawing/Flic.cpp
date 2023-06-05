// Flic.cpp : 
//

#include "../../platform/windows.h"
#include "../../platform/ddraw.h"

#include "../core/Errors.h"
#include "../core/Files.h"
#include "../core/Memory.h"
#include "../Main.h"
#include "DirectDraw.h"
#include "Draw.h"
#include "Images.h"

#include "Flic.h"


/**********************************************************************************
 ******** Macro Functions
 **********************************************************************************/

#pragma region Macro Functions

// Internal macro functions (these can be moved to Flic.cpp)

// (commented out in Gods98 source)
//#define FHCOL(n)  (*(fsp->fsPalette64k)+n)
#define FHCOL(n)  getFlicCol(n,fsp)

#define DoScreenConversion(fs, val) ((fs->is15bit) ? ((((val) & 0xFFE0) >> 1) | ((val) & 0x1F)) : (val))

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00483f40>
bool32 __cdecl Gods98::Flic_Setup(const char* filename, OUT Flic** pFsp, FlicUserFlags flags)
{
	log_firstcall();

	// I'm not sure on the reasoning for storing this as a pointer,
	//  the only thing I can think of would be stack "safety", but still...
	//IDirectDraw4* dDraw = DirectDraw();
	//IDirectDraw4** lpDD = &dDraw;

	// Alocate flic structure memory
	Flic* fsp = *pFsp = (Flic*)Mem_Alloc(sizeof(Flic));
	std::memset(fsp, 0, sizeof(Flic));

	fsp->fsDisplayMode = FlicMode::FLICMODE_HICOLOR;

	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	::_splitpath(filename, drive, dir, fname, ext);
	if (!Flic_LoadHeader(filename, pFsp))
		return false;

	sint32 xsize = fsp->fsHeader.width;
	sint32 ysize = fsp->fsHeader.height;

	// create a DirectDrawSurface for this bitmap
	DDSURFACEDESC2 ddsd;
	std::memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

	ddsd.dwWidth = xsize;
	ddsd.dwHeight = ysize;
	fsp->fsXsize = xsize;
	fsp->fsYsize = ysize;

	if (DirectDraw()->CreateSurface(&ddsd, &fsp->fsSurface, nullptr) != DD_OK) {
		Error_Fatal(true, "Failed to create surface for flic");
	}

	uint32 low = 0, high = 0;
	DDCOLORKEY colourKey = { low, high };
	fsp->fsSurface->SetColorKey(DDCKEY_SRCBLT, &colourKey);

	fsp->userflags = flags;
	std::strcpy(fsp->filename, filename);

	fsp->fsXc = 0;
	fsp->fsYc = 0;
	fsp->currentframe = 0;
	fsp->overallframe = 0;
	fsp->framerate = (static_cast<sint32>(STANDARD_FRAMERATE) << 16); // Fixed number with HIWORD being whole and LOWORD being fraction?
	if ((fsp->userflags & FlicUserFlags::FLICRESIDE) == FlicUserFlags::FLICMEMORY) {
		fsp->pointerposition = 0;

		fsp->rambufferhandle = (sint8*)Mem_Alloc(fsp->fsHeader.size);
		if (!fsp->rambufferhandle) {
			Error_Warn(true, "No flic buffer handle");
		}

		if ((fsp->filehandle = File_Open(filename, "rb")) == nullptr) {
			Error_Fatal(true, "Could not open the flic");
		}
		File_Seek(fsp->filehandle, 128, SeekOrigin::Set);
		File_Read(fsp->rambufferhandle, fsp->fsHeader.size, 1, fsp->filehandle);
		File_Close(fsp->filehandle);
	}
	else {
		fsp->pointerposition = 128;

		if ((fsp->filehandle = File_Open(filename, "rb")) == nullptr) {
			char buff[128];
			std::sprintf(buff, "Not Enough Memory > %s", filename);
			Error_Warn(true, buff);
			return false;
		}
		File_Seek(fsp->filehandle, 128, SeekOrigin::Set);
	}

	fsp->fsLoadBuffer = Mem_Alloc(20000);
	return true;
}

// <LegoRR.exe @004841c0>
bool32 __cdecl Gods98::Flic_Close(Flic* fsp)
{
	log_firstcall();

	if (!fsp)
		return false;

	if ((fsp->userflags & FlicUserFlags::FLICRESIDE) == FlicUserFlags::FLICMEMORY) {
		Mem_Free(fsp->rambufferhandle);
	}
	else {
		File_Close(fsp->filehandle);
	}
	if (fsp->fsLoadBuffer) {
		Mem_Free(fsp->fsLoadBuffer);
		fsp->fsLoadBuffer = nullptr;
	}
	fsp = nullptr;
	return true;
}


/// CUSTOM:
uint32 Gods98::Flic_GetFrameCount(const Flic* fsp)
{
	return fsp->fsHeader.frames;
}

/// CUSTOM:
uint32 Gods98::Flic_GetCurrentFrame(const Flic* fsp)
{
	// Frames are 1-indexed. Zero is reserved for 'not started',
	//  and will always force a frame advance on the first call to Flic_Animate.
	if (!Flic_IsStarted(fsp))
		return 0;
	else if (fsp->currentframe == 0)
		return Flic_GetFrameCount(fsp);
	else
		return std::clamp(static_cast<uint32>(fsp->currentframe), 1u, Flic_GetFrameCount(fsp) + 1u) - 1u;
}

/// CUSTOM:
bool Gods98::Flic_IsStarted(const Flic* fsp)
{
	if ((fsp->userflags & FlicUserFlags::FLICRESIDE) == FlicUserFlags::FLICMEMORY) {
		// For in-memory flics, currentframe == 0 always means we have yet to render our first frame.
		return (fsp->currentframe > 0);
	}
	else {
		// Streamed flics will reset currentFrame to 0 when looping, so we need to detect that here.
		// ringframe is assigned to pointerposition on the first frame (which is non-zero),
		//  so with that we can know we've rendered at least once.
		return (fsp->currentframe > 0 || fsp->ringframe > 0);
	}
}


// <unused>
bool32 __cdecl Gods98::Flic_SetFrameRate(Flic* fsp, sint32 rate)
{
	if (!fsp)
		return false;

	if ((fsp->userflags & FlicUserFlags::FLICSOUND) == FlicUserFlags::FLICSOUNDON)
		return true;

	if (rate < 0) rate = 0;
	fsp->framerate = rate;

	return true;
}

// <unused>
sint32 __cdecl Gods98::Flic_GetFrameRate(const Flic* fsp)
{
	return fsp->framerate;
}


// <LegoRR.exe @00484220>
bool32 __cdecl Gods98::Flic_LoadHeader(const char* filename, Flic** pFsp)
{
	log_firstcall();

	char buff[128];

	Flic* fsp = *pFsp;

	File* headerhandle;
	if ((headerhandle = File_Open(filename, "rb")) == nullptr) {
		std::sprintf(buff,"Flic File Not Found %s",filename);
		Error_Warn(true, buff);
		return false;
	}

	File_Seek(headerhandle, 0, SeekOrigin::Set);
	File_Read(&fsp->fsHeader, sizeof(FLICHEADERSTRUCT), 1, headerhandle);
	if (fsp->fsHeader.size != File_Length(headerhandle)) {
		File_Close(headerhandle);
		/// FIX APPLY: Missing "%s" format specifier for filename
		std::sprintf(buff, "Flic File Invalid %s",filename);
		Error_Warn(true, buff);
		return false;
	}

	switch (fsp->fsHeader.magic) {
	case 0xaf11: break;
	case 0xaf12: break;
	case 0x9119: break;
	case 0xaf43: break;
	case 0x1234: break;
	default:
		{
			File_Close(headerhandle);
			std::sprintf(buff,"Flic File Invalid %s",filename);
			Error_Warn(true, buff);
			return false;
		}
		break;
	}
	File_Close(headerhandle);

	return true;
}


// (debug)
// <unused>
void __cdecl Gods98::flicTest(Flic* fsp)
{
	static sint32 n = 0;
	
	uint16* sp = (uint16*)fsp->fsSPtr;
	
	n++;
	
	for(sint32 y = 0; y < 128; y++) {
		for(sint32 x = 0; x < 128; x++) {
			sp[x + (y * fsp->fsXsize)] = (uint16)n;
		}
	}
}


// <LegoRR.exe @00484330>
bool32 __cdecl Gods98::Flic_Animate(Flic* fsp, const Area2F* destArea, bool32 advance, bool32 trans)
{
	return Flic_Animate2(fsp, destArea, (advance ? 1 : 0), trans, true);
}

/// REPLACEMENT FOR: Flic_Animate
// Animates the flic using the frame rate from Main_GetDeltaTime()
// <LegoRR.exe @00484330>
bool32 __cdecl Gods98::Flic_AnimateMainDeltaTime(Flic* fsp, const Area2F* destArea, bool32 advance, bool32 trans)
{
	return Flic_AnimateDeltaTime(fsp, destArea, advance, trans, Main_GetDeltaTime());
}

/// CUSTOM:
bool Gods98::Flic_AnimateDeltaTime(Flic* fsp, const Area2F* destArea, bool advance, bool trans, real32 elapsed)
{
	// Don't animate if advance is false, or if we haven't started.
	uint32 advanceCount = 0;
	if (advance && Flic_IsStarted(fsp)) {
		fsp->frameTimer += elapsed;
		while (fsp->frameTimer >= 1.0f) {
			fsp->frameTimer -= 1.0f;
			advanceCount++;
		}
	}
	return Flic_Animate2(fsp, destArea, advanceCount, trans, true);
}

/// CUSTOM:
bool Gods98::Flic_Animate2(Flic* fsp, OPTIONAL const Area2F* destArea, uint32 advanceCount, OPTIONAL bool trans, bool render)
{
	log_firstcall();

	HRESULT hr;
	
	// I'm not sure on the reasoning for storing this as a pointer,
	//  the only thing I can think of would be stack "safety", but still...
	//IDirectDrawSurface4* BackBuffer = DirectDraw_bSurf();
	//IDirectDrawSurface4* *lpBB = &BackBuffer;

	FlicError flicRetVal = FlicError::FLICNOERROR;

	/// NOTE: Don't try to get around rendering to the flic surface.
	///       There's some delta filtering based on previous frames, so we can't just skip around. :(

	DDSURFACEDESC2 dds;
	//::ZeroMemory(&dds, sizeof(dds));
	std::memset(&dds, 0, sizeof(dds));
   	dds.dwSize = sizeof(dds);
   	hr = fsp->fsSurface->Lock(nullptr, &dds, DDLOCK_WAIT, nullptr);
   	fsp->fsSPtr = dds.lpSurface;
	fsp->fsPitch = dds.lPitch;
	fsp->is15bit = (dds.ddpfPixelFormat.dwGBitMask == 0x3E0);
	fsp->bitDepth = dds.ddpfPixelFormat.dwRGBBitCount;

	fsp->rBits  = DirectDraw_CountMaskBits(dds.ddpfPixelFormat.dwRBitMask);
	fsp->gBits  = DirectDraw_CountMaskBits(dds.ddpfPixelFormat.dwGBitMask);
	fsp->bBits  = DirectDraw_CountMaskBits(dds.ddpfPixelFormat.dwBBitMask);
	fsp->aBits  = DirectDraw_CountMaskBits(dds.ddpfPixelFormat.dwRGBAlphaBitMask);

	fsp->rShift = DirectDraw_CountMaskBitShift(dds.ddpfPixelFormat.dwRBitMask);
	fsp->gShift = DirectDraw_CountMaskBitShift(dds.ddpfPixelFormat.dwGBitMask);
	fsp->bShift = DirectDraw_CountMaskBitShift(dds.ddpfPixelFormat.dwBBitMask);
	fsp->aShift = DirectDraw_CountMaskBitShift(dds.ddpfPixelFormat.dwRGBAlphaBitMask);

	fsp->aValue = DirectDraw_ShiftChannelByte(0xff, fsp->aBits, fsp->aShift);

	// We need to advance one more frame to our 'first' frame.
	//if (fsp->currentframe == 0) advance = true;
	if (fsp->currentframe == 0 && advanceCount == 0)
		advanceCount = 1;
	//if (fsp->currentframe == 0)
	//	advanceCount++;

	// Don't waste time looping over and over again.
	advanceCount %= Flic_GetFrameCount(fsp);

	//if (advance) {
	for (uint32 i = 0; i < advanceCount && flicRetVal != FlicError::FLICFINISHED; i++) {

		if ((fsp->userflags & FlicUserFlags::FLICRESIDE) == FlicUserFlags::FLICMEMORY)
			flicRetVal = Flic_Memory(fsp);
		else
			flicRetVal = Flic_Load(fsp);

		//if (fsp->currentframe == 0)
		//	advanceCount++; // Rendering the first frame is special(?)
	}
		
	hr = fsp->fsSurface->Unlock(nullptr);

	if (render) {
		/// FIXME FUTURE: This will be partly responsible for clipping an
		///                animated pointer cursor to the screen bounds.
		/// FIXME: Cast from float to unsigned
		RECT destRect = { // sint32 casts to stop compiler from complaining
			static_cast<sint32>(static_cast<uint32>(destArea->x)),
			static_cast<sint32>(static_cast<uint32>(destArea->y)),
			static_cast<sint32>(static_cast<uint32>(destArea->x) + static_cast<uint32>(destArea->width)),
			static_cast<sint32>(static_cast<uint32>(destArea->y) + static_cast<uint32>(destArea->height)),
		};
		destRect.left   *= Main_RenderScale();
		destRect.top    *= Main_RenderScale();
		destRect.right  *= Main_RenderScale();
		destRect.bottom *= Main_RenderScale();

		RECT* dest = &destRect;

		DDBLTFX ddBltFx = { 0 };
		ddBltFx.dwSize = sizeof(DDBLTFX);
		ddBltFx.dwFillColor = 0xff00;

		Draw_AssertUnlocked("Flic_Animate2");
		//hr = (*lpBB)->Blt(dest, fsp->fsSurface, nullptr, /*DDBLT_COLORFILL |*/ DDBLT_WAIT|(trans?DDBLT_KEYSRC:0), &ddBltFx);
		hr = DirectDraw_bSurf()->Blt(dest, fsp->fsSurface, nullptr, /*DDBLT_COLORFILL |*/ DDBLT_WAIT|(trans?DDBLT_KEYSRC:0), &ddBltFx);
	}

	return (flicRetVal == FlicError::FLICNOERROR);// ? true : false;
}

// <LegoRR.exe @00484490>
Gods98::FlicError __cdecl Gods98::Flic_Memory(Flic* fsp)
{
	log_firstcall();

	FlicError flicRetVal = FlicError::FLICNOERROR;

	/// CHANGE: Only read if the flic hasn't ended (for non-looping).
	if ((fsp->userflags & FlicUserFlags::FLICLOOPING) == FlicUserFlags::FLICLOOPINGON ||
		(fsp->currentframe < (fsp->fsHeader.frames + 1)))
	{
		Flic_FindChunk(fsp);
	}
	
	if (fsp->currentframe == 0) {
		fsp->ringframe = fsp->pointerposition;
	}

	fsp->currentframe++;
	fsp->overallframe++;

	if ((fsp->userflags & FlicUserFlags::FLICLOOPING) == FlicUserFlags::FLICLOOPINGON) {
		if (fsp->currentframe >= (fsp->fsHeader.frames + 1)) {
			fsp->pointerposition = fsp->ringframe;
			fsp->currentframe = 1;
		}
	}
	else {
		if (fsp->currentframe >= (fsp->fsHeader.frames + 1)) {
			/// CHANGE: Cap current frame at end.
			fsp->currentframe = (fsp->fsHeader.frames + 1);
			fsp->overallframe--; // We didn't change frames.

			flicRetVal = FlicError::FLICFINISHED;
		}
	}
	
	return flicRetVal;
}

// <LegoRR.exe @00484520>
Gods98::FlicError __cdecl Gods98::Flic_Load(Flic* fsp)
{
	log_firstcall();

	FlicError flicRetVal = FlicError::FLICNOERROR;

	sint16* source = (sint16*)fsp->fsLoadBuffer;
	
	if (fsp->currentframe == 0) {
		fsp->ringframe = fsp->pointerposition;
	}
	
	/// CHANGE: Only read if the flic hasn't ended (for non-looping).
	if ((fsp->userflags & FlicUserFlags::FLICLOOPING) == FlicUserFlags::FLICLOOPINGON ||
		(fsp->currentframe < (fsp->fsHeader.frames + 1)))
	{
		File_Read(source, 16, 1, fsp->filehandle);
		Flic_FindChunk(fsp);
	}
	
	fsp->currentframe++;
	fsp->overallframe++;
	
	if ((fsp->userflags & FlicUserFlags::FLICLOOPING) == FlicUserFlags::FLICLOOPINGON) {
		if (fsp->currentframe >= (fsp->fsHeader.frames + 1)) {
			File_Seek(fsp->filehandle, fsp->ringframe, SeekOrigin::Set);
			fsp->pointerposition = fsp->ringframe;
			// Currently I don't think this has any meaningful effect other than
			//  forcing streamed flics to advance on the first frame after looping.
			// Maybe change to fsp->currentframe = 1;??
			fsp->currentframe = 0;
		}
	}
	else {
		if (fsp->currentframe >= (fsp->fsHeader.frames + 1)) {
			/// CHANGE: Cap current frame at end.
			fsp->currentframe = (fsp->fsHeader.frames + 1);
			fsp->overallframe--; // We didn't change frames.

			flicRetVal = FlicError::FLICFINISHED;
		}
	}
	
	return flicRetVal;
}

// <LegoRR.exe @004845e0>
Gods98::FlicError __cdecl Gods98::Flic_FindChunk(Flic* fsp)
{
	log_firstcall();

	sint32 storepointer;
	sint32 originalpointer = fsp->pointerposition;

    if ((fsp->userflags & FlicUserFlags::FLICRESIDE) == FlicUserFlags::FLICMEMORY) {
		uint8* source = (uint8*)fsp->rambufferhandle + fsp->pointerposition;
        storepointer = *(sint32*)source;

		bool32 flag = false;
        while (!flag && (fsp->pointerposition < fsp->fsHeader.size)) {
			uint16 chunktype = *((uint16*)source + 2);
            switch (chunktype) {
			case 0xf1fa:
				Flic_FrameChunk(fsp);
				flag = true; // end loop
				break;
					
			case 0x4b4c:
				Flic_LoadPointers(fsp);
				fsp->pointerposition += *(sint32*)source;
				source += *(sint32*)source;
				storepointer += *(sint32*)source;
				break;

			case 0x000b:
				Flic_LoadPalette64(fsp);
				fsp->pointerposition += *(sint32*)source;
				source += *(sint32*)source;
				storepointer += *(sint32*)source;
				break;
					
			default:
				fsp->pointerposition += *(sint32*)source;
				source += *(sint32*)source;
				storepointer += *(sint32*)source;
				break;
			}
        }
    }
    else {
		uint8* source = (uint8*)fsp->fsLoadBuffer;
        storepointer = *(sint32*)source;

		bool32 flag = false;
        while (!flag && (fsp->pointerposition < fsp->fsHeader.size)) {
			uint16 chunktype = *((uint16*)source + 2);
            switch (chunktype) {
            case 0xf1fa:
				Flic_FrameChunk(fsp);
				flag = true; // end loop
				break;

			case 0x4b4c:
				Flic_LoadPointers(fsp);
				fsp->pointerposition += *(sint32*)source;
                File_Seek(fsp->filehandle, fsp->pointerposition, SeekOrigin::Set);
				source = (uint8*)fsp->fsLoadBuffer;
                File_Read(source, 16, 1, fsp->filehandle);
				storepointer += *(sint32*)source;
				break;

			case 0x000b:
				Flic_LoadPalette64(fsp);
				fsp->pointerposition += *(sint32*)source;
                File_Seek(fsp->filehandle, fsp->pointerposition, SeekOrigin::Set);
				source = (uint8*)fsp->fsLoadBuffer;
                File_Read(source, 16, 1, fsp->filehandle);
                storepointer += *(sint32*)source;
				break;

            default:
				fsp->pointerposition += *(sint32*)source;      //this bit must load in the next
                File_Seek(fsp->filehandle, fsp->pointerposition, SeekOrigin::Set);
				source = (uint8*)fsp->fsLoadBuffer;
                File_Read(source, 16, 1, fsp->filehandle);
                storepointer += *(sint32*)source;
                break;
            }
        }
    }

    fsp->pointerposition = storepointer + originalpointer;
    return FlicError::FLICNOERROR;
}

// <LegoRR.exe @00484770>
bool32 __cdecl Gods98::Flic_FrameChunk(Flic* fsp)
{
	log_firstcall();

	sint8 chunkcount;
    sint8* source;
	char buff[128];

    if ((fsp->userflags & FlicUserFlags::FLICRESIDE) == FlicUserFlags::FLICMEMORY) {
        source = fsp->rambufferhandle;
        source += (fsp->pointerposition);
        chunkcount = *((sint8*)source + 6);
        source += 16;
        fsp->pointerposition += 16;
        while ((chunkcount > 0) && (fsp->pointerposition < fsp->fsHeader.size)) {
			fsp->fsSource = source;
            Flic_DoChunk(fsp);
            fsp->pointerposition += *(sint32*)source;
            source = (sint8*)source + *(sint32*)source;
            chunkcount--;
        }
    }
    else {
		source = (sint8*)fsp->fsLoadBuffer;
        chunkcount = *((sint8*)source + 6);
		sint32 flicmaxchunk = ((*(sint32*)source) - 16);
        if (flicmaxchunk > fsp->fsLoadBufferSize) {

            if (fsp->fsLoadBuffer)
				Mem_Free(fsp->fsLoadBuffer);

            fsp->fsLoadBufferSize = flicmaxchunk;
            fsp->fsLoadBuffer = Mem_Alloc(fsp->fsLoadBufferSize);
            if (!fsp->fsLoadBuffer) {
                std::sprintf(buff,"Flic Not Enough Memory");
				Error_Warn(true, buff);
				return false;
            }
            source = (sint8*)fsp->fsLoadBuffer;
        }

        File_Read(source, flicmaxchunk, 1, fsp->filehandle);
        fsp->pointerposition += 16;
        while ((chunkcount > 0) && (fsp->pointerposition < fsp->fsHeader.size)) {
			fsp->fsSource = source;
            Flic_DoChunk(fsp);
            fsp->pointerposition += *(sint32*)source;
            source = (sint8*)source + *(sint32*)source;
            chunkcount--;
        }
    }
    return true ;
}

// <LegoRR.exe @004848d0>
Gods98::FlicError __cdecl Gods98::Flic_DoChunk(Flic* fsp)
{
	log_firstcall();

    sint16 chunktype = *((sint16*)fsp->fsSource + 2);
    switch (chunktype) {
    case 0x0004:
        Flic_Palette256(fsp);
		break;

    case 0x0007:
		Flic_DeltaWord(fsp);
		break;

    case 0x000b:
        Flic_Palette64(fsp); // Does nothing.
		break;

    case 0x000c: 
		Flic_DeltaByte(fsp); // Does nothing.
		break;

	case 0x000d:
		Flic_Black(fsp); // Does nothing.
		break;

	case 0x000f:
		Flic_BrunDepack(fsp);
		break;

	case 0x0010:
		Flic_Copy(fsp); // Does nothing in all subfunctions.
		break;

	case 0x0019:
		Flic_BrunDepack(fsp);
		break;

	case 0x001b:
		Flic_DeltaWord(fsp);
		break;

	case 0x5344: // no sound support(?)
//		Flic_SoundChunk(fsp);
		break;

	case 0x5555:
		Flic_Unpack(fsp); // Does nothing.
		break;

	default:
		return FlicError::FLICNOERROR;
	}
	return FlicError::FLICFINISHED;
}


// (unsused and commented out in Flic.prot)
// <unused>
sint32 __cdecl Gods98::Flic_FindMaxChunk(Flic* fsp)
{
	sint32 max = 0;
    sint8 buffer[16];

    sint32 storepointer = fsp->pointerposition;
    sint32 storefpointer = File_Tell(fsp->filehandle);
    void* source = &buffer;

    fsp->pointerposition = 128;

    while (fsp->pointerposition < fsp->fsHeader.size) {
        File_Seek(fsp->filehandle, fsp->pointerposition, SeekOrigin::Set);
        File_Read(&buffer, 16, 1, fsp->filehandle);
		sint32 chunklength = *(sint32*)source;
        if (chunklength > max) max = chunklength;
        fsp->pointerposition += chunklength;
    }

    File_Seek(fsp->filehandle, storefpointer, SeekOrigin::Set);
    fsp->pointerposition = storepointer;
    return max;
}


// Function to load pointers for each frame of flic
// <LegoRR.exe @004849e0>
Gods98::FlicError __cdecl Gods98::Flic_LoadPointers(Flic* fsp)
{
	log_firstcall();

	sint32 flicmaxchunk;
    sint8 chunkcount;
    void *source;
	char buff[256];

    if ((fsp->userflags & FlicUserFlags::FLICRESIDE) == FlicUserFlags::FLICMEMORY) {
        source = fsp->rambufferhandle;
        source = (sint8*)source + fsp->pointerposition;
    }
    else {
        source = (fsp->fsLoadBuffer);
        flicmaxchunk = *(sint32*)source;
        if (flicmaxchunk>fsp->fsLoadBufferSize) {

            if (fsp->fsLoadBuffer)
				Mem_Free(fsp->fsLoadBuffer);

            fsp->fsLoadBufferSize = flicmaxchunk;
            fsp->fsLoadBuffer = Mem_Alloc(fsp->fsLoadBufferSize);
            if (!fsp->fsLoadBuffer) {

                std::sprintf(buff, "Flic Not Enough Memory");
				Error_Warn(true, buff);
				return FlicError::FLICNOERROR; // returned as (FlicError)false;
            }
            source = fsp->fsLoadBuffer;
        }

        File_Seek(fsp->filehandle, fsp->pointerposition, SeekOrigin::Set);
        File_Read(source,flicmaxchunk, 1, fsp->filehandle);
    }


	/// FIXME GODS98: This is not initialized for `FLICRESIDE == FLICMEMORY`!!!!
	chunkcount = (flicmaxchunk / 8);
	source = (sint8*)source + 6;


//	fsp->fsPointer = std::malloc(flicmaxchunk);

	return FlicError::FLICNOERROR ;
}

// Function to load an 8 bit palette
// <LegoRR.exe @00484a90>
Gods98::FlicError __cdecl Gods98::Flic_LoadPalette64(Flic* fsp)
{
	log_firstcall();

	sint32 flicmaxchunk;
    sint8* source;
	char buff[128];		  		  	

    if ((fsp->userflags & FlicUserFlags::FLICRESIDE) == FlicUserFlags::FLICMEMORY) {
        source = fsp->rambufferhandle;
        source = (sint8*)source + fsp->pointerposition;
    }
    else {
        source = (sint8*)fsp->fsLoadBuffer;
        flicmaxchunk = *(sint32*)source;
        if (flicmaxchunk > fsp->fsLoadBufferSize) {

            if (fsp->fsLoadBuffer)
				Mem_Free(fsp->fsLoadBuffer);

            fsp->fsLoadBufferSize = flicmaxchunk;
            fsp->fsLoadBuffer = Mem_Alloc(fsp->fsLoadBufferSize);
            if (!fsp->fsLoadBuffer) {
                std::sprintf(buff, "Flic Not Enough Memory");
                Error_Warn(true, buff);
				return FlicError::FLICFINISHED; // returned as (FlicError)true;
            }
            source = (sint8*)fsp->fsLoadBuffer;
        }

        File_Seek(fsp->filehandle, fsp->pointerposition, SeekOrigin::Set);
        File_Read(source,flicmaxchunk, 1, fsp->filehandle);
    }

	
    return FlicError::FLICNOERROR;
}

// <LegoRR.exe @00484b40>
bool32 __cdecl Gods98::Flic_Copy(Flic* fsp)
{
	log_firstcall();

	if (fsp->fsBitPlanes == 16) {
		FlicCopyHiColorFlic(fsp); // Does nothing.
		return true;
	}

	if (fsp->fsDisplayMode == FlicMode::FLICMODE_BYTEPERPIXEL) {
		FlicCopyBytePerPixel(fsp); // Does nothing.
	}
	if (fsp->fsDisplayMode == FlicMode::FLICMODE_HICOLOR) {
		FlicCopyHiColor(fsp); // Does nothing.
	}
	
	return true;
}


// Simply returns 100
// <unused>
sint32 __cdecl Gods98::Flic_GetEventRate(void)
{
	int timer = 100;

    return timer;
}

/*
// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 Gods98::FlicBRunDepackBytePerPixel(Flic* fsp) { return true; }
*/

// <LegoRR.exe @00484b90>
bool32 __cdecl Gods98::FlicBRunDepackHiColor(Flic* fsp)
{
	log_firstcall();

//421 6259 karl

	uint8* src = (uint8*)fsp->fsSource;
	src += 6;

	uint16 height = fsp->fsYsize;
	uint16 line = 0;
	src++;
	while (line < height) {
		uint16 width = fsp->fsXsize;
		uint8* dst = (uint8*)fsp->fsSPtr + (fsp->fsPitch * line);
		//uint16* dst = (uint16*)fsp->fsSPtr + ((fsp->fsPitch / 2) * line);

		while (width > 0) {
			uint16 cnt = *src++;
			
			if (cnt < 128) {
				width -= cnt;
				uint8 wrd = *src++;
				while (cnt) {
					dst = _Flic_WritePixel8(fsp, dst, wrd);
					//dst = _Flic_WritePixel16(fsp, dst, FHCOL(wrd));
					//*dst++ = FHCOL(wrd);
					cnt--;
				}
			}
			else {
				cnt = (256 - cnt);
				width -= cnt;
				while (cnt) {
					uint8 wrd = *src++;
					dst = _Flic_WritePixel8(fsp, dst, wrd);
					//dst = _Flic_WritePixel16(fsp, dst, FHCOL(wrd));
					//*dst++ = FHCOL(wrd);
					cnt--;
				}
			}
		}
		line++;
		src++;
	}
	
	return true;
}

// <LegoRR.exe @00484c90>
bool32 __cdecl Gods98::FlicBRunDepackHiColorFlic32k(Flic* fsp)
{
	log_firstcall();

	uint8* dst = (uint8*)fsp->fsSPtr;
	//uint16* dst = (uint16*)fsp->fsSPtr;
	uint8* src = (uint8*)fsp->fsSource;
	src += 6;
	src++;

	uint16 line = 0;
	uint16 height = fsp->fsYsize;
	while (line < height) {
		uint16 width = fsp->fsXsize;
		dst = (uint8*)fsp->fsSPtr + (fsp->fsPitch * line);
		//dst = (uint16*)fsp->fsSPtr + ((fsp->fsPitch / 2) * line);
		
		while (width > 0) {
			uint16 cnt = *src++;
			if (cnt < 128) {
				width -= cnt;
				uint16 tmp = *(uint16*)src;
				uint16 wrd = ((tmp & 0xffe0) << 1) | (tmp & 0x1f);
				src += 2;
				/*uint16 wrd = *(uint16*)src;
				src += 2;
				uint16 tmp = ((wrd & 0xffe0) << 1);
				tmp |= (wrd & 0x1f);
				wrd = tmp;*/
				while (cnt) {
					dst = _Flic_WritePixel16(fsp, dst, wrd);
					//*dst++ = DoScreenConversion(fsp, wrd);
					cnt--;
				}
				
			}
			else {
				cnt = (256 - cnt);
				width -= cnt;
				while (cnt) {
					uint16 tmp = *(uint16*)src;
					uint16 wrd = ((tmp & 0xffe0) << 1) | (tmp & 0x1f);
					src += 2;
					/*uint16 tmp = *(uint16*)src;
					src += 2;
					uint16 wrd = ((tmp & 0xffe0) << 1);
					wrd |= (tmp & 0x1f);*/
					dst = _Flic_WritePixel16(fsp, dst, wrd);
					//*dst++ = DoScreenConversion(fsp, wrd);
					
					cnt--;
				}
			}

		}
		src++;
		line++;
	}
	
	return true;
}

/*
// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 Gods98::FlicBRunDepackHiColorFlic(Flic* fsp) { return true; }

// return 0;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline sint32 Gods98::UnpackM2(void* input, void* output) { return 0; }
*/

// <LegoRR.exe @00484de0>
bool32 __cdecl Gods98::Flic_BrunDepack(Flic* fsp)
{
	log_firstcall();

	if (fsp->fsHeader.depth == 8) {
		if (fsp->fsDisplayMode == FlicMode::FLICMODE_BYTEPERPIXEL) {
			FlicBRunDepackBytePerPixel(fsp); // Does nothing.
		}
		if (fsp->fsDisplayMode == FlicMode::FLICMODE_HICOLOR) {
			FlicBRunDepackHiColor(fsp);
		}

	}
	else if (fsp->fsHeader.depth == 16) {
		if (fsp->fsHeader.magic == 0xaf43) {
			FlicBRunDepackHiColorFlic32k(fsp);
		}
		else {
			FlicBRunDepackHiColorFlic(fsp); // Does nothing.
		}
	}
	return true;
}

/*
// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 Gods98::Flic_Black(Flic* fsp) { return true; }

// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 Gods98::Flic_DeltaByte(Flic* fsp) { return true; }

// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 Gods98::Flic_Palette64(Flic* fsp) { return true; }

// (shared, internal)
// DON'T HOOK THIS, it's internal only(?)
// <LegoRR.exe @00484e50>
//bool32 __cdecl Gods98::Flic_NERPFunc__True(sint32* stack); // return 1;
*/

// <LegoRR.exe @00484e60>
void __cdecl Gods98::FlicCreateHiColorTable(Flic* fsp)
{
	log_firstcall();

	//uint8* src = (uint8*)&fsp->fsPalette256;
	uint16* dst = fsp->fsPalette64k;

	for (sint32 i = 0; i < 256; i++) {
		uint16 col;
		uint16 wrd = fsp->fsPalette256[i].red; // src[0];
		/// CHANGE: Palette colour channels were originally stored as 6-bit, change that and unlock the whole 8 bits.
		wrd >>= 3;
		//col >>= 1;
		wrd <<= 11;

		col = wrd;

		wrd = fsp->fsPalette256[i].green; // src[1];
		/// CHANGE: Palette colour channels were originally stored as 6-bit, change that and unlock the whole 8 bits.
		wrd >>= 2;
		wrd <<= 5;

		col |= wrd;

		wrd = fsp->fsPalette256[i].blue; // src[2];
		/// CHANGE: Palette colour channels were originally stored as 6-bit, change that and unlock the whole 8 bits.
		wrd >>= 3;
		//wrd >>= 1;

		col |= wrd;

		*dst++ = col;

		//src += 3;
	}
	fsp->fsPalette64k[0] = 0x00;
}

// <LegoRR.exe @00484ec0>
bool32 __cdecl Gods98::Flic_Palette256(Flic* fsp)
{
	log_firstcall();

	uint8* src = (uint8*)fsp->fsSource;
	src += 6;

	uint16 cnt = *(uint16*)src;
	src += 2;

	//uint8* dst = (uint8*)&fsp->fsPalette256;
	ColourRGBPacked* dst = fsp->fsPalette256;

	while (cnt) {
		uint16 indx = *src++;
		//dst += (indx * 3);
		dst += indx;
		uint16 ccnt = *src++;

		if (!ccnt) {
			cnt = 1;
			ccnt = 256;
			//dst = (uint8*)&fsp->fsPalette256;
			dst = fsp->fsPalette256;
		}
		//ccnt *= 3;
		while (ccnt) {
			//uint8 col = *src++;
			/// CHANGE: Palette colour channels were originally stored as 6-bit, change that and unlock the whole 8 bits.
			//col >>= 2;
			dst->red   = *src++;
			dst->green = *src++;
			dst->blue  = *src++;
			dst++;
			//*dst++ = col;
			ccnt--;
		}
		cnt--;
	}

	if (fsp->fsDisplayMode == FlicMode::FLICMODE_HICOLOR) {
	    FlicCreateHiColorTable(fsp);
	}


	return true;
}

/*
// return 0;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline sint32 Gods98::Flic_Unpack(Flic* fsp) { return 0; }

// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void Gods98::FlicCopyHiColor(Flic* fsp) { return; }

// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void Gods98::FlicCopyBytePerPixel(Flic* fsp) { return; }

// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void Gods98::FlicCopyHiColorFlic(Flic* fsp) { return; }

// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void Gods98::FlicDeltaWordBytePerPixel(Flic* fsp) { return; }

// (shared, internal)
// DON'T HOOK THIS, it's internal only(?)
// <LegoRR.exe @00484f50>
//void __cdecl Gods98::Flic_logf_removed(const char* msg, ...); // return;
*/

// <LegoRR.exe @00484f60>
void __cdecl Gods98::FlicDeltaWordHiColor(Flic* fsp)
{
	log_firstcall();

	const uint32 byteDepth = DirectDraw_BitToByteDepth(fsp->bitDepth);

	uint8* dst = (uint8*)fsp->fsSPtr;
	uint16 height = fsp->fsYsize;

	uint8* src = (uint8*)fsp->fsSource;
	src += 6;

	uint16 line = 0;
	uint16 cnt = *(uint16*)src;
	src += 2;
	while (cnt) {
		dst = (uint8*)fsp->fsSPtr + (fsp->fsPitch * line);

		uint16 ctrl = *(uint16*)src;
		ctrl >>= 12;
		ctrl &= 0xc;
		if (ctrl == 0xc) {
			uint16 wrd = *(uint16*)src;
			src += 2;
			wrd = (65536 - wrd);

			line += wrd;
		}
		else if (ctrl == 0x4) {
			return;
		}
		else {
			bool16 mflag = false;
			uint8 mfval;
			if (ctrl == 0x8) {
				mfval = *src;
				src += 2;
				mflag = true;
			}
			uint16 mcnt = *(uint16*)src;
			src += 2;
			while (mcnt) {
				uint16 offs = *src++;
				dst += (offs * byteDepth);
				uint16 cpy = *src++;
				if (cpy & 0x80) {
					cpy = (256 - cpy);
					uint8 w1 = *src++;
					uint8 w2 = *src++;
					while (cpy) {
						//*(uint16*)dst = FHCOL(w1);
						//dst += 2;
						//*(uint16*)dst = FHCOL(w2);
						//dst += 2;
						//dst = _Flic_WritePixel16(fsp, dst, FHCOL(w1));
						//dst = _Flic_WritePixel16(fsp, dst, FHCOL(w2));
						dst = _Flic_WritePixel8(fsp, dst, w1);
						dst = _Flic_WritePixel8(fsp, dst, w2);
						cpy--;
					}
				}
				else {
					while (cpy) {
						uint8 w1 = *src++;
						uint8 w2 = *src++;
						//*(uint16*)dst = FHCOL(w1);
						//dst += 2;
						//*(uint16*)dst = FHCOL(w2);
						//dst += 2;
						//dst = _Flic_WritePixel16(fsp, dst, FHCOL(w1));
						//dst = _Flic_WritePixel16(fsp, dst, FHCOL(w2));
						dst = _Flic_WritePixel8(fsp, dst, w1);
						dst = _Flic_WritePixel8(fsp, dst, w2);
						cpy--;
					}
					
				}
				mcnt--;
			}
			if (mflag) {
				/// TODO: Is not dst++ intentional here??
				_Flic_WritePixel8(fsp, dst, mfval);
				//_Flic_WritePixel16(fsp, dst, FHCOL(mfval));
				//*(uint16*)dst = FHCOL(mfval);
			}

			cnt--;
			line++;
		}
	}
}

/*
// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void Gods98::FlicDeltaWordHiColorDZ(Flic* fsp) { return; }
*/

// <LegoRR.exe @00485110>
void __cdecl Gods98::FlicDeltaWordHiColorFlic32k(Flic* fsp)
{
	log_firstcall();

	const uint32 byteDepth = DirectDraw_BitToByteDepth(fsp->bitDepth);

	uint8* dst = (uint8*)fsp->fsSPtr;
	uint16 height = fsp->fsYsize;

	uint8* src = (uint8*)fsp->fsSource;
	src += 6;

	uint16 line = 0;
	uint16 cnt = *(uint16*)src;
	src += 2;
	while (cnt) {
		dst = (uint8*)fsp->fsSPtr + (fsp->fsPitch * line);

		uint16 ctrl = *(uint16*)src;
		ctrl >>= 12;
		ctrl &= 0xc;
		if (ctrl == 0xc) {
			uint16 wrd = *(uint16*)src;
			src += 2;
			wrd = (65536 - wrd);

			line += wrd;

		}
		else if (ctrl == 0x4) {
			return;
		}
		else {
			bool16 mflag = false;
			uint16 mfval;
			if (ctrl == 0x8) {
				//mfval = DoScreenConversion(fsp, *(uint16*)src);
				mfval = *(uint16*)src;
				src += 2;
				mflag = true;
			}
			uint16 mcnt = *(uint16*)src;
			src += 2;
			while (mcnt) {
				dst += ((*src++) * byteDepth);
				uint16 cpy = *src++;
				if (cpy & 0x80) {
					uint16 tmp = *(uint16*)src;
					uint16 wrd = ((tmp & 0xffe0) << 1) | (tmp & 0x1f);
					src += 2;
					/*uint16 tmp = *(uint16*)src;
					src += 2;
					uint16 wrd = ((tmp & 0xffe0) << 1);
					wrd |= (tmp & 0x1f);*/
					cpy = (256 - cpy);
					while(cpy) {
						//*(uint16*)dst = DoScreenConversion(fsp, wrd);
						//dst += 2;
						dst = _Flic_WritePixel16(fsp, dst, wrd);
						cpy--;
					}
				}
				else {
					while (cpy) {
						uint16 tmp = *(uint16*)src;
						uint16 wrd = ((tmp & 0xffe0) << 1) | (tmp & 0x1f);
						src += 2;
						/*uint16 tmp = *(uint16*)src;
						src += 2;
						uint16 wrd = ((tmp & 0xffe0) << 1);
						wrd |= (tmp & 0x1f);*/
						//*(uint16*)dst = DoScreenConversion(fsp, wrd);
						//dst += 2;
						dst = _Flic_WritePixel16(fsp, dst, wrd);
						
						cpy--;
					}
					
				}
				mcnt--;
			}
			if (mflag) {
				/// FIX APPLY: WHY ARE WE TRUNCATING TO 1 BYTE!??
				_Flic_WritePixel16(fsp, dst, /*(sint8)*/mfval);
				//*dst = /*(sint8)*/mfval;
				//*dst = (sint8)mfval;
			}

			line++;
			cnt--;
		}
	}
}

/*
// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void Gods98::FlicDeltaWordHiColorFlic(Flic* fsp) { return; }
*/

// <LegoRR.exe @004852f0>
bool32 __cdecl Gods98::Flic_DeltaWord(Flic* fsp)
{
	log_firstcall();

	if (fsp->fsHeader.depth == 8) {
		if (fsp->fsDisplayMode == FlicMode::FLICMODE_BYTEPERPIXEL) {
		    FlicDeltaWordBytePerPixel(fsp); // Does nothing.
		}
		if (fsp->fsDisplayMode == FlicMode::FLICMODE_HICOLOR) {
		    FlicDeltaWordHiColor(fsp);
		}
	}
	else if (fsp->fsHeader.depth == 16) {
		if (fsp->fsHeader.magic == 0x1234) {
			FlicDeltaWordHiColorDZ(fsp); // Does nothing.
		}
		else if (fsp->fsHeader.magic == 0xaf43) {
			FlicDeltaWordHiColorFlic32k(fsp);
		}
		else {
            FlicDeltaWordHiColorFlic(fsp); // Does nothing.
		}
	}
	return true;
}

// <LegoRR.exe @00485380>
uint16 __cdecl Gods98::getFlicCol(uint8 n, const Flic* fsp)
{
	log_firstcall();

	const uint16* ctab = fsp->fsPalette64k;

	uint16 ret = ctab[n];

	return ret;
}


/// CUSTOM: Writes a paletted pixel to the target surface (of 16-bit, 24-bit, or 32-bit).
///         Returns the pointer to the next pixel in the target surface.
uint8* Gods98::_Flic_WritePixel8(const Flic* fsp, uint8* dst, uint8 index)
{
	const uint8 r = fsp->fsPalette256[index].red;
	const uint8 g = fsp->fsPalette256[index].green;
	const uint8 b = fsp->fsPalette256[index].blue;

	return _Flic_WritePixelRGB(fsp, dst, r, g, b);
}

/// CUSTOM: Writes a 16-bit pixel to the target surface (of 16-bit, 24-bit, or 32-bit).
///         Returns the pointer to the next pixel in the target surface.
uint8* Gods98::_Flic_WritePixel16(const Flic* fsp, uint8* dst, uint16 value)
{
	// Flic never stores 16-bit colour as 15-bit.
	const uint8 r = DirectDraw_UnshiftChannelByte(value, 5, 11);
	const uint8 g = DirectDraw_UnshiftChannelByte(value, 6, 5);
	const uint8 b = DirectDraw_UnshiftChannelByte(value, 5, 0);

	return _Flic_WritePixelRGB(fsp, dst, r, g, b);
}

/// CUSTOM: Writes RGB values to the target surface (of 16-bit, 24-bit, or 32-bit).
///         Returns the pointer to the next pixel in the target surface.
uint8* Gods98::_Flic_WritePixelRGB(const Flic* fsp, uint8* dst, uint8 r, uint8 g, uint8 b)
{
	const uint32 surfaceValue =
		DirectDraw_ShiftChannelByte(r, fsp->rBits, fsp->rShift) |
		DirectDraw_ShiftChannelByte(g, fsp->gBits, fsp->gShift) |
		DirectDraw_ShiftChannelByte(b, fsp->bBits, fsp->bShift) |
		fsp->aValue;

	switch (fsp->bitDepth) {
	case 16:
		*(uint16*)dst = static_cast<uint16>(surfaceValue);
		dst += 2;
		break;
	case 24:
		*dst++ = (surfaceValue      ) & 0xff;
		*dst++ = (surfaceValue >>  8) & 0xff;
		*dst++ = (surfaceValue >> 16) & 0xff;
		break;
	case 32:
		*(uint32*)dst = surfaceValue;
		dst += 4;
		break;
	}
	return dst;
}


// (shared) "AnimClone_IsLws__Flic_GetWidth"
// THIS FUNCTION MUST BE HOOKED ON AN INDIVIDUAL BASIS
// There are 5 calls made to this:
//  type:Flic (Flic_GetWidth)  -> FUN_004120e0  <@004120f7>
//                                      Panel_Draw  <@0045ab17>
//                                      Pointer_DrawPointer  <@0045cfc8>
//  type:FlocksData (Flocks_???)     -> LegoObject_Flocks_FUN_0044bef0  <@0044bfc3>
//  type:AnimClone (AnimClone_IsLws) -> Container_FormatPartName  <@00473f60>
// <called @004120f7, 0045ab17, 0045cfc8>
// <LegoRR.exe @00489a90>
uint32 __cdecl Gods98::Flic_GetWidth(const Flic* fsp)
{
	log_firstcall();

	return static_cast<uint32>(fsp->fsXsize);
}


// <LegoRR.exe @004853a0>
uint32 __cdecl Gods98::Flic_GetHeight(const Flic* fsp)
{
	log_firstcall();

	return static_cast<uint32>(fsp->fsYsize);
}

#pragma endregion
