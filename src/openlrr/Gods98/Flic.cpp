
#include "Files.h"

#include <windows.h>
#include <ddraw.h>

#include "Maths.h"
#include "DirectDraw.h"
#include "Files.h"
#include "Flic.h"
#include "Errors.h"
#include "Memory.h"


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
bool32 __cdecl Gods98::Flic_Setup(const char* filename, OUT FLICSTRUCT** fsp, FlicUserFlags flags)
{
	log_firstcall();

	sint32 count, xsize, ysize;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char buff[128];
	DDSURFACEDESC2 ddsd;
	HRESULT hr=0;
	LPDIRECTDRAW4 dDraw = DirectDraw();
	LPDIRECTDRAW4 *lpDD = &dDraw;
	DDCOLORKEY ColourKey;
	// Alocate flic structure memory
	*fsp=(FLICSTRUCT *)Mem_Alloc(sizeof(FLICSTRUCT));
	//::ZeroMemory((*fsp),sizeof(FLICSTRUCT));
	std::memset((*fsp), 0, sizeof(FLICSTRUCT));
	(*fsp)->fsDisplayMode = FlicMode::FLICMODE_HICOLOR;
	::_splitpath(filename, drive, dir, fname, ext);
	if(!Flic_LoadHeader(filename,fsp))
		return(false);
	// create a DirectDrawSurface for this bitmap
	//::ZeroMemory(&ddsd, sizeof(ddsd));
	std::memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	xsize = (*fsp)->fsHeader.width;
	ysize = (*fsp)->fsHeader.height;
	ddsd.dwWidth = xsize;
	ddsd.dwHeight = ysize;
	(*fsp)->fsXsize = xsize;
	(*fsp)->fsYsize = ysize;
	if (hr = (*lpDD)->CreateSurface(&ddsd, &(*fsp)->fsSurface, nullptr)) {
		Error_Fatal(true, "Failed to create surface for flic");
	}
	ColourKey.dwColorSpaceLowValue = 0;
	ColourKey.dwColorSpaceHighValue = 0;
	(*fsp)->fsSurface->SetColorKey(DDCKEY_SRCBLT, &ColourKey);
	count=0;
	(*fsp)->userflags=flags;
	std::strcpy((*fsp)->filename,filename);
	(*fsp)->fsXc=0;
	(*fsp)->fsYc=0;
	(*fsp)->currentframe=0;
	(*fsp)->overallframe=0;
	(*fsp)->framerate = 25<<16;
	if((((*fsp)->userflags)&FlicUserFlags::FLICRESIDE)==FlicUserFlags::FLICMEMORY) {
		(*fsp)->pointerposition=0;
		(*fsp)->rambufferhandle=(sint8*)Mem_Alloc((*fsp)->fsHeader.size);
		if(!(*fsp)->rambufferhandle) {
			Error_Warn(true, "No flic buffer handle");
		}
		if(((*fsp)->filehandle=File_Open(filename,"rb"))==nullptr) {
			Error_Fatal(true, "Could not open the flic");
		}
		File_Seek((*fsp)->filehandle,128,SEEK_SET);
		File_Read((*fsp)->rambufferhandle,(*fsp)->fsHeader.size,1,(*fsp)->filehandle);
		File_Close((*fsp)->filehandle);
	} else {
		(*fsp)->pointerposition=128;
		if(((*fsp)->filehandle=File_Open(filename,"rb"))==nullptr) {
			std::sprintf(buff, "Not Enough Memory > %s",filename);
			Error_Warn(true,buff);
			return false;
		}
		File_Seek((*fsp)->filehandle,128,SEEK_SET);
	}
	(*fsp)->fsLoadBuffer=Mem_Alloc(20000);
	return true;
}

// <LegoRR.exe @004841c0>
bool32 __cdecl Gods98::Flic_Close(FLICSTRUCT* fsp)
{
	log_firstcall();

	if(!fsp)
		return false;
	if((fsp->userflags & FlicUserFlags::FLICRESIDE)==FlicUserFlags::FLICMEMORY) {
		Mem_Free(fsp->rambufferhandle);
	} else {
		File_Close(fsp->filehandle);
	}
	if (fsp->fsLoadBuffer) {
		Mem_Free(fsp->fsLoadBuffer);
		fsp->fsLoadBuffer=nullptr;
	}
	fsp= nullptr;
	return true;
}


// <unused>
bool32 __cdecl Gods98::Flic_SetFrameRate(FLICSTRUCT* fsp, sint32 rate)
{
	if(!fsp) {
		return false;
	}
	if ((fsp->userflags&FlicUserFlags::FLICSOUND) == FlicUserFlags::FLICSOUNDON) {
		return (true);
	}	
	if(rate<0) {
		rate=0;
	}
	fsp->framerate=rate;
	return true;
}

// <unused>
sint32 __cdecl Gods98::Flic_GetFrameRate(FLICSTRUCT* fsp)
{
	return(fsp->framerate);
}


// <LegoRR.exe @00484220>
bool32 __cdecl Gods98::Flic_LoadHeader(const char* filename, FLICSTRUCT** fsp)
{
	log_firstcall();

	File* headerhandle;
	char buff[128];

    if((headerhandle=File_Open(filename,"rb"))==nullptr)
    {
        std::sprintf(buff,"Flic File Not Found %s",filename);
		Error_Warn(true, buff);
		return false;
    }
    File_Seek(headerhandle,0,SEEK_SET);
    File_Read((sint8*)&(*fsp)->fsHeader,sizeof(FLICHEADERSTRUCT),1,headerhandle);
    if(((*fsp)->fsHeader.size)!=File_Length(headerhandle))
    {
        File_Close(headerhandle);
		/// FIX APPLY: Missing "%s" format specifier for filename
        std::sprintf(buff, "Flic File Invalid %s",filename);
		Error_Warn(true, buff);
		return false;
    }
    switch((*fsp)->fsHeader.magic)
    {
        case 0xaf11: break;
        case 0xaf12: break;
        case 0x9119: break;
        case 0xaf43: break;
        case 0x1234: break;
        default :
			{
				File_Close(headerhandle);
				std::sprintf(buff,"Flic File Invalid %s",filename);
				Error_Warn(true, buff);
				return false;
            }
    }
    File_Close(headerhandle);

    return(true);
}


// (debug)
// <unused>
void __cdecl Gods98::flicTest(FLICSTRUCT* fsp)
{
	static sint32 n=0;
	
	uint16* sp = (uint16*)fsp->fsSPtr;
	
	n++;
	
	for(sint32 i=0;i<128;i++)
	{
		for(sint32 j=0;j<128;j++)
		{
			*(sp+(i*(fsp->fsXsize))+j) = n;
		}
	}
}


// <LegoRR.exe @00484330>
bool32 __cdecl Gods98::Flic_Animate(FLICSTRUCT* fsp, const Area2F* destArea, bool32 advance, bool32 trans)
{
	log_firstcall();

	DDSURFACEDESC2 dds;
	HRESULT hr;
	DDBLTFX ddBltFx;
	IDirectDrawSurface4* BackBuffer = DirectDraw_bSurf();
	IDirectDrawSurface4* *lpBB = &BackBuffer;

	/// FIXME FUTURE: This will be partly responsible for clipping an
	//                 animated pointer cursor to the screen bounds.
	RECT destRect = { (uint32) destArea->x, (uint32) destArea->y, (uint32) destArea->x + (uint32) destArea->width, (uint32) destArea->y + (uint32) destArea->height };
	RECT *dest = &destRect;
	FlicError flicRetVal = FlicError::FLICNOERROR;

	//::ZeroMemory(&dds, sizeof(dds));
	std::memset(&dds, 0, sizeof(dds));
   	dds.dwSize=sizeof(dds);
   	hr = fsp->fsSurface->Lock(nullptr,&dds ,DDLOCK_WAIT ,nullptr);
   	fsp->fsSPtr = dds.lpSurface;
	fsp->fsPitch = dds.lPitch;
	fsp->is15bit = (dds.ddpfPixelFormat.dwGBitMask == 0x3E0);

	if (fsp->currentframe == 0) advance = true;

	if( advance )
	{	
		if(((fsp->userflags)&FlicUserFlags::FLICRESIDE)==FlicUserFlags::FLICMEMORY)
			flicRetVal = Flic_Memory(fsp);
		else
			flicRetVal = Flic_Load(fsp);
	}
		
	hr = fsp->fsSurface->Unlock(nullptr);

	//::ZeroMemory(&ddBltFx, sizeof(DDBLTFX));
	std::memset(&ddBltFx, 0, sizeof(DDBLTFX));
	ddBltFx.dwSize= sizeof (DDBLTFX);
	ddBltFx.dwFillColor=0xff00;

	hr = (*lpBB)->Blt(dest,
		fsp->fsSurface, nullptr, /*DDBLT_COLORFILL |*/ DDBLT_WAIT|(trans?DDBLT_KEYSRC:0), &ddBltFx);

	return (flicRetVal == FlicError::FLICNOERROR);// ? true : false;
}

// <LegoRR.exe @00484490>
Gods98::FlicError __cdecl Gods98::Flic_Memory(FLICSTRUCT* fsp)
{
	log_firstcall();

	FlicError flicRetVal = FlicError::FLICNOERROR;

	Flic_FindChunk(fsp);
	
	if(fsp->currentframe==0) {
		fsp->ringframe=fsp->pointerposition;
	}

	fsp->currentframe++;
	fsp->overallframe++;

	if((fsp->userflags & FlicUserFlags::FLICLOOPING) == FlicUserFlags::FLICLOOPINGON) {
		if(fsp->currentframe==(fsp->fsHeader.frames+1)) {
			fsp->pointerposition=fsp->ringframe;
			fsp->currentframe=1;
		}
	} else {
		if (fsp->currentframe == (fsp->fsHeader.frames+1)) {
			flicRetVal = FlicError::FLICFINISHED;
		}
	}
	
	return flicRetVal;
}

// <LegoRR.exe @00484520>
Gods98::FlicError __cdecl Gods98::Flic_Load(FLICSTRUCT* fsp)
{
	log_firstcall();

	FlicError flicRetVal = FlicError::FLICNOERROR;

	sint16* source=(sint16*)fsp->fsLoadBuffer;
	
	if(fsp->currentframe==0) {
		fsp->ringframe=fsp->pointerposition;
	}
	
	File_Read(source,16,1,fsp->filehandle);
	Flic_FindChunk(fsp);
	
	fsp->currentframe++;
	fsp->overallframe++;
	
	if((fsp->userflags&FlicUserFlags::FLICLOOPING) == FlicUserFlags::FLICLOOPINGON) {
		if(fsp->currentframe>(fsp->fsHeader.frames)) {
			File_Seek(fsp->filehandle,fsp->ringframe,SEEK_SET);
			fsp->pointerposition=fsp->ringframe;
			fsp->currentframe=0;
		}
	} else {
		if (fsp->currentframe>(fsp->fsHeader.frames)) {
			flicRetVal = FlicError::FLICFINISHED;
		}
	}
	
	return flicRetVal;
}

// <LegoRR.exe @004845e0>
Gods98::FlicError __cdecl Gods98::Flic_FindChunk(FLICSTRUCT* fsp)
{
	log_firstcall();

	uint16 chunktype;
    sint32 flag;
    void *source;
    sint32 storepointer,originalpointer;

    if(((fsp->userflags)&FlicUserFlags::FLICRESIDE)==FlicUserFlags::FLICMEMORY)
        source=fsp->rambufferhandle;
    else
        source=fsp->fsLoadBuffer;

    originalpointer=fsp->pointerposition;

    if(((fsp->userflags)&FlicUserFlags::FLICRESIDE)==FlicUserFlags::FLICMEMORY)
    {
        source=(sint8*)source+fsp->pointerposition;
        storepointer=*(sint32 *)(sint8 *)source;
        flag=0;
        while((flag==0)&&((fsp->pointerposition)<(fsp->fsHeader.size)))
        {
            chunktype=(*((uint16*)source+2));
            switch(chunktype)
            {
			case	(uint16)0xf1fa:
				{
					Flic_FrameChunk(fsp);
					flag=1;
					break;
				}
					
			case	0x4b4c:
				{
					Flic_LoadPointers(fsp);
					fsp->pointerposition+=*(sint32*)source;
					source=(sint8*)source+(*(sint32*)source);
					storepointer+=(*(sint32*)source);
					break;
				}
			case	0x000b:
				{
					Flic_LoadPalette64(fsp);
					fsp->pointerposition+=*(sint32*)source;
					source=(sint8*)source+(*(sint32*)source);
					storepointer+=(*(sint32*)source);
					break;
				}
					
			default:
				{
					fsp->pointerposition+=*(sint32*)source;
					source=(sint8*)source+(*(sint32*)source);
					storepointer+=(*(sint32*)source);
					break;
				}
            }
        }
    }
    else
    {
        storepointer=(*(sint32*)source);
        flag=0;
        while((flag==0)&&((fsp->pointerposition)<(fsp->fsHeader.size)))
        {
            chunktype=(*((uint16*)source+2));
            switch(chunktype)
            {
                case (uint16)0xf1fa:
					{
					Flic_FrameChunk(fsp);
                                flag=1;
                                break;
					}
						case	0x4b4c:
										Flic_LoadPointers(fsp);
										fsp->pointerposition+=*(sint32*)source;
                                File_Seek(fsp->filehandle,fsp->pointerposition,SEEK_SET);
								source=fsp->fsLoadBuffer;
                                File_Read((sint8*)source,16,1,fsp->filehandle);
                                storepointer+=(*(sint32*)source);
										break;

						case	0x000b:
										Flic_LoadPalette64(fsp);
										fsp->pointerposition+=*(sint32*)source;
                                File_Seek(fsp->filehandle,fsp->pointerposition,SEEK_SET);
								source=fsp->fsLoadBuffer;
                                File_Read((sint8*)source,16,1,fsp->filehandle);
                                storepointer+=(*(sint32*)source);
										break;

                default:     fsp->pointerposition+=*(sint32*)source;      //this bit must load in the next
                                File_Seek(fsp->filehandle,fsp->pointerposition,SEEK_SET);
								source=fsp->fsLoadBuffer;
                                File_Read((sint8*)source,16,1,fsp->filehandle);
                                storepointer+=(*(sint32*)source);
                                break;
            }
        }
    }
    fsp->pointerposition=storepointer+originalpointer;
    return(FlicError::FLICNOERROR);
}

// <LegoRR.exe @00484770>
bool32 __cdecl Gods98::Flic_FrameChunk(FLICSTRUCT* fsp)
{
	log_firstcall();

	sint32 flicmaxchunk;
	sint8 chunkcount;
    sint8* source;
	char buff[128];

    if(((fsp->userflags)&FlicUserFlags::FLICRESIDE)==FlicUserFlags::FLICMEMORY)
    {
        source=fsp->rambufferhandle;
        source += (fsp->pointerposition);
        chunkcount =*(source+6);
        source += 16;
        fsp->pointerposition+=16;
        while((chunkcount>0)&&((fsp->pointerposition)<(fsp->fsHeader.size)))
        {
			fsp->fsSource = source;
            Flic_DoChunk(fsp);
            fsp->pointerposition+=*(sint32 *)source;
            source=(sint8 *)source+(*(sint32 *)source);
            chunkcount--;
        }
    }
    else
    {
		source=(sint8*)fsp->fsLoadBuffer;
        chunkcount=(*((sint8 *)source+6));
        flicmaxchunk=(*(sint32 *)source)-16;
        if(flicmaxchunk>fsp->fsLoadBufferSize)
        {
            if(fsp->fsLoadBuffer)
				Mem_Free(fsp->fsLoadBuffer);
            fsp->fsLoadBufferSize=flicmaxchunk;
            fsp->fsLoadBuffer=Mem_Alloc(fsp->fsLoadBufferSize);
            if(!fsp->fsLoadBuffer)
            {
                std::sprintf(buff,"Flic Not Enough Memory");
				Error_Warn(true, buff);
				return false;
            }
            source=(sint8*)(fsp->fsLoadBuffer);
        }

        File_Read(source,flicmaxchunk,1,fsp->filehandle);
        fsp->pointerposition+=16;
        while((chunkcount>0)&&((fsp->pointerposition)<(fsp->fsHeader.size)))
        {
			fsp->fsSource = source;
            Flic_DoChunk(fsp);
            fsp->pointerposition+=*(sint32 *)source;
            source=(sint8 *)source+(*(sint32 *)source);
            chunkcount--;
        }
    }
    return(true);
}

// <LegoRR.exe @004848d0>
Gods98::FlicError __cdecl Gods98::Flic_DoChunk(FLICSTRUCT* fsp)
{
	log_firstcall();

	sint16 chunktype;
	FlicError flag=FlicError::FLICNOERROR;
    chunktype=(*((sint16*)(fsp->fsSource)+2));
    switch(chunktype)
    {
        case 0x0004:
                        Flic_Palette256(fsp);
                        flag=FlicError::FLICFINISHED;
                        break;
        case 0x0007:
                        Flic_DeltaWord(fsp);
                        flag=FlicError::FLICFINISHED;
                        break;
        case 0x000b:
                        Flic_Palette64(fsp);
                        flag=FlicError::FLICFINISHED;
                        break;
        case 0x000c: 
                        Flic_DeltaByte(fsp);
                        flag=FlicError::FLICFINISHED;
                        break;
        case 0x000d:
                        Flic_Black(fsp);
                        flag=FlicError::FLICFINISHED;
                        break;
        case 0x000f:
                        Flic_BrunDepack(fsp);
                        flag=FlicError::FLICFINISHED;
                        break;
        case 0x0010:
                        Flic_Copy(fsp);
                        flag=FlicError::FLICFINISHED;
                        break;

        case 0x0019:
                        Flic_BrunDepack(fsp);
                        flag=FlicError::FLICFINISHED;
                        break;
        case 0x001b:
                        Flic_DeltaWord(fsp);
                        flag=FlicError::FLICFINISHED;
                        break;
        case 0x5344:
//                         Flic_SoundChunk(fsp);
                        flag=FlicError::FLICFINISHED;
                        break;                         
        case 0x5555:
                        Flic_Unpack(fsp);
                        flag=FlicError::FLICFINISHED;
                        break;                         
        default:     break;
    }
    return(flag);
}


// (unsused and commented out in Flic.prot)
// <unused>
sint32 __cdecl Gods98::Flic_FindMaxChunk(FLICSTRUCT* fsp)
{
	void *source;
	sint32 max=0;
	sint32 chunklength;
	sint32 storepointer,storefpointer;
    sint8 buffer[16];

    storepointer=fsp->pointerposition;
    storefpointer=File_Tell(fsp->filehandle);
    source=&buffer;
    fsp->pointerposition=128;

    while(((fsp->pointerposition)<(fsp->fsHeader.size)))
    {
        File_Seek(fsp->filehandle,fsp->pointerposition,SEEK_SET);
        File_Read(&buffer,16,1,fsp->filehandle);
        chunklength=*(sint32 *)source;
        if(chunklength>max) max=chunklength;
        fsp->pointerposition+=chunklength;
    }

    File_Seek(fsp->filehandle,storefpointer,SEEK_SET);
    fsp->pointerposition=storepointer;
    return(max);
}


// Function to load pointers for each frame of flic
// <LegoRR.exe @004849e0>
Gods98::FlicError __cdecl Gods98::Flic_LoadPointers(FLICSTRUCT* fsp)
{
	log_firstcall();

	sint32 flicmaxchunk;
    sint8 chunkcount;
    void *source;
	char buff[256];

    if(((fsp->userflags)&FlicUserFlags::FLICRESIDE)==FlicUserFlags::FLICMEMORY)
    {
        source=(fsp->rambufferhandle);
        source=(sint8 *)source+fsp->pointerposition;
    }
    else
    {
        source=(fsp->fsLoadBuffer);
        flicmaxchunk=(*(sint32*)source);
        if(flicmaxchunk>fsp->fsLoadBufferSize)
        {
            if(fsp->fsLoadBuffer)
				Mem_Free(fsp->fsLoadBuffer);
            fsp->fsLoadBufferSize=flicmaxchunk;
            fsp->fsLoadBuffer=Mem_Alloc(fsp->fsLoadBufferSize);
            if(!fsp->fsLoadBuffer)
            {

                std::sprintf(buff, "Flic Not Enough Memory");
				Error_Warn(true, buff);
				return FlicError::FLICNOERROR; // returned as (FlicError)false;
            }
            source=(fsp->fsLoadBuffer);
        }

        File_Seek(fsp->filehandle,fsp->pointerposition,SEEK_SET);
        File_Read(source,flicmaxchunk,1,fsp->filehandle);
        }

		chunkcount=(flicmaxchunk/8);
		source=(sint8 *)source+6;


//			fsp->fsPointer = malloc( flicmaxchunk );

    return(FlicError::FLICNOERROR);
}

// Function to load an 8 bit palette
// <LegoRR.exe @00484a90>
Gods98::FlicError __cdecl Gods98::Flic_LoadPalette64(FLICSTRUCT* fsp)
{
	log_firstcall();

	sint32 flicmaxchunk;
    sint8* source;
	char buff[128];		  		  	

    if(((fsp->userflags)&FlicUserFlags::FLICRESIDE)==FlicUserFlags::FLICMEMORY)
    {
        source=(fsp->rambufferhandle);
        source=(sint8*)source+fsp->pointerposition;
    }
    else
    {
        source=(sint8*)(fsp->fsLoadBuffer);
        flicmaxchunk=(*(sint32 *)source);
        if(flicmaxchunk>fsp->fsLoadBufferSize)
        {
            if(fsp->fsLoadBuffer)
				Mem_Free(fsp->fsLoadBuffer);
            fsp->fsLoadBufferSize=flicmaxchunk;
            fsp->fsLoadBuffer=Mem_Alloc(fsp->fsLoadBufferSize);
            if(!fsp->fsLoadBuffer)
            {
                std::sprintf(buff, "Flic Not Enough Memory");
                Error_Warn(true, buff);
				return (FlicError::FLICFINISHED); // returned as (FlicError)true;
            }
            source=(sint8*)fsp->fsLoadBuffer;
        }

        File_Seek(fsp->filehandle,fsp->pointerposition,SEEK_SET);
        File_Read(source,flicmaxchunk,1,fsp->filehandle);
        }

	
    return(FlicError::FLICNOERROR);
}

// <LegoRR.exe @00484b40>
bool32 __cdecl Gods98::Flic_Copy(FLICSTRUCT* fsp)
{
	log_firstcall();

	if(fsp->fsBitPlanes==16)
	{
		FlicCopyHiColorFlic(fsp);
		return true;
	}
	if(fsp->fsDisplayMode==FlicMode::FLICMODE_BYTEPERPIXEL)
	{
		FlicCopyBytePerPixel(fsp);
	}
	if(fsp->fsDisplayMode==FlicMode::FLICMODE_HICOLOR)
	{
		FlicCopyHiColor(fsp);
	}
	
	return true;
}


// Simply returns 100
// <unused>
sint32 __cdecl Gods98::Flic_GetEventRate(void)
{
	int timer;

	timer = 100;

    return(timer);
}

/*
// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 Gods98::FlicBRunDepackBytePerPixel(FLICSTRUCT* fsp) { return true; }
*/

// <LegoRR.exe @00484b90>
bool32 __cdecl Gods98::FlicBRunDepackHiColor(FLICSTRUCT* fsp)
{
	log_firstcall();

	uint8 *src, wrd;
	uint16 *dst;
	uint16 cnt,width,height,line;

//421 6259 karl

	src = (uint8*)fsp->fsSource;
	src += 6;

	height = fsp->fsYsize;
	line = 0;
	src++;
	while(line<height)
	{
		width = fsp->fsXsize;
		dst = (uint16*)(fsp->fsSPtr)+(fsp->fsPitch/2)*line;

		while(width>0)
		{
			cnt=*src++;
			
			if(cnt<128)
			{
				width -= cnt;
				wrd = *src++;
				while(cnt)
				{
					*dst++ = FHCOL(wrd);
					cnt--;
				}
			}
			else
			{
				cnt = 256-cnt;
				width -= cnt;
				while(cnt)
				{
					wrd = *src++;
					*dst++ = FHCOL(wrd);
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
bool32 __cdecl Gods98::FlicBRunDepackHiColorFlic32k(FLICSTRUCT* fsp)
{
	log_firstcall();

	sint16 height,width;
	uint8 *src;
	uint16 wrd, cnt, tmp, line;
	uint16*dst;

	dst=(uint16*)fsp->fsSPtr;
	src= (uint8*)fsp->fsSource;
	src+=6;
	src++;

	line = 0;
	height = fsp->fsYsize;
	while(line<height)
	{
		width  = fsp->fsXsize;
		dst = (uint16*)(fsp->fsSPtr) + ((fsp->fsPitch/2) * line);
		
		while(width>0)
		{
			cnt = *src++;
			if(cnt < 128)
			{
				width -= cnt;
				wrd = *(uint16*)src;
				src += 2;
				tmp = ((wrd & 0xffe0)<<1);
				tmp |= wrd & 0x1f;
				wrd = tmp;
				while(cnt)
				{
					*dst++ = DoScreenConversion(fsp, wrd);
					cnt--;
				}
				
			}
			else
			{
				cnt = 256-cnt;
				width -= cnt;
				while(cnt)
				{
					tmp = *(uint16 *)src;
					src += 2;
					wrd = ((tmp & 0xffe0)<<1);
					wrd |= tmp & 0x1f;
					*dst++ = DoScreenConversion(fsp, wrd);
					
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
__inline bool32 Gods98::FlicBRunDepackHiColorFlic(FLICSTRUCT* fsp) { return true; }

// return 0;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline sint32 Gods98::UnpackM2(void* input, void* output) { return 0; }
*/

// <LegoRR.exe @00484de0>
bool32 __cdecl Gods98::Flic_BrunDepack(FLICSTRUCT* fsp)
{
	log_firstcall();

	if(fsp->fsHeader.depth == 8)
	{
		if(fsp->fsDisplayMode== FlicMode::FLICMODE_BYTEPERPIXEL)
		{
			FlicBRunDepackBytePerPixel(fsp);
		}
		if(fsp->fsDisplayMode==FlicMode::FLICMODE_HICOLOR)
		{
			FlicBRunDepackHiColor(fsp);
		}

	}
	else if(fsp->fsHeader.depth == 16)
	{
		if(fsp->fsHeader.magic == 0xaf43)
		{
			FlicBRunDepackHiColorFlic32k(fsp);
		}
		else
		{
			FlicBRunDepackHiColorFlic(fsp);
		}
	}
	return true;
}

/*
// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 Gods98::Flic_Black(FLICSTRUCT* fsp) { return true; }

// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 Gods98::Flic_DeltaByte(FLICSTRUCT* fsp) { return true; }

// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 Gods98::Flic_Palette64(FLICSTRUCT* fsp) { return true; }

// (shared, internal)
// DON'T HOOK THIS, it's internal only(?)
// <LegoRR.exe @00484e50>
//bool32 __cdecl Gods98::Flic_NERPFunc__True(sint32* stack); // return 1;
*/

// <LegoRR.exe @00484e60>
void __cdecl Gods98::FlicCreateHiColorTable(FLICSTRUCT* fsp)
{
	log_firstcall();

	uint8* src;
	uint16* dst, wrd, col;

	src=(uint8 *)&fsp->fsPalette256;
	dst=(uint16*)&fsp->fsPalette64k;

	for(int i=0;i<256;i++)
	{
		col = src[0];
		col >>= 1;
		col <<= 11;

		wrd = src[1];
		wrd <<=5;

		col |= wrd;

		wrd = src[2];
		wrd >>= 1;

		col |= wrd;

		*dst++ = col;

		src += 3;

	}
	fsp->fsPalette64k[0]=0x00;
}

// <LegoRR.exe @00484ec0>
bool32 __cdecl Gods98::Flic_Palette256(FLICSTRUCT* fsp)
{
	log_firstcall();

	uint8* src;
	uint8* dst, col;
	uint16 cnt, indx, ccnt;

	src = (uint8*)fsp->fsSource;
	src+=6;

	cnt = *(uint16*)src;
	src+=2;

	dst = (uint8*)&fsp->fsPalette256;

	while(cnt)
	{
		indx = *src++;
		dst += (indx*3);
		ccnt = *src++;

		if(!ccnt)
		{
			cnt=1;
			ccnt=256;
			dst = (uint8*)&fsp->fsPalette256;
		}
		ccnt *= 3;
		while(ccnt)
		{
			col	= *src++;
			col >>= 2;

			*dst++ = col;
			ccnt--;
		}
		cnt--;
	}

	if(fsp->fsDisplayMode == FlicMode::FLICMODE_HICOLOR)
	{
	    FlicCreateHiColorTable(fsp);
	}


	return true;
}

/*
// return 0;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline sint32 Gods98::Flic_Unpack(FLICSTRUCT* fsp) { return 0; }

// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void Gods98::FlicCopyHiColor(FLICSTRUCT* fsp) { return; }

// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void Gods98::FlicCopyBytePerPixel(FLICSTRUCT* fsp) { return; }

// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void Gods98::FlicCopyHiColorFlic(FLICSTRUCT* fsp) { return; }

// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void Gods98::FlicDeltaWordBytePerPixel(FLICSTRUCT* fsp) { return; }

// (shared, internal)
// DON'T HOOK THIS, it's internal only(?)
// <LegoRR.exe @00484f50>
//void __cdecl Gods98::Flic_logf_removed(const char* msg, ...); // return;
*/

// <LegoRR.exe @00484f60>
void __cdecl Gods98::FlicDeltaWordHiColor(FLICSTRUCT* fsp)
{
	log_firstcall();

	uint8* dst;
	uint8* src, w1, w2, mfval;
	uint16 wrd, cnt, ctrl, cpy, mcnt, mflag, line, height, offs;

	dst=(uint8*)fsp->fsSPtr;
	height = fsp->fsYsize;

	src = (uint8*)fsp->fsSource;
	src+=6;

	line = 0;
	cnt = *(uint16*)src;
	src += 2;
	while(cnt)
	{
		dst=(uint8 *)(fsp->fsSPtr)+((fsp->fsPitch) * line);

		ctrl = *(uint16*)src;
		ctrl >>= 12;
		ctrl &= 0xc;
		if(ctrl == 0xc)
		{
			wrd = *(uint16*)src;
			src += 2;
			wrd = 65536-wrd;

			line += wrd;
		}
		else if(ctrl == 0x4)
		{
			return;
		}
		else
		{
			mflag = 0;
			if(ctrl == 0x8)
			{
				mfval = *src;
				src += 2;
				mflag = 1;
			}
			mcnt = *(uint16*)src;
			src += 2;
			while(mcnt)
			{
				offs = *src++;
				dst += 2*(offs);
				cpy = *src++;
				if((cpy & 0x80))
				{
					cpy = 256 - cpy;
					w1= *src++;
					w2= *src++;
					while(cpy)
					{
						*(uint16*)dst = FHCOL(w1);
						dst += 2;
						*(uint16*)dst = FHCOL(w2);
						dst += 2;
						cpy--;
					}
				}
				else
				{
					while(cpy)
					{
						w1 = *src++;
						w2 = *src++;
						*(uint16*)dst = FHCOL(w1);
						dst += 2;
						*(uint16*)dst = FHCOL(w2);
						dst += 2;
						
						cpy--;
					}
					
				}
				mcnt--;
			}
			if(mflag)
				*(uint16*)dst = FHCOL(mfval);
			cnt--;
			line++;
		}
	}
}

/*
// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void Gods98::FlicDeltaWordHiColorDZ(FLICSTRUCT* fsp) { return; }
*/

// <LegoRR.exe @00485110>
void __cdecl Gods98::FlicDeltaWordHiColorFlic32k(FLICSTRUCT* fsp)
{
	log_firstcall();

	uint8* dst;
	uint8* src;
	uint16 wrd, cnt, ctrl, cpy, mcnt, tmp, mflag, mfval, line, height;

	dst=(uint8*)fsp->fsSPtr;
	height = fsp->fsYsize;

	src = (uint8*)fsp->fsSource;
	src+=6;

	line = 0;
	cnt = *(uint16*)src;
	src += 2;
	while(cnt)
	{
		dst=(uint8 *)(fsp->fsSPtr)+((fsp->fsPitch) * line);

		ctrl = *(uint16*)src;
		ctrl >>= 12;
		ctrl &= 0xc;
		if(ctrl == 0xc)
		{
			wrd = *(uint16*)src;
			src += 2;
			wrd = 65536-wrd;

			line += wrd;

		}
		else if(ctrl == 0x4)
		{
			return;
		}
		else
		{
			mflag = 0;
			if(ctrl == 0x8)
			{
				mfval = DoScreenConversion(fsp, *(uint16*)src);
				src += 2;
				mflag = 1;
			}
			mcnt = *(uint16*)src;
			src += 2;
			while(mcnt)
			{
				dst += 2*(*src++);
				cpy = *src++;
				if((cpy & 0x80))
				{
					tmp = *(uint16*)src;
					src += 2;
					wrd = ((tmp & 0xffe0)<<1);
					wrd |= tmp & 0x1f;
					cpy = 256 - cpy;
					while(cpy)
					{
						*(uint16*)dst = DoScreenConversion(fsp, wrd);
						dst += 2;
						cpy--;
					}
				}
				else
				{
					while(cpy)
					{
						tmp = *(uint16*)src;
						src += 2;
						wrd = ((tmp & 0xffe0)<<1);
						wrd |= tmp & 0x1f;
						*(uint16*)dst = DoScreenConversion(fsp, wrd);
						dst += 2;
						
						cpy--;
					}
					
				}
				mcnt--;
			}
			if(mflag)
				*dst = (sint8)mfval;
			line ++;
			cnt--;
		}
	}
}

/*
// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void Gods98::FlicDeltaWordHiColorFlic(FLICSTRUCT* fsp) { return; }
*/

// <LegoRR.exe @004852f0>
bool32 __cdecl Gods98::Flic_DeltaWord(FLICSTRUCT* fsp)
{
	log_firstcall();

	if(fsp->fsHeader.depth == 8)
	{
		if(fsp->fsDisplayMode==FlicMode::FLICMODE_BYTEPERPIXEL)
		{
		    FlicDeltaWordBytePerPixel(fsp);
		}
		if(fsp->fsDisplayMode==FlicMode::FLICMODE_HICOLOR)
		{
		    FlicDeltaWordHiColor(fsp);
		}
	}
	else
	{
		if(fsp->fsHeader.depth == 16)
		{
			if(fsp->fsHeader.magic == 0x1234)
			{
			    FlicDeltaWordHiColorDZ(fsp);
			}
			else if(fsp->fsHeader.magic == 0xaf43)
			{
			    FlicDeltaWordHiColorFlic32k(fsp);
			}
			else
			{
                FlicDeltaWordHiColorFlic(fsp);
			}
		}
	}
	return true;
}

// <LegoRR.exe @00485380>
uint16 __cdecl Gods98::getFlicCol(uint8 n, FLICSTRUCT* fsp)
{
	log_firstcall();

	uint16* ctab = fsp->fsPalette64k;

	uint16 ret = ctab[n];

	return ret;
}


// (shared) "AnimClone_IsLws__Flic_GetWidth"
// THIS FUNCTION MUST BE HOOKED ON AN INDIVIDUAL BASIS
// There are 5 calls made to this:
//  type:FLICSTRUCT (Flic_GetWidth)  -> FUN_004120e0  <@004120f7>
//                                      Panel_FUN_0045a9f0  <@0045ab17>
//                                      Pointer_DrawPointer  <@0045cfc8>
//  type:FlocksData (Flocks_???)     -> LiveObject_Flocks_FUN_0044bef0  <@0044bfc3>
//  type:AnimClone (AnimClone_IsLws) -> Container_FormatPartName  <@00473f60>
// <called @004120f7, 0045ab17, 0045cfc8>
// <LegoRR.exe @00489a90>
uint32 __cdecl Gods98::Flic_GetWidth(FLICSTRUCT* fsp)
{
	log_firstcall();

	return (uint32)fsp->fsXsize;
}


// <LegoRR.exe @004853a0>
uint32 __cdecl Gods98::Flic_GetHeight(FLICSTRUCT* fsp)
{
	log_firstcall();

	return (uint32)fsp->fsYsize;
}

#pragma endregion
