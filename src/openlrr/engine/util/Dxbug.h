// Dxbug.h : 
//

#pragma once

#include "../../platform/windows.h"

#include "../../common.h"

#include "../core/Errors.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

// Supported error modules/objects
enum class DxbugModule : sint32
{
	ERROR_UNKNOWN, ERROR_D3DRM, ERROR_DINPUT, ERROR_DDRAW, ERROR_DSOUND, ERROR_DPLAY, ERROR_D3DIM, ERROR_IUNKNOWN,
};
assert_sizeof(DxbugModule, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

// DirectX (dxbug.c) debugging loose static variables
struct Dxbug_Globs
{
	// [globs: NONE]
	/*000,4*/ HRESULT errnum;
	/*004,4*/ sint32 line;
	/*008,4*/ DxbugModule DXModuleNumber;
	/*00c,4*/ sint32 DXNumErrorsSet;
	/*010,4*/ const char* file;
	/*014,4*/ uint32 reserved1;
	/*018,800*/ char DXErrorString[2048]; // Last error that was set (actually [1024], but the space is unused)
	/*818*/
};
assert_sizeof(Dxbug_Globs, 0x818);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004ac788>
extern const char* (&DXModuleName)[8];// = { "Unknown module", "Direct3D Retained Mode", "Direct Input", "Direct Draw", "Direct Sound", "Direct Play", "Direct3D Immediate mode", "IUknown Interface" };

// <LegoRR.exe @005498a8>
extern Dxbug_Globs & dxbugGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

// Set a directX error value
#define SET_DXERROR(hres, module) Gods98::Error_SetDXError(hres, module, Error__FILE__, __LINE__)

// Set a DX erro valuer for a supported module 
// (ie. R.etained M.ode / D.irect D.raw / D.irect I.nput / I.U.nknown)
#define SET_RMERROR(hres) SET_DXERROR((hres), Gods98::DxbugModule::ERROR_D3DRM)
#define SET_DDERROR(hres) SET_DXERROR((hres), Gods98::DxbugModule::ERROR_DDRAW)
#define SET_DIERROR(hres) SET_DXERROR((hres), Gods98::DxbugModule::ERROR_DINPUT)
#define SET_IUERROR(hres) SET_DXERROR((hres), Gods98::DxbugModule::ERROR_IUNKNOWN)

// Check a Direct X return value
#define CHKRM(hres) {if ((hres) != D3DRM_OK)	{ SET_RMERROR(hres);}}
#define CHKDD(hres) {if ((hres) != DD_OK)		{ SET_DDERROR(hres);}}
#define CHKDI(hres) {if ((hres) != DI_OK)		{ SET_DIERROR(hres);}}
#define CHKIU(hres) {if ((hres) != S_OK)		{ SET_IUERROR(hres);}}

// Check the return value and return if failed
#define CHKRM_V(hres) {if ((hres) != D3DRM_OK)	{ SET_RMERROR(hres); return; }}
#define CHKDD_V(hres) {if ((hres) != DD_OK)		{ SET_DDERROR(hres); return; }}
#define CHKDI_V(hres) {if ((hres) != DI_OK)		{ SET_DIERROR(hres); return; }}
#define CHKIU_V(hres) {if ((hres) != S_OK)		{ SET_IUERROR(hres); return; }}

// as above but return an arbitrary value
#define CHKRM_RET(hres,retval) {if ((hres) != D3DRM_OK)	{ SET_RMERROR(hres); return (retval); }}
#define CHKDD_RET(hres,retval) {if ((hres) != DD_OK)	{ SET_DDERROR(hres); return (retval); }}
#define CHKDI_RET(hres,retval) {if ((hres) != DI_OK)	{ SET_DIERROR(hres); return (retval); }}
#define CHKIU_RET(hres,retval) {if ((hres) != S_OK)		{ SET_IUERROR(hres); return (retval); }}

// As above but return TRUE
#define CHKRM_T(hres) CHKRM_RET(hres,true);
#define CHKDD_T(hres) CHKDD_RET(hres,true);
#define CHKDI_T(hres) CHKDI_RET(hres,true);
#define CHKIU_T(hres) CHKIU_RET(hres,true);

// As above but return FALSE
#define CHKRM_F(hres) CHKRM_RET(hres,false);
#define CHKDD_F(hres) CHKDD_RET(hres,false);
#define CHKDI_F(hres) CHKDI_RET(hres,false);
#define CHKIU_F(hres) CHKIU_RET(hres,false);

// Return a boolean depending on result
#define CHKRM_B(hres) {if (hres == D3DRM_OK)	return true; else { SET_RMERROR(hres); return false; }}
#define CHKDD_B(hres) {if (hres == DD_OK)		return true; else { SET_DDERROR(hres); return false; }}
#define CHKDI_B(hres) {if (hres == DI_OK)		return true; else { SET_DIERROR(hres); return false; }}
#define CHKIU_B(hres) {if (hres == S_OK)		return true; else { SET_IUERROR(hres); return false; }}

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0048a050>
void __cdecl SE(const char* error, const char* errdesc);

// <LegoRR.exe @0048a090>
HRESULT __cdecl Error_SetDXError(HRESULT err, DxbugModule DXModule, const char* File, sint32 Line);


// <unused or inlined>
sint32 __cdecl Error_GetNumDXErrors(void);

// <unused or inlined>
const char* __cdecl Error_GetLastDXError(void);

// <unused or inlined>
void __cdecl Error_ShowLastDXError(void);

#pragma endregion

}
