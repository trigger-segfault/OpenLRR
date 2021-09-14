#pragma once

#include "../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef void (__cdecl* SoundCDStopCallback)(void);

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define WAVEVERSION 1

#ifndef ER_MEM
#define ER_MEM 				0xe000
#endif

#ifndef ER_CANNOTOPEN
#define ER_CANNOTOPEN 		0xe100
#endif

#ifndef ER_NOTWAVEFILE
#define ER_NOTWAVEFILE 		0xe101
#endif

#ifndef ER_CANNOTREAD
#define ER_CANNOTREAD 		0xe102
#endif

#ifndef ER_CORRUPTWAVEFILE
#define ER_CORRUPTWAVEFILE	0xe103
#endif

#ifndef ER_CANNOTWRITE
#define ER_CANNOTWRITE		0xe104
#endif

#define SAMP_MULTI 2
#define SAMP_LOOP 1
#define SAMP_ONCE 0

#define MAX_SAMPLES 100

#define MULTI_SOUND 5

#define MCI_RETURN_SIZE 200

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

/*enum_scoped(SoundMode2) : uint32
{
	Sound_Multi = SAMP_MULTI,
	Sound_Loop  = SAMP_LOOP,
	Sound_Once  = SAMP_ONCE
};
static_assert(sizeof(SoundMode2) == 0x4, "");
enum_scoped_end(SoundMode2);*/

namespace _ns_SoundMode {
enum SoundMode : uint32
{
	Sound_Multi = SAMP_MULTI,
	Sound_Loop  = SAMP_LOOP,
	Sound_Once  = SAMP_ONCE
};
static_assert(sizeof(SoundMode) == 0x4, "");
} using SoundMode = _ns_SoundMode::SoundMode;

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

// (unused)
struct SND
{
	char sName[128];
	sint32 sSize;
	sint32 cFreq;
	sint32 cVolume;
	sint32 sOffset;
	sint32 sActive;
	char* sData;
	WAVEFORMATEX* pwf;
	IDirectSoundBuffer* lpDsb;
	IDirectSoundBuffer* lpDsb3D;
#ifdef MULTI_SOUND
	IDirectSoundBuffer* lpDsbM[MULTI_SOUND];
	IDirectSoundBuffer* lpDsbM3D[MULTI_SOUND];
	sint32 sVoice;
	sint32 sVoice3D;
#endif
};


struct Sound
{
	/*0,4*/ uint32 handle;
	/*4*/
};// Sound, * lpSound;
static_assert(sizeof(Sound) == 0x4, "");


struct Sound_Globs
{
	// [globs: start]
	/*000,4*/ uint32 useSound;		// Number of sounds in soundList
	/*004,4*/ bool32 initialised;
	/*008,190*/ Sound soundList[MAX_SAMPLES];
	/*198,4*/ sint32 currTrack;
	/*19c,4*/ bool32 loopCDTrack;
	/*1a0,4*/ SoundCDStopCallback CDStopCallback;
	/*1a4,4*/ bool32 updateCDTrack;
	// [globs: end]
	/*1a8,4*/ uint32 s_Update_lastUpdate;
	/*1ac,8*/ uint32 reserved1[2];
	/*1b4,4*/ MCIERROR mciErr;
	/*1b8*/
};
static_assert(sizeof(Sound_Globs) == 0x1b8, "");

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

//extern SND sndTbl[MAX_SAMPLES];

// <LegoRR.exe @00545868>
extern Sound_Globs & soundGlobs;

// <LegoRR.exe @00577500>
extern char (& mciReturn)[MCI_RETURN_SIZE];

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00488e10>
bool32 __cdecl Sound_Initialise(bool32 nosound);

// <LegoRR.exe @00488e50>
bool32 __cdecl Sound_IsInitialised(void);

// <LegoRR.exe @00488e70>
bool32 __cdecl Sound_PlayCDTrack(uint32 track, SoundMode mode, SoundCDStopCallback StopCallback);

// <LegoRR.exe @00488eb0>
bool32 __cdecl Sound_StopCD(void);

// <LegoRR.exe @00488ec0>
void __cdecl Sound_Update(bool32 cdtrack);

// <LegoRR.exe @00488f30>
sint32 __cdecl WaveOpenFile(void* fileData, uint32 fileSize,
				OUT HMMIO* phmmioIn, OUT WAVEFORMATEX** ppwfxInfo, OUT MMCKINFO* pckInRIFF);

// <LegoRR.exe @00489130>
uint32 __cdecl GetWaveAvgBytesPerSec(const char* pszFileName);

// <LegoRR.exe @004891d0>
sint32 __cdecl WaveOpenFile2(IN const char* pszFileName,
				OUT HMMIO* phmmioIn, OUT WAVEFORMATEX** ppwfxInfo, OUT MMCKINFO* pckInRIFF);

// <LegoRR.exe @00489380>
sint32 __cdecl WaveStartDataRead(HMMIO* phmmioIn, MMCKINFO* pckIn, MMCKINFO* pckInRIFF);

// <LegoRR.exe @004893c0>
sint32 __cdecl WaveReadFile(IN HMMIO hmmioIn, IN uint32 cbRead, OUT uint8* pbDest,
				IN MMCKINFO* pckIn, OUT uint32* cbActualRead);

// <LegoRR.exe @00489490>
sint32 __cdecl WaveCloseReadFile(IN HMMIO* phmmio, IN WAVEFORMATEX** ppwfxSrc);

// <LegoRR.exe @004894d0>
bool32 __cdecl Restart_CDTrack(sint32 track);

// <LegoRR.exe @00489520>
void __cdecl ReportCDError(void);

// <LegoRR.exe @00489540>
bool32 __cdecl Status_CDTrack(sint32 track);

// <LegoRR.exe @004895f0>
bool32 __cdecl Play_CDTrack(sint32 track);

// <LegoRR.exe @00489660>
bool32 __cdecl Stop_CDTrack(void);

#pragma endregion

}
