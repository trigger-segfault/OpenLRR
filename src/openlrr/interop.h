#pragma once

#include "common.h"


constexpr const uint32 PROCESS_EIP       = 0x0048f2c0; // LegoRR.exe!entry
constexpr const uint32 PROCESS_WINMAIN   = 0x00477a60; // LegoRR.exe!WinMain


#pragma region DLL Import Hooks
bool interop_hook_calls_WINMM_timeGetTime(void);
#pragma endregion

#pragma region C Runtime Hooks
bool interop_hook_CRT_rand(void);
#pragma endregion

#pragma region Gods98 Engine Hooks
bool interop_hook_WinMain(void);
bool interop_hook_Gods98_3DSound(void);
bool interop_hook_Gods98_Animation(void);
bool interop_hook_calls_Gods98_AnimClone(void);
bool interop_hook_Gods98_AnimClone(void);
bool interop_hook_Gods98_Bmp(void);
bool interop_hook_Gods98_Compress(void);
bool interop_hook_Gods98_Config(void);
bool interop_hook_Gods98_Containers(void);
bool interop_hook_Gods98_DirectDraw(void);
bool interop_hook_Gods98_Draw(void);
bool interop_hook_Gods98_Dxbug(void);
bool interop_hook_Gods98_Errors(void);
bool interop_hook_Gods98_Files(void);
bool interop_hook_calls_Gods98_Flic(void);
bool interop_hook_Gods98_Flic(void);
bool interop_hook_Gods98_Fonts(void);
bool interop_hook_Gods98_Images(void);
bool interop_hook_Gods98_Input(void);
bool interop_hook_Gods98_Keys(void);
bool interop_hook_Gods98_Lws(void);
bool interop_hook_Gods98_Lwt(void);
bool interop_hook_Gods98_Main(void);
bool interop_hook_Gods98_Materials(void);
bool interop_hook_Gods98_Maths(void);
bool interop_hook_Gods98_Memory(void);
bool interop_hook_Gods98_Mesh(void);
bool interop_hook_Gods98_Movie(void);
bool interop_hook_Gods98_Registry(void);
bool interop_hook_Gods98_Sound(void);
bool interop_hook_Gods98_TextWindow(void);
bool interop_hook_Gods98_Utils(void);
bool interop_hook_Gods98_Viewports(void);
bool interop_hook_Gods98_Wad(void);
bool interop_hook_Gods98_Init(void);
#pragma endregion

#pragma region LegoRR Game Hooks
bool interop_hook_LegoRR_Advisor(void);
bool interop_hook_LegoRR_AITask(void);
bool interop_hook_LegoRR_BezierCurve(void);
bool interop_hook_LegoRR_Bubbles(void);
bool interop_hook_LegoRR_Building(void);
bool interop_hook_LegoRR_Collision(void);
bool interop_hook_LegoRR_Construction(void);
bool interop_hook_LegoRR_Creature(void);
bool interop_hook_LegoRR_Credits(void);
bool interop_hook_LegoRR_DamageText(void);
bool interop_hook_LegoRR_Dependencies(void);
bool interop_hook_LegoRR_Effect(void);
bool interop_hook_LegoRR_ElectricFence(void);
bool interop_hook_LegoRR_Encyclopedia(void);
bool interop_hook_LegoRR_Erosion(void);
bool interop_hook_LegoRR_Fallin(void);
bool interop_hook_LegoRR_FrontEnd(void);
bool interop_hook_LegoRR_Game(void);
bool interop_hook_LegoRR_InfoMessages(void);
bool interop_hook_LegoRR_Interface(void);
bool interop_hook_LegoRR_LegoCamera(void);
bool interop_hook_LegoRR_LightEffects(void);
bool interop_hook_LegoRR_Loader(void);
bool interop_hook_LegoRR_MeshLOD(void);
bool interop_hook_LegoRR_Messages(void);
bool interop_hook_LegoRR_NERPsFile(void);
bool interop_hook_LegoRR_NERPsFunctions(void);
bool interop_hook_LegoRR_Object(void);
bool interop_hook_LegoRR_ObjectRecall(void);
bool interop_hook_LegoRR_Objective(void);
bool interop_hook_LegoRR_ObjInfo(void);
bool interop_hook_LegoRR_Pointers(void);
bool interop_hook_LegoRR_Priorities(void);
bool interop_hook_LegoRR_PTL(void);
bool interop_hook_LegoRR_RadarMap(void);
bool interop_hook_LegoRR_Reward(void);
bool interop_hook_LegoRR_Roof(void);
bool interop_hook_LegoRR_SelectPlace(void);
bool interop_hook_LegoRR_SFX(void);
bool interop_hook_LegoRR_Smoke(void);
bool interop_hook_LegoRR_Stats(void);
bool interop_hook_LegoRR_ToolTip(void);
bool interop_hook_LegoRR_TextMessages(void);
bool interop_hook_LegoRR_Vehicle(void);
bool interop_hook_LegoRR_Water(void);
bool interop_hook_LegoRR_Weapons(void);
#pragma endregion

#pragma region LegoRR Game Special Hooks
bool interop_hook_replace_LegoRR_PanelRadarMapZoom(void);
#pragma endregion

#pragma region Hook All
bool interop_hook_all(void);
#pragma endregion

