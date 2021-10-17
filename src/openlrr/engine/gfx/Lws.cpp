// Lws.cpp : 
//

#include "../../platform/d3drm.h"		// <d3drmwin.h>

#include "../audio/3DSound.h"
#include "../core/Errors.h"
#include "../core/Files.h"
#include "../core/Maths.h"
#include "../core/Memory.h"
#include "../core/Utils.h"
#include "../Graphics.h"
#include "Mesh.h"

#include "Lws.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00541838>
Gods98::Lws_Globs & Gods98::lwsGlobs = *(Gods98::Lws_Globs*)0x00541838;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00486cb0>
Gods98::Lws_Info* __cdecl Gods98::Lws_Parse(const char* fname, bool32 looping)
{
	File* ifp;
	Lws_Info* scene = nullptr;
	char path[FILE_MAXPATH];

#ifdef _DEBUG_2
	Error_Fatal(!lwsGlobs.initialised, "Lws_Initialise() has not been called");
#endif _DEBUG_2

	std::sprintf(path, "%s.%s", fname, LWS_FILESUFFIX);

	if (ifp = File_Open(path, "r")) {

		char line[LWS_MAXLINELEN];
		char lineSplit[LWS_MAXLINELEN];

		File_GetLine(line, sizeof(line), ifp);
		uint16 length = (uint16)std::strlen(line) + 1;
		File_GetLine(&line[length], sizeof(line) - length, ifp);

		if (std::strcmp(line, LWS_HEADER1) == 0 && std::strcmp(&line[length], LWS_HEADER2) == 0) {

			char* argv[LWS_MAXARGS];
			sint32 argc, loop;
			sint32 motionMode = 0, dissolveMode = 0;
			Lws_Node* currNode = nullptr;
			Lws_Node* parent;
			uint16* parentList;
			char* s;
			char* t;

			for (s=t=path ; *s!='\0' ; s++) if (*s == '\\') t = s;
			*(t+(t==path?0:1)) = '\0';

			scene = (Lws_Info*)Mem_Alloc(sizeof(Lws_Info));
			std::memset(scene, 0, sizeof(Lws_Info));
			if (path[0] != '\0') scene->filePath = Util_StrCpy(path);
			else scene->filePath = nullptr;

			scene->nodeListSize = LWS_NODELISTBLOCKSIZE;
			scene->referenceCount = 1;
			if (looping) scene->flags |= LwsFlags::LWS_FLAG_LOOPING;

			scene->nodeList = (Lws_Node*)Mem_Alloc(sizeof(Lws_Node) * scene->nodeListSize);
			parentList = (uint16*)Mem_Alloc(sizeof(uint16) * scene->nodeListSize);

			while (File_GetLine(line, sizeof(line), ifp)){
				std::strcpy(lineSplit, line);
				if (argc = Util_WSTokenise(lineSplit, argv)) {
					if (currNode == nullptr) {
						if (std::strcmp("FirstFrame", argv[0]) == 0) scene->firstFrame = std::atoi(argv[1]);
						if (std::strcmp("LastFrame", argv[0]) == 0) scene->lastFrame = std::atoi(argv[1]);
						if (scene->lastFrame == 0) scene->lastFrame = 1;
						if (std::strcmp("FramesPerSecond", argv[0]) == 0) scene->fps = (real32)std::atof(argv[1]);
						if (std::strcmp("AddNullObject", argv[0]) == 0 || std::strcmp("LoadObject", argv[0]) == 0) {

							if (scene->nodeCount == scene->nodeListSize) {
								scene->nodeListSize += LWS_NODELISTBLOCKSIZE;
								scene->nodeList = (Lws_Node*)Mem_ReAlloc(scene->nodeList, sizeof(Lws_Node) * scene->nodeListSize);
								parentList = (uint16*)Mem_ReAlloc(parentList, sizeof(uint16) * scene->nodeListSize);
							}
							parentList[scene->nodeCount] = 0;
							currNode = &scene->nodeList[scene->nodeCount];
							std::memset(currNode, 0, sizeof(Lws_Node));

							if (std::strcmp("AddNullObject", argv[0]) == 0) {
								currNode->flags |= Lws_NodeFlags::LWSNODE_FLAG_NULL;
								currNode->name = Util_StrCpy(&line[std::strlen("AddNullObject") + 1]);
								if (::_strnicmp(currNode->name, LWS_SOUNDTRIGGERPREFIX, std::strlen(LWS_SOUNDTRIGGERPREFIX)) == 0 &&
									::_strnicmp(&currNode->name[std::strlen(LWS_SOUNDTRIGGERPREFIX)], LWS_SOUNDTRIGGERSEPERATOR, std::strlen(LWS_SOUNDTRIGGERSEPERATOR)) == 0){
									currNode->flags |= Lws_NodeFlags::LWSNODE_FLAG_SOUNDTRIGGER;
									Error_Fatal(scene->triggerCount == 256, "Too many sound trigger frames");
									scene->triggerCount++;
								}

							} else {
								char* lwoStr;
								std::strcpy(lineSplit, line);
								argc = Util_Tokenise(line, argv, "\\");
								if (lwoStr = Util_StrIStr(argv[argc-1], ".lwo")) lwoStr[0] = '\0';
								currNode->name = Util_StrCpy(argv[argc-1]);
							}

							for (loop=scene->nodeCount-1 ; loop>=0 ; loop--) {
								if (((scene->nodeList[loop].flags & Lws_NodeFlags::LWSNODE_FLAG_NULL) && (currNode->flags & Lws_NodeFlags::LWSNODE_FLAG_NULL)) ||
									(!(scene->nodeList[loop].flags & Lws_NodeFlags::LWSNODE_FLAG_NULL) && !(currNode->flags & Lws_NodeFlags::LWSNODE_FLAG_NULL))) {

									if (std::strcmp(currNode->name, scene->nodeList[loop].name) == 0) {
										currNode->reference = scene->nodeList[loop].reference + 1;
										break;
									}
								}
							}

						}
					} else if (motionMode) {

						// argv[1] is the first item as there is some white space at ther start of the line...

						if (motionMode == 1) { Error_Fatal(std::atoi(argv[1]) != 9, "Scene file error");
						} else if (motionMode == 2) {
							currNode->keyCount = std::atoi(argv[1]);
//							currNode->keyPos = (Vector3F*)Mem_Alloc(sizeof(Vector3F) * (currNode->keyCount + 1));
//							currNode->keyHPB = (Vector3F*)Mem_Alloc(sizeof(Vector3F) * (currNode->keyCount + 1));
//							currNode->keyScale = (Vector3F*)Mem_Alloc(sizeof(Vector3F) * (currNode->keyCount + 1));
//							currNode->keyFrame = (uint16*)Mem_Alloc(sizeof(uint16) * (currNode->keyCount + 1));
							currNode->keyList = (Lws_KeyInfo*)Mem_Alloc(sizeof(Lws_KeyInfo) * (currNode->keyCount + 1));
						} else {
							uint16 key = (motionMode - 3) / 2;
							if (key < currNode->keyCount) {
								if (motionMode % 2) {
									currNode->keyList[key].position.x = (real32)std::atof(argv[1]);
									currNode->keyList[key].position.y = (real32)std::atof(argv[2]);
									currNode->keyList[key].position.z = (real32)std::atof(argv[3]);
									currNode->keyList[key].hpb.x = ((real32)std::atof(argv[4]) / 180.0f) * M_PI;
									currNode->keyList[key].hpb.y = ((real32)std::atof(argv[5]) / 180.0f) * M_PI;
									currNode->keyList[key].hpb.z = ((real32)std::atof(argv[6]) / 180.0f) * M_PI;
									currNode->keyList[key].scale.x = (real32)std::atof(argv[7]);
									currNode->keyList[key].scale.y = (real32)std::atof(argv[8]);
									currNode->keyList[key].scale.z = (real32)std::atof(argv[9]);
								} else {
									currNode->keyList[key].frame = std::atoi(argv[1]);
								}
							}
						}

						motionMode++;

						if (std::strcmp(argv[0], "EndBehavior") == 0) motionMode = 0;

					} else if (dissolveMode) {

						// argv[1] is the first item as there is some white space at ther start of the line...

						if (dissolveMode == 1) { Error_Fatal(std::atoi(argv[1]) != 1, "Scene file error");
						} else if (dissolveMode == 2) {
							currNode->dissolveCount = std::atoi(argv[1]);
							currNode->dissolveLevel = (real32*)Mem_Alloc(sizeof(real32) * (currNode->dissolveCount + 1));
							currNode->dissolveFrame = (uint16*)Mem_Alloc(sizeof(uint16) * (currNode->dissolveCount + 1));
						} else {

							uint16 key = (dissolveMode - 3) / 2;
							if (key < currNode->dissolveCount) {
								if (dissolveMode % 2) {
									currNode->dissolveLevel[key] = (real32)std::atof(argv[1]);
									if (currNode->dissolveLevel[key] < 0.0f) currNode->dissolveLevel[key] = 0.0f;
									if (currNode->dissolveLevel[key] > 1.0f) currNode->dissolveLevel[key] = 1.0f;
									currNode->dissolveLevel[key] = 1.0f - currNode->dissolveLevel[key];
								} else currNode->dissolveFrame[key] = std::atoi(argv[1]);
							}
						}

						dissolveMode++;

						if (std::strcmp(argv[0], "EndBehavior") == 0) dissolveMode = 0;

					} else {

						if (std::strcmp(argv[0], "ObjectMotion") == 0) motionMode = 1;
						else if (std::strcmp(argv[0], "ObjDissolve") == 0) {
							if (std::strcmp(argv[1], "(envelope)") == 0) dissolveMode = 1;
							else {
								Error_Fatal(LWS_MAXSTATICDISSOLVES==lwsGlobs.staticDissolveCount, "LWS_MAXSTATICDISSOLVES too small");
								lwsGlobs.staticDissolveLevel[lwsGlobs.staticDissolveCount] = (real32)std::atof(argv[1]);
								currNode->dissolveLevel = &lwsGlobs.staticDissolveLevel[lwsGlobs.staticDissolveCount];
								lwsGlobs.staticDissolveCount++;
								currNode->dissolveLevel = 0;
							}
						} else if (std::strcmp(argv[0], "ParentObject") == 0) parentList[scene->nodeCount] = std::atoi(argv[1]);
						else if (std::strcmp(argv[0], "PivotPoint") == 0) {
							currNode->pivotVector.x = (real32) -std::atof(argv[1]);
							currNode->pivotVector.y = (real32) -std::atof(argv[2]);
							currNode->pivotVector.z = (real32) -std::atof(argv[3]);
						} else if (std::strcmp(argv[0], "UnseenByCamera") == 0 && std::atoi(argv[1]) == 1) {
							currNode->flags |= Lws_NodeFlags::LWSNODE_FLAG_FACECAMERA;
						} else if (std::strcmp(argv[0], "ShadowOptions") == 0) {
							currNode = nullptr;
							scene->nodeCount++;
						}
					}
				}
			}

			// If there is more than one key and the last one is before the end of the animation
			// then create a duplicate of the last one at the end.
			// The arrays are over allocated by one to ensure that there is enough space.

			for (loop=0 ; loop<(sint32)scene->nodeCount ; loop++) {
				currNode = &scene->nodeList[loop];
				if (1 != currNode->keyCount && currNode->keyList[currNode->keyCount-1].frame < scene->lastFrame) {
					currNode->keyList[currNode->keyCount].frame = scene->lastFrame;
					currNode->keyList[currNode->keyCount].position = currNode->keyList[currNode->keyCount-1].position;
					currNode->keyList[currNode->keyCount].hpb = currNode->keyList[currNode->keyCount-1].hpb;
					currNode->keyList[currNode->keyCount].scale = currNode->keyList[currNode->keyCount-1].scale;
					currNode->keyCount++;
				}
			}

			// Do the same for the dissolve level (assume that if an envelope is defined, more than one key
			// will be used)...

			for (loop=0 ; loop<(sint32)scene->nodeCount ; loop++) {
				currNode = &scene->nodeList[loop];
				if (currNode->dissolveCount) {
					if (currNode->dissolveFrame[currNode->dissolveCount-1] < scene->lastFrame) {
						currNode->dissolveFrame[currNode->dissolveCount] = scene->lastFrame;
						currNode->dissolveLevel[currNode->dissolveCount] = currNode->dissolveLevel[currNode->dissolveCount-1];
						currNode->dissolveCount++;
					}
				}
			}

			// Link the hierarchy from the node and parent lists...
			// New links are added to the front of the list, so reverse through the array to preserve the order...

			for (loop=scene->nodeCount-1 ; loop>=0 ; loop--) {
				currNode = &scene->nodeList[loop];
				if (parentList[loop]) {
					parent = &scene->nodeList[parentList[loop] - 1];
					currNode->next = parent->childList;
					parent->childList = currNode;
				} else {
					currNode->next = scene->masterNode;
					scene->masterNode = currNode;
				}
			}

			Mem_Free(parentList);

			Lws_SetupSoundTriggers(scene);

		} else Error_Fatal(true, "Invalid scene file");

		File_Close(ifp);
	}

	return scene;
}

// <LegoRR.exe @00487980>
void __cdecl Gods98::Lws_Initialise(const char* sharedDir, LwsFindSFXIDFunc FindSFXID,
								LwsPlaySample3DFunc PlaySample3D, LwsSoundEnabledFunc SoundEnabled)
{
	if (sharedDir) {
		uint16 len = (uint16)std::strlen(sharedDir);
		if (sharedDir[len-1] != '\\') len++;
		lwsGlobs.sharedDir = (char*)Mem_Alloc(len+1);
		std::strcpy(lwsGlobs.sharedDir, sharedDir);
		lwsGlobs.sharedDir[len-1] = '\\';
		lwsGlobs.sharedDir[len] = '\0';
	}

//	Error_Fatal(FindSFXID && (PlaySFX == nullptr || StopSFX == nullptr), "If FindSFXID() is passed, so should PlaySFX() and StopSFX()");

//	lwsGlobs.FindSFXIDFunc = FindSFXID;
//	lwsGlobs.PlaySFXFunc = PlaySFX;
//	lwsGlobs.StopSFXFunc = StopSFX;

//	lwsGlobs.FindSoundFunc = FindSound;

	lwsGlobs.FindSFXIDFunc = FindSFXID;
	lwsGlobs.PlaySample3DFunc = PlaySample3D;
//	lwsGlobs.GetSoundFunc = GetSound;
	lwsGlobs.SoundEnabledFunc = SoundEnabled;

#ifdef _DEBUG_2
	lwsGlobs.initialised = true;
#endif // _DEBUG_2
}

// <LegoRR.exe @00487a20>
void __cdecl Gods98::Lws_Shutdown(void)
{
	uint16 loop;

	for (loop=0 ; loop<lwsGlobs.meshPathCount; loop++) {
		Lws_MeshPath* mp = &lwsGlobs.meshPathList[loop];
		Mem_Free(mp->path);
	}
	for (loop=0 ; loop<lwsGlobs.meshPathCountShared ; loop++) {
		Lws_MeshPath* mp = &lwsGlobs.meshPathListShared[loop];
		Mem_Free(mp->path);
	}
}

// <LegoRR.exe @00487a90>
uint32 __cdecl Gods98::Lws_GetFrameCount(Lws_Info* scene)
{
	return (uint32)scene->lastFrame; // from uint16 -> uint32
}

// <LegoRR.exe @00487aa0>
void __cdecl Gods98::Lws_SetupSoundTriggers(Lws_Info* scene)
{
	if (lwsGlobs.FindSFXIDFunc) {
//	if (lwsGlobs.FindSoundFunc) {
		Lws_Node* node;
		char line[LWS_MAXLINELEN];
		uint16 loop;
		uint32 argc, index;
		char* argv[256];
		uint8 triggerIndex = 0;
		Lws_SoundTrigger* st;

		if (scene->triggerCount) {
			scene->triggerList = (Lws_SoundTrigger*)Mem_Alloc(sizeof(Lws_SoundTrigger) * scene->triggerCount);
			for (loop=0 ; loop<scene->nodeCount ; loop++) {
				node = &scene->nodeList[loop];
				if (node->flags & Lws_NodeFlags::LWSNODE_FLAG_SOUNDTRIGGER) {
					st = &scene->triggerList[triggerIndex];
					node->triggerIndex = triggerIndex;
					triggerIndex++;
					std::strcpy(line, node->name);
					argc = Util_Tokenise(line, argv, LWS_SOUNDTRIGGERSEPERATOR);
					st->count = (uint16) argc - 2;
					Error_Fatal(st->count >= LWS_MAXTRIGGERKEYS, "LWS_MAXTRIGGERKEYS too small");
					Error_Fatal(st->count == 0, "No trigger frames specified");
					{
						bool32 result = lwsGlobs.FindSFXIDFunc(argv[1], &st->sfxID);
						if (mainGlobs.flags & MainFlags::MAIN_FLAG_REDUCESAMPLES) {
							Error_Warn(!result, Error_Format("Cannot match sound with %s", argv[1]));
							if (!result)
								st->sfxID=0;
						} else {
							Error_Fatal(!result, Error_Format("Cannot match sound with %s", argv[1]));
						}
					}
					for (index=0 ; index<st->count ; index++) {
						char* end = std::strstr(argv[index+2], "-");
						st->frameStartList[index] = std::atoi(argv[index+2]);
						if (end) st->frameEndList[index] = std::atoi(&end[1]);
						else st->frameEndList[index] = st->frameStartList[index];
					}
				}
			}
		}
	} else scene->triggerCount = 0;
}

// <LegoRR.exe @00487c50>
void __cdecl Gods98::Lws_LoadMeshes(Lws_Info* scene, IDirect3DRMFrame3* parent)
{
	// Generate the frame hierarchy and load in the meshes...
	// Must run through the hierarchy, not the array...

	Lws_Node* root;
	uint16 frameCount = 0;

	scene->frameList = (IDirect3DRMFrame3**)Mem_Alloc(sizeof(IDirect3DRMFrame3*) * scene->nodeCount);

	for (root=scene->masterNode ; root ; root=root->next) {
		Lws_CreateFrames(scene, root, parent, &frameCount);
		Lws_LoadNodes(scene, root);
	}
	Lws_SetTime(scene, 0.0f);
}


// <inlined>
/*__inline*/ Gods98::Mesh* __cdecl Gods98::Lws_GetNodeMesh(Lws_Info* scene, Lws_Node* node) {

	IUnknown* iunknown;
	IDirect3DRMUserVisual* uv = nullptr; // dummy init
	Mesh* mesh;
	DWORD count;
	HRESULT r;
	IDirect3DRMFrame3* frame = scene->frameList[node->frameIndex];
	uint32 rc;

	frame->GetVisuals(&count, nullptr);
	Error_Fatal(count != 1, Error_Format("Failed to obtain lwo mesh from lightwave scene.\nLws_GetNodeMesh() - Node name == '%s'", node->name));
	frame->GetVisuals(&count, &iunknown);
	r = iunknown->QueryInterface(Idl::IID_IDirect3DRMUserVisual, (void**)&uv);
	Error_Fatal(r != D3DRM_OK, "Cannot get user visual");
	rc = iunknown->Release();
	mesh = (Mesh*)uv->GetAppData();
	Error_Fatal(mesh == nullptr, "Cannot get mesh");
	rc = uv->Release();

	return mesh;
}


// <LegoRR.exe @00487cc0>
Gods98::Lws_Info* __cdecl Gods98::Lws_Clone(Lws_Info* scene, IDirect3DRMFrame3* parent)
{
	Lws_Info* clone = (Lws_Info*)Mem_Alloc(sizeof(Lws_Info));
	Lws_Node* root;
	Lws_Node* node;
	Mesh* mesh;
	uint16 frameCount = 0, loop;
	IDirect3DRMFrame3* nodeParent;
	Matrix4F mat;

	if (scene->clonedFrom) scene = scene->clonedFrom;

	std::memset(clone, 0, sizeof(Lws_Info));
	*clone = *scene;
	clone->referenceCount = 1;

	clone->frameList = (IDirect3DRMFrame3**)Mem_Alloc(sizeof(IDirect3DRMFrame3*) * clone->nodeCount);

	for (root=clone->masterNode ; root ; root=root->next) {
		Lws_CreateFrames(clone, root, parent, &frameCount);
	}
	for (loop=0 ; loop<clone->nodeCount ; loop++) {
		node = &clone->nodeList[loop];

		// Copy the transformation...
		scene->frameList[loop]->GetParent(&nodeParent);
		scene->frameList[loop]->GetTransform(nodeParent, mat);
		nodeParent->Release();
		clone->frameList[loop]->AddTransform(D3DRMCOMBINE_REPLACE, mat);

		if (!(node->flags & Lws_NodeFlags::LWSNODE_FLAG_NULL)) {
			mesh = Lws_GetNodeMesh(scene, node);
			Mesh_Clone(mesh, clone->frameList[node->frameIndex]);
		}
	}

	clone->clonedFrom = scene;
	scene->referenceCount++;

	Lws_SetTime(clone, 0.0f);
	return clone;
}

// <LegoRR.exe @00487e60>
void __cdecl Gods98::Lws_SetTime(Lws_Info* scene, real32 time)
{
	uint16 prev;
	uint16 loop;
	real32 delta;
	Lws_Node* node;

	scene->lastTime = scene->time;
	if (scene->flags & LwsFlags::LWS_FLAG_LOOPING) scene->time = (real32)std::fmod(time, (real32) scene->lastFrame);
	else if (time > scene->lastFrame) scene->time = scene->lastFrame;
	else scene->time = time;

	for (loop=0 ; loop<scene->nodeCount ; loop++) {
		node = &scene->nodeList[loop];
		if (node->keyCount > 1) {
			delta = Lws_FindPrevKey(node, scene->time, &prev);
			Lws_InterpolateKeys(scene, node, prev, delta);
		}
		if (node->dissolveCount > 1) {
			delta = Lws_FindPrevDissolve(node, scene->time, &prev);
			Lws_InterpolateDissolve(scene, node, prev, delta);
		}
		Lws_AnimateTextures(scene, node);
		Lws_HandleTrigger(scene, node);
	}
}

// <LegoRR.exe @00487f70>
real32 __cdecl Gods98::Lws_FindPrevKey(Lws_Node* node, real32 time, uint16* prev)
{
	uint16 low=0, middle, high=node->keyCount;
	uint16 frame;

	middle = high;

	while (1) {

		middle = (low + high) / 2;
		frame = node->keyList[middle].frame;

		if (middle == low) break;

		if (time < (real32) frame) high = middle;
		else low = middle;
	}

	*prev = middle;
	return Maths_InterpolationDelta((real32) frame, (real32) node->keyList[middle+1].frame, time);
}

// <LegoRR.exe @00488010>
void __cdecl Gods98::Lws_AnimateTextures(Lws_Info* scene, Lws_Node* node)
{
	if (!(node->flags & Lws_NodeFlags::LWSNODE_FLAG_NULL)) {
		Mesh* mesh = Lws_GetNodeMesh(scene, node);
		Mesh_SetTextureTime(mesh, scene->time);
	}
}

// <LegoRR.exe @004880a0>
void __cdecl Gods98::Lws_HandleTrigger(Lws_Info* scene, Lws_Node* node)
{
	if (lwsGlobs.FindSFXIDFunc) {
		if (node->flags & Lws_NodeFlags::LWSNODE_FLAG_SOUNDTRIGGER) {

			uint16 loop;
			bool32 loopMode;
			IDirect3DRMFrame3* frame = scene->frameList[node->frameIndex];
			Lws_SoundTrigger* st = &scene->triggerList[node->triggerIndex];


			for (loop=0 ; loop<st->count ; loop++) {
				loopMode = (st->frameStartList[loop] != st->frameEndList[loop]);
				if (Lws_KeyPassed(scene, st->frameStartList[loop])) {
	//				st->loopUID[loop] = Sound3D_PlayOnFrame(frame, st->sound, loopMode);
	//				if (lwsGlobs.SoundEnabledFunc()) st->loopUID[loop] = Sound3D_PlayOnFrame(frame, lwsGlobs.GetSoundFunc(st->sfxID), loopMode);

					{
						if (lwsGlobs.SoundEnabledFunc()) st->loopUID[loop] = lwsGlobs.PlaySample3DFunc(frame, st->sfxID, loopMode, true, nullptr);
					}
				}
				if (loopMode &&
					Lws_KeyPassed(scene, st->frameEndList[loop])) {
					Sound3D_StopSound(st->loopUID[loop]);
				}
			}
		}
	}
}

// <LegoRR.exe @00488190>
bool32 __cdecl Gods98::Lws_KeyPassed(Lws_Info* scene, uint32 key)
{
	real32 keyTime = (real32) key;
	real32 lastTime = scene->lastTime;
	real32 currTime = scene->time;
	real32 totalTime = (real32) Lws_GetFrameCount(scene);

	real32 maxTime = std::max(lastTime, currTime);
	real32 minTime = std::min(lastTime, currTime);

	if ((maxTime - minTime) / totalTime < 0.5f) {
		if (keyTime <= maxTime && keyTime >= minTime) return true;
	} else {
		if (keyTime >= maxTime || keyTime <= minTime) return true;
	}

	return false;

//	if (scene->lastTime < (real32) key && scene->time >= (real32) key) return true;
//	return false;
}

// <LegoRR.exe @00488280>
real32 __cdecl Gods98::Lws_FindPrevDissolve(Lws_Node* node, real32 time, uint16* prev)
{
	// Assume that there will be a small number of these keys...

	uint16 loop;
	uint16 frame, prevTime;

	for (loop=1 ; loop<node->dissolveCount ; loop++) {
		frame = node->dissolveFrame[loop];
		if (time <= (real32) frame) {
			*prev = loop - 1;
			prevTime = node->dissolveFrame[*prev];
			return Maths_InterpolationDelta((real32) prevTime, (real32) frame, time);
		}
	}

	*prev = node->dissolveCount - 1;
	return 0.0f;
}

// <LegoRR.exe @00488330>
void __cdecl Gods98::Lws_InterpolateDissolve(Lws_Info* scene, Lws_Node* node, uint16 prev, real32 delta)
{
	real32 level;

	if (prev == node->dissolveCount - 1) level = node->dissolveLevel[prev];
	else level = Maths_Interpolate(node->dissolveLevel[prev], node->dissolveLevel[prev+1], delta);

	Lws_SetDissolveLevel(scene, node, level);
}

// <LegoRR.exe @00488390>
void __cdecl Gods98::Lws_SetDissolveLevel(Lws_Info* scene, Lws_Node* node, real32 level)
{
	Mesh* mesh = Lws_GetNodeMesh(scene, node);

	uint32 groupCount = Mesh_GetGroupCount(mesh);
	for (uint32 group=0 ; group<groupCount ; group++) {
		Mesh_SetGroupAlpha(mesh, group, level);
	}
}

// <LegoRR.exe @00488430>
void __cdecl Gods98::Lws_InterpolateKeys(Lws_Info* scene, Lws_Node* node, uint16 key, real32 delta)
{
	Vector3F pos, scale, rotation;

	Maths_Vector3DInterpolate(&pos, &node->keyList[key].position, &node->keyList[key+1].position, delta);
	Maths_Vector3DInterpolate(&scale, &node->keyList[key].scale, &node->keyList[key+1].scale, delta);
	Maths_Vector3DInterpolate(&rotation, &node->keyList[key].hpb, &node->keyList[key+1].hpb, delta);

	Lws_SetupNodeTransform(scene, node, &pos, &rotation, &scale);
}

// <LegoRR.exe @004885a0>
void __cdecl Gods98::Lws_SetupNodeTransform(Lws_Info* scene, Lws_Node* node, const Vector3F* pos, const Vector3F* hpb, const Vector3F* scale)
{
	// Scale the object, rotate then translate...
	// Heading(.x) pitch(.y) and bank(.z) (y, x, z in world coordinates) must be multiplied as
	// bank then heading then pitch...

	IDirect3DRMFrame3* frame = scene->frameList[node->frameIndex];
	D3DRMMATRIX4D m; // same as Matrix4F

	if (node->flags & Lws_NodeFlags::LWSNODE_FLAG_FACECAMERA){

		IDirect3DRMFrame3* parent;
		frame->GetParent(&parent);

		frame->GetTransform(parent, m);
		parent->Release();
		parent = nullptr;
		m[0][0] *= scale->x;
		m[1][1] *= scale->y;
		m[2][2] *= scale->z;
		m[3][0] = pos->x;
		m[3][1] = pos->y;
		m[3][2] = pos->z;
		frame->AddTransform(D3DRMCOMBINE_REPLACE, m);

	} else {
	/*	frame->AddTranslation(D3DRMCOMBINE_REPLACE, node->pivotVector.x, node->pivotVector.y, node->pivotVector.z);
		frame->AddScale(D3DRMCOMBINE_AFTER, scale->x, scale->y, scale->z);
		frame->AddRotation(D3DRMCOMBINE_AFTER, 0.0f, 0.0f, 1.0f, hpb->z);
		frame->AddRotation(D3DRMCOMBINE_AFTER, 1.0f, 0.0f, 0.0f, hpb->y);
		frame->AddRotation(D3DRMCOMBINE_AFTER, 0.0f, 1.0f, 0.0f, hpb->x);
		frame->AddTranslation(D3DRMCOMBINE_AFTER, pos->x, pos->y, pos->z);
	*/
		// Each of the sub terms
		real32 Cx = Maths_Cos(hpb->y);
		real32 Sx = Maths_Sin(hpb->y);
		real32 Cz = Maths_Cos(hpb->z);
		real32 Sz = Maths_Sin(hpb->z);
		real32 Cy = Maths_Cos(hpb->x);
		real32 Sy = Maths_Sin(hpb->x);

	/////////////////////////////////////////////////////////////////////
	// Formula - Unoptimised version
	//	m[0][0] = Cz * Cy + Sz * Sx * Sy;
	//	m[0][1] = Sz * Cx;
	//	m[0][2] = Sz * Sx * Cy - Cz * Sy;
	//	m[0][3] = 0.0f;
	//
	//	m[1][0] = Cz * Sx * Sy - Sz * Cy;
	//	m[1][1] = Cz * Cx;
	//	m[1][2] = Sz * Sy + Cz * Sx * Cy;
	//	m[1][3] = 0.0f;
	//
	//	m[2][0] = Cx * Sy;
	//	m[2][1] = -Sx;
	//	m[2][2] = Cx * Cy;
	//	m[2][3] = 0.0f;
	//
	//	m[3][0] = k->posx;
	//	m[3][1] = k->posy;
	//	m[3][2] = k->posz;
	//	m[3][3] = 1.0f;
	///////////////////////////////////////////////////////////////////
	/*
		// (Slightly) optimised sub terms - dont know if this is really worth it.
		REAL SxSy = Sx * Sy;
		REAL CzCy = Cz * Cy;

		m[0][0] =  ( ( CzCy ) + ( Sz * SxSy ) ) * scale->x;
		m[0][1] =  Sz * Cx * scale->x;
		m[0][2] =  ( ( Sz * Sx * Cy) - (Cz * Sy) ) * scale->x;
		m[0][3] =  0.0f;

		m[1][0] =  ( ( Cz * SxSy) - ( Sz * Cy) ) * scale->y;
		m[1][1] =  Cz * Cx * scale->y;
		m[1][2] =  ( ( Sz * Sy ) + ( CzCy * Sx) ) * scale->y;
		m[1][3] =  0.0f;

		m[2][0] =  Cx * Sy * scale->z;
		m[2][1] = -Sx * scale->z;
		m[2][2] =  Cx * Cy * scale->z;
		m[2][3] =  0.0f;

		m[3][0] =  pos->x;
		m[3][1] =  pos->y;
		m[3][2] =  pos->z;
		m[3][3] =  1.0f;

		frame->lpVtbl->AddTranslation(frame, D3DRMCOMBINE_REPLACE, node->pivotVector.x, node->pivotVector.y, node->pivotVector.z);
	*/
		real32 pVx = node->pivotVector.x;
		real32 pVy = node->pivotVector.y;
		real32 pVz = node->pivotVector.z;

		m[0][0] = (((((1.0f * scale->x) *  Cz) * 1.0f) *  Cy) + ((((1.0f * scale->x) *  Sz) *  Sx) *  Sy) * 1.0f);
		m[0][1] = (((((1.0f * scale->x) *  Sz) *  Cx) * 1.0f) * 1.0f);
		m[0][2] = (((((1.0f * scale->x) *  Cz) * 1.0f) * -Sy) + ((((1.0f * scale->x) *  Sz) *  Sx) *  Cy) * 1.0f);
		m[0][3] = 0.0f;
		m[1][0] = (((((1.0f * scale->y) * -Sz) * 1.0f) *  Cy) + ((((1.0f * scale->y) *  Cz) *  Sx) *  Sy) * 1.0f);
		m[1][1] = (((((1.0f * scale->y) *  Cz) *  Cx) * 1.0f) * 1.0f);
		m[1][2] = (((((1.0f * scale->y) * -Sz) * 1.0f) * -Sy) + ((((1.0f * scale->y) *  Cz) *  Sx) *  Cy) * 1.0f);
		m[1][3] = 0.0f;
		m[2][0] = (((((1.0f * scale->z) * 1.0f) *  Cx) *  Sy) * 1.0f);
		m[2][1] = (((((1.0f * scale->z) * 1.0f) * -Sx) * 1.0f) * 1.0f);
		m[2][2] = (((((1.0f * scale->z) * 1.0f) *  Cx) *  Cy) * 1.0f);
		m[2][3] = 0.0f;
		m[3][0] = (m[0][0] * pVx) + (m[1][0] * pVy) + (m[2][0] * pVz) + pos->x;
		m[3][1] = (m[0][1] * pVx) + (m[1][1] * pVy) + (m[2][1] * pVz) + pos->y;
		m[3][2] = (m[0][2] * pVx) + (m[1][2] * pVy) + (m[2][2] * pVz) + pos->z;
		m[3][3] = 1.0f;

		frame->AddTransform(D3DRMCOMBINE_REPLACE, m);
	}
}

// <LegoRR.exe @00488880>
Gods98::Mesh* __cdecl Gods98::Lws_LoadMesh(const char* baseDir, const char* fname, IDirect3DRMFrame3* frame, bool32 noTextures)
{
	char path[FILE_MAXPATH];
	Mesh* mesh;

	if (baseDir) std::sprintf(path, "%s%s", baseDir, fname);
	else std::strcpy(path, fname);

	if (mesh = Lws_SearchMeshPathList(lwsGlobs.meshPathList, lwsGlobs.meshPathCount, path)) {
		Mesh_Clone(mesh, frame);
		return mesh;
	} else if (mesh = Mesh_Load(path, frame, noTextures)) {
		Lws_AddMeshPathEntry(lwsGlobs.meshPathList, &lwsGlobs.meshPathCount, path, mesh);
		return mesh;
	} else if (lwsGlobs.sharedDir) {

		std::sprintf(path, "%s%s", lwsGlobs.sharedDir, fname);

		if (mesh = Lws_SearchMeshPathList(lwsGlobs.meshPathListShared, lwsGlobs.meshPathCountShared, fname)) {
			Mesh_Clone(mesh, frame);
			return mesh;
		} else if (mesh = Mesh_Load(path, frame, noTextures)) {
			Lws_AddMeshPathEntry(lwsGlobs.meshPathListShared, &lwsGlobs.meshPathCountShared, fname, mesh);
			return mesh;
		}
	}

	Error_Warn(true, Error_Format("Cannot find or load mesh >(%s\\)%s<", baseDir, fname));

	return nullptr;
}

// <LegoRR.exe @004889f0>
Gods98::Mesh* __cdecl Gods98::Lws_SearchMeshPathList(Lws_MeshPath* list, uint32 count, const char* path)
{
	uint16 loop;

	for (loop=0 ; loop<count ; loop++) {
		if (::_stricmp(path, list[loop].path) == 0) return list[loop].mesh;
	}

	return nullptr;
}

// <LegoRR.exe @00488a50>
void __cdecl Gods98::Lws_AddMeshPathEntry(Lws_MeshPath* list, IN OUT uint32* count, const char* path, Mesh* mesh)
{
	Error_Fatal(*count==LWS_MAXMESHFILES, "LWS_MAXMESHFILES too small");

	list[*count].path = Util_StrCpy(path);
	list[*count].mesh = mesh;
	(*count)++;
}

// <LegoRR.exe @00488a80>
void __cdecl Gods98::Lws_CreateFrames(Lws_Info* scene, Lws_Node* node, IDirect3DRMFrame3* parent, IN OUT uint16* frameCount)
{
	IDirect3DRMFrame3* frame;
	Lws_Node* child;

	lpD3DRM()->CreateFrame(parent, &frame);

	if (node->flags & Lws_NodeFlags::LWSNODE_FLAG_FACECAMERA) {
		D3DRMMATRIX4D m; // same as Matrix4F
		std::memset(m, 0, sizeof(D3DRMMATRIX4D));
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
		m[3][0] = m[3][1] = m[3][2] = 0.0f;
		frame->AddTransform(D3DRMCOMBINE_REPLACE, m);
	}

	scene->frameList[*frameCount] = frame;
	node->frameIndex = *frameCount;
	(*frameCount)++;

	{
		char name[1024];
		char* s;
		std::sprintf(name, "%s_%0.2i", node->name, node->reference);
		for (s=name ; '\0'!=*s ; s++) if (!std::isprint(*s)) *s = '_';
		frame->SetName(name);
	}

	for (child=node->childList ; child ; child=child->next) {
		Lws_CreateFrames(scene, child, frame, frameCount);
	}
}

// <LegoRR.exe @00488bc0>
void __cdecl Gods98::Lws_LoadNodes(Lws_Info* scene, Lws_Node* node)
{
	IDirect3DRMFrame3* frame;
	Lws_Node* child;
	Mesh* mesh;

	frame = scene->frameList[node->frameIndex];

	// If there is only one keyframe, then set it now...
	if (node->keyCount == 1) Lws_SetAbsoluteKey(scene, node, 0);

	if (!(node->flags & Lws_NodeFlags::LWSNODE_FLAG_NULL)) {

		if (mesh = Lws_LoadMesh(scene->filePath, node->name, frame, false)) {
			if (node->flags & Lws_NodeFlags::LWSNODE_FLAG_FACECAMERA) mesh->flags |= MeshFlags::MESH_FLAG_FACECAMERA;
			if (node->dissolveLevel != nullptr && node->dissolveCount == 0) {
				Lws_SetDissolveLevel(scene, node, node->dissolveLevel[0]);
				node->dissolveLevel = nullptr;
			}
		} else Error_Fatal(true, Error_Format("Missing object file in lightwave scene \"%s\"", node->name));

/*		IDirect3DRMMeshBuilder3* testMesh;
		IDirect3DRMMesh* mesh;
		char path[256];

		std::sprintf(path, "%s.x", node->name);
	
		lpD3DRM()->CreateMeshBuilder(&testMesh);
		if (testMesh->Load(path, nullptr, D3DRMLOAD_FROMFILE, nullptr, nullptr) == D3DRM_OK) {
			testMesh->CreateMesh(&mesh);
			frame->AddVisual((IUnknown*)mesh);
		}// else Error_Fatal(true, "Cannot find file");
		testMesh->Release();
*/	}

	for (child=node->childList ; child ; child=child->next) {
		Lws_LoadNodes(scene, child);
	}
}

// <LegoRR.exe @00488c60>
void __cdecl Gods98::Lws_SetAbsoluteKey(Lws_Info* scene, Lws_Node* node, uint16 key)
{
	Lws_SetupNodeTransform(scene, node, &node->keyList[key].position, &node->keyList[key].hpb, &node->keyList[key].scale);
}

// <LegoRR.exe @00488c90>
void __cdecl Gods98::Lws_Free(Lws_Info* scene)
{
	Lws_Node* node;
	Lws_Info* clonedFrom;
	uint16 loop;

	clonedFrom = scene->clonedFrom;
	scene->referenceCount--;

	if (scene->referenceCount == 0) {
		for (loop=0 ; loop<scene->nodeCount ; loop++) {
			node = &scene->nodeList[loop];
			Lws_FreeNode(scene, node);
		}

		if (scene->clonedFrom == nullptr) {
			Mem_Free(scene->nodeList);
			Mem_Free(scene->filePath);
			if (scene->triggerCount) Mem_Free(scene->triggerList);
		}

		Mem_Free(scene->frameList);
		Mem_Free(scene);
	}

	if (clonedFrom) Lws_Free(clonedFrom);
}

// <LegoRR.exe @00488d30>
void __cdecl Gods98::Lws_FreeNode(Lws_Info* scene, Lws_Node* node)
{
	if (!(node->flags & Lws_NodeFlags::LWSNODE_FLAG_NULL)) {
		Mesh_Remove(Lws_GetNodeMesh(scene, node), scene->frameList[node->frameIndex]);
	}

	if (scene->clonedFrom == nullptr && scene->referenceCount == 0) {
		Mem_Free(node->name);
		Mem_Free(node->keyList);
		if (node->dissolveCount) {
			Mem_Free(node->dissolveLevel);
			Mem_Free(node->dissolveFrame);
		}
	}
}

#pragma endregion
