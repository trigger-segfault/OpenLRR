// GameState.cpp : A seperate source file is used to define these functions,
//                 due to their massive size.
//

#include "../engine/audio/3DSound.h"
#include "../engine/audio/Sound.h"
#include "../engine/core/Config.h"
#include "../engine/core/Errors.h"
#include "../engine/core/Files.h"
#include "../engine/core/Maths.h"
#include "../engine/core/Memory.h"
#include "../engine/core/Utils.h"
#include "../engine/drawing/DirectDraw.h"
#include "../engine/drawing/Images.h"
#include "../engine/drawing/Fonts.h"
#include "../engine/drawing/TextWindow.h"
#include "../engine/gfx/Containers.h"
#include "../engine/gfx/Lws.h"
#include "../engine/gfx/Mesh.h"
#include "../engine/gfx/Viewports.h"
#include "../engine/input/Input.h"
#include "../engine/input/Keys.h"
#include "../engine/input/MouseButtons.h"
#include "../engine/video/Movie.h"
#include "../engine/Graphics.h"
#include "../engine/Main.h"
#include "../engine/geometry.h"
#include "../engine/colour.h"
#include "audio/SFX.h"
#include "effects/DamageText.h"
#include "effects/Effects.h"
#include "effects/LightEffects.h"
#include "effects/Smoke.h"
#include "front/FrontEnd.h"
#include "front/Loader.h"
#include "front/Reward.h"
#include "interface/hud/Bubbles.h"
#include "interface/hud/ObjInfo.h"
#include "interface/Advisor.h"
#include "interface/Encyclopedia.h"
#include "interface/HelpWindow.h"
#include "interface/InfoMessages.h"
#include "interface/Interface.h"
#include "interface/Panels.h"
#include "interface/Pointers.h"
#include "interface/Priorities.h"
#include "interface/RadarMap.h"
#include "interface/ScrollInfo.h"
#include "interface/TextMessages.h"
#include "interface/ToolTip.h"
#include "mission/Messages.h"
#include "mission/NERPsFile.h"
#include "mission/NERPsFunctions.h"
#include "mission/Objective.h"
#include "object/AITask.h"
#include "object/Building.h"
#include "object/Creature.h"
#include "object/Dependencies.h"
#include "object/MeshLOD.h"
#include "object/Object.h"
#include "object/Stats.h"
#include "object/Upgrade.h"
#include "object/Vehicle.h"
#include "object/Weapons.h"
#include "world/Camera.h"
#include "world/Construction.h"
#include "world/Detail.h"
#include "world/ElectricFence.h"
#include "world/Erode.h"
#include "world/Fallin.h"
#include "world/Map3D.h"
#include "world/Roof.h"
#include "world/SelectPlace.h"
#include "world/SpiderWeb.h"
#include "world/Teleporter.h"
#include "world/Water.h"
#include "Debug.h"
#include "Shortcuts.hpp"
#include "Game.h"


using Gods98::Keys;
using Shortcuts::ShortcutID;


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

#if true
// <LegoRR.exe @0041fa80>
bool32 __cdecl LegoRR::Lego_Initialise(void)
{
	char* Window_stringParts[4];
	char* Button_stringParts[10];
	char Button_buffer[128];
	char* ProgressWindow_stringParts[10];

	const uint32 loadStartTime = Gods98::Main_GetTime();

	legoGlobs.DragBoxRGB = ColourRGBF { 0.2f, 0.7f, 1.0f }; // { 51, 178.5, 255 }

	SurfaceType_RegisterName(Lego_SurfaceType_Tunnel);
	SurfaceType_RegisterName(Lego_SurfaceType_Immovable);
	SurfaceType_RegisterName(Lego_SurfaceType_Hard);
	SurfaceType_RegisterName(Lego_SurfaceType_Medium);
	SurfaceType_RegisterName(Lego_SurfaceType_Loose);
	SurfaceType_RegisterName(Lego_SurfaceType_Soil);
	SurfaceType_RegisterName(Lego_SurfaceType_Lava);
	SurfaceType_RegisterName(Lego_SurfaceType_Water);
	SurfaceType_RegisterName(Lego_SurfaceType_OreSeam);
	SurfaceType_RegisterName(Lego_SurfaceType_Lake);
	SurfaceType_RegisterName(Lego_SurfaceType_CrystalSeam);
	SurfaceType_RegisterName(Lego_SurfaceType_RechargeSeam);
	SurfaceType_RegisterName(Lego_SurfaceType_Rubble);
	SurfaceType_RegisterName(Lego_SurfaceType_Reinforcement);
	SurfaceType_RegisterName(Lego_SurfaceType_Path);
	SurfaceType_RegisterName(Lego_SurfaceType_SlugHole);
	SurfaceType_RegisterName(Lego_SurfaceType_Undiscovered);
	SurfaceType_RegisterName(Lego_SurfaceType_Cavern);

	for (uint32 surfaceIdx_unused = 0; surfaceIdx_unused < Lego_SurfaceType_Count; surfaceIdx_unused++) {
		// Debug checks (can be found in Fallin/RockFall(?) beta)
	}


	Lego_SetGameSpeed(1.0f);

	Gods98::Keys_Initialise();
	/// NEW: MouseButtons enum
	Gods98::MouseButtons_Initialise();
	Gods98::Viewport_Initialise();

	LegoObject_Initialise();

	Gods98::Image_Initialise();
	
	Shortcuts::shortcutManager.Initialise();
	Shortcuts::shortcutManager.Load();

	//Gods98::TextWindow_Initialise(); // Determined using RockFall beta (function does nothing).

	RadarMap_Initialise();
	Panel_Initialise();
	AITask_Initialise();
	Message_Initialise();
	Smoke_Initialise();
	Pointer_Initialise();
	Bubble_Initialise();


	legoGlobs.fontStandard   = Gods98::Font_Load("Interface\\Fonts\\FONT5_HI.bmp");
	legoGlobs.fontToolTip    = Gods98::Font_Load("Interface\\Fonts\\ToolTipFont.bmp");
	legoGlobs.fontTallYellow = Gods98::Font_Load("Interface\\Fonts\\DeskTopFont.bmp");
	legoGlobs.fontTextWindow = Gods98::Font_Load("Interface\\Fonts\\font5_HI.bmp");
	legoGlobs.fontBriefingLo = Gods98::Font_Load("Interface\\Fonts\\MBriefFONT.bmp");
	legoGlobs.fontBriefingHi = Gods98::Font_Load("Interface\\Fonts\\MBriefFONT2.bmp");
	legoGlobs.fontCredits    = Gods98::Font_Load("Interface\\Fonts\\RSFont.bmp");


	legoGlobs.rootCont = Gods98::Container_Initialise(legoGlobs.gameName);
	if (legoGlobs.rootCont == nullptr)
		return false;

	Gods98::Container_SetTriggerFrameCallback(LegoObject_TriggerFrameCallback, nullptr);


	/// NEW: Handle extra command line options for config loading.
	{
		// Handle overriding the root config name.
		const char* rootConfigName = Gods98::Main_GetConfigName(LEGO_CONFIGFILENAME);

		// Handle overriding the load location priority of the root config.
		Gods98::FileFlags rootConfigFlags = Gods98::FileFlags::FILE_FLAGS_DEFAULT;
		if (Gods98::Main_IsConfigDataPriority())
			rootConfigFlags |= Gods98::FileFlags::FILE_FLAG_DATAPRIORITY;

		legoGlobs.config = Gods98::Config_Load2(rootConfigName, rootConfigFlags);
	}
	Gods98::Config* legoConfig = legoGlobs.config;
	if (legoGlobs.config == nullptr) {
		//goto LAB_0042274b;
		Gods98::Container_Shutdown();
		return false;
	}

	/// NEW: Handle command line option to append additional configs.
	for (const auto& nextConfigName : Gods98::Main_GetConfigAppends()) {
		Gods98::Config* nextConfig = Gods98::Config_Load2(nextConfigName.c_str(), Gods98::FileFlags::FILE_FLAG_DATAPRIORITY);
		Error_WarnF((nextConfig==nullptr), "Could not append config. Failed to load: %s", nextConfigName.c_str());
		if (nextConfig != nullptr) {
			Gods98::Config_AppendConfig(legoConfig, nextConfig);
		}
	}

	/// NEW: Store LoseFocusAndPause property so that we don't need to look it up on-demand.
	legoGlobs2.loseFocusAndPause = Config_GetBoolOrFalse(legoConfig, Main_ID("LoseFocusAndPause"));

	ColourRGBF ToolTipRGB = { 0.0f }; // dummy init

	if (!Gods98::Config_GetRGBValue(legoConfig, Main_ID("ToolTipRGB"), &ToolTipRGB.red, &ToolTipRGB.green, &ToolTipRGB.blue)) {
		ToolTipRGB.red   = (83.0f / 255.0f);// 0.3254902f;
		ToolTipRGB.green = (83.0f / 255.0f);// 0.3254902f;
		ToolTipRGB.blue  = (83.0f / 255.0f);// 0.3254902f;
	}

	ToolTip_Initialise(legoGlobs.fontToolTip, 2, 1, 1.0f, Gods98::appWidth(), Gods98::appHeight(),
					   32, ToolTipRGB.red, ToolTipRGB.green, ToolTipRGB.blue);

	/// FIX APPLY: Move Interface_Initialise after ToolTip_Initialise so that the SetRightAlign call actually works.
	Info_Initialise(legoGlobs.fontStandard); // This font is never used by the Info module.
	Interface_Initialise(565, 18, legoGlobs.fontToolTip);

	legoGlobs.CreditsTextFile = Gods98::Config_GetStringValue(legoConfig, Main_ID("CreditsTextFile"));
	legoGlobs.CreditsBackAVI = Gods98::Config_GetStringValue(legoConfig, Main_ID("CreditsBackAVI"));


	legoGlobs.BuildingUpgradeCostOre = Config_GetIntValue(legoConfig, Main_ID("BuildingUpgradeCostOre"));
	if (legoGlobs.BuildingUpgradeCostOre == 0)
		legoGlobs.BuildingUpgradeCostOre = 5;

	legoGlobs.BuildingUpgradeCostStuds = Config_GetIntValue(legoConfig, Main_ID("BuildingUpgradeCostStuds"));
	if (legoGlobs.BuildingUpgradeCostStuds == 0)
		legoGlobs.BuildingUpgradeCostStuds = 1;

	legoGlobs.RenameReplace = Gods98::Config_GetStringValue(legoConfig, Main_ID("RenameReplace"));
	if (legoGlobs.RenameReplace != nullptr) {
		for (char* s = legoGlobs.RenameReplace; *s != '\0'; s++) {
			if (*s == '_') *s = ' ';
		}
	}

	uint32 videoMemTotal = 0, videoMemFree = 0; // dummy inits
	if (Gods98::DirectDraw_GetAvailTextureMem(&videoMemTotal, &videoMemFree)) {

		uint32 TextureUsage = Config_GetIntValue(legoConfig, Main_ID("TextureUsage"));
		if (TextureUsage != 0) {
			// Convert from...*breaths in*... PIXELS!?... to bytes... why THIS is the unit of choice...??
			Gods98::DirectDraw_AdjustTextureUsage(&TextureUsage);
			if (TextureUsage <= videoMemFree) {
				// No need to manage textures if we can have them all loaded at once(?)
				Gods98::Graphics_DisableTextureManagement();
			}
		}
	}

	const Area2F fontRender80_rect = Area2F { 55.0f, 435.0f, 325.0f, 42.0f };
	legoGlobs.textOnlyWindow = Gods98::TextWindow_Create(legoGlobs.fontTextWindow, &fontRender80_rect, 0x400);

	const Area2F fontRender84_rect = Area2F { 55.0f, 435.0f, 260.0f, 42.0f };
	legoGlobs.textImageWindow = Gods98::TextWindow_Create(legoGlobs.fontTextWindow, &fontRender84_rect, 0x400);

	real32 textPauseTime = Config_GetRealValue(legoConfig, Main_ID("TextPauseTime"));
	Text_Load(legoGlobs.textOnlyWindow, legoGlobs.textImageWindow, 316, 425, (textPauseTime * STANDARD_FRAMERATE));


	const Area2F MsgPanel_rect1 = Area2F { 55.0f, 435.0f, 325.0f, 42.0f };
	const Area2F MsgPanel_rect2 = Area2F { 55.0f, 363.0f, 325.0f, 140.0f };
	Text_Initialise("Interface\\MessagePanel\\MsgPanel.bmp", 42, 409, 338, &MsgPanel_rect1, &MsgPanel_rect2, 20.0f);


	if (Gods98::Main_IsShowVersion()) {
		char* startMessage = Gods98::Config_GetStringValue(legoConfig, Main_ID("StartMessage"));
		const char* versionString = Gods98::Config_GetTempStringValue(legoConfig, Main_ID("Version"));
		if (startMessage != nullptr) {
			/// NOTE: Original code reassigns startMessage to return value, which has no effect.
			///       But for sanity's sake, it's bad logic if we're planning on freeing startMessage down below.
			//startMessage = Front_Util_StringReplaceChar(startMessage, '_', ' ');
			Front_Util_StringReplaceChar(startMessage, '_', ' ');

			if (versionString == nullptr) {
				Gods98::TextWindow_PrintF(legoGlobs.textOnlyWindow, "\n%s", startMessage);
			}
			else {
				Gods98::TextWindow_PrintF(legoGlobs.textOnlyWindow, "\n%s (%s)", startMessage, versionString);
			}
			Gods98::Mem_Free(startMessage); //util::free2(startMessage);
		}
	}

	// When a movie is loaded, it changes something about how audio memory is handled (possibly DMA, Direct Memory Access).
	// When this change is made, audio in LRR/OpenLRR loads lightning fast, and load times overall are increased by ~200%!!
	// Normally it's the biggest load time, but now it's not even on the radar.
	const bool abuseMovieLoad = true;

	// This doesn't need to be called if we're already successfully loading other movie files.
	// But the Movie module won't detect if it succesfully opened a movie file that wasn't valid media.
	// So always place this in at the top. In-case the user removes the AVI's or if the files just aren't there.
	if (abuseMovieLoad) {
		Gods98::Movie_ImproveAudioLoadSpeed();
	}

	if (Front_IsFrontEndEnabled() && Front_IsIntrosEnabled() && Gods98::Main_IsIntrosEnabled()) {
		Front_PlayIntroMovie("LegoAvi", (Gods98::Main_ProgrammerMode() != 0));
		Front_PlayIntroSplash("DDILogo", true, "DDILogoTime");
		Front_PlayIntroMovie("DDIAvi", true);
		Front_PlayIntroMovie("RRAvi", true);
		Front_PlayIntroSplash("LMILogo", (Gods98::Main_ProgrammerMode() != 0), "LMILogoTime");
	}

	SFX_Initialise();



	const char* progressBarName = Gods98::Config_GetTempStringValue(legoConfig, Main_ID("ProgressBar"));
	const char* loadScreenName = Gods98::Config_GetTempStringValue(legoConfig, Main_ID("LoadScreen"));
	const char* shutdownScreenName = Gods98::Config_GetTempStringValue(legoConfig, Main_ID("ShutdownScreen"));
	const char* loadingText = Gods98::Config_GetTempStringValue(legoConfig, Main_ID("LoadingText"));

	Direction progressBarDirection = DIRECTION__COUNT; // dummy init
	Area2F progressWindowRect = Area2F { 0.0f, 0.0f, 0.0f, 0.0f }; // dummy init

	char* progressWindow = Gods98::Config_GetStringValue(legoConfig, Main_ID("ProgressWindow"));
	if (progressWindow == nullptr) {
		progressBarName = nullptr;
	}
	else {
		// Case-insensitive switch case for direction character
		// NOTE: A config value can never be an empty string. So this will always access valid memory.
		progressWindow[0] = (char)std::toupper(progressWindow[0]);

		switch (*progressWindow) {
		case 'D': progressBarDirection = DIRECTION_DOWN;   break;
		case 'L': progressBarDirection = DIRECTION_LEFT;   break;
		case 'R': progressBarDirection = DIRECTION_RIGHT;  break;
		case 'U': progressBarDirection = DIRECTION_UP;     break;
		default:  progressBarDirection = DIRECTION__COUNT; break;
		}

		const uint32 numParts = Gods98::Util_Tokenise(progressWindow, ProgressWindow_stringParts, ",");
		if (numParts == 4 && progressWindow[1] == ':' && progressBarDirection != DIRECTION__COUNT) {
			progressWindowRect = Area2F {
				(real32)std::atoi(ProgressWindow_stringParts[0] + 2), // skip first 2 characters with direction prefix
				(real32)std::atoi(ProgressWindow_stringParts[1]),
				(real32)std::atoi(ProgressWindow_stringParts[2]),
				(real32)std::atoi(ProgressWindow_stringParts[3]),
			};
		}

		/// FIX APPLY: Move memory free INSIDE block that confirms its non-null.
		Gods98::Mem_Free(progressWindow);// util::free2(progressWindow);
	}


	const char* loaderProfileName;
	if (!Gods98::Sound_IsInitialised())
		loaderProfileName = "LoaderProfileNoSound.txt";
	else
		loaderProfileName = "LoaderProfile.txt";

	Loader_Initialise(loadScreenName, shutdownScreenName, legoGlobs.fontStandard, loaderProfileName,
					  progressBarDirection, progressBarName, &progressWindowRect, loadingText);

	Loader_display_loading_bar("Game Data");


	legoGlobs.cameraMain = Camera_Create(legoGlobs.rootCont, LegoCamera_Top);
	if (legoGlobs.cameraMain == nullptr) {
		//goto LAB_0042274b;
		Gods98::Container_Shutdown();
		return false;
	}

	legoGlobs.cameraFP = Camera_Create(legoGlobs.rootCont, LegoCamera_FP);
	if (legoGlobs.cameraFP == nullptr) {
		//goto LAB_0042274b;
		Gods98::Container_Shutdown();
		return false;
	}

	legoGlobs.cameraTrack = Camera_Create(legoGlobs.rootCont, LegoCamera_Radar);
	if (legoGlobs.cameraTrack == nullptr) {
		//goto LAB_0042274b;
		Gods98::Container_Shutdown();
		return false;
	}

	legoGlobs.viewMain = Gods98::Viewport_Create(0.0f, 0.0f, 1.0f, 1.0f, (legoGlobs.cameraMain)->contCam);
	if (legoGlobs.viewMain == nullptr) {
		//goto LAB_0042274b;
		Gods98::Container_Shutdown();
		return false;
	}

	legoGlobs.viewTrack = Gods98::Viewport_CreatePixel(16, 13, 151, 151, (legoGlobs.cameraTrack)->contCam);
	if (legoGlobs.viewTrack == nullptr) {
		//goto LAB_0042274b;
		Gods98::Container_Shutdown();
		return false;
	}


	if (Config_GetBoolOrFalse(legoConfig, Main_ID("StreamNERPSSpeach"))) {
		legoGlobs.flags1 |= GAME1_STREAMNERPSSPEACH;
	}

	if (Config_GetBoolOrFalse(legoConfig, Main_ID("ShowDebugToolTips")) || Gods98::Main_ProgrammerMode()) {
		legoGlobs.flags2 |= GAME2_SHOWDEBUGTOOLTIPS;
	}

	if (Config_GetBoolOrFalse(legoConfig, Main_ID("AllowDebugKeys")) || Gods98::Main_ProgrammerMode()) {
		legoGlobs.flags2 |= GAME2_ALLOWDEBUGKEYS;
	}

	if (Config_GetBoolOrFalse(legoConfig, Main_ID("AllowEditMode")) || Gods98::Main_ProgrammerMode()) {
		legoGlobs.flags2 |= GAME2_ALLOWEDITMODE;
	}


	Gods98::Config_GetRGBValue(legoConfig, Main_ID("PowerCrystalRGB"),
							   &legoGlobs.PowerCrystalRGB.red, &legoGlobs.PowerCrystalRGB.green, &legoGlobs.PowerCrystalRGB.blue);

	Gods98::Config_GetRGBValue(legoConfig, Main_ID("UnpoweredCrystalRGB"),
							   &legoGlobs.UnpoweredCrystalRGB.red, &legoGlobs.UnpoweredCrystalRGB.green, &legoGlobs.UnpoweredCrystalRGB.blue);


	Lego_LoadSamples(legoConfig, !Gods98::Graphics_IsReduceSamples());

	Lego_LoadSurfaceTypeDescriptions_sound(legoConfig, legoGlobs.gameName);

	Priorities_LoadPositions(legoConfig, legoGlobs.gameName);
	Priorities_LoadImages(legoConfig, legoGlobs.gameName);
	Lego_LoadToolTipInfos(legoConfig, legoGlobs.gameName);
	Object_LoadToolNames(legoConfig, legoGlobs.gameName);
	Panel_RotationControl_Initialise(legoConfig, legoGlobs.gameName);


	// See: SFX_GetType
	//      SFX_Random_PlaySound3DOnFrame
	const char* sharedObjectDir = Gods98::Config_GetTempStringValue(legoConfig, Main_ID("SharedObjects"));
	Gods98::Lws_Initialise(sharedObjectDir, SFX_Callback_FindSFXIDFunc, SFX_Callback_PlaySample3DFunc, SFX_IsSoundOn);

	const char* sharedTextureDir = Gods98::Config_GetTempStringValue(legoConfig, Main_ID("SharedTextures"));
	if (sharedTextureDir != nullptr) {
		Gods98::Container_SetSharedTextureDirectory(sharedTextureDir);
	}
	Gods98::Mesh_Initialise(sharedTextureDir);


	std::sprintf(Button_buffer, "NextButton%ix%i", Gods98::appWidth(), Gods98::appHeight());
	const char* nextButton = Gods98::Config_GetTempStringValue(legoConfig, Main_ID(Button_buffer)); // "NextButton%ix%i"
	if (nextButton != nullptr) {
		legoGlobs.NextButtonImage = Gods98::Image_LoadBMP(nextButton);
		if (legoGlobs.NextButtonImage != nullptr) {
			Gods98::Image_SetupTrans(legoGlobs.NextButtonImage, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

			std::sprintf(Button_buffer, "NextButtonPos%ix%i", Gods98::appWidth(), Gods98::appHeight());
			char* nextButtonPos = Gods98::Config_GetStringValue(legoConfig, Main_ID(Button_buffer)); // "NextButtonPos%ix%i"
			if (nextButtonPos != nullptr) {
				const uint32 numParts = Gods98::Util_Tokenise(nextButtonPos, Button_stringParts, ",");
				if (numParts == 2) {
					legoGlobs.NextButtonPos.x = (real32)std::atof(Button_stringParts[0]);
					legoGlobs.NextButtonPos.y = (real32)std::atof(Button_stringParts[1]);
				}
				Gods98::Mem_Free(nextButtonPos);// util::free2(nextButtonPos);
			}
		}
	}

	std::sprintf(Button_buffer, "BackButton%ix%i", Gods98::appWidth(), Gods98::appHeight());
	const char* backButton = Gods98::Config_GetTempStringValue(legoConfig, Main_ID(Button_buffer)); // "BackButton%ix%i"
	if (backButton != nullptr) {

		legoGlobs.RepeatButtonImage = Gods98::Image_LoadBMP(backButton);
		if (legoGlobs.RepeatButtonImage != nullptr) {
			Gods98::Image_SetupTrans(legoGlobs.RepeatButtonImage, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

			std::sprintf(Button_buffer, "BackButtonPos%ix%i", Gods98::appWidth(), Gods98::appHeight());
			char* backButtonPos = Gods98::Config_GetStringValue(legoConfig, Main_ID(Button_buffer)); // "BackButtonPos%ix%i"
			if (backButtonPos != nullptr) {
				const uint32 numParts = Gods98::Util_Tokenise(backButtonPos, Button_stringParts, ",");
				if (numParts == 2) {
					legoGlobs.RepeatButtonPos.x = (real32)std::atof(Button_stringParts[0]);
					legoGlobs.RepeatButtonPos.y = (real32)std::atof(Button_stringParts[1]);
				}
				Gods98::Mem_Free(backButtonPos);// util::free2(backButtonPos);
			}
		}
	}

	const char* backArrow = Gods98::Config_GetTempStringValue(legoConfig, Main_ID("BackArrow"));
	if (backArrow != nullptr) {
		legoGlobs.BackArrowImage = Gods98::Image_LoadBMP(backArrow);
		if (legoGlobs.BackArrowImage != nullptr) {
			Gods98::Image_SetupTrans(legoGlobs.BackArrowImage, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		}
	}

	NERPs_SetHasNextButton((legoGlobs.NextButtonImage != nullptr));

	if (Lego_LoadLighting() && Lego_LoadGraphicsSettings() && Lego_LoadUpgradeTypes() &&
		Lego_LoadVehicleTypes() && Lego_LoadMiniFigureTypes() && Lego_LoadRockMonsterTypes() &&
		Lego_LoadBuildingTypes())
	{

		real32 camMaxSpeed = Config_GetRealValue(legoConfig, Main_ID("CameraSpeed"));
		real32 camDeceleration = Config_GetRealValue(legoConfig, Main_ID("CameraDropOff"));
		real32 camAcceleration = Config_GetRealValue(legoConfig, Main_ID("CameraAcceleration"));
		sint32 mouseScrollIndent = Config_GetIntValue(legoConfig, Main_ID("MouseScrollIndent"));
		Camera_InitCameraMovements(camMaxSpeed, camDeceleration, camAcceleration, mouseScrollIndent);

		legoGlobs.MinDistFor3DSoundsOnTopView = Config_GetRealValue(legoConfig, Main_ID("MinDistFor3DSoundsOnTopView"));
		if (legoGlobs.MinDistFor3DSoundsOnTopView == 0.0f)
			legoGlobs.MinDistFor3DSoundsOnTopView = 150.0f;


		
		real32 maxDistFor3DSounds = Config_GetRealValue(legoConfig, Main_ID("MaxDistFor3DSounds"));
		if (maxDistFor3DSounds != 0.0f) {
			Gods98::Sound3D_SetMaxDist(maxDistFor3DSounds);
		}

		real32 rollOffFor3DSounds = Config_GetRealValue(legoConfig, Main_ID("RollOffFor3DSounds"));
		if (rollOffFor3DSounds == 0.0f)
			rollOffFor3DSounds = 1.0f;
		/// REFACTOR: Original code would always call this with 1.0f first, then
		///           only call this to change it if config value was non-zero.
		Gods98::Sound3D_SetRollOffFactor(rollOffFor3DSounds);


		legoGlobs.selectPlace = SelectPlace_Create(legoGlobs.rootCont, 5.0f);
		SelectPlace_SetArrowVisibility(legoGlobs.selectPlace, SelectPlace_ArrowVisibility::Always);
		SelectPlace_Hide(legoGlobs.selectPlace, true);


		legoGlobs.MaxReturnedCrystals = Config_GetIntValue(legoConfig, Main_ID("MaxReturnedCrystals"));
		if (legoGlobs.MaxReturnedCrystals == 0)
			legoGlobs.MaxReturnedCrystals = 10;


		legoGlobs.MouseScrollBorder = Config_GetIntValue(legoConfig, Main_ID("MouseScrollBorder"));
		if (legoGlobs.MouseScrollBorder == 0)
			legoGlobs.MouseScrollBorder = 5;

		Lego_LoadObjectNames(legoConfig);
		Lego_LoadObjectTheNames(legoConfig);

		if (Config_GetBoolOrFalse(legoConfig, Main_ID("HelpWindowOn"))) {
			HelpWindow_SetFont(legoGlobs.fontBriefingHi);
			HelpWindow_Initialise(legoConfig, legoGlobs.gameName);
		}

		ObjInfo_Initialise(legoConfig, legoGlobs.gameName);
		LightEffects_Load(legoConfig, legoGlobs.gameName);
		Stats_Initialise(legoConfig, legoGlobs.gameName);
		Dependencies_Initialise(legoConfig, legoGlobs.gameName);
		Bubble_LoadBubbles(legoConfig);
		Encyclopedia_Initialise(legoConfig, legoGlobs.gameName);
		LegoObject_LoadObjTtsSFX(legoConfig, legoGlobs.gameName);
		Weapon_Initialise(legoConfig, legoGlobs.gameName);

		//Editor_OpenHelpTextFile(); // RockFall beta @004101a0, opens file "Information\\Help\\EditorHelp.txt"

		legoGlobs.viewMode = ViewMode_Top;
		
		Gods98::Sound3D_MakeContainerListener(legoGlobs.cameraMain->contListener);
		Gods98::Sound3D_SetMinDistForAtten(legoGlobs.MinDistFor3DSoundsOnTopView);

		const real32 mindist = Config_GetRealValue(legoConfig, Main_ID("mindist"));
		const real32 maxdist = Config_GetRealValue(legoConfig, Main_ID("maxdist"));

		const real32 mintilt = Gods98::Config_GetAngle(legoConfig, Main_ID("mintilt"));
		const real32 maxtilt = Gods98::Config_GetAngle(legoConfig, Main_ID("maxtilt"));

		Camera_SetZoomRange(legoGlobs.cameraMain, mindist, maxdist);
		Camera_SetTiltRange(legoGlobs.cameraMain, mintilt, maxtilt);

		legoGlobs.TVClipDist = Config_GetRealValue(legoConfig, Main_ID("tvclipdist"));

		Gods98::Viewport_SetBackClip(legoGlobs.viewMain, legoGlobs.TVClipDist);
		Gods98::Viewport_SetBackClip(legoGlobs.viewTrack, legoGlobs.TVClipDist);

		legoGlobs.DynamiteDamageRadius = Config_GetRealValue(legoConfig, Main_ID("DynamiteDamageRadius"));

		legoGlobs.DynamiteMaxDamage = Config_GetRealValue(legoConfig, Main_ID("DynamiteMaxDamage"));

		legoGlobs.DynamiteWakeRadius = Config_GetRealValue(legoConfig, Main_ID("DynamiteWakeRadius"));

		legoGlobs.BirdScarerRadius = Config_GetRealValue(legoConfig, Main_ID("BirdScarerRadius"));


		/// NOTE: This will be disabled again if Main::CDTracks property is 0 or missing.
		if (Config_GetBoolOrFalse(legoConfig, Main_ID("MusicOn"))) {
			legoGlobs.flags2 |= GAME2_MUSICREADY;
		}

		/// FIXME: Sanity dictates we should assign CDStartTrack to a default value AFTER
		///        parsing CDTracks. We don't even know if a 3rd track even exists yet!
		legoGlobs.CDStartTrack = Config_GetIntValue(legoConfig, Main_ID("CDStartTrack"));
		if (legoGlobs.CDStartTrack == 0)
			legoGlobs.CDStartTrack = 2;

		legoGlobs.CDTracks = Config_GetIntValue(legoConfig, Main_ID("CDTracks"));
		if (legoGlobs.CDTracks == 0) {
			// NOTE: 1 is probably a sanity value for when modulus/division is used for random tracks.
			//       Even if music is disabled, it's only disabled in a way that allows the GUI to turn it back on.
			legoGlobs.CDTracks = 1;
			legoGlobs.flags2 &= ~GAME2_MUSICREADY;
		}


		if (Config_GetBoolOrFalse(legoConfig, Main_ID("SoundOn"))) {
			legoGlobs.flags1 |= GAME1_USESFX;
		}

		legoGlobs.flags1 |= (GAME1_USEDETAIL|GAME1_DDRAWCLEAR|GAME1_RENDERPANELS|GAME1_DYNAMICPM);

		if (!Config_GetBoolOrTrue(legoConfig, Main_ID("Clear"))) {
			legoGlobs.flags1 &= ~GAME1_DDRAWCLEAR;
		}

		if (!Config_GetBoolOrTrue(legoConfig, Main_ID("Panels"))) {
			legoGlobs.flags1 &= ~GAME1_RENDERPANELS;
		}

		if (!Config_GetBoolOrTrue(legoConfig, Main_ID("WallProMeshes"))) {
			legoGlobs.flags1 &= ~GAME1_USEDETAIL;
		}

		if (!Config_GetBoolOrTrue(legoConfig, Main_ID("DynamicPM"))) {
			legoGlobs.flags1 &= ~GAME1_DYNAMICPM;
		}

		legoGlobs.HPBlocks = Config_GetIntValue(legoConfig, Main_ID("HPBlocks"));


		if (Config_GetBoolOrFalse(legoConfig, Main_ID("OnlyBuildOnPaths"))) {
			legoGlobs.flags1 |= GAME1_ONLYBUILDONPATHS;
		}

		if (Config_GetBoolOrFalse(legoConfig, Main_ID("AlwaysRockFall"))) {
			legoGlobs.flags1 |= GAME1_ALWAYSROCKFALL;
		}

		legoGlobs.MiniFigureRunAway = Config_GetRealValue(legoConfig, Main_ID("MiniFigureRunAway"));
		if (legoGlobs.MiniFigureRunAway == 0.0f)
			legoGlobs.MiniFigureRunAway = 10.0f;



		legoGlobs.DrainTime = Config_GetRealValue(legoConfig, Main_ID("DrainTime"));
		if (legoGlobs.DrainTime == 0.0f)
			legoGlobs.DrainTime = 10.0f;
		legoGlobs.DrainTime *= STANDARD_FRAMERATE;


		legoGlobs.ReinforceHits = Config_GetIntValue(legoConfig, Main_ID("ReinforceHits"));
		if (legoGlobs.ReinforceHits == 0)
			legoGlobs.ReinforceHits = 4;


		legoGlobs.MedPolyRange = Config_GetRealValue(legoConfig, Main_ID("MedPolyRange"));

		legoGlobs.HighPolyRange = Config_GetRealValue(legoConfig, Main_ID("HighPolyRange"));

		legoGlobs.MinEnergyForEat = Config_GetRealValue(legoConfig, Main_ID("MinEnergyForEat"));

		Lego_LoadMiscObjects(legoConfig);


		Pointer_Load(Gods98::Config_FindArray(legoConfig, Lego_ID("Pointers")));
		Pointer_SetCurrent_IfTimerFinished(Pointer_Standard);

		/// CHANGE: Move this handling to before Message_Update. We're handling keybinds differently now.
		///         And even before then, we want AllowDebugKeys to be toggleable at runtime.
		/*if (Lego_IsAllowDebugKeys()) {
			Message_RegisterHotKeyEvent(Keys::KEY_ONE, Message_FirstPerson, nullptr, Message_Argument(0), nullptr);
			Message_RegisterHotKeyEvent(Keys::KEY_TWO, Message_FirstPerson, nullptr, Message_Argument(1), nullptr);
			Message_RegisterHotKeyEvent(Keys::KEY_THREE, Message_TopView, nullptr, MESSAGE_ARGUMENT_NONE, nullptr);
			Message_RegisterHotKeyEvent(Keys::KEY_FOUR, Message_TrackObject, nullptr, MESSAGE_ARGUMENT_NONE, nullptr);
		}*/

		Smoke_LoadTextures("MiscAnims\\Smoke", "Smoke", 3);
		DamageFont_LoadFrames("Interface\\FONTS\\HealthFont", "a000_");


		Point2F Dialog_center = Point2F { 0.0f, 0.0f };

		///FIXME: FREE YOUR MEMORY AFTER USING Config_GetStringValue!!!
		char* dialogImage = Gods98::Config_GetStringValue(legoConfig, Lego_ID("Dialog", "Image"));
		if (dialogImage != nullptr) {
			legoGlobs.DialogImage = Gods98::Image_LoadBMP(dialogImage);
			if (legoGlobs.DialogImage != nullptr) {
				Gods98::Image_SetupTrans(legoGlobs.DialogImage, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

				Dialog_center.x = (real32)(Gods98::appWidth()  / 2) - (real32)(Gods98::Image_GetWidth(legoGlobs.DialogImage)  / 2);
				Dialog_center.y = (real32)(Gods98::appHeight() / 2) - (real32)(Gods98::Image_GetHeight(legoGlobs.DialogImage) / 2);
			}
		}

		///FIXME: FREE YOUR MEMORY AFTER USING Config_GetStringValue!!!
		char* dialogTitleWindow = Gods98::Config_GetStringValue(legoConfig, Lego_ID("Dialog", "TitleWindow"));
		if (dialogTitleWindow != nullptr) {
			const uint32 numParts = Gods98::Util_Tokenise(dialogTitleWindow, Window_stringParts, "|");
			if (numParts == 4) {

				const Area2F Window_rect = Area2F {
					(real32)std::atoi(Window_stringParts[0]) + Dialog_center.x,
					(real32)std::atoi(Window_stringParts[1]) + Dialog_center.y,
					(real32)std::atoi(Window_stringParts[2]),
					(real32)std::atoi(Window_stringParts[3]),
				};
				legoGlobs.DialogTextWndTitle = Gods98::TextWindow_Create(legoGlobs.fontBriefingHi, &Window_rect, 0x400);
			}
		}

		///FIXME: FREE YOUR MEMORY AFTER USING Config_GetStringValue!!!
		char* dialogTextWindow = Gods98::Config_GetStringValue(legoConfig, Lego_ID("Dialog", "TextWindow"));
		if (dialogTextWindow != nullptr) {
			const uint32 numParts = Gods98::Util_Tokenise(dialogTextWindow, Window_stringParts, "|");
			if (numParts == 4) {

				const Area2F Window_rect = Area2F {
					(real32)std::atoi(Window_stringParts[0]) + Dialog_center.x,
					(real32)std::atoi(Window_stringParts[1]) + Dialog_center.y,
					(real32)std::atoi(Window_stringParts[2]),
					(real32)std::atoi(Window_stringParts[3]),
				};
				legoGlobs.DialogTextWndMessage = Gods98::TextWindow_Create(legoGlobs.fontBriefingLo, &Window_rect, 0x400);
			}
		}

		///FIXME: FREE YOUR MEMORY AFTER USING Config_GetStringValue!!!
		char* dialogOkWindow = Gods98::Config_GetStringValue(legoConfig, Lego_ID("Dialog", "OkWindow"));
		if (dialogOkWindow != nullptr) {
			const uint32 numParts = Gods98::Util_Tokenise(dialogOkWindow, Window_stringParts, "|");
			if (numParts == 4) {

				const Area2F Window_rect = Area2F {
					(real32)std::atoi(Window_stringParts[0]) + Dialog_center.x,
					(real32)std::atoi(Window_stringParts[1]) + Dialog_center.y,
					(real32)std::atoi(Window_stringParts[2]),
					(real32)std::atoi(Window_stringParts[3]),
				};
				legoGlobs.DialogTextWndOK = Gods98::TextWindow_Create(legoGlobs.fontBriefingLo, &Window_rect, 0x400);
			}
		}

		///FIXME: FREE YOUR MEMORY AFTER USING Config_GetStringValue!!!
		char* dialogCancelWindow = Gods98::Config_GetStringValue(legoConfig, Lego_ID("Dialog", "CancelWindow"));
		if (dialogCancelWindow != nullptr) {
			const uint32 numParts = Gods98::Util_Tokenise(dialogCancelWindow, Window_stringParts, "|");
			if (numParts == 4) {

				const Area2F Window_rect = Area2F {
					(real32)std::atoi(Window_stringParts[0]) + Dialog_center.x,
					(real32)std::atoi(Window_stringParts[1]) + Dialog_center.y,
					(real32)std::atoi(Window_stringParts[2]),
					(real32)std::atoi(Window_stringParts[3]),
				};
				legoGlobs.DialogTextWndCancel = Gods98::TextWindow_Create(legoGlobs.fontBriefingLo, &Window_rect, 0x400);
			}
		}

		///FIXME: FREE YOUR MEMORY AFTER USING Config_GetStringValue!!!
		char* dialogContrastOverlay = Gods98::Config_GetStringValue(legoConfig, Lego_ID("Dialog", "ContrastOverlay"));
		if (dialogContrastOverlay != nullptr) {
			legoGlobs.DialogContrastOverlay = Gods98::Image_LoadBMP(dialogContrastOverlay);
			if (legoGlobs.DialogContrastOverlay != nullptr) {
				Gods98::Image_SetupTrans(legoGlobs.DialogContrastOverlay, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			}
		}

		legoGlobs.flags1 |= (GAME1_RADARON|GAME1_RADAR_TRACKOBJECTVIEW);

		//util::logf_removed((char*)legoGlobs.rootCont, "animtex\\dirt"); // Dust_Setup

		legoGlobs.radarZoom = 15.0f;
		legoGlobs.radarScreenRect = Area2F { 16.0f, 13.0f, 151.0f, 151.0f };

		Effect_Load_RockFallStylesAll(legoConfig, legoGlobs.gameName, legoGlobs.rootCont);

		Lego_LoadToolTips(legoConfig);
		Object_LoadToolTipIcons(legoConfig);
		Lego_LoadUpgradeNames(legoConfig);
		Lego_LoadInfoMessages(legoConfig);
		Info_SetOverFlowImageFile("Interface\\MessageTabs\\infoOverFlow.bmp");
		Lego_LoadTextMessages(legoConfig);


		Lego_LoadPanels(legoConfig, Gods98::appWidth(), Gods98::appHeight());

		Panel_Crystals_Initialise("Interface\\RightPanel\\SmallCrystal.bmp", "Interface\\RightPanel\\UsedCrystal.bmp", "Interface\\RightPanel\\NoSmallCrystal.bmp");

		Panel_AirMeter_Initialise("Interface\\AirMeter\\msgpanel_air_juice.bmp", 85, 6, 236, "Interface\\AirMeter\\msgpanel_noair.bmp", 21, 0);

		Panel_CryOreSideBar_Initialise("Interface\\RightPanel\\crystalsidebar_ore.bmp", 615, 434, 423);


		Lego_LoadPanelButtons(legoConfig, Gods98::appWidth(), Gods98::appHeight());
		Lego_LoadTutorialIcon(legoConfig);

		Interface_InitBuildItems();
		Interface_LoadMenuItems(legoConfig, legoGlobs.gameName);
		Interface_LoadItemPanels(legoConfig, legoGlobs.gameName);
		Interface_LoadPlusMinusImages("Interface\\Dependencies\\+.bmp", "Interface\\Dependencies\\=.bmp");

		Reward_LoadGraphics(legoConfig, legoGlobs.gameName);
		Reward_Initialise();

		Panel_LoadInterfaceButtons_ScrollInfo();
		Panel_Button_Hide(Panel_Radar, PanelButton_Radar_ZoomIn, true);
		Panel_Button_Hide(Panel_Radar, PanelButton_Radar_ZoomOut, true);
		

		Front_ResetSaveNumber();

		if (Front_IsFrontEndEnabled()) {
			Front_Initialise(legoConfig);
		}

		Advisor_Initialise(legoGlobs.gameName, legoGlobs.cameraMain, legoGlobs.viewMain);
		Advisor_InitViewport(200.0f);
		Advisor_LoadAnims(legoConfig, legoGlobs.gameName);

		if (Config_GetBoolOrFalse(legoConfig, Main_ID("DisplayAdvisor"))) {
			Advisor_LoadPositions(legoConfig, legoGlobs.gameName, Gods98::appWidth(), Gods98::appHeight());
		}

		
		if (legoGlobs.flags1 & GAME1_USESFX) {
			Lego_SetSoundOn(true);
		}


		/// CUSTOM: Track completion of Lego_Initialise, so that we know what modifications can be made to global variables.
		legoGlobs2.legoInit = true;

		const uint32 loadTime = Gods98::Main_GetTime() - loadStartTime;
		Error_InfoF("Loading took %.03f seconds", (double)loadTime/1000.0);

		const char* startLevelName;
		/// ALTERATION: allow -startlevel command line option to ALWAYS boot directly into a level.
		// Programmer modes >= 3 also disable the front end.
		if (Gods98::Main_GetStartLevel() == nullptr && Gods98::Main_ProgrammerMode() < 3 &&
			Config_GetBoolOrFalse(legoConfig, Main_ID("FrontEnd")))
		{
			Loader_display_loading_bar(nullptr);
			Front_RunScreenMenuType(Menu_Screen_Title);

			if (Front_IsTriggerAppQuit())
				return false;

			startLevelName = Front_GetSelectedLevel();
		}
		else {
			startLevelName = Gods98::Main_GetStartLevel();
			if (startLevelName == nullptr) {
				startLevelName = Gods98::Config_GetTempStringValue(legoConfig, Main_ID("startlevel"));
			}
		}
		Front_LoadOptionParameters(true, false);

		// Override Lego_LoadLevel to store SeeThroughWalls property.
		if (Lego_LoadLevel2(startLevelName)) {
			SFX_Random_PlaySoundNormal(SFX_AmbientLoop, true);
			return true;
		}
	}

	Gods98::Config_Free(legoConfig);
//LAB_0042274b:
	Gods98::Container_Shutdown();
	return false;
}


// <LegoRR.exe @00423210>
bool32 __cdecl LegoRR::Lego_MainLoop(real32 elapsed)
{
	bool32 keyDownT, keyDownR, keyDownAddSelection;

	// Programmer mode 10 operates by booting up each level in the links one-by-one,
	// Once started, a level immediately ends, then jumps right into the next level.
	// 
	// This is used to populate `LoaderProfile*.txt` by manually going through and loading
	// "every" asset in the game. The combined file sizes of these loaded assets are then
	// used to calculate the progress bar for loading levels and the main game.
	// 
	// Lastly, after all levels are loaded, the game performs a FULL shutdown, and writes
	// the `LoaderProfile*.txt` data.
	//
	// This *also* functions as a sloppy unit test to ensure all levels load without crashing. :)
	if (Gods98::Main_ProgrammerMode() == 10 && !Lego_EndLevel()) { // No next level after this.
		// ProgrammerMode 10 has finished cycling through every level.
		// Now shutdown the game, and write the load sizes to `LoaderProfile*.txt`.
		return false;
	}


	// During the first tick of a level, the game should update at NORMAL SPEED (1 standard frame),
	// as there's no "previous" tick to compare it against. Otherwise you'll usually get 300% speed
	// for the first tick (which is the capped max elapsed time in WinMain loop).
	const bool32 firstTick = (legoGlobs.flags1 & GAME1_LEVELSTART);
	legoGlobs.flags1 &= ~GAME1_LEVELSTART; // Clear this flag (which is set again only during `Lego_LoadLevel`).


	/// FIX APPLY: Change first-tick rules to apply to ALL elapsed time values: (Real, Interface, World).
	///            Originally only world "game" time would be set to 1.0f for the first tick (with two exceptions).
	// TIME PASSED: Front end animations like pointers, tooltips, SFX, menu logic, etc.
	const real32 elapsedReal = (!firstTick ? elapsed : 1.0f);

	// NOTE: Yes... the flag `GAME1_CAMERADISABLED` has been in dire need of renaming for months. It's wrong.
	// Interface and world elapsed times are set to zero.
	const bool32 timeStopped = (legoGlobs.flags1 & GAME1_FREEZEINTERFACE);
	// TIME PASSED: Interface behaviours that stop when paused like advisor, world effects like damage text, etc.
	// ~~This *reeaaallllly* should follow the same first-tick timing used by world logic, just a minor QoL thing.~~
	// WARNING: 0.0f time messes with lots of things that use division, these will need to be hunted down and fixed.
	const real32 elapsedInterface = (!timeStopped ? elapsedReal : 0.0f);

	// In first person view, game speed is locked at 100%.
	//  (...unless programmer mode >= 2, in which case... speedy legoman go zooooooooom).
	const bool32 fpForceGameSpeed = ((legoGlobs.viewMode == ViewMode_FP) && (Gods98::Main_ProgrammerMode() < 2));
	// TIME PASSED: In-game in the world. This *SHOULD* be used for all game logic and world animations.
	const real32 elapsedWorld = (!fpForceGameSpeed ? (elapsedInterface * Lego_GetGameSpeed()) : (elapsedInterface * 1.0f));


	// Only ever used by `NERPsRuntime_EndExecute`.
	legoGlobs.elapsedAbs = elapsedInterface;


	/// NOTE: Update handles ShortcutID::ReloadKeyBinds.
	Shortcuts::shortcutManager.Update(elapsed, true);

	Lego_HandleRenameInput();



	/// FIX APPLY: These updates did not account for elapsed world time being exactly 1 frame on the first tick of a level.
	Teleporter_Update(elapsedWorld);
	Lego_UnkTeleporterInit_FUN_00435980();


	/// FIX ME: OH MY GOD, MONSTER EMERGE TRIGGERS ARE BASED ON INTERFACE TIME PASSED, AND NOT WORLD TIME!!
	Level_Emerge_FUN_0042c370(Lego_GetLevel(), elapsedInterface);


	/// FIX APPLY: These updates did not account for elapsed world time being exactly 1 frame on the first tick of a level.
	Lego_UpdateFallins(elapsedWorld);
	Lego_UpdateSlug_FUN_004260f0(elapsedWorld);


	/// GFX LOGIC: "Action Stations" red flashing ambient light.
	if (!(legoGlobs.flags2 & GAME2_CALLTOARMS)) {
		if (legoGlobs.cameraFP != nullptr && (legoGlobs.cameraFP)->contCam != nullptr && legoGlobs.dirLightCallToArms != nullptr) {
			Gods98::Container_Hide(legoGlobs.dirLightCallToArms, true);
		}
	}
	else if (legoGlobs.cameraFP != nullptr && (legoGlobs.cameraFP)->contCam != nullptr &&
			 legoGlobs.dirLightCallToArms != nullptr && !(legoGlobs.flags1 & GAME1_FREEZEINTERFACE))
	{
		Gods98::Container_Hide(legoGlobs.dirLightCallToArms, false);
		Gods98::Container_AddRotation(legoGlobs.dirLightCallToArms, Gods98::Container_Combine::After, 0.0f, 1.0f, 0.0f, elapsedInterface * 0.1f);
	}

	if (!(legoGlobs.flags1 & GAME1_FREEZEINTERFACE) && ((legoGlobs.flags1 & GAME1_CAMERAGOTO) || legoGlobs.gotoSmooth)) {
		// Slowly move camera towards gotoTargetPos
		if (legoGlobs.gotoSmooth) {

			/// FIXME: DIVIDE BY ZERO
			const real32 smoothSpeed = (1.0f / elapsedInterface) * 5.0f; // (aka 5/6)

			Vector3F camPos;
			Gods98::Container_GetPosition((legoGlobs.cameraMain)->cont3, nullptr, &camPos);


			// MATH EXPLANATION: travel 1/6th distance towards targetPos every full tick.
			// This is an optimized calculation that doesn't require direction/length math.

			// ((camPos * speed*5.0f) + (legoGlobs.gotoTargetPos * 1.0f)) / (speed*5.0f + 1.0f)
			// 
			// Multiply current camera position by smoothing speed.   (aka 5/6)
			// Add target camera position, implicit multiply by 1.0f. (aka 1/6)
			// Then divide by smoothing speed + 1.0f.                 (aka 6/6)
			// 
			// Consider if smoothSpeed was multiplied by 2.0f instead of 5.0f.
			// In this case, then we would travel 1/2 distance towards targetPos every full tick.
			// Basically we would be calculating the midpoint `(A + B) / 2`, then.

			// smoothSpeed * camPos
			Gods98::Maths_Vector2DScale(&legoGlobs.gotoNewPos, reinterpret_cast<Point2F*>(&camPos), smoothSpeed);
			// (... + legoGlobs.gotoNewPos)
			Gods98::Maths_Vector2DAdd(&legoGlobs.gotoNewPos, &legoGlobs.gotoNewPos, &legoGlobs.gotoTargetPos);
			// ... / (smoothSpeed + 1.0f)
			Gods98::Maths_Vector2DScale(&legoGlobs.gotoNewPos, &legoGlobs.gotoNewPos, 1.0f / (smoothSpeed + 1.0f));

			// Original vector math code:
			//legoGlobs.gotoNewPos.x = (smoothSpeed * camPos.x + legoGlobs.gotoTargetPos.x) / (smoothSpeed + 1.0f);
			//legoGlobs.gotoNewPos.y = (smoothSpeed * camPos.y + legoGlobs.gotoTargetPos.y) / (smoothSpeed + 1.0f);


			/// FIXME: Smoothing stop does not take into account elapsed time, or world BlockSize!!

			// Smoothing stops when the distance traveled is less than 2.0f, which is affected by update speed.
			// Intended effect (probably): "if distance traveled is less than 2.0f, then we're slowing down enough to stop"
			// It's also possible this was intended to compare against targetPos, rather than camPos
			const real32 gotoDistance = Gods98::Maths_Vector2DDistance(&legoGlobs.gotoNewPos, reinterpret_cast<Point2F*>(&camPos));

			if (gotoDistance < 2.0f) {
				// We're close enough to treat as having reached gotoTargetPos.
				legoGlobs.gotoSmooth = false;
			}
		}

		Camera_SetTopdownPosition(legoGlobs.cameraMain, legoGlobs.gotoNewPos.x, legoGlobs.gotoNewPos.y);
		legoGlobs.flags1 &= ~GAME1_CAMERAGOTO;
	}

	/// REFACTOR: Level first-tick logic moved above camera functions here.
	/*if (legoGlobs.flags1 & GAME1_LEVELSTART) {
		// First tick of a level should always be one standard frame.
		elapsedWorld = 1.0f;
		legoGlobs.flags1 &= ~GAME1_LEVELSTART;
		elapsedWorld = 1.0f;
	}*/


	/// BETA: RockFall. Restore FPS and Memory monitors.
	static uint32 sMonitorFramesCountUp = 0;
	static uint32 sFpsTimeGet = 0;

	/// FIX APPLY: Respect the firstTick flag, rather than rely on yet another static variable.
	if (firstTick) {
		sMonitorFramesCountUp = 0;
		sFpsTimeGet = Gods98::Main_GetTime();
	}
	if (++sMonitorFramesCountUp >= 200) {
		const real32 timeDiff = (real32)(Gods98::Main_GetTime() - sFpsTimeGet);
		if (legoGlobs.flags1 & GAME1_SHOWFPS) {
			Gods98::TextWindow_PrintF(legoGlobs.textOnlyWindow, "\nFPS: %f", (double)(1.0f / ((timeDiff / 1000.0f) / 200.0f)));
		}

		if (legoGlobs.flags1 & GAME1_SHOWMEMORY) {
			uint32 totalMem, freeMem;
			if (Gods98::DirectDraw_GetAvailTextureMem(&totalMem, &freeMem)) {
				Gods98::TextWindow_PrintF(legoGlobs.textOnlyWindow, "\nTexture mem: %i avail: %i used: %i", totalMem, freeMem, totalMem - freeMem);
			}
		}

		sMonitorFramesCountUp = 0;
		sFpsTimeGet = Gods98::Main_GetTime();
	}


	// Output keyDown* parameters are later used by `Lego_unkGameLoop_FUN_00426450`.
	bool32 dontExit = Lego_HandleKeys(elapsedWorld, elapsedInterface, &keyDownT, &keyDownR, &keyDownAddSelection);
	if (!dontExit)
		return false;


	if (!(legoGlobs.flags1 & GAME1_FREEZEINTERFACE)) {
		Lego_HandleRadarInput();
	}


	AITask_UpdateAll(elapsedWorld);
	Level_UpdateEffects(legoGlobs.currLevel, elapsedWorld);

	DamageFont_UpdateAll(elapsedInterface);

	LegoObject_UpdateAll(elapsedWorld);
	Weapon_Update(elapsedWorld);
	Erode_Update(elapsedWorld);
	Level_BlockActivity_UpdateAll(legoGlobs.currLevel, elapsedWorld);

	/// CHANGE: Moved from Lego_Initialise so that these can be configured.
	/// CHANGE: These are no longer debug keys.
	if (Lego_IsAllowDebugKeys() || !NERPs_AnyTutorialFlags()) {
		/// KEYBIND: [1]  "Changes to first-person Eye view for selected unit."
		if (Shortcut_IsPressed(ShortcutID::ChangeViewFP1)) {
			Message_PostEvent(Message_FirstPerson, nullptr, Message_Argument(0), nullptr);
		}
		/// KEYBIND: [2]  "Changes to first-person Shoulder view for selected unit."
		if (Shortcut_IsPressed(ShortcutID::ChangeViewFP2)) {
			Message_PostEvent(Message_FirstPerson, nullptr, Message_Argument(1), nullptr);
		}
		/// KEYBIND: [3]  "Changes to top-down view."
		if (Shortcut_IsPressed(ShortcutID::ChangeViewTop)) {
			Message_PostEvent(Message_TopView, nullptr, MESSAGE_ARGUMENT_NONE, nullptr);
		}
		/// KEYBIND: [4]  "Tracks the selected unit in the radar."
		if (Shortcut_IsPressed(ShortcutID::TrackUnit)) {
			Message_PostEvent(Message_TrackObject, nullptr, MESSAGE_ARGUMENT_NONE, nullptr);
		}
	}
	/// DEBUG KEYBIND: NULL  "Deletes all selected units."
	if (Lego_IsAllowDebugKeys() && Lego_IsAllowEditMode() && Shortcut_IsPressed(ShortcutID::Edit_DestroyUnits)) {
		Message_PostEvent(Message_Debug_DestroyAll, nullptr, MESSAGE_ARGUMENT_NONE, nullptr);
	}

	Message_Update();

	Camera_Update(legoGlobs.cameraMain, legoGlobs.currLevel, elapsedInterface, elapsedWorld);
	Camera_Update(legoGlobs.cameraTrack, legoGlobs.currLevel, elapsedInterface, elapsedWorld);
	Camera_Update(legoGlobs.cameraFP, legoGlobs.currLevel, elapsedInterface, elapsedWorld);

	Construction_UpdateAll(elapsedWorld);

	Interface_FUN_0041b940(elapsedReal);

	Lego_UnkUpdateMapsWorldUnk_FUN_004290d0(elapsedInterface, false);

	LegoObject_HideAllCertainObjects();
	ElectricFence_UpdateAll(elapsedWorld);


	// Input keyDown* parameters are obtained from `Lego_HandleKeys`.
	// Handle input/interactions between world and UI.
	Lego_HandleWorld(elapsedReal, elapsedInterface, keyDownT, keyDownR, keyDownAddSelection);


	Map3D_Update((legoGlobs.currLevel)->map, elapsedWorld);
	Water_Update(legoGlobs.currLevel, elapsedWorld);

	Gods98::Sound3D_UpdateListener();

	Smoke_Update(elapsedWorld);
	SpiderWeb_Update(elapsedWorld);
	LightEffects_Update(elapsedWorld);

	Info_FUN_0041a1f0(elapsedReal);

	if (legoGlobs.IsFallinsEnabled) {
		Fallin_Update(elapsedWorld);
	}

	// IsKeyPressed(KEY_F6) (64)
	//  "Toggles fallin mode."
	if (Lego_IsAllowDebugKeys() && Shortcut_IsPressed(ShortcutID::Debug_ToggleFallins)) {
		legoGlobs.IsFallinsEnabled = !legoGlobs.IsFallinsEnabled;

		const char* fallinMode = (legoGlobs.IsFallinsEnabled ? "On" : "Off");
		Gods98::TextWindow_PrintF(legoGlobs.textOnlyWindow, "\nFallin Mode %s", fallinMode);
	}


	// GFX LOGIC: Render main viewport.
	// PREPARE: ambientLight is the only container shared by FP and Topdown rendering.
	Gods98::Container_Hide(legoGlobs.ambientLight, false);

	/// CUSTOM: Render routes in top-down / FP view.
	Debug_RouteVisual_UpdateAll(elapsedWorld, elapsedInterface);

	if (firstTick) {
		// Ensure fog settings are configured for the current view mode.
		Lego_SetSceneFogParams(Lego_GetViewMode());
	}

	if (legoGlobs.viewMode == ViewMode_Top) {
		// Dummy for passing to LegoObject_FP_SetRanges until we have a function that can go without it.
		LegoObject dummyFPObj;
		std::memset(&dummyFPObj, 0, sizeof(LegoObject));

		//Roof_Hide(false);
		//Roof_Update();

		/// CUSTOM: Support LOD in topdown view.
		if (Lego_GetTopdownLOD() != LOD_LowPoly) {
			/// NOTE: We need to use some arbitrary number to increase the ranges by so that they work well in topdown.
			///       This makes assumptions based on the default LOD values defined in Lego.cfg.
			// Medium LODs don't look great on Mini-Figures at any topdown distance, so only use Medium LOD when requested.
			const real32 medRange  = (Lego_GetTopdownLOD() == LOD_MediumPoly ? legoGlobs.MedPolyRange  : 0.0f) *  6.0f; // ~15 blocks
			const real32 highRange = (Lego_GetTopdownLOD() == LOD_HighPoly   ? legoGlobs.HighPolyRange : 0.0f) * 10.0f; // ~15 blocks
			LegoObject_FP_SetRanges(&dummyFPObj, legoGlobs.cameraMain->contCam,
									medRange, highRange, true);
		}
			
		/// CUSTOM: Render fog used in FP view when in topdown view.
		if (Lego_IsTopdownFogOn()) {
			Lego_UpdateSceneFog(true, elapsedWorld); // Turn ON fog for rendering.
		}


		// PREPARE: And unhide everything for rendering.
		if (!(legoGlobs.flags1 & GAME1_FREEZEINTERFACE)) {
			Gods98::Container_Hide(legoGlobs.rootSpotlight, false);
		}
		Gods98::Container_Hide(legoGlobs.spotlightTop, false);

		// RENDER: Everything to the viewport.
		Gods98::Viewport_Clear(legoGlobs.viewMain, true);
		Gods98::Viewport_Render(legoGlobs.viewMain, legoGlobs.rootCont, elapsedInterface);

		// CLEANUP: And hide everything again.
		Gods98::Container_Hide(legoGlobs.spotlightTop, true);
		Gods98::Container_Hide(legoGlobs.rootSpotlight, true);


		/// CUSTOM: Render fog used in FP view when in topdown view.
		if (Lego_IsTopdownFogOn()) {
			Lego_UpdateSceneFog(false, elapsedWorld); // Turn OFF fog for rendering.
		}

		if (Lego_GetTopdownLOD() != LOD_LowPoly) {
			LegoObject_FP_SetRanges(&dummyFPObj, nullptr, 0.0f, 0.0f, false);
		}

		//Roof_HideAllVisibleBlocks();
		//Roof_Hide(true);

		//Level_RemoveAll_ProMeshes();
	}
	else if (legoGlobs.viewMode == ViewMode_FP) {
		// PREPARE: And unhide everything for rendering.
		Roof_Hide(false);
		Roof_Update();

		LegoObject_FP_SetRanges(legoGlobs.objectFP, (legoGlobs.cameraFP)->contCam, legoGlobs.MedPolyRange, legoGlobs.HighPolyRange, true);
		LegoObject_SwapPolyFP(legoGlobs.objectFP, Camera_GetFPCameraFrame(legoGlobs.cameraFP), true);

		Lego_UpdateSceneFog(true, elapsedWorld); // Turn ON fog for rendering.

		if (!(legoGlobs.flags1 & GAME1_FREEZEINTERFACE)) {
			Gods98::Container_Hide(legoGlobs.pointLightFP, false);
			if (legoGlobs.dirLightFP != nullptr) {
				/// ALTERATION: Change FPRotLightRGB property to something actually useful, a forward-facing flashlight.
				//Vector3F fpPos = { 0.0f }, fpDir = { 0.0f };
				//LegoObject_FP_GetPositionAndHeading(legoGlobs.objectFP, Camera_GetFPCameraFrame(legoGlobs.cameraFP), &fpPos, &fpDir);
				//Gods98::Container_SetPosition(legoGlobs.dirLightFP, nullptr, fpPos.x, fpPos.y, fpPos.z);
				//fpDir.z = 0.0f; // Should we not change Z direction for head bobbing and other movement?
				//Gods98::Container_SetOrientation(legoGlobs.dirLightFP, nullptr, fpDir.x, fpDir.y, fpDir.z, 0.0f, 0.0f, -1.0f);

				/// OLD CODE:
				Gods98::Container_AddRotation(legoGlobs.dirLightFP, Gods98::Container_Combine::After, 0.0f, 1.0f, 0.0f, elapsedInterface * 0.01f);

				Gods98::Container_Hide(legoGlobs.dirLightFP, false);
			}
		}

		// RENDER: Everything to the viewport.
		Gods98::Viewport_Clear(legoGlobs.viewMain, true);
		Gods98::Viewport_Render(legoGlobs.viewMain, legoGlobs.rootCont, elapsedInterface);


		// CLEANUP: And hide everything again.
		Gods98::Container_Hide(legoGlobs.pointLightFP, true);
		if (legoGlobs.dirLightFP != nullptr) {
			Gods98::Container_Hide(legoGlobs.dirLightFP, true);
		}

		Lego_UpdateSceneFog(false, elapsedWorld); // Turn OFF fog for rendering.

		LegoObject_SwapPolyFP(legoGlobs.objectFP, Camera_GetFPCameraFrame(legoGlobs.cameraFP), false);
		LegoObject_FP_SetRanges(legoGlobs.objectFP, nullptr, 0.0f, 0.0f, false);

		Roof_HideAllVisibleBlocks();
		Roof_Hide(true);

		Level_RemoveAll_ProMeshes();
	}
	// CLEANUP: ambientLight is the only container shared by FP and Topdown rendering.
	Gods98::Container_Hide(legoGlobs.ambientLight, true);


	if (Panel_IsFullyOpen(Panel_Radar))
		legoGlobs.flags1 |= GAME1_RADARON;
	else
		legoGlobs.flags1 &= ~GAME1_RADARON;


	if (Message_AnyUnitSelected()) {
		// Only lock if we know we're going to be drawing selection boxes,
		//  this doesn't account for tracker, but no big deal.
		Lego_BeginDrawSelectionBoxes(); // Start of only Draw calls for selection boxes.
	}

	if (legoGlobs.viewMode == ViewMode_Top) {
		Lego_DrawAllSelectedUnitBoxes(legoGlobs.viewMain);
		Lego_DrawAllLaserTrackerBoxes(legoGlobs.viewMain);
		Encyclopedia_DrawSelectBox(legoGlobs.viewMain);
	}
	Lego_DrawDragSelectionBox(legoGlobs.currLevel);

	if (Message_AnyUnitSelected()) {
		Lego_EndDrawSelectionBoxes(); // End of only Draw calls for selection boxes.
	}

	if (legoGlobs.viewMode == ViewMode_Top) {
		Lego_DrawAllSelectedUnitNames(legoGlobs.viewMain);
		Lego_DrawAllLaserTrackerNames(legoGlobs.viewMain);
		Encyclopedia_DrawSelectName(legoGlobs.viewMain);
	}



	if (!(legoGlobs.flags1 & GAME1_FREEZEINTERFACE)) {
		Bubble_DrawAllObjInfos(elapsedInterface);
	}

	// GFX LOGIC: Render radar tracker viewport.
	if ((legoGlobs.flags1 & GAME1_RADARON) && (legoGlobs.flags1 & GAME1_RADAR_TRACKOBJECTVIEW)) {
		Gods98::Viewport_Clear(legoGlobs.viewTrack, true);
		if (!(legoGlobs.flags1 & GAME1_RADAR_NOTRACKOBJECT)) {

			Lego_UnkUpdateMapsWorldUnk_FUN_004290d0(elapsedWorld, true);
			LegoObject_HideAllCertainObjects();
			Gods98::Container_Hide(legoGlobs.spotlightTrack, false);
			if (legoGlobs.cameraFP != nullptr && (legoGlobs.cameraFP)->contCam != nullptr && legoGlobs.dirLightCallToArms != nullptr) {
				Gods98::Container_Hide(legoGlobs.dirLightCallToArms, true);
			}

			/// CUSTOM: Don't render debug visuals in Radar viewport.
			Debug_RouteVisual_HideAll(true);

			Gods98::Viewport_Render(legoGlobs.viewTrack, legoGlobs.rootCont, elapsedInterface);
			Gods98::Container_Hide(legoGlobs.spotlightTrack, true);
			Map3D_HideVisibleBlocksList((legoGlobs.currLevel)->map);
		}
	}

	// ALL RENDERING HAS FINISHED
	Gods98::Graphics_Finalise3D();
	Smoke_HideAll(true);

	SFX_Update(elapsedReal);

	if (legoGlobs.viewMode == ViewMode_Top && !(legoGlobs.flags1 & GAME1_FREEZEINTERFACE)) {
		Level_UpdateTutorialBlockFlashing(legoGlobs.currLevel, legoGlobs.viewMain, elapsedWorld, elapsedInterface);
		NERPsRuntime_DrawTutorialIcon(legoGlobs.currLevel, legoGlobs.viewMain, elapsedWorld, elapsedInterface);
	}

	Map3D_HideVisibleBlocksList((legoGlobs.currLevel)->map);



	const bool32 inRadarMapView = ((legoGlobs.flags1 & GAME1_RADARON) && (legoGlobs.flags1 & GAME1_RADAR_MAPVIEW));

	if (inRadarMapView) {
		RadarMap_BeginDraw(Lego_GetRadarMap()); // Start of only Draw calls for radar map.

		Lego_DrawRadarMap();
	}
	LegoObject_UpdateAllRadarSurvey(elapsedWorld, inRadarMapView);
	
	if (inRadarMapView) {
		RadarMap_EndDraw(Lego_GetRadarMap()); // End of only Draw calls for radar map.
	}



	if (legoGlobs.flags1 & GAME1_RENDERPANELS) {
		// Yes, this is really checking if NOT on...
		if (!(legoGlobs.flags1 & GAME1_RADARON)) {
			Panel_Draw(Panel_RadarFill, elapsedInterface);
		}

		Panel_Draw(Panel_Radar, elapsedInterface);

		if ((legoGlobs.flags1 & GAME1_RADARON) && (legoGlobs.flags1 & GAME1_RADAR_TRACKOBJECTVIEW) &&
			!(legoGlobs.flags1 & GAME1_RADAR_NOTRACKOBJECT))
		{
			Panel_Draw(Panel_RadarOverlay, elapsedInterface);
		}

		Panel_Draw(Panel_MessagesSide, elapsedInterface);
		Panel_Draw(Panel_CameraControl, elapsedInterface);
		Panel_Draw(Panel_TopPanel, elapsedInterface);
		Panel_Draw(Panel_PriorityList, elapsedInterface);
		Priorities_Draw();
		Panel_Draw(Panel_Encyclopedia, elapsedInterface);
		Encyclopedia_Update(elapsedInterface);
		if (!(legoGlobs.flags2 & GAME2_INMENU)) {
			Interface_FUN_0041b3c0();
		}
		// Somewhere in here causes a crash if an unsupported object type is selected with certain interface menu types.
		Interface_FUN_0041b860(elapsedInterface);


		Panel_Draw(Panel_CrystalSideBar, elapsedInterface);
		/// JANK: Crystals panel drawing still happening outside of level,
		///       and relying on values only assigned when in-level.
		/// JANK: Why are these being stored as floats???
		uint32 crystalsTotal = 0;// (uint32)elapsedInterface;
		uint32 crystalsFree = 0;// (uint32)elapsedInterface;
		if (legoGlobs.currLevel != nullptr) {
			crystalsTotal = legoGlobs.currLevel->crystals;
			crystalsFree = (legoGlobs.currLevel->crystals - legoGlobs.currLevel->crystalsDrained);
		}
		Panel_Crystals_Draw(crystalsTotal, crystalsFree, elapsedWorld);

		Panel_CryOreSideBar_Draw();
		/// HARDCODED STUD ORE COUNT
		const uint32 oreTotal = (legoGlobs.currLevel->ore + (legoGlobs.currLevel->studs * 5));
		Panel_PrintF(Panel_CrystalSideBar, legoGlobs.fontToolTip, 16, 469, true, "%i", oreTotal);

		Panel_Draw(Panel_Information, elapsedInterface);
		ScrollInfo_Update(false);
		Info_DrawPanel(elapsedInterface);
		Panel_Draw(Panel_InfoDock, elapsedInterface);
		Info_Draw(elapsedInterface);
	}
	HelpWindow_FUN_00418930();

	const real32 advisorElapsed = (Objective_IsShowing() ? elapsedReal : elapsedInterface);
	Advisor_Update(advisorElapsed);

	Gods98::Sound_Update(Lego_IsMusicPlaying());


	/// GAME/DEBUG LOGIC: NERPs script
	if (!(legoGlobs.flags1 & GAME1_DEBUG_NONERPS)) {
		NERPsRuntime_Execute(elapsedInterface);
	}

	if (!NERPs_AnyTutorialFlags()) {
		if (Lego_GetFollowUnit() != nullptr) {
			Lego_Goto(Lego_GetFollowUnit(), nullptr, true);
		}
	}


	Objective_Update(legoGlobs.textOnlyWindow, legoGlobs.currLevel, elapsedWorld, elapsedReal);

	if (Gods98::Main_ProgrammerMode() < 2) {
		if ((legoGlobs.currLevel)->MaxStolen != 0.0f &&
			(uint32)(legoGlobs.currLevel)->MaxStolen <= (uint32)(legoGlobs.currLevel)->crystalsStolen)
		{
			Objective_SetStatus(LevelStatus::LEVELSTATUS_FAILED_CRYSTALS);
		}

		const uint32 pathsPresent = LegoObject_GetLevelObjectsBuilt(LegoObject_Path, (LegoObject_ID)0, 0, true);

		if (pathsPresent == 0) {
			uint32 pilotsPresent = 0;

			const uint32 pilotLevels = Stats_GetLevels(LegoObject_MiniFigure, LegoObject_ID_Pilot);
			for (uint32 lvl = 0; lvl < pilotLevels; lvl++) {
				pilotsPresent += LegoObject_GetLevelObjectsBuilt(LegoObject_MiniFigure, LegoObject_ID_Pilot, lvl, true);
			}
			if (pilotsPresent == 0 && Lego_GetLevel()->status == LevelStatus::LEVELSTATUS_INCOMPLETE) {
				// No power paths or MiniFigures, it's impossible to win since a ToolStore can no longer be constructed.
				Objective_SetStatus(LevelStatus::LEVELSTATUS_FAILED);
			}
		}
	}

	Text_Update(elapsedInterface);
	if ((legoGlobs.flags2 & GAME2_MENU_HASNEXT) && legoGlobs.NextButtonImage != nullptr) {
		Gods98::Image_DisplayScaled(legoGlobs.NextButtonImage, nullptr, &legoGlobs.menuNextPoint, nullptr);
	}
	if ((legoGlobs.flags2 & GAME2_MENU_HASPREVIOUS) && legoGlobs.BackArrowImage != nullptr) {
		Gods98::Image_DisplayScaled(legoGlobs.BackArrowImage, nullptr, &legoGlobs.menuPrevPoint, nullptr);
	}

	if (((legoGlobs.flags2 & GAME2_MESSAGE_HASNEXT) && legoGlobs.NextButtonImage != nullptr) &&
		((!(legoGlobs.flags2 & GAME2_CAMERAMOVING) || Interface_GetFlashingState())))
	{
		Gods98::Image_DisplayScaled(legoGlobs.NextButtonImage, nullptr, &legoGlobs.NextButtonPos, nullptr);
	}
	if ((legoGlobs.flags2 & GAME2_MESSAGE_HASREPEAT) && legoGlobs.RepeatButtonImage != nullptr) {
		Gods98::Image_DisplayScaled(legoGlobs.RepeatButtonImage, nullptr, &legoGlobs.RepeatButtonPos, nullptr);
	}
	if ((legoGlobs.flags2 & GAME2_INOPTIONSMENU) && Front_IsFrontEndEnabled()) {

		if (Front_Options_Update(elapsedInterface, Menu_Modal_Options)) {
			legoGlobs.flags2 &= ~GAME2_INOPTIONSMENU;

			if (!Objective_IsShowing()) {
				Lego_SetPaused(false, false);
			}
			else {
				legoGlobs.flags1 |= GAME1_PAUSED;
			}
		}
	}

	Lego_DrawRenameInput(elapsedReal);

	if (!(legoGlobs.flags1 & GAME1_PAUSED)) {
		Pointer_Update(elapsedReal);
		if (legoGlobs.renameInput == nullptr) {
			//goto LAB_00423e04;
			Pointer_DrawPointer(Gods98::msx(), Gods98::msy());
		}
	}
	else {
		if (!Front_IsFrontEndEnabled() || !Front_Options_Update(elapsedInterface, Menu_Modal_Paused)) {
			Pointer_Update(elapsedReal);
		//LAB_00423e04:
			Pointer_DrawPointer(Gods98::msx(), Gods98::msy());
		}
		else {
			legoGlobs.flags1 ^= GAME1_PAUSED; // toggle flag
			if (!Front_IsTriggerMissionQuit()) {

				if (!Objective_IsShowing()) {
					Lego_SetPaused(false, false);
				}
			}
			else {
				Lego_QuitLevel();
				Lego_GetLevel()->status = LevelStatus::LEVELSTATUS_FAILED_CRYSTALS;
				RewardQuota_CountUnits();

				// This is the only instance where music is stopped, while the level is still playing without an objective showing.
				/// TODO: Maybe move this to Lego_QuitLevel???
				Lego_ChangeMusicPlaying(false); // End music.
			}
		}
	}


	if (Front_IsTriggerMissionRestart() || Gods98::Main_ProgrammerMode() == 11) {
		Front_RestartLevel();
	}


	/// UI+GFX LOGIC: Switch vertex/block mode
	if (legoGlobs.flags1 & GAME1_DEBUG_SHOWVERTEXMODE) {

		const char* showVertexMode = ((legoGlobs.flags1 & GAME1_VERTEXMODE) ? "Vertex" : "Block");
		Gods98::Font_PrintF(legoGlobs.fontTallYellow, 10, 10, "%s mode", showVertexMode);
	}
	if ((legoGlobs.flags1 & GAME1_VERTEXMODE) && legoGlobs.digVertexShowPointer) {
		Vector4F transform;
		Vector3F vertPos;
		Map3D_GetBlockFirstVertexPosition(legoGlobs.currLevel->map, legoGlobs.digVertexBlockPos.x, legoGlobs.digVertexBlockPos.y, &vertPos);

		Gods98::Viewport_Transform(legoGlobs.viewMain, &transform, &vertPos);
		const Point2I pointerPos = {
			(sint32)(transform.x / transform.w),
			(sint32)(transform.y / transform.w),
		};

		Pointer_SetCurrent_IfTimerFinished(Pointer_Drill);
		Pointer_DrawPointer((uint32)pointerPos.x, (uint32)pointerPos.y);
		legoGlobs.flags1 &= ~GAME1_VERTEXLOCKONPOINTER;
	}

	// UI LOGIC: Tooltips
	ToolTip_Update(Gods98::msx(), Gods98::msy(), elapsedReal);


	/// DEBUG KEY UI LOGIC: Change upgrade model
	// Awkwardly... this is only the UI VISUAL for the debug command to change upgrade parts.
	if (Lego_IsAllowDebugKeys()) {
		/// DEBUG KEYBIND: [5]  "Change selected unit visual upgrade parts (Carry level bit)"
		/// DEBUG KEYBIND: [6]  "Change selected unit visual upgrade parts (Scan  level bit)"
		/// DEBUG KEYBIND: [7]  "Change selected unit visual upgrade parts (Speed level bit)"
		/// DEBUG KEYBIND: [8]  "Change selected unit visual upgrade parts (Drill level bit)"
		if (Shortcut_IsPressed(ShortcutID::Debug_ToggleUpgradeCarry) ||
			Shortcut_IsPressed(ShortcutID::Debug_ToggleUpgradeScan) ||
			Shortcut_IsPressed(ShortcutID::Debug_ToggleUpgradeSpeed) ||
			Shortcut_IsPressed(ShortcutID::Debug_ToggleUpgradeDrill))
		{
			gamectrlGlobs.dbgUpgradeChangeTimer = 1.0f;
		}
		if (gamectrlGlobs.dbgUpgradeChangeTimer > 0.0f) {
			uint32 unitsCount = 0; // dummy init
			LegoObject** selectedUnits = Message_GetSelectedUnits2(&unitsCount);
			if (unitsCount != 0) {
				LegoObject* primaryUnit = selectedUnits[0];

				const UpgradesModel* upgrades;
				if (primaryUnit->type == LegoObject_Building)
					upgrades = &primaryUnit->building->upgrades;
				else if (primaryUnit->type == LegoObject_Vehicle)
					upgrades = &primaryUnit->vehicle->upgrades;
				else
					upgrades = nullptr;

				if (upgrades != nullptr) {
					// The compiler must have been drunk to choose this over the normal bit shifts....
					//const uint32 upgLvl = upgrades->currentLevel;
					//uVar11 = CONCAT44(upgLvl, upgLvl >> 1) & 0x100000001;
					//Gods98::Font_PrintF(legoGlobs.fontStandard, 0, 0, "Upgrade - (%i%i%i%i)",
					//					upgLvl >> 3 & 1, upgLvl >> 2 & 1, (uint32)uVar11, (uint32)(uVar11 >> 0x20));

					const uint32 upgLvl = upgrades->currentLevel;
					Gods98::Font_PrintF(legoGlobs.fontStandard, 0, 0, "Upgrade - (%i%i%i%i)",
										((upgLvl & UPGRADE_FLAG_CARRY) ? 1 : 0),
										((upgLvl & UPGRADE_FLAG_SCAN)  ? 1 : 0),
										((upgLvl & UPGRADE_FLAG_SPEED) ? 1 : 0),
										((upgLvl & UPGRADE_FLAG_DRILL) ? 1 : 0));
				}
			}
			gamectrlGlobs.dbgUpgradeChangeTimer -= 0.1f;
		}
	}


	// SFX LOGIC: AMBIENCE SFX
	if (updateGlobs.dripSFXTimer < 0.0f) {
		SFX_Random_PlaySoundNormal(SFX_Drip, false);
		updateGlobs.dripSFXTimer = Gods98::Maths_RandRange(75.0f, 125.0f);
	}
	if (updateGlobs.ambientSFXTimer < 0.0f) {
		SFX_Random_PlaySoundNormal(SFX_Ambient, false);
		updateGlobs.ambientSFXTimer = Gods98::Maths_RandRange(250.0f, 750.0f);
	}
	updateGlobs.dripSFXTimer -= elapsedInterface;
	updateGlobs.ambientSFXTimer -= elapsedInterface;

	/// UI LOGIC: Game speed increment/decrement.
	{
		if (gamectrlGlobs.dbgSpeedChangeTimer > 0.0f) {
			gamectrlGlobs.dbgSpeedChangeTimer -= elapsedInterface;
			const real32 gameSpeedPercent = (Lego_GetGameSpeed() * 100.0f);
			Gods98::Font_PrintF(legoGlobs.fontStandard, 10, 80, "Game Speed %0.0f%%", (double)gameSpeedPercent);
		}
	}

	/// DEBUG KEY GAME+UI LOGIC: Sound3D roll-off factor.
	if (Lego_IsAllowDebugKeys()) {

		/// FIX ME: dbgRollOffFactorValue is never assigned to the loaded Lego.cfg value,
		///         so once we change this value, it will be changed from 1.0f, instead of its initial state... :\

		// This condition is never hit (value is always clamped to [0.0f,10.0f]),
		//  and the initial variable value is 1.0f...
		// --==<POWERED BY JANK>==--
		if (updateGlobs.dbgRollOffFactorValue == -1.0f) {
			updateGlobs.dbgRollOffFactorValue = Gods98::Sound3D_SetRollOffFactor(-1.0f);
		}

		// IsKeyDown(KEY_NINE) (10)
		//  "Decreases 3D sound roll-off factor." (has priority over KEY_ZERO)
		// IsKeyDown(KEY_ZERO) (11)
		//  "Increases 3D sound roll-off factor."
		real32 rollOffFactorAmount = 0.0f;
		if (Shortcut_IsDown(ShortcutID::Debug_DecreaseSound3DRollOffFactor))
			rollOffFactorAmount = -0.05f;
		else if (Shortcut_IsDown(ShortcutID::Debug_IncreaseSound3DRollOffFactor))
			rollOffFactorAmount = 0.05f;

		if (rollOffFactorAmount != 0.0f) {
			updateGlobs.dbgRollOffFactorValue += rollOffFactorAmount;
			// Re-assign in-case there's any change from clamping the value(?)
			updateGlobs.dbgRollOffFactorValue = Gods98::Sound3D_SetRollOffFactor(updateGlobs.dbgRollOffFactorValue);

			gamectrlGlobs.dbgRollOffChangeTimer = (STANDARD_FRAMERATE * 1.0f); // 1 second
		}

		if (gamectrlGlobs.dbgRollOffChangeTimer > 0.0f) {
			Gods98::Font_PrintF(legoGlobs.fontStandard, 10, 80, "3D sound fall off: %f", (double)updateGlobs.dbgRollOffFactorValue);
		}
		gamectrlGlobs.dbgRollOffChangeTimer -= elapsedInterface;
	}

	/// GAME LOGIC: NERPs timers
	/// DEBUG UI LOGIC: NoNERPs.
	// Aside from clearing tutorial flags when toggled on, it seems this really is the only
	// other difference. Along with preventing NERPsRuntime_Exexute higher up in the MainLoop function.
	if (!(legoGlobs.flags1 & GAME1_DEBUG_NONERPS)) {
		NERPsRuntime_UpdateTimers(elapsedInterface);
	}
	else {
		Gods98::Font_PrintF(legoGlobs.fontStandard, 0, 0, "NoNERPS");
	}


	// UI LOGIC: Oxygen info/text messages for change in level.
	if ((legoGlobs.currLevel)->oxygenLevel > 0.0f) {

		if ((legoGlobs.currLevel)->oxygenLevel > (legoGlobs.currLevel)->oxygenLevelMessage) {
			// Oxygen level has increased by any%.

			if ((legoGlobs.currLevel)->oxygenLevel == 100.0f) {
				// Show info if oxygen level is back at 100% only.
				Info_Send(Info_AirRestored, nullptr, nullptr, nullptr);
			}

			// Running out message is effectively reset every tick that oxygen increases.
			//  So the running-out message's target will be moved to the `(max level reached - 5%)`.
			(legoGlobs.currLevel)->oxygenLevelMessage = (legoGlobs.currLevel)->oxygenLevel;
		}
		else if ((legoGlobs.currLevel)->oxygenLevel < ((legoGlobs.currLevel)->oxygenLevelMessage - 5.0f)) {
			// Oxygen level has decreased by at least 5% from the highest level reached.

			//Text_Type airBeatText;
			if ((legoGlobs.currLevel)->oxygenLevel > 10.0f) {
				Info_Send(Info_AirDepleting, nullptr, nullptr, nullptr);
				Text_DisplayMessage(Text_AirSupplyRunningOut, true, false);
			}
			else {
				Info_Send(Info_AirLow, nullptr, nullptr, nullptr);
				Text_DisplayMessage(Text_AirSupplyLow, true, false);
			}

			// Don't annoy the user with messages every tick,
			(legoGlobs.currLevel)->oxygenLevelMessage = (legoGlobs.currLevel)->oxygenLevel;
		}
	}
	else { // if ((legoGlobs.currLevel)->oxygenLevel <= 0.0f)
		Info_Send(Info_AirOut, nullptr, nullptr, nullptr);
	}

	// SFX+UI LOGIC: Oxygen meter heartbeat for low oxygen and flashing O2 low image.
	SFX_ID airBeatSFX = SFX_NULL;
	if ((legoGlobs.currLevel)->oxygenLevel > 10.0f) {
		// Stop air beat SFX. We're not low enough yet to FEEEEL THE PRESSSURRREE~

		// This panelGlobs variable tracks both the air beat SFX playing, and the flashing O2 low icon.
		if (Panel_AirMeter_IsOxygenLow() && SFX_GetType("SND_AirBeat", &airBeatSFX)) {
			/// CHANGE: Get the current-assigned sound3DHandle (instead of a random one).
			SFX_Sound3D_StopSound(SFX_Current_GetSound3DHandle(airBeatSFX));
		}

		Panel_AirMeter_SetOxygenLow(false);
	}
	else {
		// Set SFX to play if the last state did not have it playing.
		
		if (!Panel_AirMeter_IsOxygenLow() && SFX_GetType("SND_AirBeat", &airBeatSFX)) {
			SFX_Random_PlaySoundNormal(airBeatSFX, true);
		}

		if (SFX_GetType("SND_AirBeat", &airBeatSFX)) {
			// Increase air beat SFX volume based on how close we are to zero.

			/// NOTE: Sound3D volume has a working range of [-10000,0].
			///       This means that multiplication for its range is always calculated off the
			///       minimum, rather than the maximum value... which is zero...

			/// FIX APPLY: Change math to work off the volume RANGE, rather than assuming our
			///            volume system will always remain the same, with a negative minimum
			///            and zero maximum.
			/// 
			/// WARNING: THIS STILL ASSUMES MAX VOLUME IS 100% (and not higher)!
			const sint32 volMax = Gods98::Sound3D_MaxVolume();
			const sint32 volMin = Gods98::Sound3D_MinVolume();
			const sint32 volRange = volMax - volMin;
			

			// Divide by 10% (max level when SFX plays) so we get range [1.0f,0.0f] for O2 level [10%,0%]
			real32 airDelta = ((legoGlobs.currLevel)->oxygenLevel / 10.0f);
			const real32 airMaxVol = (real32)volMax;                             // Volume at 100% as O2 nears 0%.
			const real32 airMinVol = (real32)volMin + ((real32)volRange * 0.9f); // Volume at 90% as O2 nears 10%.

			// Oxygen Level [0%,10%] ==> SFX Volume [0,-1000] aka [100%,90%]
			sint32 airBeatNewVol = (sint32)Gods98::Maths_Interpolate(airMaxVol, airMinVol, airDelta);

			/// ORIGINAL MATH RELYING ON VOLUME MIN:
			//sint32 airBeatNewVol = (sint32)((legoGlobs.currLevel)->oxygenLevel * 0.1f * (real32)volMin * 0.1f);

			// Only set volume if changed.
			sint32 airBeatCurrVol = SFX_Random_GetBufferVolume(airBeatSFX);
			if (airBeatNewVol != airBeatCurrVol) {
				SFX_Random_SetBufferVolume(airBeatSFX, airBeatNewVol);
			}
		}

		Panel_AirMeter_SetOxygenLow(true);
	}

	/// BETA: RockFall. Show listset counts using the F8 key.
	/// CHANGE: Instead of holding F8, F8 is now a toggle.
	/// DEBUG KEYBIND: [F8]  Switch between debug overlays for ListSets, NERPs, or nothing.
	static uint32 sShowDebugOverlayType = 0;
	if (Lego_IsAllowDebugKeys()) {
		const uint32 DEBUGOVERLAY_LISTSETS = 1;
		const uint32 DEBUGOVERLAY_NERPS    = 2;
		const uint32 DEBUGOVERLAY_WATER    = 3;
		const uint32 DEBUGOVERLAY_MAXTYPES = 4;
		sint32 dbgX = 100;
		sint32 dbgY = 100 - 12;
		const uint32 r = 12;

		if (Shortcut_IsPressed(ShortcutID::Debug_SwitchDebugOverlay)) {
			sShowDebugOverlayType = (sShowDebugOverlayType + 1) % DEBUGOVERLAY_MAXTYPES;
		}

		switch (sShowDebugOverlayType) {
		case DEBUGOVERLAY_LISTSETS:
			{
				uint32 row = 0;
				Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "[ListSets]");
				Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "AITasks %i", (sint32)aiListSet.EnumerateAlive().Count());
				Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "EFences %i", (sint32)efenceListSet.EnumerateAlive().Count());
				Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "Objects %i", (sint32)objectListSet.EnumerateAlive().Count());
				// `CountAlive()` can only be used when we are SURE we've hooked
				// and replaced all functions creating and removing listset items.
				// Keeping the game module functions as `EnumerateAlive().Count()`, will
				//  allow hooking and unhooking them without having to go back and change this.
				Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "Configs %i", (sint32)Gods98::configListSet.CountAlive());
				Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "Containers %i", (sint32)Gods98::containerListSet.CountAlive());
				Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "Fonts %i", (sint32)Gods98::fontListSet.CountAlive());
				Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "Images %i", (sint32)Gods98::imageListSet.CountAlive());
				Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "Meshes %i", (sint32)Gods98::meshListSet.CountAlive());
				Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "Viewports %i", (sint32)Gods98::viewportListSet.CountAlive());
			}
			break;
		case DEBUGOVERLAY_NERPS:
			{
				uint32 row = 0;
				Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "[NERPs]");
				for (uint32 i = 0; i < _countof(nerpsruntimeGlobs.registers); i++) {
					Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "R%i %i", i, (sint32)nerpsruntimeGlobs.registers[i]);
				}
				for (uint32 i = 0; i < _countof(nerpsruntimeGlobs.timers); i++) {
					const real32 seconds = nerpsruntimeGlobs.timers[i] / 1000.0f;
					Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "Timer%i %0.3fs", i, seconds);
				}
				Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "Flags %04x", (uint32)NERPs_GetTutorialFlags());
			}
			break;
		case DEBUGOVERLAY_WATER:
			{
				static constexpr const auto DIRCHARS = array_of<char>('U', 'R', 'D', 'L');
				static constexpr const Point2F DIRS[4] = {
					{  0.0f, -1.0f },
					{  1.0f,  0.0f },
					{  0.0f,  1.0f },
					{ -1.0f,  0.0f },
				};
				dbgX = 10;
				dbgY = 206;

				uint32 row = 0;
				Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "[Water]");
				if (waterGlobs.poolCount == 0) {
					Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "No pools of water in this level");
				}
				else {
					// Only show the first pool of water, any more and we'd run out of screen.
					auto pool = &waterGlobs.poolList[0];
					Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "Water Level %0.1f/%0.1f", (double)pool->currWaterLevel, (double)pool->highWaterLevel);
					row++;
					for (uint32 j = 0; j < pool->drainCount; j++) {
						auto drain = &pool->drainList[j];
						const sint32 drainX = static_cast<sint32>(pool->blocks[drain->blockIndex].x + DIRS[drain->direction].x);
						const sint32 drainY = static_cast<sint32>(pool->blocks[drain->blockIndex].y + DIRS[drain->direction].y);
						Gods98::Font_PrintF(legoGlobs.fontToolTip, dbgX, dbgY+r*(row++), "Drain (%i,%i) Level %0.1f  [%0.1f, %0.1f]  '%c'%s",
											drainX, drainY,
											(double)drain->drainWaterLevel,
											(double)drain->elapsedUp_c,
											(double)drain->elapsedDown_10,
											DIRCHARS[drain->direction],
											(drain->active ? " (Active)" : ""));
					}
				}
			}
			break;
		}
	}


	legoGlobs.timerGame_e3c -= elapsedWorld;

	return true;
}


// <LegoRR.exe @00424c20>
void __cdecl LegoRR::Lego_Shutdown_Quick(void)
{
	Lego_Exit();
}

// This is only called as a ProgrammerMode_10 setting, where proper cleanup is performed.

// <LegoRR.exe @00424c30>
void __cdecl LegoRR::Lego_Shutdown_Full(void)
{
	Loader_display_shutdown();

	Reward_Shutdown();

	Lego_ChangeMusicPlaying(false); // End music.
	Lego_SetSoundOn(false);

	Level_Free();

	Interface_Shutdown();

	Gods98::Config_Free(legoGlobs.config);
	Gods98::Viewport_Remove(legoGlobs.viewMain);
	Gods98::Container_Remove(legoGlobs.rootCont);

	Camera_Free(legoGlobs.cameraMain);
	Camera_Free(legoGlobs.cameraTrack);
	Camera_Free(legoGlobs.cameraFP);

	// FIXME: There seems to be memory cleanup for lang*_name, but not lang*_theName...

	// Free vehicle object data:
	for (uint32 i = 0; i < legoGlobs.vehicleCount; i++) {
		Vehicle_Remove(&legoGlobs.vehicleData[i]);
		Gods98::Mem_Free(legoGlobs.vehicleName[i]);
	}
	Gods98::Mem_Free(legoGlobs.vehicleData);
	Gods98::Mem_Free(legoGlobs.vehicleName);
	Gods98::Mem_Free(legoGlobs.langVehicle_name);

	// Free minifigure object data:
	for (uint32 i = 0; i < legoGlobs.miniFigureCount; i++) {
		Creature_Remove(&legoGlobs.miniFigureData[i]);
		Gods98::Mem_Free(legoGlobs.miniFigureName[i]);
	}
	Gods98::Mem_Free(legoGlobs.miniFigureData);
	Gods98::Mem_Free(legoGlobs.miniFigureName);
	Gods98::Mem_Free(legoGlobs.langMiniFigure_name);

	// Free monster object data:
	for (uint32 i = 0; i < legoGlobs.rockMonsterCount; i++) {
		Creature_Remove(&legoGlobs.rockMonsterData[i]);
		Gods98::Mem_Free(legoGlobs.rockMonsterName[i]);
	}
	Gods98::Mem_Free(legoGlobs.rockMonsterData);
	Gods98::Mem_Free(legoGlobs.rockMonsterName);
	Gods98::Mem_Free(legoGlobs.langRockMonster_name);

	// Free upgrade part object data:
	for (uint32 i = 0; i < legoGlobs.upgradeCount; i++) {
		Upgrade_Part_Remove(&legoGlobs.upgradeData[i]);
		Gods98::Mem_Free(legoGlobs.upgradeName[i]);
	}
	Gods98::Mem_Free(legoGlobs.upgradeData);
	Gods98::Mem_Free(legoGlobs.upgradeName);
	Gods98::Mem_Free(legoGlobs.langUpgrade_name);

	// Free building object data:
	for (uint32 i = 0; i < legoGlobs.buildingCount; i++) {
		Building_Remove(&legoGlobs.buildingData[i]);
		Gods98::Mem_Free(legoGlobs.buildingName[i]);
	}
	Gods98::Mem_Free(legoGlobs.buildingData);
	Gods98::Mem_Free(legoGlobs.buildingName);
	Gods98::Mem_Free(legoGlobs.langBuilding_name);


	Gods98::Mem_Free(legoGlobs.langPowerCrystal_name);
	Gods98::Mem_Free(legoGlobs.langOre_name);
	Gods98::Mem_Free(legoGlobs.langProcessedOre_name);
	Gods98::Mem_Free(legoGlobs.langDynamite_name);
	Gods98::Mem_Free(legoGlobs.langBarrier_name);
	Gods98::Mem_Free(legoGlobs.langElectricFence_name);
	Gods98::Mem_Free(legoGlobs.langSpiderWeb_name);
	Gods98::Mem_Free(legoGlobs.langOohScary_name);
	Gods98::Mem_Free(legoGlobs.langPath_name);


	Gods98::TextWindow_Remove(legoGlobs.textOnlyWindow);

	Gods98::Font_Remove(legoGlobs.fontStandard);
	Gods98::Font_Remove(legoGlobs.fontToolTip);

	Effect_RemoveAll_RockFall();
	AITask_Shutdown();
	LegoObject_Shutdown();

	Gods98::Container_Shutdown();
	Gods98::Viewport_Shutdown();

	const char* loaderProfileName = nullptr;
	if (Gods98::Main_ProgrammerMode() == 10) {
		if (!Gods98::Sound_IsInitialised())
			loaderProfileName = "LoaderProfileNoSound.txt";
		else
			loaderProfileName = "LoaderProfile.txt";
	}
	Loader_Shutdown(loaderProfileName);

	//Gods98::TextWindow_Shutdown(); // Determined using RockFall beta (function does nothing).

	Gods98::Image_Shutdown();
	Gods98::Lws_Shutdown();
	Gods98::Sound3D_ShutDown();
	Gods98::DirectDraw_ClearRGB(nullptr, 0, 0, 0); // black
}

// Simply halts music playback, then calls std::exit(0);
// (LRR_Exit)
// <LegoRR.exe @00424fd0>
__declspec(noreturn) void __cdecl LegoRR::Lego_Exit(void)
{
	Lego_ChangeMusicPlaying(false); // End music.
	Gods98::Main_Exit();
}
#endif

// Returning FALSE will naturally exit the program (as handled by Lego_MainLoop).
// 
// keyDownT is unused, and no keybinds exist.
// keyDownR is unused, but debug keybinds exist for the rewards screen.
// keyDownAddSelection is only needed by Lego_HandleWorld().
// <LegoRR.exe @00424ff0>
bool32 __cdecl LegoRR::Lego_HandleKeys(real32 elapsedGame, real32 elapsedInterface, OUT bool32* keyDownT, OUT bool32* keyDownR, OUT bool32* keyDownAddSelection)
{
	/// CHANGE: Move these to the top for clarity. These only appeared after return false cases, so there were no issues.
	*keyDownT = false;
	*keyDownR = false;
	*keyDownAddSelection = false;


	/// KEYBIND: [Return]  "Start/submit unit rename input."
	// Evaluate if we can perform a rename before checking key (for future hotkey implementation).
	if (!(legoGlobs.flags1 & GAME1_FREEZEINTERFACE) && (legoGlobs.flags2 & GAME2_ALLOWRENAME) &&
		legoGlobs.viewMode == ViewMode_Top && Message_AnyUnitSelected())
	{
		LegoObject* renameObj = Message_GetPrimarySelectedUnit();

		// Can only rename *unique* Mini-Figures. AKA, ones that have upgraded or trained an ability.
		if (renameObj != nullptr && renameObj->type == LegoObject_MiniFigure &&
			(renameObj->abilityFlags != ABILITY_FLAG_NONE || renameObj->objLevel != 0) &&
			Shortcut_IsReleased(ShortcutID::RenameUnit))
		{
			Vector3F renameWorldPos;
			Gods98::Container* cont = LegoObject_GetActivityContainer(renameObj);
			Gods98::Container_GetPosition(cont, nullptr, &renameWorldPos);
			renameWorldPos.z -= StatsObject_GetCollHeight(renameObj) / 2.0f; // Raise Z to center of collision box.

			// Don't try to rename units that are off-screen.
			Point2F renameScreenPos;
			Viewport_WorldToScreen(legoGlobs.viewMain, &renameScreenPos, &renameWorldPos);
			if (renameScreenPos.x >= 40.0f && renameScreenPos.x <= Gods98::appWidth() &&
				renameScreenPos.y >= 40.0f && renameScreenPos.y <= Gods98::appHeight())
			{
				Lego_SetPaused(false, true);
				if (renameObj->customName == nullptr) {
					// Ensure a customName buffer is created, so that we can use it for editing.
					LegoObject_SetCustomName(renameObj, "Object");
					renameObj->customName[0] = '\0';
				}
				legoGlobs.renameInput = renameObj->customName;
				legoGlobs.renamePosition = renameScreenPos;
			}
		}
	}

	if (!(legoGlobs.flags1 & GAME1_LEVELENDING) && !(legoGlobs.flags2 & GAME2_INOPTIONSMENU) &&
		!HelpWindow_IsEnabled_AndFlags_3_AndNoTutorialFlags() && !Objective_IsShowing())
	{
		/// DEBUG KEYBIND: [Space]+[Esc]  "Exits program forcefully (while held)."
		if (Lego_IsAllowDebugKeys() && Shortcut_IsDown(ShortcutID::Debug_HardExit)) {
			Gods98::Sound3D_ShutDown();
			Lego_Exit();
			return false; // IMMEDIATE EXIT
		}

		/// DEBUG KEYBIND: [Return]+[Esc]  "Exits program naturally (while held)."
		if (Lego_IsAllowDebugKeys() && Shortcut_IsDown(ShortcutID::Debug_SoftExit)) {
			return false;
		}

		/// KEYBIND: [Esc]  "Pauses the game (but does not unpause, while held)."
		if (Shortcut_IsDown(ShortcutID::EscapePause)) {
			Lego_SetPaused(false, true);
			legoGlobs.flags1 |= GAME1_PAUSED;
		}
	}
	/*else {
		/// UNREACHABLE CODE: Condition is identical to condition for above block.
		// Input_IsKeyDown(Keys::KEY_ESCAPE) (1)
		if (Input_IsKeyDown(Keys::KEY_ESCAPE) &&
			!(legoGlobs.flags1 & GAME1_LEVELENDING) && !(legoGlobs.flags2 & GAME2_INOPTIONSMENU) &&
			!HelpWindow_IsEnabled_AndFlags_3_AndNoTutorialFlags() && !Objective_IsShowing())
		{
			Gods98::Sound3D_ShutDown();
			Lego_Exit();
			return false; // IMMEDIATE EXIT
		}
	}*/

	if (legoGlobs.flags2 & GAME2_LEVELEXITING) {

		/// CHANGE: Now requires debug keys and [Esc] modifier.
		/// DEBUG KEYBIND: [(new)Esc]+[Space]  "Exits program forcefully (while held during level exit)."
		if (!Front_IsFrontEndEnabled() || (Lego_IsAllowDebugKeys() && Shortcut_IsDown(ShortcutID::Debug_HardExit))) {
			// If !Front_IsFrontEndEnabled() then game was booted up
			//  into a single level, exit now that the level is done.
			Gods98::Sound3D_ShutDown();
			Lego_Exit();
			return false; // IMMEDIATE EXIT
		}
		/// CHANGE: Now requires debug keys and [Esc] modifier.
		/// DEBUG KEYBIND: [(new)Esc]+[Return]  "Exits program naturally (while held during level exit)."
		if (Lego_IsAllowDebugKeys() && Shortcut_IsDown(ShortcutID::Debug_SoftExit)) {
			return false;
		}

		// Complete the end of the level. WHY IS THIS IN HANDLEKEYS!??
		if (!Lego_EndLevel()) { // No next level after this.
			return false;
		}
	}

	if (!(legoGlobs.flags1 & (GAME1_FREEZEINTERFACE|GAME1_LEVELENDING))) {

		/// KEYBIND: [F2]+(any)  "Interface menu actions defined in Lego.cfg."
		Interface_DoF2InterfaceKeyAction();

		/// KEYBIND: [Space]  "Toggle unit info bubbles/HUDs visibility."
		if (Shortcut_IsPressed(ShortcutID::ToggleObjectInfo)) {
			Bubble_ToggleObjectUIsAlwaysVisible();
		}

		/// CONFLICT KEYBIND: [LShift], [RShift]  Used by Lego_HandleWorld to avoid certain actions.
		//if (Input_IsKeyDown(Keys::KEY_LEFTSHIFT) || Input_IsKeyDown(Keys::KEY_RIGHTSHIFT)) {
		if (Shortcut_IsDown(ShortcutID::AddSelectionModifier)) {
			*keyDownAddSelection = true;
		}


		/// CUSTOM: FP controls in topdown view.
		if (legoGlobs2.topdownFPControlsOn && !(legoGlobs.flags1 & (GAME1_FREEZEINTERFACE|GAME1_LEVELENDING))) {

			LegoObject* fpUnit = Message_GetPrimarySelectedUnit();
			if (fpUnit != nullptr && Message_GetNumSelectedUnits() == 1 && Lego_GetViewMode() == ViewMode_Top) {
				// Input_ClearKey is used on success so that we don't influence the Camera movement.

				// Turning is ridiculously fast at max speed, it's already fast enough at 100% speed.
				real32 fasterTurnSpeed = 3.5f;
				if (Lego_GetGameSpeed() > 1.0f) {
					fasterTurnSpeed /= Lego_GetGameSpeed();
				}

				// First person view controls
				/// CHEAT KEYBIND: [Up]   "First person view: Move forward."
				if (Shortcut_IsDown(ShortcutID::Cheat_TopdownFPMoveForward)) {
					LegoObject_FP_Move(fpUnit, 1, 0, 0.0f);
					//Input_ClearKey(Keys::KEY_CURSORUP);
				}
				/// CHEAT KEYBIND: [Down]   "First person view: Move back."
				if (Shortcut_IsDown(ShortcutID::Cheat_TopdownFPMoveBackward)) {
					LegoObject_FP_Move(fpUnit, -1, 0, 0.0f);
					//Input_ClearKey(Keys::KEY_CURSORDOWN);
				}
				/// CHEAT KEYBIND: [Left]   "First person view: Turn left."
				if (Shortcut_IsDown(ShortcutID::Cheat_TopdownFPTurnLeft)) {
					LegoObject_FP_Move(fpUnit, 0, 0, -0.05f * Cheat_GetFasterUnitCoef(fpUnit, fasterTurnSpeed));
					//Input_ClearKey(Keys::KEY_CURSORLEFT);
				}
				/// CHEAT KEYBIND: [Right]   "First person view: Turn right."
				if (Shortcut_IsDown(ShortcutID::Cheat_TopdownFPTurnRight)) {
					LegoObject_FP_Move(fpUnit, 0, 0, 0.05f * Cheat_GetFasterUnitCoef(fpUnit, fasterTurnSpeed));
					//Input_ClearKey(Keys::KEY_CURSORRIGHT);
				}
				/// CHEAT KEYBIND: [Z]  "First person view: Strafe left."
				if (Shortcut_IsDown(ShortcutID::Cheat_TopdownFPStrafeLeft)) {
					LegoObject_FP_Move(fpUnit, 0, -1, 0.0f);
					//Input_ClearKey(Keys::KEY_Z);
				}
				/// CHEAT KEYBIND: [X]  "First person view: Strafe right."
				if (Shortcut_IsDown(ShortcutID::Cheat_TopdownFPStrafeRight)) {
					LegoObject_FP_Move(fpUnit, 0, 1, 0.0f);
					//Input_ClearKey(Keys::KEY_X);
				}
			}
		}

		if ((!legoGlobs2.topdownFPControlsOn || Message_GetPrimarySelectedUnit() == nullptr) &&
			legoGlobs.viewMode == ViewMode_Top &&
			!(NERPs_GetTutorialFlags() & TUTORIAL_FLAG_NOCAMERA)) // Freeze/control camera flag?
		{
			// Topdown camera controls.

			/// KEYBIND: [Down]  "Topdown view: Pitch lower (around center, while held)."
			if (Shortcut_IsDown(ShortcutID::CameraTiltDown)) {
				Camera_AddTilt(legoGlobs.cameraMain, elapsedInterface * 0.02f);
			}

			/// KEYBIND: [Up]  "Topdown view: Pitch raise (around center, while held)."
			if (Shortcut_IsDown(ShortcutID::CameraTiltUp)) {
				Camera_AddTilt(legoGlobs.cameraMain, elapsedInterface * -0.02f);
			}

			/// KEYBIND: [Left]  "Topdown view: Rotate clockwise (around center, while held)."
			if (Shortcut_IsDown(ShortcutID::CameraTurnLeft)) {
				Camera_AddRotation(legoGlobs.cameraMain, elapsedInterface * 0.02f);
			}

			/// KEYBIND: [Right]  "Topdown view: Rotate counter-clockwise (around center, while held)."
			if (Shortcut_IsDown(ShortcutID::CameraTurnRight)) {
				Camera_AddRotation(legoGlobs.cameraMain, elapsedInterface * -0.02f);
			}

			/// KEYBIND: [-]  "Topdown view: Zoom out (while held)."
			if (Shortcut_IsDown(ShortcutID::CameraZoomOut)) {
				Camera_AddZoom(legoGlobs.cameraMain, elapsedInterface * 3.0f);
			}

			/// KEYBIND: [=]  "Topdown view: Zoom in (while held)."
			if (Shortcut_IsDown(ShortcutID::CameraZoomIn)) {
				Camera_AddZoom(legoGlobs.cameraMain, elapsedInterface * -3.0f);
			}
		}


		/// CHANGE: Moved Increase/Decrease shortcuts from Lego_MainLoop.
		/// CHANGE: No longer a debug key.
		/// KEY GAME+UI LOGIC: Game speed increment/decrement.
		//if (Lego_IsAllowDebugKeys())
		if (Lego_IsAllowDebugKeys() || !NERPs_AnyTutorialFlags()) {
			bool showSpeed = true;
			real32 gameSpeedAmount = 0.0f;

			/// KEYBIND: [Numpad 7]  "Set game speed to 200%, or 300% if debug keys are enabled (while held)."
			if (Shortcut_IsDown(ShortcutID::MaxGameSpeed)) {
				if (Lego_IsAllowDebugKeys())
					Lego_SetGameSpeed(3.0f);
				else
					Lego_SetGameSpeed(2.0f);
				showSpeed = true;
			}
			/// KEYBIND: NULL  "Set game speed to 100% (while held)."
			else if (Shortcut_IsDown(ShortcutID::DefaultGameSpeed)) {
				Lego_SetGameSpeed(1.0f);
				showSpeed = true;
			}
			/// KEYBIND: NULL  "Set game speed to 33%.
			else if (Shortcut_IsDown(ShortcutID::MinGameSpeed)) {
				Lego_SetGameSpeed(1.0f / 3.0f);
				showSpeed = true;
			}
			/// DEBUG KEYBIND: NULL  "Set game speed to 0%.
			else if (Lego_IsAllowDebugKeys() && Shortcut_IsDown(ShortcutID::Debug_FreezeGameSpeed)) {
				Lego_SetGameSpeed(0.0f);
				showSpeed = true;
			}
			/// KEYBIND: [Numpad 9]  "Increases game speed while held."
			/// KEYBIND: [Numpad 8]  "Decreases game speed while held."
			else if (Shortcut_IsDown(ShortcutID::IncreaseGameSpeed)) {
				gameSpeedAmount =  1.0f;
			}
			else if (Shortcut_IsDown(ShortcutID::DecreaseGameSpeed)) {
				gameSpeedAmount = -1.0f;
			}
			else {
				showSpeed = false;
			}

			if (gameSpeedAmount != 0.0f) {
				// Why is it that DEBUG COMMANDS are the ones that properly pace things with elapsed time??
				// Game speed changes at a rate of 25% per second (or 1% per standard frame).
				const real32 maxGameSpeed = (Lego_IsAllowDebugKeys() ? 3.0f : 2.0f);
				const real32 minGameSpeed = (Lego_IsAllowDebugKeys() ? 0.0f : (1.0f / 3.0f));
				real32 newGameSpeed = Lego_GetGameSpeed() + (gameSpeedAmount * elapsedInterface * 0.01f);

				// Clamp game speed to [0.0f,3.0f] or [0.33f,2.0f]
				if (newGameSpeed > maxGameSpeed) newGameSpeed = maxGameSpeed;
				if (newGameSpeed < minGameSpeed) newGameSpeed = minGameSpeed;

				Lego_SetGameSpeed(newGameSpeed);

				// Clamp game speed to [0.0f,3.0f]
				//if (Lego_GetGameSpeed() > 3.0f) Lego_SetGameSpeed(3.0f);
				//if (Lego_GetGameSpeed() < 0.0f) Lego_SetGameSpeed(0.0f);
			}

			/// CHANGE: Show game speed for other speed-changing commands as well.
			if (showSpeed) {
				// Set timer to display game speed in the UI for a short time.
				gamectrlGlobs.dbgSpeedChangeTimer = (STANDARD_FRAMERATE * 1.0f); // 1 second
			}
		}

		if (Lego_IsAllowDebugKeys()) {
			/// DEBUG CONFLICT KEYBIND: [T]  Is not used for any keybinds.
			if (Input_IsKeyDown(Keys::KEY_T)) {
				*keyDownT = true;
			}

			/// DEBUG CONFLICT KEYBIND: [R]  Is only used for Rewards screen debug keybinds, but this variable is never used.
			if (Input_IsKeyDown(Keys::KEY_R)) {
				*keyDownR = true;
			}
		}

		/// DEBUG KEYBIND: [L]  "Instantly wins the level and goes to rewards screen."
		if (Lego_IsAllowDebugKeys() && Shortcut_IsPressed(ShortcutID::Debug_WinLevelInstant)) {
			if (!Lego_EndLevel()) { // No next level after this.
				return false;
			}
		}

		/// DEBUG KEYBIND: [LCtrl]+[F]  "Toggle framerate monitor."
		if (Lego_IsAllowDebugKeys() && Shortcut_IsPressed(ShortcutID::Debug_ToggleFPSMonitor)) {

			if (!(legoGlobs.flags1 & GAME1_SHOWFPS)) {
				legoGlobs.flags1 |= GAME1_SHOWFPS;
			}
			else {
				legoGlobs.flags1 &= ~GAME1_SHOWFPS;
			}

			const char* onOff = ((legoGlobs.flags1 & GAME1_SHOWFPS) ? "ON" : "OFF");
			TextWindow_PrintF(legoGlobs.textOnlyWindow, "\nFrame Rate Monitor: %s", onOff);
		}

		/// CHANGE: No longer a debug key.
		/// KEYBIND: [Tab]  "Toggle between Radar Map/Track Object View."
		if ((legoGlobs.flags1 & GAME1_RADARON)) {
			if ((Lego_IsAllowDebugKeys() || !NERPs_AnyTutorialFlags()) && Shortcut_IsPressed(ShortcutID::SwitchRadarMode)) {

				bool newIsTrackObjectView = !(legoGlobs.flags1 & GAME1_RADAR_TRACKOBJECTVIEW);
				if (newIsTrackObjectView) {
					legoGlobs.flags1 &= ~GAME1_RADAR_MAPVIEW;
					legoGlobs.flags1 |= GAME1_RADAR_TRACKOBJECTVIEW;
				}
				else {
					legoGlobs.flags1 &= ~GAME1_RADAR_TRACKOBJECTVIEW;
					legoGlobs.flags1 |= GAME1_RADAR_MAPVIEW;
				}
				// Flags_10 must be "disabled"
				Panel_Button_Hide(Panel_Radar, PanelButton_Radar_ZoomIn, newIsTrackObjectView);
				Panel_Button_Hide(Panel_Radar, PanelButton_Radar_ZoomOut, newIsTrackObjectView);
			}
		}

		/// KEYBIND: NULL  "Follows the selected unit in top-down view."
		if ((Lego_IsAllowDebugKeys() || !NERPs_AnyTutorialFlags()) && legoGlobs.viewMode == ViewMode_Top &&
			Shortcut_IsPressed(ShortcutID::FollowUnit))
		{
			LegoObject* primaryUnit = Message_GetPrimarySelectedUnit();
			if (primaryUnit != nullptr && primaryUnit != Lego_GetFollowUnit()) {
				Lego_SetFollowUnit(primaryUnit);
			}
			else {
				Lego_SetFollowUnit(nullptr);
			}
		}

		/// KEYBIND: NULL  "Enters or exits laser tracker mode for the selected unit (the red selection box)."
		if ((Lego_IsAllowDebugKeys() || !NERPs_AnyTutorialFlags()) && legoGlobs.viewMode == ViewMode_Top &&
			Shortcut_IsPressed(ShortcutID::LaserTrackerUnit))
		{
			LegoObject* primaryUnit = Message_GetPrimarySelectedUnit();

			if (primaryUnit != nullptr && !(primaryUnit->flags4 & LIVEOBJ4_LASERTRACKERMODE) &&
				(StatsObject_GetStatsFlags2(primaryUnit) & STATS2_CANDOUBLESELECT))
			{
				// It's necessary to clear selected units before entering laser tracker mode.
				Message_ClearSelectedUnits();
				LegoObject_TryEnterLaserTrackerMode(primaryUnit);
			}
			else {
				// Exit mode for all objects, then reselect the object originally in the mode.
				LegoObject* reselectUnit = nullptr;
				for (LegoObject* obj : objectListSet.EnumerateSkipUpgradeParts()) {
					if (obj->flags4 & LIVEOBJ4_LASERTRACKERMODE) {
						reselectUnit = obj;
					}
					LegoObject_Callback_ExitLaserTrackerMode(obj, nullptr);
				}
				if (reselectUnit != nullptr) {
					Message_ClearSelectedUnits();
					Message_SelectObject(reselectUnit, true);
				}
			}
		}

		/// KEYBIND: NULL  "Cycle the camera to the next building (or minifigure if no buildings) in the level."
		if ((Lego_IsAllowDebugKeys() || !NERPs_AnyTutorialFlags()) && Shortcut_IsPressed(ShortcutID::CameraCycleNextUnit)) {
			LegoObject_CameraCycleUnits();
		}

		/// DEBUG KEYBIND: [LCtrl]+[G]  "Toggle memory monitor."
		if (Lego_IsAllowDebugKeys() && Shortcut_IsPressed(ShortcutID::Debug_ToggleMemoryMonitor)) {

			if (!(legoGlobs.flags1 & GAME1_SHOWMEMORY)) {
				legoGlobs.flags1 |= GAME1_SHOWMEMORY;
			}
			else {
				legoGlobs.flags1 &= ~GAME1_SHOWMEMORY;
			}

			const char* onOff = ((legoGlobs.flags1 & GAME1_SHOWMEMORY) ? "ON" : "OFF");
			TextWindow_PrintF(legoGlobs.textOnlyWindow, "\nMemory Monitor: %s", onOff);
		}

		/// DEBUG KEYBIND: [LCtrl]+[Return]  "Toggle Noclip in first/second person view."
		if (Lego_IsAllowDebugKeys() && Shortcut_IsPressed(ShortcutID::Debug_ToggleFPNoClip)) {

			if (!(legoGlobs.flags1 & GAME1_DEBUG_NOCLIP_FPS)) {
				legoGlobs.flags1 |= GAME1_DEBUG_NOCLIP_FPS;
			}
			else {
				legoGlobs.flags1 &= ~GAME1_DEBUG_NOCLIP_FPS;
			}
		}

		if (legoGlobs.viewMode == ViewMode_FP && !(legoGlobs.objectFP->flags2 & LIVEOBJ2_THROWING)) {
			// First person view controls.

			/// KEYBIND: [Up]  "First person view: Move forward (while held)."
			if (Shortcut_IsDown(ShortcutID::FPMoveForward)) {
				LegoObject_FP_Move(legoGlobs.objectFP, 1, 0, 0.0f);
			}

			/// KEYBIND: [Down]  "First person view: Move back (while held)."
			if (Shortcut_IsDown(ShortcutID::FPMoveBackward)) {
				LegoObject_FP_Move(legoGlobs.objectFP, -1, 0, 0.0f);
			}

			/// KEYBIND: [Left]  "First person view: Turn left (while held)."
			if (Shortcut_IsDown(ShortcutID::FPTurnLeft)) {
				LegoObject_FP_Move(legoGlobs.objectFP, 0, 0, -0.05f * Cheat_GetFasterUnitCoef(legoGlobs.objectFP, 3.5f));
			}

			/// KEYBIND: [Right]  "First person view: Turn right (while held)."
			if (Shortcut_IsDown(ShortcutID::FPTurnRight)) {
				LegoObject_FP_Move(legoGlobs.objectFP, 0, 0, 0.05f * Cheat_GetFasterUnitCoef(legoGlobs.objectFP, 3.5f));
			}

			/// KEYBIND: [Z]  "First person view: Strafe left (while held)."
			if (Shortcut_IsDown(ShortcutID::FPStrafeLeft)) {
				LegoObject_FP_Move(legoGlobs.objectFP, 0, -1, 0.0f);
			}

			/// KEYBIND: [X]  "First person view: Strafe right (while held)."
			if (Shortcut_IsDown(ShortcutID::FPStrafeRight)) {
				LegoObject_FP_Move(legoGlobs.objectFP, 0, 1, 0.0f);
			}
		}

		/// CHANGE: No longer a debug key.
		/// HELPER KEYIND: [S]  "Toggle sound On/Off."
		if (Shortcut_IsPressed(ShortcutID::ToggleSound)) {

			if (!(legoGlobs.flags1 & GAME1_USESFX)) {
				Lego_SetSoundOn(true);
				SFX_Random_PlaySoundNormal(SFX_AmbientLoop, true);
			}
			else {
				Lego_SetSoundOn(false);
			}

			const char* onOff = ((legoGlobs.flags1 & GAME1_USESFX) ? "ON" : "OFF");
			TextWindow_PrintF(legoGlobs.textOnlyWindow, "\nSound Effects: %s", onOff);
		}

		/// CHANGE: No longer a debug key.
		/// HELPER KEYIND: [M]  "Toggle music On/Off."
		if (Shortcut_IsPressed(ShortcutID::ToggleMusic)) {

			Lego_SetMusicOn(!Lego_IsMusicOn());
			//Lego_SetMusicPlaying(!Lego_IsMusicPlaying());

			const char* onOff = (Lego_IsMusicOn() ? "ON" : "OFF");
			//const char* onOff = (Lego_IsMusicPlaying() ? "ON" : "OFF");
			TextWindow_PrintF(legoGlobs.textOnlyWindow, "\nMusic: %s", onOff);
		}

		/// CHANGE: Switch to normal debug mode *or* edit mode key (we want to hijack edit mode for something actually useful).
		/// EDIT DEBUG KEYBIND: [Numpad 0]  "Toggle unrestricted camera movement."
		if ((Lego_IsAllowDebugKeys() || Lego_IsAllowEditMode()) && Shortcut_IsPressed(ShortcutID::Debug_ToggleFreeCameraMovement)) {
			Camera_EnableFreeMovement(legoGlobs.cameraMain, !Camera_IsFreeMovement(legoGlobs.cameraMain));
			SelectPlace_Hide(legoGlobs.selectPlace, true); // Not sure what this line is doing.
		}

		/// HELPER KEYBIND: [Z]  "Shake the screen."
		if (Lego_IsAllowDebugKeys() && Shortcut_IsPressed(ShortcutID::Debug_ShakeScreen)) {
			Camera_Shake(legoGlobs.cameraMain, 5.0f, 25.0f);
		}
		/// HELPER KEYBIND: [Z]  "Make primary selected unit eat."
		if (Lego_IsAllowDebugKeys() && Shortcut_IsPressed(ShortcutID::Debug_CommandEat)) {
			LegoObject* primaryUnit = Message_GetPrimarySelectedUnit();
			if (primaryUnit != nullptr) {
				primaryUnit->flags1 |= LIVEOBJ1_EATING;
				LegoObject_SetActivity(primaryUnit, Activity_Eat, 0);
				LegoObject_UpdateActivityChange(primaryUnit);
				AITask_DoAnimationWait(primaryUnit);
			}
		}
	}

	bool32 leftButtonReleased = false;
	if (gamectrlGlobs.handleKeysLastLeftMouseButtonDown && !Gods98::mslb()) {
		leftButtonReleased = true;
	}
	
	// This logic seems very wrong...
	// if ((INPUT.Key_Map[57] == false) || (INPUT.prevKey_Map[57] == INPUT.Key_Map[57]))
	/// CONFLICT KEYBIND: [Space]  Space behaviour for skipping phases used in the Rewards screen(?).
	const bool32 spaceKeyPressed = Input_IsKeyPressed(Keys::KEY_SPACE);

	bool32 exitGame = false;
	// This return value is identical to `Objective_IsShowing()`.
	const bool32 isObjectiveShowing = Objective_HandleKeys(spaceKeyPressed, leftButtonReleased, &exitGame);

	if (!isObjectiveShowing && !(legoGlobs.flags1 & GAME1_LEVELENDING) && !(legoGlobs.flags2 & GAME2_INOPTIONSMENU) &&
		!HelpWindow_IsEnabled_AndFlags_3_AndNoTutorialFlags() && !Objective_IsShowing())
	{
		/// KEYBIND: [P]  "Pause/unpause the game."
		if (Shortcut_IsPressed(ShortcutID::TogglePause)) {
			Lego_SetPaused(true, false);
			legoGlobs.flags1 ^= GAME1_PAUSED;
		}

		if (!Gods98::Main_AppActive()) {
			/// FIXME: Store this CFG value for access, so that we can properly fiddle with config constants.
			if (Lego_IsLoseFocusAndPause() && !(legoGlobs.flags1 & GAME1_LEVELENDING)) {
				Lego_SetPaused(false, true);
				//if (!(legoGlobs.flags1 & GAME1_PAUSED)) {
				//	legoGlobs.flags1 |= GAME1_PAUSED;
				//}
				/// SHORTHAND: No need to waste checking.
				legoGlobs.flags1 |= GAME1_PAUSED;
			}
		}
	}

	gamectrlGlobs.handleKeysLastLeftMouseButtonDown = Gods98::mslb();

	if (exitGame) {
		return false;
	}

	/// DEBUG KEYBIND: [Shift]+[5]     "Increase topdown spotlight penumbra (while held)."
	/// DEBUG KEYBIND: (no Shift)+[5]  "Decrease topdown spotlight penumbra (while held)."
	if (Lego_IsAllowDebugKeys() && Shortcut_IsDown(ShortcutID::Debug_IncreaseSpotlightPenumbra)) {
		updateGlobs.currentBaseLightLevel += 0.02f;
		Gods98::Container_Light_SetSpotPenumbra(legoGlobs.rootSpotlight, updateGlobs.currentBaseLightLevel);
	}
	else if (Lego_IsAllowDebugKeys() && Shortcut_IsDown(ShortcutID::Debug_DecreaseSpotlightPenumbra)) {
		updateGlobs.currentBaseLightLevel -= 0.02f;
		Gods98::Container_Light_SetSpotPenumbra(legoGlobs.rootSpotlight, updateGlobs.currentBaseLightLevel);
	}
	/*if (Lego_IsAllowDebugKeys() && Input_IsKeyDown(Keys::KEY_FIVE)) {
		/// FIXME: Change this to elapsed real time, so that high FPS doesn't affect speed.
		if (Input_IsKeyDown(Keys::KEY_LEFTSHIFT)) {
			updateGlobs.currentBaseLightLevel += 0.02f;
		}
		else {
			updateGlobs.currentBaseLightLevel -= 0.02f;
		}
		Gods98::Container_Light_SetSpotPenumbra(legoGlobs.rootSpotlight, updateGlobs.currentBaseLightLevel);
	}*/

	/// DEBUG KEYBIND: [Shift]+[6]     "Increase topdown spotlight umbra (while held)."
	/// DEBUG KEYBIND: (no Shift)+[6]  "Decrease topdown spotlight umbra (while held)."
	if (Lego_IsAllowDebugKeys() && Shortcut_IsDown(ShortcutID::Debug_IncreaseSpotlightUmbra)) {
		gamectrlGlobs.dbgCursorLightLevel += 0.02f;
		Gods98::Container_Light_SetSpotUmbra(legoGlobs.rootSpotlight, gamectrlGlobs.dbgCursorLightLevel);
	}
	else if (Lego_IsAllowDebugKeys() && Shortcut_IsDown(ShortcutID::Debug_DecreaseSpotlightUmbra)) {
		gamectrlGlobs.dbgCursorLightLevel -= 0.02f;
		Gods98::Container_Light_SetSpotUmbra(legoGlobs.rootSpotlight, gamectrlGlobs.dbgCursorLightLevel);
	}
	/*if (Lego_IsAllowDebugKeys() && Input_IsKeyDown(Keys::KEY_SIX)) {
		/// FIXME: Change this to elapsed real time, so that high FPS doesn't affect speed.
		if (Input_IsKeyDown(Keys::KEY_LEFTSHIFT)) {
			gamectrlGlobs.dbgCursorLightLevel += 0.02f;
		}
		else {
			gamectrlGlobs.dbgCursorLightLevel -= 0.02f;
		}
		Gods98::Container_Light_SetSpotUmbra(legoGlobs.rootSpotlight, gamectrlGlobs.dbgCursorLightLevel);
	}*/

	if (Lego_IsAllowDebugKeys() || Gods98::Main_IsDebugComplete()) {

		/// DEBUG KEYBIND: [LCtrl]+[D]  "Instantly fails the current level."
		if (Shortcut_IsPressed(ShortcutID::Debug_LoseLevel)) {
			NERPFunc__SetLevelFail(nullptr);
		}

		/// DEBUG KEYBIND: [LCtrl]+[S]  "Instantly completes the current level."
		if (Shortcut_IsPressed(ShortcutID::Debug_WinLevel)) {
			NERPFunc__SetLevelCompleted(nullptr);
		}

		/// DEBUG KEYBIND: [RCtrl]+[S]  "Instantly fails the current level with reason, too many crystals stolen."
		if (Shortcut_IsPressed(ShortcutID::Debug_LoseLevelCrystals)) {
			Objective_SetStatus(LEVELSTATUS_FAILED_CRYSTALS);
		}

		/// DEBUG KEYBIND: NULL  "Increases oxygen level (while held)."
		/// DEBUG KEYBIND: [O]  "Decreases oxygen level (while held)."
		if (Shortcut_IsDown(ShortcutID::Debug_IncreaseOxygen)) {
			/// FIXME: Change this to elapsed real time, so that high FPS doesn't affect speed.
			legoGlobs.currLevel->oxygenLevel += 1.0f;
		}
		else if (Shortcut_IsDown(ShortcutID::Debug_DecreaseOxygen)) {
			/// FIXME: Change this to elapsed real time, so that high FPS doesn't affect speed.
			legoGlobs.currLevel->oxygenLevel -= 1.0f;
		}
	}

	return true;
}

#pragma endregion
