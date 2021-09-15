
#include <d3drm.h>
#include <dsound.h>
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

#include "Sound.h"
#include "Errors.h"
#include "Memory.h"
#include "Containers.h"
#include "3DSound.h"
#include "Files.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

/// TODO: Remove me once Container module is finished
namespace Gods98 {
	struct Container;
	struct AnimClone;
}
#define Container_Frame_SetAppData ((void(__cdecl*)(IDirect3DRMFrame3*, Container*, AnimClone*, const char*, uint32*, const char*, real32*, real32*, const char*, void*, uint32*))0x00476230)


/*// <LegoRR.exe @00476230>
void __cdecl Container_Frame_SetAppData(IDirect3DRMFrame3* frame, Container* owner,
	OPTIONAL AnimClone* animClone, OPTIONAL const char* asfname, OPTIONAL uint32* frameCount,
	OPTIONAL const char* frameName, OPTIONAL real32* currTime, OPTIONAL real32* transCo,
	OPTIONAL const char* actSample, OPTIONAL void* soundRecord, OPTIONAL uint32* trigger);*/

#pragma endregion



/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

//[0xFFFFD8F0, 0xFFFFEC78, 0xFFFFF254, 0xFFFFF830, 0xFFFFFA24, 0xFFFFFC18, 0xFFFFFD44, 0xFFFFFE70, 0xFFFFFF38, 0xFFFFFFB0, 0x0]
// { -10000, -5000, -3500, -2000, -1500, -1000, -700, -400, -200, -80, 0 }
// <LegoRR.exe @004abe30>
const sint32 (& Gods98::c_SetGlobalVolumePrescaled_realVol)[11] = *(sint32(*)[11])0x004abe30;

// <LegoRR.exe @005075b8>
Gods98::Sound3D_Globs & Gods98::sound3DGlobs = *(Gods98::Sound3D_Globs*)0x005075b8;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0047a900>
bool32 __cdecl Gods98::Sound3D_Initialise(HWND hwndParent)
{
	log_firstcall();

	DSBUFFERDESC dsbd;
    sint32 ret = IDRETRY;

	sound3DGlobs.intialised = false;
	sound3DGlobs.listenerFrame = nullptr;

	// Try to create the Directsound objects until we either do it, are told to ignore it, or are told to abort
	while( ret == IDRETRY )
	{
		// Create the directsound object
		if( ::DirectSoundCreate(nullptr, &sound3DGlobs.lpDSnd, nullptr) == DS_OK)
		{	// Set cooperative level - NEEDS TO BE 'DSSCL_PRIORITY' OR HIGHER TO DO 'SetFormat'
			if( lpDSnd()->SetCooperativeLevel(hwndParent, DSSCL_PRIORITY) == DS_OK )
			{
				// Create a primary buffer so we can query for a 3D Listener interface
				std::memset( &dsbd, 0, sizeof(DSBUFFERDESC) );
				dsbd.dwSize = sizeof(DSBUFFERDESC);
				dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME;
				if( lpDSnd()->CreateSoundBuffer(&dsbd, &sound3DGlobs.lpDSBuff, nullptr) == DS_OK)
				{					
					// Make the primary 44.1 KHz so that it sounds better
					WAVEFORMATEX wfx;
					wfx.wFormatTag = WAVE_FORMAT_PCM;
					wfx.nChannels = 2;
					wfx.wBitsPerSample = 16;
					wfx.nSamplesPerSec = 22050;
					wfx.nBlockAlign = wfx.wBitsPerSample / 8 * wfx.nChannels;
					wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign; 
					//wfx.nBlockAlign = 4;
					//wfx.nAvgBytesPerSec = 44100*2*2;
					wfx.cbSize = 0;
					if( lpDSBuff()->SetFormat(&wfx) != DS_OK )
						Error_Warn( true, "Could not set format on primary buffer - low quality output will be used." );
					
					// Get the 3D listener information
					if( lpDSBuff()->QueryInterface(IID_IDirectSound3DListener, (void**) &sound3DGlobs.lp3DListenerInfo) == DS_OK)
						sound3DGlobs.lp3DListenerInfo->SetDopplerFactor( D3DVAL(100.0), DS3D_IMMEDIATE );
					else 
					{
						Error_Warn( true, "DirectSound 3D failed to get listener info." );
						lpDSBuff()->Release();
						lpDSnd()->Release();
						sound3DGlobs.lpDSnd = nullptr;
					}
				}
				else
				{	
					Error_Warn( true, "DirectSound 3D failed to create a primary buffer." );
					lpDSnd()->Release();
					sound3DGlobs.lpDSnd = nullptr;
				}
			}
			else
			{	
				Error_Warn( true, "DirectSound 3D failed to set cooperative level." );
				lpDSnd()->Release();
				sound3DGlobs.lpDSnd = nullptr;
			}
		}

		if( !lpDSnd() )
		{	
			Error_Warn( true, "DirectSound 3D could not initialize." );
//			ret = MessageBox( hwndParent, "DirectSound 3D could not initialize", "Warning", MB_APPLMODAL|MB_ICONWARNING|MB_ABORTRETRYIGNORE );
//			if (ret == IDABORT)
//				return false;
			ret = IDIGNORE;
		}
		else
		{	
			ret = IDOK;

			sound3DGlobs.intialised = true;
		}
	}
	
	if( Sound3D_Initialised() )
	{
		//SET THE PRIMARY BUFFER TO CONTINUOUSLY PLAY - OPTIMISATION FOR FREQUENT STARTING AND STOPPING OF PLAYBACK
		lpDSBuff()->Play( 0,0,DSBPLAY_LOOPING );

		//DEFAULT VALUES
		lp3DListenerInfo()->SetRolloffFactor( 1, DS3D_DEFERRED );
		//DISABLE DOPPLER - DONT NEED TO SET VELOCITY ON SOUNDS
		lp3DListenerInfo()->SetDopplerFactor( 0, DS3D_DEFERRED );
		
		if( lp3DListenerInfo()->CommitDeferredSettings() != DS_OK)
			Error_Warn( true, "DirectSound 3D failed commit listener info settings." );

		sound3DGlobs.minDistanceForAttentuation = 1;//DS3D_DEFAULTMINDISTANCE;
		sound3DGlobs.maxDistance = 800;//DS3D_DEFAULTMAXDISTANCE;

		lpDSBuff()->GetVolume( (LPLONG)&sound3DGlobs.windowsVolume );

		Sound3D_SetVolumeToDefault();
	}

	return true;
}

// <LegoRR.exe @0047aac0>
void __cdecl Gods98::Sound3D_ShutDown(void)
{
	log_firstcall();

	if( Sound3D_Initialised() )
	{
		Sound3D_SetGlobalVolume( sound3DGlobs.windowsVolume );

		lpDSBuff()->Stop();

		lpDSBuff()->Release();
		lpDSnd()->Release();
		sound3DGlobs.lpDSnd = nullptr;

		sound3DGlobs.intialised = false;
	}
}

// <LegoRR.exe @0047ab10>
bool32 __cdecl Gods98::Sound3D_CheckVolumeLimits(sint32 vol)
{
	if( (vol > DSBVOLUME_MAX) || (vol < DSBVOLUME_MIN) )
	{
		Error_Warn( true, "Invalid volume specified." );

		return false;
	}

	return true;
}

// <LegoRR.exe @0047ab30>
sint32 __cdecl Gods98::Sound3D_Load(const char* fName, bool32 stream, bool32 simultaneous, sint32 volume)
{
	log_firstcall();

	bool32 okay = false;
	char buffer[MAX_PATH];
	FILE *file;
	uint32 freeSoundIndex;


	if (Sound3D_Initialised()) {

		if (Sound3D_GetFreeSoundIndex(&freeSoundIndex)) {

			sound3DGlobs.soundTable[freeSoundIndex].flags = Sound3DFlags::SOUND3D_FLAG_NONE;
			std::sprintf(buffer, "%s.wav", fName);

			if (stream) {

				const char* hdFileName = File_VerifyFilename(buffer);
				const char* useFile = nullptr;
				char cdFileName[FILE_MAXPATH];
				FILE *mfp;

				if (mfp = std::fopen(hdFileName, "r")) {
					useFile = hdFileName;
					std::fclose(mfp);
				} else {
					if (File_GetCDFilePath(cdFileName, buffer)) {
						useFile = cdFileName;
					}
				}

				if (useFile) {

					Error_Fatal(simultaneous, "Cannot have a multi streaming sound!");

					//NEED TO FIND THE SIZE OF THE FILE FOR 'Sound3D_GetSamplePlayTime'
					if (file = std::fopen(useFile, "r")) {

						std::fseek(file, 0, SEEK_END);

						sound3DGlobs.soundTable[freeSoundIndex].size = std::ftell(file);
						sound3DGlobs.soundTable[freeSoundIndex].avgBytesPerSec = GetWaveAvgBytesPerSec(useFile);
						sound3DGlobs.soundTable[freeSoundIndex].flags |= Sound3DFlags::SOUND3D_FLAG_STREAM;

						std::fclose(file);
						
						okay = true;
					}
				}
			}
			else if( Sound3D_LoadSample(&sound3DGlobs.soundTable[freeSoundIndex], buffer, simultaneous) )
			{
				sound3DGlobs.soundTable[freeSoundIndex].flags &= ~Sound3DFlags::SOUND3D_FLAG_STREAM;
				if( simultaneous )
					sound3DGlobs.soundTable[freeSoundIndex].flags |= Sound3DFlags::SOUND3D_FLAG_MULTI;
				
				okay = true;
			}
			
			if( okay )
			{
				std::strcpy( sound3DGlobs.soundTable[freeSoundIndex].fName, buffer );
				
				if( Sound3D_CheckVolumeLimits(volume) )
					sound3DGlobs.soundTable[freeSoundIndex].volume = volume;
				else
					sound3DGlobs.soundTable[freeSoundIndex].volume = DSBVOLUME_MAX;
				
				sound3DGlobs.soundTable[freeSoundIndex].flags |= Sound3DFlags::SOUND3D_FLAG_ACTIVE;
				sound3DGlobs.soundTable[freeSoundIndex].flags |= Sound3DFlags::SOUND3D_FLAG_USED;

				return freeSoundIndex;
			}
		}
		else
			Error_Warn(true, "Could not get free sound." );

		Error_Warn( true, Error_Format("Cannot load sound \"%s\".", fName) );
	}

	return -1;
}

// <LegoRR.exe @0047ad90>
bool32 __cdecl Gods98::Sound3D_GetFreeSoundIndex(OUT uint32* soundTableIndex)
{
	log_firstcall();

	for( uint32 loopSound = 0; loopSound < SOUND3D_MAXSAMPLES; loopSound++ )
	{
		if( !(sound3DGlobs.soundTable[loopSound].flags & Sound3DFlags::SOUND3D_FLAG_USED) )
		{
			*soundTableIndex = loopSound;

			return true;
		}
	}

	Error_Fatal(true, "Run out of samples - SOUND3D_MAXSAMPLES too small." );

	return false;
}

// <LegoRR.exe @0047adc0>
bool32 __cdecl Gods98::Sound3D_Remove(sint32 soundTableIndex)
{
	log_firstcall();

	if( Sound3D_Initialised() && (soundTableIndex != -1) )
	{
		Sound3D_SoundData* soundData = &sound3DGlobs.soundTable[soundTableIndex];

		soundData->flags &= ~Sound3DFlags::SOUND3D_FLAG_USED;

		if( soundData->pwf )
		{
			::GlobalFree( soundData->pwf );
			soundData->pwf = nullptr;
		}

		for( uint32 loopBuffer = 0; loopBuffer < SOUND3D_MAXSIMULTANEOUS; loopBuffer++ )
		{
			if( soundData->lpDsb3D[loopBuffer] )
			{
				soundData->lpDsb3D[loopBuffer]->Release();
				soundData->lpDsb3D[loopBuffer] = nullptr;
			}
		}

		return true;
	}

	return false;
}

// <LegoRR.exe @0047ae40>
void __cdecl Gods98::Sound3D_MakeListener(IDirect3DRMFrame3* frame)
{
	log_firstcall();

	if( Sound3D_Initialised() )
	{
		//if( sound3DGlobs.listenerFrame )
		//	sound3DGlobs.listenerFrame->DeleteMoveCallback( Sound3D_ListenerCallback, nullptr );
		
		//frame->AddMoveCallback( Sound3D_ListenerCallback, nullptr, D3DRMCALLBACK_PREORDER );

		sound3DGlobs.listenerFrame = frame;
	}
}

// <LegoRR.exe @0047ae60>
void __cdecl Gods98::Sound3D_UpdateListener(void)
{
	log_firstcall();

	if( Sound3D_Initialised() && sound3DGlobs.listenerFrame )
		Sound3D_ListenerCallback( sound3DGlobs.listenerFrame, nullptr, 0.0f );
}

// <LegoRR.exe @0047ae80>
void __cdecl Gods98::Sound3D_ListenerCallback(LPDIRECT3DRMFRAME3 obj, void* arg, D3DVALUE delta)
{
	log_firstcall();

	Vector3F rlvCameraInfo, rlvCameraUp;
	IDirect3DRMFrame3* root;
	//static Vector3F oldPos = { 0, 0, 0 };
	//static Vector3F oldOrien = { 0, 0, 0 };
	//static Vector3F oldOrienUp = { 0, 0, 0 };
	bool32 change = false;


	obj->GetScene( &root );
	
	obj->GetPosition( root, (LPD3DVECTOR)&rlvCameraInfo );
	if( !Sound3D_D3DVectorEqual(&rlvCameraInfo, &sound3DGlobs.s_ListenerCallback_oldPos) )
	{	
		lp3DListenerInfo()->SetPosition(
			rlvCameraInfo.x, rlvCameraInfo.y, rlvCameraInfo.z, DS3D_DEFERRED );

		sound3DGlobs.s_ListenerCallback_oldPos = rlvCameraInfo;
		change = true;
	}

	obj->GetOrientation( root, (LPD3DVECTOR)&rlvCameraInfo, (LPD3DVECTOR)&rlvCameraUp );
	if( !Sound3D_D3DVectorEqual(&rlvCameraInfo, &sound3DGlobs.s_ListenerCallback_oldOrien) || !Sound3D_D3DVectorEqual(&rlvCameraUp, &sound3DGlobs.s_ListenerCallback_oldOrienUp) )
	{	
		lp3DListenerInfo()->SetOrientation(
			rlvCameraInfo.x, rlvCameraInfo.y, rlvCameraInfo.z,
			rlvCameraUp.x, rlvCameraUp.y, rlvCameraUp.z, DS3D_DEFERRED);

		sound3DGlobs.s_ListenerCallback_oldOrien = rlvCameraInfo;
		sound3DGlobs.s_ListenerCallback_oldOrienUp = rlvCameraUp;
		change = true;
	}
	
	if( change )
	{
		if( lp3DListenerInfo()->CommitDeferredSettings() != DS_OK)
			Error_Warn( true, "DirectSound 3D failed commit listener info settings." );
	}

	root->Release();
}

// <LegoRR.exe @0047afd0>
void __cdecl Gods98::Sound3D_SetWorldPos(IDirectSound3DBuffer* sound3DBuff, const Vector3F* wPos, const Vector3F* vel)
{
	log_firstcall();

	sound3DBuff->SetPosition(  wPos->x, wPos->y, wPos->z, DS3D_IMMEDIATE );
}

// <LegoRR.exe @0047aff0>
bool32 __cdecl Gods98::Sound3D_CheckAlreadyExists(IDirect3DRMFrame3* frame, IDirectSound3DBuffer* sound3DBuff)
{
	log_firstcall();

	Sound3D_SoundRecord* check = sound3DGlobs.soundRecord;
	
	while( check )
	{
		if( sound3DBuff == check->sound3DBuff )
		{
			//REMOVE THE SOUND BECAUSE IT WILL BE USED ELSEWHERE
			Sound3D_RemoveSound( check->frame, check->sound3DBuff );

			return true;
		}

		check = check->next;
	}

	return false;
}

// <LegoRR.exe @0047b030>
sint32 __cdecl Gods98::Sound3D_Play2(Sound3D_Play play, IDirect3DRMFrame3* frame, sint32 soundTableIndex, bool32 loop, OPTIONAL const Vector3F* wPos)
{
	log_firstcall();

	IDirectSoundBuffer* soundBuff;
	IDirectSound3DBuffer* sound3DBuff;
	IDirect3DRMFrame3* root;
	Vector3F cPos;
	Sound3D_SoundData* sound;

	if (Sound3D_Initialised() && (soundTableIndex) != -1) {
		
		sound = &sound3DGlobs.soundTable[soundTableIndex];

		if (sound->flags & Sound3DFlags::SOUND3D_FLAG_ACTIVE) {
			// Play a streamed sound
			if (sound->flags & Sound3DFlags::SOUND3D_FLAG_STREAM) {
				const char* hdFileName = File_VerifyFilename(sound->fName);
				const char* useFile = nullptr;
				char cdFileName[FILE_MAXPATH];
				FILE *mfp;
				Error_Fatal(Sound3D_Play::Sound3D_Play_Normal != play, "Can only play a streaming sound noramlly, not 3D.");
				if (mfp = fopen(hdFileName, "r")) {
					useFile = hdFileName;
					fclose(mfp);
				} else {
					if (File_GetCDFilePath(cdFileName, sound->fName)) {
						useFile = cdFileName;
					}
				}
				if (useFile) {
					Sound3D_Stream_Play(useFile, loop, sound->volume);
					//STREAM SOUNDS CANNOT BE PLAYED MULTI SO RETURN FIRST VOICE
					return (soundTableIndex * SOUND3D_MAXSIMULTANEOUS);
				} else return -1;
			}

			// If this is a multisound, select alternate sound buffer from list
			if( sound->flags & Sound3DFlags::SOUND3D_FLAG_MULTI ) {
				//NOTE A LOOPING/PLAYING SOUND MAY GET OVERWRITTEN IF MORE THAN 'SOUND3D_MAXSIMULTANEOUS', 'soundTableIndex' SOUNDS ARE PLAYED BEFORE THE SOUND IS STOPPED...
				if( ++sound->voice >= SOUND3D_MAXSIMULTANEOUS )
					sound->voice = 0;
				soundBuff = sound->lpDsb3D[ sound->voice ];
			}
			else
				soundBuff = sound->lpDsb3D[0];

			soundBuff->QueryInterface( IID_IDirectSound3DBuffer, (void**)&sound3DBuff );
			
			//DEFAULT SETTINGS
			{
				sint32 volume = sound->volume;
				if (Sound3D_Play::Sound3D_Play_Normal == play) volume += -800;
				soundBuff->SetVolume(volume);
			}

			sound3DBuff->SetMinDistance( sound3DGlobs.minDistanceForAttentuation, DS3D_DEFERRED );
			sound3DBuff->SetMaxDistance( sound3DGlobs.maxDistance, DS3D_DEFERRED );

			if (Sound3D_Play::Sound3D_Play_OnFrame == play) {
				sound3DBuff->SetMode(DS3DMODE_NORMAL, DS3D_DEFERRED);			
				frame->GetScene(&root);		
				Sound3D_CheckAlreadyExists(frame, sound3DBuff );
				Sound3D_AttachSound(frame, sound3DBuff );
				Sound3D_AddSoundRecord(frame, soundBuff, sound3DBuff );
				frame->GetPosition(root, (LPD3DVECTOR) &cPos);
				//Sound3D_SetWorldPos(sound3DBuff, &cPos, nullptr);
				sound3DBuff->SetPosition(cPos.x, cPos.y, cPos.z, DS3D_DEFERRED);
				root->Release();
			}
			else if (Sound3D_Play::Sound3D_Play_OnPos == play) {
				sound3DBuff->SetMode( DS3DMODE_NORMAL, DS3D_DEFERRED);
				Sound3D_SetWorldPos( sound3DBuff, wPos, nullptr);
				Sound3D_CheckAlreadyExists(nullptr, sound3DBuff );
				Sound3D_AddSoundRecord(nullptr, soundBuff, sound3DBuff );
			}
			else if (Sound3D_Play::Sound3D_Play_Normal == play) {	
				sound3DBuff->SetMode( DS3DMODE_DISABLE, DS3D_DEFERRED);

				Sound3D_CheckAlreadyExists(nullptr, sound3DBuff );

				Sound3D_AddSoundRecord(nullptr, soundBuff, sound3DBuff );
			}

			if( lp3DListenerInfo()->CommitDeferredSettings() != DS_OK )
				Error_Warn( true, "DirectSound 3D failed commit listener info settings." );

			soundBuff->SetCurrentPosition(0);
			if (loop)
				soundBuff->Play(0, 0, DSBPLAY_LOOPING);
			else
				soundBuff->Play(0, 0, 0);

			//RETURN THE INDEX OF THE BUFFER IN THE ARRAY OF DUPLICATED BUFFERS OF SOUND TYPE 'soundTableIndex'
			//JUST RETURN THE FIRST BUFFER INDEX IF THE SAMPLE IS NOT MULTI - DUPLICATES DONT EXIST
			return ((soundTableIndex*SOUND3D_MAXSIMULTANEOUS) + sound->voice);
		}
	}

	return -1;
}

// <LegoRR.exe @0047b2e0>
void __cdecl Gods98::Sound3D_AddSoundRecord(IDirect3DRMFrame3* frame, IDirectSoundBuffer* soundBuff, IDirectSound3DBuffer* sound3DBuff)
{
	log_firstcall();

	Sound3D_SoundRecord* temp = (Sound3D_SoundRecord*)Mem_Alloc( sizeof(Sound3D_SoundRecord) );
	temp->next = sound3DGlobs.soundRecord;
	temp->frame = frame;
	temp->sound3DBuff = sound3DBuff;
	temp->soundBuff = soundBuff;
	sound3DGlobs.soundRecord = temp;
	// Set default values for this sound
//	temp->sound3DBuff->SetMode(DS3DMODE_HEADRELATIVE, DS3D_IMMEDIATE);
//	temp->sound3DBuff->SetMinDistance(80, DS3D_IMMEDIATE);
//	temp->sound3DBuff->SetMaxDistance(800, DS3D_IMMEDIATE);
//	temp->sound3DBuff->SetConeAngles(360, 360, DS3D_IMMEDIATE);
}

// <LegoRR.exe @0047b310>
void __cdecl Gods98::Sound3D_SetBufferVolume(sint32 handle, sint32 newvolume)
{
	log_firstcall();

	IDirectSoundBuffer* soundBuff;
	Sound3D_SoundData* sound;
	sound = &sound3DGlobs.soundTable[handle];
	sound->volume = (Sound3D_CheckVolumeLimits(newvolume) ? newvolume : 0);
	// Now set the volume to the buffer
	if (sound->flags & Sound3DFlags::SOUND3D_FLAG_MULTI) {
		for (uint32 i=0; i<SOUND3D_MAXSIMULTANEOUS; i++) {
			if (soundBuff=sound->lpDsb3D[i]) {
				soundBuff->SetVolume(sound->volume);
			}
		}
	} else if (soundBuff=sound->lpDsb3D[0]) {
		soundBuff->SetVolume(sound->volume);
	}
}

// <LegoRR.exe @0047b390>
sint32 __cdecl Gods98::Sound3D_GetBufferVolume(sint32 handle)
{
	log_firstcall();

	const Sound3D_SoundData* sound = &sound3DGlobs.soundTable[handle];
	return sound->volume;
}

// <LegoRR.exe @0047b3b0>
IDirectSoundBuffer* __cdecl Gods98::Sound3D_GetSoundBuffer(sint32 soundHandle)
{
	log_firstcall();

	if (soundHandle == -1)
		return nullptr;

	uint32 remainder = soundHandle % SOUND3D_MAXSIMULTANEOUS /*3*/;

	return sound3DGlobs.soundTable[(soundHandle - remainder) / SOUND3D_MAXSIMULTANEOUS].lpDsb3D[remainder];
}

// <LegoRR.exe @0047b3f0>
void __cdecl Gods98::Sound3D_StopSound(sint32 soundHandle)
{
	log_firstcall();

	IDirectSoundBuffer* soundBuff = Sound3D_GetSoundBuffer(soundHandle);

	if (Sound3D_Initialised() && (soundHandle != -1))
	{
		if (soundBuff)
			soundBuff->Stop();
	}
}

// <LegoRR.exe @0047b420>
void __cdecl Gods98::Sound3D_StopAllSounds(void)
{
	log_firstcall();

	Sound3D_SoundRecord* temp = sound3DGlobs.soundRecord;

	while (temp) {
		temp->soundBuff->Stop();
		temp = temp->next;
	}

	Sound3D_Stream_Stop(false);
	Sound3D_Stream_Stop(true);
}


// <inlined>
/*__inline*/ void __cdecl Gods98::Sound3D_RegisterUpdateFrame(IDirect3DRMFrame3* frame)
{
	for (uint32 loop = 0; loop < SOUND3D_MAXUPDATEFRAMES; loop++) {
		if (sound3DGlobs.updateFrameList[loop] == nullptr) {
			sound3DGlobs.updateFrameList[loop] = frame;
//			frame->AddDestroyCallback(Sound3D_FrameRemoveCallback, frame);
			break;
		}
	}
}


// <LegoRR.exe @0047b460>
void __cdecl Gods98::Sound3D_AttachSound(IDirect3DRMFrame3* frame, IDirectSound3DBuffer* sound3DBuff)
{
	log_firstcall();

	Container_AppData *data = (Container_AppData *)frame->GetAppData();

	Sound3D_SoundFrameRecord* temp = (Sound3D_SoundFrameRecord * )Mem_Alloc( sizeof(Sound3D_SoundFrameRecord) );
	if( data )
		temp->next = data->soundList;
	else
		temp->next = nullptr;
	temp->sound3DBuff = sound3DBuff;

	temp->pos.x = 0;
	temp->pos.y = 0;
	temp->pos.z = 0;

	Container_Frame_SetAppData( frame, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, temp, nullptr);

	Sound3D_RegisterUpdateFrame(frame);

	//NOTE REMOVING THIS LINE MEANS SOUNDS WILL NEVER MOVE IF A FRAME IS MOVED
	//THEREFORE A SOUND WILL PLAY IN ITS ENTIRITY FROM WHERE IT WAS ACTIVATED
	//frame->AddMoveCallback( Sound3D_SoundCallback, nullptr, D3DRMCALLBACK_PREORDER );
}


// <inlined>
/*__inline*/ void __cdecl Gods98::Sound3D_RemoveUpdateFrame(IDirect3DRMFrame3* frame)
{
	for (uint32 loop = 0; loop < SOUND3D_MAXUPDATEFRAMES; loop++) {
		if (frame == sound3DGlobs.updateFrameList[loop]) {
			sound3DGlobs.updateFrameList[loop] = nullptr;
		}
	}
}


// <LegoRR.exe @0047b4e0>
void __cdecl Gods98::Sound3D_RemoveSound(IDirect3DRMFrame3* owner, IDirectSound3DBuffer* sound3DBuff)
{
	log_firstcall();

	Sound3D_SoundFrameRecord* temp;
	Container_AppData* data;
	
	sound3DBuff->Release();

	if( !owner )
	{	
		Sound3D_RemoveSoundRecord( nullptr, sound3DBuff );

		return;
	}

	data = (Container_AppData *)owner->GetAppData();

	if( Sound3D_RecurseRemoveSound(owner, sound3DBuff, data->soundList) )
	{	
		Sound3D_RemoveSoundRecord( owner, sound3DBuff );

		temp = data->soundList;
		data->soundList = data->soundList->next;
	 Mem_Free( temp );
	}

	Sound3D_RemoveUpdateFrame(owner);

	//if( nullptr == data->soundList )
		//owner->lpVtbl->DeleteMoveCallback( owner, Sound3D_SoundCallback, nullptr );
}

// <LegoRR.exe @0047b560>
void __cdecl Gods98::Sound3D_RemoveSoundRecord(IDirect3DRMFrame3* frame, IDirectSound3DBuffer* sound3DBuff)
{
	log_firstcall();

	Sound3D_SoundRecord* temp;
	
	if( Sound3D_RecurseRemoveSoundRecord(frame, sound3DBuff, sound3DGlobs.soundRecord) )
	{
		temp = sound3DGlobs.soundRecord;
		sound3DGlobs.soundRecord = sound3DGlobs.soundRecord->next;
		Mem_Free( temp );
	}
}

// <LegoRR.exe @0047b5a0>
bool32 __cdecl Gods98::Sound3D_RecurseRemoveSoundRecord(IDirect3DRMFrame3* owner, IDirectSound3DBuffer* sound3DBuff, Sound3D_SoundRecord* record)
{
	log_firstcall();

	Sound3D_SoundRecord* temp;
	
	if( record == nullptr )
		return false;

	if( Sound3D_RecurseRemoveSoundRecord(owner, sound3DBuff, record->next) )
	{	
		temp = record->next;
		record->next = temp->next;
		Mem_Free( temp );
	}

	//if( (owner == record->frame) && (sound3DBuff == record->sound3DBuff) )
	if( sound3DBuff == record->sound3DBuff )
		return true;

	return false;
}

// <LegoRR.exe @0047b5f0>
bool32 __cdecl Gods98::Sound3D_RecurseRemoveSound(IDirect3DRMFrame3* owner, IDirectSound3DBuffer* sound3DBuff, Sound3D_SoundFrameRecord* record)
{
	log_firstcall();

	Sound3D_SoundFrameRecord* temp;
	
	if( !record )
		return false;

	if( Sound3D_RecurseRemoveSound(owner, sound3DBuff, record->next) )
	{	
		Sound3D_RemoveSoundRecord( owner, sound3DBuff );

		temp = record->next;
		record->next = temp->next;
		Mem_Free( temp );
	}

	if( sound3DBuff == record->sound3DBuff )
		return true;

	return false;
}


// <inlined>
/*__inline*/ void __cdecl Gods98::Sound3D_UpdateFrames(void)
{
	for (uint32 loop = 0; loop < SOUND3D_MAXUPDATEFRAMES; loop++) {
		if (sound3DGlobs.updateFrameList[loop]) {
			Sound3D_SoundCallback(sound3DGlobs.updateFrameList[loop], nullptr, 0.0f);
		}
	}
}


// <LegoRR.exe @0047b650>
void __cdecl Gods98::Sound3D_Update(void)
{
	log_firstcall();

	Sound3D_SoundRecord* next;
	Sound3D_SoundRecord* temp = sound3DGlobs.soundRecord;
	DWORD status;

	Sound3D_UpdateFrames();

	while (temp) {

		next = temp->next;

		temp->soundBuff->GetStatus(&status);

		if (!(status & DSBSTATUS_PLAYING)) Sound3D_RemoveSound(temp->frame, temp->sound3DBuff);
	
		temp = next;
	}

	Sound3D_Stream_CheckPosition(false);
	Sound3D_Stream_CheckPosition(true);
}

// <LegoRR.exe @0047b6d0>
void __cdecl Gods98::Sound3D_SoundCallback(LPDIRECT3DRMFRAME3 tFrame, void* arg, D3DVALUE delay)
{
	log_firstcall();

	Container_AppData* data = (Container_AppData *)tFrame->GetAppData();
	Vector3F rlvVisualInfo;
	IDirect3DRMFrame3* root;
	Sound3D_SoundFrameRecord* temp = data->soundList;

	tFrame->GetScene(&root);
	
	while(temp)
	{
		//GET POSITION OF THE 3D SOUND WHEN IT WAS LAST SET, THEN COMPARE IT WITH THE POSITION OF ITS FRAME
		//IF DIFFERENT, UPDATE THE SOUND POSITION
		tFrame->GetPosition(root, (LPD3DVECTOR)&rlvVisualInfo);
		if (!Sound3D_D3DVectorEqual(&rlvVisualInfo, &temp->pos)) {
			temp->sound3DBuff->SetPosition( rlvVisualInfo.x, rlvVisualInfo.y,rlvVisualInfo.z, DS3D_IMMEDIATE );
			temp->pos = rlvVisualInfo;
		}

		temp = temp->next;
	}

	root->Release();
}

// <LegoRR.exe @0047b760>
void __cdecl Gods98::Sound3D_SetMinDistForAtten(real32 dist)
{
	log_firstcall();

	Sound3D_SoundRecord* temp = sound3DGlobs.soundRecord;
	
	sound3DGlobs.minDistanceForAttentuation = dist;

	while( temp )
	{	
		temp->sound3DBuff->SetMinDistance( dist, DS3D_IMMEDIATE );
		
		temp = temp->next;
	}
}

// <LegoRR.exe @0047b790>
void __cdecl Gods98::Sound3D_SetMaxDist(real32 dist)
{
	log_firstcall();

	Sound3D_SoundRecord* temp = sound3DGlobs.soundRecord;
	
	sound3DGlobs.maxDistance = dist;

	while( temp )
	{	
		temp->sound3DBuff->SetMaxDistance( dist, DS3D_IMMEDIATE );
		
		temp = temp->next;
	}
}

// <LegoRR.exe @0047b7c0>
void __cdecl Gods98::Sound3D_SetGlobalVolume(sint32 vol)
{
	log_firstcall();

	if( Sound3D_Initialised() )
	{
		if( Sound3D_CheckVolumeLimits(vol) )
		{
			sound3DGlobs.volume = vol;

			lpDSBuff()->SetVolume( vol );

			//if( sound3DGlobs.streamData.playing && sound3DGlobs.streamData.fileOpen )
			//	lpDSStreamBuff()->SetVolume( vol );
		}
	}
}

// <LegoRR.exe @0047b7f0>
void __cdecl Gods98::Sound3D_SetGlobalVolumePrescaled(sint32 vol)
{
	log_firstcall();

	//static sint32 realVol[11] = {
	//	-10000, -5000, -3500, -2000, -1500, -1000, -700, -400, -200, -80, 0
	//};

	if (vol < 0 || vol > 10) return;

	Sound3D_SetGlobalVolume(c_SetGlobalVolumePrescaled_realVol[vol]);
}

// <LegoRR.exe @0047b810>
void __cdecl Gods98::Sound3D_SetVolumeToDefault(void)
{
	log_firstcall();

	if( Sound3D_Initialised() )
	{
		static_assert(SOUND3D_DEFAULTSOUNDVOLUME == -300, "SOUND3D_DEFAULTSOUNDVOLUME does not match that of LegoRR");
		sound3DGlobs.volume = SOUND3D_DEFAULTSOUNDVOLUME;

		lpDSBuff()->SetVolume( SOUND3D_DEFAULTSOUNDVOLUME );

		//if( sound3DGlobs.streamData.playing && sound3DGlobs.streamData.fileOpen )
		//	lpDSStreamBuff()->SetVolume( SOUND3D_DEFAULTSOUNDVOLUME );
	}
}

// <LegoRR.exe @0047b840>
bool32 __cdecl Gods98::Sound3D_LoadSample(OUT Sound3D_SoundData* sound, const char* fName, bool32 simultaneous)
{
	log_firstcall();

	HMMIO hmmioIn;        
	MMCKINFO ckInRiff, ckIn;
	sint32 nError;
	uint32 cbActualRead;
	void* fileData;
	uint32 fileSize;


	std::memset( sound, 0, sizeof(Sound3D_SoundData) );

	if( fileData = File_LoadBinary(fName, &fileSize) )
	{
		if( (nError = WaveOpenFile(fileData, fileSize, &hmmioIn, &sound->pwf, &ckInRiff)) != 0 )
			goto err;

		if( (nError = WaveStartDataRead(&hmmioIn, &ckIn, &ckInRiff)) != 0 )
			goto err;

		// Ok, size of wave data is in ckIn, allocate that buffer.
		if( (sound->data = (uint8 *)::GlobalAlloc(GMEM_FIXED, ckIn.cksize)) == nullptr )
		{	
			nError = ER_MEM;
			goto err;
		}

		if( (nError = WaveReadFile(hmmioIn, ckIn.cksize, sound->data, &ckIn, &cbActualRead)) != 0 )
			goto err;
		
		sound->size = cbActualRead;

		// Close the wave file. 
		if( hmmioIn != nullptr)
		{	::mmioClose( hmmioIn, 0 );
			hmmioIn = nullptr;
		}

		if( Sound3D_CreateSoundBuffer(sound, simultaneous) == false )
			goto err;
		if( Sound3D_SendSoundToBuffer(sound) == false )
			goto err;

		if( sound->data != nullptr)
		{	
			::GlobalFree(sound->data);
			sound->data = nullptr;
		}

		Mem_Free( fileData );

		return true;
	}

err:
	Error_Warn( true, "Error loading sample." );

	if( sound->data != nullptr)
	{	
		::GlobalFree( sound->data );
		sound->data = nullptr;
	}

	return false;
}

// <LegoRR.exe @0047b980>
bool32 __cdecl Gods98::Sound3D_CreateSoundBuffer(Sound3D_SoundData* sound, bool32 simultaneous)
{
	log_firstcall();

	DSBUFFERDESC dsbuf;
	uint32 soundNum;


	std::memset( &dsbuf, 0, sizeof(DSBUFFERDESC) );
	dsbuf.dwSize = sizeof(DSBUFFERDESC);
	dsbuf.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRL3D | DSBCAPS_STATIC | DSBCAPS_MUTE3DATMAXDISTANCE;
	dsbuf.dwBufferBytes = sound->size;
	dsbuf.lpwfxFormat = sound->pwf;

	if( lpDSnd()->CreateSoundBuffer(&dsbuf, &sound->lpDsb3D[0], nullptr) != DS_OK )
	{	
		Error_Warn( true, "Cannot create sound buffer." );
		
		return false;
	}

	sound->voice = 0;

	if( simultaneous )
	{			
		for( soundNum = 1; soundNum < SOUND3D_MAXSIMULTANEOUS; soundNum++ )
		{
			if( lpDSnd()->DuplicateSoundBuffer(sound->lpDsb3D[0], &sound->lpDsb3D[ soundNum ]) != DS_OK )
			{
				Error_Warn(true, "Cannot duplicate sound buffer." );

				return false;
			}
		}
	}
	
	return true;
}

// <LegoRR.exe @0047ba50>
bool32 __cdecl Gods98::Sound3D_SendSoundToBuffer(Sound3D_SoundData* sound)
{
	log_firstcall();

	LPVOID	lpvPtr1;
	DWORD	dwBytes1=0;
	LPVOID	lpvPtr2;
	DWORD	dwBytes2=0;
	HRESULT hr;


	hr = sound->lpDsb3D[0]->Lock( sound->offset, sound->size, &lpvPtr1,
		&dwBytes1, &lpvPtr2, &dwBytes2, 0);

	if( hr == DSERR_BUFFERLOST )
	{	
		sound->lpDsb3D[0]->Restore();
		hr = sound->lpDsb3D[0]->Lock( sound->offset, sound->size, &lpvPtr1,
			&dwBytes1, &lpvPtr2, &dwBytes2, 0 );
	}
	else if( hr != DS_OK)
	{	
		Error_Warn(true, "Error locking sound buffer." );

		return false;
	}

	CopyMemory( lpvPtr1, sound->data , dwBytes1 );

	if( lpvPtr2 != nullptr )
		std::memcpy( lpvPtr2, sound->data+dwBytes1, dwBytes2 );
		//CopyMemory( lpvPtr2, sound->data+dwBytes1, dwBytes2 );
	
	if( sound->lpDsb3D[0]->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2) != DS_OK)
	{	
		Error_Warn(true, "Error unlocking sound buffer." );

		return false;
	}	

	if( sound->lpDsb3D[0]->GetFrequency((LPDWORD)&sound->freq) != DS_OK)
	{	
		Error_Warn(true, "Error getting sound frequency buffer." );

		return false;
	}

	if( sound->lpDsb3D[0]->GetVolume((LPLONG)&sound->volume) != DS_OK)
	{	
		Error_Warn(true, "Error getting sound volume buffer." );

		return false;
	}

	return true;
}

// <LegoRR.exe @0047bba0>
real32 __cdecl Gods98::Sound3D_GetSamplePlayTime(sint32 handle)
{
	log_firstcall();

	if( handle == -1 )
		return 0.0f;

	const Sound3D_SoundData* soundData = &sound3DGlobs.soundTable[handle];

	if( soundData->flags & Sound3DFlags::SOUND3D_FLAG_STREAM )
		return ( (real32)soundData->size / (real32)soundData->avgBytesPerSec );
	else
		return ( (real32)soundData->size / (real32)soundData->pwf->nAvgBytesPerSec );
}

// <LegoRR.exe @0047bc30>
bool32 __cdecl Gods98::Sound3D_Stream_Play(const char* fName, bool32 loop, sint32 volume)
{
	log_firstcall();

	Sound3D_StreamData* streamData;

	if (loop) streamData = &sound3DGlobs.loopStreamData;
	else streamData = &sound3DGlobs.streamData;

	if (streamData->playing || streamData->fileOpen) Sound3D_Stream_Stop(loop);

	if (Sound3D_Stream_BufferSetup(fName, loop, volume)) {

		// Ensure that position is at 0, ready to go
		lpDSStreamBuff(loop)->SetCurrentPosition(0);

		//MUST BE PLAYED LOOPING ON STREAM BUFFER
		lpDSStreamBuff(loop)->Play(0, 0, DSBPLAY_LOOPING);

		streamData->playing = true;
		return true;

	} else {

		streamData->playing = false;
		return false;
	}
}

// <LegoRR.exe @0047bce0>
bool32 __cdecl Gods98::Sound3D_Stream_Stop(bool32 looping)
{
	log_firstcall();

	Sound3D_StreamData* streamData;

	if (looping) streamData = &sound3DGlobs.loopStreamData;
	else streamData = &sound3DGlobs.streamData;

	streamData->playing = false;

	if (streamData->fileOpen) {

		lpDSStreamBuff(looping)->Stop();
		WaveCloseReadFile(&streamData->wiWave.hmmio, &streamData->wiWave.pwfx);
		lpDSStreamBuff(looping)->Release();

		streamData->fileOpen = false;
	}

	return true;
}

// <LegoRR.exe @0047bd60>
bool32 __cdecl Gods98::Sound3D_Stream_BufferSetup(const char* waveFName, bool32 loop, sint32 volume)
{
	log_firstcall();

	DSBUFFERDESC dsbd;
    sint32 nChkErr;
	sint32 nRem;
	Sound3D_StreamData* streamData;
	
	if (loop) streamData = &sound3DGlobs.loopStreamData;
	else streamData = &sound3DGlobs.streamData;

    if(( nChkErr = WaveOpenFile2( waveFName, &streamData->wiWave.hmmio, &streamData->wiWave.pwfx, &streamData->wiWave.mmckInRIFF )) != 0 )
    {
		Error_Warn(true, "Error opening wave file." );
		return false;
	}

    if( streamData->wiWave.pwfx->wFormatTag != WAVE_FORMAT_PCM )
    {
		Error_Warn( true, "Wave file not PCM." );
		WaveCloseReadFile( &streamData->wiWave.hmmio, &streamData->wiWave.pwfx );
		return false;
	}
	
	// Seek to the data chunk. mmck.ckSize will be the size of all the data in the file.
    if(( nChkErr = WaveStartDataRead( &streamData->wiWave.hmmio, &streamData->wiWave.mmck, &streamData->wiWave.mmckInRIFF )) != 0 )
	{
		Error_Warn(true, "Seek failed." );
		WaveCloseReadFile( &streamData->wiWave.hmmio, &streamData->wiWave.pwfx );
		return false;
	}

    // Calculate a buffer length 3 sec. long. This should be an integral number of the
	// number of bytes in one notification period. 
	streamData->wiWave.dwNotifySize = streamData->wiWave.pwfx->nSamplesPerSec * 3 * (DWORD)streamData->wiWave.pwfx->nBlockAlign;
	streamData->wiWave.dwNotifySize = streamData->wiWave.dwNotifySize / SOUND3D_NUMOFPLAYNOTIFICATIONS;
	// the notify size should be an intergral multiple of the nBlockAlignvalue.
	if ((nRem = streamData->wiWave.dwNotifySize % (DWORD)streamData->wiWave.pwfx->nBlockAlign) != 0)
		streamData->wiWave.dwNotifySize += (streamData->wiWave.pwfx->nBlockAlign - nRem);
	streamData->wiWave.dwBufferSize = streamData->wiWave.dwNotifySize * SOUND3D_NUMOFPLAYNOTIFICATIONS;	

    //Create the secondary DirectSoundBuffer object to receive our sound data.
    std::memset( &dsbd, 0, sizeof( DSBUFFERDESC ));
    dsbd.dwSize = sizeof( DSBUFFERDESC );
    // Use new GetCurrentPosition() accuracy (DirectX 2 feature)
    dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2;
    dsbd.dwBufferBytes = streamData->wiWave.dwBufferSize;

    //Set Format properties according to the WAVE file we just opened
    dsbd.lpwfxFormat = streamData->wiWave.pwfx;
    
	{
		LPDIRECTSOUNDBUFFER newBuffer;
		if (lpDSnd()->CreateSoundBuffer(&dsbd, &newBuffer, nullptr) != DS_OK) {
			Error_Warn(true, "Cannot create sound buffer." );
			return false;
		}

		if (loop) sound3DGlobs.lpDSLoopStreamBuff = newBuffer;
		else sound3DGlobs.lpDSStreamBuff = newBuffer;

	}

	lpDSStreamBuff(loop)->SetVolume(volume);
			
    streamData->wiWave.bFoundEnd = false;
	streamData->wiWave.dwNextWriteOffset = 0;
	//KEEP STREAM WRITE 2 NOTIFICATIONS BEHIND PLAY
	//AND WRITE SINGLE NOTIFICATION BLOCKS OF DATA
	streamData->wiWave.dwNextProgressCheck = streamData->wiWave.dwNotifySize * 2;

	//ADD LOOP CAP HERE
    streamData->wiWave.bLoopFile = loop;

	// Fill data in the buffer.
    Sound3D_Stream_FillDataBuffer(loop);

	// we're set to play now.
    streamData->wiWave.bDonePlaying = false;

	streamData->fileOpen = true;

    return true;
}

// <LegoRR.exe @0047bef0>
bool32 __cdecl Gods98::Sound3D_Stream_FillDataBuffer(bool32 looping)
{
	log_firstcall();

	Sound3D_StreamData* streamData;
	uint8* lpWrite1;
	uint8* lpWrite2;
    DWORD	dwLen1, dwLen2;
    uint32	uActualBytesWritten;
    DWORD dwBytes;

	if (looping) streamData = &sound3DGlobs.loopStreamData;
	else streamData = &sound3DGlobs.streamData;

	dwBytes = streamData->wiWave.dwBufferSize;

	// This is the initial read. So we fill the entire buffer.
	// This will not wrap around so the 2nd pointers will be NULL.
    if (lpDSStreamBuff(looping)->Lock(0, dwBytes, (void**)&lpWrite1, &dwLen1, (void**)&lpWrite2, &dwLen2, 0) != DS_OK) {

		Error_Warn(true, "Cannot lock stream buffer.");
		return false;
	}

    WaveReadFile(streamData->wiWave.hmmio, (uint32)dwLen1, lpWrite1, &streamData->wiWave.mmck, &uActualBytesWritten);
	
	// if the number of bytes written is less than the 
	// amount we requested, we have a short file.
	if (uActualBytesWritten < dwLen1) {
		if (!streamData->wiWave.bLoopFile) {

			// we set the bFoundEnd flag if the length is less than
			// one notify period long which is when the first notification comes in.
			// The next notification will then call send a message to process a stop. 
			if (uActualBytesWritten < streamData->wiWave.dwNotifySize) streamData->wiWave.bFoundEnd = true;
			// Fill in silence for the rest of the buffer.
			//::FillMemory(lpWrite1+uActualBytesWritten, dwLen1-uActualBytesWritten, (uint8)(streamData->wiWave.pwfx->wBitsPerSample == 8 ? 128 : 0));
			std::memset(lpWrite1 + uActualBytesWritten, (uint8)(streamData->wiWave.pwfx->wBitsPerSample == 8 ? 128 : 0), dwLen1 - uActualBytesWritten);

		} else {

			// we are looping.
			uint32 uWritten = uActualBytesWritten;	// from previous call above.

			while (uWritten < dwLen1) {	

				// this will keep reading in until the buffer is full. For very short files.
				WaveStartDataRead(&streamData->wiWave.hmmio, &streamData->wiWave.mmck, &streamData->wiWave.mmckInRIFF);
				WaveReadFile(streamData->wiWave.hmmio, (uint32)dwLen1-uWritten, lpWrite1 + uWritten, &streamData->wiWave.mmck, &uActualBytesWritten);
				uWritten += uActualBytesWritten;
			}
		}
	}

	// now unlock the buffer.
	
	lpDSStreamBuff(looping)->Unlock((LPVOID)lpWrite1, dwLen1, nullptr, 0);
                                                                        
	streamData->wiWave.dwNextWriteOffset += dwLen1;	
	// this is a circular buffer. Do mod buffersize.
	if (streamData->wiWave.dwNextWriteOffset >= streamData->wiWave.dwBufferSize) streamData->wiWave.dwNextWriteOffset -= streamData->wiWave.dwBufferSize; 

	streamData->wiWave.dwProgress = 0;
	streamData->wiWave.dwLastPos = 0;

	return true;
}

// <LegoRR.exe @0047c070>
void __cdecl Gods98::Sound3D_Stream_CheckPosition(bool32 looping)
{
	log_firstcall();

	Sound3D_StreamData* streamData;
	HRESULT hr = DS_OK;
	//DWORD hRet = 0; // (unused)
	DWORD dwPlayPos, dwWritePos, dwPlayed;
	bool32 sameLoop;

	if (looping) streamData = &sound3DGlobs.loopStreamData;
	else streamData = &sound3DGlobs.streamData;

	if (streamData->playing && streamData->fileOpen) {

		lpDSStreamBuff(looping)->GetCurrentPosition(&dwPlayPos, &dwWritePos);

		if (dwPlayPos < streamData->wiWave.dwLastPos) dwPlayed = streamData->wiWave.dwBufferSize - streamData->wiWave.dwLastPos + dwPlayPos;
		else dwPlayed = dwPlayPos - streamData->wiWave.dwLastPos;
		
		streamData->wiWave.dwProgress += dwPlayed;
		streamData->wiWave.dwLastPos = dwPlayPos;	
		sameLoop = false;

		while (streamData->wiWave.dwProgress > streamData->wiWave.dwNextProgressCheck) {

			// A play notification has been received.
			LPBYTE lpWrite1;
			DWORD dwWrite1;
			uint32 cbActual = 0;
			
			//ON NEXT NOTIFICATION, IF DONE PLAYING IS SET STOP THE SOUND
			if (streamData->wiWave.bDonePlaying && !sameLoop) {
				Sound3D_Stream_Stop(looping);
				break;
			}

			sameLoop = true;

			// If the entire file has been read into the buffer, bFoundEnd will be TRUE.
			if (!streamData->wiWave.bFoundEnd) {	
				// read in more of the file.
				
				Error_Debug( Error_Format("Next write pos =	%i\n", streamData->wiWave.dwNextWriteOffset) );
				Error_Debug( Error_Format("Write size =	%i\n\n", streamData->wiWave.dwNotifySize) );

				// Lock the buffer.
				if (DS_OK != lpDSStreamBuff(looping)->Lock(streamData->wiWave.dwNextWriteOffset, streamData->wiWave.dwNotifySize, (void**)&lpWrite1, &dwWrite1, NULL, NULL, 0)) {
					Error_Warn(true, "Cannot lock stream buffer." );
					return;
				}
				
				// now read the file. 
				WaveReadFile(streamData->wiWave.hmmio, dwWrite1, lpWrite1, &streamData->wiWave.mmck, &cbActual);

				//	if cbActual is less than the amount asked for, we may have
				//	reached end of file. If we are looping, we will simply read the 
				//	file in again until we have written dwWrite1 bytes into the buffer.
				//	If not looping we fill with silence.
				if ((DWORD)cbActual < dwWrite1) {
					if (!streamData->wiWave.bLoopFile) {

						streamData->wiWave.bFoundEnd = true;
						// fill up the rest of the buffer with silence.
						/// BEWARE: FillMemory swaps the last two arguments when compared to memset.
						//::FillMemory(lpWrite1 + cbActual, (uint32)dwWrite1 - cbActual, (uint8)(streamData->wiWave.pwfx->wBitsPerSample == 8 ? 128 : 0));
						std::memset(lpWrite1 + cbActual, (uint8)(streamData->wiWave.pwfx->wBitsPerSample == 8 ? 128 : 0), (uint32)dwWrite1 - cbActual);

					} else {
						
						// we're looping. Read in the file again.
						sint32 nChkErr = 0;
						uint32 cbWritten = cbActual;	// from previous call above.
						
						while (cbWritten < (uint32)dwWrite1) {	// write only one notification worth.

							// this will keep reading in until the buffer is full. For very short files.
							WaveStartDataRead( &streamData->wiWave.hmmio, &streamData->wiWave.mmck, &streamData->wiWave.mmckInRIFF );
							WaveReadFile( streamData->wiWave.hmmio, (uint32)dwWrite1-cbWritten,
													lpWrite1 + cbWritten, &streamData->wiWave.mmck, &cbActual );
							cbWritten += cbActual;
						}
					}
				}

				//unlock now
				lpDSStreamBuff(looping)->Unlock((LPVOID)lpWrite1, dwWrite1,  nullptr, 0);
				// setup the nextwrite offset.
				streamData->wiWave.dwNextWriteOffset += dwWrite1;
				//check += dwWrite1;
				streamData->wiWave.dwNextProgressCheck += dwWrite1;
				//if (check >= streamData->wiWave.dwBufferSize)
				//	lastCheckWrap = true;
				if (streamData->wiWave.dwNextWriteOffset >= streamData->wiWave.dwBufferSize) streamData->wiWave.dwNextWriteOffset -= streamData->wiWave.dwBufferSize;

			} else {

				// We have read in the whole file. We will keep filling in silence
				// in the buffer to allow the remaining sound bytes filled earlier 
				// to be played. 
				
				// Allow the rest of the bytes to be played and fill here
				// with silence. The next notification will quit the while loop.
				lpDSStreamBuff(looping)->Lock(streamData->wiWave.dwNextWriteOffset, streamData->wiWave.dwNotifySize, (void**)&lpWrite1, &dwWrite1, nullptr, nullptr, 0 );
				/// BEWARE: FillMemory swaps the last two arguments when compared to memset.
				//::FillMemory( lpWrite1, dwWrite1, (uint8)(streamData->wiWave.pwfx->wBitsPerSample == 8 ? 128 : 0) );
				std::memset( lpWrite1, (uint8)(streamData->wiWave.pwfx->wBitsPerSample == 8 ? 128 : 0), dwWrite1 );

				hr = lpDSStreamBuff(looping)->Unlock((LPVOID)lpWrite1, dwWrite1,  nullptr, 0);
				
				// We don't want to cut off the sound before it's done playing.
				//ON NEXT NOTIFICATION, IF DONE PLAYING IS SET STOP THE SOUND
				if (streamData->wiWave.mmckInRIFF.cksize > streamData->wiWave.dwNotifySize &&
					streamData->wiWave.dwProgress >= streamData->wiWave.mmckInRIFF.cksize - streamData->wiWave.dwNotifySize) {

					streamData->wiWave.bDonePlaying = true;

				} else if (streamData->wiWave.dwProgress >= streamData->wiWave.mmckInRIFF.cksize) {
					// for short files.
					streamData->wiWave.bDonePlaying = true;
				}
				
				streamData->wiWave.dwNextProgressCheck += dwWrite1;
			}
		}
	}
}

// <LegoRR.exe @0047c380>
bool32 __cdecl Gods98::Sound3D_D3DVectorEqual(const Vector3F* a, const Vector3F* b)
{
	log_firstcall();

	if( (a->x == b->x) &&
		(a->y == b->y) &&
		(a->z == b->z) )
		return true;

	return false;
}
//bool32 __cdecl Gods98::Sound3D_D3DVectorEqual(const D3DVECTOR* a, const D3DVECTOR* b);

// <LegoRR.exe @0047c3c0>
real32 __cdecl Gods98::Sound3D_SetRollOffFactor(real32 rollOff)
{
	log_firstcall();

	if (Sound3D_Initialised()) {

		if( rollOff < DS3D_MINROLLOFFFACTOR )
			rollOff = DS3D_MINROLLOFFFACTOR;

		if( rollOff > DS3D_MAXROLLOFFFACTOR )
			rollOff = DS3D_MAXROLLOFFFACTOR;
		
		lp3DListenerInfo()->SetRolloffFactor( rollOff, DS3D_IMMEDIATE );
	}

	return rollOff;
}

// <LegoRR.exe @0047c420>
sint32 __cdecl Gods98::Sound3D_MinVolume(void)
{
	log_firstcall();

	static_assert(DSBVOLUME_MIN == -10000, "DSBVOLUME_MIN does not match that of LegoRR");
	return DSBVOLUME_MIN;
}

// return 0;
// <shared>
sint32 __cdecl Gods98::Sound3D_MaxVolume(void)
{
	log_firstcall();

	static_assert(DSBVOLUME_MAX == 0, "DSBVOLUME_MAX does not match that of LegoRR");
	return DSBVOLUME_MAX;
}

#pragma endregion
