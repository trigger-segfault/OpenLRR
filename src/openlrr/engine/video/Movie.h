// Movie.h : Header file for the C G98CMovie wrapper around the C++ implementation.
//
/// PURPOSE: Movie is used by all types of AVI video playback EXCEPT the credits starfield loop
///          This uses the IAMMultiMediaStream system, so this is why other video types are supported.
///
/// FILEIO: Data[std,cd]
/// APIS: IDirectDraw[24], IDirectDrawSurface[134], IDirectDrawStreamSample,
///       IDirectDrawMediaStream, IMediaStream, IAMMultiMediaStream
/// DEPENDENCIES: DirectDraw, Files
/// DEPENDENTS: FrontEnd, Lego, Rewards

#pragma once

#include "../../common.h"
#include "../geometry.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Typedefs
 **********************************************************************************/

#pragma region Typedefs

// C wrapper typedef around G98CMovie class.
typedef void Movie_t;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00472820>
Movie_t* __cdecl Movie_Load(const char* fName);

// cannot be const, due to using IDirectDraw-type interface
// <LegoRR.exe @00472930>
void __cdecl Movie_GetSize(Movie_t* mov, OUT uint32* width, OUT uint32* height);

// Gets the movie duration in milliseconds.
// cannot be const, due to using IMultiMedia-type interface
// <LegoRR.exe @00472980>
sint64 __cdecl Movie_GetDuration(Movie_t* mov);

// also performs BlitToBackBuffer
// float speed parameter is unused (name is assumed as 1.0f is always passed)
// <LegoRR.exe @00472990>
bool32 __cdecl Movie_Update(Movie_t* mov, real32 speed, const Rect2I* destRect);

// <LegoRR.exe @004729b0>
void __cdecl Movie_Free(Movie_t* mov);

#pragma endregion

} /* namespace Gods98 */
