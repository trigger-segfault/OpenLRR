
#include "Errors.h"
#include "Utils.h"
#include "Config.h"
#include "Activities.h"


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// Possibly inlined inside Container_Load
// <unknown>
uint32 __cdecl Gods98::Activity_Load(const char* filename, const char* gameID, IN char* arglist, ActivityReadCallback ReadCallBack, void* data)
{
	char* argv[ACTIVITY_MAXARGS];
	char* result;
	char searchString[UTIL_DEFSTRINGLEN];
	uint32 calls=0;
	Config* conf;
	
	std::sprintf(searchString, "%s\\%s.ae", filename, filename);
	if (conf = Config_Load(searchString)){
		
		uint32 count;
		if (count = Util_Tokenise(arglist, argv, ACTIVITY_SEPARATOR)){
			for (uint32 loop=0 ; loop<count ; loop++){
				
				std::sprintf(searchString, "%s%s%s", gameID, CONFIG_SEPARATOR, argv[loop]);
				
				if (result = Config_GetStringValue(conf, searchString)){
					ReadCallBack(argv[loop], result, std::atoi(result), (real32)std::atof(result), data);
					calls++;
				}

			}
		}
	} else Error_Fatal(true, "Could not get Config");

	return calls;
}

#pragma endregion
