// Movie.cpp : Definitions file for the C++ Movie implementation and C wrapper.
//

#include "../drawing/DirectDraw.h"
#include "../core/Files.h"

#include "Movie.hpp"
#include "Movie.h"


/**********************************************************************************
 ******** Class Functions
 **********************************************************************************/

#pragma region Class Functions

// <LegoRR.exe @004724a0>
bool Gods98::G98CMovie::InitSample(IAMMultiMediaStream* lpAMMMStream)
{
	log_firstcall();

	this->m_err = lpAMMMStream->GetMediaStream(MSPID_PrimaryVideo, &this->m_sampleBaseStream);
	if (this->m_err >= 0) {

		this->m_err = this->m_sampleBaseStream->QueryInterface(IID_IDirectDrawMediaStream, (void**)&this->m_sampleStream);
		if (this->m_err >= 0) {

			this->m_err = this->m_sampleStream->CreateSample(nullptr, nullptr, 0, &this->m_sample);
			if (this->m_err >= 0) {

				this->m_err = this->m_sample->GetSurface(&this->m_baseSurf, (RECT*)&this->m_movieRect);
				if (this->m_err >= 0) {

					this->m_err = this->m_baseSurf->QueryInterface(IID_IDirectDrawSurface3, (void**)&this->m_surf);
					if (this->m_err >= 0) {

						this->m_err = lpAMMMStream->SetState(STREAMSTATE_RUN /*0x1*/);
						if (this->m_err >= 0)
							return true;

					}
				}
			}
		}
	}

	// hoh boy... failure is not an option...
	this->m_sampleBaseStream->Release(); // failure to GetMediaStream this->m_sampleBaseStream will probably crash here
	this->m_sampleStream->Release(); // failure to QueryInterface this->m_sampleStream will probably crash here
	this->m_sample->Release(); // failure to CreateSample this->m_sample will probably crash here
	this->m_baseSurf->Release(); // failure to GetSurface this->m_sampleSurf will probably crash here
	return (this->m_err >= 0);
}

// <LegoRR.exe @00472570>
bool Gods98::G98CMovie::OpenAMStream(const char* fName, IAMMultiMediaStream** lplpAMMMStream, IDirectDraw2* ddraw2)
{
	log_firstcall();

	IAMMultiMediaStream* amMMStream = nullptr; // dummy init
	wchar_t wFileName[MAX_PATH /*260*/];

	HRESULT r = ::CoCreateInstance(CLSID_AMMultiMediaStream, nullptr, CLSCTX_INPROC_SERVER /*0x1*/, //CLSCTX_INPROC_HANDLER /*0x1*/,
		IID_IAMMultiMediaStream, (void**)&amMMStream);
	if (r >= 0) {

		r = amMMStream->Initialize(STREAMTYPE_READ /*0x0*/, 0 /*separate-thread Filter Graph*/, nullptr);
		if (r >= 0) {

			r = amMMStream->AddMediaStream((IUnknown *)ddraw2, &MSPID_PrimaryVideo, 0 /*no flags*/, nullptr);
			if (r >= 0) {

				r = amMMStream->AddMediaStream(nullptr, &MSPID_PrimaryAudio, AMMSF_ADDDEFAULTRENDERER /*0x1*/, nullptr);
				if (r < 0)
					r = 0; // failure is just outright ignored here?

				// The IAMMultiMediaStream::OpenFile function only supports unicode,
				//  so convert our fName to wide chars.
				::MultiByteToWideChar(CP_ACP /*0x0*/, 0 /*no flags*/, fName, -1 /*null-terminated*/,
					wFileName, sizeof(wFileName) / sizeof(wchar_t) /*MAX_PATH, 260*/);

				//std::wprintf(L"%s\n", wFileName);
				amMMStream->OpenFile(wFileName, 0);

				*lplpAMMMStream = amMMStream; // assign to output
				amMMStream->AddRef(); // add ref for release at the bottom, kinda lazy
			}
		}
	}
	amMMStream->Release(); // failure to CoCreate amMMStream will probably crash here
	return (r >= 0);
}

// <LegoRR.exe @00472650>
Gods98::G98CMovie::G98CMovie(const char* fName, IDirectDrawSurface3* bSurf3, IDirectDraw2* ddraw2)
	: m_sampleBaseStream(nullptr), m_sampleStream(nullptr), m_baseSurf(nullptr), m_surf(nullptr), m_sample(nullptr)
	, m_movieRect({0,0,0,0}) // added, not originally part of ctor
{
	log_firstcall();

	this->m_bSurf = bSurf3;
	this->m_ddraw2 = ddraw2;

	::CoInitialize(nullptr);

	if (fName != nullptr && std::strlen(fName) != 0) {
		this->m_filename = ::_strdup(fName);

		if (this->OpenAMStream(fName, &this->m_amStream, ddraw2)) {
			this->InitSample(this->m_amStream);
		}
	}
}

// <LegoRR.exe @004726f0>
Gods98::G98CMovie::~G98CMovie()
{
	if (this->m_sampleBaseStream != nullptr)
		this->m_sampleBaseStream->Release();
		
	if (this->m_sampleStream != nullptr)
		this->m_sampleStream->Release();
		
	if (this->m_sample != nullptr)
		this->m_sample->Release();
		
	if (this->m_baseSurf != nullptr)
		this->m_baseSurf->Release();
		
	if (this->m_bSurf != nullptr)
		this->m_bSurf->Release();
		
	if (this->m_ddraw2 != nullptr)
		this->m_ddraw2->Release();
		
	if (this->m_amStream != nullptr)
		this->m_amStream->Release();

	if (this->m_filename != nullptr)
		std::free(this->m_filename);
}

// float speed parameter is unused (name is assumed as 1.0f is always passed)
// <LegoRR.exe @00472760>
bool Gods98::G98CMovie::Update(real32 speed, const Rect2I* destRect)
{
	if (this->m_sample == nullptr)
		return false;

	// Update playback (and renders to draw surface?)
	if (this->m_sample->Update(0 /*no flags*/, nullptr, nullptr, (DWORD_PTR)nullptr) != DD_OK) {
		// On failure... SHUTDOWN EVERYTHING!
		// This also probably executes once the movie is finished
		this->m_sampleBaseStream->Release();
		this->m_sampleBaseStream = nullptr;

		this->m_sampleStream->Release();
		this->m_sampleStream = nullptr;

		this->m_sample->Release();
		this->m_sample = nullptr;

		this->m_baseSurf->Release();
		this->m_baseSurf = nullptr;

		if (this->m_amStream != nullptr) {
			this->m_amStream->Release();
			this->m_amStream = nullptr;
		}
		return false;
	}

	// Copy from draw surface to render surface
	HRESULT r = this->m_bSurf->Blt(const_cast<RECT*>((const RECT*)destRect), this->m_surf, nullptr, DDBLT_WAIT /*0x1000000*/, nullptr);
	return (r == DD_OK);
}

// cannot be const, due to using IMultiMedia-type interface
// <LegoRR.exe @004727f0>
sint64 Gods98::G98CMovie::GetDuration()
{
	/*sint64*/ STREAM_TIME duration = 0LL; // dummy init
	this->m_amStream->GetDuration(&duration);
	// __alldiv - 64-bit integer division in 32-bit environments
	return duration / 10000LL; // 100-nanosecond units -> milliseconds
}

#pragma endregion


/**********************************************************************************
 ******** C Functions
 **********************************************************************************/

#pragma region C Functions

// <LegoRR.exe @00472820>
Gods98::Movie_t* __cdecl Gods98::Movie_Load(const char* fName)
{
	log_firstcall();

	char cdFileName[512];
	IDirectDraw2* ddraw2;
	IDirectDrawSurface3* bSurf3;

	/* actual start of C++ function */
	const char* name = File_VerifyFilename(fName);

	DirectDraw()->QueryInterface(IID_IDirectDraw2, (void**)&ddraw2);
	DirectDraw_bSurf()->QueryInterface(IID_IDirectDrawSurface3, (void**)&bSurf3);

	G98CMovie* movie = nullptr;
	FILE* file = std::fopen(name, "r"); // the "r" rather than "rb" seems like a bad idea...
	if (file != nullptr) {
		// Open file in local Data folder.
		// Close file stream, since it's only the equivalent of "file exists"
		std::fclose(file);

		movie = new G98CMovie(name, bSurf3, ddraw2);
	}
	else if (File_GetCDFilePath(cdFileName, fName)) {
		// File not found in local Data folder, Try open in CDROM/Data folder.
		// Does not check if the file exists here.

		movie = new G98CMovie(cdFileName, bSurf3, ddraw2);
	}
	//else // file not found in local data dir, and CDROM not found (doesn't check if exists on CDROM)

	return (Movie_t*)movie;
}

// cannot be const, due to using IDirectDraw-type interface
// <LegoRR.exe @00472930>
void __cdecl Gods98::Movie_GetSize(Movie_t* mov, OUT uint32* width, OUT uint32* height)
{
	log_firstcall();

	DDSURFACEDESC surfDesc;
	std::memset(&surfDesc, 0, sizeof(surfDesc) /*0x6c*/);
	surfDesc.dwSize = sizeof(surfDesc) /*0x6c*/;

	G98CMovie* movie = (G98CMovie*)mov;

	movie->GetSurface()->GetSurfaceDesc(&surfDesc);
	//movie->m_surf->GetSurfaceDesc(&surfDesc);

	*width  = surfDesc.dwWidth;
	*height = surfDesc.dwHeight;
}

// Gets the movie duration in milliseconds.
// cannot be const, due to using IMultiMedia-type interface
// <LegoRR.exe @00472980>
sint64 __cdecl Gods98::Movie_GetDuration(Movie_t* mov)
{
	log_firstcall();

	G98CMovie* movie = (G98CMovie*)mov;

	return movie->GetDuration();
}

// float speed parameter is unused (name is assumed as 1.0f is always passed)
// <LegoRR.exe @00472990>
bool32 __cdecl Gods98::Movie_Update(Movie_t* mov, real32 speed, const Rect2I* destRect)
{
	log_firstcall();

	G98CMovie* movie = (G98CMovie*)mov;

	return (bool32)(movie->Update(speed, destRect) != false);
}

// <LegoRR.exe @004729b0>
void __cdecl Gods98::Movie_Free(Movie_t* mov)
{
	log_firstcall();

	G98CMovie* movie = (G98CMovie*)mov;

	if (movie) delete movie;

	// delete movie;
	//Movie___dtor((Movie *)movie);
	// This function is multi-purpose (the linker assigned functions with the same
	//  functionality to different areas). So distinguishing is annoying.
	//game::LevelStruct3C_Free_Unwind((LevelStruct_3c *)movie);
}

#pragma endregion
