
#include "hook.h"
#include "interop.h"
#include "openlrr.h"
#include "Gods98/3DSound.h"
#include "Gods98/Animation.h"
//#include "Gods98/AnimClone.h"
#include "Gods98/Bmp.h"
#include "Gods98/Compress.h"
#include "Gods98/Config.h"
//#include "Gods98/Containers.h"
#include "Gods98/DirectDraw.h"
#include "Gods98/Draw.h"
#include "Gods98/Dxbug.h"
#include "Gods98/Errors.h"
#include "Gods98/Files.h"
#include "Gods98/Flic.h"
#include "Gods98/Fonts.h"
#include "Gods98/Images.h"
#include "Gods98/Input.h"
#include "Gods98/Keys.h"
//#include "Gods98/Lws.h"
//#include "Gods98/Lwt.h"
#include "Gods98/Main.h"
#include "Gods98/Materials.h"
#include "Gods98/Maths.h"
#include "Gods98/Memory.h"
//#include "Gods98/Mesh.h"
#include "Gods98/Movie.h"
#include "Gods98/Registry.h"
#include "Gods98/Sound.h"
#include "Gods98/TextWindow.h"
#include "Gods98/Utils.h"
#include "Gods98/Viewports.h"
#include "Gods98/Wad.h"
#include "Gods98Init/Init.h"


#define return_interop(result) { std::printf("%s %s\n", __FUNCTION__, (result?"OK":"failed")); } return result;


bool interop_hook_WinMain_call(void)
{   bool result = true;
	result &= hook_write_call(0x0048f3fa, OpenLRR_WinMain, nullptr);
	return_interop(result);
}

bool interop_hook_Gods98_WinMain(void)
{   bool result = true;
	//result &= hook_write_jmpret(0x00477a60, Gods98::Main_WinMain);
	result &= hook_write_jmpret(0x00477a60, OpenLRR_WinMain);
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

	// used by: NERPsFile_LoadMessageFile, SFX_Sample_LoadProperty
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

	// used by: SFX_Sample_Random_SetBufferVolume
	result &= hook_write_jmpret(0x0047b310, Gods98::Sound3D_SetBufferVolume);
	// used by: SFX_Sample_Random_GetBufferVolume
	result &= hook_write_jmpret(0x0047b390, Gods98::Sound3D_GetBufferVolume);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0047b3b0, Gods98::Sound3D_GetSoundBuffer);

	// used by: Lws_HandleTrigger, SFX_Sample_Sound3D_StopSound
	result &= hook_write_jmpret(0x0047b3f0, Gods98::Sound3D_StopSound);

	// used by: Game_SetPaused, Level_Free, Lego_GoBackToMissionSelect,
	//           SFX_SetSoundStates_IsOn_StopAll
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

	// used by: NERPs_Level_NERPMessage_Parse, SFX_Sample_Random_GetSamplePlayTime
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

/*bool interop_hook_Gods98_AnimClone(void)
{   bool result = true;

	return result;
}*/

bool interop_hook_Gods98_Bmp(void)
{   bool result = true;
	// used by: Container_LoadTextureSurface, Image_LoadBMPScaled
	result &= hook_write_jmpret(0x00489ef0, Gods98::BMP_Parse);
	result &= hook_write_jmpret(0x0048a030, Gods98::BMP_Cleanup);
	return_interop(result);
}

bool interop_hook_Gods98_Compress(void)
{   bool result = true;
	// not ready yet, do not hook!
	//result &= hook_write_jmpret(0x0049ca80, Gods98::RNC_Uncompress);
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

/*bool interop_hook_Gods98_Containers(void)
{   bool result = true;

	return result;
}*/

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
	//  type:FLICSTRUCT (Flic_GetWidth)  -> FUN_004120e0  <@004120f7>
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

/*bool interop_hook_Gods98_Lws(void)
{   bool result = true;

	return result;
}*/

/*bool interop_hook_Gods98_Lwt(void)
{   bool result = true;

	return result;
}*/

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
	result &= hook_write_jmpret(0x00477e90, Gods98::Main_DisableTextureManagement);

	// used by: WinMain
	//result &= hook_write_jmpret(0x00477eb0, Gods98::Main_ParseCommandLine);

	result &= hook_write_jmpret(0x004781f0, Gods98::Main_LoopUpdate);

	// used by: Level_BlockUpdateSurface
	result &= hook_write_jmpret(0x00478230, Gods98::Main_GetCLFlags);

	// used by: DirectDraw_EnumModeCallback, Init_SetModeList
	//result &= hook_write_jmpret(0x00478240, Gods98::Main_GetWindowsBitDepth);

	// used by: WinMain, Main_LoopUpdate, Lego_MainLoop
	result &= hook_write_jmpret(0x00478260, Gods98::Main_Finalise3D);

	// used by: Gods_Go
	result &= hook_write_jmpret(0x00478290, Gods98::Main_SetState);

	result &= hook_write_jmpret(0x004782c0, Gods98::Main_GetTime);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004782d0, Gods98::Main_DispatchMessage);
	// used by: WinMain, Main_LoopUpdate
	//result &= hook_write_jmpret(0x00478300, Gods98::Main_HandleIO);

	// used by: DirectDraw_Setup
	result &= hook_write_jmpret(0x00478370, Gods98::Main_SetupDisplay);
	result &= hook_write_jmpret(0x00478490, Gods98::Main_SetupDirect3D);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004785d0, Gods98::Main_AdjustWindowRect);

	// used by: Lego_LoadGraphicsSettings
	result &= hook_write_jmpret(0x004785f0, Gods98::Main_Setup3D);

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
	result &= hook_write_jmpret(0x00478b90, Gods98::Main_ChangeRenderState);

	// used by: Mesh_RenderCallback, Mesh_PostRenderAll
	result &= hook_write_jmpret(0x00478c00, Gods98::Main_RestoreStates);

	/// TODO: I'm going to wait and hook these later,
	///        I'd rather not risk some bug blowing out my ear drums.
	// used by: PausedMenu_SliderMusicVolume
	//result &= hook_write_jmpret(0x00478c40, Gods98::Main_SetCDVolume);
	// used by: PausedMenu_CalcSliderCDVolume
	//result &= hook_write_jmpret(0x00478c60, Gods98::Main_GetCDVolume);
	// used by: Main_SetCDVolume, Main_GetCDVolume
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

/*bool interop_hook_Gods98_Mesh(void)
{   bool result = true;

	return result;
}*/

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

bool interop_hook_Gods98Init_Init(void)
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


bool interop_hook_all(void)
{   bool result = true;


	result &= interop_hook_Gods98_3DSound();
	result &= interop_hook_Gods98_Animation();
	/// TODO: result &= interop_hook_Gods98_AnimClone();
	result &= interop_hook_Gods98_Bmp();
	//result &= interop_hook_Gods98_Compress(); // not ready yet, don't hook in for now, since RNC is a pain
	result &= interop_hook_Gods98_Config();
	/// TODO: result &= interop_hook_Gods98_Containers();
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
	/// TODO: result &= interop_hook_Gods98_Lws();
	/// TODO: result &= interop_hook_Gods98_Lwt();
	result &= interop_hook_Gods98_Main();
	result &= interop_hook_Gods98_Materials();
	result &= interop_hook_Gods98_Maths();
	result &= interop_hook_Gods98_Memory();
	/// TODO: result &= interop_hook_Gods98_Mesh();
	result &= interop_hook_Gods98_Movie();
	//result &= interop_hook_Gods98_Registry(); // no need to hook, used by: WinMain, File_Initialise
	result &= interop_hook_Gods98_Sound();
	result &= interop_hook_Gods98_TextWindow();
	result &= interop_hook_Gods98_Utils();
	result &= interop_hook_Gods98_Viewports();
	//result &= interop_hook_Gods98_Wad(); // no need to hook, used by: Files
	//result &= interop_hook_Gods98Init_Init(); // no need to hook, used by: WinMain

	return_interop(result);
}
