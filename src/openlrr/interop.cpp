
#include "hook.h"
#include "interop.h"
#include "OpenLRR.h"

#include "engine/audio/3DSound.h"
#include "engine/gfx/Activities.h"
#include "engine/video/Animation.h"
#include "engine/gfx/AnimClone.h"
#include "engine/drawing/Bmp.h"
#include "engine/util/Compress.h"
#include "engine/core/Config.h"
#include "engine/gfx/Containers.h"
#include "engine/drawing/DirectDraw.h"
#include "engine/drawing/Draw.h"
#include "engine/util/Dxbug.h"
#include "engine/core/Errors.h"
#include "engine/core/Files.h"
#include "engine/drawing/Flic.h"
#include "engine/drawing/Fonts.h"
#include "engine/drawing/Images.h"
#include "engine/input/Input.h"
#include "engine/input/Keys.h"
#include "engine/gfx/Lws.h"
#include "engine/gfx/Lwt.h"
#include "engine/Graphics.h"
#include "engine/Main.h"
#include "engine/gfx/Materials.h"
#include "engine/core/Maths.h"
#include "engine/core/Memory.h"
#include "engine/gfx/Mesh.h"
#include "engine/video/Movie.h"
#include "engine/util/Registry.h"
#include "engine/audio/Sound.h"
#include "engine/drawing/TextWindow.h"
#include "engine/core/Utils.h"
#include "engine/gfx/Viewports.h"
#include "engine/core/Wad.h"
#include "engine/Init.h"

#include "game/audio/SFX.h"
#include "game/effects/DamageText.h"
#include "game/effects/Effects.h"
#include "game/effects/LightEffects.h"
#include "game/effects/Smoke.h"
#include "game/front/Credits.h"
#include "game/front/FrontEnd.h"
#include "game/front/Loader.h"
#include "game/front/Reward.h"
#include "game/interface/hud/Bubbles.h"
#include "game/interface/hud/ObjInfo.h"
#include "game/interface/Advisor.h"
#include "game/interface/Encyclopedia.h"
#include "game/interface/InfoMessages.h"
#include "game/interface/Interface.h"
#include "game/interface/Panels.h"
#include "game/interface/Pointers.h"
#include "game/interface/Priorities.h"
#include "game/interface/RadarMap.h"
#include "game/interface/TextMessages.h"
#include "game/interface/ToolTip.h"
#include "game/mission/Messages.h"
#include "game/mission/NERPsFile.h"
#include "game/mission/NERPsFunctions.h"
#include "game/mission/Objective.h"
#include "game/mission/PTL.h"
#include "game/object/AITask.h"
#include "game/object/BezierCurve.h"
#include "game/object/Building.h"
#include "game/object/Collision.h"
#include "game/object/Creature.h"
#include "game/object/Dependencies.h"
#include "game/object/MeshLOD.h"
#include "game/object/Object.h"
#include "game/object/ObjectRecall.h"
#include "game/object/Stats.h"
#include "game/object/Vehicle.h"
#include "game/object/Weapons.h"
#include "game/world/Camera.h"
#include "game/world/Construction.h"
#include "game/world/Detail.h"
#include "game/world/ElectricFence.h"
#include "game/world/Erosion.h"
#include "game/world/Fallin.h"
#include "game/world/Roof.h"
#include "game/world/SelectPlace.h"
#include "game/world/Water.h"
#include "game/Game.h"


#define return_interop(result) { std::printf("%s %s\n", __FUNCTION__, (result?"OK":"failed")); } return result;


#pragma region DLL Import Hooks

// NOTE: Unthunked calls to DLL functions use an absolute pointer to a function pointer.
//       So we need to write the absolute *address of an address* to the function we're replacing.

// Stdcall wrapper around `Main_GetTime`.
static uint32 __stdcall _wrapTimeGetTime() { return Gods98::Main_GetTime(); }
// Hooks need to point to this instead of the actual function.
static decltype(_wrapTimeGetTime)* _ptrTimeGetTime = _wrapTimeGetTime;
// Hooks need to point to this instead of the actual function.
static decltype(_ptrTimeGetTime)* _importTimeGetTime = &_ptrTimeGetTime;

bool interop_hook_calls_WINMM_timeGetTime(void)
{
	bool result = true;

	// currently hooked, don't replace these
	// calls in: Credits_Play:
	//result &= hook_write_addr(0x0040a004 + 2, &_importTimeGetTime);
	//result &= hook_write_addr(0x0040a190 + 2, &_importTimeGetTime);
	//result &= hook_write_addr(0x0040a1da + 2, &_importTimeGetTime);
	//result &= hook_write_addr(0x0040a1e4 + 2, &_importTimeGetTime);


	// calls in: Front_ScreenMenuLoop:
	result &= hook_write_addr(0x00413b1f + 2, &_importTimeGetTime);
	result &= hook_write_addr(0x00413bb9 + 2, &_importTimeGetTime);

	// currently hooked, don't replace these
	// calls in: Front_PlayIntroSplash:
	//result &= hook_write_addr(0x00415777 + 2, &_importTimeGetTime);


	// currently hooked, don't replace these
	// calls in: Main_WinMain:
	//result &= hook_write_addr(0x00477d3a + 2, &_importTimeGetTime);
	
	// timeGetTime is also called by Main_GetTime.
	// But we can't be hooking the call that we're replacing the call with.


	// currently hooked, don't replace these
	// calls in: G98CAnimation___ctor:
	//result &= hook_write_addr(0x0047eb87 + 2, &_importTimeGetTime);

	// calls in: G98CAnimation__Update:
	//result &= hook_write_addr(0x0047ee23 + 2, &_importTimeGetTime);

	// calls in: G98CAnimation__SetTime:
	//result &= hook_write_addr(0x0047eec9 + 2, &_importTimeGetTime);


	// currently hooked, don't replace these
	// calls in: Sound_Update:
	//result &= hook_write_addr(0x00488eca + 2, &_importTimeGetTime);

	return_interop(result);
}

#pragma endregion


#pragma region C Runtime Hooks

bool interop_hook_CRT_rand(void)
{
	bool result = true;
	
	// Hook random calls, so that we can have full control over I/O.
	//result &= hook_write_jmpret(0x0048e420, legacy::srand);
	//result &= hook_write_jmpret(0x0048e430, legacy::rand);
	result &= hook_write_jmpret(0x0048e420, Gods98::Maths_SeedRand);
	result &= hook_write_jmpret(0x0048e430, Gods98::Maths_RandInt32);

	return_interop(result);
}

#pragma endregion


#pragma region Gods98 Engine Hooks

bool interop_hook_WinMain(void)
{
	bool result = hook_write_jmpret(PROCESS_WINMAIN, StartOpenLRRInjected);
	return_interop(result);
}

bool interop_hook_Gods98_3DSound(void)
{
	bool result = true;
	// used by: Sound_Initialise
	result &= hook_write_jmpret(0x0047a900, Gods98::Sound3D_Initialise);

	// used by: Lego_Shutdown_Full, Lego_HandleKeys
	result &= hook_write_jmpret(0x0047aac0, Gods98::Sound3D_ShutDown);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047ab10, Gods98::Sound3D_CheckVolumeLimits);

	// used by: NERPsFile_LoadMessageFile, SFX_LoadSampleProperty
	result &= hook_write_jmpret(0x0047ab30, Gods98::Sound3D_Load);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047ad90, Gods98::Sound3D_GetFreeSoundIndex);

	// used by: NERPsFile_Free
	result &= hook_write_jmpret(0x0047adc0, Gods98::Sound3D_Remove);

	// used by: Lego_Initialise, Lego_SetViewMode
	result &= hook_write_jmpret(0x0047ae40, Gods98::Sound3D_MakeListener);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0047ae60, Gods98::Sound3D_UpdateListener);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047ae80, Gods98::Sound3D_ListenerCallback);
	//result &= hook_write_jmpret(0x0047afd0, Gods98::Sound3D_SetWorldPos);
	//result &= hook_write_jmpret(0x0047aff0, Gods98::Sound3D_CheckAlreadyExists);

	// used by: NERPs.c, SFX.c
	result &= hook_write_jmpret(0x0047b030, Gods98::Sound3D_Play2);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047b2e0, Gods98::Sound3D_AddSoundRecord);

	// used by: SFX_Random_SetBufferVolume
	result &= hook_write_jmpret(0x0047b310, Gods98::Sound3D_SetBufferVolume);
	// used by: SFX_Random_GetBufferVolume
	result &= hook_write_jmpret(0x0047b390, Gods98::Sound3D_GetBufferVolume);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047b3b0, Gods98::Sound3D_GetSoundBuffer);

	// used by: Lws_HandleTrigger, SFX_Sound3D_StopSound
	result &= hook_write_jmpret(0x0047b3f0, Gods98::Sound3D_StopSound);

	// used by: Game_SetPaused, Level_Free, Lego_GoBackToMissionSelect,
	//           SFX_SetSoundOn
	result &= hook_write_jmpret(0x0047b420, Gods98::Sound3D_StopAllSounds);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047b460, Gods98::Sound3D_AttachSound);
	//result &= hook_write_jmpret(0x0047b4e0, Gods98::Sound3D_RemoveSound);
	//result &= hook_write_jmpret(0x0047b560, Gods98::Sound3D_RemoveSoundRecord);
	//result &= hook_write_jmpret(0x0047b5a0, Gods98::Sound3D_RecurseRemoveSoundRecord);
	//result &= hook_write_jmpret(0x0047b5f0, Gods98::Sound3D_RecurseRemoveSound);

	// used by: Container_Clone, SFX_Sound3D_Update
	result &= hook_write_jmpret(0x0047b650, Gods98::Sound3D_Update);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047b6d0, Gods98::Sound3D_SoundCallback);

	// used by: Lego_Initialise, Lego_SetViewMode
	result &= hook_write_jmpret(0x0047b760, Gods98::Sound3D_SetMinDistForAtten);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x0047b790, Gods98::Sound3D_SetMaxDist);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047b7c0, Gods98::Sound3D_SetGlobalVolume);

	// used by: PausedMenu_SliderSoundVolume
	result &= hook_write_jmpret(0x0047b7f0, Gods98::Sound3D_SetGlobalVolumePrescaled);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047b810, Gods98::Sound3D_SetVolumeToDefault);
	//result &= hook_write_jmpret(0x0047b840, Gods98::Sound3D_LoadSample);
	//result &= hook_write_jmpret(0x0047b980, Gods98::Sound3D_CreateSoundBuffer);
	//result &= hook_write_jmpret(0x0047ba50, Gods98::Sound3D_SendSoundToBuffer);

	// used by: NERPs_Level_NERPMessage_Parse, SFX_Random_GetSamplePlayTime
	result &= hook_write_jmpret(0x0047bba0, Gods98::Sound3D_GetSamplePlayTime);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047bc30, Gods98::Sound3D_Stream_Play);

	result &= hook_write_jmpret(0x0047bce0, Gods98::Sound3D_Stream_Stop);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047bd60, Gods98::Sound3D_Stream_BufferSetup);
	//result &= hook_write_jmpret(0x0047bef0, Gods98::Sound3D_Stream_FillDataBuffer);
	//result &= hook_write_jmpret(0x0047c070, Gods98::Sound3D_Stream_CheckPosition);
	//result &= hook_write_jmpret(0x0047c380, Gods98::Sound3D_D3DVectorEqual);
	
	// used by: Lego_Initialise, Lego_MainLoop
	result &= hook_write_jmpret(0x0047c3c0, Gods98::Sound3D_SetRollOffFactor);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0047c420, Gods98::Sound3D_MinVolume);

	return_interop(result);
}

bool interop_hook_Gods98_Animation(void)
{
	bool result = true;
	// Only the C wrapper API's need to be hooked

	// used by: WinMain
	result &= hook_write_jmpret(0x0047ef40, Gods98::Animation_Initialise);

	// used by: Credits_Play
	result &= hook_write_jmpret(0x0047ef50, Gods98::Animation_Load);
	result &= hook_write_jmpret(0x0047efb0, Gods98::Animation_Update);
	result &= hook_write_jmpret(0x0047efc0, Gods98::Animation_BlitToBackBuffer);
	result &= hook_write_jmpret(0x0047f000, Gods98::Animation_SetTime);
	result &= hook_write_jmpret(0x0047f010, Gods98::Animation_GetTime);
	result &= hook_write_jmpret(0x0047f020, Gods98::Animation_GetLength);
	result &= hook_write_jmpret(0x0047f030, Gods98::Animation_Free);
	result &= hook_write_jmpret(0x0047f040, Gods98::Animation_IsOk);

	return_interop(result);
}

bool interop_hook_calls_Gods98_AnimClone(void)
{
	bool result = true;

	// (shared) "AnimClone_IsLws__Flic_GetWidth"
	// THIS FUNCTION MUST BE HOOKED ON AN INDIVIDUAL BASIS
	// There are 5 calls made to this:
	//  type:Flic (Flic_GetWidth)  -> FUN_004120e0  <@004120f7>
	//                                      Panel_Draw  <@0045ab17>
	//                                      Pointer_DrawPointer  <@0045cfc8>
	//  type:FlocksData (Flocks_???)     -> LegoObject_Flocks_FUN_0044bef0  <@0044bfc3>
	//  type:AnimClone (AnimClone_IsLws) -> Container_FormatPartName  <@00473f60>
	// <called @00473f60>
	result &= hook_write_call(0x00473f60, Gods98::AnimClone_IsLws);

	return result;
}

bool interop_hook_Gods98_AnimClone(void)
{
	bool result = true;

	// used by: Container_LoadAnimSet
	result &= hook_write_jmpret(0x004897e0, Gods98::AnimClone_Register);
	result &= hook_write_jmpret(0x00489880, Gods98::AnimClone_RegisterLws);
	// used by: Container_Clone, Container_AddActivity2
	result &= hook_write_jmpret(0x00489920, Gods98::AnimClone_Make);
	// used by: Container_Remove2
	result &= hook_write_jmpret(0x00489a10, Gods98::AnimClone_Remove);


	// merged call: Gods98::AnimClone_IsLws must be hooked individually for each call.
	// moved to: interop_hook_calls_Gods98_AnimClone


	// used by: Container_SetAnimationTime, Container_ForceAnimationUpdate
	result &= hook_write_jmpret(0x00489aa0, Gods98::AnimClone_SetTime);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00489ba0, Gods98::AnimClone_FrameCountCallback);
	//result &= hook_write_jmpret(0x00489bb0, Gods98::AnimClone_SetupFrameArrayCallback);
	//result &= hook_write_jmpret(0x00489bd0, Gods98::AnimClone_WalkTree);
	//result &= hook_write_jmpret(0x00489cb0, Gods98::AnimClone_CreateCopy);
	//result &= hook_write_jmpret(0x00489df0, Gods98::AnimClone_CloneLwsMesh);
	//result &= hook_write_jmpret(0x00489e80, Gods98::AnimClone_ReferenceVisuals);

	return result;
}

bool interop_hook_Gods98_Bmp(void)
{
	bool result = true;
	// used by: Container_LoadTextureSurface, Image_LoadBMPScaled
	result &= hook_write_jmpret(0x00489ef0, Gods98::BMP_Parse);
	result &= hook_write_jmpret(0x0048a030, Gods98::BMP_Cleanup);
	return_interop(result);
}

bool interop_hook_Gods98_Compress(void)
{
	bool result = true;

	// this and one subfunction are implemented, but beyond that, everything is calling native code
	result &= hook_write_jmpret(0x0049ca80, Gods98::RNC_Uncompress);

	return_interop(result);
}

bool interop_hook_Gods98_Config(void)
{
	bool result = true;
	result &= hook_write_jmpret(0x004790b0, Gods98::Config_Initialise);
	result &= hook_write_jmpret(0x004790e0, Gods98::Config_Shutdown);

	result &= hook_write_jmpret(0x00479120, Gods98::Config_Load);
	result &= hook_write_jmpret(0x00479210, Gods98::Config_BuildStringID);
	result &= hook_write_jmpret(0x004792b0, Gods98::Config_FindArray);
	result &= hook_write_jmpret(0x004792e0, Gods98::Config_GetNextItem);
	result &= hook_write_jmpret(0x00479310, Gods98::Config_GetStringValue);
	result &= hook_write_jmpret(0x00479370, Gods98::Config_GetTempStringValue);

	result &= hook_write_jmpret(0x00479390, Gods98::Config_GetBoolValue);
	result &= hook_write_jmpret(0x004793d0, Gods98::Config_GetAngle);
	result &= hook_write_jmpret(0x00479430, Gods98::Config_GetRGBValue);
	result &= hook_write_jmpret(0x00479500, Gods98::Config_Free);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00479530, Gods98::Config_Create);
	//result &= hook_write_jmpret(0x00479580, Gods98::Config_Remove);

	result &= hook_write_jmpret(0x004795a0, Gods98::Config_FindItem);
	
	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00479750, Gods98::Config_AddList);
	return_interop(result);
}

bool interop_hook_Gods98_Containers(void)
{
	bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x004729d0, Gods98::Container_Initialise);
	// used by: Lego_Initialise, Lego_Shutdown_Full
	result &= hook_write_jmpret(0x00472ac0, Gods98::Container_Shutdown);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00472b80, Gods98::Container_SetSharedTextureDirectory);
	// used by: LiveObject_Create
	result &= hook_write_jmpret(0x00472ba0, Gods98::Container_EnableSoundTriggers);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00472bc0, Gods98::Container_SetTriggerFrameCallback);
	// used by: SFX_Initialise
	result &= hook_write_jmpret(0x00472be0, Gods98::Container_SetSoundTriggerCallback);
	// used by: Mesh_CreateOnFrame, Lego_LoadLighting,
	//           LiveObject_Flocks_Initialise, Smoke_CreateSmokeArea
	result &= hook_write_jmpret(0x00472c00, Gods98::Container_GetRoot);

	result &= hook_write_jmpret(0x00472c10, Gods98::Container_Create);
	result &= hook_write_jmpret(0x00472d00, Gods98::Container_Remove);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00472d10, Gods98::Container_Remove2);

	result &= hook_write_jmpret(0x00472f90, Gods98::Container_Load);

	// used by: Vehicle_SetActivity_AndRemoveCarryCameraFrames
	result &= hook_write_jmpret(0x00473600, Gods98::Container_IsCurrentActivity);

	result &= hook_write_jmpret(0x00473630, Gods98::Container_SetActivity);

	result &= hook_write_jmpret(0x00473720, Gods98::Container_Light_SetSpotPenumbra);
	result &= hook_write_jmpret(0x00473740, Gods98::Container_Light_SetSpotUmbra);

	// used by: Lego_LoadLighting
	result &= hook_write_jmpret(0x00473760, Gods98::Container_Light_SetSpotRange);
	// used by: Advisor_Update
	result &= hook_write_jmpret(0x00473780, Gods98::Container_Light_SetEnableContainer);

	// used by: Advisor_Initialise, Menu_LoadMenus, Lego_LoadLighting
	result &= hook_write_jmpret(0x004737b0, Gods98::Container_MakeLight);

	// used by: Map3D_LoadSurfaceMap, Roof_Initialise, SelectPlace_Create,
	//           Water_InitVertices
	result &= hook_write_jmpret(0x00473820, Gods98::Container_MakeMesh2);

	// used by: SFX_Random_PlaySound3DOnContainer
	result &= hook_write_jmpret(0x00473940, Gods98::Container_GetMasterFrame);

	result &= hook_write_jmpret(0x00473950, Gods98::Container_Clone);

	// used by: LiveObject_Hide2
	result &= hook_write_jmpret(0x00473de0, Gods98::Container_Hide2);

	result &= hook_write_jmpret(0x00473e00, Gods98::Container_Hide);

	// used by: Object_IsHidden, LiveObject_IsHidden,
	//           LegoObject_Flocks_FUN_0044bef0
	result &= hook_write_jmpret(0x00473e60, Gods98::Container_IsHidden);

	result &= hook_write_jmpret(0x00473e80, Gods98::Container_SearchTree);
	result &= hook_write_jmpret(0x00473f20, Gods98::Container_FormatPartName);

	// used by: Object_SetOwnerObject, LiveObject_Create, Vehicle_SetOwnerObject
	result &= hook_write_jmpret(0x00474060, Gods98::Container_SetUserData);
	// used by: Game_Container_TriggerFrameCallback
	result &= hook_write_jmpret(0x00474070, Gods98::Container_GetUserData);
	// used by: Game_UpdateSceneFog
	result &= hook_write_jmpret(0x00474080, Gods98::Container_EnableFog);
	// used by: Game_UpdateSceneFog, Lego_LoadLevel
	result &= hook_write_jmpret(0x004740d0, Gods98::Container_SetFogColour);
	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x00474130, Gods98::Container_SetFogMode);
	result &= hook_write_jmpret(0x00474160, Gods98::Container_SetFogParams);
	// used by: Vehicle_Duplicate
	result &= hook_write_jmpret(0x00474180, Gods98::Container_SetPerspectiveCorrection);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00474230, Gods98::Container_SetPerspectiveCorrectionCallback);

	// used by: Mesh_LoadTexture
	result &= hook_write_jmpret(0x00474310, Gods98::Container_LoadTextureSurface);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004746d0, Gods98::Container_GetDecalColour);

	// used by: DynamicPM_LoadTextureBaseName, Roof_SetTexture
	result &= hook_write_jmpret(0x004747b0, Gods98::Container_LoadTexture2);
	// used by: DynamicPM_Free_SurfaceTextureGrid, Roof_SetTexture,
	//           Roof_Shutdown
	result &= hook_write_jmpret(0x004749d0, Gods98::Container_FreeTexture);
	// used by: MeshPoly_Container_SwapFrame (FUN_00451e80)
	result &= hook_write_jmpret(0x00474a20, Gods98::Container_Mesh_Swap);
	// used by: Map3D_LoadSurfaceMap, Map3D_FadeInBlock, Roof_LowerBlockRoofVertices,
	//           SelectPlace_Create, Water_InitVertices
	result &= hook_write_jmpret(0x00474bb0, Gods98::Container_Mesh_AddGroup);

	result &= hook_write_jmpret(0x00474ce0, Gods98::Container_Mesh_GetGroupCount);

	// used by: Map3D_LoadSurfaceMap, SelectPlace_Create, Vehicle_LoadActivityFile
	result &= hook_write_jmpret(0x00474d20, Gods98::Container_Mesh_SetQuality);
	// used by: Map3D_IsBlockMeshHidden
	result &= hook_write_jmpret(0x00474da0, Gods98::Container_Mesh_IsGroupHidden);

	result &= hook_write_jmpret(0x00474df0, Gods98::Container_Mesh_HideGroup);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00474ec0, Gods98::Container_Mesh_HandleSeperateMeshGroups);

	// used by: DynamicPM_LoadPromesh_AB
	result &= hook_write_jmpret(0x00474f00, Gods98::Container_Mesh_GetGroup);

	result &= hook_write_jmpret(0x00474f80, Gods98::Container_Mesh_GetVertices);
	result &= hook_write_jmpret(0x00474ff0, Gods98::Container_Mesh_SetVertices);
	result &= hook_write_jmpret(0x00475060, Gods98::Container_Mesh_SetTexture);

	// used by: DynamicPM_LoadPromesh_AB, Map3D_SetPerspectiveCorrectionAll,
	//           Roof_LowerBlockRoofVertices
	result &= hook_write_jmpret(0x004750f0, Gods98::Container_Mesh_SetPerspectiveCorrection);
	// used by: DynamicPM_LoadPromesh_AB
	result &= hook_write_jmpret(0x00475150, Gods98::Container_Mesh_Scale);
	// used by: Vehicle_LoadActivityFile
	result &= hook_write_jmpret(0x004751d0, Gods98::Container_Mesh_GetBox);

	// used by: LiveObject_SetCrystalPoweredColor, Map3D_Coords_SetEmissive,
	//           SelectPlace_CheckAndUpdate
	result &= hook_write_jmpret(0x004752b0, Gods98::Container_Mesh_SetEmissive);

	result &= hook_write_jmpret(0x004752e0, Gods98::Container_Mesh_SetColourAlpha);
	result &= hook_write_jmpret(0x00475330, Gods98::Container_Transform);

	// used by: DynamicPM_Sub2_FUN_0040bac0
	result &= hook_write_jmpret(0x00475350, Gods98::Container_InverseTransform);

	result &= hook_write_jmpret(0x00475370, Gods98::Container_SetColourAlpha);
	result &= hook_write_jmpret(0x004753e0, Gods98::Container_MoveAnimation);
	result &= hook_write_jmpret(0x00475400, Gods98::Container_SetAnimationTime);

	// used by: Game_LiveObjectLargeCallback
	result &= hook_write_jmpret(0x004755c0, Gods98::Container_ForceAnimationUpdate);

	result &= hook_write_jmpret(0x00475650, Gods98::Container_GetAnimationTime);
	result &= hook_write_jmpret(0x004756b0, Gods98::Container_GetAnimationFrames);
	result &= hook_write_jmpret(0x004756f0, Gods98::Container_SetPosition);
	result &= hook_write_jmpret(0x00475730, Gods98::Container_SetOrientation);
	result &= hook_write_jmpret(0x00475780, Gods98::Container_GetPosition);
	result &= hook_write_jmpret(0x004757c0, Gods98::Container_GetOrientation);
	result &= hook_write_jmpret(0x00475840, Gods98::Container_AddRotation);

	// used by: DynamicPM_Sub1_FUN_0040b930, LiveObject_TeleportUp
	result &= hook_write_jmpret(0x00475870, Gods98::Container_AddScale);

	result &= hook_write_jmpret(0x004758a0, Gods98::Container_AddTranslation);

	// used by: Building_SetUpgradeActivity, Vehicle_SetUpgradeActivity
	result &= hook_write_jmpret(0x004758d0, Gods98::Container_ClearTransform);
	// used by: DynamicPM_Sub2_FUN_0040bac0, FlocksMatrix_FUN_0044ba60
	result &= hook_write_jmpret(0x00475970, Gods98::Container_AddTransform);

	// used by: Creature_CheckThrowNull
	result &= hook_write_jmpret(0x00475990, Gods98::Container_GetZXRatio);
	result &= hook_write_jmpret(0x004759d0, Gods98::Container_SetParent);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00475a60, Gods98::Container_GetParent);

	// used by: Creature_GetTransCoef, Vehicle_GetTransCoef
	result &= hook_write_jmpret(0x00475ab0, Gods98::Container_GetTransCoef);

	// used by: Mesh_RenderCallback
	result &= hook_write_jmpret(0x00475af0, Gods98::Container_SearchOwner);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00475b40, Gods98::Container_Frame_GetContainer);
	//result &= hook_write_jmpret(0x00475bc0, Gods98::Container_GetFrames);
	//result &= hook_write_jmpret(0x00475bf0, Gods98::Container_ParseTypeString);
	//result &= hook_write_jmpret(0x00475cb0, Gods98::Container_AddList);
	//result &= hook_write_jmpret(0x00475d30, Gods98::Container_GetActivities);
	//result &= hook_write_jmpret(0x00475ec0, Gods98::Container_SetTypeData);
	//result &= hook_write_jmpret(0x00475f40, Gods98::Container_FreeTypeData);
	//result &= hook_write_jmpret(0x00475fd0, Gods98::Container_AddActivity2);
	//result &= hook_write_jmpret(0x004760d0, Gods98::Container_Frame_ReferenceDestroyCallback);
	//result &= hook_write_jmpret(0x00476100, Gods98::Container_Frame_Find);

	// used by: Sound3D_AttachSound
	result &= hook_write_jmpret(0x00476230, Gods98::Container_Frame_SetAppData);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004763a0, Gods98::Container_Frame_RemoveAppData);
	//result &= hook_write_jmpret(0x004763e0, Gods98::Container_Frame_GetOwner);
	//result &= hook_write_jmpret(0x00476400, Gods98::Container_Frame_GetAnimSetFileName);
	//result &= hook_write_jmpret(0x00476420, Gods98::Container_Frame_GetAnimClone);
	//result &= hook_write_jmpret(0x00476440, Gods98::Container_Frame_GetFrameCount);
	//result &= hook_write_jmpret(0x00476460, Gods98::Container_Frame_GetCurrTime);
	//result &= hook_write_jmpret(0x00476480, Gods98::Container_Frame_GetTransCo);
	//result &= hook_write_jmpret(0x004764a0, Gods98::Container_Frame_GetSample);
	//result &= hook_write_jmpret(0x004764c0, Gods98::Container_Frame_GetTrigger);
	//result &= hook_write_jmpret(0x004764e0, Gods98::Container_Frame_SafeAddChild);
	//result &= hook_write_jmpret(0x00476530, Gods98::Container_Frame_FormatName);
	//result &= hook_write_jmpret(0x004765b0, Gods98::Container_Frame_FreeName);
	//result &= hook_write_jmpret(0x004765d0, Gods98::Container_Frame_GetName);
	//result &= hook_write_jmpret(0x004765f0, Gods98::Container_Frame_WalkTree);
	//result &= hook_write_jmpret(0x004766d0, Gods98::Container_Frame_SearchCallback);
	//result &= hook_write_jmpret(0x00476880, Gods98::Container_LoadAnimSet);
	//result &= hook_write_jmpret(0x00476a30, Gods98::Container_GetAnimFileFrameCount);
	//result &= hook_write_jmpret(0x00476aa0, Gods98::Container_FrameLoad);
	//result &= hook_write_jmpret(0x00476b10, Gods98::Container_MeshLoad);
	//result &= hook_write_jmpret(0x00476bc0, Gods98::Container_TextureLoadCallback);
	//result &= hook_write_jmpret(0x00476eb0, Gods98::Container_YFlipTexture);
	//result &= hook_write_jmpret(0x00476fa0, Gods98::Container_TextureSetSort);
	//result &= hook_write_jmpret(0x00476fd0, Gods98::Container_TextureDestroyCallback);

	return result;
}

bool interop_hook_Gods98_Dxbug(void)
{
	bool result = true;

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048a050, Gods98::SE);

	// used by: Viewport_Render, Main_SetupDirect3D, Image_GetScreenshot,
	//           Input_InitKeysAndDI
	result &= hook_write_jmpret(0x0048a090, Gods98::Error_SetDXError);
	return_interop(result);
}

bool interop_hook_Gods98_DirectDraw(void)
{
	bool result = true;

	// used by: Movie_Load
	result &= hook_write_jmpret(0x00406500, Gods98::noinline(DirectDraw));
	// used by: Movie_Load, Animation_BlitToBackBuffer
	result &= hook_write_jmpret(0x00406510, Gods98::noinline(DirectDraw_bSurf));

	// used by: WinMain
	result &= hook_write_jmpret(0x0047c430, Gods98::DirectDraw_Initialise);
	// used by: Init_Initialise
	result &= hook_write_jmpret(0x0047c480, Gods98::DirectDraw_EnumDrivers);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047c4b0, Gods98::DirectDraw_EnumDriverCallback);

	// used by: Init_Initialise, Init_SetDeviceList
	result &= hook_write_jmpret(0x0047c5a0, Gods98::DirectDraw_EnumDevices);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047c640, Gods98::DirectDraw_EnumDeviceCallback);

	// used by: Init_Initialise, Init_SetModeList
	result &= hook_write_jmpret(0x0047c770, Gods98::DirectDraw_EnumModes);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047c810, Gods98::DirectDraw_EnumModeCallback);

	// used by: Init_Initialise
	result &= hook_write_jmpret(0x0047c8d0, Gods98::DirectDraw_Setup);

	// used by: WinMain, Main_LoopUpdate, Loader_display_shutdown, NERPFunc__SetMessage
	result &= hook_write_jmpret(0x0047cb90, Gods98::DirectDraw_Flip);

	// used by: Image_SaveBMP
	result &= hook_write_jmpret(0x0047cbb0, Gods98::DirectDraw_SaveBMP);

	// used by: Lego_PlayMovie_old
	result &= hook_write_jmpret(0x0047cee0, Gods98::DirectDraw_ReturnFrontBuffer);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047cf10, Gods98::DirectDraw_BlitBuffers);

	// used by: WinMain
	result &= hook_write_jmpret(0x0047cfb0, Gods98::DirectDraw_Shutdown);

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x0047d010, Gods98::DirectDraw_AdjustTextureUsage);
	result &= hook_write_jmpret(0x0047d090, Gods98::DirectDraw_GetAvailTextureMem);

	// used by: Main_LoopUpdate, Lego_Shutdown_Full, RadarMap_Draw
	result &= hook_write_jmpret(0x0047d0e0, Gods98::DirectDraw_Clear);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047d1a0, Gods98::DirectDraw_CreateClipper);

	// used by: Container_LoadTextureSurface
	result &= hook_write_jmpret(0x0047d2c0, Gods98::DirectDraw_Blt8ToSurface);
	result &= hook_write_jmpret(0x0047d590, Gods98::DirectDraw_GetColour);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047d6b0, Gods98::DirectDraw_CountMaskBits);

	return_interop(result);
}

bool interop_hook_Gods98_Draw(void)
{
	bool result = true;

	// used by: WinMain
	result &= hook_write_jmpret(0x00486140, Gods98::Draw_Initialise);

	result &= hook_write_jmpret(0x00486160, Gods98::Draw_SetClipWindow);
	result &= hook_write_jmpret(0x00486270, Gods98::Draw_GetClipWindow);
	result &= hook_write_jmpret(0x004862b0, Gods98::Draw_LineListEx);

	// used by: ObjInfo_DrawHealthBar, ToolTip_DrawBox
	result &= hook_write_jmpret(0x00486350, Gods98::Draw_RectListEx);

	// used by: RadarMap_Draw
	result &= hook_write_jmpret(0x004864d0, Gods98::Draw_RectList2Ex);
	// used by: RadarMap_DrawSurveyDotCircle
	result &= hook_write_jmpret(0x00486650, Gods98::Draw_DotCircle);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00486790, Gods98::Draw_GetColour);
	//result &= hook_write_jmpret(0x00486810, Gods98::Draw_LockSurface);
	//result &= hook_write_jmpret(0x00486910, Gods98::Draw_UnlockSurface);
	//result &= hook_write_jmpret(0x00486950, Gods98::Draw_SetDrawPixelFunc);
	//result &= hook_write_jmpret(0x004869e0, Gods98::Draw_LineActual);
	//result &= hook_write_jmpret(0x00486b40, Gods98::Draw_Pixel8);
	//result &= hook_write_jmpret(0x00486b60, Gods98::Draw_Pixel16);
	//result &= hook_write_jmpret(0x00486b90, Gods98::Draw_Pixel16XOR);
	//result &= hook_write_jmpret(0x00486bc0, Gods98::Draw_Pixel16HalfTrans);
	//result &= hook_write_jmpret(0x00486c60, Gods98::Draw_Pixel24);
	//result &= hook_write_jmpret(0x00486c90, Gods98::Draw_Pixel32);
	return_interop(result);
}

bool interop_hook_Gods98_Errors(void)
{
	bool result = true;
	// used by: WinMain
	//result &= hook_write_jmpret(0x0048b520, Gods98::Error_Initialise);
	// used by: Main_SetupDisplay
	//result &= hook_write_jmpret(0x0048b540, Gods98::Error_FullScreen);
	// used by: WinMain
	//result &= hook_write_jmpret(0x0048b550, Gods98::Error_CloseLog);
	//result &= hook_write_jmpret(0x0048b5b0, Gods98::Error_Shutdown);
	return_interop(result);
}

bool interop_hook_Gods98_Files(void)
{
	bool result = true;
	result &= hook_write_jmpret(0x0047f3f0, Gods98::File_Initialise);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047f7b0, Gods98::File_FindDataCD);
	//result &= hook_write_jmpret(0x0047f850, Gods98::File_SetBaseSearchPath);

	// used by: Wad_Load
	//result &= hook_write_jmpret(0x0047f8c0, Gods98::File_Error);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047f900, Gods98::File_LoadWad);

	// used by: Movie_Load, Sound3D_Load, Sound3D_Play2
	//result &= hook_write_jmpret(0x0047f920, Gods98::File_GetCDFilePath);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047f960, Gods98::File_MakeDir);

	result &= hook_write_jmpret(0x0047f9a0, Gods98::File_Open);
	result &= hook_write_jmpret(0x0047fb10, Gods98::File_Seek);
	result &= hook_write_jmpret(0x0047fc40, Gods98::File_Read);
	result &= hook_write_jmpret(0x0047fd10, Gods98::File_Write);
	result &= hook_write_jmpret(0x0047fd80, Gods98::File_Close);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047fdd0, Gods98::File_Tell);

	result &= hook_write_jmpret(0x0047fe20, Gods98::File_Exists);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047fee0, Gods98::File_GetC);

	// used by: ObjectRecall_LoadRROSFile
	result &= hook_write_jmpret(0x0047ff60, Gods98::File_Length);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047ffa0, Gods98::File_InternalFGetS);

	// used by: Encyclopedia_Update, Loader_Initialise, Objective_Level_LoadObjectiveText
	result &= hook_write_jmpret(0x00480000, Gods98::File_GetS);

	// used by: Loader_Shutdown
	result &= hook_write_jmpret(0x00480070, Gods98::File_PrintF);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004800e0, Gods98::_File_GetSystem);
	//result &= hook_write_jmpret(0x004800f0, Gods98::_File_CheckSystem);
	//result &= hook_write_jmpret(0x00480160, Gods98::_File_OpenWad);
	//result &= hook_write_jmpret(0x00480190, Gods98::_File_Alloc);
	//result &= hook_write_jmpret(0x004801f0, Gods98::_File_Malloc);
	//result &= hook_write_jmpret(0x00480200, Gods98::_File_Free);
	//result &= hook_write_jmpret(0x00480210, Gods98::_File_Dealloc);
	//result &= hook_write_jmpret(0x00480280, Gods98::_File_GetWadName);

	// used by: Mesh_UViewMesh, Lws_Parse
	result &= hook_write_jmpret(0x00480310, Gods98::File_GetLine);

	result &= hook_write_jmpret(0x00480360, Gods98::File_LoadBinary);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00480380, Gods98::File_Load);

	// used by: Lego_Load*Map (9 functions)
	result &= hook_write_jmpret(0x00480430, Gods98::File_LoadBinaryHandle);

	// used by: Movie_Load, Container_LoadTexture2, Sound3D_Load, Sound3D_Play2, G98CAnimation___ctor
	result &= hook_write_jmpret(0x004804e0, Gods98::File_VerifyFilename);

	// used by: Loader_Initialise
	result &= hook_write_jmpret(0x00480570, Gods98::File_SetLoadCallback);

	// used by: Error_Shutdown (unreachable?)
	//result &= hook_write_jmpret(0x00480590, Gods98::File_CheckRedundantFiles);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00480650, Gods98::File_CheckDirectory);
	//result &= hook_write_jmpret(0x00480830, Gods98::File_CheckFile);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00484f50, Gods98::File_ErrorFile);
	return_interop(result);
}

bool interop_hook_calls_Gods98_Flic(void)
{
	bool result = true;

	// (shared) "AnimClone_IsLws__Flic_GetWidth"
	// THIS FUNCTION MUST BE HOOKED ON AN INDIVIDUAL BASIS
	// There are 5 calls made to this:
	//  type:Flic (Flic_GetWidth)  -> FUN_004120e0  <@004120f7>
	//                                      Panel_Draw  <@0045ab17>
	//                                      Pointer_DrawPointer  <@0045cfc8>
	//  type:FlocksData (Flocks_???)     -> LegoObject_Flocks_FUN_0044bef0  <@0044bfc3>
	//  type:AnimClone (AnimClone_IsLws) -> Container_FormatPartName  <@00473f60>
	// <called @004120f7, 0045ab17, 0045cfc8>
	result &= hook_write_call(0x004120f7, Gods98::Flic_GetWidth);
	result &= hook_write_call(0x0045ab17, Gods98::Flic_GetWidth);
	result &= hook_write_call(0x0045cfc8, Gods98::Flic_GetWidth);


	// This is the only scenario where the game properly handles framerate for flics,
	// So trying to apply Flic_AnimateMainDeltaTime would cause main menu animations to play incredibly slow.
	// used in: Front_Menu_UpdateOverlays
	result &= hook_write_call(0x004121d1, Gods98::Flic_Animate);

	return_interop(result);
}

bool interop_hook_Gods98_Flic(void)
{
	bool result = true;

	result &= hook_write_jmpret(0x00483f40, Gods98::Flic_Setup);
	result &= hook_write_jmpret(0x004841c0, Gods98::Flic_Close);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00484220, Gods98::Flic_LoadHeader);

	// used by: Front_Menu_UpdateOverlays, Panel_Draw, Pointer_DrawPointer, Reward_DrawItem
	result &= hook_write_jmpret(0x00484330, Gods98::Flic_AnimateMainDeltaTime);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00484490, Gods98::Flic_Memory);
	//result &= hook_write_jmpret(0x00484520, Gods98::Flic_Load);
	//result &= hook_write_jmpret(0x004845e0, Gods98::Flic_FindChunk);
	//result &= hook_write_jmpret(0x00484770, Gods98::Flic_FrameChunk);
	//result &= hook_write_jmpret(0x004848d0, Gods98::Flic_DoChunk);
	//result &= hook_write_jmpret(0x004849e0, Gods98::Flic_LoadPointers);
	//result &= hook_write_jmpret(0x00484a90, Gods98::Flic_LoadPalette64);
	//result &= hook_write_jmpret(0x00484b40, Gods98::Flic_Copy);
	//result &= hook_write_jmpret(0x00484b90, Gods98::FlicBRunDepackHiColor);
	//result &= hook_write_jmpret(0x00484c90, Gods98::FlicBRunDepackHiColorFlic32k);
	//result &= hook_write_jmpret(0x00484de0, Gods98::Flic_BrunDepack);
	//result &= hook_write_jmpret(0x00484e60, Gods98::FlicCreateHiColorTable);
	//result &= hook_write_jmpret(0x00484ec0, Gods98::Flic_Palette256);
	//result &= hook_write_jmpret(0x00484f60, Gods98::FlicDeltaWordHiColor);
	//result &= hook_write_jmpret(0x00485110, Gods98::FlicDeltaWordHiColorFlic32k);
	//result &= hook_write_jmpret(0x004852f0, Gods98::Flic_DeltaWord);
	//result &= hook_write_jmpret(0x00485380, Gods98::getFlicCol);


	// merged call: Gods98::Flic_GetWidth must be hooked individually for each call.
	// moved to: interop_hook_calls_Gods98_Flic


	result &= hook_write_jmpret(0x004853a0, Gods98::Flic_GetHeight);

	return_interop(result);
}

bool interop_hook_Gods98_Fonts(void)
{
	bool result = true;

	result &= hook_write_jmpret(0x00401b90, Gods98::noinline(Font_GetStringWidth));

	// used by: Loader_Initialise, ToolTip_SetContent
	result &= hook_write_jmpret(0x00401bc0, Gods98::noinline(Font_GetStringInfo));

	result &= hook_write_jmpret(0x0047a1a0, Gods98::Font_Load);

	// used by: Panel_PrintF
	result &= hook_write_jmpret(0x0047a410, Gods98::Font_VGetStringInfo);

	result &= hook_write_jmpret(0x0047a440, Gods98::Font_PrintF);

	// used by: Panel_PrintF
	result &= hook_write_jmpret(0x0047a470, Gods98::Font_VPrintF);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047a4a0, Gods98::Font_VPrintF2);

	// used by: TextWindow_Update, TextWindow_UpdateOverlay
	result &= hook_write_jmpret(0x0047a730, Gods98::Font_OutputChar);

	// used by: TextWindow_Update, TextWindow_UpdateOverlay, Lego_RenameInputUnk
	result &= hook_write_jmpret(0x0047a7e0, Gods98::Font_GetCharWidth);

	result &= hook_write_jmpret(0x0047a800, Gods98::Font_GetHeight);

	// used by: Lego_Shutdown_Full, Reward_CleanupBaseFont
	result &= hook_write_jmpret(0x0047a810, Gods98::Font_Remove);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047a840, Gods98::Font_Create);
	//result &= hook_write_jmpret(0x0047a880, Gods98::Font_AddList);

	return_interop(result);
}

bool interop_hook_Gods98_Images(void)
{
	bool result = true;

	// used by: Lego_Initialise (new Gods98 calls in WinMain)
	result &= hook_write_jmpret(0x0047d6d0, Gods98::Image_Initialise);

	// used by: Lego_Shutdown_Full
	result &= hook_write_jmpret(0x0047d6f0, Gods98::Image_Shutdown);

	result &= hook_write_jmpret(0x0047d730, Gods98::Image_Remove);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047d750, Gods98::Image_CopyToDataToSurface);
	//result &= hook_write_jmpret(0x0047d7e0, Gods98::Image_8BitSourceCopy);
	//result &= hook_write_jmpret(0x0047d9c0, Gods98::Image_CountMaskBits);
	//result &= hook_write_jmpret(0x0047d9e0, Gods98::Image_CountMaskBitShift);
	//result &= hook_write_jmpret(0x0047da00, Gods98::Image_FlipSurface);
	//result &= hook_write_jmpret(0x0047dac0, Gods98::Image_24BitSourceCopy);

	result &= hook_write_jmpret(0x0047dc90, Gods98::Image_LoadBMPScaled);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047de50, Gods98::Image_RGB2CR);

	result &= hook_write_jmpret(0x0047de80, Gods98::Image_SetPenZeroTrans);
	result &= hook_write_jmpret(0x0047deb0, Gods98::Image_SetupTrans);
	result &= hook_write_jmpret(0x0047df70, Gods98::Image_DisplayScaled);

	// used by: Font_Load
	//result &= hook_write_jmpret(0x0047e120, Gods98::Image_LockSurface);
	//result &= hook_write_jmpret(0x0047e190, Gods98::Image_UnlockSurface);
	//result &= hook_write_jmpret(0x0047e1b0, Gods98::Image_GetPen255BigEndian);
	//result &= hook_write_jmpret(0x0047e210, Gods98::Image_GetPixelMaskBigEndian);

	// REPLACEMENT FOR: Image_GetPixel, because the functions that use GetPixel are checking specifically for black (0).
	result &= hook_write_jmpret(0x0047e260, Gods98::Image_GetPixelTruncate);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047e310, Gods98::Image_Create);
	//result &= hook_write_jmpret(0x0047e380, Gods98::Image_AddList);
	//result &= hook_write_jmpret(0x0047e3f0, Gods98::Image_RemoveAll);
	//result &= hook_write_jmpret(0x0047e450, Gods98::Image_DDColorMatch);
	//result &= hook_write_jmpret(0x0047e590, Gods98::Image_CR2RGB);

	// used by: Objective_SetCompleteStatus
	result &= hook_write_jmpret(0x0047e5c0, Gods98::Image_GetScreenshot);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047e6a0, Gods98::Image_InitFromSurface);

	// used by: Front_ScreenSubmenuLoop
	result &= hook_write_jmpret(0x0047e700, Gods98::Image_SaveBMP);

	return_interop(result);
}

bool interop_hook_Gods98_Input(void)
{
	bool result = true;
	// used by: LiveObject_FUN_00471fe0
	result &= hook_write_jmpret(0x00410a60, Gods98::noinline(msx));
	result &= hook_write_jmpret(0x00410a70, Gods98::noinline(msy));
	result &= hook_write_jmpret(0x00410a80, Gods98::noinline(mslb));

	// used by: WinMain
	//result &= hook_write_jmpret(0x0047f050, Gods98::Input_InitKeysAndDI);

	// used by: WinMain, Main_LoopUpdate, NERPFunc__SetMessage
	result &= hook_write_jmpret(0x0047f1b0, Gods98::Input_ReadKeys);

	// used by: Credits_Play, Front_PlayMovie, Front_PlayIntroSplash, NERPFunc__GetAnyKeyPressed
	result &= hook_write_jmpret(0x0047f270, Gods98::Input_AnyKeyPressed);

	// used by: WinMain
	//result &= hook_write_jmpret(0x0047f290, Gods98::Input_ReleaseKeysAndDI);
	// used by: WinMain, Main_LoopUpdate
	//result &= hook_write_jmpret(0x0047f2d0, Gods98::Input_ReadMouse2);

	// used by: Objective_DoHotkeyChecks, Panel_RotationControl_HandleRotation,
	//           Priorities_MoveCursorToPriorityUpButton, Priorities_MoveCursorToPriorityImage
	result &= hook_write_jmpret(0x0047f390, Gods98::Input_SetCursorPos);
	return_interop(result);
}

bool interop_hook_Gods98_Keys(void)
{
	bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00485ce0, Gods98::Keys_Initialise);

	// used by: Interface_LoadInterfaceBuildImages (FUN_0041aa30)
	//           Interface_LoadInterfaceImages (FUN_0041acd0)
	result &= hook_write_jmpret(0x004860f0, Gods98::Key_Find);
	return_interop(result);
}

bool interop_hook_Gods98_Lws(void)
{
	bool result = true;
	// used by: Container_LoadAnimSet
	result &= hook_write_jmpret(0x00486cb0, Gods98::Lws_Parse);

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00487980, Gods98::Lws_Initialise);
	// used by: Lego_Shutdown_Full
	result &= hook_write_jmpret(0x00487a20, Gods98::Lws_Shutdown);

	// used by: Container_LoadAnimSet
	result &= hook_write_jmpret(0x00487a90, Gods98::Lws_GetFrameCount);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00487aa0, Gods98::Lws_SetupSoundTriggers);

	// used by: Container_LoadAnimSet
	result &= hook_write_jmpret(0x00487c50, Gods98::Lws_LoadMeshes);
	// used by: AnimClone_Make
	result &= hook_write_jmpret(0x00487cc0, Gods98::Lws_Clone);
	// used by: Container_LoadAnimSet, AnimClone_SetTime
	result &= hook_write_jmpret(0x00487e60, Gods98::Lws_SetTime);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00487f70, Gods98::Lws_FindPrevKey);
	//result &= hook_write_jmpret(0x00488010, Gods98::Lws_AnimateTextures);
	//result &= hook_write_jmpret(0x004880a0, Gods98::Lws_HandleTrigger);
	//result &= hook_write_jmpret(0x00488190, Gods98::Lws_KeyPassed);
	//result &= hook_write_jmpret(0x00488280, Gods98::Lws_FindPrevDissolve);
	//result &= hook_write_jmpret(0x00488330, Gods98::Lws_InterpolateDissolve);
	//result &= hook_write_jmpret(0x00488390, Gods98::Lws_SetDissolveLevel);
	//result &= hook_write_jmpret(0x00488430, Gods98::Lws_InterpolateKeys);
	//result &= hook_write_jmpret(0x004885a0, Gods98::Lws_SetupNodeTransform);
	//result &= hook_write_jmpret(0x00488880, Gods98::Lws_LoadMesh);
	//result &= hook_write_jmpret(0x004889f0, Gods98::Lws_SearchMeshPathList);
	//result &= hook_write_jmpret(0x00488a50, Gods98::Lws_AddMeshPathEntry);
	//result &= hook_write_jmpret(0x00488a80, Gods98::Lws_CreateFrames);
	//result &= hook_write_jmpret(0x00488bc0, Gods98::Lws_LoadNodes);
	//result &= hook_write_jmpret(0x00488c60, Gods98::Lws_SetAbsoluteKey);

	// used by: AnimClone_Remove
	result &= hook_write_jmpret(0x00488c90, Gods98::Lws_Free);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00488d30, Gods98::Lws_FreeNode);
	return result;
}

bool interop_hook_Gods98_Lwt(void)
{
	bool result = true;
	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048c300, Gods98::lwExtractString);
	//result &= hook_write_jmpret(0x0048c380, Gods98::stringAlloc);
	//result &= hook_write_jmpret(0x0048c3e0, Gods98::texMapType);
	//result &= hook_write_jmpret(0x0048c440, Gods98::surfFree);
	//result &= hook_write_jmpret(0x0048c490, Gods98::LWD3D);
	//result &= hook_write_jmpret(0x0048c4d0, Gods98::PNTSprc);
	//result &= hook_write_jmpret(0x0048c620, Gods98::CRVSprc);
	//result &= hook_write_jmpret(0x0048c6a0, Gods98::POLSprc);
	//result &= hook_write_jmpret(0x0048c950, Gods98::SRFSprc);
	//result &= hook_write_jmpret(0x0048cae0, Gods98::SURFprc);
	//result &= hook_write_jmpret(0x0048d580, Gods98::LoadLWOB);

	// used by: Mesh_Load
	result &= hook_write_jmpret(0x0048da80, Gods98::LoadAppObj);

	// used by: Mesh_Load   ... is memory properly being elsewhere freed!????
	result &= hook_write_jmpret(0x0048db30, Gods98::FreeLWOB);
	return result;
}

bool interop_hook_Gods98_Main(void)
{
	bool result = true;

	// used by: Lego_Initialise, Info_Send
	result &= hook_write_jmpret(0x00401b30, Gods98::noinline(Main_ProgrammerMode));
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00401b40, Gods98::noinline(Main_GetStartLevel));
	
	// used by: Lego_Initialise, LiveObject_DrawSelectedBox,
	//           RadarMap_Initialise, RadarMap_Draw
	result &= hook_write_jmpret(0x00401b70, Gods98::noinline(appWidth));
	result &= hook_write_jmpret(0x00401b80, Gods98::noinline(appHeight));

	// used by: entry (we're currently hooking this one in a more direct fashion, so ignore this.)
	//result &= hook_write_jmpret(0x00477a60, Gods98::Main_WinMain);

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00477e90, Gods98::Graphics_DisableTextureManagement);

	// used by: WinMain
	//result &= hook_write_jmpret(0x00477eb0, Gods98::Main_ParseCommandLine);

	result &= hook_write_jmpret(0x004781f0, Gods98::Main_LoopUpdate);

	// used by: Level_BlockUpdateSurface
	result &= hook_write_jmpret(0x00478230, Gods98::noinline(Main_GetCLFlags));

	// used by: DirectDraw_EnumModeCallback, Init_SetModeList
	//result &= hook_write_jmpret(0x00478240, Gods98::Graphics_GetWindowsBitDepth);

	// used by: WinMain, Main_LoopUpdate, Lego_MainLoop
	result &= hook_write_jmpret(0x00478260, Gods98::Graphics_Finalise3D);

	// used by: Gods_Go
	result &= hook_write_jmpret(0x00478290, Gods98::Main_SetState);

	result &= hook_write_jmpret(0x004782c0, Gods98::Main_GetTime);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004782d0, Gods98::Main_DispatchMessage);
	// used by: WinMain, Main_LoopUpdate
	//result &= hook_write_jmpret(0x00478300, Gods98::Main_HandleIO);

	// used by: DirectDraw_Setup
	result &= hook_write_jmpret(0x00478370, Gods98::Main_SetupDisplay);
	result &= hook_write_jmpret(0x00478490, Gods98::Graphics_SetupDirect3D);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004785d0, Gods98::Main_AdjustWindowRect);

	// used by: Lego_LoadGraphicsSettings
	result &= hook_write_jmpret(0x004785f0, Gods98::Graphics_Setup3D);

	// used by: Gods_Go
	result &= hook_write_jmpret(0x00478690, Gods98::Main_SetTitle);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004786b0, Gods98::Main_InitApp);
	//result &= hook_write_jmpret(0x00478780, Gods98::Main_WndProc_Fullscreen);
	//result &= hook_write_jmpret(0x00478980, Gods98::Main_WndProc_Windowed);

	// used by: Main_InitApp (*)
	//result &= hook_write_jmpret(0x00478b40, Gods98::Main_WndProc);

	// used by: Mesh_SetMeshRenderDesc, Mesh_SetRenderDesc, Mesh_SetAlphaRender,
	//           Mesh_SetGroupRenderDesc, Mesh_RenderTriangleList
	result &= hook_write_jmpret(0x00478b90, Gods98::Graphics_ChangeRenderState);

	// used by: Mesh_RenderCallback, Mesh_PostRenderAll
	result &= hook_write_jmpret(0x00478c00, Gods98::Graphics_RestoreStates);

	// used by: PausedMenu_SliderMusicVolume
	result &= hook_write_jmpret(0x00478c40, Gods98::Main_SetCDVolume);
	// used by: PausedMenu_CalcSliderCDVolume
	result &= hook_write_jmpret(0x00478c60, Gods98::Main_GetCDVolume);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00478c80, Gods98::Main_CDVolume);

	return_interop(result);
}

bool interop_hook_Gods98_Materials(void)
{
	bool result = true;
	// used by: Map3D_LoadSurfaceMap
	result &= hook_write_jmpret(0x00489780, Gods98::Material_Create);
	return_interop(result);
}

bool interop_hook_Gods98_Maths(void)
{
	bool result = true;
	// used by: LiveObject_DrawSelectedBox
	result &= hook_write_jmpret(0x00401240, Gods98::noinline(Maths_Vector2DDistance));
	result &= hook_write_jmpret(0x004013e0, Gods98::noinline(Maths_Vector3DCrossProduct));
	result &= hook_write_jmpret(0x00401470, Gods98::noinline(Maths_Vector3DAdd));
	result &= hook_write_jmpret(0x00401630, Gods98::noinline(Maths_Vector3DSubtract));

	// used by: LiveObject_DrawSelectedBox, Vehicle_SetPosition
	result &= hook_write_jmpret(0x00401660, Gods98::noinline(Maths_Vector3DScale));

	// used by: LiveObject_DrawSelectedBox
	result &= hook_write_jmpret(0x00401690, Gods98::noinline(Maths_Vector3DNormalize));

	result &= hook_write_jmpret(0x004797c0, Gods98::Maths_Vector3DRandom);
	result &= hook_write_jmpret(0x004797d0, Gods98::Maths_Vector3DRotate);
	result &= hook_write_jmpret(0x004797f0, Gods98::Maths_PlaneNormal);

	// used by: DynamicPM_FUN_0040b3a0
	result &= hook_write_jmpret(0x004798f0, Gods98::Maths_TriangleAreaZ);

	result &= hook_write_jmpret(0x00479b60, Gods98::Maths_Rand);
	result &= hook_write_jmpret(0x00479b70, Gods98::Maths_RandRange);
	result &= hook_write_jmpret(0x00479ba0, Gods98::Maths_RayPlaneIntersection);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00479cf0, Gods98::Maths_RayPlaneDistance);
	//result &= hook_write_jmpret(0x00479d70, Gods98::Maths_RayEndPoint);

	// used by: Map3D_FUN_0044fe50, LiveObject_CollisionBox_FUN_00470570
	result &= hook_write_jmpret(0x00479db0, Gods98::Maths_Vector2DIntersection);

	result &= hook_write_jmpret(0x00479e40, Gods98::Maths_PointInsidePoly);

	// used by: LiveObject_Callback_FUN_0043b670
	result &= hook_write_jmpret(0x00479ed0, Gods98::Maths_RaySphereIntersection);

	// used by: Flocks.c
	result &= hook_write_jmpret(0x00479fa0, Gods98::Matrix_Mult);
	result &= hook_write_jmpret(0x0047a010, Gods98::Matrix_RotX);
	result &= hook_write_jmpret(0x0047a060, Gods98::Matrix_RotY);
	result &= hook_write_jmpret(0x0047a0b0, Gods98::Matrix_RotZ);
	result &= hook_write_jmpret(0x0047a100, Gods98::Matrix_Translate);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047a130, Gods98::Matrix_Identity);
	//result &= hook_write_jmpret(0x0047a160, Gods98::Matrix_Zero);

	// used by: Flocks.c
	result &= hook_write_jmpret(0x0047a170, Gods98::Matrix_Copy);

	return_interop(result);
}

bool interop_hook_Gods98_Memory(void)
{
	bool result = true;
	// used by: WinMain
	result &= hook_write_jmpret(0x004896b0, Gods98::Mem_Initialise);

	// used by: File_LoadBinaryHandle
	//result &= hook_write_jmpret(0x004896d0, Gods98::Mem_AllocHandle);

	// used by: Lego_Load*Map (9 functions)
	result &= hook_write_jmpret(0x00489720, Gods98::Mem_FreeHandle);

	// used by: MapShared_GetBlock, MapShared_GetDimensions,
	//           File_LoadBinaryHandle
	result &= hook_write_jmpret(0x00489760, Gods98::Mem_AddressHandle);
	return_interop(result);
}

bool interop_hook_Gods98_Mesh(void)
{
	bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00480870, Gods98::Mesh_Initialise);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00480910, Gods98::Mesh_CreateGlobalMaterial);
	//result &= hook_write_jmpret(0x00480a40, Gods98::Mesh_SetMaterial);
	//result &= hook_write_jmpret(0x00480a60, Gods98::Mesh_ObtainFromList);
	//result &= hook_write_jmpret(0x00480a90, Gods98::Mesh_ReturnToList);
	//result &= hook_write_jmpret(0x00480ab0, Gods98::Mesh_AddList);
	
	// used by: Container_MakeMesh2, DamageFont.c, Smoke.c, Weapons.c
	result &= hook_write_jmpret(0x00480b30, Gods98::Mesh_CreateOnFrame);
	// used by: Container_Clone, Lws_Clone, Lws_LoadMesh, AnimClone_CloneLwsMesh
	result &= hook_write_jmpret(0x00480bc0, Gods98::Mesh_Clone);
	// used by: Container_Load, Lws_LoadMesh
	result &= hook_write_jmpret(0x00480ca0, Gods98::Mesh_Load);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00480d80, Gods98::Mesh_ParseLWO);
	//result &= hook_write_jmpret(0x00481ae0, Gods98::Mesh_GetSurfInfo);
	//result &= hook_write_jmpret(0x00481d80, Gods98::Mesh_GetTextureSeqInfo);
	//result &= hook_write_jmpret(0x00481e40, Gods98::Mesh_GetNextInSequence);
	//result &= hook_write_jmpret(0x00481f10, Gods98::Mesh_UViewMesh);
	//result &= hook_write_jmpret(0x00482260, Gods98::Mesh_GetTextureUVsWrap);

	// used by: Lws_AnimateTextures
	result &= hook_write_jmpret(0x00482300, Gods98::Mesh_SetTextureTime2);

	// used by: Container_Remove2, Container_FreeTypeData, Lws_FreeNode,
	//           Smoke.c, Weapons.c
	result &= hook_write_jmpret(0x00482390, Gods98::Mesh_Remove);

	// used by: Container_Mesh_GetGroup
	result &= hook_write_jmpret(0x00482460, Gods98::Mesh_GetGroup);

	// used by: Container_Mesh_GetGroupCount, Lws_SetDissolveLevel
	result &= hook_write_jmpret(0x004824d0, Gods98::Mesh_GetGroupCount);

	// used by: Container_Mesh_AddGroup, DamageFont.c, Smoke.c
	//           Struct34_FUN_00470a20
	result &= hook_write_jmpret(0x004824e0, Gods98::Mesh_AddGroup);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00482610, Gods98::Mesh_AlterGroupRenderFlags);

	// used by: Container_Mesh_Scale
	result &= hook_write_jmpret(0x00482630, Gods98::Mesh_Scale);
	// used by: Container_Mesh_SetVertices
	result &= hook_write_jmpret(0x004826a0, Gods98::Mesh_SetVertices);
	// used by: Container_Mesh_GetVertices
	result &= hook_write_jmpret(0x00482730, Gods98::Mesh_GetVertices);

	// used by: DamageFont.c, Smoke.c
	result &= hook_write_jmpret(0x004827c0, Gods98::Mesh_SetVertices_PointNormalAt);
	// used by: Struct34_FUN_00470a20
	result &= hook_write_jmpret(0x004828e0, Gods98::Mesh_SetVertices_SameNormal);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00482980, Gods98::Mesh_SetVertices_VNT);

	// used by: Container_Mesh_IsGroupHidden
	result &= hook_write_jmpret(0x00482a40, Gods98::Mesh_IsGroupHidden);

	// used by: Container_Mesh_HideGroup, DamageFont.c, Smoke.c
	result &= hook_write_jmpret(0x00482a60, Gods98::Mesh_HideGroup);
	// used by: Smoke_Hide
	result &= hook_write_jmpret(0x00482a90, Gods98::Mesh_Hide);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00482ab0, Gods98::Mesh_RenderCallback);
	result &= hook_write_jmpret(0x00482d80, Gods98::Mesh_SetMeshRenderDesc);
	result &= hook_write_jmpret(0x00482e10, Gods98::Mesh_SetRenderDesc);
	result &= hook_write_jmpret(0x00482f70, Gods98::Mesh_SetAlphaRender);
	result &= hook_write_jmpret(0x00482fa0, Gods98::Mesh_AddToPostRenderList);
	result &= hook_write_jmpret(0x00482ff0, Gods98::Mesh_ClearPostRenderList);

	// use by: Viewport_Render
	result &= hook_write_jmpret(0x00483020, Gods98::Mesh_PostRenderAll);

	// used by: DamageFont.c, Smoke.c
	result &= hook_write_jmpret(0x00483130, Gods98::Mesh_LoadTexture);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004832f0, Gods98::Mesh_SearchTexturePathList);
	//result &= hook_write_jmpret(0x00483340, Gods98::Mesh_AddTexturePathEntry);

	// used by: Container_Mesh_SetTexture, DamageFont.c, Smoke.c
	result &= hook_write_jmpret(0x00483380, Gods98::Mesh_SetGroupTexture);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00483400, Gods98::Mesh_RemoveGroupTexture);
	//result &= hook_write_jmpret(0x00483430, Gods98::Mesh_CreateGroupMaterial);
	//result &= hook_write_jmpret(0x00483500, Gods98::Mesh_SetGroupMaterial);

	// used by: Container_Mesh_SetEmissive, Container_Mesh_SetColourAlpha
	//           DamageFont.c, Smoke.c, Struct34_FUN_00470a20
	result &= hook_write_jmpret(0x00483530, Gods98::Mesh_SetGroupColour);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004836c0, Gods98::Mesh_GetGroupMaterial);

	// used by: Container_Mesh_SetColourAlpha, Lws_SetDissolveLevel
	//           DamageFont.c, Smoke.c, Struct34_FUN_00470a20
	result &= hook_write_jmpret(0x004836e0, Gods98::Mesh_SetGroupMaterialValues);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00483800, Gods98::Mesh_SetIdentityMatrix);
	//result &= hook_write_jmpret(0x00483840, Gods98::Mesh_SetCurrentViewport);
	//result &= hook_write_jmpret(0x004838c0, Gods98::Mesh_SetCurrentGODSViewport);
	//result &= hook_write_jmpret(0x00483950, Gods98::Mesh_SetTransform);
	//result &= hook_write_jmpret(0x00483ad0, Gods98::Mesh_ChangeTextureStageState);
	//result &= hook_write_jmpret(0x00483b70, Gods98::Mesh_StoreTextureAndMat);
	//result &= hook_write_jmpret(0x00483c00, Gods98::Mesh_RestoreTextureAndMat);
	//result &= hook_write_jmpret(0x00483c80, Gods98::Mesh_RenderMesh);
	//result &= hook_write_jmpret(0x00483d30, Gods98::Mesh_CanRenderGroup);
	//result &= hook_write_jmpret(0x00483d50, Gods98::Mesh_RenderGroup);
	//result &= hook_write_jmpret(0x00483dc0, Gods98::Mesh_SetGroupRenderDesc);
	//result &= hook_write_jmpret(0x00483e30, Gods98::Mesh_RenderTriangleList);

	return result;
}

bool interop_hook_Gods98_Movie(void)
{
	bool result = true;
	// Only the C wrapper API's need to be hooked

	result &= hook_write_jmpret(0x00472820, Gods98::Movie_Load);

	// used by: Lego_PlayMovie_old
	result &= hook_write_jmpret(0x00472930, Gods98::Movie_GetSize);

	// used by: Front_PlayMovie
	result &= hook_write_jmpret(0x00472980, Gods98::Movie_GetDuration);
	// used by: Front_PlayMovie, Lego_PlayMovie_old
	result &= hook_write_jmpret(0x00472990, Gods98::Movie_Update);

	result &= hook_write_jmpret(0x004729b0, Gods98::Movie_Free);
	return_interop(result);
}

bool interop_hook_Gods98_Registry(void)
{
	bool result = true;
	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048b5f0, Gods98::Registry_GetKeyFromPath);

	// used by: WinMain, File_Initialise
	result &= hook_write_jmpret(0x0048b620, Gods98::Registry_GetValue);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048b650, Gods98::Registry_GetValue_Recursive);
	return_interop(result);
}

bool interop_hook_Gods98_Sound(void)
{
	bool result = true;

	// used by: WinMain
	result &= hook_write_jmpret(0x00488e10, Gods98::Sound_Initialise);

	// used by: Lego_Initialise, Lego_Shutdown_Full
	result &= hook_write_jmpret(0x00488e50, Gods98::Sound_IsInitialised);

	result &= hook_write_jmpret(0x00488e70, Gods98::Sound_PlayCDTrack);
	result &= hook_write_jmpret(0x00488eb0, Gods98::Sound_StopCD);

	result &= hook_write_jmpret(0x00488ec0, Gods98::Sound_Update);

	// used by: Sound3D_LoadSample
	result &= hook_write_jmpret(0x00488f30, Gods98::WaveOpenFile);
	// used by: Sound3D_Load
	result &= hook_write_jmpret(0x00489130, Gods98::GetWaveAvgBytesPerSec);
	// used by: Sound3D_Stream_BufferSetup
	result &= hook_write_jmpret(0x004891d0, Gods98::WaveOpenFile2);
	// used by: Sound3D_LoadSample, Sound3D_Stream_BufferSetup,
	//           Sound3D_Stream_FillDataBuffer, Sound3D_Stream_CheckPosition
	result &= hook_write_jmpret(0x00489380, Gods98::WaveStartDataRead);
	// used by: Sound3D_Stream_Stop,
	//           Sound3D_Stream_FillDataBuffer, Sound3D_Stream_CheckPosition
	result &= hook_write_jmpret(0x004893c0, Gods98::WaveReadFile);
	// used by: Sound3D_Stream_Stop, Sound3D_Stream_BufferSetup
	result &= hook_write_jmpret(0x00489490, Gods98::WaveCloseReadFile);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004894d0, Gods98::Restart_CDTrack);
	//result &= hook_write_jmpret(0x00489520, Gods98::ReportCDError);
	//result &= hook_write_jmpret(0x00489540, Gods98::Status_CDTrack);
	//result &= hook_write_jmpret(0x004895f0, Gods98::Play_CDTrack);
	//result &= hook_write_jmpret(0x00489660, Gods98::Stop_CDTrack);

	return_interop(result);
}

bool interop_hook_Gods98_TextWindow(void)
{
	bool result = true;

	result &= hook_write_jmpret(0x004853b0, Gods98::TextWindow_Create);
	result &= hook_write_jmpret(0x00485420, Gods98::TextWindow_EnableCentering);
	result &= hook_write_jmpret(0x00485450, Gods98::TextWindow_Remove);
	result &= hook_write_jmpret(0x00485470, Gods98::TextWindow_ChangePosition);
	result &= hook_write_jmpret(0x00485490, Gods98::TextWindow_ChangeSize);
	result &= hook_write_jmpret(0x004854d0, Gods98::TextWindow_PagePrintF);
	result &= hook_write_jmpret(0x004854f0, Gods98::TextWindow_PrintF);
	result &= hook_write_jmpret(0x00485510, Gods98::TextWindow_VPrintF);
	result &= hook_write_jmpret(0x004855c0, Gods98::TextWindow_PrintFOverlay);
	result &= hook_write_jmpret(0x00485650, Gods98::TextWindow_Update);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004859d0, Gods98::TextWindow_UpdateOverlay);

	result &= hook_write_jmpret(0x00485c70, Gods98::TextWindow_Clear);
	result &= hook_write_jmpret(0x00485cc0, Gods98::TextWindow_GetInfo);

	return_interop(result);
}

bool interop_hook_Gods98_Utils(void)
{
	bool result = true;
	result &= hook_write_jmpret(0x00477700, Gods98::Util_Tokenise);
	result &= hook_write_jmpret(0x00477770, Gods98::Util_WSTokenise);
	result &= hook_write_jmpret(0x00477810, Gods98::Util_StrCpy);
	result &= hook_write_jmpret(0x00477850, Gods98::Util_RemoveUnderscores);
	result &= hook_write_jmpret(0x004778d0, (char* (__cdecl*)(char*,const char*))Gods98::Util_StrIStr); // no const ambiguity
	result &= hook_write_jmpret(0x00477930, Gods98::Util_HashString);
	result &= hook_write_jmpret(0x004779d0, Gods98::Util_GetBoolFromString);
	return_interop(result);
}

bool interop_hook_Gods98_Viewports(void)
{
	bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00477010, Gods98::Viewport_Initialise);
	// used by: Lego_Shutdown_Full
	result &= hook_write_jmpret(0x00477040, Gods98::Viewport_Shutdown);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00477080, Gods98::Viewport_Create);
	result &= hook_write_jmpret(0x00477110, Gods98::Viewport_CreatePixel);

	// used by: Lego_FPHighPolyBlocks_FUN_00433db0
	result &= hook_write_jmpret(0x004771d0, Gods98::Viewport_GetSize);

	// used by: Lego_SetViewMode
	result &= hook_write_jmpret(0x00477210, Gods98::Viewport_SetCamera);

	// used by: DamageText_MeshRenderCallback, SaveMenu_FUN_00412b30
	//           Smoke_Group_MeshRenderCallback
	result &= hook_write_jmpret(0x00477230, Gods98::Viewport_GetCamera);
	// used by: Lego_Initialise, Lego_SetViewMode
	result &= hook_write_jmpret(0x00477270, Gods98::Viewport_SetBackClip);

	// used by: Advisor_InitViewport
	result &= hook_write_jmpret(0x00477290, Gods98::Viewport_GetBackClip);
	result &= hook_write_jmpret(0x004772b0, Gods98::Viewport_GetFrontClip);

	// used by: SaveMenu_FUN_00412b30, Lego_MainLoop
	result &= hook_write_jmpret(0x004772d0, Gods98::Viewport_Clear);
	result &= hook_write_jmpret(0x00477410, Gods98::Viewport_Render);

	// used by: Lego_Shutdown_Full
	result &= hook_write_jmpret(0x004774e0, Gods98::Viewport_Remove);
	// used by: Lego_SetViewMode
	result &= hook_write_jmpret(0x00477500, Gods98::Viewport_SmoothSetField);
	result &= hook_write_jmpret(0x00477510, Gods98::Viewport_SetField);

	// used by: Lego_FPHighPolyBlocks_FUN_00433db0
	result &= hook_write_jmpret(0x00477530, Gods98::Viewport_GetField);

	result &= hook_write_jmpret(0x00477550, Gods98::Viewport_InverseTransform);

	// used by: DynamicPM_FUN_0040b3a0, Lego_MainLoop
	//           LiveObject_CallbackDoSelection, Map3D_GetIntersections
	result &= hook_write_jmpret(0x00477570, Gods98::Viewport_Transform);

	result &= hook_write_jmpret(0x00477590, Gods98::Viewport_WorldToScreen);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004775d0, Gods98::Viewport_GetScene);
	//result &= hook_write_jmpret(0x00477630, Gods98::Viewport_AddList);
	//result &= hook_write_jmpret(0x004776a0, Gods98::Viewport_RemoveAll);

	return_interop(result);
}

bool interop_hook_Gods98_Wad(void)
{
	bool result = true;

	// DO NOT HOOK: used by: Files.c (shared function call with Files.c!File_Error)
	//result &= hook_write_jmpret(0x0047f8c0, Gods98::Wad_Error);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048b760, Gods98::GetFileName);

	// used by: File_LoadWad
	result &= hook_write_jmpret(0x0048b7a0, Gods98::Wad_Load);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048bfa0, Gods98::Wad_Get);
	//result &= hook_write_jmpret(0x0048bfb0, Gods98::Wad_FileLength);
	//result &= hook_write_jmpret(0x0048bfd0, Gods98::Wad_FileCompressedLength);
	//result &= hook_write_jmpret(0x0048bff0, Gods98::Wad_FindFreeFileHandle);

	// used by: File_Exists, _File_CheckSystem
	result &= hook_write_jmpret(0x0048c010, Gods98::Wad_IsFileInWad);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048c060, Gods98::_Wad_IsFileInWad);
	//result &= hook_write_jmpret(0x0048c0c0, Gods98::Wad_GetFreeWadSlot);
	//result &= hook_write_jmpret(0x0048c100, Gods98::_Wad_FileOpen);

	// used by: _File_OpenWad
	result &= hook_write_jmpret(0x0048c230, Gods98::Wad_FileOpen);

	// used by: _File_Dealloc
	result &= hook_write_jmpret(0x0048c280, Gods98::Wad_FileClose);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048c2b0, Gods98::Wad_FileGetPointer);

	// used by: File_Seek, File_Read, File_GetC
	result &= hook_write_jmpret(0x0048c2d0, Gods98::Wad_hLength);

	// used by: File_Read, File_GetC
	result &= hook_write_jmpret(0x0048c2f0, Gods98::Wad_hData);

	return_interop(result);
}

bool interop_hook_Gods98_Init(void)
{
	bool result = true;

	// used by: WinMain
	result &= hook_write_jmpret(0x0049d2f0, Gods98::Init_Initialise);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0049d5b0, Gods98::Init_DialogProc);
	//result &= hook_write_jmpret(0x0049d8a0, Gods98::Init_SetFullScreen);
	//result &= hook_write_jmpret(0x0049d8f0, Gods98::Init_SetModeList);
	//result &= hook_write_jmpret(0x0049da40, Gods98::Init_SetDeviceList);
	//result &= hook_write_jmpret(0x0049db30, Gods98::Init_AddValidMode);
	//result &= hook_write_jmpret(0x0049db90, Gods98::Init_IsValidMode);
	//result &= hook_write_jmpret(0x0049dc10, Gods98::Init_GetMode);
	//result &= hook_write_jmpret(0x0049dc90, Gods98::Init_HandleWindowButton);
	return_interop(result);
}

#pragma endregion


#pragma region LegoRR Game Hooks

bool interop_hook_LegoRR_Advisor(void)
{
	bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00401000, LegoRR::Advisor_Initialise);

	// used by: Level_Free
	result &= hook_write_jmpret(0x004011c0, LegoRR::Advisor_Stop);

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00401210, LegoRR::Advisor_InitViewport);
	result &= hook_write_jmpret(0x00401270, LegoRR::Advisor_LoadAnims);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004013a0, LegoRR::Advisor_GetAdvisorType);
	//result &= hook_write_jmpret(0x00401430, LegoRR::Advisor_GetAnimType);

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x004014a0, LegoRR::Advisor_LoadPositions);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004016f0, LegoRR::Advisor_AddPosition);

	// used by: Panel_SetCurrentAdvisorFromButton
	result &= hook_write_jmpret(0x00401780, LegoRR::Advisor_SetParameters);
	result &= hook_write_jmpret(0x004017d0, LegoRR::Advisor_GetOrigPos);

	// used by: Interface_FUN_0041c420, NERPs_Level_NERPMessage_Parse, Objective_Update,
	//          Panel_SetCurrentAdvisorFromButton
	result &= hook_write_jmpret(0x00401800, LegoRR::Advisor_Start);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00401870, LegoRR::Advisor_PlaySFX);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x004018d0, LegoRR::Advisor_Update);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004019b0, LegoRR::Advisor_MoveAnimation);

	// used by: Lego_HandleWorldDebugKeys, NERPFunc__FlashCallToArmsIcon, NERPsRuntime_UpdateTimers,
	//          NERPsRuntime_FlashIcon, Objective_StopShowing, Objective_Update
	result &= hook_write_jmpret(0x00401a60, LegoRR::Advisor_End);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00401a70, LegoRR::Advisor_ViewportTransform);

	// used by: Interface_FUN_0041c420
	result &= hook_write_jmpret(0x00401b60, LegoRR::Advisor_IsAnimating);
	
	return_interop(result);
}

bool interop_hook_LegoRR_AITask(void)
{
	bool result = true;

	// used by: Priorities_LoadImages, Priorities_LoadLevel
	result &= hook_write_jmpret(0x00401bf0, LegoRR::AIPriority_GetType);

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00401c30, LegoRR::AITask_Initialise);

	// used by: Lego_Shutdown_Full
	result &= hook_write_jmpret(0x00402000, LegoRR::AITask_Shutdown);

	// internal, no need to hook these
	// used by: AITask_Remove
	//result &= hook_write_jmpret(0x00402c00, LegoRR::AITask_RemoveGetToolReferences);

	// used by: AITask
	result &= hook_write_jmpret(0x00406330, LegoRR::AITask_Clone);
	result &= hook_write_jmpret(0x00406370, LegoRR::AITask_Create);
	result &= hook_write_jmpret(0x004063b0, LegoRR::AITask_Remove);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004063f0, LegoRR::AITask_AddList);

	// used by: AITask
	result &= hook_write_jmpret(0x00406470, LegoRR::AITask_RunThroughLists);
	
	return_interop(result);
}

bool interop_hook_LegoRR_Bubbles(void)
{
	bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00406fe0, LegoRR::Bubble_Initialise);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00407170, LegoRR::Bubble_LoadBubbles);
	// used by: Bubble_LoadBubbles
	result &= hook_write_jmpret(0x00407230, LegoRR::Bubble_GetBubbleType);
	// used by: Lego_HandleKeys, Objective_HandleKeys
	result &= hook_write_jmpret(0x00407270, LegoRR::Bubble_ToggleObjectUIsAlwaysVisible);
	// used by: Objective_HandleKeys
	result &= hook_write_jmpret(0x00407290, LegoRR::Bubble_GetObjectUIsAlwaysVisible);
	// used by: AITask_Callback_UpdateObject
	result &= hook_write_jmpret(0x004072a0, LegoRR::Bubble_ResetObjectBubbleImage);
	// used by: LegoObject_Remove
	result &= hook_write_jmpret(0x004072d0, LegoRR::Bubble_RemoveObjectReferences);
	// used by: LegoObject_UpdatePowerConsumption
	result &= hook_write_jmpret(0x00407340, LegoRR::Bubble_ShowPowerOff);
	// used by: AITask_Game_PTL_GotoOrRMGoto, AITask_FUN_00406290, LegoObject_TryRunAway
	result &= hook_write_jmpret(0x00407380, LegoRR::Bubble_ShowBubble);
	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x004073e0, LegoRR::Bubble_ShowCallToArms);
	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x00407440, LegoRR::Bubble_SetCallToArmsTimer);
	// used by: LegoObject_AddDamage2
	result &= hook_write_jmpret(0x00407470, LegoRR::Bubble_ShowHealthBar);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x004074d0, LegoRR::Bubble_DrawAllObjInfos);
	// used by: Bubble_DrawAllObjInfos, Bubble_Callback_DrawObjInfo
	result &= hook_write_jmpret(0x004077f0, LegoRR::Bubble_UpdateAndGetBubbleImage);
	// used by: Bubble_DrawAllObjInfos
	result &= hook_write_jmpret(0x00407890, LegoRR::Bubble_Callback_DrawObjInfo);
	// used by: Bubble_ResetObjectBubbleImage, Bubble_UpdateAndGetBubbleImage
	result &= hook_write_jmpret(0x00407940, LegoRR::Bubble_EvaluateObjectBubbleImage);
	
	return_interop(result);
}

bool interop_hook_LegoRR_Building(void)
{
	bool result = true;

	// All functions not listed are merged functions, and are best left unhooked.

	// used by: Lego_LoadBuildingTypes
	result &= hook_write_jmpret(0x00407c90, LegoRR::Building_Load);
	// used by: LegoObject_ProccessCarriedObjects
	result &= hook_write_jmpret(0x00408210, LegoRR::Building_ChangePowerLevel);
	// used by: Construction_CleanupBuildingFoundation, ElectricFence_Callback_ChainBeamsFromBuilding,
	//          Lego_LoadOLObjectList, HiddenObject_ExposeBlock, LegoObject_Callback_PickSphereSelection,
	//          LegoObject_Callback_HideCertainObjects, LegoObject_UpdateBuildingPlacement
	result &= hook_write_jmpret(0x00408290, LegoRR::Building_GetShapePoints);
	// used by: Building_SetActivity, Building_SetUpgradeLevel
	result &= hook_write_jmpret(0x004082d0, LegoRR::Building_SetUpgradeActivity);
	// used by: LegoObject_UpdateActivityChange
	result &= hook_write_jmpret(0x004084a0, LegoRR::Building_SetActivity);
	// used by: LegoObject_UpdateActivityChange
	result &= hook_write_jmpret(0x00408520, LegoRR::Building_GetCameraNull);
	// used by: LegoObject_Create
	result &= hook_write_jmpret(0x00408550, LegoRR::Building_Clone);
	// used by: Lego_LoadBuildingTypes, LegoObject_Hide
	result &= hook_write_jmpret(0x004085a0, LegoRR::Building_Hide);

	// used by: LegoObject_SetHeadingOrDirection, LegoObject_SetPositionAndHeading
	result &= hook_write_jmpret(0x004085f0, LegoRR::Building_SetOrientation);
	// used by: LegoObject_SetPositionAndHeading, LegoObject_UpdateWorldStickyPosition
	result &= hook_write_jmpret(0x00408640, LegoRR::Building_SetPosition);
	// used by: LegoObject_UpdateCarrying
	result &= hook_write_jmpret(0x004086a0, LegoRR::Building_GetCarryNull);
	// used by: LegoObject_TryDepositCarried, LegoObject_GetDepositNull,
	//          LegoObject_ProccessCarriedObjects, LegoObject_UpdateTeleporter
	result &= hook_write_jmpret(0x004086e0, LegoRR::Building_GetDepositNull);
	// used by: AITask_FUN_00404ef0, LegoObject_FUN_00440470, LegoObject_TryFindDriver_FUN_00440690,
	//          LegoObject_SpawnDropCrystals_FUN_0044b110
	result &= hook_write_jmpret(0x00408710, LegoRR::Building_GetEntranceNull);
	// used by: LegoObject_TryDepositCarried, LegoObject_SpawnCarryableObject, LegoObject_DoGetTool
	result &= hook_write_jmpret(0x00408740, LegoRR::Building_GetToolNull);
	// used by: LegoObject_Create
	result &= hook_write_jmpret(0x00408780, LegoRR::Building_GetCarryNullFrames);

	// used by: LegoObject_MoveAnimation
	result &= hook_write_jmpret(0x00408790, LegoRR::Building_MoveAnimation);
	// used by: LegoObject_CalculateSpeeds
	result &= hook_write_jmpret(0x00408860, LegoRR::Building_GetTransCoef);
	// used by: Lego_Shutdown_Full, LegoObject_Remove
	result &= hook_write_jmpret(0x00408870, LegoRR::Building_Remove);
	// used by: Lego_HandleWorldDebugKeys
	result &= hook_write_jmpret(0x004088a0, LegoRR::Building_CanUpgradeType);
	// used by: Building_Remove, Lego_HandleWorldDebugKeys, LegoObject_Create, LegoObject_UpgradeBuilding
	result &= hook_write_jmpret(0x004088d0, LegoRR::Building_SetUpgradeLevel);

	return_interop(result);
}

bool interop_hook_LegoRR_Collision(void)
{
	bool result = true;
	
	// used by: Collision_DistanceToPolyOutline
	result &= hook_write_jmpret(0x00408900, LegoRR::Collision_DistanceToLine);

	// used by: LegoObject_CallbackCollisionRadius_FUN_004460b0,
	//          LegoObject_CallbackCollisionBox_FUN_004463b0
	result &= hook_write_jmpret(0x00408a30, LegoRR::Collision_DistanceToPolyOutline);

	// used by: Weapon_LegoObject_CollisionRadius_FUN_00470800
	result &= hook_write_jmpret(0x00408a90, LegoRR::Collision_PointOnLine);

	// used by: Weapon_LegoObject_Callback_FUN_00471630
	result &= hook_write_jmpret(0x00408b20, LegoRR::Collision_PointOnLineRay);
	
	return_interop(result);
}

bool interop_hook_LegoRR_Construction(void)
{
	bool result = true;

	// used by: Stats_Initialise
	result &= hook_write_jmpret(0x00408bb0, LegoRR::Construction_GetBuildingBase);
	// used by: AITask_LiveObject_SetAITaskUnk, LegoObject_UpdateBuildingPlacement
	result &= hook_write_jmpret(0x00408c10, LegoRR::Construction_Zone_PlaceResource);
	// used by: AITask_LiveObject_SetAITaskUnk
	result &= hook_write_jmpret(0x00408ca0, LegoRR::Construction_Zone_NeedsMoreOfResource);
	// used by: Construction_Zone_NeedsMoreOfResource, Lego_ShowBlockToolTip
	result &= hook_write_jmpret(0x00408d40, LegoRR::Construction_Zone_CountOfResourcePlaced);
	// used by: Construction_Zone_RequestPathResources, LegoObject_UpdateBuildingPlacement
	result &= hook_write_jmpret(0x00408d80, LegoRR::Construction_Zone_RequestResource);
	// used by: LegoObject_UpdateBuildingPlacement
	result &= hook_write_jmpret(0x00408df0, LegoRR::Construction_Zone_RequestBarriers);
	// used by: Interface_HandleMenuItem, Lego_LoadPathMap, Level_BlockUpdateSurface,
	//          AITask_DoClearTypeAction, Level_DestroyWall, Level_CanBuildOnBlock
	result &= hook_write_jmpret(0x00408fd0, LegoRR::Construction_Zone_ExistsAtBlock);
	// used by: Construction_Zone_PlaceResource, Construction_Zone_NeedsMoreOfResource,
	//          Construction_Zone_RequestResource, Construction_Zone_RequestBarriers,
	//          Construction_Zone_ExistsAtBlock, Construction_Zone_CompletePath,
	//          Construction_Zone_CancelPath, Construction_Zone_CancelBuilding,
	//          LegoObject_Callback_Update
	result &= hook_write_jmpret(0x00408ff0, LegoRR::Construction_Zone_FindByHandleOrAtBlock);
	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x00409040, LegoRR::Construction_Zone_CompletePath);
	// used by: Interface_DoAction_FUN_0041dbd0
	result &= hook_write_jmpret(0x00409080, LegoRR::Construction_Zone_CancelPath);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00409110, LegoRR::Construction_UpdateAll);
	// used by: Construction_UpdateAll
	result &= hook_write_jmpret(0x004091a0, LegoRR::Construction_Zone_NoForeignObjectsInside);
	// used by: Construction_Zone_NoForeignObjectsInside
	result &= hook_write_jmpret(0x004091c0, LegoRR::Construction_Zone_ObjectCallback_IsForeignObjectInside);
	// used by: Interface_DoAction_FUN_0041dbd0
	result &= hook_write_jmpret(0x00409230, LegoRR::Construction_Zone_StartPath);
	// used by: Construction_Zone_StartPath, Construction_Zone_StartBuilding
	result &= hook_write_jmpret(0x00409280, LegoRR::Construction_Zone_Create);
	// used by: LegoObject_UpdatePowerConsumption
	result &= hook_write_jmpret(0x004092e0, LegoRR::Construction_PowerGrid_PowerAdjacentBlocks);
	// used by: LegoObject_UpdatePowerConsumption
	result &= hook_write_jmpret(0x00409380, LegoRR::Construction_PowerGrid_DrainAdjacentBlocks);
	// used by: Construction_PowerGrid_DrainAdjacentBlocks_Recurse
	result &= hook_write_jmpret(0x004093a0, LegoRR::Construction_PowerGrid_DrainAdjacentBlocks_Recurse);
	// used by: Construction_UpdateAll
	result &= hook_write_jmpret(0x00409480, LegoRR::Construction_Zone_RequestPathResources);
	// used by: Interface_DoAction_FUN_0041dbd0
	result &= hook_write_jmpret(0x00409530, LegoRR::Construction_Zone_CancelBuilding);
	// used by: LegoObject_UpdateBuildingPlacement
	result &= hook_write_jmpret(0x004096c0, LegoRR::Construction_Zone_StartBuilding);
	// used by: Construction_Zone_StartBuilding, Construction_SpawnBuilding, Level_DestroyWall
	result &= hook_write_jmpret(0x00409870, LegoRR::Construction_FlattenGround);
	// used by: Level_Free
	result &= hook_write_jmpret(0x00409900, LegoRR::Construction_RemoveAll);
	// used by: Construction_Zone_CompletePath, Construction_Zone_CancelPath,
	//          Construction_UpdateAll, Construction_Zone_CancelBuilding, Construction_RemoveAll
	result &= hook_write_jmpret(0x00409920, LegoRR::Construction_Zone_Free);
	// used by: Construction_Zone_CompletePath, Construction_UpdateAll
	result &= hook_write_jmpret(0x00409970, LegoRR::Construction_Zone_ConsumePlacedResources);
	// used by: Lego_LoadOLObjectList, HiddenObject_ExposeBlock
	result &= hook_write_jmpret(0x004099c0, LegoRR::Construction_SpawnBuilding);
	// used by: Construction_UpdateAll, Construction_SpawnBuilding
	result &= hook_write_jmpret(0x00409a60, LegoRR::Construction_Zone_CompleteBuilding);
	// used by: Construction_Zone_CompleteBuilding
	result &= hook_write_jmpret(0x00409c00, LegoRR::Construction_DeselectAdjacentWalls);
	// used by: Lego_QuitLevel, Lego_LoadLevel, Objective_SetStatus
	result &= hook_write_jmpret(0x00409c70, LegoRR::Construction_DisableCryOreDrop);
	// used by: LegoObject_UpdateRemoval
	result &= hook_write_jmpret(0x00409c80, LegoRR::Construction_CleanupBuildingFoundation);
	// used by: Construction_CleanupBuildingFoundation
	result &= hook_write_jmpret(0x00409e50, LegoRR::Construction_GenerateCryOreDrop);
	// used by: LegoObject_UpdateRemoval, Message_Debug_DestroySelectedUnits
	result &= hook_write_jmpret(0x00409f20, LegoRR::Construction_RemoveBuildingObject);

	return_interop(result);
}

bool interop_hook_LegoRR_Creature(void)
{
	bool result = true;

	// All functions not listed are merged functions, and are best left unhooked.

	// used by: LegoObject_FP_GetPositionAndHeading
	result &= hook_write_jmpret(0x004068b0, LegoRR::Creature_IsCameraFlipDir);
	// used by: Lego_LoadMiniFigureTypes, Lego_LoadRockMonsterTypes
	result &= hook_write_jmpret(0x004068c0, LegoRR::Creature_Load);
	// used by: LegoObject_FP_Callback_SwapPolyMeshParts
	result &= hook_write_jmpret(0x00406b30, LegoRR::Creature_SwapPolyMedium);
	// used by: LegoObject_FP_Callback_SwapPolyMeshParts
	result &= hook_write_jmpret(0x00406b60, LegoRR::Creature_SwapPolyHigh);
	// used by: LegoObject_SwapPolyFP
	result &= hook_write_jmpret(0x00406b90, LegoRR::Creature_SwapPolyFP);

	// used by: LegoObject_Create
	result &= hook_write_jmpret(0x00406be0, LegoRR::Creature_Clone);

	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x00406c40, LegoRR::Creature_SetAnimationTime);
	// used by: LegoObject_MoveAnimation
	result &= hook_write_jmpret(0x00406c60, LegoRR::Creature_MoveAnimation);
	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x00406cd0, LegoRR::Creature_GetAnimationTime);
	// used by: LegoObject_Route_End, LegoObject_SetHeadingOrDirection, LegoObject_UpdateRoutingVectors,
	//          LegoObject_SetPositionAndHeading, LegoObject_Route_UpdateMovement
	result &= hook_write_jmpret(0x00406cf0, LegoRR::Creature_SetOrientation);
	// used by: LegoObject_UpdateRoutingVectors, LegoObject_SetPositionAndHeading,
	//          LegoObject_FP_UpdateMovement, LegoObject_UpdateWorldStickyPosition
	result &= hook_write_jmpret(0x00406d20, LegoRR::Creature_SetPosition);

	// used by: LegoObject_UpdateRemoval, LegoObject_UpdateActivityChange
	result &= hook_write_jmpret(0x00406d70, LegoRR::Creature_SetActivity);
	// used by: Lego_Shutdown_Full, LegoObject_Remove
	result &= hook_write_jmpret(0x00406df0, LegoRR::Creature_Remove);

	// used by: LegoObject_UpdateActivityChange, LegoObject_FP_GetPositionAndHeading
	result &= hook_write_jmpret(0x00406eb0, LegoRR::Creature_GetCameraNull);
	// used by: LegoObject_Callback_Update, LegoObject_GetDrillNullPosition
	result &= hook_write_jmpret(0x00406ee0, LegoRR::Creature_GetDrillNull);
	// used by: LegoObject_UpdateCarrying
	result &= hook_write_jmpret(0x00406f10, LegoRR::Creature_GetCarryNull);
	// used by: LegoObject_GetDepositNull
	result &= hook_write_jmpret(0x00406f40, LegoRR::Creature_GetDepositNull);
	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x00406f70, LegoRR::Creature_CheckThrowNull);
	// used by: LegoObject_CalculateSpeeds
	result &= hook_write_jmpret(0x00406fc0, LegoRR::Creature_GetTransCoef);

	// used by: 
	//result &= hook_write_jmpret(0x, LegoRR::);
	// used by: 
	//result &= hook_write_jmpret(0x, LegoRR::);
	
	return_interop(result);
}

bool interop_hook_LegoRR_Credits(void)
{
	bool result = true;

	// used by: Front_Callback_TriggerPlayCredits
	result &= hook_write_jmpret(0x00409ff0, LegoRR::Credits_Play);
	
	return_interop(result);
}

bool interop_hook_LegoRR_BezierCurve(void)
{
	bool result = true;

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00406520, LegoRR::BezierCurve_Curve);
	//result &= hook_write_jmpret(0x00406660, LegoRR::BezierCurve_Vector2DDistance);

	// used by: LegoObject, Weapons
	result &= hook_write_jmpret(0x00406690, LegoRR::BezierCurve_Vector2DChangeLength);
	result &= hook_write_jmpret(0x004066e0, LegoRR::BezierCurve_UpdateDistances);
	result &= hook_write_jmpret(0x00406750, LegoRR::BezierCurve_BuildPoints);
	result &= hook_write_jmpret(0x004067f0, LegoRR::BezierCurve_Interpolate);
	
	return_interop(result);
}

bool interop_hook_LegoRR_DamageText(void)
{
	bool result = true;

	// used by: Level_Free
	result &= hook_write_jmpret(0x0040a300, LegoRR::DamageText_RemoveAll);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x0040a330, LegoRR::DamageText_LoadTextures);
	// used by: LegoObject_UpdateEnergyHealthAndContact
	result &= hook_write_jmpret(0x0040a3e0, LegoRR::DamageText_ShowNumber);
	// used by: DamageText_ShowNumber
	result &= hook_write_jmpret(0x0040a4f0, LegoRR::DamageText_GetNextFree);
	// used by: DamageText_ShowNumber
	result &= hook_write_jmpret(0x0040a510, LegoRR::DamageText_SetNumber);
	// used by: DamageText_ShowNumber
	result &= hook_write_jmpret(0x0040a670, LegoRR::DamageText_MeshRenderCallback);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0040a940, LegoRR::DamageText_UpdateAll);
	// used by: DamageText_UpdateAll
	result &= hook_write_jmpret(0x0040a970, LegoRR::DamageText_Update);
	// used by: DamageText_ShowNumber, Lego_ShowObjectToolTip,
	//          ObjInfo_DrawHealthBar
	result &= hook_write_jmpret(0x0040aa10, LegoRR::DamageText_CanShow);
	
	return_interop(result);
}

bool interop_hook_LegoRR_Dependencies(void)
{
	bool result = true;

	// used by: Lego_HandleWorldDebugKeys, OpenLRR_HandleCommand
	result &= hook_write_jmpret(0x0040aa40, LegoRR::Dependencies_SetEnabled);
	// used by: Level_Free
	result &= hook_write_jmpret(0x0040aa60, LegoRR::Dependencies_Reset_ClearAllLevelFlags_10c);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x0040aaa0, LegoRR::Dependencies_Initialise);
	// used by: Lego_HandleWorldDebugKeys, AITask_DoBuildPath_AtPosition, Dependencies_Prepare_Unk,
	// Interface_ChangeObjectIconFlag1_FUN_0041c730, Interface_HandleMenuItem
	result &= hook_write_jmpret(0x0040add0, LegoRR::Dependencies_Object_FUN_0040add0);
	// used by: Dependencies_Object_FUN_0040add0, Interface_FUN_0041ebd0
	result &= hook_write_jmpret(0x0040ae70, LegoRR::Dependencies_LiveObject_CallbackCheck_FUN_0040ae70);
	// used by: Dependencies_Object_FUN_0040add0, Dependencies_Object_Unlock, Dependencies_Object_Unlock,
	// Interface_FUN_0041ebd0, Interface_FUN_0041edb0
	result &= hook_write_jmpret(0x0040aec0, LegoRR::Dependencies_Object_GetRequirements);
	// used by: HelpWindow_Object_Unlock
	result &= hook_write_jmpret(0x0040af30, LegoRR::Dependencies_Object_Unlock);
	// used by: Dependencies_Initialise, HelpWindow_RecallDependencies
	result &= hook_write_jmpret(0x0040b0e0, LegoRR::Dependencies_Prepare_Unk);
	// used by: HelpWindow_Object_Unlock
	result &= hook_write_jmpret(0x0040b180, LegoRR::Dependencies_Object_IsLevelFlag4);
	// used by: HelpWindow_RecallDependencies
	result &= hook_write_jmpret(0x0040b1d0, LegoRR::Dependencies_Object_AddLevelFlag_100);
	// used by: HelpWindow_RecallDependencies
	result &= hook_write_jmpret(0x0040b210, LegoRR::Dependencies_Object_GetLevelFlag_100);

	return_interop(result);
}

bool interop_hook_LegoRR_Effect(void)
{
	bool result = true;

	// used by: Level_Free
	result &= hook_write_jmpret(0x0040bcf0, LegoRR::Effect_StopAll);
	// used by: LegoObject_DestroyBoulder_AndCreateExplode
	result &= hook_write_jmpret(0x0040bd10, LegoRR::Effect_Spawn_BoulderExplode_AtSimpleObject);
	// used by: Effect_Spawn_BoulderExplode_AtSimpleObject, Weapon_GunHitObject, Weapon_FireLazer
	result &= hook_write_jmpret(0x0040bd40, LegoRR::Effect_Spawn_BoulderExplode);
	// used by: LegoObject_AttackPath
	result &= hook_write_jmpret(0x0040bde0, LegoRR::Effect_Spawn_SmashPath);
	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x0040bea0, LegoRR::Effect_FindRockFallStyle);
	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x0040bef0, LegoRR::Effect_SetRockFallStyle);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x0040bf00, LegoRR::Effect_Load_RockFallStylesAll);
	// used by: Effect_Load_RockFallStylesAll
	result &= hook_write_jmpret(0x0040c000, LegoRR::Effect_Load_RockFallStyle);
	// used by: Lego_LoadMiscObjects
	result &= hook_write_jmpret(0x0040c0e0, LegoRR::Effect_Load_ElectricFenceBeam);
	// used by: Fallin_GenerateLandSlide, Lego_PTL_RockFall
	result &= hook_write_jmpret(0x0040c160, LegoRR::Effect_Spawn_RockFall);
	// used by: ElectricFence_TrySpawnBeamBetweenBlocks, ElectricFence_SparkObject
	result &= hook_write_jmpret(0x0040c220, LegoRR::Effect_Spawn_ElectricFenceBeam);
	// used by: Effect_StopAll, Level_UpdateEffects
	result &= hook_write_jmpret(0x0040c2d0, LegoRR::Effect_UpdateAll);
	// used by: Effect_UpdateAll
	result &= hook_write_jmpret(0x0040c400, LegoRR::Effect_Update_BoulderExplode);
	// used by: Effect_UpdateAll
	result &= hook_write_jmpret(0x0040c450, LegoRR::Effect_Update_SmashPath);
	// used by: Level_UpdateEffects
	result &= hook_write_jmpret(0x0040c4a0, LegoRR::Effect_GetBlockPos_RockFall);
	// used by: LegoObject_UpdateWorldStickyPosition
	result &= hook_write_jmpret(0x0040c4e0, LegoRR::Effect_GetTumbleNull_RockFall);
	// used by: Level_Free
	result &= hook_write_jmpret(0x0040c5c0, LegoRR::Effect_RemoveAll_BoulderExplode);
	// used by: Lego_Shutdown_Full
	result &= hook_write_jmpret(0x0040c5f0, LegoRR::Effect_RemoveAll_RockFall);
	// used by: Lego_LoadMiscObjects
	result &= hook_write_jmpret(0x0040c650, LegoRR::Effect_Load_Explosion);
	// used by: LegoObject_SimpleObject_MoveAnimation
	result &= hook_write_jmpret(0x0040c680, LegoRR::Effect_Spawn_Explosion);
	// used by: Effect_UpdateAll
	result &= hook_write_jmpret(0x0040c760, LegoRR::Effect_Update_Explosion);
	// used by: Effect_Spawn_Particle
	result &= hook_write_jmpret(0x0040c7d0, LegoRR::Effect_GetMiscEffectData);
	// used by: Effect_Initialise
	result &= hook_write_jmpret(0x0040c850, LegoRR::Effect_Load_Misc);
	// used by: Lego_LoadMiscObjects
	result &= hook_write_jmpret(0x0040c8c0, LegoRR::Effect_Initialise);
	// used by: Effect_Update_MiscEffectsAll
	result &= hook_write_jmpret(0x0040caa0, LegoRR::Effect_Update_MiscEffect);
	// used by: Effect_UpdateAll
	result &= hook_write_jmpret(0x0040cb70, LegoRR::Effect_Update_MiscEffectsAll);
	// used by: Construction_CleanupBuildingFoundation, Erosion_Update, Level_Block_ClearRubbleLayer,
	//          LegoObject_UpgradeBuilding, LegoObject_SimpleObject_MoveAnimation,
	//          Weapon_GunHitObject, Weapon_FireLazer
	result &= hook_write_jmpret(0x0040cc10, LegoRR::Effect_Spawn_Particle);

	return_interop(result);
}

bool interop_hook_LegoRR_Encyclopedia(void)
{
	bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x0040e3c0, LegoRR::Encyclopedia_Initialise);

	// used by: Lego_HandleWorld
	result &= hook_write_jmpret(0x0040e630, LegoRR::Encyclopedia_SelectObject);

	// used by: Interface_DoAction_FUN_0041dbd0, Interface_FUN_0041e9f0
	result &= hook_write_jmpret(0x0040e710, LegoRR::Encyclopedia_ClearSelection);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0040e720, LegoRR::Encyclopedia_Update);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0040e800, LegoRR::Encyclopedia_DrawSelectBox);

	// used by: LegoObject_Remove
	result &= hook_write_jmpret(0x0040e840, LegoRR::Encyclopedia_RemoveCurrentReference);

	return_interop(result);
}

bool interop_hook_LegoRR_Erosion(void)
{
	bool result = true;

	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x0040e860, LegoRR::Erosion_Initialise);
	// used by: Erosion_AddActiveBlock, Erosion_Update
	result &= hook_write_jmpret(0x0040e8c0, LegoRR::Erosion_GetFreeActiveIndex);
	// used by: Erosion_AddActiveBlock, Erosion_Update
	result &= hook_write_jmpret(0x0040e8f0, LegoRR::Erosion_GetBlockErodeRate);
	// used by: Lego_LoadErodeMap
	result &= hook_write_jmpret(0x0040e940, LegoRR::Erosion_AddActiveBlock);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0040e9e0, LegoRR::Erosion_Update);
	// used by: Erosion_Update, Erosion_AddOrRemoveLavaBlock
	result &= hook_write_jmpret(0x0040ed30, LegoRR::Erosion_AddLockedBlock);
	// used by: Erosion_AddActiveBlock, Erosion_Update, Level_BlockUpdateSurface,
	//          Level_Block_SetPath
	result &= hook_write_jmpret(0x0040ed80, LegoRR::Erosion_AddOrRemoveLavaBlock);
	// used by: Erosion_FindAdjacentBlockPos
	result &= hook_write_jmpret(0x0040eee0, LegoRR::Erosion_IsBlockLocked);
	// used by: Erosion_Update
	result &= hook_write_jmpret(0x0040ef30, LegoRR::Erosion_FindAdjacentBlockPos);

	return_interop(result);
}

bool interop_hook_LegoRR_Fallin(void)
{
	bool result = true;

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0040f010, LegoRR::Fallin_Update);
	// used by: Fallin_Update, Fallin_GenerateCaveIn, Lego_UpdateFallins,
	//          Level_GenerateLandSlideNearBlock
	result &= hook_write_jmpret(0x0040f0c0, LegoRR::Fallin_TryGenerateLandSlide);
	// used by: Fallin_TryGenerateLandSlide, Fallin_GenerateCaveIn
	result &= hook_write_jmpret(0x0040f1e0, LegoRR::Fallin_CanLandSlideAtBlock);
	// used by: Fallin_TryGenerateLandSlide, Fallin_GenerateCaveIn, Lego_HandleWorldDebugKeys
	result &= hook_write_jmpret(0x0040f260, LegoRR::Fallin_GenerateLandSlide);
	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x0040f510, LegoRR::Fallin_Initialise);
	// used by: Fallin_GenerateLandSlide
	result &= hook_write_jmpret(0x0040f520, LegoRR::Fallin_GenerateCaveIn);

	return_interop(result);
}

bool interop_hook_LegoRR_FrontEnd(void)
{
	bool result = true;

	// used by: Front_Menu_Update, Front_LoadMenuTextWindow, Front_Initialise
	result &= hook_write_jmpret(0x004101e0, LegoRR::Front_Util_ReplaceTextSpaces);

	// used by: Front_Menu_Update, Front_Callback_SelectMissionItem
	result &= hook_write_jmpret(0x00410250, LegoRR::Front_LevelSelect_LevelNamePrintF);

	// used by: Front_Initialise, Reward_Show
	result &= hook_write_jmpret(0x00410300, LegoRR::Front_Callback_TriggerPlayCredits);

	// used by: Front_ScreenMenuLoop
	result &= hook_write_jmpret(0x00410320, LegoRR::Front_RockWipe_Play);

	// used by: Front_Menu_Update, Front_ScreenMenuLoop
	result &= hook_write_jmpret(0x00410370, LegoRR::Front_RockWipe_Stop);

	// used by: Front_Cache_Create
	result &= hook_write_jmpret(0x00410380, LegoRR::Front_Cache_FindByName);

	// used by: Front_Cache_LoadImage, Front_Cache_LoadFont
	result &= hook_write_jmpret(0x004103c0, LegoRR::Front_Cache_Create);

	// used by: Front_MenuItem_AddSelectItem, Front_Menu_LoadMenuImage,
	//          Front_Menu_LoadSliderImages
	result &= hook_write_jmpret(0x00410450, LegoRR::Front_Cache_LoadImage);

	// used by: Front_LoadMenuSet
	result &= hook_write_jmpret(0x00410490, LegoRR::Front_Cache_LoadFont);

	// used by: Front_MenuItem_CreateSelect, Front_MenuItem_AddSelectItem,
	//          Front_MenuItem_AddCycleName, Front_MenuItem_CreateBannerItem,
	//          Front_MenuItem_CreateImageItem, Front_Menu_CreateMenu,
	//          Front_Menu_CreateOverlay, Front_LoadLevelSet
	result &= hook_write_jmpret(0x004104d0, LegoRR::Front_Util_StrCpy);

	// used by: Front_LoadLevels, MainMenuFull_AddMissionsDisplay
	result &= hook_write_jmpret(0x00410520, LegoRR::Front_MenuItem_CreateSelect);

	// used by: Front_MenuItem_FreeMenuItem
	result &= hook_write_jmpret(0x004105c0, LegoRR::Front_MenuItem_FreeSelect);

	// used by: Front_LoadLevels, Front_LevelInfo_Callback_AddItem
	result &= hook_write_jmpret(0x00410670, LegoRR::Front_MenuItem_AddSelectItem);

	// used by: Front_LoadMenuSet
	result &= hook_write_jmpret(0x00410940, LegoRR::Front_MenuItem_CreateCycle);

	// used by: Front_LoadMenuSet
	result &= hook_write_jmpret(0x004109d0, LegoRR::Front_MenuItem_AddCycleName);

	// used by: Front_MenuItem_CreateCycle, Front_MenuItem_FreeMenuItem
	result &= hook_write_jmpret(0x00410a10, LegoRR::Front_MenuItem_FreeCycle);

	// used by: Front_LoadMenuSet
	result &= hook_write_jmpret(0x00410a90, LegoRR::Front_MenuItem_CreateTrigger);

	// used by: Front_MenuItem_FreeMenuItem
	// merged with freeNonNull @ 0040f6e0
	//result &= hook_write_jmpret(0x0040f6e0, LegoRR::Front_MenuItem_FreeTrigger);

	// used by: Front_MenuItem_FreeMenuItem
	// merged with freeNonNull @ 0040f6e0
	//result &= hook_write_jmpret(0x0040f6e0, LegoRR::Front_MenuItem_FreeTextInput);

	// used by: Front_LoadMenuSet
	result &= hook_write_jmpret(0x00410ac0, LegoRR::Front_MenuItem_CreateRealSlider);

	// used by: Front_MenuItem_FreeMenuItem
	// merged with freeNonNull @ 0040f6e0
	//result &= hook_write_jmpret(0x0040f6e0, LegoRR::Front_MenuItem_FreeRealSlider);

	// used by: Front_LoadMenuSet
	result &= hook_write_jmpret(0x00410b20, LegoRR::Front_MenuItem_CreateSlider);

	// used by: Front_MenuItem_FreeMenuItem
	// merged with freeNonNull @ 0040f6e0
	//result &= hook_write_jmpret(0x0040f6e0, LegoRR::Front_MenuItem_FreeSlider);

	// used by: Front_Menu_FreeMenu
	result &= hook_write_jmpret(0x00410ba0, LegoRR::Front_MenuItem_FreeMenuItem);

	// used by: Front_LoadMenuSet, Front_LoadLevels, MainMenuFull_AddMissionsDisplay
	result &= hook_write_jmpret(0x00410c80, LegoRR::Front_MenuItem_CreateBannerItem);

	// used by: Front_LoadMenuSet
	result &= hook_write_jmpret(0x00410d50, LegoRR::Front_MenuItem_CreateImageItem);

	// used by: Front_Menu_CreateMenu
	result &= hook_write_jmpret(0x00410e60, LegoRR::Front_Menu_FreeMenu);

	// used by: Front_LoadMenuSet
	result &= hook_write_jmpret(0x00410ee0, LegoRR::Front_Menu_LoadMenuImage);

	// used by: Front_LoadMenuSet
	result &= hook_write_jmpret(0x00411030, LegoRR::Front_Menu_CreateMenu);

	// used by: Front_LoadMenuSet, Front_LoadLevels, MainMenuFull_AddMissionsDisplay
	result &= hook_write_jmpret(0x00411190, LegoRR::Front_Menu_AddMenuItem);

	// used by: Front_Maths_IsPointInsideRectCentered, Front_Menu_FindItemUnderMouse,
	//          Front_MenuItem_SliderHandleInput, Front_MenuItem_DrawSlider
	result &= hook_write_jmpret(0x00411210, LegoRR::Front_Maths_IsPointInsideRect);

	// used by: Front_MenuItem_Select_TestStringCollision
	result &= hook_write_jmpret(0x00411250, LegoRR::Front_Maths_IsPointInsideRectCentered);

	// used by: Front_Menu_IsLevelItemUnderMouse, Front_Menu_UpdateMenuItemsInput, Front_Menu_Update
	result &= hook_write_jmpret(0x00411290, LegoRR::Front_MenuItem_Select_TestStringCollision);

	// used by: Front_Menu_FindItemUnderMouse
	result &= hook_write_jmpret(0x00411420, LegoRR::Front_Menu_IsLevelItemUnderMouse);

	// used by: Front_Menu_FindItemUnderMouse
	result &= hook_write_jmpret(0x00411460, LegoRR::Front_Menu_GetItemBounds);

	// used by: Front_Menu_UpdateMenuItemsInput
	result &= hook_write_jmpret(0x004116c0, LegoRR::Front_Menu_FindItemUnderMouse);

	// used by: Front_Menu_UpdateMenuItemsInput
	result &= hook_write_jmpret(0x00411770, LegoRR::Front_GetMousePressedState);

	// used by: Front_Menu_UpdateMenuItemsInput
	result &= hook_write_jmpret(0x004117a0, LegoRR::Front_MenuItem_SliderHandleInput);

	// used by: Front_Menu_UpdateMenuItemsInput, Front_Menu_Update
	result &= hook_write_jmpret(0x00411900, LegoRR::Front_MenuItem_CheckNotInTutoAnyTutorialFlags);

	// used by: Front_Menu_Update
	result &= hook_write_jmpret(0x00411930, LegoRR::Front_Menu_UpdateMenuItemsInput);

	// used by: Lego_HandleRenameInput
	result &= hook_write_jmpret(0x00411e30, LegoRR::Front_Input_GetKeyCharacter);

	// used by: Front_Menu_Update
	result &= hook_write_jmpret(0x00411e40, LegoRR::Front_MenuItem_DrawSlider);

	// used by: Front_Menu_UpdateOverlays
	result &= hook_write_jmpret(0x004120a0, LegoRR::Front_Menu_GetOverlayCount);

	// used by: Front_Menu_UpdateOverlays
	// Fix to stop menu overlays from playing incredibly often at faster framerates.
	result &= hook_write_jmpret(0x004120c0, LegoRR::Front_Menu_ShouldRandomPlay);

	// used by: Front_Menu_Update
	result &= hook_write_jmpret(0x004120e0, LegoRR::Front_Menu_UpdateOverlays);

	// used by: Front_Menu_Update
	result &= hook_write_jmpret(0x00412380, LegoRR::Front_MenuItem_DrawSelectItem);

	// used by: Front_Menu_Update
	result &= hook_write_jmpret(0x00412420, LegoRR::Front_MenuItem_DrawSaveImage);

	// used by: Front_Menu_Update
	result &= hook_write_jmpret(0x00412680, LegoRR::Front_Menu_DrawLoadSaveText);

	// used by: Front_Menu_Update
	result &= hook_write_jmpret(0x00412900, LegoRR::Front_MenuItem_DrawSelectTextWindow);

	// used by: Front_Menu_Update
	result &= hook_write_jmpret(0x00412a20, LegoRR::Front_Menu_DrawMenuImage);

	// used by: Front_ScreenMenuLoop, Front_Options_Update
	result &= hook_write_jmpret(0x00412b30, LegoRR::Front_Menu_Update);

	// used by: Front_ScreenMenuLoop
	result &= hook_write_jmpret(0x004138a0, LegoRR::Front_Menu_UpdateMousePosition);

	// used by: Front_ScreenMenuLoop
	result &= hook_write_jmpret(0x00413990, LegoRR::Front_LoadSaveSlotImages);

	// used by: Front_ScreenMenuLoop
	result &= hook_write_jmpret(0x00413a80, LegoRR::Front_FreeSaveSlotImages);

	// used by: Front_RunScreenMenu
	result &= hook_write_jmpret(0x00413ab0, LegoRR::Front_ScreenMenuLoop);

	// used by: Front_RunScreenMenuType
	result &= hook_write_jmpret(0x00413d50, LegoRR::Front_RunScreenMenu);

	// used by: Front_LoadMenuSet
	result &= hook_write_jmpret(0x00413d90, LegoRR::Front_MenuItem_ParseTypeString);

	// used by: Front_LoadMenuSet, Front_LoadLevelSet, Front_LoadLevels,
	//          Front_Callback_SelectMissionItem, Lego_Initialise, Reward_LoadItemText
	result &= hook_write_jmpret(0x00413e30, LegoRR::Front_Util_StringReplaceChar);

	// used by: Front_Menu_CreateOverlay
	result &= hook_write_jmpret(0x00413e60, LegoRR::Front_Menu_GetOverlayType);

	// used by: Front_LoadMenuSet
	result &= hook_write_jmpret(0x00413ec0, LegoRR::Front_Menu_CreateOverlay);

	// used by: Front_LoadMenuSet
	result &= hook_write_jmpret(0x00413f40, LegoRR::Front_Menu_LoadSliderImages);

	// used by: Front_LoadMenuSet
	result &= hook_write_jmpret(0x00413fa0, LegoRR::Front_CreateMenuSet);

	// used by: Front_Initialise
	result &= hook_write_jmpret(0x00413ff0, LegoRR::Front_LoadMenuSet);

	// used by: Front_LoadLevels
	result &= hook_write_jmpret(0x00414bc0, LegoRR::Front_GetMenuIDByName);

	// used by: Lego_Initialise
	//result &= hook_write_jmpret(0x00414c10, LegoRR::Front_IsIntrosEnabled);

	// used by: Front_UpdateOptionsSliders, Front_Initialise
	result &= hook_write_jmpret(0x00414c60, LegoRR::Front_Callback_SliderBrightness);

	// used by: Front_UpdateOptionsSliders, Front_Initialise
	result &= hook_write_jmpret(0x00414d10, LegoRR::Front_Callback_SliderSoundVolume);

	// used by: Front_UpdateOptionsSliders, Front_Initialise
	result &= hook_write_jmpret(0x00414d20, LegoRR::Front_Callback_SliderMusicVolume);

	// used by: Front_Initialise
	result &= hook_write_jmpret(0x00414d40, LegoRR::Front_CalcSliderCDVolume);

	// used by: Front_Initialise
	result &= hook_write_jmpret(0x00414d80, LegoRR::Front_Callback_CycleWallDetail);

	// used by: Front_Initialise
	result &= hook_write_jmpret(0x00414db0, LegoRR::Front_Callback_CycleAutoGameSpeed);

	// used by: Front_Initialise
	result &= hook_write_jmpret(0x00414dd0, LegoRR::Front_Callback_CycleMusic);

	// used by: Front_Initialise
	result &= hook_write_jmpret(0x00414df0, LegoRR::Front_Callback_CycleSound);

	// used by: Front_Initialise
	result &= hook_write_jmpret(0x00414e10, LegoRR::Front_Callback_CycleHelpWindow);

	// used by: Front_Initialise
	result &= hook_write_jmpret(0x00414e40, LegoRR::Front_Callback_TriggerReplayObjective);

	// used by: Front_Initialise
	result &= hook_write_jmpret(0x00414e50, LegoRR::Front_Callback_SliderGameSpeed);

	// used by: Lego_SetGameSpeed
	result &= hook_write_jmpret(0x00414ec0, LegoRR::Front_UpdateSliderGameSpeed);

	// used by: Front_UpdateGameSpeedSliderLevel, Front_Initialise
	result &= hook_write_jmpret(0x00414f60, LegoRR::Front_CalcSliderGameSpeed);

	// used by: Front_ScreenMenuLoop, Front_LoadLevels
	result &= hook_write_jmpret(0x00414fe0, LegoRR::Front_Callback_SelectLoadSave);

	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x00415080, LegoRR::Front_UpdateOptionsSliders);

	// used by: Front_Initialise
	result &= hook_write_jmpret(0x004150c0, LegoRR::Front_Callback_TriggerBackSave);

	// used by: Lego_MainLoop
	// Hook this function because we need to override level loading.
	result &= hook_write_jmpret(0x00415150, LegoRR::Front_RestartLevel);

	// used by: Front_LoadLevels
	result &= hook_write_jmpret(0x004151f0, LegoRR::Front_Save_GetLevelCompleteWithPoints);

	// used by: Front_Options_Update
	result &= hook_write_jmpret(0x00415290, LegoRR::Front_UpdateGameSpeedSliderLevel);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x004152a0, LegoRR::Front_Options_Update);

	// used by: Front_Initialise
	result &= hook_write_jmpret(0x004153e0, LegoRR::Front_LoadLevelSet);

	// QoL apply for always-skippable splash screens and movies
	// used by: Front_PlayIntroMovie, Front_PlayLevelMovie, Reward_Show
	result &= hook_write_jmpret(0x00415630, LegoRR::Front_PlayMovie);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x004156f0, LegoRR::Front_PlayIntroSplash);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00415840, LegoRR::Front_PlayIntroMovie);
	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x004158c0, LegoRR::Front_PlayLevelMovie);

	// used by: Front_Initialise
	result &= hook_write_jmpret(0x00415940, LegoRR::Front_LoadLevels);

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00415c20, LegoRR::Front_ResetSaveNumber);

	// used by: Front_Initialise
	result &= hook_write_jmpret(0x00415c30, LegoRR::Front_LoadMenuTextWindow);

	// used by: Front_Callback_SelectMissionItem
	result &= hook_write_jmpret(0x00416080, LegoRR::Front_LevelSelect_PlayLevelNameSFX);

	// used by: Front_Callback_SelectMissionItem, Front_Callback_SelectTutorialItem
	result &= hook_write_jmpret(0x004160d0, LegoRR::Front_LevelSelect_PlayTutoLevelNameSFX);

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00416120, LegoRR::Front_Initialise);

	// used by: Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00416840, LegoRR::Front_SaveOptionParameters);

	// used by: Front_PrepareScreenMenuType, Lego_Initialise, Lego_EndLevel
	result &= hook_write_jmpret(0x00416870, LegoRR::Front_LoadOptionParameters);

	// used by: Front_RunScreenMenuType
	result &= hook_write_jmpret(0x004168f0, LegoRR::Front_PrepareScreenMenuType);

	// used by: Lego_Initialise, Lego_EndLevel, Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00416bb0, LegoRR::Front_RunScreenMenuType);

	// used by: Lego_Initialise, Lego_MainLoop, Lego_HandleKeys, Lego_EndLevel,
	//          Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00416c30, LegoRR::Front_IsFrontEndEnabled);

	// used by: Front_GetSelectedLevel, Lego_EndLevel, Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00416c80, LegoRR::Front_IsMissionSelected);

	// used by: Front_GetSelectedLevel, Front_Levels_GetTutoOrMissions, Front_Save_SetLevelCompleted,
	//          Lego_EndLevel, Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00416c90, LegoRR::Front_IsTutorialSelected);

	// used by: Lego_Initialise, Lego_EndLevel
	result &= hook_write_jmpret(0x00416ca0, LegoRR::Front_GetSelectedLevel);

	// used by: Lego_Initialise, Lego_EndLevel
	result &= hook_write_jmpret(0x00416d00, LegoRR::Front_IsTriggerAppQuit);

	// used by: Lego_MainLoop
	//result &= hook_write_jmpret(0x00416d10, LegoRR::Front_IsTriggerMissionQuit);

	// used by: Lego_MainLoop
	//result &= hook_write_jmpret(0x00416d20, LegoRR::Front_IsTriggerMissionRestart);

	// used by: Reward_GotoSaveMenu, Reward_GotoAdvance
	result &= hook_write_jmpret(0x00416d30, LegoRR::Front_Levels_GetTutoOrMissions);

	// used by: Front_LevelSet_LoadLevelLinks, Reward_GotoSaveMenu, Reward_GotoAdvance
	result &= hook_write_jmpret(0x00416d50, LegoRR::Front_LevelSet_IndexOf);

	// used by: Front_LoadLevels, Front_LevelLink_FindByLinkIndex, Front_Levels_UpdateAvailable,
	//          MainMenuFull_AddMissionsDisplay, Front_Save_ReadSaveFile, Front_Save_WriteSaveFiles
	result &= hook_write_jmpret(0x00416da0, LegoRR::Front_Levels_ResetVisited);

	// used by: Front_LevelSet_LoadLevelLinks
	result &= hook_write_jmpret(0x00416e00, LegoRR::Front_LevelSet_SetLinkVisited);

	// used by: Front_LevelSet_LoadLevelLinks
	result &= hook_write_jmpret(0x00416e70, LegoRR::Front_LevelSet_IsLinkVisited);

	// used by: Front_LevelSet_LoadLevelLinks
	result &= hook_write_jmpret(0x00416ee0, LegoRR::Front_LevelSet_SetLevelLink);

	// used by: Front_LevelSet_LoadLevelLinks
	result &= hook_write_jmpret(0x00416f50, LegoRR::Front_LevelSet_GetLevelLink);

	// used by: Front_LoadLevels
	result &= hook_write_jmpret(0x00416fc0, LegoRR::Front_LevelSet_LoadLevelLinks);

	// used by: Front_LevelLink_FindByLinkIndex, MainMenuFull_AddMissionsDisplay,
	//          Front_Save_ReadSaveFile, Front_Save_WriteSaveFiles
	result &= hook_write_jmpret(0x004170f0, LegoRR::Front_LevelLink_RunThroughLinks);

	// used by: MainMenuFull_AddMissionsDisplay, Front_Save_ReadSaveFile, Front_Save_WriteSaveFiles
	result &= hook_write_jmpret(0x00417170, LegoRR::Front_LevelLink_Callback_IncCount);

	// used by: Front_LevelLink_FindByLinkIndex
	result &= hook_write_jmpret(0x00417180, LegoRR::Front_LevelLink_Callback_FindByLinkIndex);

	// used by: Front_LevelLink_FindSetIndexOf, Front_Callback_SelectMissionItem,
	//          Front_Callback_SelectTutorialItem
	result &= hook_write_jmpret(0x004171b0, LegoRR::Front_LevelLink_FindByLinkIndex);

	// used by: Front_GetSelectedLevel
	result &= hook_write_jmpret(0x00417200, LegoRR::Front_LevelLink_FindSetIndexOf);

	// used by: Front_Levels_UpdateAvailable
	result &= hook_write_jmpret(0x00417220, LegoRR::Front_Levels_UpdateAvailable_Recursive);

	// used by: Front_Callback_SelectLoadSave, Front_Callback_TriggerBackSave,
	//          Front_PrepareScreenMenuType, Front_Save_WriteEmptySaveFiles, Reward_GotoAdvance
	result &= hook_write_jmpret(0x00417310, LegoRR::Front_Levels_UpdateAvailable);

	// used by: Front_Save_GetLevelCompleteWithPoints, Front_Callback_SelectMissionItem
	result &= hook_write_jmpret(0x00417360, LegoRR::Front_Save_GetLevelScore);

	// used by: Front_LoadLevels
	result &= hook_write_jmpret(0x00417390, LegoRR::Front_Callback_SelectMissionItem);

	// used by: Front_LoadLevels
	result &= hook_write_jmpret(0x00417630, LegoRR::Front_Callback_SelectTutorialItem);

	// used by: MainMenuFull_AddMissionsDisplay
	result &= hook_write_jmpret(0x00417710, LegoRR::Front_LevelInfo_Callback_AddItem);

	// used by: Front_LoadLevels
	result &= hook_write_jmpret(0x004178e0, LegoRR::MainMenuFull_AddMissionsDisplay);

	// used by: Front_LoadSaveSlotImages, Front_Save_LoadAllSaveFiles, Reward_GotoAdvance
	result &= hook_write_jmpret(0x004179c0, LegoRR::Front_Save_ReadSaveFile);

	// used by: Front_Save_ReadSaveFile, Front_Save_LoadAllSaveFiles, Front_Save_WriteCurrentSaveFiles,
	//          Front_Save_WriteEmptySaveFiles
	result &= hook_write_jmpret(0x00417b00, LegoRR::Front_Save_WriteSaveFiles);

	// used by: Front_LoadLevels
	result &= hook_write_jmpret(0x00417d20, LegoRR::Front_Save_LoadAllSaveFiles);

	// used by: Front_UpdateOptionsSliders, Front_Save_GetCurrentSaveData, Reward_GotoAdvance
	result &= hook_write_jmpret(0x00417d80, LegoRR::Front_Save_GetSaveDataAt);

	// used by: FrontEnd, Lego_LoadLevel, Reward_GotoSaveMenu, Reward_GotoAdvance
	result &= hook_write_jmpret(0x00417da0, LegoRR::Front_Save_GetCurrentSaveData);

	// used by: Front_UpdateOptionsSliders, Front_Save_WriteEmptySaveFiles, Lego_LoadLevel,
	//          Reward_GotoSaveMenu, Reward_GotoAdvance
	result &= hook_write_jmpret(0x00417dc0, LegoRR::Front_Save_GetSaveNumber);

	// used by: Front_Callback_SelectLoadSave, Reward_GotoSaveMenu, Reward_GotoAdvance
	result &= hook_write_jmpret(0x00417dd0, LegoRR::Front_Save_SetSaveNumber);

	// used by: Reward_GotoSaveMenu, Reward_GotoAdvance
	result &= hook_write_jmpret(0x00417de0, LegoRR::Front_Save_SetLevelCompleted);

	// used by: Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00417e50, LegoRR::Front_Save_SetSaveStruct18);

	// used by: Reward_GotoSaveMenu, Reward_GotoAdvance
	result &= hook_write_jmpret(0x00417e70, LegoRR::Front_Save_SetRewardLevel);

	// used by: Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00417ec0, LegoRR::Front_Save_GetRewardLevel);

	// used by: Reward_GotoSaveMenu, Reward_GotoAdvance
	result &= hook_write_jmpret(0x00417ef0, LegoRR::Front_Save_WriteCurrentSaveFiles);

	// used by: Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00417f10, LegoRR::Front_Save_GetHasNoSaveData);

	// used by: Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00417f20, LegoRR::Front_Save_SetHasNoSaveData);

	// used by: Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00417f30, LegoRR::Front_Save_WriteEmptySaveFiles);

	// used by: Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00417f70, LegoRR::Front_Save_CopySaveData);

	// used by: Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00417ff0, LegoRR::Front_Save_SetSaveData);

	// used by: Front_Initialise, Front_PrepareScreenMenuType, Reward_GotoAdvance
	result &= hook_write_jmpret(0x00418040, LegoRR::Front_Save_SetShouldClearUnlockedLevels);

	// used by: Reward_Show
	result &= hook_write_jmpret(0x00418050, LegoRR::Front_Save_IsGame100Percented);

	return_interop(result);
}

bool interop_hook_LegoRR_ElectricFence(void)
{
	bool result = true;

	// internal, no need to hook these
	// used by: ElectricFence_Restart
	//result &= hook_write_jmpret(0x0040ccf0, LegoRR::ElectricFence_Initialise);
	//result &= hook_write_jmpret(0x0040cd60, LegoRR::ElectricFence_Shutdown);

	// used by: Lego_LoadLevel, Lego_LoadMapSet
	result &= hook_write_jmpret(0x0040cdb0, LegoRR::ElectricFence_Restart);
	// used by: Construction_Zone_CompleteBuilding, Construction_CleanupBuildingFoundation,
	//          ElectricFence_Create, ElectricFence_Remove
	result &= hook_write_jmpret(0x0040cdd0, LegoRR::ElectricFence_UpdateConnectionEStuds);
	// used by: Lego_LoadOLObjectList, HiddenObject_ExposeBlock, LegoObject_SimpleObject_MoveAnimation
	result &= hook_write_jmpret(0x0040ce80, LegoRR::ElectricFence_CreateFence);
	// used by: ElectricFence_CreateFence, ElectricFence_Debug_PlaceFence
	result &= hook_write_jmpret(0x0040ceb0, LegoRR::ElectricFence_Create);
	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0040cf60, LegoRR::ElectricFence_AddList);
	// used by: ElectricFence_Debug_RemoveFence, LegoObject_UpdateRemoval, Message_Debug_DestroySelectedUnits
	result &= hook_write_jmpret(0x0040cfd0, LegoRR::ElectricFence_RemoveFence);
	// used by: ElectricFence_RemoveFence
	//result &= hook_write_jmpret(0x0040d030, LegoRR::ElectricFence_Remove);
	// used by: Lego_HandleWorldDebugKeys
	result &= hook_write_jmpret(0x0040d0a0, LegoRR::ElectricFence_Debug_PlaceFence);
	// used by: Lego_HandleWorldDebugKeys
	result &= hook_write_jmpret(0x0040d120, LegoRR::ElectricFence_Debug_RemoveFence);
	// used by: ElectricFence_Debug_PlaceFence, Interface_HandleMenuItem,
	//          LegoObject_TryElecFence_FUN_00448d20
	result &= hook_write_jmpret(0x0040d170, LegoRR::ElectricFence_CanPlaceFenceAtBlock);
	// used by: ElectricFence_CanPlaceFenceAtBlock, ElectricFence_ChainBeamsFromBuildingOrBlock,
	//          ElectricFence_Callback_ChainBeamsFromBuilding, ElectricFence_ChainBeams_Recurse,
	//          ElectricFence_TrySparkObject, ElectricFence_IsBlockBetweenConnection
	result &= hook_write_jmpret(0x0040d320, LegoRR::ElectricFence_HasBuilding);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0040d380, LegoRR::ElectricFence_UpdateAll);
	// used by: ElectricFence_UpdateAll, ElectricFence_ChainBeamsFromBuildingOrBlock
	result &= hook_write_jmpret(0x0040d3c0, LegoRR::ElectricFence_RunThroughLists);
	// used by: ElectricFence_TrySparkObject, Lego_HandleWorldDebugKeys
	result &= hook_write_jmpret(0x0040d420, LegoRR::ElectricFence_ChainBeamsFromBuildingOrBlock);
	// used by: ElectricFence_UpdateAll
	result &= hook_write_jmpret(0x0040d510, LegoRR::ElectricFence_Callback_ChainBeamsFromBuilding);
	// used by: ElectricFence_UpdateAll, ElectricFence_ChainBeamsFromBuildingOrBlock
	result &= hook_write_jmpret(0x0040d650, LegoRR::ElectricFence_Callback_ResetBuildingVisited);
	// used by: ElectricFence_UpdateAll, ElectricFence_ChainBeamsFromBuildingOrBlock
	result &= hook_write_jmpret(0x0040d6a0, LegoRR::ElectricFence_Callback_ResetVisitedAndUpdateTimer);
	// used by: ElectricFence_ChainBeamsFromBuildingOrBlock, ElectricFence_Callback_ChainBeamsFromBuilding
	result &= hook_write_jmpret(0x0040d780, LegoRR::ElectricFence_ChainBeams_Recurse);
	// used by: ElectricFence_ChainBeams_Recurse
	result &= hook_write_jmpret(0x0040db50, LegoRR::ElectricFence_TrySpawnBeamBetweenBlocks);
	// used by: ElectricFence_TrySpawnBeamBetweenBlocks
	result &= hook_write_jmpret(0x0040dcc0, LegoRR::ElectricFence_GetDirectionBetweenBlocks);
	// used by: LegoObject_UpdateEnergyHealthAndContact
	result &= hook_write_jmpret(0x0040dd70, LegoRR::ElectricFence_TrySparkObject);
	// used by: ElectricFence_TrySparkObject
	result &= hook_write_jmpret(0x0040dff0, LegoRR::ElectricFence_SparkObject);
	// used by: Level_BlockUpdateSurface
	result &= hook_write_jmpret(0x0040e110, LegoRR::ElectricFence_IsBlockBetweenConnection);
	// used by: Level_BlockUpdateSurface
	result &= hook_write_jmpret(0x0040e280, LegoRR::ElectricFence_AddOrRemoveEStud);
	// used by: Lego_HandleWorldDebugKeys, Level_CanBuildOnBlock
	result &= hook_write_jmpret(0x0040e390, LegoRR::ElectricFence_HasFence);

	return_interop(result);
}

bool interop_hook_LegoRR_Game(void)
{
	bool result = true;

	// used by: LegoObject_PutAwayCarriedObject, LegoObject_AddCrystalsStored, NERPFunc__AddPoweredCrystals
	result &= hook_write_jmpret(0x0041f7a0, LegoRR::Level_IncCrystalsStored);
	// used by: LegoObject_SpawnCarryableObject, LegoObject_UpdateTeleporter
	result &= hook_write_jmpret(0x0041f7b0, LegoRR::Level_SubtractCrystalsStored);
	// used by: LegoObject_AddThisDrainedCrystals
	result &= hook_write_jmpret(0x0041f7d0, LegoRR::Level_AddCrystalsDrained);
	// used by: LegoObject_RequestPowerGridUpdate
	result &= hook_write_jmpret(0x0041f7f0, LegoRR::Level_ResetCrystalsDrained);
	// used by: Interface_ChangeObjectIconFlag1_FUN_0041c730, LegoObject_AddThisDrainedCrystals,
	//          LegoObject_CanSpawnCarryableObject, LegoObject_UpdatePowerConsumption, LegoObject_UpdateTeleporter,
	//          NERPFunc__GetCrystalsCurrentlyStored, Weapon_LegoObject_UpdateSelectedTracker
	result &= hook_write_jmpret(0x0041f810, LegoRR::Level_GetCrystalCount);
	// used by: Interface_DoSomethingWithRenameReplace, LegoObject_GetBuildingUpgradeCost,
	//          LegoObject_HasEnoughOreToUpgrade, LegoObject_CanSpawnCarryableObject, LegoObject_Callback_Update
	result &= hook_write_jmpret(0x0041f830, LegoRR::Level_GetOreCount);
	// used by: LegoObject_FinishEnteringWallHole, LegoObject_UpdateEnergyHealthAndContact
	result &= hook_write_jmpret(0x0041f850, LegoRR::Level_AddStolenCrystals);
	// used by: Camera_Update
	result &= hook_write_jmpret(0x0041f870, LegoRR::Lego_SetRadarNoTrackObject);

	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x0041f8b0, LegoRR::Level_IncCrystalsPickedUp);
	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x0041f8c0, LegoRR::Level_IncOrePickedUp);
	// used by: LegoObject_PutAwayCarriedObject, LegoObject_AddOreStored, NERPFunc__AddStoredOre
	result &= hook_write_jmpret(0x0041f8d0, LegoRR::Level_IncOreStored);
	// used by: LegoObject_UpgradeBuilding, LegoObject_SpawnCarryableObject, LegoObject_Callback_Update
	result &= hook_write_jmpret(0x0041f910, LegoRR::Level_SubtractOreStored);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0041f9b0, LegoRR::Lego_QuitLevel);

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00422780, LegoRR::Lego_LoadMiscObjects);
	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x00422fb0, LegoRR::Lego_GetCurrentCamera_Container);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00422ff0, LegoRR::Lego_DrawRenameInput);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00423120, LegoRR::Lego_HandleRenameInput);

	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x00424490, LegoRR::Level_ConsumeObjectOxygen);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00424530, LegoRR::Level_UpdateEffects);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00424660, LegoRR::Lego_UpdateSceneFog);
	// used by: Lego_DrawAllLaserTrackerBoxes
	result &= hook_write_jmpret(0x00424700, LegoRR::Lego_Callback_DrawObjectLaserTrackerBox);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00424740, LegoRR::Lego_DrawAllLaserTrackerBoxes);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00424760, LegoRR::Lego_DrawAllSelectedUnitBoxes);
	// used by: Encyclopedia_DrawSelectBox, Lego_Callback_DrawObjectLaserTrackerBox, Lego_DrawAllSelectedUnitBoxes
	result &= hook_write_jmpret(0x004247e0, LegoRR::Lego_DrawObjectSelectionBox);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00424ff0, LegoRR::Lego_HandleKeys);
	// used by: Lego_SetPaused, LegoObject_UpdateAll
	result &= hook_write_jmpret(0x00425a70, LegoRR::Lego_UpdateAll3DSounds);
	// used by: Lego_UpdateAll3DSounds
	result &= hook_write_jmpret(0x00425a90, LegoRR::Lego_Callback_UpdateObject3DSounds);
	// used by: FrontEnd, HelpWindow, Lego, NERPs, Objective, Panels
	result &= hook_write_jmpret(0x00425b60, LegoRR::Lego_SetPaused);

	// used by: Info_Send, Lego_Initialise, Lego_MainLoop, Lego_HandleKeys,
	//          Level_Free, NERPFunc__SetGameSpeed
	result &= hook_write_jmpret(0x00425c10, LegoRR::Lego_SetGameSpeed);
	// used by: Interface_DoAction_FUN_0041dbd0, Message_Update
	result &= hook_write_jmpret(0x00425c80, LegoRR::Lego_TrackObjectInRadar);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00426180, LegoRR::Lego_DrawRadarMap);

	// Restore debug tooltip support.
	// used by: Lego_HandleWorld
	result &= hook_write_jmpret(0x00427f50, LegoRR::Lego_ShowObjectToolTip);
	// used by: Lego_HandleRadarInput, Lego_HandleWorld
	result &= hook_write_jmpret(0x00428260, LegoRR::Lego_ShowBlockToolTip);

	// used by: Lego_HandleWorld
	result &= hook_write_jmpret(0x00428810, LegoRR::Lego_HandleWorldDebugKeys);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x004292e0, LegoRR::Lego_DrawDragSelectionBox);

	// used by: AITask_Callback_UpdateObject, LegoObject_TryRunAway
	result &= hook_write_jmpret(0x004294f0, LegoRR::Lego_IsFPObject);
	// used by: Lego_LoadLevel, Lego_EndLevel, Message_Update, Message_RemoveObjectReference, Message_EnterFirstPersonView
	result &= hook_write_jmpret(0x00429520, LegoRR::Lego_SetViewMode);
	// used by: Lego_SetMusicPlaying
	result &= hook_write_jmpret(0x004296d0, LegoRR::Lego_CDTrackPlayNextCallback);
	// used by: Front_Callback_CycleMusic, Lego_MainLoop, Lego_Shutdown_Full, Lego_Exit, Lego_HandleKeys,
	//          Lego_CDTrackPlayNextCallback, Objective_StopShowing, Objective_Update
	result &= hook_write_jmpret(0x004296e0, LegoRR::Lego_SetMusicPlaying);
	// used by: Front_Callback_CycleSound, Lego_Initialise, Lego_Shutdown_Full, Lego_HandleKeys
	result &= hook_write_jmpret(0x00429740, LegoRR::Lego_SetSoundOn);

	// used by: Lego_LoadMapSet
	result &= hook_write_jmpret(0x0042be70, LegoRR::Lego_LoadErodeMap);

	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x0042c260, LegoRR::Level_HandleEmergeTriggers);

	// used by: Lego_LoadMapSet
	result &= hook_write_jmpret(0x0042c3b0, LegoRR::Lego_LoadTerrainMap);

	// used by: Lego_LoadMapSet
	result &= hook_write_jmpret(0x0042c900, LegoRR::Lego_LoadFallinMap);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0042caa0, LegoRR::Lego_UpdateFallins);

	// used by: Front_RestartLevel, Lego_Shutdown_Full, Lego_EndLevel
	result &= hook_write_jmpret(0x0042eff0, LegoRR::Level_Free);

	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x00431020, LegoRR::Level_Block_RemoveReinforcement);
	// used by: LegoObject_Update_Reinforcing
	result &= hook_write_jmpret(0x00431070, LegoRR::Level_Block_Reinforce);
	// used by: Lego_LoadTerrainMap, Level_Block_Reinforce
	result &= hook_write_jmpret(0x00431100, LegoRR::Level_BlockActivity_Create);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x004312e0, LegoRR::Level_BlockActivity_UpdateAll);
	// used by: Level_BlockUpdateSurface, Level_DestroyWall, Level_Block_RemoveReinforcement
	result &= hook_write_jmpret(0x00431380, LegoRR::Level_BlockActivity_Destroy);
	// used by: Level_BlockActivity_UpdateAll, Level_BlockActivity_RemoveAll
	result &= hook_write_jmpret(0x004313f0, LegoRR::Level_BlockActivity_Remove);
	// used by: Level_Free
	result &= hook_write_jmpret(0x00431460, LegoRR::Level_BlockActivity_RemoveAll);

	// used by: Message_Update
	result &= hook_write_jmpret(0x004316b0, LegoRR::Lego_PTL_RockFall);

	// used by: SelectPlace_CheckAndUpdate
	result &= hook_write_jmpret(0x00431a50, LegoRR::Level_CanBuildOnBlock);

	// used by: Construction_PowerGrid_PowerAdjacentBlocks
	result &= hook_write_jmpret(0x00432030, LegoRR::Level_PowerGrid_AddPoweredBlock);
	// used by: Construction_PowerGrid_PowerAdjacentBlocks, ElectricFence_HasBuilding,
	//          RadarMap_GetBlockColour
	result &= hook_write_jmpret(0x004320a0, LegoRR::Level_Block_IsPowered);
	// used by: LegoObject_UpdateAll
	result &= hook_write_jmpret(0x004320d0, LegoRR::Level_PowerGrid_UpdateUnpoweredBlockSurfaces);
	// used by: LegoObject_UpdateAll
	result &= hook_write_jmpret(0x00432130, LegoRR::Level_PowerGrid_UnpowerPoweredBlocks);
	// used by: Construction_PowerGrid_DrainAdjacentBlocks_Recurse
	result &= hook_write_jmpret(0x004321a0, LegoRR::Level_PowerGrid_AddDrainPowerBlock);
	// used by: Construction_PowerGrid_DrainAdjacentBlocks_Recurse
	result &= hook_write_jmpret(0x00432200, LegoRR::Level_PowerGrid_IsDrainPowerBlock);
	// used by: Construction_PowerGrid_DrainAdjacentBlocks
	result &= hook_write_jmpret(0x00432230, LegoRR::Level_PowerGrid_ClearDrainPowerBlocks);

	// used by: LegoObject_UpdateWorldStickyPosition
	result &= hook_write_jmpret(0x00432a30, LegoRR::Level_Block_IsRockFallFX);

	// used by: Fallin_CanLandSlideAtBlock
	result &= hook_write_jmpret(0x00432dc0, LegoRR::Level_Block_IsInitiallyExposed);

	// used by: LegoObject_AttackPath
	result &= hook_write_jmpret(0x00435160, LegoRR::Level_GenerateLandSlideNearBlock);

	// used by: Lego_MainLoop, Lego_HandleKeys, Objective_HandleKeys
	result &= hook_write_jmpret(0x00435870, LegoRR::Lego_EndLevel);

	return_interop(result);
}

bool interop_hook_LegoRR_InfoMessages(void)
{
	bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00419160, LegoRR::Info_Initialise);
	// used by: Info_SetText_internal, Lego_LoadInfoMessages
	result &= hook_write_jmpret(0x00419310, LegoRR::Info_GetInfoType);
	// used by: Info_SetText_internal
	result &= hook_write_jmpret(0x00419350, LegoRR::Info_GetTypePtr4);
	// used by: Info_Initialise
	result &= hook_write_jmpret(0x00419360, LegoRR::Info_SetOverFlowImageFile);
	// used by: Front_Callback_CycleAutoGameSpeed
	result &= hook_write_jmpret(0x00419380, LegoRR::Info_SetAutoGameSpeed);
	// used by: Lego_LoadInfoMessages
	result &= hook_write_jmpret(0x004193a0, LegoRR::Info_SetTypeChangeGameSpeed);
	// used by: Lego_LoadInfoMessages
	result &= hook_write_jmpret(0x004193e0, LegoRR::Info_SetTypeText);
	// used by: Lego_LoadInfoMessages
	result &= hook_write_jmpret(0x00419420, LegoRR::Info_SetTypeFlag_20000);
	// used by: Info_SetTypeText, Info_CreateInstance
	result &= hook_write_jmpret(0x00419460, LegoRR::Info_SetText_internal);
	// used by: Lego_LoadInfoMessages
	result &= hook_write_jmpret(0x00419580, LegoRR::Info_SetTypeImageFile);
	// used by: Lego_LoadInfoMessages
	result &= hook_write_jmpret(0x004195b0, LegoRR::Info_SetTypeSFX);
	// used by: Info_RemoveObjectReferences, Info_RemoveAllAtBlockPos,
	//          Info_HasTypeAtObjectOrBlockPos
	result &= hook_write_jmpret(0x004195d0, LegoRR::Info_EnumerateMessageInstances);
	// used by: Info_Send
	result &= hook_write_jmpret(0x00419620, LegoRR::Info_AddMessageInstance);
	// used by: Info_RemoveMessageInstance
	result &= hook_write_jmpret(0x004196b0, LegoRR::Info_GetMessageInstance);
	// used by: Info_AddMessageInstance, Info_RemoveObjectReferences,
	//          Info_RemoveAllAtBlockPos, Info_RemoveMessage
	result &= hook_write_jmpret(0x004196e0, LegoRR::Info_RemoveMessageInstance);
	// used by: Info_RemoveObjectReferences
	result &= hook_write_jmpret(0x00419740, LegoRR::Info_Callback_FindObjectReference);
	// used by: LegoObject_Remove
	result &= hook_write_jmpret(0x00419760, LegoRR::Info_RemoveObjectReferences);
	// used by: Info_RemoveAllAtBlockPos
	result &= hook_write_jmpret(0x004197f0, LegoRR::Info_Callback_FindBlockPos);
	// used by: Level_Block_ClearRubbleLayer
	result &= hook_write_jmpret(0x00419820, LegoRR::Info_RemoveAllAtBlockPos);
	// used by: Info_HasTypeAtObjectOrBlockPos
	result &= hook_write_jmpret(0x004198d0, LegoRR::Info_Callback_FindObjectAndBlockPos);
	// used by: Info_Send
	result &= hook_write_jmpret(0x00419920, LegoRR::Info_HasTypeAtObjectOrBlockPos);
	// used by: Info_Send
	result &= hook_write_jmpret(0x004199b0, LegoRR::Info_CreateInstance);
	// used by: Lego_LoadLevel, Level_Free
	result &= hook_write_jmpret(0x00419a10, LegoRR::Info_SetFlag4);
	// used by: Info_Send
	result &= hook_write_jmpret(0x00419a30, LegoRR::Info_HasTypeText);
	// used by: Info_Send
	result &= hook_write_jmpret(0x00419a50, LegoRR::Info_FindExistingMessageType);
	// used by: Info_Send
	result &= hook_write_jmpret(0x00419a80, LegoRR::Info_FUN_00419a80);
	// used by: Construction_Zone_CompleteBuilding, Erode_Update, Fallin_Update,
	//          Fallin_Block_FUN_0040f260, Lego_MainLoop, Lego_HandleWorldDebugKeys,
	//          Lego_UpdateFallins, Level_DestroyWall, Level_Block_Reinforce,
	//          HiddenObject_ExposeBlock, LegoObject_AddThisDrainedCrystals, Level_GenerateCrystal,
	//          LegoObject_TryGenerateSlug, LegoObject_TryGenerateRMonster, LegoObject_UpdateRemoval,
	//          LegoObject_UpdateElapsedTimes, LegoObject_Callback_Update, LegoObject_ProccessCarriedObjects,
	//          LegoObject_TrainMiniFigure_instantunk, LegoObject_FUN_00447880, LegoObject_SimpleObject_MoveAnimation
	result &= hook_write_jmpret(0x00419ab0, LegoRR::Info_Send);
	// used by: Panel_CheckCollision
	result &= hook_write_jmpret(0x00419cd0, LegoRR::Info_GotoFirst);
	// used by: Info_Update_FUN_0041a0d0, Panel_CheckCollision
	result &= hook_write_jmpret(0x00419d10, LegoRR::Info_UpdateMessage);
	// used by: Panel_CheckCollision
	result &= hook_write_jmpret(0x00419d90, LegoRR::Info_PopFirstMessage);
	// used by: Info_ClearAllMessages, Info_PopFirstMessage, Info_RemoveAllAtBlockPos, Info_RemoveObjectReferences
	result &= hook_write_jmpret(0x00419db0, LegoRR::Info_RemoveMessage);
	// used by: Level_Free
	result &= hook_write_jmpret(0x00419e40, LegoRR::Info_ClearAllMessages);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00419e60, LegoRR::Info_Draw);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00419fb0, LegoRR::Info_DrawPanel);
	// used by: Lego_HandleWorld
	result &= hook_write_jmpret(0x0041a0d0, LegoRR::Info_Update_FUN_0041a0d0);
	// used by: Info_DrawPanel
	result &= hook_write_jmpret(0x0041a180, LegoRR::Info_FUN_0041a180);
	// used by: Info_FUN_0041a180, Panel_CheckCollision
	result &= hook_write_jmpret(0x0041a1c0, LegoRR::Info_UpdateInt6EC_FromScrollInfo);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0041a1f0, LegoRR::Info_FUN_0041a1f0);

	return_interop(result);
}

bool interop_hook_LegoRR_Interface(void)
{
	bool result = true;

	// used by: Lego_HandleKeys
	result &= hook_write_jmpret(0x0041c370, LegoRR::Interface_DoF2InterfaceKeyAction);
	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0041c3a0, LegoRR::Interface_CallbackDoMenuIconKeyAction);

	// used by: Interface_FUN_0041b3c0, Priorities_Draw
	result &= hook_write_jmpret(0x0041cc60, LegoRR::Interface_DrawHoverOutline);

	return_interop(result);
}

bool interop_hook_LegoRR_LegoCamera(void)
{
	bool result = true;
	
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00435a50, LegoRR::Camera_Create);
	
	// used by: Lego_HandleKeys
	// INTEROP HOOK REPLACEMENT TO ALLOW TOGGLING FREE MOVEMENT MODE.
	result &= hook_write_jmpret(0x00435cc1, LegoRR::Camera_ToggleFreeMovement);
	//result &= hook_write_jmpret(0x00435cc1, LegoRR::Camera_EnableFreeMovement);
	
	// used by: Lego_Shutdown_Full
	result &= hook_write_jmpret(0x00435cf8, LegoRR::Camera_Free);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00435d3e, LegoRR::Camera_InitCameraMovements);

	// (unused)
	//result &= hook_write_jmpret(0x00435d65, LegoRR::Camera_CopyFPPositionOrientation);

	result &= hook_write_jmpret(0x00435deb, LegoRR::Camera_TrackObject);
	result &= hook_write_jmpret(0x00435e24, LegoRR::Camera_SetFPObject);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00435e3b, LegoRR::Camera_GetFPCameraFrame);
	// used by: Game_UpdateTopdownCamera
	result &= hook_write_jmpret(0x00435e46, LegoRR::Camera_StopMovement);

	result &= hook_write_jmpret(0x00435e58, LegoRR::Camera_GetMouseScrollIndent);
	result &= hook_write_jmpret(0x00435e62, LegoRR::Camera_Shake);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00435e8c, LegoRR::Camera_Update);

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00436a53, LegoRR::Camera_SetTiltRange);
	result &= hook_write_jmpret(0x00436a82, LegoRR::Camera_SetTilt);
	result &= hook_write_jmpret(0x00436b22, LegoRR::Camera_AddTilt);

	// (unused)
	//result &= hook_write_jmpret(0x00436b43, LegoRR::Camera_SetRotationRange);
	
	result &= hook_write_jmpret(0x00436b75, LegoRR::Camera_SetRotation);
	result &= hook_write_jmpret(0x00436c16, LegoRR::Camera_AddRotation);

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00436c3a, LegoRR::Camera_SetZoomRange);
	
	result &= hook_write_jmpret(0x00436c6c, LegoRR::Camera_SetZoom);
	result &= hook_write_jmpret(0x00436cc7, LegoRR::Camera_AddZoom);

	// (unused)
	//result &= hook_write_jmpret(0x00436ceb, LegoRR::Camera_AddTranslation2D);
	result &= hook_write_jmpret(0x00436d0b, LegoRR::Camera_GetTopdownPosition);
	result &= hook_write_jmpret(0x00436d2d, LegoRR::Camera_SetTopdownPosition);
	result &= hook_write_jmpret(0x00436d55, LegoRR::Camera_GetTopdownWorldPos);

	// (unused)
	//result &= hook_write_jmpret(0x00436d9b, LegoRR::Camera_GetRotation);
	
	// used by: Game_UpdateTopdownCamera
	result &= hook_write_jmpret(0x00436da9, LegoRR::Camera_Move);
	
	return_interop(result);
}

bool interop_hook_LegoRR_LightEffects(void)
{
	bool result = true;

	// used by: Lego_LoadLighting
	result &= hook_write_jmpret(0x0044c9d0, LegoRR::LightEffects_Initialise);
	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0044ca20, LegoRR::LightEffects_ResetSpotlightColour);
	// used by: Lego_HandleWorldDebugKeys
	result &= hook_write_jmpret(0x0044ca50, LegoRR::LightEffects_SetDisabled);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x0044ca80, LegoRR::LightEffects_Load);
	// used by: LightEffects_Load
	result &= hook_write_jmpret(0x0044cab0, LegoRR::LightEffects_LoadBlink);
	// used by: LightEffects_LoadBlink
	result &= hook_write_jmpret(0x0044cc30, LegoRR::LightEffects_SetBlink);
	// used by: LightEffects_Load
	result &= hook_write_jmpret(0x0044cc80, LegoRR::LightEffects_LoadFade);
	// used by: LightEffects_LoadFade
	result &= hook_write_jmpret(0x0044ced0, LegoRR::LightEffects_SetFade);
	// used by: LightEffects_Load
	result &= hook_write_jmpret(0x0044cf60, LegoRR::LightEffects_LoadMove);
	// used by: LightEffects_LoadMove
	result &= hook_write_jmpret(0x0044d1b0, LegoRR::LightEffects_SetMove);
	// used by: Lego_HandleWorldDebugKeys
	result &= hook_write_jmpret(0x0044d230, LegoRR::LightEffects_InvalidatePosition);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0044d260, LegoRR::LightEffects_Update);
	// used by: LightEffects_Update
	result &= hook_write_jmpret(0x0044d2b0, LegoRR::LightEffects_UpdateSpotlightColour);
	// used by: LightEffects_Update
	result &= hook_write_jmpret(0x0044d390, LegoRR::LightEffects_UpdateBlink);
	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0044d510, LegoRR::LightEffects_FlipSign);
	// used by: LightEffects_Update
	result &= hook_write_jmpret(0x0044d540, LegoRR::LightEffects_UpdateFade);
	// used by: LightEffects_UpdateFade
	result &= hook_write_jmpret(0x0044d9d0, LegoRR::LightEffects_RandomizeFadeSpeedRGB);
	// used by: LightEffects_Update
	result &= hook_write_jmpret(0x0044da20, LegoRR::LightEffects_UpdateMove);
	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0044dc60, LegoRR::LightEffects_CheckMoveLimit);
	// used by: Lego_HandleWorld
	result &= hook_write_jmpret(0x0044dce0, LegoRR::LightEffects_SetDimmerMode);
	// used by: LightEffects_Update
	result &= hook_write_jmpret(0x0044dd10, LegoRR::LightEffects_UpdateDimmer);

	return_interop(result);
}

bool interop_hook_LegoRR_Loader(void)
{
	bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x0044de10, LegoRR::Loader_Initialise);
	// used by: Lego_Shutdown_Full
	result &= hook_write_jmpret(0x0044e000, LegoRR::Loader_Shutdown);
	// used by: Lego_Initialise, Lego_LoadLevel
	result &= hook_write_jmpret(0x0044e0a0, LegoRR::Loader_display_loading_bar);
	// used by: Loader_Initialise, Loader_display_loading_bar
	result &= hook_write_jmpret(0x0044e180, LegoRR::Loader_FileLoadCallback);
	// used by: Lego_Shutdown_Full
	result &= hook_write_jmpret(0x0044e360, LegoRR::Loader_display_shutdown);

	return_interop(result);
}


bool interop_hook_LegoRR_MeshLOD(void)
{
	bool result = true;

	// used by: LegoObject_LoadMeshLOD
	result &= hook_write_jmpret(0x00451c70, LegoRR::MeshLOD_Create);
	// used by: LegoObject_LoadMeshLOD
	result &= hook_write_jmpret(0x00451d70, LegoRR::MeshLOD_CreateEmpty);
	// used by: Creature_Clone, Vehicle_Clone
	result &= hook_write_jmpret(0x00451df0, LegoRR::MeshLOD_Clone);
	// used by: Creature_SwapPolyMedium, Creature_SwapPolyHigh, Creature_SwapPolyFP, Vehicle_SwapPolyMedium
	result &= hook_write_jmpret(0x00451e80, LegoRR::MeshLOD_SwapTarget);
	// used by: Creature_SetActivity, Vehicle_SetActivity
	result &= hook_write_jmpret(0x00451ef0, LegoRR::MeshLOD_RemoveTargets);
	// used by: Creature_Remove
	result &= hook_write_jmpret(0x00451f10, LegoRR::MeshLOD_Free);

	return_interop(result);
}

bool interop_hook_LegoRR_Messages(void)
{
	bool result = true;
	
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00451f90, LegoRR::Message_Initialise);

	// used by: Message_RemoveObjectReference
	result &= hook_write_jmpret(0x00452220, LegoRR::Message_RemoveEventsWithObject);

	// used by: PTL_Initialise
	result &= hook_write_jmpret(0x00452290, LegoRR::Message_ParsePTLName);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x004522d0, LegoRR::Message_RegisterHotKeyEvent);

	// used by: AITask, Construction_Zone_PlaceResource, Interface, Lego, LegoObject, Message, Panel_CheckCollision
	result &= hook_write_jmpret(0x00452320, LegoRR::Message_PostEvent);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00452390, LegoRR::Message_Update);
	// used by: Message_Update
	result &= hook_write_jmpret(0x004526f0, LegoRR::Message_PickRandomFloorBlock);
	// used by: LegoObject_Remove, LegoObject_UpdateRemoval
	result &= hook_write_jmpret(0x00452770, LegoRR::Message_RemoveObjectReference);

	result &= hook_write_jmpret(0x004527e0, LegoRR::Message_CopySelectedUnits);
	result &= hook_write_jmpret(0x00452840, LegoRR::Message_GetSelectedUnits2);
	result &= hook_write_jmpret(0x00452870, LegoRR::Message_AnyUnitSelected);
	result &= hook_write_jmpret(0x00452880, LegoRR::Message_GetPrimarySelectedUnit);
	result &= hook_write_jmpret(0x004528a0, LegoRR::Message_GetSelectedUnits);
	result &= hook_write_jmpret(0x004528b0, LegoRR::Message_GetNumSelectedUnits);

	// used by: Level_Free
	result &= hook_write_jmpret(0x004528c0, LegoRR::Message_CleanupSelectedUnitsCount);

	// used by: AITask, Interface_BackToMain_IfUnitIsSelected, Lego_HandleWorld, Message, NERPFunc__GetSelectedRecordObject
	result &= hook_write_jmpret(0x004528d0, LegoRR::Message_IsUnitSelected);

	result &= hook_write_jmpret(0x00452910, LegoRR::Message_FindIndexOfObject);

	// used by: Message_SelectObject2
	result &= hook_write_jmpret(0x00452950, LegoRR::Message_IsObjectDoubleSelectable);
	// used by: Message_Update
	result &= hook_write_jmpret(0x00452980, LegoRR::Message_SelectObject);
	// used by: Message_SelectObject
	result &= hook_write_jmpret(0x004529a0, LegoRR::Message_SelectObject2);
	// used by: Message_SelectObject2
	result &= hook_write_jmpret(0x00452b30, LegoRR::Message_IsObjectSelectable);
	// used by: Message_Update
	result &= hook_write_jmpret(0x00452b80, LegoRR::Message_ReduceSelectedUnits);
	// used by: Interface_DoAction_FUN_0041dbd0, Message_Update
	result &= hook_write_jmpret(0x00452ea0, LegoRR::Message_ClearSelectedUnits);
	// used by: LegoObject_UpdateRemoval, LegoObject_FUN_00440470, Message_Update, Message_RemoveObjectReference
	result &= hook_write_jmpret(0x00452f10, LegoRR::Message_DeselectObject);
	// used by: Message_Update
	result &= hook_write_jmpret(0x00452f80, LegoRR::Message_Debug_DestroySelectedUnits);
	// used by: Message_Update
	result &= hook_write_jmpret(0x00453020, LegoRR::Message_EnterFirstPersonView);

	return_interop(result);
}

bool interop_hook_LegoRR_NERPsFile(void)
{
	bool result = true;

	// NERPs interpreter functions (except for GetMessageLine)

	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x004530b0, LegoRR::NERPsFile_LoadScriptFile);
	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x00453130, LegoRR::NERPsFile_LoadMessageFile);
	// used by: NERPsRuntime_AdvanceMessage, NERPFunc__SetMessage
	result &= hook_write_jmpret(0x004534c0, LegoRR::NERPsFile_GetMessageLine);
	// used by: Level_Free
	result &= hook_write_jmpret(0x004534e0, LegoRR::NERPsFile_Free);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004535a0, LegoRR::NERPsRuntime_LoadLiteral);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x004535e0, LegoRR::NERPsRuntime_Execute);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00453bc0, LegoRR::NERPs_SetHasNextButton);
	// used by: Lego_HandleWorld
	result &= hook_write_jmpret(0x00453bd0, LegoRR::NERPsRuntime_RepeatMessage);
	// used by: Lego_HandleWorld, NERPFunc__AdvanceMessage
	result &= hook_write_jmpret(0x00453be0, LegoRR::NERPsRuntime_AdvanceMessage);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00453e70, LegoRR::NERPsRuntime_UpdateTimers);
	// used by: NERPsRuntime_Execute
	result &= hook_write_jmpret(0x00454060, LegoRR::NERPsRuntime_EndExecute);

	// used by: NERPFunc__GetRockMonsterRunningAway, NERPFunc__SetRockMonsterPainThreshold,
	//          NERPFunc__SetRockMonsterHealth
	result &= hook_write_jmpret(0x00454c70, LegoRR::NERPs_LiveObject_Callback_SetRockMonsterHealthType);


	// used by: Text_SetNERPsMessage, Text_Update
	result &= hook_write_jmpret(0x00456af0, LegoRR::NERPs_Level_NERPMessage_Parse);

	// used by: NERPsFunctions, NERPsRuntime_SetTutorialPointer, c_NERPsRuntime_TutorialActionCallback
	result &= hook_write_jmpret(0x00456fc0, LegoRR::NERPsRuntime_TutorialActionCallback);


	return_interop(result);
}

bool interop_hook_LegoRR_NERPsFunctions(void)
{
	bool result = true;

	/// WARNING: ABSOLUTELY DO NOT EVER HOOK THESE FUNCTIONS DIRECTLY!!
	// NERPFunc__False
	// NERPFunc__True

	// The following NERPs functions are used in places other than just the c_nerpsFunctions array.

	// used by: NERPsRuntime_AdvanceMessage, NERPsRuntime_EndExecute
	result &= hook_write_jmpret(0x00453b60, LegoRR::NERPFunc__SetGameSpeed);

	// used by: Lego_HandleKeys
	result &= hook_write_jmpret(0x00454e30, LegoRR::NERPFunc__SetLevelCompleted);

	// used by: Lego_HandleKeys
	result &= hook_write_jmpret(0x00454e40, LegoRR::NERPFunc__SetLevelFail);

	// used by: AITask, Front, HelpWindow, Info, Interface, Lego, LegoObject, NERPs, Objective, Panel, Text
	result &= hook_write_jmpret(0x00456500, LegoRR::NERPFunc__GetTutorialFlags);

	// used by: Lego_HandleWorldDebugKeys, NERPFunc__ClickOnlyObjects, NERPFunc__ClickOnlyMap,
	//          NERPFunc__ClickOnlyIcon, NERPFunc__ClickOnlyCalltoarms, NERPFunc__DisallowAll
	result &= hook_write_jmpret(0x00456810, LegoRR::NERPFunc__SetTutorialFlags);

	// used by: Lego_UnkObjective_CompleteSub_FUN_004262f0, NERPsFile_LoadScriptFile
	result &= hook_write_jmpret(0x004568b0, LegoRR::NERPFunc__SetMessagePermit);

	// used by: Objective_StopShowing
	result &= hook_write_jmpret(0x00456a80, LegoRR::NERPFunc__SetObjectiveSwitch);


	// The remaining NERPs functions can be hooked by writing to the c_nerpsFunctions array instead.

	#pragma region NERPs_hook_function
	// Also hooked directly.
	result &= NERPs_hook_function(SetGameSpeed);
	
	result &= NERPs_hook_function(GetMessagesAreUpToDate);
	result &= NERPs_hook_function(SupressArrow);
	result &= NERPs_hook_function(AdvanceMessage);
	result &= NERPs_hook_function(AllowCameraMovement);
	result &= NERPs_hook_function(ClickOnlyObjects);
	result &= NERPs_hook_function(ClickOnlyMap);
	result &= NERPs_hook_function(ClickOnlyIcon);
	result &= NERPs_hook_function(ClickOnlyCalltoarms);
	result &= NERPs_hook_function(DisallowAll);
	result &= NERPs_hook_function(FlashCallToArmsIcon);

	result &= NERPs_hook_function(GetTimer0);
	result &= NERPs_hook_function(GetTimer1);
	result &= NERPs_hook_function(GetTimer2);
	result &= NERPs_hook_function(GetTimer3);
	result &= NERPs_hook_function(SetTimer0);
	result &= NERPs_hook_function(SetTimer1);
	result &= NERPs_hook_function(SetTimer2);
	result &= NERPs_hook_function(SetTimer3);

	result &= NERPs_hook_function(CameraLockOnMonster);

	result &= NERPs_hook_function(CameraLockOnObject);
	result &= NERPs_hook_function(CameraUnlock);
	result &= NERPs_hook_function(CameraZoomIn);
	result &= NERPs_hook_function(CameraZoomOut);
	result &= NERPs_hook_function(CameraRotate);
	result &= NERPs_hook_function(GetSelectedRecordObject);
	result &= NERPs_hook_function(SetCrystalPriority);
	result &= NERPs_hook_function(SetMonsterAttackPowerstation);

	result &= NERPs_hook_function(SetMonsterAttackNowt);

	result &= NERPs_hook_function(GetRecordObjectAtTutorial);
	result &= NERPs_hook_function(GetRecordObjectAmountAtTutorial);

	result &= NERPs_hook_function(SetRecordObjectPointer);

	result &= NERPs_hook_function(GetOxygenLevel);
	result &= NERPs_hook_function(GenerateSlug);
	result &= NERPs_hook_function(SetAttackDefer);
	result &= NERPs_hook_function(SetCallToArms);
	result &= NERPs_hook_function(GetCallToArmsButtonClicked);
	result &= NERPs_hook_function(SetRockMonster);
	result &= NERPs_hook_function(GetRockMonstersDestroyed);

	result &= NERPs_hook_function(GetHiddenObjectsFound);
	result &= NERPs_hook_function(SetHiddenObjectsFound);
	result &= NERPs_hook_function(SetUpgradeBuildingIconClicked);
	result &= NERPs_hook_function(GetUpgradeBuildingIconClicked);
	result &= NERPs_hook_function(FlashUpgradeBuildingIcon);
	result &= NERPs_hook_function(SetGoBackIconClicked);
	result &= NERPs_hook_function(GetGoBackIconClicked);
	result &= NERPs_hook_function(FlashGoBackIcon);
	result &= NERPs_hook_function(GetRockMonsterRunningAway);

	result &= NERPs_hook_function(SetRockMonsterPainThreshold);
	result &= NERPs_hook_function(SetRockMonsterHealth);
	result &= NERPs_hook_function(SetPauseGame);
	result &= NERPs_hook_function(GetAnyKeyPressed);
	result &= NERPs_hook_function(SetIconPos);
	result &= NERPs_hook_function(SetIconSpace);
	result &= NERPs_hook_function(SetIconWidth);
	// Also hooked directly.
	result &= NERPs_hook_function(SetLevelCompleted);
	result &= NERPs_hook_function(SetGameCompleted);
	// Also hooked directly.
	result &= NERPs_hook_function(SetLevelFail);
	result &= NERPs_hook_function(SetGameFail);
	result &= NERPs_hook_function(SetTutorialPointer);

	result &= NERPs_hook_function(SetTutorialBlockClicks);
	
	result &= NERPs_hook_function(SetTutorialCrystals);
	
	result &= NERPs_hook_function(SetOreAtIconPositions);

	result &= NERPs_hook_function(GetMiniFigureSelected);
	result &= NERPs_hook_function(GetSmallTruckSelected);
	result &= NERPs_hook_function(GetSmallDiggerSelected);
	result &= NERPs_hook_function(GetRapidRiderSelected);
	result &= NERPs_hook_function(GetSmallHelicopterSelected);
	result &= NERPs_hook_function(GetGraniteGrinderSelected);
	result &= NERPs_hook_function(GetChromeCrusherSelected);

	result &= NERPs_hook_function(AddPoweredCrystals);
	result &= NERPs_hook_function(AddStoredOre);
	
	result &= NERPs_hook_function(GetTutorialCrystals);
	
	result &= NERPs_hook_function(GetTutorialBlockClicks);

	result &= NERPs_hook_function(GetMiniFigureinGraniteGrinder);
	result &= NERPs_hook_function(GetMiniFigureinChromeCrusher);
	result &= NERPs_hook_function(GetMiniFigureinSmallDigger);
	result &= NERPs_hook_function(GetMiniFigureinRapidRider);
	result &= NERPs_hook_function(GetMiniFigureinSmallTruck);
	result &= NERPs_hook_function(GetMiniFigureinSmallHelicopter);
	result &= NERPs_hook_function(SetBarracksLevel);
	result &= NERPs_hook_function(SetDocksLevel);
	result &= NERPs_hook_function(SetGeoDomeLevel);
	result &= NERPs_hook_function(SetPowerstationLevel);
	result &= NERPs_hook_function(SetToolStoreLevel);
	result &= NERPs_hook_function(SetGunstationLevel);
	result &= NERPs_hook_function(SetTeleportPadLevel);
	result &= NERPs_hook_function(SetSuperTeleportLevel);
	result &= NERPs_hook_function(SetUpgradeStationLevel);
	result &= NERPs_hook_function(GetBarracksSelected);
	result &= NERPs_hook_function(GetDocksSelected);
	result &= NERPs_hook_function(GetGeoDomeSelected);
	result &= NERPs_hook_function(GetPowerstationSelected);
	result &= NERPs_hook_function(GetToolStoreSelected);
	result &= NERPs_hook_function(GetGunstationSelected);
	result &= NERPs_hook_function(GetTeleportPadSelected);
	result &= NERPs_hook_function(GetSuperTeleportSelected);
	result &= NERPs_hook_function(GetUpgradeStationSelected);
	result &= NERPs_hook_function(GetPoweredBarracksBuilt);
	result &= NERPs_hook_function(GetPoweredDocksBuilt);
	result &= NERPs_hook_function(GetPoweredGeodomeBuilt);
	result &= NERPs_hook_function(GetPoweredPowerstationsBuilt);
	result &= NERPs_hook_function(GetPoweredToolStoresBuilt);
	result &= NERPs_hook_function(GetPoweredGunstationsBuilt);
	result &= NERPs_hook_function(GetPoweredTeleportsBuilt);
	result &= NERPs_hook_function(GetPoweredVehicleTeleportsBuilt);
	result &= NERPs_hook_function(GetPoweredUpgradeStationsBuilt);
	result &= NERPs_hook_function(GetBarracksBuilt);
	result &= NERPs_hook_function(GetDocksBuilt);
	result &= NERPs_hook_function(GetGeodomeBuilt);
	result &= NERPs_hook_function(GetPowerstationsBuilt);
	result &= NERPs_hook_function(GetToolStoresBuilt);
	result &= NERPs_hook_function(GetGunstationsBuilt);
	result &= NERPs_hook_function(GetTeleportsBuilt);
	result &= NERPs_hook_function(GetVehicleTeleportsBuilt);
	result &= NERPs_hook_function(GetUpgradeStationsBuilt);
	result &= NERPs_hook_function(GetLevel1BarracksBuilt);
	result &= NERPs_hook_function(GetLevel1DocksBuilt);
	result &= NERPs_hook_function(GetLevel1GeodomeBuilt);
	result &= NERPs_hook_function(GetLevel1PowerstationsBuilt);
	result &= NERPs_hook_function(GetLevel1ToolStoresBuilt);
	result &= NERPs_hook_function(GetLevel1GunstationsBuilt);
	result &= NERPs_hook_function(GetLevel1TeleportsBuilt);
	result &= NERPs_hook_function(GetLevel1VehicleTeleportsBuilt);
	result &= NERPs_hook_function(GetLevel1UpgradeStationsBuilt);
	result &= NERPs_hook_function(GetLevel2BarracksBuilt);
	result &= NERPs_hook_function(GetLevel2DocksBuilt);
	result &= NERPs_hook_function(GetLevel2GeodomeBuilt);
	result &= NERPs_hook_function(GetLevel2PowerstationsBuilt);
	result &= NERPs_hook_function(GetLevel2ToolStoresBuilt);
	result &= NERPs_hook_function(GetLevel2GunstationsBuilt);
	result &= NERPs_hook_function(GetLevel2TeleportsBuilt);
	result &= NERPs_hook_function(GetLevel2VehicleTeleportsBuilt);
	result &= NERPs_hook_function(GetLevel2UpgradeStationsBuilt);
	result &= NERPs_hook_function(GetBarracksIconClicked);
	result &= NERPs_hook_function(GetGeodomeIconClicked);
	result &= NERPs_hook_function(GetPowerstationIconClicked);
	result &= NERPs_hook_function(GetToolStoreIconClicked);
	result &= NERPs_hook_function(GetGunstationIconClicked);
	result &= NERPs_hook_function(GetTeleportPadIconClicked);
	result &= NERPs_hook_function(GetVehicleTransportIconClicked);
	result &= NERPs_hook_function(GetUpgradeStationIconClicked);
	result &= NERPs_hook_function(SetBarracksIconClicked);
	result &= NERPs_hook_function(SetGeodomeIconClicked);
	result &= NERPs_hook_function(SetPowerstationIconClicked);
	result &= NERPs_hook_function(SetToolStoreIconClicked);
	result &= NERPs_hook_function(SetGunstationIconClicked);
	result &= NERPs_hook_function(SetTeleportPadIconClicked);
	result &= NERPs_hook_function(SetVehicleTransportIconClicked);
	result &= NERPs_hook_function(SetUpgradeStationIconClicked);
	result &= NERPs_hook_function(FlashBarracksIcon);
	result &= NERPs_hook_function(FlashGeodomeIcon);
	result &= NERPs_hook_function(FlashPowerStationIcon);
	result &= NERPs_hook_function(FlashToolStoreIcon);
	result &= NERPs_hook_function(FlashGunstationIcon);
	result &= NERPs_hook_function(FlashTeleportPadIcon);
	result &= NERPs_hook_function(FlashVehicleTransportIcon);
	result &= NERPs_hook_function(FlashUpgradeStationIcon);
	result &= NERPs_hook_function(GetPathsBuilt);
	result &= NERPs_hook_function(GetStudCount);
	result &= NERPs_hook_function(GetSmallHelicoptersOnLevel);
	result &= NERPs_hook_function(GetGraniteGrindersOnLevel);
	result &= NERPs_hook_function(GetRapidRidersOnLevel);
	result &= NERPs_hook_function(GetSmallDiggersOnLevel);
	result &= NERPs_hook_function(GetSlugsOnLevel);
	result &= NERPs_hook_function(GetMiniFiguresOnLevel);
	result &= NERPs_hook_function(GetOreRefineriesBuilt);
	result &= NERPs_hook_function(GetCrystalRefineriesBuilt);
	result &= NERPs_hook_function(GetTeleportIconClicked);
	result &= NERPs_hook_function(GetDynamiteClicked);
	result &= NERPs_hook_function(GetMountIconClicked);
	result &= NERPs_hook_function(GetTrainIconClicked);
	result &= NERPs_hook_function(GetDropSonicBlasterIconClicked);
	result &= NERPs_hook_function(GetGetToolIconClicked);
	result &= NERPs_hook_function(GetGetPusherIconClicked);
	result &= NERPs_hook_function(GetGetSonicBlasterIconClicked);
	result &= NERPs_hook_function(GetTrainSailorIconClicked);
	result &= NERPs_hook_function(GetTrainPilotIconClicked);
	result &= NERPs_hook_function(GetTrainDriverIconClicked);
	result &= NERPs_hook_function(GetGetLaserIconClicked);
	result &= NERPs_hook_function(GetDismountIconClicked);
	result &= NERPs_hook_function(GetDigIconClicked);
	result &= NERPs_hook_function(GetBuildIconClicked);
	result &= NERPs_hook_function(GetLayPathIconClicked);
	result &= NERPs_hook_function(GetPlaceFenceIconClicked);
	result &= NERPs_hook_function(SetTeleportIconClicked);
	result &= NERPs_hook_function(SetDynamiteClicked);
	result &= NERPs_hook_function(SetTrainIconClicked);
	result &= NERPs_hook_function(SetTrainDriverIconClicked);
	result &= NERPs_hook_function(SetTrainSailorIconClicked);
	result &= NERPs_hook_function(SetGetToolIconClicked);
	result &= NERPs_hook_function(SetDropSonicBlasterIconClicked);
	result &= NERPs_hook_function(SetGetLaserIconClicked);
	result &= NERPs_hook_function(SetGetPusherIconClicked);
	result &= NERPs_hook_function(SetGetSonicBlasterIconClicked);
	result &= NERPs_hook_function(SetDismountIconClicked);
	result &= NERPs_hook_function(SetTrainPilotIconClicked);
	result &= NERPs_hook_function(SetMountIconClicked);
	result &= NERPs_hook_function(SetDigIconClicked);
	result &= NERPs_hook_function(SetBuildIconClicked);
	result &= NERPs_hook_function(SetLayPathIconClicked);
	result &= NERPs_hook_function(SetPlaceFenceIconClicked);
	result &= NERPs_hook_function(FlashTeleportIcon);
	result &= NERPs_hook_function(FlashDynamiteIcon);
	result &= NERPs_hook_function(FlashMountIcon);
	result &= NERPs_hook_function(FlashTrainIcon);
	result &= NERPs_hook_function(FlashTrainDriverIcon);
	result &= NERPs_hook_function(FlashTrainPilotIcon);
	result &= NERPs_hook_function(FlashTrainSailorIcon);
	result &= NERPs_hook_function(FlashDismountIcon);
	result &= NERPs_hook_function(FlashGetToolIcon);
	result &= NERPs_hook_function(FlashDropSonicBlasterIcon);
	result &= NERPs_hook_function(FlashGetLaserIcon);
	result &= NERPs_hook_function(FlashGetPusherIcon);
	result &= NERPs_hook_function(FlashGetSonicBlasterIcon);
	result &= NERPs_hook_function(FlashDigIcon);
	result &= NERPs_hook_function(FlashBuildIcon);
	result &= NERPs_hook_function(FlashLayPathIcon);
	result &= NERPs_hook_function(FlashPlaceFenceIcon);
	result &= NERPs_hook_function(GetRandom);
	result &= NERPs_hook_function(GetRandomTrueFalse);
	result &= NERPs_hook_function(GetRandom10);
	result &= NERPs_hook_function(GetRandom100);
	result &= NERPs_hook_function(GetCrystalsPickedUp);
	result &= NERPs_hook_function(GetCrystalsCurrentlyStored);

	result &= NERPs_hook_function(False);
	result &= NERPs_hook_function(Null);
	result &= NERPs_hook_function(Stop);
	result &= LegoRR::NERPs_HookFunction("**End Of List**", LegoRR::NERPFunc__End_Of_List);
	result &= NERPs_hook_function(GetCrystalsUsed);
	result &= NERPs_hook_function(GetCrystalsStolen);
	result &= NERPs_hook_function(GetOreUsed);
	result &= NERPs_hook_function(GetOreStolen);

	result &= NERPs_hook_function(GetOrePickedUp);
	result &= NERPs_hook_function(GetOreCurrentlyStored);
	// Also hooked directly.
	result &= NERPs_hook_function(GetTutorialFlags);
	result &= NERPs_hook_function(GetR0);
	result &= NERPs_hook_function(GetR1);
	result &= NERPs_hook_function(GetR2);
	result &= NERPs_hook_function(GetR3);
	result &= NERPs_hook_function(GetR4);
	result &= NERPs_hook_function(GetR5);
	result &= NERPs_hook_function(GetR6);
	result &= NERPs_hook_function(GetR7);
	result &= NERPs_hook_function(AddR0);
	result &= NERPs_hook_function(AddR1);
	result &= NERPs_hook_function(AddR2);
	result &= NERPs_hook_function(AddR3);
	result &= NERPs_hook_function(AddR4);
	result &= NERPs_hook_function(AddR5);
	result &= NERPs_hook_function(AddR6);
	result &= NERPs_hook_function(AddR7);
	result &= NERPs_hook_function(SubR0);
	result &= NERPs_hook_function(SubR1);
	result &= NERPs_hook_function(SubR2);
	result &= NERPs_hook_function(SubR3);
	result &= NERPs_hook_function(SubR4);
	result &= NERPs_hook_function(SubR5);
	result &= NERPs_hook_function(SubR6);
	result &= NERPs_hook_function(SubR7);
	result &= NERPs_hook_function(SetR0);
	result &= NERPs_hook_function(SetR1);
	result &= NERPs_hook_function(SetR2);
	result &= NERPs_hook_function(SetR3);
	result &= NERPs_hook_function(SetR4);
	result &= NERPs_hook_function(SetR5);
	result &= NERPs_hook_function(SetR6);
	result &= NERPs_hook_function(SetR7);
	// Also hooked directly.
	result &= NERPs_hook_function(SetTutorialFlags);
	result &= NERPs_hook_function(GetTrainFlags);
	result &= NERPs_hook_function(SetTrainFlags);
	result &= NERPs_hook_function(GetMonstersOnLevel);
	result &= NERPs_hook_function(GetBuildingsTeleported);
	result &= NERPs_hook_function(SetBuildingsTeleported);
	// Also hooked directly.
	result &= NERPs_hook_function(SetMessagePermit);

	result &= NERPs_hook_function(SetMessageWait);
	result &= NERPs_hook_function(SetMessageTimerValues);
	result &= NERPs_hook_function(GetMessageTimer);
	result &= NERPs_hook_function(SetMessage);
	// Also hooked directly.
	result &= NERPs_hook_function(SetObjectiveSwitch);
	result &= NERPs_hook_function(GetObjectiveSwitch);
	result &= NERPs_hook_function(GetObjectiveShowing);

	result &= NERPs_hook_function(MakeSomeoneOnThisBlockPickUpSomethingOnThisBlock);

	result &= NERPs_hook_function(SetCongregationAtTutorial);
	result &= NERPs_hook_function(SetRockMonsterAtTutorial);
	result &= NERPs_hook_function(SetCameraGotoTutorial);
	result &= NERPs_hook_function(GetCameraAtTutorial);
	result &= NERPs_hook_function(GetTutorialBlockIsGround);
	result &= NERPs_hook_function(GetTutorialBlockIsPath);
	result &= NERPs_hook_function(SetTutorialBlockIsGround);
	result &= NERPs_hook_function(SetTutorialBlockIsPath);
	result &= NERPs_hook_function(GetUnitAtBlock);
	result &= NERPs_hook_function(GetMonsterAtTutorial);

	result &= NERPs_hook_function(True);
	#pragma endregion

	return_interop(result);
}

bool interop_hook_LegoRR_Object(void)
{
	bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00436ee0, LegoRR::LegoObject_Initialise);

	// used by: Lego_Shutdown_Full
	result &= hook_write_jmpret(0x00437310, LegoRR::LegoObject_Shutdown);

	// used by: Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00437370, LegoRR::Object_Save_CopyStruct18);
	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x00437390, LegoRR::Object_Save_OverwriteStruct18);
	// used by: AITask_FUN_00405880
	result &= hook_write_jmpret(0x004373c0, LegoRR::LegoObject_GetObjectsBuilt);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00437410, LegoRR::Object_LoadToolTipIcons);
	// used by: Level_Free
	result &= hook_write_jmpret(0x00437560, LegoRR::LegoObject_CleanupLevel);
	// used by: LegoObject_FUN_00447880, Weapon_Projectile_AddStraightPath, Weapon_FireLazer
	result &= hook_write_jmpret(0x004375c0, LegoRR::LegoObject_Weapon_FUN_004375c0);
	// used by: LegoObject_Callback_Update, LegoObject_FUN_0043fee0, LegoObject_TryFindLoad_FUN_00440130
	result &= hook_write_jmpret(0x00437690, LegoRR::LegoObject_DoOpeningClosing);

	// used by: LegoObject_CleanupLevel
	result &= hook_write_jmpret(0x00437700, LegoRR::LegoObject_CleanupObjectLevels);
	// used by: Lego_MainLoop, LegoObject_GetObjectsBuilt, NERPsRuntime_GetLevelObjectsBuilt, Reward_GetLevelObjectsBuilt
	result &= hook_write_jmpret(0x00437720, LegoRR::LegoObject_GetLevelObjectsBuilt);
	// used by: NERPsRuntime_GetPreviousLevelObjectsBuilt, RewardQuota_CountBuildings
	result &= hook_write_jmpret(0x00437760, LegoRR::LegoObject_GetPreviousLevelObjectsBuilt);
	// used by: Level_BlockUpdateSurface
	result &= hook_write_jmpret(0x00437790, LegoRR::LegoObject_IncLevelPathsBuilt);
	// used by: LegoObject_Remove
	result &= hook_write_jmpret(0x004377b0, LegoRR::LegoObject_RemoveRouteToReferences);
	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004377d0, LegoRR::LegoObject_Callback_RemoveRouteToReference);

	result &= hook_write_jmpret(0x00437800, LegoRR::LegoObject_Remove);
	result &= hook_write_jmpret(0x00437a70, LegoRR::LegoObject_RunThroughListsSkipUpgradeParts);

	// used by: LegoObject_RunThroughListsSkipUpgradeParts, LegoObject_UpdateAll, LegoObject_HideAllCertainObjects
	result &= hook_write_jmpret(0x00437a90, LegoRR::LegoObject_RunThroughLists);

	// used by: Lego_HandleKeys, Lego_ShowObjectToolTip, ObjectRecall_RecallMiniFigure
	result &= hook_write_jmpret(0x00437b40, LegoRR::LegoObject_SetCustomName);
	// used by: LegoObject_CleanupLevel
	result &= hook_write_jmpret(0x00437ba0, LegoRR::HiddenObject_RemoveAll);
	// used by: Level_DestroyWall
	result &= hook_write_jmpret(0x00437c00, LegoRR::HiddenObject_ExposeBlock);
	// used by: Lego_LoadOLObjectList
	result &= hook_write_jmpret(0x00437ee0, LegoRR::HiddenObject_Add);
	// used by: AITask_DoAttackRockMonster_Target, LegoObject_Create
	result &= hook_write_jmpret(0x00437f80, LegoRR::LegoObject_CanShootObject);

	// used by: Lego_HandleWorldDebugKeys, Lego_LoadLevel, Lego_LoadOLObjectList,
	//          HiddenObject_ExposeBlock, LegoObject_CreateInWorld, LegoObject_PTL_GatherRock,
	//          Upgrade_SetUpgradeLevel
	result &= hook_write_jmpret(0x00437fc0, LegoRR::LegoObject_Create);
	// used by: LegoObject_Create
	result &= hook_write_jmpret(0x00438580, LegoRR::LegoObject_Create_internal);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004385d0, LegoRR::LegoObject_AddList);

	// used by: NERPFunc__GetBuildingsTeleported
	result &= hook_write_jmpret(0x00438650, LegoRR::LegoObject_GetNumBuildingsTeleported);
	// used by: LegoObject_CleanupLevel, NERPFunc__SetBuildingsTeleported
	result &= hook_write_jmpret(0x00438660, LegoRR::LegoObject_SetNumBuildingsTeleported);
	// used by: LegoObject_Weapon_FUN_004375c0, LegoObject_Create, LegoObject_Callback_Update
	result &= hook_write_jmpret(0x00438670, LegoRR::LegoObject_SetCrystalPoweredColour);
	// used by: Lego_LoadOLObjectList, HiddenObject_ExposeBlock, LegoObject_UpdateRemoval,
	//          LegoObject_Callback_Update
	result &= hook_write_jmpret(0x00438720, LegoRR::LegoObject_FUN_00438720);
	// used by: Interface_DoAction_FUN_0041dbd0
	result &= hook_write_jmpret(0x00438840, LegoRR::LegoObject_SetPowerOn);
	// used by: AITask, Interface_Callback_HasObject, Lego_HandleWorld, LegoObject,
	//          Message_IsObjectSelectable, Weapon_GenericDamageObject, Weapon_LegoObject_IsActiveWithTracker
	result &= hook_write_jmpret(0x00438870, LegoRR::LegoObject_IsActive);
	// used by: Construction_Zone_CompleteBuilding, ElectricFence, Level_GenerateCrystal, Level_GenerateOre,
	//          LegoObject, SpiderWeb_SpawnAt, Weapon_Projectile_AddStraightPath, Weapon_Projectile_AddCurvedPath
	result &= hook_write_jmpret(0x004388d0, LegoRR::LegoObject_CreateInWorld);
	// used by: Construction_PowerGrid_DrainAdjacentBlocks_Recurse
	result &= hook_write_jmpret(0x00438930, LegoRR::LegoObject_FindPoweredBuildingAtBlockPos);
	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00438970, LegoRR::LegoObject_Callback_FindPoweredBuildingAtBlockPos);
	
	// used by: 
	//result &= hook_write_jmpret(0x, LegoRR::);

	result &= hook_write_jmpret(0x00439c50, LegoRR::LegoObject_RequestPowerGridUpdate);

	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x0043aeb0, LegoRR::LegoObject_AttackPath);
	// used by: LegoObject_AttackPath
	result &= hook_write_jmpret(0x0043af50, LegoRR::LegoObject_Callback_StampMiniFigureWithCrystal);
	// used by: Lego_UpdateSlug_FUN_004260f0, NERPFunc__GenerateSlug
	result &= hook_write_jmpret(0x0043b010, LegoRR::LegoObject_TryGenerateSlug);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0043b530, LegoRR::LegoObject_UpdateAll);

	// used by: LegoObject_Shutdown, LegoObject_CleanupLevel
	result &= hook_write_jmpret(0x0043b5e0, LegoRR::LegoObject_RemoveAll);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0043b610, LegoRR::LegoObject_Callback_Remove);

	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x0043bf00, LegoRR::LegoObject_UpdateRemoval);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0043c570, LegoRR::LegoObject_UpdateAllRadarSurvey);
	// used by: LegoObject_UpdateAllRadarSurvey
	result &= hook_write_jmpret(0x0043c5b0, LegoRR::LegoObject_Callback_UpdateRadarSurvey);
	
	// used by: LegoObject_Create, LegoObject_Callback_Update, StatsObject_Debug_ToggleSelfPowered
	result &= hook_write_jmpret(0x0043c830, LegoRR::LegoObject_UpdatePowerConsumption);
	// used by: AITask_FUN_00405880
	result &= hook_write_jmpret(0x0043c910, LegoRR::LegoObject_CheckCanSteal);
	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x0043c970, LegoRR::LegoObject_UpdateElapsedTimes);
	// used by: LegoObject_UpdateAll
	result &= hook_write_jmpret(0x0043cad0, LegoRR::LegoObject_Callback_Update);
	// used by: LegoObject_Callback_Update, LegoObject_Interrupt
	result &= hook_write_jmpret(0x0043f160, LegoRR::LegoObject_ProccessCarriedObjects);
	// used by: Lego_LoadOLObjectList, HiddenObject_ExposeBlock, LegoObject_Callback_Update
	result &= hook_write_jmpret(0x0043f3c0, LegoRR::LegoObject_ClearDockOccupiedFlag);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x0043f3f0, LegoRR::LegoObject_TriggerFrameCallback);
	// used by: Lego_TryTeleportObject
	result &= hook_write_jmpret(0x0043f410, LegoRR::LegoObject_QueueTeleport);
	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x0043f450, LegoRR::LegoObject_UpdateTeleporter);
	// used by: LegoObject_Remove, LegoObject_FUN_00438720
	result &= hook_write_jmpret(0x0043f820, LegoRR::LegoObject_RemoveTeleportDownReference);
	// used by: LegoObject_RemoveTeleportDownReference
	result &= hook_write_jmpret(0x0043f840, LegoRR::LegoObject_Callback_RemoveTeleportDownReference);
	// used by: AITask_LiveObject_SetAITaskUnk, Lego_LoadOLObjectList, HiddenObject_ExposeBlock
	result &= hook_write_jmpret(0x0043f870, LegoRR::LegoObject_TrainMiniFigure_instantunk);
	// used by: Lego_HandleWorldDebugKeys, LegoObject_Callback_Update,
	//          LegoObject_UpdateEnergyHealthAndContact, LegoObject_Callback_DynamiteExplosion,
	//          LegoObject_Callback_SlipAndScare, LegoObject_BumpDamageRouteObject, LegoObject_FUN_00447880,
	//          Weapon_GenericDamageObject, Weapon_GunDamageObject
	result &= hook_write_jmpret(0x0043f960, LegoRR::LegoObject_AddDamage2);
	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x0043fa90, LegoRR::LegoObject_UpdateEnergyHealthAndContact);
	// used by: LegoObject_UpdateEnergyHealthAndContact
	result &= hook_write_jmpret(0x0043fe00, LegoRR::LegoObject_MiniFigurePlayHurtSound);

	// used by: Lego_HandleKeys, LegoObject
	result &= hook_write_jmpret(0x00440ca0, LegoRR::LegoObject_SetActivity);

	// used by: AITask_Callback_UpdateObject, AITask_LiveObject_FUN_00404d30, AITask_FUN_00405b40
	result &= hook_write_jmpret(0x00440ef0, LegoRR::LegoObject_Route_BuildListToTarget);
	// used by: LegoObject_Route_BuildList
	result &= hook_write_jmpret(0x00440f30, LegoRR::LegoObject_Route_BuildListWithoutScore);
	// used by: LegoObject
	result &= hook_write_jmpret(0x004413b0, LegoRR::LegoObject_Route_BuildList);
	// used by: AITask, LegoObject
	result &= hook_write_jmpret(0x004419c0, LegoRR::LegoObject_Route_Begin);
	// used by: AITask, Lego, LegoObject, Message, NERPs
	result &= hook_write_jmpret(0x00441c00, LegoRR::LegoObject_Route_End);

	// used by: ElectricFence_SparkObject, LegoObject_UpdateRemoval
	result &= hook_write_jmpret(0x004424d0, LegoRR::LegoObject_StartCrumbling);
	// used by: AITask, Construction, Effect, ElectricFence, Lego, LegoObject, Message,
	//          NERPs, RadarMap, Teleporter, Weapon
	result &= hook_write_jmpret(0x00442520, LegoRR::LegoObject_GetPosition);
	// used by: AITask_FUN_00405b40, Construction_CleanupBuildingFoundation,
	//          ElectricFence_Callback_ChainBeamsFromBuilding, LegoObject_GetHeading,
	//          LegoObject_Callback_HideCertainObjects, SpiderWeb_CheckCollision
	result &= hook_write_jmpret(0x00442560, LegoRR::LegoObject_GetFaceDirection);
	// used by: LegoObject_UpdateTeleporter, LegoObject_FireBeamWeaponAtObject
	result &= hook_write_jmpret(0x004425c0, LegoRR::LegoObject_SetHeadingOrDirection);
	// used by: LegoObject_Callback_Update, LegoObject_SimpleObject_MoveAnimation,
	//          LegoObject_PlaceBirdScarer_AndTickDown, LegoObject_Freeze
	result &= hook_write_jmpret(0x00442740, LegoRR::LegoObject_GetHeading);
	// used by: AITask, Construction, ElectricFence, Interface, Lego, LegoObject,
	//          NERPs, Objective, SpiderWeb
	result &= hook_write_jmpret(0x004427b0, LegoRR::LegoObject_GetBlockPos);
	// used by: LegoObject_UpdateRoutingVectors, LegoObject_SetPositionAndHeading,
	//          LegoObject_FP_UpdateMovement, LegoObject_UpdateWorldStickyPosition
	result &= hook_write_jmpret(0x00442800, LegoRR::LegoObject_GetWorldZCallback);
	// used by: LegoObject_UpdateRoutingVectors, LegoObject_SetPositionAndHeading,
	//          LegoObject_FP_UpdateMovement, LegoObject_UpdateWorldStickyPosition
	result &= hook_write_jmpret(0x00442820, LegoRR::LegoObject_GetWorldZCallback_Lake);
	// used by: LegoObject_Route_UpdateMovement
	result &= hook_write_jmpret(0x004428b0, LegoRR::LegoObject_UpdateRoutingVectors);
	// used by: Lego_LoadOLObjectList, HiddenObject_ExposeBlock, LegoObject_CreateInWorld,
	//          LegoObject_Callback_Update, LegoObject_UpdateTeleporter, LegoObject_PTL_GatherRock,
	//          LegoObject_SimpleObject_MoveAnimation, LegoObject_UpdatePushing
	result &= hook_write_jmpret(0x00442b60, LegoRR::LegoObject_SetPositionAndHeading);

	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x00443240, LegoRR::LegoObject_UpdateWorldStickyPosition);

	// used by: LegoObject_Callback_ScareUnitWithBigBang, LegoObject_Callback_ScareTrainedMiniFiguresAwayFromTickingDynamite,
	//          LegoObject_Callback_SlipAndScare
	result &= hook_write_jmpret(0x00444720, LegoRR::LegoObject_TryRunAway);

	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x004459a0, LegoRR::LegoObject_UpdateSlipAndScare);
	// used by: LegoObject_UpdateSlipAndScare
	result &= hook_write_jmpret(0x00445a30, LegoRR::LegoObject_Callback_ScareTrainedMiniFiguresAwayFromTickingDynamite);
	// used by: LegoObject_UpdateSlipAndScare
	result &= hook_write_jmpret(0x00445af0, LegoRR::LegoObject_Callback_SlipAndScare);

	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x004477b0, LegoRR::LegoObject_BumpDamageRouteObject);

	// used by: Lego_HandleKeys, LegoObject
	result &= hook_write_jmpret(0x00447f00, LegoRR::LegoObject_UpdateActivityChange);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00449ec0, LegoRR::LegoObject_HideAllCertainObjects);

	// used by: Lego_HandleWorld
	result &= hook_write_jmpret(0x0044abd0, LegoRR::LegoObject_UpdateBuildingPlacement);

	// used by: Lego_QuitLevel, Lego_LoadLevel, Objective_SetStatus
	result &= hook_write_jmpret(0x0044b080, LegoRR::LegoObject_SetLevelEnding);

	// used by: LegoObject_UpdateRemoval
	result &= hook_write_jmpret(0x0044b0a0, LegoRR::LegoObject_FUN_0044b0a0);


	// used by: Weapon_GunHitObject
	result &= hook_write_jmpret(0x0044c2f0, LegoRR::LegoObject_Freeze);


	// used by: Panel_CheckCollision
	result &= hook_write_jmpret(0x0044c810, LegoRR::LegoObject_CameraCycleUnits);
	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0044c8b0, LegoRR::LegoObject_Callback_CameraCycleFindUnit);

	return_interop(result);
}

bool interop_hook_LegoRR_ObjectRecall(void)
{
	bool result = true;

	//  used by : LegoObject_UpdateRemoval
	result &= hook_write_jmpret(0x00459450, LegoRR::ObjectRecall_StoreMiniFigure);
	//  used by : LegoObject_UpdateTeleporter
	result &= hook_write_jmpret(0x00459500, LegoRR::ObjectRecall_RecallMiniFigure);
	//  used by : ObjectRecall_Save_CreateNewObjectRecall
	result &= hook_write_jmpret(0x00459560, LegoRR::ObjectRecall_Save_FreeObjectRecall);
	//  used by : Objective_Update
	result &= hook_write_jmpret(0x004595a0, LegoRR::ObjectRecall_Save_CopyToNewObjectRecallData);
	//  used by : Front_RestartLevel,Objective_Update
	result &= hook_write_jmpret(0x00459620, LegoRR::ObjectRecall_Save_CreateNewObjectRecall);
	//  used by : Lego_LoadLevel
	result &= hook_write_jmpret(0x00459690, LegoRR::ObjectRecall_IsLoaded);
	//  used by : Front_Save_WriteSaveFiles
	result &= hook_write_jmpret(0x004596a0, LegoRR::ObjectRecall_SaveRROSFile);
	//  used by : Lego_LoadLevel
	result &= hook_write_jmpret(0x00459730, LegoRR::ObjectRecall_LoadRROSFile);

	return_interop(result);
}

bool interop_hook_LegoRR_Objective(void)
{
	bool result = true;

	// used by: Objective_LoadLevel
	result &= hook_write_jmpret(0x004577a0, LegoRR::Objective_LoadObjectiveText);
	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x00458000, LegoRR::Objective_LoadLevel);
	// used by: Objective_LoadLevel
	result &= hook_write_jmpret(0x00458840, LegoRR::Objective_SetCryOreObjectives);
	// used by: Objective_LoadLevel
	result &= hook_write_jmpret(0x00458880, LegoRR::Objective_SetBlockObjective);
	// used by: Objective_LoadLevel
	result &= hook_write_jmpret(0x004588b0, LegoRR::Objective_SetTimerObjective);
	// used by: Objective_LoadLevel
	result &= hook_write_jmpret(0x004588e0, LegoRR::Objective_SetConstructionObjective);
	// used by: Reward_GotoSaveMenu
	result &= hook_write_jmpret(0x00458910, LegoRR::Objective_IsObjectiveAchieved);
	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x00458920, LegoRR::Objective_SetEndTeleportEnabled);
	// used by: Front_Callback_TriggerReplayObjective, Lego_MainLoop, Level_ConsumeObjectOxygen,
	//          Lego_HandleKeys, Lego_LoadLevel, NERPFunc__SetLevelCompleted, (NERPFunc__SetGameCompleted),
	//          NERPFunc__SetLevelFail, NERPFunc__SetGameFail, Objective_Update
	result &= hook_write_jmpret(0x00458930, LegoRR::Objective_SetStatus);
	// used by: Objective_HandleKeys, Objective_Update
	result &= hook_write_jmpret(0x00458ba0, LegoRR::Objective_StopShowing);
	// used by: Front_Options_Update, Lego_MainLoop, Lego_MainLoop, NERPFunc__GetObjectiveShowing, Objective_HandleKeys
	result &= hook_write_jmpret(0x00458c60, LegoRR::Objective_IsShowing);
	// used by: Lego_HandleKeys
	result &= hook_write_jmpret(0x00458c80, LegoRR::Objective_HandleKeys);
	// used by: Lego_MainLoop, Lego_LoadLevel
	result &= hook_write_jmpret(0x00458ea0, LegoRR::Objective_Update);
	// used by: Objective_Update
	result &= hook_write_jmpret(0x00459310, LegoRR::Objective_CheckCompleted);
	// used by: Objective_CheckCompleted
	result &= hook_write_jmpret(0x004593c0, LegoRR::Objective_Callback_CheckCompletedObject);

	return_interop(result);
}

bool interop_hook_LegoRR_ObjInfo(void)
{
	bool result = true;
	
	// used by: Bubble_DrawAllObjInfos, Bubble_Callback_DrawObjInfo
	result &= hook_write_jmpret(0x00459dc0, LegoRR::ObjInfo_DrawHealthBar);
	// used by: Bubble_Callback_DrawObjInfo
	result &= hook_write_jmpret(0x0045a210, LegoRR::ObjInfo_DrawHungerImage);
	// used by: Bubble_DrawAllObjInfos, Bubble_Callback_DrawObjInfo
	result &= hook_write_jmpret(0x0045a290, LegoRR::ObjInfo_DrawBubbleImage);
	
	return_interop(result);
}

bool interop_hook_LegoRR_Pointers(void)
{
	bool result = true;
	
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x0045caf0, LegoRR::Pointer_Initialise);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x0045cd30, LegoRR::Pointer_Load);
	// used by: Info_SetText_internal, Pointer_Load
	result &= hook_write_jmpret(0x0045ce90, LegoRR::Pointer_GetType);
	// used by: Info_SetText_internal
	result &= hook_write_jmpret(0x0045ced0, LegoRR::Pointer_GetImage);
	// used by: Front_Menu_Update, Lego_Initialise, Lego_MainLoop, Lego_HandleRadarInput,
	//          Lego_HandleWorld, Level_SetPointer_FromSurfaceType, LegoObject_UpdateBuildingPlacement,
	//          Reward_LoopUpdate
	result &= hook_write_jmpret(0x0045cee0, LegoRR::Pointer_SetCurrent_IfTimerFinished);
	// used by: Lego_SetPointerSFX
	result &= hook_write_jmpret(0x0045cf00, LegoRR::Pointer_SetCurrent);
	// used by: Front_Menu_Update, Lego_HandleWorld
	result &= hook_write_jmpret(0x0045cf20, LegoRR::Pointer_GetCurrentType);
	// used by: Front_Menu_Update, Lego_MainLoop, Reward_LoopUpdate
	result &= hook_write_jmpret(0x0045cf30, LegoRR::Pointer_DrawPointer);
	// used by: Lego_MainLoop, Reward_LoopUpdate
	result &= hook_write_jmpret(0x0045d050, LegoRR::Pointer_Update);
	
	return_interop(result);
}

bool interop_hook_LegoRR_Priorities(void)
{
	bool result = true;

	// used by: Lego_Initalize
	result &= hook_write_jmpret(0x0045d080, LegoRR::Priorities_LoadImages);
	// used by: Panel_CheckCollision
	result &= hook_write_jmpret(0x0045d1c0, LegoRR::Priorities_Reset);
	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x0045d210, LegoRR::Priorities_LoadLevel);
	// used by: Lego_Initalize
	result &= hook_write_jmpret(0x0045d320, LegoRR::Priorities_LoadPositions);
	// used by: Lego_HandleWorld
	result &= hook_write_jmpret(0x0045d3d0, LegoRR::Priorities_HandleInput);
	// used by: Priorities_HandleInput
	result &= hook_write_jmpret(0x0045d5c0, LegoRR::Priorities_ChangePriorityPressed);
	// used by: Priorities_HandleInput
	result &= hook_write_jmpret(0x0045d630, LegoRR::Priorities_SetHoverArea);
	// used by: Priorities_HandleInput
	result &= hook_write_jmpret(0x0045d680, LegoRR::Priorities_MovePriorityUpOrTop);
	// used by: Priorities_MovePriorityUpOrTop
	result &= hook_write_jmpret(0x0045d6b0, LegoRR::Priorities_MovePriorityTop);
	// used by: Priorities_MovePriorityUpOrTop
	result &= hook_write_jmpret(0x0045d730, LegoRR::Priorities_MovePriorityUp);
	// used by: Panel_PriorityList_FUN_0045b8e0
	result &= hook_write_jmpret(0x0045d7b0, LegoRR::Priorities_TurnPriorityOff);
	// used by: Priorities_MovePriorityUp
	result &= hook_write_jmpret(0x0045d810, LegoRR::Priorities_MoveCursorToPriorityUpButton);
	// used by: Priorities_MoveCursorToPriorityUpButton
	result &= hook_write_jmpret(0x0045d900, LegoRR::Priorities_MoveCursorToPriorityImage);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0045d990, LegoRR::Priorities_Draw);

	return_interop(result);
}


bool interop_hook_LegoRR_PTL(void)
{
	bool result = true;
	
	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x0045daa0, LegoRR::PTL_Initialise);

	// used by: Message_Update
	result &= hook_write_jmpret(0x0045db30, LegoRR::PTL_TranslateEvent);
	
	return_interop(result);
}

bool interop_hook_LegoRR_RadarMap(void)
{
	bool result = true;
	
	// used by: Lego_DrawRadarMap
	result &= hook_write_jmpret(0x0045db60, LegoRR::RadarMap_SetZoom);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x0045db70, LegoRR::RadarMap_Initialise);
	// used by: Lego_LoadMapSet
	result &= hook_write_jmpret(0x0045dd50, LegoRR::RadarMap_Create);
	
	/// THIS IS A MERGED FUNCTION! DON'T HOOK!
	// used by: Level_Free
	//result &= hook_write_jmpret(0x0045ddb0, LegoRR::RadarMap_Free);
	
	// used by: LegoObject_Callback_UpdateRadarSurvey
	result &= hook_write_jmpret(0x0045ddc0, LegoRR::RadarMap_DrawSurveyDotCircle);
	// used by: Lego_DrawRadarMap
	result &= hook_write_jmpret(0x0045de80, LegoRR::RadarMap_Draw);
	// used by: RadarMap_Callback_AddObjectDrawRect
	result &= hook_write_jmpret(0x0045e6c0, LegoRR::RadarMap_CanShowObject);
	// used by: RadarMap_Draw
	result &= hook_write_jmpret(0x0045e720, LegoRR::RadarMap_Callback_AddObjectDrawRect);
	// used by: Lego_HandleRadarInput, Lego_HandleWorld
	result &= hook_write_jmpret(0x0045e920, LegoRR::RadarMap_InsideRadarScreen);
	// used by: Lego_HandleRadarInput, RadarMap_TrySelectObject
	result &= hook_write_jmpret(0x0045e990, LegoRR::RadarMap_ScreenToWorldBlockPos);
	// used by: Lego_HandleRadarInput
	result &= hook_write_jmpret(0x0045eae0, LegoRR::RadarMap_TrySelectObject);
	// used by: RadarMap_TrySelectObject
	result &= hook_write_jmpret(0x0045eba0, LegoRR::RadarMap_Callback_FindObjectInClickArea);
	// used by: RadarMap_DrawSurveyDotCircle, RadarMap_Draw, RadarMap_Callback_AddObjectDrawRect
	result &= hook_write_jmpret(0x0045ec00, LegoRR::RadarMap_TransformRect);
	// used by: RadarMap_Draw
	result &= hook_write_jmpret(0x0045eca0, LegoRR::RadarMap_GetBlockColour);
	
	return_interop(result);
}

bool interop_hook_LegoRR_Reward(void)
{
	bool result = true;
	
	// used by: Reward_Show, Reward_DrawScore, Reward_DrawAllValues
	result &= hook_write_jmpret(0x00461a50, LegoRR::Reward_DrawItem);

	// used by: Reward_HandleButtons
	result &= hook_write_jmpret(0x004622f0, LegoRR::Reward_GotoSaveMenu);

	// used by: Reward_HandleButtons, Reward_LoopBegin
	result &= hook_write_jmpret(0x00462530, LegoRR::Reward_GotoAdvance);

	// used by: Reward_Show
	result &= hook_write_jmpret(0x004629c0, LegoRR::Reward_LoopUpdate);
	
	return_interop(result);
}


bool interop_hook_LegoRR_Roof(void)
{
	bool result = true;

	// used by: Lego_LoadMapSet
	result &= hook_write_jmpret(0x00463060, LegoRR::Roof_Initialise);

	// used by: Lego_LoadTextureSet
	result &= hook_write_jmpret(0x00463120, LegoRR::Roof_SetTexture);

	// used by: Level_Free
	result &= hook_write_jmpret(0x00463190, LegoRR::Roof_Shutdown);

	// used by: Level_Block_LowerRoofVertices
	result &= hook_write_jmpret(0x004631e0, LegoRR::Roof_SetBlockRoofVertices);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x004634f0, LegoRR::Roof_Update);

	// used by: Lego_LoadMapSet, Lego_MainLoop
	result &= hook_write_jmpret(0x00463770, LegoRR::Roof_Hide);

	// used by: Lego_FPHighPolyBlocks
	result &= hook_write_jmpret(0x004637b0, LegoRR::Roof_AddVisibleBlock);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00463800, LegoRR::Roof_HideAllVisibleBlocks);

	// used by: Roof_Update
	result &= hook_write_jmpret(0x00463850, LegoRR::Roof_GetBlockPlaneNormal);

	return_interop(result);
}

bool interop_hook_LegoRR_SelectPlace(void)
{
	bool result = true;
	
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x004641c0, LegoRR::SelectPlace_Create);
	// used by: Construction_CleanupBuildingFoundation, ElectricFence_Callback_ChainBeamsFromBuilding,
	//          Lego_LoadOLObjectList, HiddenObject_ExposeBlock, LegoObject_Callback_PickSphereSelection,
	//          LegoObject_Callback_HideCertainObjects, SelectPlace_CheckAndUpdate
	result &= hook_write_jmpret(0x004643d0, LegoRR::SelectPlace_TransformShapePoints);
	// used by: LegoObject_UpdateBuildingPlacement
	result &= hook_write_jmpret(0x00464480, LegoRR::SelectPlace_CheckAndUpdate);
	// used by: Lego_Initialise, Lego_HandleKeys, Lego_HandleWorld, Lego_StopUserAction,
	//          LegoObject_UpdateBuildingPlacement
	result &= hook_write_jmpret(0x004649e0, LegoRR::SelectPlace_Hide);
	
	return_interop(result);
}

bool interop_hook_LegoRR_SFX(void)
{
	bool result = true;

	// Ensure data is initialized to zero, testing if NERPs in-game messages are using garbage data
	result &= hook_write_jmpret(0x00464a00, LegoRR::SFX_Initialise);

	result &= hook_write_jmpret(0x00464ee0, LegoRR::SFX_Container_SoundTriggerCallback);
	result &= hook_write_jmpret(0x00464f10, LegoRR::SFX_SetSamplePopulateMode);
	result &= hook_write_jmpret(0x00464f30, LegoRR::SFX_GetType);

	// Fix apply for sound groups cutting out the first-listed sound
	result &= hook_write_jmpret(0x00464fc0, LegoRR::SFX_LoadSampleProperty);

	result &= hook_write_jmpret(0x004650e0, LegoRR::SFX_Random_GetSound3DHandle);
	result &= hook_write_jmpret(0x00465140, LegoRR::SFX_StopGlobalSample);
	result &= hook_write_jmpret(0x00465180, LegoRR::SFX_SetGlobalSampleDurationIfLE0_AndNullifyHandle);
	result &= hook_write_jmpret(0x004651b0, LegoRR::SFX_Random_SetAndPlayGlobalSample);
	result &= hook_write_jmpret(0x00465220, LegoRR::SFX_AddToQueue);
	result &= hook_write_jmpret(0x00465260, LegoRR::SFX_Random_PlaySoundNormal);
	result &= hook_write_jmpret(0x004652d0, LegoRR::SFX_Random_SetBufferVolume);
	result &= hook_write_jmpret(0x004652f0, LegoRR::SFX_Random_GetBufferVolume);
	result &= hook_write_jmpret(0x00465310, LegoRR::SFX_Random_PlaySound3DOnContainer);
	result &= hook_write_jmpret(0x00465350, LegoRR::SFX_Random_PlaySound3DOnFrame);
	result &= hook_write_jmpret(0x00465420, LegoRR::SFX_Random_GetSamplePlayTime);
	result &= hook_write_jmpret(0x00465450, LegoRR::SFX_Sound3D_StopSound);
	result &= hook_write_jmpret(0x00465460, LegoRR::SFX_Update);
	result &= hook_write_jmpret(0x00465510, LegoRR::SFX_Sound3D_Update);
	result &= hook_write_jmpret(0x00465520, LegoRR::SFX_SetSoundOn);
	result &= hook_write_jmpret(0x00465560, LegoRR::SFX_SetSoundOn_AndStopAll);
	result &= hook_write_jmpret(0x00465570, LegoRR::SFX_IsQueueMode);
	result &= hook_write_jmpret(0x00465580, LegoRR::SFX_SetQueueMode_AndFlush);
	result &= hook_write_jmpret(0x00465590, LegoRR::SFX_SetQueueMode);
	result &= hook_write_jmpret(0x00465630, LegoRR::SFX_IsSoundOn);

	return_interop(result);
}

bool interop_hook_LegoRR_Smoke(void)
{
	bool result = true;
	
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00465640, LegoRR::Smoke_Initialise);
	result &= hook_write_jmpret(0x00465660, LegoRR::Smoke_LoadTextures);

	// used by: Level_BlockUpdateSurface
	result &= hook_write_jmpret(0x004656f0, LegoRR::Smoke_CreateSmokeArea);

	// used by: Level_Free
	result &= hook_write_jmpret(0x00465c30, LegoRR::Smoke_RemoveAll);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00465c70, LegoRR::Smoke_Remove);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00465d50, LegoRR::Smoke_HideAll);

	// used by: Lego_XYCallback_AddVisibleSmoke, Level_BlockUpdateSurface, Lego_FPHighPolyBlocks_FUN_00433db0
	result &= hook_write_jmpret(0x00465d80, LegoRR::Smoke_Hide);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x00465dc0, LegoRR::Smoke_Update);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00465f10, LegoRR::Smoke_Group_Show);
	//result &= hook_write_jmpret(0x004660c0, LegoRR::Smoke_Group_Update);
	//result &= hook_write_jmpret(0x004661a0, LegoRR::Smoke_MeshRenderCallback);
	//result &= hook_write_jmpret(0x00466200, LegoRR::Smoke_Group_MeshRenderCallback);
	
	return_interop(result);
}

bool interop_hook_LegoRR_Stats(void)
{
	bool result = true;
	
	result &= hook_write_jmpret(0x00466aa0, LegoRR::Stats_Initialise);
	result &= hook_write_jmpret(0x00469d50, LegoRR::Stats_AddToolTaskType);
	result &= hook_write_jmpret(0x00469d80, LegoRR::Stats_GetCostOre);
	result &= hook_write_jmpret(0x00469db0, LegoRR::Stats_GetCostCrystal);
	result &= hook_write_jmpret(0x00469de0, LegoRR::Stats_GetCostRefinedOre);
	result &= hook_write_jmpret(0x00469e10, LegoRR::StatsObject_GetCrystalDrain);
	result &= hook_write_jmpret(0x00469e40, LegoRR::StatsObject_GetCapacity);
	result &= hook_write_jmpret(0x00469e70, LegoRR::StatsObject_GetMaxCarry);
	result &= hook_write_jmpret(0x00469ea0, LegoRR::StatsObject_GetCarryStart);
	result &= hook_write_jmpret(0x00469ed0, LegoRR::StatsObject_SetObjectLevel);
	result &= hook_write_jmpret(0x00469f70, LegoRR::StatsObject_GetRouteSpeed);
	result &= hook_write_jmpret(0x00469f80, LegoRR::StatsObject_GetDrillTimeType);
	result &= hook_write_jmpret(0x00469fa0, LegoRR::StatsObject_GetRubbleCoef);
	result &= hook_write_jmpret(0x00469fc0, LegoRR::StatsObject_GetWakeRadius);
	result &= hook_write_jmpret(0x00469fe0, LegoRR::StatsObject_GetPathCoef);
	result &= hook_write_jmpret(0x0046a000, LegoRR::StatsObject_GetCollRadius);
	result &= hook_write_jmpret(0x0046a010, LegoRR::StatsObject_GetCollHeight);
	result &= hook_write_jmpret(0x0046a020, LegoRR::StatsObject_GetPickSphere);
	result &= hook_write_jmpret(0x0046a030, LegoRR::StatsObject_GetPainThreshold);
	result &= hook_write_jmpret(0x0046a050, LegoRR::StatsObject_GetAlertRadius);
	result &= hook_write_jmpret(0x0046a060, LegoRR::StatsObject_GetCollBox);
	result &= hook_write_jmpret(0x0046a070, LegoRR::StatsObject_GetTrackDist);
	result &= hook_write_jmpret(0x0046a080, LegoRR::StatsObject_GetHealthDecayRate);
	result &= hook_write_jmpret(0x0046a0a0, LegoRR::StatsObject_GetEnergyDecayRate);
	result &= hook_write_jmpret(0x0046a0c0, LegoRR::Stats_GetOxygenCoef);
	result &= hook_write_jmpret(0x0046a0e0, LegoRR::StatsObject_GetOxygenCoef);
	result &= hook_write_jmpret(0x0046a100, LegoRR::StatsObject_GetSurveyRadius);
	result &= hook_write_jmpret(0x0046a120, LegoRR::StatsObject_GetStatsFlags1);
	result &= hook_write_jmpret(0x0046a140, LegoRR::StatsObject_GetStatsFlags2);
	result &= hook_write_jmpret(0x0046a160, LegoRR::StatsObject_GetStatsFlags3);
	result &= hook_write_jmpret(0x0046a180, LegoRR::Stats_GetStatsFlags1);
	result &= hook_write_jmpret(0x0046a1a0, LegoRR::Stats_GetStatsFlags2);
	result &= hook_write_jmpret(0x0046a1c0, LegoRR::Stats_GetStatsFlags3);
	result &= hook_write_jmpret(0x0046a1e0, LegoRR::StatsObject_GetRepairValue);
	result &= hook_write_jmpret(0x0046a200, LegoRR::Stats_GetLevels);
	result &= hook_write_jmpret(0x0046a220, LegoRR::Stats_GetWaterEntrances);
	result &= hook_write_jmpret(0x0046a250, LegoRR::StatsObject_GetDrillSoundType);
	result &= hook_write_jmpret(0x0046a280, LegoRR::StatsObject_GetEngineSound);
	result &= hook_write_jmpret(0x0046a2a0, LegoRR::StatsObject_GetRestPercent);
	result &= hook_write_jmpret(0x0046a2c0, LegoRR::StatsObject_GetCarryMinHealth);
	result &= hook_write_jmpret(0x0046a2e0, LegoRR::StatsObject_GetAttackRadius);
	result &= hook_write_jmpret(0x0046a300, LegoRR::StatsObject_GetStampRadius);
	result &= hook_write_jmpret(0x0046a320, LegoRR::StatsObject_GetNumOfToolsCanCarry);
	result &= hook_write_jmpret(0x0046a340, LegoRR::StatsObject_GetUpgradeTime);
	result &= hook_write_jmpret(0x0046a360, LegoRR::StatsObject_GetFunctionCoef);
	result &= hook_write_jmpret(0x0046a380, LegoRR::Stats_GetUpgradeCostOre);
	result &= hook_write_jmpret(0x0046a3b0, LegoRR::Stats_GetUpgradeCostStuds);
	result &= hook_write_jmpret(0x0046a3e0, LegoRR::Stats_FindToolFromTaskType);
	result &= hook_write_jmpret(0x0046a430, LegoRR::StatsObject_GetFlocks_Height);
	result &= hook_write_jmpret(0x0046a450, LegoRR::StatsObject_GetFlocks_Randomness);
	result &= hook_write_jmpret(0x0046a470, LegoRR::StatsObject_GetFlocks_Turn);
	result &= hook_write_jmpret(0x0046a490, LegoRR::StatsObject_GetFlocks_Tightness);
	result &= hook_write_jmpret(0x0046a4b0, LegoRR::StatsObject_GetFlocks_Speed);
	result &= hook_write_jmpret(0x0046a4d0, LegoRR::StatsObject_GetFlocks_Size);
	result &= hook_write_jmpret(0x0046a4f0, LegoRR::StatsObject_GetFlocks_GoalUpdate);
	result &= hook_write_jmpret(0x0046a510, LegoRR::StatsObject_GetFlocks_AttackTime);
	result &= hook_write_jmpret(0x0046a530, LegoRR::StatsObject_GetAwarenessRange);
	result &= hook_write_jmpret(0x0046a550, LegoRR::StatsObject_GetPusherDist);
	result &= hook_write_jmpret(0x0046a570, LegoRR::StatsObject_GetPusherDamage);
	result &= hook_write_jmpret(0x0046a590, LegoRR::StatsObject_GetLaserDamage);
	result &= hook_write_jmpret(0x0046a5b0, LegoRR::StatsObject_GetFreezerDamage);
	result &= hook_write_jmpret(0x0046a5d0, LegoRR::StatsObject_GetFreezerTime);
	result &= hook_write_jmpret(0x0046a5f0, LegoRR::StatsObject_Debug_ToggleSelfPowered);
	
	return_interop(result);
}

bool interop_hook_LegoRR_TextMessages(void)
{
	bool result = true;

	// used by: Lego_Initalize
	result &= hook_write_jmpret(0x0046aab0, LegoRR::Text_Load);

	// used by: Lego_Initalize
	result &= hook_write_jmpret(0x0046ac10, LegoRR::Text_Initialise);

	// used by: Text_Initalize, Text_Clear, Text_Update
	result &= hook_write_jmpret(0x0046aca0, LegoRR::Text_UpdatePositionAndSize);

	// used by: Level_Free, NERPsFile_LoadScriptFile, NERPsFile_Free
	result &= hook_write_jmpret(0x0046ad50, LegoRR::Text_Clear);

	// used by: Advisor_LoadPositions, Lego_LoadTextMessages
	result &= hook_write_jmpret(0x0046ad90, LegoRR::Text_GetTextType);

	// used by: Text_SetMessageWithImage,Lego_LoadTextMessages
	result &= hook_write_jmpret(0x0046add0, LegoRR::Text_SetMessage);

	// used by: Lego_LoadTextMessages
	result &= hook_write_jmpret(0x0046ae70, LegoRR::Text_SetMessageWithImage);

	// used by: NERPsRuntime_AdvanceMessage, NERPFunc__SetMessage
	result &= hook_write_jmpret(0x0046aee0, LegoRR::Text_SetNERPsMessage);

	// used by: Advisor_Update,AITask_LiveObject_SetAITaskUnk,Mainloop,
	//			Lego_HandleWorld, Level_GenerateCrystal, Level_GenerateOre,
	//			LegoObject_CompleteVehicleUpgrade, LegoObject_UnkBuildingPlaceDirection,
	//			LegoObject_UnkBuildingPlaceDirection, Objective_Update, Priorities_TurnPriorityOff
	result &= hook_write_jmpret(0x0046af20, LegoRR::Text_DisplayMessage);

	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0046afc0, LegoRR::Text_Update);

	return_interop(result);
}



bool interop_hook_LegoRR_ToolTip(void)
{
	bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x0046b490, LegoRR::ToolTip_Initialise);
	// used by: Front_MenuItem_CreateImageItem, Lego_LoadPanelButtons, Lego_LoadToolTips
	result &= hook_write_jmpret(0x0046b790, LegoRR::ToolTip_GetType);
	// used by: HelpWindow_LoadButtons, Interface_LoadBackButton, Interface_DoSomethingWithRenameReplace,
	//          Lego_HandleRadarInput, Lego_ShowObjectToolTip, Lego_ShowBlockToolTip, Lego_LoadToolTips,
	//          Priorities_HandleInput
	result &= hook_write_jmpret(0x0046b7e0, LegoRR::ToolTip_SetContent);
	// used by: Interface_DoSomethingWithRenameReplace, Lego_ShowObjectToolTip
	result &= hook_write_jmpret(0x0046b920, LegoRR::ToolTip_AddIcon);
	// used by: Lego_HandleRadarInput, Lego_ShowObjectToolTip, Lego_ShowBlockToolTip, Lego_LoadToolTips
	result &= hook_write_jmpret(0x0046b9c0, LegoRR::ToolTip_SetSFX);
	// used by: Interface_Initialise
	result &= hook_write_jmpret(0x0046b9f0, LegoRR::ToolTip_SetRightAlign);
	// used by: Front_Menu_Update, HelpWindow, Interface_DoSomethingWithRenameReplace, Lego,
	//          Objective_HandleKeys, Panel_CheckButtonCollision, Priorities_HandleInput,
	//          Reward_HandleButtons
	result &= hook_write_jmpret(0x0046ba30, LegoRR::ToolTip_Activate);
	// used by: Interface_DoSomethingWithRenameReplace
	result &= hook_write_jmpret(0x0046ba60, LegoRR::ToolTip_ShowInstant);
	// used by: Front_ScreenMenuLoop, Lego_MainLoop, Reward_LoopUpdate
	result &= hook_write_jmpret(0x0046ba80, LegoRR::ToolTip_Update);
	// used by: Reward_DrawItem, ToolTip_Draw
	result &= hook_write_jmpret(0x0046bb70, LegoRR::ToolTip_DrawBox);
	// used by: ToolTip_Update
	result &= hook_write_jmpret(0x0046bef0, LegoRR::ToolTip_Draw);
	
	return_interop(result);
}

bool interop_hook_LegoRR_Vehicle(void)
{
	bool result = true;

	// used by: LegoObject_FP_GetPositionAndHeading
	result &= hook_write_jmpret(0x0046c690, LegoRR::Vehicle_IsCameraFlipDir);
	// used by: LegoObject_UpdateActivityChange
	result &= hook_write_jmpret(0x0046c6b0, LegoRR::Vehicle_SetActivity);
	// used by: Vehicle_SetActivity, Vehicle_SetUpgradeLevel
	result &= hook_write_jmpret(0x0046c7d0, LegoRR::Vehicle_SetUpgradeActivity);
	// used by: Lego_LoadVehicleTypes
	result &= hook_write_jmpret(0x0046c9b0, LegoRR::Vehicle_Load);
	// used by: Lego_Shutdown_Full, LegoObject_Remove
	result &= hook_write_jmpret(0x0046d0d0, LegoRR::Vehicle_Remove);
	// used by: Vehicle_SwapPolyHigh, LegoObject_FP_Callback_SwapPolyMeshParts
	result &= hook_write_jmpret(0x0046d190, LegoRR::Vehicle_SwapPolyMedium);
	// used by: LegoObject_FP_Callback_SwapPolyMeshParts
	result &= hook_write_jmpret(0x0046d1e0, LegoRR::Vehicle_SwapPolyHigh);
	// used by: LegoObject_HasEnoughOreToUpgrade, Lego_HandleDebugKeys
	result &= hook_write_jmpret(0x0046d200, LegoRR::Vehicle_CanUpgradeType);
	// used by: Vehicle_CanUpgradeType, LegoObject_CompleteVehicleUpgrade
	result &= hook_write_jmpret(0x0046d240, LegoRR::Vehicle_GetUpgradeLevel);

	// used by: Lego_HandleWorldDebugKeys, LegoObject_Create, LegoObject_CompleteVehicleUpgrade,
	//          LegoObject_UpdateRemoval, Vehicle_Remove
	result &= hook_write_jmpret(0x0046d250, LegoRR::Vehicle_SetUpgradeLevel);
	// used by: LegoObject_FUN_00438720, LegoObject_UpdateTeleporter
	result &= hook_write_jmpret(0x0046d280, LegoRR::Vehicle_HideWheels);
	// used by: LegoObject_Create
	result &= hook_write_jmpret(0x0046d2b0, LegoRR::Vehicle_Clone);
	// used by: LegoObject_Create
	result &= hook_write_jmpret(0x0046d400, LegoRR::Vehicle_SetOwnerObject);
	// used by: LegoObject_Callback_Update
	result &= hook_write_jmpret(0x0046d460, LegoRR::Vehicle_GetAnimationTime);
	// used by: LegoObject_MoveAnimation
	result &= hook_write_jmpret(0x0046d480, LegoRR::Vehicle_MoveAnimation);
	// used by: Vehicle_SetActivity, Vehicle_Clone
	result &= hook_write_jmpret(0x0046d520, LegoRR::Vehicle_PopulateWheelNulls);
	// used by: Lego_LoadVehicleTypes, LegoObject_Hide
	result &= hook_write_jmpret(0x0046d580, LegoRR::Vehicle_Hide);
	// used by: LegoObject_IsHidden
	result &= hook_write_jmpret(0x0046d5f0, LegoRR::Vehicle_IsHidden);
	// used by: LegoObject_SetHeadingOrDirection, LegoObject_UpdateRoutingVectors, LegoObject_SetPositionAndHeading
	result &= hook_write_jmpret(0x0046d610, LegoRR::Vehicle_SetOrientation);
	// used by: LegoObject_UpdateRoutingVectors, LegoObject_SetPositionAndHeading, LegoObject_FP_UpdateMovement, LegoObject_UpdateWorldStickyPosition
	result &= hook_write_jmpret(0x0046d640, LegoRR::Vehicle_SetPosition);
	// used by: LegoObject_UpdateRoutingVectors, LegoObject_GetActivityContainer
	result &= hook_write_jmpret(0x0046dca0, LegoRR::Vehicle_GetActivityContainer);
	// used by: LegoObject_UpdateRoutingVectors, LegoObject_GetActivityContainer
	result &= hook_write_jmpret(0x0046dcb0, LegoRR::Vehicle_FindNull);
	// used by: LegoObject_UpdateActivityChange, LegoObject_FP_GetPositionAndHeading
	result &= hook_write_jmpret(0x0046dd10, LegoRR::Vehicle_GetCameraNull);
	// used by: LegoObject_GetDrillNullPosition
	result &= hook_write_jmpret(0x0046dd50, LegoRR::Vehicle_GetDrillNull);
	// used by: LegoObject_GetDepositNull
	result &= hook_write_jmpret(0x0046dd80, LegoRR::Vehicle_GetDepositNull);
	// used by: LegoObject_UpdateDriverStickyPosition
	result &= hook_write_jmpret(0x0046ddb0, LegoRR::Vehicle_GetDriverNull);
	// used by: LegoObject_UpdateCarrying
	result &= hook_write_jmpret(0x0046dde0, LegoRR::Vehicle_GetCarryNull);
	// used by: LegoObject_Create
	result &= hook_write_jmpret(0x0046de20, LegoRR::Vehicle_GetCarryNullFrames);
	// used by: LegoObject_CalculateSpeeds
	result &= hook_write_jmpret(0x0046de30, LegoRR::Vehicle_GetTransCoef);
	
	return_interop(result);
}

bool interop_hook_LegoRR_Water(void)
{
	bool result = true;

	// used by: Lego_LoadMapSet
	result &= hook_write_jmpret(0x0046de50, LegoRR::Water_Initialise);
	// used by: Water_Initialise
	result &= hook_write_jmpret(0x0046dfd0, LegoRR::Water_InitPoolDrains);
	// used by: Water_Initialise
	result &= hook_write_jmpret(0x0046e140, LegoRR::Water_InitVertices);
	// used by: Lego_SetViewMode
	result &= hook_write_jmpret(0x0046e480, LegoRR::Water_ChangeViewMode_removed);
	// used by: Level_DestroyWall
	result &= hook_write_jmpret(0x0046e4e0, LegoRR::Water_DestroyWallComplete);
	// used by: Level_Debug_WKey_NeedsBlockFlags1_8_FUN_004303a0
	result &= hook_write_jmpret(0x0046e5f0, LegoRR::Water_DamBlock);
	// used by: Lego_MainLoop
	result &= hook_write_jmpret(0x0046e650, LegoRR::Water_Update);
	// used by: Water_Update
	result &= hook_write_jmpret(0x0046e8d0, LegoRR::Water_UpdateNotHotBlocks);
	// used by: Level_Debug_WKey_NeedsBlockFlags1_8_FUN_004303a0, Water_DestroyWallComplete, Water_DamBlock
	result &= hook_write_jmpret(0x0046eb60, LegoRR::Water_FindPoolDrain);
	// used by: Water_Initialise
	result &= hook_write_jmpret(0x0046ec60, LegoRR::Water_QsortComparePools);
	// used by: Water_Initialise
	result &= hook_write_jmpret(0x0046ec90, LegoRR::Water_FindPoolAndMergeRows);
	// used by: Water_Initialise, Water_AddPool
	result &= hook_write_jmpret(0x0046ed90, LegoRR::Water_AddPoolRowBlocks);
	// used by: Water_Initialise
	result &= hook_write_jmpret(0x0046edf0, LegoRR::Water_AddPool);
	
	return_interop(result);
}

bool interop_hook_LegoRR_Weapons(void)
{
	bool result = true;

	//result &= hook_write_jmpret(0x0046ee40, LegoRR::Weapon_Initialise);
	result &= hook_write_jmpret(0x0046f390, LegoRR::Weapon_GetWeaponIDByName);
	result &= hook_write_jmpret(0x0046f3d0, LegoRR::Weapon_GetRechargeTime);
	result &= hook_write_jmpret(0x0046f400, LegoRR::Weapon_GetDischargeRate);
	result &= hook_write_jmpret(0x0046f430, LegoRR::Weapon_GetWeaponRange);
	result &= hook_write_jmpret(0x0046f460, LegoRR::Weapon_GetWallDestroyTime);
	result &= hook_write_jmpret(0x0046f490, LegoRR::Weapon_GetDamageForObject);
	result &= hook_write_jmpret(0x0046f530, LegoRR::Weapon_GenericDamageObject);
	result &= hook_write_jmpret(0x0046f640, LegoRR::Weapon_GunDamageObject);
	result &= hook_write_jmpret(0x0046f670, LegoRR::Weapon_Projectile_FUN_0046f670);
	//result &= hook_write_jmpret(0x0046f810, LegoRR::Weapon_Update);
	result &= hook_write_jmpret(0x0046f8d0, LegoRR::Weapon_LegoObject_Callback_UpdateObject);
	result &= hook_write_jmpret(0x0046fa30, LegoRR::Weapon_GunHitObject);
	result &= hook_write_jmpret(0x0046fbe0, LegoRR::Weapon_Projectile_UpdatePath);
	result &= hook_write_jmpret(0x0046fdb0, LegoRR::Weapon_Projectile_AddStraightPath);
	result &= hook_write_jmpret(0x0046ff30, LegoRR::Weapon_Projectile_AddCurvedPath);
	result &= hook_write_jmpret(0x004701b0, LegoRR::Weapon_GetObjectTypeAndID_ByKnownWeaponType);
	result &= hook_write_jmpret(0x00470230, LegoRR::Weapon_Projectile_GetNextAvailable);
	result &= hook_write_jmpret(0x00470250, LegoRR::Weapon_Lazer_GetNextAvailable);
	result &= hook_write_jmpret(0x00470270, LegoRR::Weapon_FireLazer);
	result &= hook_write_jmpret(0x00470520, LegoRR::Weapon_LegoObject_Collision_FUN_00470520);
	result &= hook_write_jmpret(0x00470570, LegoRR::Weapon_LegoObject_CollisionBox_FUN_00470570);
	result &= hook_write_jmpret(0x00470800, LegoRR::Weapon_LegoObject_CollisionRadius_FUN_00470800);
	result &= hook_write_jmpret(0x004708f0, LegoRR::Weapon_LegoObject_TestCollision_FUN_004708f0);
	result &= hook_write_jmpret(0x00470950, LegoRR::Weapon_Lazer_Add);
	result &= hook_write_jmpret(0x00470a20, LegoRR::Weapon_Lazer_InitMesh);
	result &= hook_write_jmpret(0x00471580, LegoRR::Weapon_LegoObject_GetWeaponsModel);
	result &= hook_write_jmpret(0x004715b0, LegoRR::Weapon_LegoObject_GetWeaponTimer);
	result &= hook_write_jmpret(0x004715d0, LegoRR::Weapon_LegoObject_SetWeaponTimer);
	result &= hook_write_jmpret(0x004715f0, LegoRR::Weapon_MathUnk_CheckVectorsZScalar_InRange);
	result &= hook_write_jmpret(0x00471630, LegoRR::Weapon_LegoObject_Callback_FUN_00471630);
	result &= hook_write_jmpret(0x004718f0, LegoRR::Weapon_LegoObject_FUN_004718f0);
	result &= hook_write_jmpret(0x00471b20, LegoRR::Weapon_DoCallbacksSearch_FUN_00471b20);
	result &= hook_write_jmpret(0x00471b90, LegoRR::Weapon_LegoObject_DoCallbacksSearch_FUN_00471b90);
	result &= hook_write_jmpret(0x00471c20, LegoRR::Weapon_LegoObject_SeeThroughWalls_FUN_00471c20);
	result &= hook_write_jmpret(0x00471ce0, LegoRR::Weapon_GetFireNull);
	result &= hook_write_jmpret(0x00471d00, LegoRR::Weapon_GetXPivotNull);
	result &= hook_write_jmpret(0x00471d10, LegoRR::Weapon_GetYPivotNull);
	result &= hook_write_jmpret(0x00471d20, LegoRR::Weapon_PivotTracker);
	result &= hook_write_jmpret(0x00471f30, LegoRR::Weapon_GetFireDirection);
	result &= hook_write_jmpret(0x00471f60, LegoRR::Weapon_LegoObject_GetCollCenterPosition);
	result &= hook_write_jmpret(0x00471fa0, LegoRR::Weapon_LegoObject_IsActiveWithTracker);
	result &= hook_write_jmpret(0x00471fe0, LegoRR::Weapon_LegoObject_UpdateSelectedTracker);
	result &= hook_write_jmpret(0x004721c0, LegoRR::Weapon_LegoObject_UpdateUnselectedTracker);
	result &= hook_write_jmpret(0x00472280, LegoRR::Weapon_LegoObject_UpdateTracker);
	result &= hook_write_jmpret(0x00472320, LegoRR::Weapon_Callback_RemoveProjectileReference);
	result &= hook_write_jmpret(0x00472340, LegoRR::Weapon_LegoObject_WithinWeaponRange);
	result &= hook_write_jmpret(0x004723f0, LegoRR::Weapon_LegoObject_WithinAwarenessRange);

	return_interop(result);
}

#pragma endregion


#pragma region LegoRR Game Special Hooks
bool interop_hook_replace_LegoRR_PanelRadarMapZoom(void)
{
	bool result = true;

	// Okay, so the Panel_CheckCollision function is really large, and taking the time to decompile it will be a hassle.
	// So this replaces the snippets we need to change involving radar zoom and replaces them with function calls.

	// MOV  EAX,dword ptr [ESP + elapsedAbs]
	// PUSH EAX
	// CALL Panal_RadarMap_Zoom*
	// ADD  ESP,0x4

	static constexpr const uint8 movEAX[]  = { 0x8b, 0x44, 0x24, 0x14 }; // MOV EAX,dword ptr [ESP + elapsedAbs]
	static constexpr const uint8 pushEAX[] = { 0x50 }; // PUSH EAX
	static constexpr const uint8 addESP[]  = { 0x83, 0xc4, 0x04 }; // ADD ESP,0x4

	static constexpr const uint32 startAddrs[] = { 0x0045b291, 0x0045b2d8 };
	static constexpr const uint32 endAddrs[]   = { 0x0045b2d1, 0x0045b318 };
	static constexpr void* funcAddrs[]         = { LegoRR::Panel_RadarMap_ZoomIn, LegoRR::Panel_RadarMap_ZoomOut };

	uint8 slide[std::max(endAddrs[0]-startAddrs[0], endAddrs[1]-startAddrs[1])];

	for (uint32 i = 0; i < 2; i++) {
		const uint32 startAddr = startAddrs[i];
		const uint32 size = endAddrs[i] - startAddrs[i];

		// NOP slide everything before changing the assembly.
		std::memset(slide, 0x90, size); // NOP
		result &= hook_write(startAddr, slide, size);

		// Write the assembly instructions.
		uint32 offset = 0;

		result &= hook_write(startAddr + offset, movEAX, sizeof(movEAX));
		offset += sizeof(movEAX);

		result &= hook_write(startAddr + offset, pushEAX, sizeof(pushEAX));
		offset += sizeof(pushEAX);

		result &= hook_write_call(startAddr + offset, funcAddrs[i]);
		offset += 5;

		result &= hook_write(startAddr + offset, addESP, sizeof(addESP));
		offset += sizeof(addESP);
	}

	return_interop(result);
}
#pragma endregion


#pragma region Hook All

bool interop_hook_all(void)
{
	bool result = true;

	// First patch in merged functions and individual calls:
	// Do this now to prevent messing up any jmpret function hooks
	//  (that contain calls being individually hooked).
	
	#pragma region Merged Calls
	result &= interop_hook_calls_WINMM_timeGetTime();
	result &= interop_hook_calls_Gods98_AnimClone();
	result &= interop_hook_calls_Gods98_Flic();
	#pragma endregion


	// Now patch in all jmpret function hooks:


	// Add C Runtime hooks here:

	#pragma region C Runtime
	result &= interop_hook_CRT_rand();
	#pragma endregion


	// Add Engine hooks here:

	#pragma region Gods98 Engine
	result &= interop_hook_Gods98_3DSound();
	result &= interop_hook_Gods98_Animation();
	result &= interop_hook_Gods98_AnimClone();
	result &= interop_hook_Gods98_Bmp();
	result &= interop_hook_Gods98_Compress();
	result &= interop_hook_Gods98_Config();
	result &= interop_hook_Gods98_Containers();
	result &= interop_hook_Gods98_DirectDraw();
	result &= interop_hook_Gods98_Draw();
	result &= interop_hook_Gods98_Dxbug(); // used by: Viewport_Render, Main_SetupDirect3D, Image_GetScreenshot, Input_InitKeysAndDI
	result &= interop_hook_Gods98_Errors(); // nothing hooked, keep if logging funcs are reinstated
	result &= interop_hook_Gods98_Files();
	result &= interop_hook_Gods98_Flic();
	result &= interop_hook_Gods98_Fonts();
	result &= interop_hook_Gods98_Images();
	result &= interop_hook_Gods98_Input();
	result &= interop_hook_Gods98_Keys();
	result &= interop_hook_Gods98_Lws();
	result &= interop_hook_Gods98_Lwt();
	result &= interop_hook_Gods98_Main();
	result &= interop_hook_Gods98_Materials();
	result &= interop_hook_Gods98_Maths();
	result &= interop_hook_Gods98_Memory();
	result &= interop_hook_Gods98_Mesh();
	result &= interop_hook_Gods98_Movie();
	//result &= interop_hook_Gods98_Registry(); // no need to hook, used by: WinMain, File_Initialise
	result &= interop_hook_Gods98_Sound();
	result &= interop_hook_Gods98_TextWindow();
	result &= interop_hook_Gods98_Utils();
	result &= interop_hook_Gods98_Viewports();
	//result &= interop_hook_Gods98_Wad(); // no need to hook, used by: Files
	//result &= interop_hook_Gods98_Init(); // no need to hook, used by: WinMain
	#pragma endregion


	// Add Game hooks here:

	#pragma region LegoRR Special
	result &= interop_hook_replace_LegoRR_PanelRadarMapZoom();
	#pragma endregion

	#pragma region LegoRR Game
	result &= interop_hook_LegoRR_Advisor();
	result &= interop_hook_LegoRR_AITask();
	result &= interop_hook_LegoRR_BezierCurve();
	result &= interop_hook_LegoRR_Bubbles();
	result &= interop_hook_LegoRR_Building();
	result &= interop_hook_LegoRR_Collision();
	result &= interop_hook_LegoRR_Construction();
	result &= interop_hook_LegoRR_Creature();
	result &= interop_hook_LegoRR_Credits();
	result &= interop_hook_LegoRR_DamageText();
	result &= interop_hook_LegoRR_Dependencies();
	result &= interop_hook_LegoRR_Effect();
	result &= interop_hook_LegoRR_ElectricFence();
	result &= interop_hook_LegoRR_Encyclopedia();
	result &= interop_hook_LegoRR_Erosion();
	result &= interop_hook_LegoRR_Fallin();
	result &= interop_hook_LegoRR_FrontEnd();
	result &= interop_hook_LegoRR_Game();
	result &= interop_hook_LegoRR_InfoMessages();
	result &= interop_hook_LegoRR_Interface();
	result &= interop_hook_LegoRR_LegoCamera();
	result &= interop_hook_LegoRR_LightEffects();
	result &= interop_hook_LegoRR_Loader();
	result &= interop_hook_LegoRR_MeshLOD();
	result &= interop_hook_LegoRR_Messages();
	result &= interop_hook_LegoRR_NERPsFile();
	result &= interop_hook_LegoRR_NERPsFunctions();
	result &= interop_hook_LegoRR_Object();
	result &= interop_hook_LegoRR_ObjectRecall();
	result &= interop_hook_LegoRR_Objective();
	result &= interop_hook_LegoRR_ObjInfo();
	result &= interop_hook_LegoRR_Pointers();
	result &= interop_hook_LegoRR_Priorities();
	result &= interop_hook_LegoRR_PTL();
	result &= interop_hook_LegoRR_RadarMap();
	result &= interop_hook_LegoRR_Reward();
	result &= interop_hook_LegoRR_Roof();
	result &= interop_hook_LegoRR_SelectPlace();
	result &= interop_hook_LegoRR_SFX();
	result &= interop_hook_LegoRR_Smoke();
	result &= interop_hook_LegoRR_Stats();
	result &= interop_hook_LegoRR_TextMessages();
	result &= interop_hook_LegoRR_ToolTip();
	result &= interop_hook_LegoRR_Vehicle();
	result &= interop_hook_LegoRR_Water();
	result &= interop_hook_LegoRR_Weapons();
	#pragma endregion



	// Add temporary hooks used for debugging here:

	#pragma region Temporary Hooks
	// Do not commit anything in here!
	#pragma endregion


	return_interop(result);
}

#pragma endregion
