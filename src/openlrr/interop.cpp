
#include "hook.h"
#include "interop.h"
#include "openlrr.h"
#include "Gods98/Compress.h"
#include "Gods98/Config.h"
#include "Gods98/Errors.h"
#include "Gods98/Files.h"
#include "Gods98/Keys.h"
#include "Gods98/Memory.h"
//#include "Gods98/Movie.h"
#include "Gods98/Registry.h"
#include "Gods98/Utils.h"
#include "Gods98/Wad.h"


#define return_interop(result) { std::printf("%s %s\n", __FUNCTION__, (result?"OK":"failed")); } return result;


bool interop_hook_WinMain_call(void)
{   bool result = true;
	result &= hook_write_call(0x0048f3fa, OpenLRR_WinMain, nullptr);
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

bool interop_hook_Gods98_Keys(void)
{   bool result = true;
	result &= hook_write_jmpret(0x00485ce0, Gods98::Keys_Initialise);
	result &= hook_write_jmpret(0x004860f0, Gods98::Key_Find);
	return_interop(result);
}

bool interop_hook_Gods98_Memory(void)
{   bool result = true;
	result &= hook_write_jmpret(0x004896b0, Gods98::Mem_Initialise);

	// used by: File_LoadBinaryHandle
	//result &= hook_write_jmpret(0x004896d0, Gods98::Mem_AllocHandle);

	result &= hook_write_jmpret(0x00489720, Gods98::Mem_FreeHandle);
	result &= hook_write_jmpret(0x00489760, Gods98::Mem_AddressHandle);
	return_interop(result);
}

bool interop_hook_Gods98_Movie(void)
{   bool result = true;
	// Only the C wrapper API's need to be hooked

	// Cannot be hooked until DirectDraw() DirectDraw_bSurf() are finished
	/*result &= hook_write_jmpret(0x00472820, Gods98::Movie_Load);
	result &= hook_write_jmpret(0x00472930, Gods98::Movie_GetSize);

	/// NOTE: Name may be changed to Movie_GetLength
	result &= hook_write_jmpret(0x00472980, Gods98::Movie_GetDuration);

	result &= hook_write_jmpret(0x00472990, Gods98::Movie_Update);
	result &= hook_write_jmpret(0x004729b0, Gods98::Movie_Free);*/
	return_interop(result);
}

bool interop_hook_Gods98_Registry(void)
{   bool result = true;
	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048b5f0, Gods98::Registry_GetKeyFromPath);

	result &= hook_write_jmpret(0x0048b620, Gods98::Registry_GetValue);

	// internal, no need to hook these
	//result &= hook_write_jmpret(0x0048b650, Gods98::Registry_GetValue_Recursive);
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


bool interop_hook_all(void)
{   bool result = true;

	result &= interop_hook_WinMain_call();

	// not ready yet, don't hook in for now, since RNC is a pain
	//result &= interop_hook_Gods98_Compress();

	result &= interop_hook_Gods98_Config();
	result &= interop_hook_Gods98_Errors();
	result &= interop_hook_Gods98_Files();
	result &= interop_hook_Gods98_Keys();
	result &= interop_hook_Gods98_Memory();

	// Cannot be hooked until DirectDraw() DirectDraw_bSurf() are finished
	//result &= interop_hook_Gods98_Movie();

	// Can be removed once Main_WinMain is finished.
	result &= interop_hook_Gods98_Registry();
	result &= interop_hook_Gods98_Utils();

	// This isn't required if Files.c is fully hooked (but for now it's being kept anyway)
	result &= interop_hook_Gods98_Wad();

	return_interop(result);
}
