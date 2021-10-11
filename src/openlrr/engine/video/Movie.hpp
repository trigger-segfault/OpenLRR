// Movie.hpp : Header file for the C++ G98CMovie implementation.
//

#pragma once

#include "../../platform/windows.h"
#include "../../platform/ddraw.h"
#include <amstream.h>

#include "../../common.h"
#include "../geometry.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Classes
 **********************************************************************************/

#pragma region Classes

/// PRIVATE:
class G98CMovie
{
public:
	// (no vftable)
	/*00,4*/ IAMMultiMediaStream* m_amStream;
	/*04,4*/ HRESULT m_err;
	/*08,4*/ IMediaStream* m_sampleBaseStream;
	/*0c,4*/ IDirectDrawMediaStream* m_sampleStream;
	/*10,4*/ IDirectDrawSurface* m_baseSurf; // base surface for IDirectDrawStreamSample* m_sample
	/*14,4*/ IDirectDrawSurface3* m_surf; // DDS3 surface for IDirectDrawStreamSample* m_sample
	/*18,4*/ IDirectDrawStreamSample* m_sample;
	/*1c,4*/ IDirectDrawSurface3* m_bSurf; // render target passed in by constructor
	/*2c,10*/ Rect2I m_movieRect; // rect for IDirectDrawStreamSample* m_sample
	/*30,4*/ char* m_filename;
	/*34,4*/ IDirectDraw2* m_ddraw2;
	/*38*/

private:
	// <LegoRR.exe @004724a0>
	bool InitSample(IAMMultiMediaStream* lpAMMMStream);
	// <LegoRR.exe @00472570>
	bool OpenAMStream(const char* fName, IAMMultiMediaStream** lplpAMMMStream, IDirectDraw2* ddraw2);

public:
	// <LegoRR.exe @00472650>
	G98CMovie(const char* fName, IDirectDrawSurface3* bSurf3, IDirectDraw2* ddraw2);
	// <LegoRR.exe @004726f0>
	~G98CMovie();

	// float speed parameter is unused (name is assumed as 1.0f is always passed)
	// <LegoRR.exe @00472760>
	bool Update(real32 speed, const Rect2I* destRect);
	// cannot be const, due to using IMultiMedia-type interface
	// <LegoRR.exe @004727f0>
	sint64 GetDuration();


	// <inlined>
	IDirectDrawSurface3* GetSurface() { return this->m_surf; }
};
static_assert(sizeof(G98CMovie) == 0x38, "");

#pragma endregion

} /* namespace Gods98 */
