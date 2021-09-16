
#include "legacy_d3drm.h"


/**********************************************************************************
 ******** IIDs
 **********************************************************************************/

#pragma region IIDs

/// TODO: We're currently just relying on the IID defined in LegoRR,
///        since our `d3drm.lib` won't give it to us. Later on this should be manually defined.

// {4a1b1be6-bfed-11d1-8ed8-00a0c967a482}
// <LegoRR.exe @004a0958>
IID & Idl::IID_IDirect3DRMViewport2 = *(IID*)0x004a0958;
// {eb16cb03-d271-11ce-ac48-0000c03825a1}
// <LegoRR.exe @004a0968>
IID & Idl::IID_IDirect3DRMFrame = *(IID*)0x004a0968;
// {ff6b7f70-a40e-11d1-91f9-0000f8758e66}
// <LegoRR.exe @004a0988>
IID & Idl::IID_IDirect3DRMFrame3 = *(IID*)0x004a0988;
// {a3a80d01-6e12-11cf-ac4a-0000c03825a1}
// <LegoRR.exe @004a09a8>
IID & Idl::IID_IDirect3DRMMesh = *(IID*)0x004a09a8;
// {4516ec77-8f20-11d0-9b6d-0000c0781bc3}
// <LegoRR.exe @004a09c8>
IID & Idl::IID_IDirect3DRMMeshBuilder2 = *(IID*)0x004a09c8;
// {eb16cb09-d271-11ce-ac48-0000c03825a1}
// <LegoRR.exe @004a0a18>
IID & Idl::IID_IDirect3DRMTexture = *(IID*)0x004a0a18;
// {59163de0-6d43-11cf-ac4a-0000c03825a1}
// <LegoRR.exe @004a0b48>
IID & Idl::IID_IDirect3DRMUserVisual = *(IID*)0x004a0b48;
// {4516ec83-8f20-11d0-9b6d-0000c0781bc3}
// <LegoRR.exe @004a0bd8>
IID & Idl::IID_IDirect3DRM3 = *(IID*)0x004a0bd8;

#pragma endregion
