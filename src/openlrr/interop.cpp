
#include "hook.h"
#include "interop.h"
#include "openlrr.h"
#include "Gods98/Animation.h"
#include "Gods98/Bmp.h"
#include "Gods98/Compress.h"
#include "Gods98/Config.h"
#include "Gods98/DirectDraw.h"
#include "Gods98/Draw.h"
#include "Gods98/Dxbug.h"
#include "Gods98/Errors.h"
#include "Gods98/Files.h"
#include "Gods98/Fonts.h"
#include "Gods98/Images.h"
#include "Gods98/Input.h"
#include "Gods98/Keys.h"
#include "Gods98/Main.h"
#include "Gods98/Memory.h"
#include "Gods98/Movie.h"
#include "Gods98/Registry.h"
#include "Gods98/TextWindow.h"
#include "Gods98/Utils.h"
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
	result &= hook_write_jmpret(0x0048b520, Gods98::Error_Initialise);
	result &= hook_write_jmpret(0x0048b540, Gods98::Error_FullScreen);
	result &= hook_write_jmpret(0x0048b550, Gods98::Error_CloseLog);
	result &= hook_write_jmpret(0x0048b5b0, Gods98::Error_Shutdown);
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
	result &= hook_write_jmpret(0x0047f920, Gods98::File_GetCDFilePath);

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
	result &= hook_write_jmpret(0x0047e120, Gods98::Image_LockSurface);
	result &= hook_write_jmpret(0x0047e190, Gods98::Image_UnlockSurface);
	result &= hook_write_jmpret(0x0047e1b0, Gods98::Image_GetPen255);
	result &= hook_write_jmpret(0x0047e210, Gods98::Image_GetPixelMask);

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
	result &= hook_write_jmpret(0x0047f050, Gods98::Input_InitKeysAndDI);

	// used by: WinMain, Main_LoopUpdate, NERPFunc__SetMessage
	result &= hook_write_jmpret(0x0047f1b0, Gods98::Input_ReadKeys);

	// used by: Credits_Play, Front_PlayMovie, Front_ShowIntroSplash, NERPFunc__GetAnyKeyPressed
	result &= hook_write_jmpret(0x0047f270, Gods98::Input_AnyKeyPressed);

	// used by: WinMain
	result &= hook_write_jmpret(0x0047f290, Gods98::Input_ReleaseKeysAndDI);
	// used by: WinMain, Main_LoopUpdate
	result &= hook_write_jmpret(0x0047f2d0, Gods98::Input_ReadMouse2);

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
	result &= hook_write_jmpret(0x00477eb0, Gods98::Main_ParseCommandLine);

	result &= hook_write_jmpret(0x004781f0, Gods98::Main_LoopUpdate);

	// used by: Level_BlockUpdateSurface
	result &= hook_write_jmpret(0x00478230, Gods98::Main_GetCLFlags);

	// used by: DirectDraw_EnumModeCallback, Init_SetModeList
	result &= hook_write_jmpret(0x00478240, Gods98::Main_GetWindowsBitDepth);

	// used by: WinMain, Main_LoopUpdate, Lego_MainLoop
	result &= hook_write_jmpret(0x00478260, Gods98::Main_Finalise3D);

	// used by: Gods_Go
	result &= hook_write_jmpret(0x00478290, Gods98::Main_SetState);

	result &= hook_write_jmpret(0x004782c0, Gods98::Main_GetTime);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x004782d0, Gods98::Main_DispatchMessage);
	// used by: WinMain, Main_LoopUpdate
	result &= hook_write_jmpret(0x00478300, Gods98::Main_HandleIO);

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

bool interop_hook_Gods98_Wad(void)
{   bool result = true;

	// used by: Files.c (shared function call with Files.c!File_Error
	//result &= hook_write_jmpret(0x0047f8c0, Gods98::Wad_Error);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048b760, Gods98::GetFileName);

	// used by: File_LoadWad
	//result &= hook_write_jmpret(0x0048b7a0, Gods98::Wad_Load);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048bfa0, Gods98::Wad_Get);
	//result &= hook_write_jmpret(0x0048bfb0, Gods98::Wad_FileLength);
	//result &= hook_write_jmpret(0x0048bfd0, Gods98::Wad_FileCompressedLength);
	//result &= hook_write_jmpret(0x0048bff0, Gods98::Wad_FindFreeFileHandle);

	// used by: File_Exists, _File_CheckSystem
	//result &= hook_write_jmpret(0x0048c010, Gods98::Wad_IsFileInWad);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048c060, Gods98::_Wad_IsFileInWad);
	//result &= hook_write_jmpret(0x0048c0c0, Gods98::Wad_GetFreeWadSlot);
	//result &= hook_write_jmpret(0x0048c100, Gods98::_Wad_FileOpen);

	// used by: _File_OpenWad
	//result &= hook_write_jmpret(0x0048c230, Gods98::Wad_FileOpen);

	// used by: _File_Dealloc
	//result &= hook_write_jmpret(0x0048c280, Gods98::Wad_FileClose);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048c2b0, Gods98::Wad_FileGetPointer);

	// used by: File_Seek, File_Read, File_GetC
	//result &= hook_write_jmpret(0x0048c2d0, Gods98::Wad_hLength);

	// used by: File_Read, File_GetC
	//result &= hook_write_jmpret(0x0048c2f0, Gods98::Wad_hData);

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

	//result &= interop_hook_WinMain_call();

	result &= interop_hook_Gods98_Animation();
	result &= interop_hook_Gods98_Bmp();

	// not ready yet, don't hook in for now, since RNC is a pain
	//result &= interop_hook_Gods98_Compress();

	result &= interop_hook_Gods98_Config();
	result &= interop_hook_Gods98_DirectDraw();
	result &= interop_hook_Gods98_Draw();
	result &= interop_hook_Gods98_Dxbug();
	result &= interop_hook_Gods98_Errors();
	result &= interop_hook_Gods98_Files();
	result &= interop_hook_Gods98_Fonts();
	result &= interop_hook_Gods98_Images();
	result &= interop_hook_Gods98_Input();
	result &= interop_hook_Gods98_Keys();
	result &= interop_hook_Gods98_Main();
	result &= interop_hook_Gods98_Memory();
	result &= interop_hook_Gods98_Movie();

	// Can be removed once Main_WinMain is finished.
	result &= interop_hook_Gods98_Registry();
	result &= interop_hook_Gods98_TextWindow();
	result &= interop_hook_Gods98_Utils();

	// This isn't required if Files.c is fully hooked (but for now it's being kept anyway)
	result &= interop_hook_Gods98_Wad();

	result &= interop_hook_Gods98Init_Init();

	return_interop(result);
}
