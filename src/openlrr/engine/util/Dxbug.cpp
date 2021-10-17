// Dxbug.cpp : 
//

#include "../../platform/d3drm.h"
#include "../../platform/ddraw.h"
#include "../../platform/dinput.h"

#include "../core/Errors.h"

#include "Dxbug.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004ac788>
const char* (& Gods98::DXModuleName)[8] = *(const char* (*)[8])0x004ac788;
// = { "Unknown module", "Direct3D Retained Mode", "Direct Input", "Direct Draw", "Direct Sound", "Direct Play", "Direct3D Immediate mode", "IUknown Interface" };

// <LegoRR.exe @005498a8>
Gods98::Dxbug_Globs & Gods98::dxbugGlobs = *(Gods98::Dxbug_Globs*)0x005498a8; // (no init)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <unused or inlined>
sint32 __cdecl Gods98::Error_GetNumDXErrors(void)
{
	return dxbugGlobs.DXNumErrorsSet;
}

// <unused or inlined>
const char* __cdecl Gods98::Error_GetLastDXError(void)
{
	return dxbugGlobs.DXErrorString;
}

// <unused or inlined>
void __cdecl Gods98::Error_ShowLastDXError(void)
{
	if (Error_GetNumDXErrors())
	{
		char title[30];
		std::sprintf(title, "ERROR! %i occured", Error_GetNumDXErrors());
		// Print a message box if there was an error recorded
		::MessageBoxA(nullptr, Error_GetLastDXError(), title, MB_OK);
	}
}


// <LegoRR.exe @0048a050>
void __cdecl Gods98::SE(const char* error, const char* errdesc)
{
	log_firstcall();

	std::sprintf(dxbugGlobs.DXErrorString,
		"A DirectX error was set in module %s at line no %i.\n"
		"Error : %s (code %i)\n"
		"DX Module : %s\n"
		"Error Description : %s\n",
		dxbugGlobs.file, dxbugGlobs.line, error, (sint32)dxbugGlobs.errnum, DXModuleName[(uint32)dxbugGlobs.DXModuleNumber], errdesc);
	Error_Warn(true, dxbugGlobs.DXErrorString);
}


#pragma warning(push)
#pragma warning(disable : 4065)  // suppress "switch statement contains 'default' but no 'case' labels"

// <LegoRR.exe @0048a090>
HRESULT __cdecl Gods98::Error_SetDXError(HRESULT err, DxbugModule DXModule, const char* File, sint32 Line)
{
	log_firstcall();

	// Take the last error code specified by the program
	dxbugGlobs.errnum = err;
	if (DXModule > DxbugModule::ERROR_UNKNOWN && DXModule <= DxbugModule::ERROR_IUNKNOWN) dxbugGlobs.DXModuleNumber = DXModule;
	else dxbugGlobs.DXModuleNumber = DxbugModule::ERROR_UNKNOWN;
	dxbugGlobs.file = File;
	dxbugGlobs.line = Line;
	dxbugGlobs.DXNumErrorsSet++;
	switch (DXModule)
	{
	case DxbugModule::ERROR_DSOUND: 
		{
			switch (err)
			{
				default: SE("Unknown", "Error handling is unsupported in this module"); break;
			}
		}
	case DxbugModule::ERROR_DPLAY:
		{
			switch (err)
			{
				default: SE("Unknown", "Error handling is unsupported in this module"); break;
			}
		}
	case DxbugModule::ERROR_D3DIM:
		{
			switch (err)
			{
				default: SE("Unknown", "Error handling is unsupported in this module"); break;
			}
		}
	case DxbugModule::ERROR_D3DRM:
		{
			switch (err)
			{
				case D3DRM_OK: SE("D3DRM_OK","No error."); break;
				case D3DRMERR_BADALLOC: SE("D3DRMERR_BADALLOC","Out of memory."); break;
				case D3DRMERR_BADDEVICE: SE("D3DRMERR_BADDEVICE","Device is not compatible with renderer."); break;
				case D3DRMERR_BADFILE: SE("D3DRMERR_BADFILE","Data file is corrupt."); break;
				case D3DRMERR_BADMAJORVERSION: SE("D3DRMERR_BADMAJORVERSION","Bad DLL major version."); break;
				case D3DRMERR_BADMINORVERSION: SE("D3DRMERR_BADMINORVERSION","Bad DLL minor version."); break;
				case D3DRMERR_BADOBJECT: SE("D3DRMERR_BADOBJECT","Object expected in argument."); break;
				case D3DRMERR_BADPMDATA: SE("D3DRMERR_BADPMDATA","The data in the X File is corrupted.\nThe conversion to a progressive mesh succeeded but\nproduced an invalid progressive mesh in\nthe X File."); break;
				case D3DRMERR_BADTYPE: SE("D3DRMERR_BADTYPE","Bad argument type passed."); break;
				case D3DRMERR_BADVALUE: SE("D3DRMERR_BADVALUE","Bad argument value passed."); break;
				case D3DRMERR_BOXNOTSET: SE("D3DRMERR_BOXNOTSET","An attempt was made to access a bounding box\n(for example, with IDirect3DRMFrame2::GetBox)\nwhen no bounding box was set on the frame."); break;
				case D3DRMERR_CONNECTIONLOST: SE("D3DRMERR_CONNECTIONLOST","Data connection was lost during a load,\nclone, or duplicate."); break;
				case D3DRMERR_FACEUSED: SE("D3DRMERR_FACEUSED","Face already used in a mesh."); break;
				case D3DRMERR_FILENOTFOUND: SE("D3DRMERR_FILENOTFOUND","File cannot be opened."); break;
//				case D3DRMERR_INVALIDDATA: SE("D3DRMERR_INVALIDDATA","The method received or accessed data\nthat is invalid."); break;
//				case D3DRMERR_INVALIDOBJECT: SE("D3DRMERR_INVALIDOBJECT","The method received a pointer to an\nobject that is invalid."); break;
//				case D3DRMERR_INVALIDPARAMS: SE("D3DRMERR_INVALIDPARAMS","One of the parameters passed to the\nmethod is invalid."); break;
				case D3DRMERR_NOTDONEYET: SE("D3DRMERR_NOTDONEYET","Unimplemented."); break;
				case D3DRMERR_NOTENOUGHDATA: SE("D3DRMERR_NOTENOUGHDATA","Not enough data has been loaded to\nperform the requested operation."); break;
				case D3DRMERR_NOTFOUND: SE("D3DRMERR_NOTFOUND","Object not found in specified place."); break;
				case D3DRMERR_PENDING: SE("D3DRMERR_PENDING","The data required to supply the requested\ninformation has not finished loading."); break;
				case D3DRMERR_REQUESTTOOLARGE: SE("D3DRMERR_REQUESTTOOLARGE","An attempt was made to set a level\nof detail in a progressive mesh greater than the maximum available."); break;
				case D3DRMERR_REQUESTTOOSMALL: SE("D3DRMERR_REQUESTTOOSMALL","An attempt was made to set the minimum\nrendering detail of a progressive mesh smaller than\nthe detail in the base mesh\n(the minimum for rendering)."); break;
				case D3DRMERR_UNABLETOEXECUTE: SE("D3DRMERR_UNABLETOEXECUTE","Unable to carry out procedure."); break;
				default: SE("UNKNOWN ERROR", "Invalid error code specified"); break;
			}
			break;
		}
	case DxbugModule::ERROR_DINPUT:
		{
			switch (err)
			{
				case DI_BUFFEROVERFLOW: SE("DI_BUFFEROVERFLOW","The device buffer overflowed and some input was lost.\nThis value is equal to the S_FALSE\nstandard COM return value."); break;
				case DI_DOWNLOADSKIPPED: SE("DI_DOWNLOADSKIPPED","The parameters of the effect were successfully updated,\nbut the effect could not be downloaded because the associated\ndevice was not acquired in exclusive mode."); break;
				case DI_EFFECTRESTARTED: SE("DI_EFFECTRESTARTED","The effect was stopped,\nthe parameters were updated,\nand the effect was restarted."); break;
//				case DI_NOEFFECT: SE("DI_NOEFFECT","The operation had no effect.\nThis value is equal to the S_FALSE standard\nCOM return value."); break;
//				case DI_NOTATTACHED: SE("DI_NOTATTACHED","The device exists but is not currently attached. This value is equal to the S_FALSE standard COM return value."); break;
				case DI_OK: SE("DI_OK","The operation completed successfully.\nThis value is equal to the S_OK standard\nCOM return value."); break;
				case DI_POLLEDDEVICE: SE("DI_POLLEDDEVICE","The device is a polled device.\nAs a result, device buffering will not collect\nany data and event notifications will not be signaled\nuntil the IDirectInputDevice2::Poll method is called."); break;
//				case DI_PROPNOEFFECT: SE("DI_PROPNOEFFECT","The change in device properties had no effect.\nThis value is equal to the S_FALSE standard\nCOM return value."); break;
				case DI_TRUNCATED: SE("DI_TRUNCATED","The parameters of the effect were successfully updated,\nbut some of them were beyond the capabilities of the\ndevice and were truncated to the nearest supported value."); break;
				case DI_TRUNCATEDANDRESTARTED: SE("DI_TRUNCATEDANDRESTARTED","Equal to DI_EFFECTRESTARTED | DI_TRUNCATED"); break;
				case DIERR_ACQUIRED: SE("DIERR_ACQUIRED","The operation cannot be performed while\nthe device is acquired."); break;
				case DIERR_ALREADYINITIALIZED: SE("DIERR_ALREADYINITIALIZED","This object is already initialized"); break;
				case DIERR_BADDRIVERVER: SE("DIERR_BADDRIVERVER","The object could not be created due to an\nincompatible driver version or mismatched or\nincomplete driver components."); break;
				case DIERR_BETADIRECTINPUTVERSION: SE("DIERR_BETADIRECTINPUTVERSION","The application was written for an unsupported\nprerelease version of DirectInput."); break;
				case DIERR_DEVICEFULL: SE("DIERR_DEVICEFULL","The device is full."); break;
				case DIERR_DEVICENOTREG: SE("DIERR_DEVICENOTREG","The device or device instance is not registered with\nDirectInput. This value is equal to the\nREGDB_E_CLASSNOTREG standard COM return value."); break;
				case DIERR_EFFECTPLAYING: SE("DIERR_EFFECTPLAYING","The parameters were updated in memory but were not\ndownloaded to the device because the device does not\nsupport updating an effect while it is still playing."); break;
				case DIERR_HASEFFECTS: SE("DIERR_HASEFFECTS","The device cannot be reinitialized because there\nare still effects attached to it"); break;
				case DIERR_GENERIC: SE("DIERR_GENERIC","An undetermined error occurred inside the DirectInput subsystem.\nThis value is equal to the E_FAIL standard\nCOM return value."); break;
				case DIERR_HANDLEEXISTS: SE("DIERR_HANDLEEXISTS","The device already has an event notification associated\nwith it. This value is equal to the E_ACCESSDENIED\nstandard COM return value."); break;
				case DIERR_INCOMPLETEEFFECT: SE("DIERR_INCOMPLETEEFFECT","The effect could not be downloaded because essential\ninformation is missing. For example, no axes have\nbeen associated with the effect, or no type-specific\ninformation has been supplied"); break;
				case DIERR_INPUTLOST: SE("DIERR_INPUTLOST","Access to the input device has been lost. It must be reacquired."); break;
				case DIERR_INVALIDPARAM: SE("DIERR_INVALIDPARAM","An invalid parameter was passed to the returning function, or the object was not in a state that permitted the function to be called. This value is equal to the E_INVALIDARG standard COM return value."); break;
				case DIERR_MOREDATA: SE("DIERR_MOREDATA","Not all the requested information fitted into the buffer"); break;
				case DIERR_NOAGGREGATION: SE("DIERR_NOAGGREGATION","This object does not support aggregation."); break;
				case DIERR_NOINTERFACE: SE("DIERR_NOINTERFACE","The specified interface is not supported by\nthe object. This value is equal to the E_NOINTERFACE\nstandard COM return value."); break;
				case DIERR_NOTACQUIRED: SE("DIERR_NOTACQUIRED","The operation cannot be performed unless\nthe device is acquired."); break;
				case DIERR_NOTBUFFERED: SE("DIERR_NOTBUFFERED","The device is not buffered.\nSet the DIPROP_BUFFERSIZE property to enable buffering"); break;
				case DIERR_NOTDOWNLOADED: SE("DIERR_NOTDOWNLOADED","The effect is not downloaded"); break;
				case DIERR_NOTEXCLUSIVEACQUIRED: SE("DIERR_NOTEXCLUSIVEACQUIRED","The operation cannot be performed unless the device\nis acquired in DISCL_EXCLUSIVE mode."); break;
				case DIERR_NOTFOUND: SE("DIERR_NOTFOUND","The requested object does not exist."); break;
				case DIERR_NOTINITIALIZED: SE("DIERR_NOTINITIALIZED","This object has not been initialized."); break;
//				case DIERR_OBJECTNOTFOUND: SE("DIERR_OBJECTNOTFOUND","The requested object does not exist."); break;
				case DIERR_OLDDIRECTINPUTVERSION: SE("DIERR_OLDDIRECTINPUTVERSION","The application requires a newer version of DirectInput."); break;
//				case DIERR_OTHERAPPHASPRIO: SE("DIERR_OTHERAPPHASPRIO","Another application has a higher priority level,\npreventing this call from succeeding.\nThis value is equal to the E_ACCESSDENIED standard COM return value.\nThis error can be returned when an application has\nonly foreground access to a device but is\nattempting to acquire the device while in the background"); break;
				case DIERR_OUTOFMEMORY: SE("DIERR_OUTOFMEMORY","The DirectInput subsystem couldn't allocate\nsufficient memory to complete the call.\nThis value is equal to the E_OUTOFMEMORY\nstandard COM return value."); break;
//				case DIERR_READONLY: SE("DIERR_READONLY","The specified property cannot be changed.\nThis value is equal to the E_ACCESSDENIED\nstandard COM return value."); break;
				case DIERR_UNSUPPORTED: SE("DIERR_UNSUPPORTED","The function called is not supported at this time.\nThis value is equal to the E_NOTIMPL\nstandard COM return value."); break;
				case E_PENDING: SE("E_PENDING","Data is not yet availabe."); break;
				default: SE("UNKNOWN ERROR", "Invalid error code specified"); break;
			};
			break;
		}
	case DxbugModule::ERROR_DDRAW:
		{
			switch (err)
			{
				case DD_OK : SE("DD_OK","The request completed successfully."); break;
				case DDERR_ALREADYINITIALIZED : SE("DDERR_ALREADYINITIALIZED","The object has already been initialized."); break;
				case DDERR_BLTFASTCANTCLIP : SE("DDERR_BLTFASTCANTCLIP","A DirectDrawClipper object is attached to a source\nsurface that has passed into a call to the\nIDirectDrawSurface3::BltFast method."); break;
				case DDERR_CANNOTATTACHSURFACE : SE("DDERR_CANNOTATTACHSURFACE","A surface cannot be attached to\nanother requested surface."); break;
				case DDERR_CANNOTDETACHSURFACE : SE("DDERR_CANNOTDETACHSURFACE","A surface cannot be detached from another\nrequested surface."); break;
				case DDERR_CANTCREATEDC : SE("DDERR_CANTCREATEDC","Windows cannot create any more device\ncontexts (DCs)."); break;
				case DDERR_CANTDUPLICATE : SE("DDERR_CANTDUPLICATE","Primary and 3-D surfaces, or surfaces that\nare implicitly created, cannot be duplicated."); break;
				case DDERR_CANTLOCKSURFACE : SE("DDERR_CANTLOCKSURFACE","Access to this surface is refused because an\nattempt was made to lock the primary\nsurface without DCI support."); break;
				case DDERR_CANTPAGELOCK : SE("DDERR_CANTPAGELOCK","An attempt to page lock a surface failed.\nPage lock will not work on a display-memory surface or\nan emulated primary surface."); break;
				case DDERR_CANTPAGEUNLOCK : SE("DDERR_CANTPAGEUNLOCK","An attempt to page unlock a surface failed.\nPage unlock will not work on a display-memory\nsurface or an emulated primary surface."); break;
				case DDERR_CLIPPERISUSINGHWND : SE("DDERR_CLIPPERISUSINGHWND","An attempt was made to set a clip list for a\nDirectDrawClipper object that is already monitoring\na window handle."); break;
				case DDERR_COLORKEYNOTSET : SE("DDERR_COLORKEYNOTSET","No source color key is specified for this operation."); break;
				case DDERR_CURRENTLYNOTAVAIL : SE("DDERR_CURRENTLYNOTAVAIL","No support is currently available."); break;
				case DDERR_DCALREADYCREATED : SE("DDERR_DCALREADYCREATED","A device context (DC) has already been returned\nfor this surface. Only one DC can be retrieved for each surface."); break;
				case DDERR_DEVICEDOESNTOWNSURFACE: SE("DDERR_DEVICEDOESNTOWNSURFAC","Surfaces created by one direct draw device cannot\nbe used directly by another direct draw device"); break;
				case DDERR_DIRECTDRAWALREADYCREATED : SE("DDERR_DIRECTDRAWALREADYCREATED","A DirectDraw object representing this driver\nhas already been created for this process."); break;
				case DDERR_EXCEPTION : SE("DDERR_EXCEPTION","An exception was encountered while performing the requested operation."); break;
				case DDERR_EXCLUSIVEMODEALREADYSET : SE("DDERR_EXCLUSIVEMODEALREADYSET","An attempt was made to set the cooperative level\nwhen it was already set to exclusive."); break;
				case DDERR_GENERIC : SE("DDERR_GENERIC","There is an undefined error condition."); break;
				case DDERR_HEIGHTALIGN : SE("DDERR_HEIGHTALIGN","The height of the provided rectangle is not\na multiple of the required alignment."); break;
				case DDERR_HWNDALREADYSET : SE("DDERR_HWNDALREADYSET","The DirectDraw cooperative level window handle\nhas already been set. It cannot be reset while the process\nhas surfaces or palettes created."); break;
				case DDERR_HWNDSUBCLASSED : SE("DDERR_HWNDSUBCLASSED","DirectDraw is prevented from restoring state\nbecause the DirectDraw cooperative level window\nhandle has been subclassed."); break;
				case DDERR_IMPLICITLYCREATED : SE("DDERR_IMPLICITLYCREATED","The surface cannot be restored because\nit is an implicitly created surface."); break;
				case DDERR_INCOMPATIBLEPRIMARY : SE("DDERR_INCOMPATIBLEPRIMARY","The primary surface creation request does\nnot match with the existing primary surface."); break;
				case DDERR_INVALIDCAPS : SE("DDERR_INVALIDCAPS","One or more of the capability bits passed to\nthe callback function are incorrect."); break;
				case DDERR_INVALIDCLIPLIST : SE("DDERR_INVALIDCLIPLIST","DirectDraw does not support the provided clip list."); break;
				case DDERR_INVALIDDIRECTDRAWGUID : SE("DDERR_INVALIDDIRECTDRAWGUID","The globally unique identifier (GUID) passed to the\nDirectDrawCreate function is not a valid DirectDraw\ndriver identifier."); break;
				case DDERR_INVALIDMODE : SE("DDERR_INVALIDMODE","DirectDraw does not support the requested mode."); break;
				case DDERR_INVALIDOBJECT : SE("DDERR_INVALIDOBJECT","DirectDraw received a pointer that was an\ninvalid DirectDraw object."); break;
				case DDERR_INVALIDPARAMS : SE("DDERR_INVALIDPARAMS","One or more of the parameters passed to the\nmethod are incorrect."); break;
				case DDERR_INVALIDPIXELFORMAT : SE("DDERR_INVALIDPIXELFORMAT","The pixel format was invalid as specified."); break;
				case DDERR_INVALIDPOSITION : SE("DDERR_INVALIDPOSITION","The position of the overlay on the destination is no longer legal."); break;
				case DDERR_INVALIDRECT : SE("DDERR_INVALIDRECT","The provided rectangle was invalid."); break;
				case DDERR_INVALIDSURFACETYPE : SE("DDERR_INVALIDSURFACETYPE","The requested operation could not be performed\nbecause the surface was of the wrong type."); break;
				case DDERR_LOCKEDSURFACES : SE("DDERR_LOCKEDSURFACES","One or more surfaces are locked, causing the failure of\nthe requested operation."); break;
				case DDERR_MOREDATA: SE("DDERR_MOREDAT","There is more data available than the\nspecified buffer size can hold"); break;
				case DDERR_NO3D : SE("DDERR_NO3D","No 3-D hardware or emulation is present."); break;
				case DDERR_NOALPHAHW : SE("DDERR_NOALPHAHW","No alpha acceleration hardware is present or\navailable, causing the failure of\nthe requested operation."); break;
				case DDERR_NOBLTHW : SE("DDERR_NOBLTHW","No blitter hardware is present."); break;
				case DDERR_NOCLIPLIST : SE("DDERR_NOCLIPLIST","No clip list is available."); break;
				case DDERR_NOCLIPPERATTACHED : SE("DDERR_NOCLIPPERATTACHED","No DirectDrawClipper object is attached\nto the surface object."); break;
				case DDERR_NOCOLORCONVHW : SE("DDERR_NOCOLORCONVHW","The operation cannot be carried out because no\ncolor-conversion hardware is present or available."); break;
				case DDERR_NOCOLORKEY : SE("DDERR_NOCOLORKEY","The surface does not currently have a color key."); break;
				case DDERR_NOCOLORKEYHW : SE("DDERR_NOCOLORKEYHW","The operation cannot be carried out because there is\nno hardware support for the destination color key."); break;
				case DDERR_NOCOOPERATIVELEVELSET : SE("DDERR_NOCOOPERATIVELEVELSET","A create function is called without the\nIDirectDraw2::SetCooperativeLevel method being called."); break;
				case DDERR_NODC : SE("DDERR_NODC","No DC has ever been created for this surface."); break;
				case DDERR_NODDROPSHW : SE("DDERR_NODDROPSHW","No DirectDraw raster operation (ROP) hardware is available."); break;
				case DDERR_NODIRECTDRAWHW : SE("DDERR_NODIRECTDRAWHW","Hardware-only DirectDraw object creation is not possible;\nthe driver does not support any hardware."); break;
				case DDERR_NODIRECTDRAWSUPPORT : SE("DDERR_NODIRECTDRAWSUPPORT","DirectDraw support is not possible\nwith the current display driver."); break;
				case DDERR_NOEMULATION : SE("DDERR_NOEMULATION","Software emulation is not available."); break;
				case DDERR_NOEXCLUSIVEMODE : SE("DDERR_NOEXCLUSIVEMODE","The operation requires the application to have exclusive mode,\nbut the application does not have exclusive mode."); break;
				case DDERR_NOFLIPHW : SE("DDERR_NOFLIPHW","Flipping visible surfaces is not supported."); break;
				case DDERR_NOGDI : SE("DDERR_NOGDI","No GDI is present."); break;
				case DDERR_NOHWND : SE("DDERR_NOHWND","Clipper notification requires a window handle,\nor no window handle has been previously set as\nthe cooperative level window handle."); break;
				case DDERR_NOMIPMAPHW : SE("DDERR_NOMIPMAPHW","The operation cannot be carried out because no mipmap\ntexture mapping hardware is present or available."); break;
				case DDERR_NOMIRRORHW : SE("DDERR_NOMIRRORHW","The operation cannot be carried out because no mirroring\nhardware is present or available."); break;
				case DDERR_NONONLOCALVIDMEM: SE("DDERR_NONONLOCALVIDME","An attempt was made to allocate non-local video\nmemory from a device that does not support non-local\nvideo memory"); break;
				case DDERR_NOOPTIMIZEHW: SE("DDERR_NOOPTIMIZEH","The device does not support optimized surfaces"); break;
				case DDERR_NOOVERLAYDEST : SE("DDERR_NOOVERLAYDEST","The IDirectDrawSurface3::GetOverlayPosition method is called\non an overlay that the IDirectDrawSurface3::UpdateOverlay method\nhas not been called on to establish a destination."); break;
				case DDERR_NOOVERLAYHW : SE("DDERR_NOOVERLAYHW","The operation cannot be carried out because no\noverlay hardware is present or available."); break;
				case DDERR_NOPALETTEATTACHED : SE("DDERR_NOPALETTEATTACHED","No palette object is attached to this surface."); break;
				case DDERR_NOPALETTEHW : SE("DDERR_NOPALETTEHW","There is no hardware support for 16- or 256-color palettes."); break;
				case DDERR_NORASTEROPHW : SE("DDERR_NORASTEROPHW","The operation cannot be carried out because no appropriate\nraster operation hardware is present or available."); break;
				case DDERR_NOROTATIONHW : SE("DDERR_NOROTATIONHW","The operation cannot be carried out because\nno rotation hardware is present or available."); break;
				case DDERR_NOSTRETCHHW : SE("DDERR_NOSTRETCHHW","The operation cannot be carried out because\nthere is no hardware support for stretching."); break;
				case DDERR_NOT4BITCOLOR : SE("DDERR_NOT4BITCOLOR","The DirectDrawSurface object is not using a 4-bit color palette\nand the requested operation requires a 4-bit color palette."); break;
				case DDERR_NOT4BITCOLORINDEX : SE("DDERR_NOT4BITCOLORINDEX","The DirectDrawSurface object is not using a 4-bit color index\npalette and the requested operation requires a\n4-bit color index palette."); break;
				case DDERR_NOT8BITCOLOR : SE("DDERR_NOT8BITCOLOR","The DirectDrawSurface object is not using an 8-bit color\npalette and the requested operation requires\nan 8-bit color palette."); break;
				case DDERR_NOTAOVERLAYSURFACE : SE("DDERR_NOTAOVERLAYSURFACE","An overlay component is called for a non-overlay surface."); break;
				case DDERR_NOTEXTUREHW : SE("DDERR_NOTEXTUREHW","The operation cannot be carried out because no\ntexture-mapping hardware is present or available."); break;
				case DDERR_NOTFLIPPABLE : SE("DDERR_NOTFLIPPABLE","An attempt has been made to flip\na surface that cannot be flipped."); break;
				case DDERR_NOTFOUND : SE("DDERR_NOTFOUND","The requested item was not found."); break;
				case DDERR_NOTINITIALIZED : SE("DDERR_NOTINITIALIZED","An attempt was made to call an interface method\nof a DirectDraw object created by CoCreateInstance before\nthe object was initialized."); break;
				case DDERR_NOTLOADED: SE("DDERR_NOTLOADE","The surface is an optimized surface,\nbut it has not yet been allocated any memory"); break;
				case DDERR_NOTLOCKED : SE("DDERR_NOTLOCKED","An attempt is made to unlock a\nsurface that was not locked."); break;
				case DDERR_NOTPAGELOCKED : SE("DDERR_NOTPAGELOCKED","An attempt is made to page unlock a\nsurface with no outstanding page locks."); break;
				case DDERR_NOTPALETTIZED : SE("DDERR_NOTPALETTIZED","The surface being used is not a\npalette-based surface."); break;
				case DDERR_NOVSYNCHW : SE("DDERR_NOVSYNCHW","The operation cannot be carried out because\nthere is no hardware support for vertical\nblank synchronized operations."); break;
				case DDERR_NOZBUFFERHW : SE("DDERR_NOZBUFFERHW","The operation to create a z-buffer in display memory\nor to perform a blit using a z-buffer cannot be\ncarried out because there is no hardware support\nfor z-buffers."); break;
				case DDERR_NOZOVERLAYHW : SE("DDERR_NOZOVERLAYHW","The overlay surfaces cannot be z-layered based on the\nz-order because the hardware does not support\nz-ordering of overlays."); break;
				case DDERR_OUTOFCAPS : SE("DDERR_OUTOFCAPS","The hardware needed for the requested\noperation has already been allocated."); break;
				case DDERR_OUTOFMEMORY : SE("DDERR_OUTOFMEMORY","DirectDraw does not have enough\nmemory to perform the operation."); break;
				case DDERR_OUTOFVIDEOMEMORY : SE("DDERR_OUTOFVIDEOMEMORY","DirectDraw does not have enough display\nmemory to perform the operation."); break;
				case DDERR_OVERLAYCANTCLIP : SE("DDERR_OVERLAYCANTCLIP","The hardware does not support clipped overlays."); break;
				case DDERR_OVERLAYCOLORKEYONLYONEACTIVE : SE("DDERR_OVERLAYCOLORKEYONLYONEACTIVE","An attempt was made to have more than one\ncolor key active on an overlay."); break;
				case DDERR_OVERLAYNOTVISIBLE : SE("DDERR_OVERLAYNOTVISIBLE","The IDirectDrawSurface3::GetOverlayPosition method\nis called on a hidden overlay."); break;
				case DDERR_PALETTEBUSY : SE("DDERR_PALETTEBUSY","Access to this palette is refused because\nthe palette is locked by another thread."); break;
				case DDERR_PRIMARYSURFACEALREADYEXISTS : SE("DDERR_PRIMARYSURFACEALREADYEXISTS","This process has already created a primary surface."); break;
				case DDERR_REGIONTOOSMALL : SE("DDERR_REGIONTOOSMALL","The region passed to the\nIDirectDrawClipper::GetClipList\nmethod is too small."); break;
				case DDERR_SURFACEALREADYATTACHED : SE("DDERR_SURFACEALREADYATTACHED","An attempt was made to attach a surface to another\nsurface to which it is already attached."); break;
				case DDERR_SURFACEALREADYDEPENDENT : SE("DDERR_SURFACEALREADYDEPENDENT","An attempt was made to make a surface a dependency\nof another surface to which it is\nalready dependent."); break;
				case DDERR_SURFACEBUSY : SE("DDERR_SURFACEBUSY","Access to the surface is refused because\nthe surface is locked by another thread."); break;
				case DDERR_SURFACEISOBSCURED : SE("DDERR_SURFACEISOBSCURED","Access to the surface is refused\nbecause the surface is obscured."); break;
				case DDERR_SURFACELOST : SE("DDERR_SURFACELOST","Access to the surface is refused because the surface\nmemory is gone. The DirectDrawSurface object representing this\nsurface should have the IDirectDrawSurface3::Restore\nmethod called on it."); break;
				case DDERR_SURFACENOTATTACHED : SE("DDERR_SURFACENOTATTACHED","The requested surface is not attached."); break;
				case DDERR_TOOBIGHEIGHT : SE("DDERR_TOOBIGHEIGHT","The height requested by DirectDraw is too large."); break;
				case DDERR_TOOBIGSIZE : SE("DDERR_TOOBIGSIZE","The size requested by DirectDraw is too large.\nHowever, the individual height and\nwidth are OK."); break;
				case DDERR_TOOBIGWIDTH : SE("DDERR_TOOBIGWIDTH","The width requested by DirectDraw\nis too large."); break;
				case DDERR_UNSUPPORTED : SE("DDERR_UNSUPPORTED","The operation is not supported."); break;
				case DDERR_UNSUPPORTEDFORMAT : SE("DDERR_UNSUPPORTEDFORMAT","The FourCC format requested is\nnot supported by DirectDraw."); break;
				case DDERR_UNSUPPORTEDMASK : SE("DDERR_UNSUPPORTEDMASK","The bitmask in the pixel format requested\nis not supported by DirectDraw."); break;
				case DDERR_UNSUPPORTEDMODE : SE("DDERR_UNSUPPORTEDMODE","The display is currently in\nan unsupported mode."); break;
				case DDERR_VERTICALBLANKINPROGRESS : SE("DDERR_VERTICALBLANKINPROGRESS","A vertical blank is in progress."); break;
				case DDERR_VIDEONOTACTIVE: SE("DDERR_VIDEONOTACTIV","The video port is not active"); break;
				case DDERR_WASSTILLDRAWING : SE("DDERR_WASSTILLDRAWING","The previous blit operation that is transferring\ninformation to or from this\nsurface is incomplete."); break;
				case DDERR_WRONGMODE : SE("DDERR_WRONGMODE","This surface cannot be restored because it\nwas created in a different mode."); break;
				case DDERR_XALIGN : SE("DDERR_XALIGN","The provided rectangle was not horizontally\naligned on a required boundary."); break;
				default: SE("UNKNOWN ERROR", "Invalid error code specified"); break;
			};
			break;
		}
		case DxbugModule::ERROR_IUNKNOWN: SE("QUERY_INTERFACE", "The requested interface could not be obtained"); break;
	default:
		SE("Unknown", "An error in an unknown DirectX module/object was raised");
		break;
	};
	return err;
}

#pragma warning(pop) // disable : 4065

#pragma endregion
