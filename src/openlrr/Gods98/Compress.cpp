
//#include "Errors.h"
#include "Compress.h"


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0049ca80>
uint32 __cdecl Gods98::RNC_Uncompress(IN void* bufferIn, OUT void** bufferOut)
{
	// There are a lot of potential issues implementing this, and little benefit.
	// 
	// No existing WAD files EVER use this method, and no tools are available to
	//  easily create WADs using this compression method.
	return ((uint32(__cdecl*)(void*, void**))0x0049ca80)(bufferIn, bufferOut);
}

#pragma endregion
