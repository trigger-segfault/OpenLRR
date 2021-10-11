// Materials.cpp : 
//

#include "../../platform/d3drm.h"

#include "../core/Errors.h"
#include "../Main.h"

#include "Materials.h"


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00489780>
Gods98::Material* __cdecl Gods98::Material_Create(real32 emissiveRed, real32 emissiveGreen, real32 emissiveBlue,
									real32 specularRed, real32 specularGreen, real32 specularBlue, real32 power)
{
	log_firstcall();

	IDirect3DRMMaterial2* mat;

	if (lpD3DRM()->CreateMaterial(power, &mat) == D3DRM_OK) {
		mat->SetEmissive(emissiveRed, emissiveGreen, emissiveBlue);
		mat->SetSpecular(specularRed, specularGreen, specularBlue);
		return (Material*) mat;
	}

	return nullptr;
}


// <unused?>
void __cdecl Gods98::Material_Remove(Material* material)
{

	/// FIXME GODS98: using IDirect3DRMMaterial* instead of IDirect3DRMMaterial2*
	IDirect3DRMMaterial* mat = (IDirect3DRMMaterial*) material;

	Error_Fatal(!mat, "Null passed as material to Material_Remove()");

	mat->Release();
}

// <unused?>
void __cdecl Gods98::Material_SetEmissive(Material* material, real32 emissiveRed, real32 emissiveGreen, real32 emissiveBlue)
{
	/// FIXME GODS98: using IDirect3DRMMaterial* instead of IDirect3DRMMaterial2*
	IDirect3DRMMaterial* mat = (IDirect3DRMMaterial*) material;

	Error_Fatal(!mat, "Null passed as material to Material_SetEmissive()");

	mat->SetEmissive(emissiveRed, emissiveGreen, emissiveBlue);
}

// <unused?>
void __cdecl Gods98::Material_SetSpecular(Material* material, real32 specularRed, real32 specularGreen, real32 specularBlue, real32 power)
{
	/// FIXME GODS98: using IDirect3DRMMaterial* instead of IDirect3DRMMaterial2*
	IDirect3DRMMaterial* mat = (IDirect3DRMMaterial*) material;

	Error_Fatal(!mat, "Null passed as material to Material_SetSpecular()");

	mat->SetPower(power);
	mat->SetSpecular(specularRed, specularGreen, specularBlue);
}

#pragma endregion
