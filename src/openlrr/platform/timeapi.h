#pragma once

#include "windows.h"
#include <timeapi.h>					// timeGetTime()


namespace legacy
{; // !<---

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0076f594>
__inline DWORD timeGetTime(void)
{
	// no thunk function, has to be called as a pointer pointer
	return (*((DWORD(__stdcall**)(void))0x0076f594))();
}

#pragma endregion

}
