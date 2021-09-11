

//#include "../DirectX/D3DRM/d3drm.h"
#include <d3drm.h>

#include "Errors.h"
#include "Main.h"
#include "Materials.h"


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00489780>
Gods98::Material_Dummy* __cdecl Gods98::Material_Create(real32 emissiveRed, real32 emissiveGreen, real32 emissiveBlue,
								real32 specularRed, real32 specularGreen, real32 specularBlue, real32 power)
{
	IDirect3DRMMaterial2* mat;

	if (lpD3DRM()->CreateMaterial(power, &mat) == D3DRM_OK) {
		mat->SetEmissive(emissiveRed, emissiveGreen, emissiveBlue);
		mat->SetSpecular(specularRed, specularGreen, specularBlue);
		return (Material_Dummy*) mat;
	}

	return nullptr;
}


// <unused?>
void __cdecl Gods98::Material_Remove(Material_Dummy* material){

	/// FIXME: using IDirect3DRMMaterial* instead of IDirect3DRMMaterial2*
	IDirect3DRMMaterial* mat = (IDirect3DRMMaterial*) material;

	Error_Fatal(!mat, "Null passed as material to Material_Remove()");

	mat->Release();
}

// <unused?>
void __cdecl Gods98::Material_SetEmissive(Material_Dummy* material, real32 emissiveRed, real32 emissiveGreen, real32 emissiveBlue){

	/// FIXME: using IDirect3DRMMaterial* instead of IDirect3DRMMaterial2*
	IDirect3DRMMaterial* mat = (IDirect3DRMMaterial*) material;

	Error_Fatal(!mat, "Null passed as material to Material_SetEmissive()");

	mat->SetEmissive(emissiveRed, emissiveGreen, emissiveBlue);
}

// <unused?>
void __cdecl Gods98::Material_SetSpecular(Material_Dummy* material, real32 specularRed, real32 specularGreen, real32 specularBlue, real32 power){

	/// FIXME: using IDirect3DRMMaterial* instead of IDirect3DRMMaterial2*
	IDirect3DRMMaterial* mat = (IDirect3DRMMaterial*) material;

	Error_Fatal(!mat, "Null passed as material to Material_SetSpecular()");

	mat->SetPower(power);
	mat->SetSpecular(specularRed, specularGreen, specularBlue);
}

#pragma endregion
