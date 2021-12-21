
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
#include "game/front/FrontEnd.h"
#include "game/interface/Messages.h"
#include "game/object/BezierCurve.h"
#include "game/object/Object.h"
#include "game/object/Stats.h"
#include "game/objective/NERPsFile.h"
#include "game/objective/PTL.h"
#include "game/world/Camera.h"
#include "game/Game.h"


#define return_interop(result) { std::printf("%s %s\n", __FUNCTION__, (result?"OK":"failed")); } return result;


bool interop_hook_WinMain_call(void)
{   bool result = true;
	result &= hook_write_call(0x0048f3fa, StartOpenLRRInjected, nullptr);
	return_interop(result);
}

bool interop_hook_Gods98_WinMain(void)
{   bool result = true;
	//result &= hook_write_jmpret(0x00477a60, Gods98::Main_WinMain);
	result &= hook_write_jmpret(0x00477a60, StartOpenLRRInjected);
	//return_interop(result);
	return result;
}


bool interop_hook_Gods98_3DSound(void)
{   bool result = true;
	// used by: Sound_Initialise
	result &= hook_write_jmpret(0x0047a900, Gods98::Sound3D_Initialise);

	// used by: Lego_Shutdown_Debug, Lego_HandleKeys
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
{   bool result = true;
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

bool interop_hook_Gods98_AnimClone(void)
{   bool result = true;

	// used by: Container_LoadAnimSet
	result &= hook_write_jmpret(0x004897e0, Gods98::AnimClone_Register);
	result &= hook_write_jmpret(0x00489880, Gods98::AnimClone_RegisterLws);
	// used by: Container_Clone, Container_AddActivity2
	result &= hook_write_jmpret(0x00489920, Gods98::AnimClone_Make);
	// used by: Container_Remove2
	result &= hook_write_jmpret(0x00489a10, Gods98::AnimClone_Remove);

	// (shared) "AnimClone_IsLws__Flic_GetWidth"
	// THIS FUNCTION MUST BE HOOKED ON AN INDIVIDUAL BASIS
	// There are 5 calls made to this:
	//  type:Flic (Flic_GetWidth)  -> FUN_004120e0  <@004120f7>
	//                                      Panel_FUN_0045a9f0  <@0045ab17>
	//                                      Pointer_DrawPointer  <@0045cfc8>
	//  type:FlocksData (Flocks_???)     -> LiveObject_Flocks_FUN_0044bef0  <@0044bfc3>
	//  type:AnimClone (AnimClone_IsLws) -> Container_FormatPartName  <@00473f60>
	// <called @00473f60>
	result &= hook_write_jmpret(0x00473f60, Gods98::AnimClone_IsLws);

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
{   bool result = true;
	// used by: Container_LoadTextureSurface, Image_LoadBMPScaled
	result &= hook_write_jmpret(0x00489ef0, Gods98::BMP_Parse);
	result &= hook_write_jmpret(0x0048a030, Gods98::BMP_Cleanup);
	return_interop(result);
}

bool interop_hook_Gods98_Compress(void)
{   bool result = true;

	// this and one subfunction are implemented, but beyond that, everything is calling native code
	result &= hook_write_jmpret(0x0049ca80, Gods98::RNC_Uncompress);

	return_interop(result);
}

bool interop_hook_Gods98_Config(void)
{   bool result = true;
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
{   bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x004729d0, Gods98::Container_Initialise);
	// used by: Lego_Initialise, Lego_Shutdown_Debug
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

	// used by: SFX_Container_Random_Play_OrInitSoundUnk
	result &= hook_write_jmpret(0x00473940, Gods98::Container_GetMasterFrame);

	result &= hook_write_jmpret(0x00473950, Gods98::Container_Clone);

	// used by: LiveObject_Hide2
	result &= hook_write_jmpret(0x00473de0, Gods98::Container_Hide2);

	result &= hook_write_jmpret(0x00473e00, Gods98::Container_Hide);

	// used by: Object_IsHidden, LiveObject_IsHidden,
	//           LiveObject_Flocks_FUN_0044bef0
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
	//           SelectPlace_DrawTiles
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

	// used by: Creature_GetThrowNull
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
{   bool result = true;

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048a050, Gods98::SE);

	// used by: Viewport_Render, Main_SetupDirect3D, Image_GetScreenshot,
	//           Input_InitKeysAndDI
	result &= hook_write_jmpret(0x0048a090, Gods98::Error_SetDXError);
	return_interop(result);
}

bool interop_hook_Gods98_DirectDraw(void)
{   bool result = true;

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

	// used by: Main_LoopUpdate, Lego_Shutdown_Debug, RadarMap_Draw_FUN_0045de80
	result &= hook_write_jmpret(0x0047d0e0, Gods98::DirectDraw_Clear);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047d1a0, Gods98::DirectDraw_CreateClipper);

	// used by: Container_LoadTextureSurface
	result &= hook_write_jmpret(0x0047d2c0, Gods98::DirectDraw_Blt8To16);
	result &= hook_write_jmpret(0x0047d590, Gods98::DirectDraw_GetColour);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047d6b0, Gods98::DirectDraw_GetNumberOfBits);

	return_interop(result);
}

bool interop_hook_Gods98_Draw(void)
{   bool result = true;

	// used by: WinMain
	result &= hook_write_jmpret(0x00486140, Gods98::Draw_Initialise);

	result &= hook_write_jmpret(0x00486160, Gods98::Draw_SetClipWindow);
	result &= hook_write_jmpret(0x00486270, Gods98::Draw_GetClipWindow);
	result &= hook_write_jmpret(0x004862b0, Gods98::Draw_LineListEx);

	// used by: ObjInfo_DrawHealthBar, ToolTip_DrawBox
	result &= hook_write_jmpret(0x00486350, Gods98::Draw_RectListEx);

	// used by: RadarMap_Draw_FUN_0045de80
	result &= hook_write_jmpret(0x004864d0, Gods98::Draw_RectList2Ex);
	// used by: RadarMap_DrawDotCircle_FUN_0045ddc0
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
{   bool result = true;
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
{   bool result = true;
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

bool interop_hook_Gods98_Flic(void)
{   bool result = true;

	result &= hook_write_jmpret(0x00483f40, Gods98::Flic_Setup);
	result &= hook_write_jmpret(0x004841c0, Gods98::Flic_Close);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x00484220, Gods98::Flic_LoadHeader);

	result &= hook_write_jmpret(0x00484330, Gods98::Flic_Animate);

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

	// (shared) "AnimClone_IsLws__Flic_GetWidth"
	// THIS FUNCTION MUST BE HOOKED ON AN INDIVIDUAL BASIS
	// There are 5 calls made to this:
	//  type:Flic (Flic_GetWidth)  -> FUN_004120e0  <@004120f7>
	//                                      Panel_FUN_0045a9f0  <@0045ab17>
	//                                      Pointer_DrawPointer  <@0045cfc8>
	//  type:FlocksData (Flocks_???)     -> LiveObject_Flocks_FUN_0044bef0  <@0044bfc3>
	//  type:AnimClone (AnimClone_IsLws) -> Container_FormatPartName  <@00473f60>
	// <called @004120f7, 0045ab17, 0045cfc8>
	result &= hook_write_call(0x004120f7, Gods98::Flic_GetWidth);
	result &= hook_write_call(0x0045ab17, Gods98::Flic_GetWidth);
	result &= hook_write_call(0x0045cfc8, Gods98::Flic_GetWidth);

	result &= hook_write_jmpret(0x004853a0, Gods98::Flic_GetHeight);

	return_interop(result);
}

bool interop_hook_Gods98_Fonts(void)
{   bool result = true;

	result &= hook_write_jmpret(0x00401b90, Gods98::noinline(Font_GetStringWidth));

	// used by: Loader_Initialise, ToolTip_SetText
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

	// used by: Lego_Shutdown_Debug, Reward_CleanupBaseFont
	result &= hook_write_jmpret(0x0047a810, Gods98::Font_Remove);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047a840, Gods98::Font_Create);
	//result &= hook_write_jmpret(0x0047a880, Gods98::Font_AddList);

	return_interop(result);
}

bool interop_hook_Gods98_Images(void)
{   bool result = true;

	// used by: Lego_Initialise (new Gods98 calls in WinMain)
	result &= hook_write_jmpret(0x0047d6d0, Gods98::Image_Initialise);

	// used by: Lego_Shutdown_Debug
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
	//result &= hook_write_jmpret(0x0047e1b0, Gods98::Image_GetPen255);
	//result &= hook_write_jmpret(0x0047e210, Gods98::Image_GetPixelMask);

	result &= hook_write_jmpret(0x0047e260, Gods98::Image_GetPixel);

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
{   bool result = true;
	// used by: LiveObject_FUN_00471fe0
	result &= hook_write_jmpret(0x00410a60, Gods98::noinline(msx));
	result &= hook_write_jmpret(0x00410a70, Gods98::noinline(msy));
	result &= hook_write_jmpret(0x00410a80, Gods98::noinline(mslb));

	// used by: WinMain
	//result &= hook_write_jmpret(0x0047f050, Gods98::Input_InitKeysAndDI);

	// used by: WinMain, Main_LoopUpdate, NERPFunc__SetMessage
	result &= hook_write_jmpret(0x0047f1b0, Gods98::Input_ReadKeys);

	// used by: Credits_Play, Front_PlayMovie, Front_ShowIntroSplash, NERPFunc__GetAnyKeyPressed
	result &= hook_write_jmpret(0x0047f270, Gods98::Input_AnyKeyPressed);

	// used by: WinMain
	//result &= hook_write_jmpret(0x0047f290, Gods98::Input_ReleaseKeysAndDI);
	// used by: WinMain, Main_LoopUpdate
	//result &= hook_write_jmpret(0x0047f2d0, Gods98::Input_ReadMouse2);

	// used by: Objective_DoHotkeyChecks, Panel_RotationControl_FUN_0045bf90,
	//           Priorities_FUN_0045d810, Priorities_FUN_0045d900
	result &= hook_write_jmpret(0x0047f390, Gods98::Input_SetCursorPos);
	return_interop(result);
}

bool interop_hook_Gods98_Keys(void)
{   bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00485ce0, Gods98::Keys_Initialise);

	// used by: Interface_LoadInterfaceBuildImages (FUN_0041aa30)
	//           Interface_LoadInterfaceImages (FUN_0041acd0)
	result &= hook_write_jmpret(0x004860f0, Gods98::Key_Find);
	return_interop(result);
}

bool interop_hook_Gods98_Lws(void)
{   bool result = true;
	// used by: Container_LoadAnimSet
	result &= hook_write_jmpret(0x00486cb0, Gods98::Lws_Parse);

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00487980, Gods98::Lws_Initialise);
	// used by: Lego_Shutdown_Debug
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
{   bool result = true;
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
{   bool result = true;

	// used by: Lego_Initialise, Info_Send
	result &= hook_write_jmpret(0x00401b30, Gods98::noinline(Main_ProgrammerMode));
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00401b40, Gods98::noinline(Main_GetStartLevel));
	
	// used by: Lego_Initialise, LiveObject_DrawSelectedBox,
	//           RadarMap_Initialise, RadarMap_Draw_FUN_0045de80
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
{   bool result = true;
	// used by: Map3D_LoadSurfaceMap
	result &= hook_write_jmpret(0x00489780, Gods98::Material_Create);
	return_interop(result);
}

bool interop_hook_Gods98_Maths(void)
{   bool result = true;
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
{   bool result = true;
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
{   bool result = true;

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
{   bool result = true;
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
{   bool result = true;
	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048b5f0, Gods98::Registry_GetKeyFromPath);

	// used by: WinMain, File_Initialise
	result &= hook_write_jmpret(0x0048b620, Gods98::Registry_GetValue);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048b650, Gods98::Registry_GetValue_Recursive);
	return_interop(result);
}

bool interop_hook_Gods98_Sound(void)
{   bool result = true;

	// used by: WinMain
	result &= hook_write_jmpret(0x00488e10, Gods98::Sound_Initialise);

	// used by: Lego_Initialise, Lego_Shutdown_Debug
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
{   bool result = true;

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
{   bool result = true;
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
{   bool result = true;

	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00477010, Gods98::Viewport_Initialise);
	// used by: Lego_Shutdown_Debug
	result &= hook_write_jmpret(0x00477040, Gods98::Viewport_Shutdown);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00477080, Gods98::Viewport_Create);
	result &= hook_write_jmpret(0x00477110, Gods98::Viewport_CreatePixel);

	// used by: Lego_FPHighPolyBlocks_FUN_00433db0
	result &= hook_write_jmpret(0x004771d0, Gods98::Viewport_GetSize);

	// used by: Lego_SetViewMode
	result &= hook_write_jmpret(0x00477210, Gods98::Viewport_SetCamera);

	// used by: DamageFont_MeshRenderCallback, SaveMenu_FUN_00412b30
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

	// used by: Lego_Shutdown_Debug
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
{   bool result = true;

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
{   bool result = true;

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


bool interop_hook_LegoRR_BezierCurve(void)
{   bool result = true;

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

bool interop_hook_LegoRR_FrontEnd(void)
{   bool result = true;

	// QoL apply for always-skippable splash screens and movies
	result &= hook_write_jmpret(0x00415630, LegoRR::Front_PlayMovie);
	result &= hook_write_jmpret(0x004156f0, LegoRR::Front_ShowIntroSplash);
	result &= hook_write_jmpret(0x00415840, LegoRR::Front_ShowIntroMovie);

	return_interop(result);
}

bool interop_hook_LegoRR_LegoCamera(void)
{   bool result = true;
	
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00435a50, LegoRR::Camera_Create);
	// used by: Lego_HandleKeys
	result &= hook_write_jmpret(0x00435cc1, LegoRR::Camera_EnableFreeMovement);
	// used by: Lego_Shutdown_Debug
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

bool interop_hook_LegoRR_Messages(void)
{   bool result = true;
	
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x00451f90, LegoRR::Message_Initialise);

	result &= hook_write_jmpret(0x00452220, LegoRR::Message_RemoveLiveObject);

	// used by: PTL_Initialise
	result &= hook_write_jmpret(0x00452290, LegoRR::Message_LookupPTLEventIndex);
	// used by: Lego_Initialise
	result &= hook_write_jmpret(0x004522d0, LegoRR::Message_Debug_RegisterSelectedUnitHotkey);

	result &= hook_write_jmpret(0x00452320, LegoRR::Message_AddMessageAction);

	/// NOT IMPLEMENTED YET
	//result &= hook_write_jmpret(0x00452390, LegoRR::Message_PTL_Update);
	//result &= hook_write_jmpret(0x004526f0, LegoRR::Message_PTL_PickRandomFloor);
	//result &= hook_write_jmpret(0x00452770, LegoRR::Message_LiveObject_FUN_00452770);

	result &= hook_write_jmpret(0x004527e0, LegoRR::Message_CopySelectedUnits);
	result &= hook_write_jmpret(0x00452840, LegoRR::Message_GetSelectedUnits2);
	result &= hook_write_jmpret(0x00452870, LegoRR::Message_AnyUnitSelected);
	result &= hook_write_jmpret(0x00452880, LegoRR::Message_GetPrimarySelectedUnit);
	result &= hook_write_jmpret(0x004528a0, LegoRR::Message_GetSelectedUnits);
	result &= hook_write_jmpret(0x004528b0, LegoRR::Message_GetNumSelectedUnits);

	// used by: Level_Free
	result &= hook_write_jmpret(0x004528c0, LegoRR::Message_CleanupSelectedUnitsCount);

	/// NOT IMPLEMENTED YET
	//result &= hook_write_jmpret(0x004528d0, LegoRR::Message_LiveObject_Check_IsSelected_OrFlags3_200000);

	result &= hook_write_jmpret(0x00452910, LegoRR::Message_FindIndexOfObject);

	/// NOT IMPLEMENTED YET
	//result &= hook_write_jmpret(0x00452950, LegoRR::Message_LiveObject_Check_FUN_00452950);
	//result &= hook_write_jmpret(0x00452980, LegoRR::Message_PTL_Select_LiveObject);
	//result &= hook_write_jmpret(0x004529a0, LegoRR::Message_LiveObject_DoSelect_FUN_004529a0);
	//result &= hook_write_jmpret(0x00452b30, LegoRR::Message_LiveObject_Check_FUN_00452b30);
	//result &= hook_write_jmpret(0x00452b80, LegoRR::Message_PTL_ReduceSelection);
	//result &= hook_write_jmpret(0x00452ea0, LegoRR::Message_PTL_ClearSelection);
	//result &= hook_write_jmpret(0x00452f10, LegoRR::Message_PTL_Deselect_LiveObject);
	//result &= hook_write_jmpret(0x00452f80, LegoRR::Message_PTL_Debug_DestroyAll);
	//result &= hook_write_jmpret(0x00453020, LegoRR::Message_PTL_FirstPerson);

	
	return_interop(result);
}

bool interop_hook_LegoRR_NERPsFile(void)
{   bool result = true;

	// NERPs interpreter functions (except for GetMessageLine)

	result &= hook_write_jmpret(0x004530b0, LegoRR::NERPsFile_LoadScriptFile);
	result &= hook_write_jmpret(0x00453130, LegoRR::NERPsFile_LoadMessageFile);
	result &= hook_write_jmpret(0x004534c0, LegoRR::NERPsFile_GetMessageLine);
	result &= hook_write_jmpret(0x004534e0, LegoRR::NERPsFile_Free);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004535a0, LegoRR::NERPsRuntime_LoadLiteral);

	result &= hook_write_jmpret(0x004535e0, LegoRR::NERPsRuntime_Execute);

	result &= hook_write_jmpret(0x00456af0, LegoRR::NERPs_Level_NERPMessage_Parse);

	return_interop(result);
}

bool interop_hook_LegoRR_PTL(void)
{   bool result = true;
	
	// used by: Lego_LoadLevel
	result &= hook_write_jmpret(0x0045daa0, LegoRR::PTL_Initialise);

	// used by: Message_PTL_Update
	result &= hook_write_jmpret(0x0045db30, LegoRR::PTL_EventToAction);
	
	return_interop(result);
}

bool interop_hook_LegoRR_SFX(void)
{   bool result = true;

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
	result &= hook_write_jmpret(0x00465260, LegoRR::SFX_Random_Play_OrAddToQueue);
	result &= hook_write_jmpret(0x004652d0, LegoRR::SFX_Random_SetBufferVolume);
	result &= hook_write_jmpret(0x004652f0, LegoRR::SFX_Random_GetBufferVolume);
	result &= hook_write_jmpret(0x00465310, LegoRR::SFX_Container_Random_Play_OrInitSoundUnk);
	result &= hook_write_jmpret(0x00465350, LegoRR::SFX_Random_Play_OrInitSoundUnk);
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

bool interop_hook_LegoRR_Stats(void)
{   bool result = true;
	
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
	result &= hook_write_jmpret(0x0046a5d0, LegoRR::StatsObject_GetObjectFreezerTime);
	result &= hook_write_jmpret(0x0046a5f0, LegoRR::StatsObject_Debug_ToggleObjectPower);
	
	return_interop(result);
}


bool interop_hook_all(void)
{   bool result = true;


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


	result &= interop_hook_LegoRR_BezierCurve();
	result &= interop_hook_LegoRR_LegoCamera();
	result &= interop_hook_LegoRR_Messages();
	result &= interop_hook_LegoRR_PTL();
	result &= interop_hook_LegoRR_Stats();

	// Only a few functions from each of these have been
	// defined in order to fix certain original bugs.
	result &= interop_hook_LegoRR_FrontEnd();
	result &= interop_hook_LegoRR_SFX();

	// Implementation for NERPs interpreter.
	result &= interop_hook_LegoRR_NERPsFile();

	//result &= hook_write_jmpret(0x0042c260, LegoRR::Level_HandleEmergeTriggers);

	return_interop(result);
}
