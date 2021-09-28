// Containers.cpp : 
//

//#define CONTAINER_MATCHHIDDENHIERARCHY
//#define CONTAINER_DONTFLIPTEXTURES

#define CONTAINER_DISABLEFRAMESETTINGS

#include "../platform/d3drm.h"
#include "../legacy.h"

#ifdef LEGO_IMMEDIATEMODEMAP
#define CONTAINER_USEOWNTEXTURELOAD
#endif //LEGO_IMMEDIATEMODEMAP

#include "3DSound.h"
#include "Activities.h"    // defines
#include "AnimClone.h"
#include "Bmp.h"
#include "Config.h"
#include "DirectDraw.h"
#include "Errors.h"
#include "Files.h"
#include "Lws.h"
#include "Main.h"
#include "Memory.h"
#include "Mesh.h"
#include "Utils.h"

#include "Containers.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00506400>
char (& Gods98::s_FormatPartName_name)[1024] = *(char(*)[1024])0x00506400;

// <LegoRR.exe @0076bd80>
Gods98::Container_Globs & Gods98::containerGlobs = *(Gods98::Container_Globs*)0x0076bd80;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <inlined>
/*__inline*/ uint32 __cdecl Gods98::Container_GetRGBAColour(real32 r, real32 g, real32 b, real32 a)
{
	if (r > 1.0f) r = 1.0f;
	if (g > 1.0f) g = 1.0f;
	if (b > 1.0f) b = 1.0f;
	if (a > 1.0f) a = 1.0f;
	if (r < 0.0f) r = 0.0f;
	if (g < 0.0f) g = 0.0f;
	if (b < 0.0f) b = 0.0f;
	if (a < 0.0f) a = 0.0f;

	return D3DRGBA(r, g, b, a);
}

// <inlined>
/*__inline*/ uint32 __cdecl Gods98::Container_GetRGBColour(real32 r, real32 g, real32 b)
{
	return Container_GetRGBAColour(r, g, b, 1.0f);
}



// <LegoRR.exe @004729d0>
Gods98::Container* __cdecl Gods98::Container_Initialise(const char* gameName)
{
	if (containerGlobs.flags & Container_GlobFlags::CONTAINER_FLAG_INITIALISED) Error_Fatal(true, "Containers already initialised");

	std::memset(&containerGlobs, 0, sizeof(Container_Globs));

	for (uint32 loop = 0; loop < CONTAINER_MAXLISTS; loop++) {
		containerGlobs.listSet[loop] = nullptr;
	}

	containerGlobs.typeName[Container_Type::Container_Null] = CONTAINER_NULLSTRING;
	containerGlobs.typeName[Container_Type::Container_Mesh] = CONTAINER_MESHSTRING;
	containerGlobs.typeName[Container_Type::Container_Frame] = CONTAINER_FRAMESTRING;
	containerGlobs.typeName[Container_Type::Container_Anim] = CONTAINER_ANIMSTRING;
	containerGlobs.typeName[Container_Type::Container_LWS] = CONTAINER_LWSSTRING;
#pragma message ( "Are all 'Container_LWO' functions written?" )
	containerGlobs.typeName[Container_Type::Container_LWO] = CONTAINER_LWOSTRING;
	containerGlobs.typeName[Container_Type::Container_FromActivity] = CONTAINER_ACTIVITYSTRING;
	containerGlobs.typeName[Container_Type::Container_Light] = nullptr;

	containerGlobs.extensionName[Container_Type::Container_Null] = "";
	containerGlobs.extensionName[Container_Type::Container_Mesh] = "x";
	containerGlobs.extensionName[Container_Type::Container_Frame] = "x";
	containerGlobs.extensionName[Container_Type::Container_Anim] = "x";
	containerGlobs.extensionName[Container_Type::Container_FromActivity] = ACTIVITY_FILESUFFIX;
	containerGlobs.extensionName[Container_Type::Container_Light] = "";

	containerGlobs.gameName = gameName;
	containerGlobs.freeList = nullptr;
	containerGlobs.listCount = 0;
	containerGlobs.flags = Container_GlobFlags::CONTAINER_FLAG_INITIALISED;

	containerGlobs.textureCount = 0;
	containerGlobs.sharedDir = nullptr;

	Container* root;
	containerGlobs.rootContainer = root = Container_Create(nullptr);

	root->masterFrame->SetSortMode(D3DRMSORT_NONE);

	/*
	#ifdef _DEBUG_2
		Container_Frame_FormatName(root->masterFrame, "Container Root Master Frame for %s", gameName);
	#endif // _DEBUG_2
	*/
	// Debuggy
//	scene->AddChild(CDF(root->masterFrame));

	return root;
}

// <LegoRR.exe @00472ac0>
void __cdecl Gods98::Container_Shutdown(void)
{
	uint32 unfreed = 0;

	Container_DebugCheckOK(CONTAINER_DEBUG_NOTREQUIRED);

	for (uint32 loop = 0; loop < CONTAINER_MAXLISTS; loop++) {

		if (containerGlobs.listSet[loop]) {

			uint32 count = 0x00000001 << loop;

			for (uint32 sub = 0; sub < count; sub++) {
				Container* testCont;
				if (testCont = &containerGlobs.listSet[loop][sub]) {
					if (testCont->nextFree == testCont) {
						Error_Debug(Error_Format("Warning: Unfreed Container type #%d\n", testCont->type));
						unfreed++;
						Container_Remove2(testCont, true);
					}
				}
			}

			Mem_Free(containerGlobs.listSet[loop]);
		}
	}

	containerGlobs.freeList = nullptr;
	containerGlobs.flags = Container_GlobFlags::CONTAINER_FLAG_NONE;

	for (uint32 loop = 0; loop < containerGlobs.textureCount; loop++) {
		if (containerGlobs.textureSet[loop].fileName) {
			Error_Debug(Error_Format("Texture %s was not removed\n", containerGlobs.textureSet[loop].fileName));
			Mem_Free(containerGlobs.textureSet[loop].fileName);
		}
	}

	if (containerGlobs.sharedDir)
		Mem_Free(containerGlobs.sharedDir);

#ifdef _DEBUG_2
	if (unfreed) Error_Debug(Error_Format("Warning: %d Unremoved Container%s\n", unfreed, unfreed == 1 ? "" : "s"));
#endif // _DEBUG_2

#ifdef _HIERARCHY_DEBUG
	Container_Debug_DumpCreationInfo();
#endif // _HIERARCHY_DEBUG
}

// <LegoRR.exe @00472b80>
void __cdecl Gods98::Container_SetSharedTextureDirectory(const char* path)
{
	containerGlobs.sharedDir = Util_StrCpy(path);
}

// <LegoRR.exe @00472ba0>
void __cdecl Gods98::Container_EnableSoundTriggers(bool32 on)
{
	if (on) containerGlobs.flags |= Container_GlobFlags::CONTAINER_FLAG_TRIGGERENABLED;
	else containerGlobs.flags &= ~Container_GlobFlags::CONTAINER_FLAG_TRIGGERENABLED;
}

// <LegoRR.exe @00472bc0>
void __cdecl Gods98::Container_SetTriggerFrameCallback(ContainerTriggerFrameCallback Callback, void* data)
{
	containerGlobs.triggerFrameCallback = Callback;
	containerGlobs.triggerFrameData = data;
}

// <LegoRR.exe @00472be0>
void __cdecl Gods98::Container_SetSoundTriggerCallback(ContainerSoundTriggerCallback Callback, void* data)
{
	Container_DebugCheckOK(CONTAINER_DEBUG_NOTREQUIRED);

	containerGlobs.soundTriggerCallback = Callback;
	containerGlobs.soundTriggerData = data;
	containerGlobs.flags |= Container_GlobFlags::CONTAINER_FLAG_TRIGGERENABLED;
}

// <LegoRR.exe @00472c00>
Gods98::Container* __cdecl Gods98::Container_GetRoot(void)
{
	return containerGlobs.rootContainer;
}

// <LegoRR.exe @00472c10>
Gods98::Container* __cdecl Gods98::Container_Create(Container* parent)
{
	IDirect3DRMFrame3* parentFrame = nullptr, *hiddenParentFrame = nullptr;

	Container_DebugCheckOK(CONTAINER_DEBUG_NOTREQUIRED);

	if (parent) {
		parentFrame = parent->masterFrame;
		hiddenParentFrame = parent->hiddenFrame;
	}

	if (containerGlobs.freeList == nullptr) Container_AddList();

	Container* newContainer = containerGlobs.freeList;
	containerGlobs.freeList = newContainer->nextFree;
	std::memset(newContainer, 0, sizeof(Container));
	newContainer->nextFree = newContainer;

	// Container Initialisation

/*	newContainer->masterFrame = nullptr;
	newContainer->activityFrame = nullptr;
	newContainer->hiddenFrame = nullptr;
	newContainer->cloneData = nullptr;
	newContainer->typeData = nullptr;
	newContainer->type = Container_Type::Container_Null;
	newContainer->flags = ContainerFlags::CONTAINER_FLAG_NONE; //0x00000000;
	newContainer->activityCallback = nullptr;
	newContainer->activityCallbackData = nullptr;
	newContainer->userData = nullptr;
*/

	if (lpD3DRM()->CreateFrame(parentFrame, &newContainer->masterFrame) == D3DRM_OK) {
		Container_NoteCreation(newContainer->masterFrame);
		if (lpD3DRM()->CreateFrame(newContainer->masterFrame, &newContainer->activityFrame) == D3DRM_OK) {
			Container_NoteCreation(newContainer->activityFrame);
			if (lpD3DRM()->CreateFrame(hiddenParentFrame, &newContainer->hiddenFrame) == D3DRM_OK) {
				Container_NoteCreation(newContainer->hiddenFrame);

				// These frames should all appear as the same part of the heirarchy
				// so set them up as all having newContainer as being their owner...
				// Getting the children of the Container will be tricky...

				Container_Frame_SetAppData(newContainer->masterFrame, newContainer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
				Container_Frame_SetAppData(newContainer->activityFrame, newContainer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
				Container_Frame_SetAppData(newContainer->hiddenFrame, newContainer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

				return newContainer;
			}
		}
	}

	Error_Fatal(true, "Unable to Create Container Frames");
	Container_Remove(newContainer);
	return nullptr;
}

// <LegoRR.exe @00472d00>
void __cdecl Gods98::Container_Remove(Container* dead)
{
	Container_Remove2(dead, false);
}

// <LegoRR.exe @00472d10>
void __cdecl Gods98::Container_Remove2(Container* dead, bool32 kill)
{
//	IDirect3DRMFrame* tempFrame1;
	IDirect3DRMFrame3* parentFrame;
	HRESULT r;

	Container_DebugCheckOK(dead);

	Error_Fatal(!dead, "NULL passed to Container_Remove()");

/*	{
		for (uint32 loop=0 ; loop<containerGlobs.rotationCount ; loop++) {
			Container_RotationData* rot = &containerGlobs.rotationList[loop];
			if (dead == rot->container) rot->container = nullptr;
		}
	}*/

	if (dead->type != Container_Type::Container_Reference) Container_SetParent(dead, nullptr);		// Unparent it first...

	if (dead->type == Container_Type::Container_FromActivity || dead->type == Container_Type::Container_Anim){

		if (dead->cloneData) {
			if (kill){
				if (dead->cloneData->cloneTable) Mem_Free(dead->cloneData->cloneTable);
				Mem_Free(dead->cloneData);
			} else {
				dead->cloneData->used = false;
				return;
			}
		}
	}

	// The Animation sets for the activity type object are not stored in the
	// typeData (as there are is an unlimited/unordered number of them) therefore
	// they are stored in the frames AppData section... So release them...

	if (dead->type == Container_Type::Container_FromActivity){
//		IDirect3DRMAnimationSet** animSetList;
		AnimClone** animCloneList;
		IDirect3DRMFrame3** frameList;

		uint32 count = Container_GetActivities(dead, nullptr, nullptr, nullptr);

		// Allocate two temporary buffers to be filled in with the list of
		// animation sets and frames (one for each activity)...
//		animSetList = (IDirect3DRMAnimationSet**)Mem_Alloc(sizeof(IDirect3DRMAnimationSet*) * count);
		animCloneList = (AnimClone**)Mem_Alloc(sizeof(AnimClone*) * count);
		frameList = (IDirect3DRMFrame3**)Mem_Alloc(sizeof(IDirect3DRMFrame3*) * count);
		Container_GetActivities(dead, frameList, animCloneList, nullptr);

		for (uint32 loop=0 ; loop<count ; loop++){

			// Release the animation set,
//			r = animSetList[loop]->lpVtbl->Release(animSetList[loop]);
			AnimClone_Remove(animCloneList[loop]);
			// and free the string defining the frame's name,
			Container_Frame_FreeName(frameList[loop]);
			// Remove the appdata...
			Container_Frame_RemoveAppData(frameList[loop]);
			// Then release our reference to the frame...
			r = frameList[loop]->Release();
		}

		// Free the temporary buffers...
		Mem_Free(animCloneList);
		Mem_Free(frameList);

	} else if (dead->type == Container_Type::Container_Mesh){
		
		Mesh* transmesh;

		if (dead->typeData){
	
			if (transmesh = dead->typeData->transMesh)
				Mesh_Remove(dead->typeData->transMesh, dead->activityFrame);
			else {
				IDirect3DRMMesh* mesh = dead->typeData->mesh;
				// Free the seperate mesh groups if used...
				Container_MeshAppData* appData;
				if (appData = (Container_MeshAppData*)mesh->GetAppData()){

					for (uint32 loop=0 ; loop<appData->usedCount ; loop++) {
						IDirect3DRMMesh* subMesh = appData->meshList[loop];
						dead->activityFrame->DeleteVisual((IUnknown*)subMesh);
						r = subMesh->Release();
					}
					// Root mesh gets removed in Container_FreeTypeData()...

					Mem_Free(appData->meshList);
					Mem_Free(appData);
				}
			}

		}
	} else if (dead->type == Container_Type::Container_Anim){
		
//		IDirect3DRMAnimationSet* animSet = Container_Frame_GetAnimSet(dead->activityFrame);
//		r = animSet->Release();
		AnimClone* animClone;
		if (animClone = Container_Frame_GetAnimClone(dead->activityFrame)){
			AnimClone_Remove(animClone);
		}
	}

#ifdef _DEBUG_2
	if (dead->type != Container_Type::Container_Reference) {
		Container_Frame_FreeName(dead->masterFrame);
		Container_Frame_FreeName(dead->activityFrame);
	}
	Container_Frame_FreeName(dead->hiddenFrame);
#endif // _DEBUG_2

	// Remove the typeData from the container and the AppData from the frames...
	Container_FreeTypeData(dead);
	if (dead->type != Container_Type::Container_Reference || !(dead->flags & ContainerFlags::CONTAINER_FLAG_DEADREFERENCE)) Container_Frame_RemoveAppData(dead->masterFrame);
	if (dead->type != Container_Type::Container_Reference) Container_Frame_RemoveAppData(dead->activityFrame);
	Container_Frame_RemoveAppData(dead->hiddenFrame);

	// Remove the frames and all unreferenced decendants from the hierachy...
	

		// Special case... Don't remove the masterFrame if this is a reference Container.
//		dead->activityFrame->GetParent(&parentFrame);
//		if (parentFrame) {
//			parentFrame->DeleteChild(dead->activityFrame);
//			parentFrame->Release();
//		}
//		r = dead->activityFrame->Release();

	if (dead->type == Container_Type::Container_Reference){

		dead->masterFrame->GetParent(&parentFrame);
//		parentFrame = CUF(tempFrame1);
//		if (tempFrame1) tempFrame1->Release();
		if (parentFrame) {
			parentFrame->DeleteChild(dead->masterFrame);
			parentFrame->Release();
		}
		r = dead->masterFrame->Release();
		r = dead->activityFrame->Release();

	//	containerGlobs.rootFrame->DeleteChild(CDF(dead->hiddenFrame));
		dead->hiddenFrame->GetParent(&parentFrame);
	//	parentFrame = CUF(tempFrame1);
		if (parentFrame) {
			parentFrame->DeleteChild(dead->hiddenFrame);
			parentFrame->Release();
		}
		r = dead->hiddenFrame->Release();
	} else {
		dead->hiddenFrame->Release();
	}

	// Trash the container structure...
	Mem_DebugTrash(dead, CONTAINER_TRASHVALUE, sizeof(Container));

	// Link the freed Container in at the begining of the free list so it may be re-used...
	dead->nextFree = containerGlobs.freeList;
	containerGlobs.freeList = dead;
}

// <LegoRR.exe @00472f90>
Gods98::Container* __cdecl Gods98::Container_Load(Container* parent, const char* filename, const char* typestr, bool32 looping)
{
	Config* rootConf;
	const Config* conf;
	char tempString[UTIL_DEFSTRINGLEN];
	char name[UTIL_DEFSTRINGLEN], baseDir[UTIL_DEFSTRINGLEN];
	Container* cont = nullptr;
	real32 scale;
	bool32 noTexture;

	Container_DebugCheckOK(CONTAINER_DEBUG_NOTREQUIRED);

	Container_Type type = Container_ParseTypeString(typestr, &noTexture);

	std::strcpy(name, filename);

	if (type == Container_Type::Container_FromActivity) {

		char* argv[20];
		uint32 argc;

		Error_Fatal(!containerGlobs.gameName, "Need to set game name in Container_Initialise() in order to load from Activity File");

		// The name of the 'FromActivity' container defines the directory
		// in which all of the files will be located.

		std::strcpy(baseDir, name);
		argc = Util_Tokenise(name, argv, "\\");
		std::sprintf(tempString, "%s\\%s.%s", baseDir, argv[argc - 1], containerGlobs.extensionName[Container_Type::Container_FromActivity]);

		Error_Debug(Error_Format("Loading activity file \"%s\"\n", tempString));

		if (rootConf = Config_Load(tempString)) {

			char tempString2[UTIL_DEFSTRINGLEN];

			std::sprintf(tempString, "%s%s%s", containerGlobs.gameName, CONFIG_SEPARATOR, ACTIVITY_ACTIVITYSTRING);
			Error_Debug(Error_Format("Searching for \"%s\" in activity file\n", tempString));
			if (conf = Config_FindArray(rootConf, tempString)) {

				cont = Container_Create(parent);
				cont->type = type;

				while (conf) {

					const char* fileStr, *sampleStr;
					real32 transCo;
					uint32 trigger;
					bool32 lws = false, looping;
					const char* itemName = conf->itemName;
					//					lpSound sample;

					if ((*itemName != '!') || !(mainGlobs.flags & MainFlags::MAIN_FLAG_REDUCEANIMATION)) {
						if (*itemName == '!') itemName++;

						std::sprintf(tempString2, "%s%s%s%s%s", containerGlobs.gameName, CONFIG_SEPARATOR, conf->dataString, CONFIG_SEPARATOR, ACTIVITY_FILESTRING);

						if (fileStr = Config_GetTempStringValue(rootConf, tempString2)) {
							std::sprintf(tempString, "%s\\%s", baseDir, fileStr);
							std::sprintf(tempString2, "%s%s%s%s%s", containerGlobs.gameName, CONFIG_SEPARATOR, conf->dataString, CONFIG_SEPARATOR, ACTIVITY_TRANSCOSTRING);
							//						if (Config_FindItem(rootConf, tempString2)){
							transCo = Config_GetRealValue(rootConf, tempString2);
							std::sprintf(tempString2, "%s%s%s%s%s", containerGlobs.gameName, CONFIG_SEPARATOR, conf->dataString, CONFIG_SEPARATOR, ACTIVITY_TRIGGERSTRING);
							trigger = Config_GetIntValue(rootConf, tempString2);
							std::sprintf(tempString2, "%s%s%s%s%s", containerGlobs.gameName, CONFIG_SEPARATOR, conf->dataString, CONFIG_SEPARATOR, "LWSFILE");
							lws = BoolTri::BOOL3_TRUE == Config_GetBoolValue(rootConf, tempString2);
							std::sprintf(tempString2, "%s%s%s%s%s", containerGlobs.gameName, CONFIG_SEPARATOR, conf->dataString, CONFIG_SEPARATOR, "LOOPING");
							if (0 == Config_GetBoolValue(rootConf, tempString2)) looping = FALSE;
							else looping = TRUE;

							// Get the activity sample...

							sprintf(tempString2, "%s%s%s%s%s", containerGlobs.gameName, CONFIG_SEPARATOR, conf->dataString, CONFIG_SEPARATOR, ACTIVITY_SAMPLESTRING);
							//							if (
							sampleStr = Config_GetStringValue(rootConf, tempString2);
							//							){
							//								sprintf(tempString2, "%s\\%s", baseDir, fileStr);
							//								if ((sample = Sound_Load(tempString2)) == nullptr) Error_Fatal(TRUE, "Cannot load sample");
							//							} else sample = nullptr;

							if (Container_AddActivity(cont, tempString, itemName, transCo, trigger, sampleStr, nullptr, lws, looping)) {

								if (!cont->typeData) Container_SetActivity(cont, itemName);

							}
							else Error_Fatal(true, Error_Format("Unable to load Activity \"%s\" from \"%s\"", itemName, tempString));

							//						} else Error_Fatal(TRUE, Error_Format("Cannot get \"%s\" value from activity file", tempString2));
						}
						else Error_Fatal(true, Error_Format("Unable to get item \"%s\" from activity file", tempString2));
					}

					conf = Config_GetNextItem(conf);
				}

			}
			else Error_Fatal(TRUE, Error_Format("Cannot Find Activity List %s", tempString));

			std::sprintf(tempString, "%s%s%s", containerGlobs.gameName, CONFIG_SEPARATOR, CONTAINER_SCALESTRING);
			scale = Config_GetRealValue(rootConf, tempString);
			if (0.0f != scale) cont->activityFrame->AddScale(D3DRMCOMBINE_REPLACE, scale, scale, scale);

			Config_Free(rootConf);
		}
		else Error_Warn(TRUE, Error_Format("Cannot Find File %s", tempString));

	}
	else if (type == Container_Type::Container_Frame) {

		cont = Container_Create(parent);
		cont->type = type;
		// Just add it onto the activity frame...
		sprintf(tempString, "%s.%s", name, containerGlobs.extensionName[type]);
		if (Container_FrameLoad(tempString, cont->activityFrame));
		else Error_Warn(true, Error_Format("Cannot Load File \"%s\".\n", tempString));

	}
	else if (type == Container_Type::Container_Mesh) {

		IDirect3DRMMesh* mesh;
		void* fdata;
		uint32 fsize;

		// Create a meshbuilder, retrieve the mesh object
		// then attach it to the activity frame...
		sprintf(tempString, "%s.%s", name, containerGlobs.extensionName[type]);
		if (fdata = File_LoadBinary(tempString, &fsize)) {
			cont = Container_Create(parent);
			cont->type = type;
			if (mesh = Container_MeshLoad(fdata, fsize, tempString, cont->activityFrame, noTexture)) {
				Container_SetTypeData(cont, nullptr, nullptr, mesh, nullptr);
			} //else Error_Warn(true, Error_Format("Cannot Load File \"%s\"", tempString));
			Mem_Free(fdata);
		}

	}
	else if (type == Container_Type::Container_Anim || type == Container_Type::Container_LWS) {

//		IDirect3DRMAnimationSet* animSet;
		AnimClone* animClone;
		uint32 frameCount;

		cont = Container_Create(parent);
		cont->type = Container_Type::Container_Anim;
		sprintf(tempString, "%s.%s", name, containerGlobs.extensionName[Container_Type::Container_Anim]);
		if (animClone = Container_LoadAnimSet(tempString, cont->activityFrame, &frameCount, (type == Container_Type::Container_LWS) /*? true : false*/, looping)) {
			Container_Frame_SetAppData(cont->activityFrame, cont, animClone, name, &frameCount, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
		}
		else {
			Error_Warn(true, Error_Format("Cannot Load File \"%s\".\n", tempString));
			Container_Remove(cont);
			cont = nullptr;
		}

	}
	else if (type == Container_Type::Container_LWO) {

		Mesh* mesh;
		cont = Container_Create(parent);
		cont->type = type;

//		std::sprintf(tempString, "%s.lwo", name);
		//if( mesh = Lws_LoadMesh(nullptr, name, cont->activityFrame, noTexture) )
		if (mesh = Mesh_Load(name, cont->activityFrame, noTexture))
			Container_SetTypeData(cont, nullptr, nullptr, nullptr, mesh);
		else
		{
			/// FIX APPLY: Proper logging, add back-in sprintf that is seen commented out above.
			///             note that the .lwo extension is automatically applied down the line in Mesh_Load
			std::sprintf(tempString, "%s.lwo", name);
			Error_Warn(true, Error_Format("Cannot Load File \"%s\".", tempString));
			Container_Remove(cont);
			cont = nullptr;
		}

	}
	else if (type == Container_Type::Container_Invalid) {

		Error_Fatal(true, "Do not recognise container type");

	}
	else {

		Error_Warn(true, Error_Format("Code not implemented for Container type #%d", type));

	}

#ifdef _DEBUG_2
	if (cont) {
		Container_Frame_FormatName(cont->masterFrame, "Master Frame (%s:%s)", name, typestr);
		Container_Frame_FormatName(cont->activityFrame, "Activity Frame (%s:%s)", name, typestr);
		Container_Frame_FormatName(cont->hiddenFrame, "Hidden Frame (%s:%s)", name, typestr);
	}
#endif // _DEBUG_2

	return cont;
}


// <inlined>
/*__inline*/ Gods98::Container_Type __cdecl Gods98::Container_GetType(const Container* cont) {

	return cont->type;
}

// <inlined>
/*__inline*/ bool32 __cdecl Gods98::Container_AddActivity(Container* cont, const char* fname, const char* actname, real32 transCo, uint32 trigger, const char* sample, AnimClone* animClone, bool32 lws, bool32 looping)
{
	return Container_AddActivity2(cont, fname, actname, transCo, trigger, sample, animClone, lws, looping);
}



// <LegoRR.exe @00473600>
bool32 __cdecl Gods98::Container_IsCurrentActivity(Container* cont, const char* actname)
{
	IDirect3DRMFrame3* frame;

	Container_DebugCheckOK(cont);

	if (cont->type == Container_Type::Container_FromActivity) {
		if (frame = Container_Frame_Find(cont, actname, 0)) return true;
	}

	return false;
}

// <LegoRR.exe @00473630>
bool32 __cdecl Gods98::Container_SetActivity(Container* cont, const char* actname)
{
	IDirect3DRMFrame3* frame, *currFrame;
	bool32 result = false;
	char* currAnimName, *name, *freeAddr = nullptr;

	Container_DebugCheckOK(cont);

	if (cont->type == Container_Type::Container_FromActivity) {
		if (frame = Container_Frame_Find(cont, actname, 1)) {
			if (cont->typeData != nullptr) {
				if (cont->typeData->name != nullptr) {
					currAnimName = cont->typeData->name;
					if (currFrame = Container_Frame_Find(cont, currAnimName, 0)) {

						Container_Frame_SafeAddChild(cont->hiddenFrame, currFrame);

					}
					freeAddr = currAnimName;
				}
				else Error_Warn(true, "container has no typedata name");
			}

			Container_Frame_SafeAddChild(cont->activityFrame, frame);
			name = (char*)Mem_Alloc(std::strlen(actname) + 1);
			std::strcpy(name, actname);
			Container_SetTypeData(cont, name, nullptr, nullptr, nullptr);

			result = true;

			// Flag that the sample should be played on the next SetTime...
			cont->flags |= ContainerFlags::CONTAINER_FLAG_TRIGGERSAMPLE;

			if (freeAddr) Mem_Free(freeAddr);

		}
		else {
			result = false;
			Error_Warn(!Container_Frame_Find(cont, actname, 0), Error_Format("Unknown activity (\"%s\") passed to Container_SetActivity()", actname));
		}

		// Notify the game that the activity has changed...

#pragma message ( "Is the activity change callback required?" )
		if (cont->activityCallback) cont->activityCallback(cont, cont->activityCallbackData);
	}

	return result;
}

// <LegoRR.exe @00473720>
bool32 __cdecl Gods98::Container_Light_SetSpotPenumbra(Container* spotlight, real32 angle)
{
	Container_DebugCheckOK(spotlight);
	Error_Fatal(spotlight->type!=Container_Type::Container_Light, "Container is not a light");

	if (spotlight->typeData->light->SetPenumbra(angle) == D3DRM_OK) {
		return true;
	}
	else return false;
}

// <LegoRR.exe @00473740>
bool32 __cdecl Gods98::Container_Light_SetSpotUmbra(Container* spotlight, real32 angle)
{
	Container_DebugCheckOK(spotlight);
	Error_Fatal(spotlight->type!=Container_Type::Container_Light, "Container is not a light");

	if (spotlight->typeData->light->SetUmbra(angle) == D3DRM_OK) {
		return true;
	}
	else return false;
}

// <LegoRR.exe @00473760>
bool32 __cdecl Gods98::Container_Light_SetSpotRange(Container* spotlight, real32 dist)
{
	Container_DebugCheckOK(spotlight);
	Error_Fatal(spotlight->type!=Container_Type::Container_Light, "Container is not a light");

	if (spotlight->typeData->light->SetRange(dist) == D3DRM_OK) {
		return true;
	}
	else return false;
}

// <LegoRR.exe @00473780>
void __cdecl Gods98::Container_Light_SetEnableContainer(Container* light, Container* enable)
{
	//IDirect3DRMLight* enableFrame;
	IDirect3DRMFrame* enableFrame;
	Container_DebugCheckOK(light || enable);
	Error_Fatal(light->type!=Container_Type::Container_Light, "Container is not a light");

	enable->masterFrame->QueryInterface(Idl::IID_IDirect3DRMFrame, (void**)&enableFrame);
	light->typeData->light->SetEnableFrame(enableFrame);
}

// <LegoRR.exe @004737b0>
Gods98::Container* __cdecl Gods98::Container_MakeLight(Container* parent, Container_Light type, real32 r, real32 g, real32 b)
{
	Container* cont = Container_Create(parent);
	IDirect3DRMLight* light;
	Container_DebugCheckOK(CONTAINER_DEBUG_NOTREQUIRED);

	if (cont) {

		cont->type = Container_Type::Container_Light;

		/// NEW GODS98: New feature not present in LegoRR
		//if (type == Container_Light::Container_Light_Ambient) Mesh_SetAmbientLight(r, g, b);

		if (lpD3DRM()->CreateLightRGB((D3DRMLIGHTTYPE)type, r, g, b, &light) == D3DRM_OK) {
			Container_NoteCreation(light);

			cont->activityFrame->AddLight( light);
			Container_SetTypeData(cont, nullptr, light, nullptr, nullptr);
		}
		else Error_Fatal(TRUE, "Unable to create light");

#ifdef _DEBUG_2
		Container_Frame_FormatName(cont->masterFrame, "Light type #%d (%0.2f,%0.2f,%0.2f)", type, r, g, b);
#endif // _DEBUG_2

	}

	return cont;
}

// <LegoRR.exe @00473820>
Gods98::Container* __cdecl Gods98::Container_MakeMesh2(Container* parent, Container_MeshType type)
{
	Container* cont = Container_Create(parent);
	IDirect3DRMMesh* mesh;

	Container_DebugCheckOK(CONTAINER_DEBUG_NOTREQUIRED);

	if (cont) {
		cont->type = Container_Type::Container_Mesh;

		/// FIX APPLY: Container_MeshType_Subtractive has no comparison to type
		///             (if statement is always true)
		if (type == Container_MeshType::Container_MeshType_Transparent ||
			type == Container_MeshType::Container_MeshType_Immediate ||
			type == Container_MeshType::Container_MeshType_Additive ||
			type == Container_MeshType::Container_MeshType_Subtractive)
		{

			Mesh_RenderFlags flags = Mesh_RenderFlags::MESH_FLAG_TRANSFORM_PARENTPOS;
			Mesh* transmesh;

			switch (type) {
			case Container_MeshType::Container_MeshType_Transparent:	flags |= Mesh_RenderFlags::MESH_FLAG_RENDER_ALPHATRANS;		break;
			case Container_MeshType::Container_MeshType_Additive:		flags |= Mesh_RenderFlags::MESH_FLAG_RENDER_ALPHASA1;			break;
			case Container_MeshType::Container_MeshType_Subtractive:	flags |= Mesh_RenderFlags::MESH_FLAG_RENDER_ALPHASA0;			break;
			}

			transmesh = Mesh_CreateOnFrame(cont->activityFrame, nullptr, flags, nullptr, Mesh_Type::Mesh_Type_Norm);
			Container_SetTypeData(cont, nullptr, nullptr, nullptr, transmesh);

		}
		else {
			if (lpD3DRM()->CreateMesh(&mesh) == D3DRM_OK) {
				Container_NoteCreation(mesh);

				cont->activityFrame->AddVisual((IUnknown*)mesh);
				Container_SetTypeData(cont, nullptr, nullptr, mesh, nullptr);

				if (type == Container_MeshType::Container_MeshType_SeperateMeshes) {

					Container_MeshAppData* appdata = (Container_MeshAppData*)Mem_Alloc(sizeof(Container_MeshAppData));
					appdata->listSize = CONTAINER_MESHGROUPBLOCKSIZE;
					appdata->usedCount = 0;
					appdata->meshList = (IDirect3DRMMesh**)Mem_Alloc(sizeof(IDirect3DRMMesh*) * appdata->listSize);
					appdata->groupZeroHidden = false;
					appdata->firstAddGroup = true;
					mesh->SetAppData((DWORD)appdata);

				}
				else mesh->SetAppData((DWORD)nullptr);

			}
			else Error_Fatal(true, "Cannot create mesh object");
		}

#ifdef _DEBUG_2
		Container_Frame_FormatName(cont->masterFrame, "Mesh");
#endif // _DEBUG_2

	}

	return cont;
}

// <LegoRR.exe @00473940>
IDirect3DRMFrame3* __cdecl Gods98::Container_GetMasterFrame(Container* cont)
{
	return cont->masterFrame;
}

// <LegoRR.exe @00473950>
Gods98::Container* __cdecl Gods98::Container_Clone(Container* orig)
{
	Container* cont = nullptr, *useOldClone = nullptr;
	D3DRMMATRIX4D mat;
	uint32 loadRef = 0;

	Container_DebugCheckOK(orig);

	if (orig->type == Container_Type::Container_Mesh) {

		// Special case - Don't allow cloning of separated group meshes...

		LPDIRECT3DRMMESH mesh = orig->typeData->mesh;
		Error_Fatal(!mesh, "TypeData missing on Object");
		if (mesh->GetAppData()) Error_Fatal(true, "Cannot clone separated mesh objects");
	}

	if (orig->type == Container_Type::Container_FromActivity || orig->type == Container_Type::Container_Anim) {

		// If the object being cloned is a clone itself then use the original
		if (orig->cloneData) if (orig->cloneData->clonedFrom) orig = orig->cloneData->clonedFrom;

		if (orig->cloneData) {

			Container* testClone;

			// If the original clone is unused then return that...
			if (!orig->cloneData->used) useOldClone = orig;
			else {

				// Find an unused clone...
				for (uint32 loop = 0; loop < orig->cloneData->cloneCount; loop++) {
					if (testClone = orig->cloneData->cloneTable[loop]) {
						if (!testClone->cloneData->used) {
							useOldClone = testClone;
							break;
						}
					}
				}
			}

			if (useOldClone) {
				useOldClone->cloneData->used = true;
				cont = useOldClone;
				Container_SetParent(cont, Container_GetParent(orig));
				Container_SetAnimationTime(cont, 0.0f);
				Error_Debug(Error_Format("Reusing freed clone\n"));
			}
			else {
				cont = Container_Create(Container_GetParent(orig));
				loadRef = orig->cloneData->cloneCount++;
				orig->cloneData->cloneTable = (Container**)Mem_ReAlloc(orig->cloneData->cloneTable, sizeof(Container*) * orig->cloneData->cloneCount);
				orig->cloneData->cloneTable[loadRef] = cont;
			}

		}
		else {
			cont = Container_Create(Container_GetParent(orig));
			orig->cloneData = (Container_CloneData*)Mem_Alloc(sizeof(Container_CloneData));
			orig->cloneData->cloneCount = 1;
			orig->cloneData->cloneTable = (Container**)Mem_Alloc(sizeof(Container*));
			orig->cloneData->cloneTable[0] = cont;
			orig->cloneData->clonedFrom = nullptr;
			orig->cloneData->cloneNumber = 0;
			orig->cloneData->used = true;
			loadRef = 0;
		}
	}
	else cont = Container_Create(Container_GetParent(orig));

	{
		// Duplicate the activity frames transformation matrix
		IDirect3DRMFrame3* parent;
		orig->activityFrame->GetParent(&parent);
		orig->activityFrame->GetTransform(parent, mat);
		cont->activityFrame->AddTransform(D3DRMCOMBINE_REPLACE, mat);
		parent->Release();
	}

	if (useOldClone) return cont;

	// Copy across any data to the clone here..
	cont->type = orig->type;
	if (orig->typeData) {
		cont->typeData = (Container_TypeData*)Mem_Alloc(sizeof(Container_TypeData));
		std::memcpy(cont->typeData, orig->typeData, sizeof(Container_TypeData));
		if (orig->typeData->name) {
			cont->typeData->name = (char*)Mem_Alloc(std::strlen(orig->typeData->name) + 1);
			std::strcpy(cont->typeData->name, orig->typeData->name);
		}
	}
	else cont->typeData = nullptr;

	if (orig->type == Container_Type::Container_FromActivity || orig->type == Container_Type::Container_Anim) {

		// The new container obviously will have no cloneData.
		cont->cloneData = (Container_CloneData*)Mem_Alloc(sizeof(Container_CloneData));
		cont->cloneData->cloneTable = nullptr;
		cont->cloneData->cloneCount = 0;
		cont->cloneData->clonedFrom = orig;
		cont->cloneData->cloneNumber = loadRef;
		cont->cloneData->used = true;
		loadRef++;
	}

	if (cont->type == Container_Type::Container_Mesh) {

		IDirect3DRMMesh* mesh = cont->typeData->mesh;
		Error_Fatal(!mesh, "TypeData missing on Object");
		cont->activityFrame->AddVisual((IUnknown*)mesh);
		mesh->AddRef();

#ifdef _DEBUG_2
		Container_Frame_FormatName(cont->masterFrame, "Master Frame Mesh Clone (0x%0.8x)", orig->masterFrame);
#endif // _DEBUG_2


	}
	else if (cont->type == Container_Type::Container_FromActivity) {

		uint32 loop, count;
		IDirect3DRMFrame3** frameList;
		char** actNameList;
		const char* fname;
		real32 transCo;
		uint32 trigger;
		const char* sample;
		AnimClone* origClone;//, newClone;

		count = Container_GetActivities(orig, nullptr, nullptr, nullptr);
		frameList = (IDirect3DRMFrame3**)Mem_Alloc(sizeof(IDirect3DRMFrame3*) * count);
		actNameList = (char**)Mem_Alloc(sizeof(char*) * count);

		Container_GetActivities(orig, frameList, nullptr, actNameList);
		for (loop = 0; loop < count; loop++) {
			fname = Container_Frame_GetAnimSetFileName(frameList[loop]);
			transCo = Container_Frame_GetTransCo(frameList[loop]);
			sample = Container_Frame_GetSample(frameList[loop]);
			origClone = Container_Frame_GetAnimClone(frameList[loop]);
			trigger = Container_Frame_GetTrigger(frameList[loop]);
			Container_Frame_SetAppData(frameList[loop], nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
//			Container_AddActivity2(cont, fname, &actNameList[loop][std::strlen(CONTAINER_ACTIVITYFRAMEPREFIX)+1], transCo, loadRef, sample);
			Container_AddActivity2(cont, fname, &actNameList[loop][std::strlen(CONTAINER_ACTIVITYFRAMEPREFIX) + 1], transCo, trigger, sample, origClone, false, false);		// Last two parameters are ignored durng cloning...
			Mem_Free(actNameList[loop]);

			//UPDATE SOUND HERE TO ENSURE STREAMING BUFFER IS UPDATED PROPERLY
			//THIS IS DUE TO CLONING TAKING A LONG TIME
			//ONLY CHECK EVERY 25 ANIMATIONS BECAUSE STREAMING SOUND IS QUITE INTENSIVE?
			if ((loop % 25) == 0)
				Sound3D_Update();
		}
		Mem_Free(frameList);
		Mem_Free(actNameList);

		if (cont->typeData) Container_SetActivity(cont, cont->typeData->name);

#ifdef _DEBUG_2
		Container_Frame_FormatName(cont->masterFrame, "Master Frame Clone (0x%0.8x)", orig->masterFrame);
		Container_Frame_FormatName(cont->activityFrame, "Activity Frame Clone (0x%0.8x)", orig->activityFrame);
		Container_Frame_FormatName(cont->hiddenFrame, "Hidden Frame Clone (0x%0.8x)", orig->hiddenFrame);
#endif // _DEBUG_2

	}
	else if (cont->type == Container_Type::Container_Anim) {

//		IDirect3DRMAnimationSet* animSet;
		const char* fname;
		uint32 frameCount;
		AnimClone* origClone, *newClone;

		fname = Container_Frame_GetAnimSetFileName(orig->activityFrame);

//		if (animSet = Container_LoadAnimSet(fname, cont->activityFrame, loadRef, &frameCount)){
//			Container_Frame_SetAppData(cont->activityFrame, cont, animSet, fname, &frameCount, nullptr, nullptr, nullptr, nullptr);
//		} else Error_Fatal(true, Error_Format("Cannot clone from reference %i of file \"%s\" (too few copies specified in xwave?).\n", loadRef, fname));

		origClone = Container_Frame_GetAnimClone(orig->activityFrame);
		newClone = AnimClone_Make(origClone, cont->activityFrame, &frameCount);
		Container_Frame_SetAppData(cont->activityFrame, cont, newClone, fname, &frameCount, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

	}
	else if (cont->type == Container_Type::Container_LWO) {

		Mesh* mesh = cont->typeData->transMesh;
		Error_Fatal(!mesh, "TypeData missing on Object");
		cont->typeData->transMesh = Mesh_Clone(mesh, cont->activityFrame);

	}
	else {
		Error_Warn(true, Error_Format("Code not implemented for Container type #%d", orig->type));
	}

	return cont;
}

// <LegoRR.exe @00473de0>
void __cdecl Gods98::Container_Hide2(Container* cont, bool32 hide)
{
	if (hide) cont->flags |= ContainerFlags::CONTAINER_FLAG_HIDDEN2;
	else cont->flags &= ~ContainerFlags::CONTAINER_FLAG_HIDDEN2;
}

// <LegoRR.exe @00473e00>
void __cdecl Gods98::Container_Hide(Container* cont, bool32 hide)
{
	// Move the activity frame onto the hidden frame (will not hide any children attached
	// to the master frame)...

	bool32 hidden;

	if (cont == nullptr)		return;

	hidden = cont->flags & ContainerFlags::CONTAINER_FLAG_HIDDEN;
	Container_DebugCheckOK(cont);

//	if (cont->type == Container_Type::Container_Mesh) {
//		Mesh* transmesh;
//		Error_Fatal(cont->typeData==nullptr, "Mesh has no typeData");
//		if (transmesh = cont->typeData->transMesh) {
//			Mesh_Hide(transmesh, hide);
//		}
	//	} else {
	if (hide && !hidden) {
		Container_Frame_SafeAddChild(cont->hiddenFrame, cont->activityFrame);
		cont->flags |= ContainerFlags::CONTAINER_FLAG_HIDDEN;
	}
	else if (!hide && hidden) {
		Container_Frame_SafeAddChild(cont->masterFrame, cont->activityFrame);
		cont->flags &= ~ContainerFlags::CONTAINER_FLAG_HIDDEN;
	}
//	}
}

// <LegoRR.exe @00473e60>
bool32 __cdecl Gods98::Container_IsHidden(Container* cont)
{
	if (cont->flags & ContainerFlags::CONTAINER_FLAG_HIDDEN)
		return true;
	else
		return false;
}

// <LegoRR.exe @00473e80>
Gods98::Container* __cdecl Gods98::Container_SearchTree(Container* root, const char* name, Container_SearchMode mode, IN OUT uint32* count)
{
	Container_SearchData search;

	Container_DebugCheckOK(root);

	search.string = name;
	search.stringLen = std::strlen(name);
	search.caseSensitive = false;
	search.resultFrame = nullptr;
	search.count = 0;
	search.mode = mode;

	if (count) search.matchNumber = *count;
	else search.matchNumber = 0;

#ifdef CONTAINER_MATCHHIDDENHIERARCHY
	if (root->flags & ContainerFlags::CONTAINER_FLAG_HIDDEN) {
		Container_Frame_WalkTree(root->activityFrame, 0, Container_Frame_SearchCallback, &search);
	}
	else {
#endif // CONTAINER_MATCHHIDDENHIERARCHY

		Container_Frame_WalkTree(root->masterFrame, 0, Container_Frame_SearchCallback, &search);

#ifdef CONTAINER_MATCHHIDDENHIERARCHY
	}
#endif // CONTAINER_MATCHHIDDENHIERARCHY

	if (mode == Container_SearchMode::Container_SearchMode_FirstMatch || mode == Container_SearchMode::Container_SearchMode_NthMatch) {
		if (search.resultFrame) return Container_Frame_GetContainer(search.resultFrame);
	}
	else if (mode == Container_SearchMode::Container_SearchMode_MatchCount) *count = search.count;

	return nullptr;
}

// Pass NULL as instance for any...
// <LegoRR.exe @00473f20>
const char* __cdecl Gods98::Container_FormatPartName(Container* cont, const char* partname, OPTIONAL uint32* instance)
{
	// e.g. xf_????????_lphead_stationary_00_DDc_00 <- clone number (redundant (always zero))
	//         ^uid     ^part  ^filename  ^instance

	//static char name[1024]; //s_FormatPartName_name
	char tempString[1024];
	/// FIX APPLY: dummy init
	IDirect3DRMFrame3* frame = nullptr;
	char* fname, *s;
	AnimClone* animClone;

	Container_DebugCheckOK(cont);

	if (cont->type == Container_Type::Container_FromActivity) {
		Error_Fatal(!cont->typeData, "Container has no typeData");
		frame = Container_Frame_Find(cont, cont->typeData->name, 0);
		Error_Fatal(frame == nullptr, "Cannot locate current activities frame");
	}
	else if (cont->type == Container_Type::Container_Anim) {
		frame = cont->activityFrame;
	}
	else {
		Error_Fatal(true, "Calling this function with a non-animation type Container serves no purpose");
	}

	animClone = Container_Frame_GetAnimClone(frame);
	// ^^^^^^ If animClone is NULL then you have probably misspelled the animation filename. Duh!

	if (AnimClone_IsLws(animClone)) {
		if (instance) std::sprintf(s_FormatPartName_name, "%s_%0.2i", partname, *instance);
		else std::sprintf(s_FormatPartName_name, "%s_??", partname);
	}
	else {
		std::sprintf(tempString, "%s", Container_Frame_GetAnimSetFileName(frame));
		::_strlwr(tempString);
		for (fname = s = tempString; *s != '\0'; s++) if (*s == '\\') fname = s + 1;
	//		std::sprintf(s_FormatPartName_name, "xf_????_%s_%s_%0.2d_DDIclone_%0.2d", partname, fname, instance, Container_GetCloneNumber(cont));
	//		std::sprintf(s_FormatPartName_name, "xf_????????_%s_%s_%0.2d_DDc_%0.2d", partname, fname, instance, Container_GetCloneNumber(cont));
		if (instance) std::sprintf(s_FormatPartName_name, "xf_????????_%s_%s_%0.2d_DDc_00", partname, fname, *instance);
		else std::sprintf(s_FormatPartName_name, "xf_????????_%s_%s_??_DDc_00", partname, fname);
	}

	return s_FormatPartName_name;
}

// <LegoRR.exe @00474060>
void __cdecl Gods98::Container_SetUserData(Container* cont, void* data)
{
	Container_DebugCheckOK(cont);

	cont->userData = data;
}

// <LegoRR.exe @00474070>
void* __cdecl Gods98::Container_GetUserData(Container* cont)
{
	Container_DebugCheckOK(cont);

	return cont->userData;
}

// <LegoRR.exe @00474080>
void __cdecl Gods98::Container_EnableFog(bool32 on)
{
	containerGlobs.rootContainer->masterFrame->SetSceneFogEnable(on);
	if (on) containerGlobs.rootContainer->masterFrame->SetSceneBackground(containerGlobs.fogColour);
	else containerGlobs.rootContainer->masterFrame->SetSceneBackground(0 /*black*/);
}

// <LegoRR.exe @004740d0>
void __cdecl Gods98::Container_SetFogColour(real32 r, real32 g, real32 b)
{
	containerGlobs.fogColour = Container_GetRGBColour(r, g, b);
	containerGlobs.rootContainer->masterFrame->SetSceneFogColor(containerGlobs.fogColour);
}

// D3DRMFOGMODE mode
// <LegoRR.exe @00474130>
void __cdecl Gods98::Container_SetFogMode(uint32 mode)
{
	containerGlobs.rootContainer->masterFrame->SetSceneFogMode((D3DRMFOGMODE)mode);
	// 3Dfx requires table fog and software requires vertex fog...
	containerGlobs.rootContainer->masterFrame->SetSceneFogMethod(Main_GetFogMethod());
}

// <LegoRR.exe @00474160>
void __cdecl Gods98::Container_SetFogParams(real32 start, real32 end, real32 density)
{
	containerGlobs.rootContainer->masterFrame->SetSceneFogParams(start, end, density);
}

// <LegoRR.exe @00474180>
void __cdecl Gods98::Container_SetPerspectiveCorrection(Container* cont, bool32 on)
{
	Container_DebugCheckOK(cont);

	if (cont->type == Container_Type::Container_FromActivity) {

		// Set on every group of every mesh of every frame of every activity of the object...

		uint32 count = Container_GetActivities(cont, nullptr, nullptr, nullptr);
		IDirect3DRMFrame3** frameList = (IDirect3DRMFrame3**)Mem_Alloc(sizeof(IDirect3DRMFrame3*) * count);

		Container_GetActivities(cont, frameList, nullptr, nullptr);
		for (uint32 loop = 0; loop < count; loop++) {
			Container_Frame_WalkTree(frameList[loop], 0, Container_SetPerspectiveCorrectionCallback, &on);
		}
		Mem_Free(frameList);

	}
	else if (cont->type == Container_Type::Container_Mesh) {

		// Set on every group of the mesh...

		uint32 count = Container_Mesh_GetGroupCount(cont);
		for (uint32 loop = 0; loop < count; loop++) {
			Container_Mesh_SetPerspectiveCorrection(cont, loop, on);
		}
	}
}

// <LegoRR.exe @00474230>
bool32 __cdecl Gods98::Container_SetPerspectiveCorrectionCallback(IDirect3DRMFrame3* frame, void* data)
{
	// Only simple group meshes here...

	bool32 on = *((bool32*)data);

	DWORD visualCount;
	frame->GetVisuals(&visualCount, nullptr);
	if (visualCount) {
//		visualArray = Mem_Alloc(sizeof(LPDIRECT3DRMVISUAL) * visualCount);
		Error_Fatal(visualCount >= CONTAINER_MAXVISUALS, "CONTAINER_MAXVISUALS too small");
		IDirect3DRMVisual** visualArray = containerGlobs.visualArray;
		frame->GetVisuals(&visualCount, (IUnknown**)visualArray);

		for (uint32 loop = 0; loop < visualCount; loop++) {

			IDirect3DRMVisual* visual = visualArray[loop];

			IDirect3DRMMeshBuilder2* builder;
			IDirect3DRMMesh* mesh;

			if (visual->QueryInterface(Idl::IID_IDirect3DRMMeshBuilder2, (void**)&builder) == D3DRM_OK) {
				builder->SetPerspective(on);
			}
			else if (visual->QueryInterface(Idl::IID_IDirect3DRMMesh, (void**)&mesh) == D3DRM_OK) {
				uint32 groupCount = mesh->GetGroupCount();
				for (uint32 sub = 0; sub < groupCount; sub++) {
					if (on) mesh->SetGroupMapping(sub, D3DRMMAP_PERSPCORRECT);
					else mesh->SetGroupMapping(sub, 0);
				}
			}
		}

//		Mem_Free(visualArray);
	}

	return false;
}

// <LegoRR.exe @00474310>
IDirectDrawSurface4* __cdecl Gods98::Container_LoadTextureSurface(const char* fname, bool32 managed,
								OUT uint32* width, OUT uint32* height, OUT bool32* trans)
{
	uint8* fileData;
	uint32 size;
	D3DRMIMAGE image;
	IDirectDrawSurface4* surface = nullptr;
	DDSURFACEDESC2 desc, descBak;
	IDirectDrawPalette* palette;
	bool32 copy = false;
	HRESULT result;

	if (fileData = (uint8*)File_LoadBinary(fname, &size)) {
		BMP_Parse(fileData, size, &image);

		if (image.rgb == false && image.depth == 8) {

			std::memset(&desc, 0, sizeof(desc));
			desc.dwSize = sizeof(DDSURFACEDESC2);
			desc.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
			desc.dwWidth = image.width;
			desc.dwHeight = image.height;
			desc.ddsCaps.dwCaps = DDSCAPS_TEXTURE;

			//#ifndef _GODS98_VIDEOMEMTEXTURES
			if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_DONTMANAGETEXTURES)) {
				if (managed)
					desc.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
				else
					desc.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
			}
			else {
				//#else //_GODS98_VIDEOMEMTEXTURES
				desc.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
			}
			//#endif //_GODS98_VIDEOMEMTEXTURES

						// Find the prefered 8 bit palettized format...
			desc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
			if (lpDevice()->FindPreferredTextureFormat(DDBD_8, D3DRMFPTF_PALETTIZED, &desc.ddpfPixelFormat) != D3DRM_OK) {

				std::memcpy(&descBak, &desc, sizeof(descBak));

				// If the card doesn't like this the create a standard 8 bit surface and then blit it to one it does like...
				std::memset(&desc.ddpfPixelFormat, 0, sizeof(desc.ddpfPixelFormat));

				desc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
				desc.ddpfPixelFormat.dwFlags = DDPF_PALETTEINDEXED8 | DDPF_RGB;
				desc.ddpfPixelFormat.dwRGBBitCount = 8;

				desc.ddsCaps.dwCaps &= ~DDSCAPS_TEXTURE;
				desc.ddsCaps.dwCaps &= ~DDSCAPS_VIDEOMEMORY;
				desc.ddsCaps.dwCaps2 &= ~DDSCAPS2_TEXTUREMANAGE;

				desc.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
				desc.ddsCaps.dwCaps |= DDSCAPS_OFFSCREENPLAIN;

				copy = true;
			}

			if (DirectDraw()->CreateSurface(&desc, &surface, nullptr) == DD_OK) {

				std::memset(&desc, 0, sizeof(desc));
				desc.dwSize = sizeof(desc);

				if ((result = surface->Lock(nullptr, &desc, DDLOCK_WAIT, nullptr)) == DD_OK) {
					///sint32 y;
					uint8* surfaceMem = (uint8*)desc.lpSurface;
					uint8* imageMem = (uint8*)image.buffer1;
					for (sint32 y = 0; y < image.height; y++) {
						std::memcpy(surfaceMem, imageMem, image.bytes_per_line);
						surfaceMem += desc.lPitch;
						imageMem += image.bytes_per_line;
					}
					surface->Unlock(nullptr);

					if (DirectDraw()->CreatePalette(DDPCAPS_INITIALIZE | DDPCAPS_8BIT | DDPCAPS_ALLOW256, (LPPALETTEENTRY)image.palette, &palette, nullptr) == D3DRM_OK) {
						if (surface->SetPalette(palette) == D3DRM_OK) {

							uint32 r, g, b;
							uint32 decalColour;

							if (copy) {		// Find the cards preferred texture format...

								std::memset(&descBak.ddpfPixelFormat, 0, sizeof(descBak.ddpfPixelFormat));
								descBak.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
								if (lpDevice()->FindPreferredTextureFormat(DDBD_16, 0, &descBak.ddpfPixelFormat) == D3DRM_OK) {

									// Replace 'surface' with one of a format that the card will accept...

									IDirectDrawSurface4* oldSurface = surface;
									if (DirectDraw()->CreateSurface(&descBak, &surface, nullptr) == D3DRM_OK) {

										if (descBak.ddpfPixelFormat.dwRGBBitCount == 16) DirectDraw_Blt8To16(surface, oldSurface, (LPPALETTEENTRY)image.palette);

										oldSurface->Release();

									}
									else Error_Fatal(true, "Error creating new texture surface");
								}
							}

							if (trans)
							{
								if (Container_GetDecalColour(fname, &decalColour)) {
									DDCOLORKEY ddck;

									//									if(desc.ddpfPixelFormat.dwRGBBitCount > 8)
									if (copy) {
										r = image.palette[decalColour].red;
										g = image.palette[decalColour].green;
										b = image.palette[decalColour].blue;
										decalColour = DirectDraw_GetColour(surface, RGB_MAKE(r, g, b));
									}

									ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = decalColour;
									surface->SetColorKey(DDCKEY_SRCBLT, &ddck);

									*trans = true;

								}
								else *trans = false;

							}

							if (width) *width = image.width;
							if (height) *height = image.height;

							surface->AddRef();
						}
						palette->Release();
					}
					else Error_Fatal(true, "Cannot create Palette");
				}
				else {
#ifdef _DEBUG_2
					char error[128];

					std::sprintf(error, "Texture file %s : Cannot lock surface ", fname);

					switch (result) {
					case DDERR_INVALIDOBJECT:	std::strcat(error, "(DDERR_INVALIDOBJECT) ");	break;
					case DDERR_INVALIDPARAMS:	std::strcat(error, "(DDERR_INVALIDPARAMS) ");	break;
					case DDERR_OUTOFMEMORY:		std::strcat(error, "(DDERR_OUTOFMEMORY) ");		break;
					case DDERR_SURFACEBUSY:		std::strcat(error, "(DDERR_SURFACEBUSY) ");		break;
					case DDERR_SURFACELOST:		std::strcat(error, "(DDERR_SURFACELOST) ");		break;
					case DDERR_WASSTILLDRAWING:	std::strcat(error, "(DDERR_WASSTILLDRAWING) ");	break;
					case DDERR_LOCKEDSURFACES:	std::strcat(error, "(DDERR_LOCKEDSURFACES) ");	break;
					default:					CHKDD(result);								break;
					}

					{
						uint32 refCount;
						surface->AddRef();
						refCount = surface->Release();
						sprintf(&error[strlen(error)], "- Reference count == %i", refCount);
					}

					Error_Fatal(true, error);
#endif // _DEBUG_2

				}
				if (surface->Release() == 0) surface = nullptr;
			}
		}
		else Error_Warn(true, Error_Format("Non 8bit/palettised texture %s", fname));

		BMP_Cleanup(&image);
		Mem_Free(fileData);
	}

	return surface;
}

// <LegoRR.exe @004746d0>
bool32 __cdecl Gods98::Container_GetDecalColour(const char* fname, OUT uint32* colour)
{
	const char* s;
	const char* t;

	for (s=t=fname ; *t!='\0' ; t++) if (*t == '\\') s = t+1;
	if ((*s == 'a' || *s == 'A') && std::isdigit(*(s+1)) && std::isdigit(*(s+2)) && std::isdigit(*(s+3)) && *(s+4) == '_') {
		*colour = std::atoi(s+1);
		return true;
	}
	return false;
}

// <LegoRR.exe @004747b0>
Gods98::Container_Texture* __cdecl Gods98::Container_LoadTexture2(const char* fname, bool32 immediate,
								OUT uint32* width, OUT uint32* height)
{
	IDirect3DRMTexture3* texture = nullptr;

	const char* path = File_VerifyFilename(fname);
	//	LPUCHAR s, t;
	Container_Texture* newText;
	uint32 decalColour;
	//#ifndef CONTAINER_USEOWNTEXTURELOAD
	HRESULT r;
	//#endif

		// This malloc() (not Mem_Alloc()) is deliberate...
	if (newText = (Container_Texture * )legacy::malloc(sizeof(Container_Texture))) {

		//#ifdef CONTAINER_USEOWNTEXTURELOAD
		if (immediate) {

			IDirectDrawSurface* surf1;
			IDirectDrawSurface4* surf;
			PALETTEENTRY entry;

			if (surf = Container_LoadTextureSurface(fname, immediate, width, height, NULL)) {

				Error_Debug(Error_Format("Loaded texture %s\n", path));

				if (surf->QueryInterface(IID_IDirectDrawSurface, (void**)&surf1) == D3DRM_OK) {
					if (lpD3DRM()->CreateTextureFromSurface(surf1, &texture) == D3DRM_OK) {
						Container_NoteCreation(texture);
						surf1->Release();

						//					for (s=t=fname ; '\0'!=*t ; t++) if ('\\' == *t) s = t+1;
						//					if ('a' == *s && isdigit(*(s+1)) && isdigit(*(s+2)) && isdigit(*(s+3)) && '_' == *(s+4)) {

						if (Container_GetDecalColour(fname, &decalColour)) {

							IDirectDrawPalette* palette;
							surf->GetPalette(&palette);

							palette->GetEntries(0, decalColour, 1, &entry);
							palette->Release();

							texture->SetDecalTransparency(true);
							texture->SetDecalTransparentColor(RGB_MAKE(entry.peRed, entry.peGreen, entry.peBlue));
						}

						newText->texture = texture;
						newText->surface = surf;
						texture->SetAppData((DWORD)newText);

						return newText;
					}
					surf1->Release();
				}
				surf->Release();
			}

		}
		else {
			//#else // CONTAINER_USEOWNTEXTURELOAD

			if ((r = lpD3DRM()->LoadTexture(path, &texture)) == D3DRM_OK) {
				D3DRMIMAGE* image;
				uint32 r, g, b;//, i;
				Container_NoteCreation(texture);

				Error_Debug(Error_Format("Loaded texture %s\n", path));
				Error_LogLoad(true, path);

				if (image = texture->GetImage()) {
					//				for (s=t=fname ; '\0'!=*t ; t++) if ('\\' == *t) s = t+1;
					//				if ('a' == *s && isdigit(*(s+1)) && isdigit(*(s+2)) && isdigit(*(s+3)) && '_' == *(s+4)) {

					if (Container_GetDecalColour(fname, &decalColour)) {

						//					i = atoi(s+1);
						r = image->palette[decalColour].red;
						g = image->palette[decalColour].green;
						b = image->palette[decalColour].blue;
						texture->SetDecalTransparency(true);
						texture->SetDecalTransparentColor(RGB_MAKE(r, g, b));
					}
					if (width) *width = image->width;
					if (height) *height = image->height;
				}

				newText->texture = texture;
				newText->surface = nullptr;
				texture->SetAppData((DWORD)newText);

				return newText;

			}
			else {

				/// FIX APPLY: This is almost definitely supposed to be a comparison, not assignment
				if (r == D3DRMERR_FILENOTFOUND)
				{
					Error_Warn(true, Error_Format("Invalid filename specified \"%s\"", path));
					Error_LogLoadError(true, Error_Format("%d\t%s", Error_LoadError_InvalidFName, path));
				}
				else
				{
					Error_LogLoadError(true, Error_Format("%d\t%s", Error_LoadError_RMTexture, path));
					Error_Warn(true, Error_Format("Cannot open file %s", path));
				}

			}

		}
		//#endif // CONTAINER_USEOWNTEXTURELOAD

		legacy::free(newText);
	}
	return nullptr;
}

// <LegoRR.exe @004749d0>
void __cdecl Gods98::Container_FreeTexture(Container_Texture* text)
{
	if (text) {		// Wrench out the textures because of the vast video memory sucking hole that is Mesh.c
		if (text->surface) while (text->surface->Release());
		if (text->texture) while (text->texture->Release());
		// This will call free(text) (Not Mem_Free()) if there are no more references to the texture...
	}
}

// <LegoRR.exe @00474a20>
void __cdecl Gods98::Container_Mesh_Swap(Container* target, Container* origin, bool32 restore)
{
	// If not restoring then move any visuals on the container onto its hidden frame
	// and add the mesh from the origin container onto the target contianer...
	// Otherwise, restore the original visuals...

	IDirect3DRMMesh* mesh;
	IDirect3DRMVisual** visuals;
	IDirect3DRMVisual* visual;
	IDirect3DRMFrame3* frame;
	DWORD count;
	Mesh* transmesh;

	Container_DebugCheckOK(target);
	Error_Fatal(target->type!=Container_Type::Container_Reference && target->type!=Container_Type::Container_Mesh, "Container_Mesh_Swap() can only be used with a reference or mesh object as the 'target' container");
	Error_Fatal((target->flags & ContainerFlags::CONTAINER_FLAG_MESHSWAPPED) && !restore, "Container_Mesh_Swap() called without restoring previous swap");
	Error_Fatal(!(target->flags & ContainerFlags::CONTAINER_FLAG_MESHSWAPPED) && restore, "Container_Mesh_Swap() called with restore without a previous swap");

	if (target->type == Container_Type::Container_Reference) frame = target->masterFrame;
	else frame = target->activityFrame;

	if (!restore) {	// Move all the existing visuals onto the hidden frame...

		frame->GetVisuals(&count, nullptr);
		if (count) {
			//			visuals = Mem_Alloc(sizeof(LPDIRECT3DRMVISUAL) * count);
			Error_Fatal(count >= CONTAINER_MAXVISUALS, "CONTAINER_MAXVISUALS too small");
			visuals = containerGlobs.visualArray;
			frame->GetVisuals(&count, (IUnknown**)visuals);

			//		Error_Debug(Error_Format("Moving %i visuals to the hidden frame\n", count));
			for (uint32 loop = 0; loop < count; loop++) {

				visual = visuals[loop];

				target->hiddenFrame->AddVisual((IUnknown*)visual);
				frame->DeleteVisual((IUnknown*)visual);
			}
			//			Mem_Free(visuals);
		}

		/*		if (origin) {
					Error_Fatal(origin->type!=Container_Type::Container_Mesh, "Container_Mesh_Swap() called with non-mesh object as 'origin' container");
					mesh = origin->typeData->mesh;
					Error_Fatal(!mesh, "Container has no mesh object");
					Error_Fatal(mesh->GetAppData(), "Not yet supported with separate mesh groups");

					// Add the origin's mesh as the new visual...
					frame->AddVisual((IUnknown*) mesh);
				}
		*/

		if (origin) {
			//			Error_Fatal(Container_Mesh != origin->type, "Container_Mesh_Swap() called with non-mesh object as 'origin' container");
			if ((mesh = origin->typeData->mesh) == nullptr) {
				transmesh = origin->typeData->transMesh;
				Error_Fatal(transmesh == nullptr, "Container has no mesh object");
				frame->AddVisual((IUnknown*)transmesh->uv);
			}
			else {
				Error_Fatal(mesh->GetAppData(), "Not yet supported with separate mesh groups");
				// Add the origin's mesh as the new visual...
				frame->AddVisual((IUnknown*)mesh);
			}

		}


		target->flags |= ContainerFlags::CONTAINER_FLAG_MESHSWAPPED;

	}
	else {

		// Delete the visual (will still be used by the origin container) then restore the original visuals...

		frame->GetVisuals(&count, nullptr);
		if (count) {
			//			visuals = Mem_Alloc(sizeof(LPDIRECT3DRMVISUAL) * count);
			Error_Fatal(count >= CONTAINER_MAXVISUALS, "CONTAINER_MAXVISUALS too small");
			visuals = containerGlobs.visualArray;
			frame->GetVisuals(&count, (IUnknown**)visuals);

			//		Error_Debug(Error_Format("Deleting %i visuals...\n", count));
			for (uint32 loop = 0; loop < count; loop++) {
				visual = visuals[loop];
				frame->DeleteVisual((IUnknown*)visual);
			}
			//			Mem_Free(visuals);
		}

		target->hiddenFrame->GetVisuals(&count, nullptr);
		if (count) {
			//			visuals = Mem_Alloc(sizeof(LPDIRECT3DRMVISUAL) * count);
			Error_Fatal(count >= CONTAINER_MAXVISUALS, "CONTAINER_MAXVISUALS too small");
			visuals = containerGlobs.visualArray;
			target->hiddenFrame->GetVisuals(&count, (IUnknown**)visuals);

			//			Error_Debug(Error_Format("Restoring %i visuals from the hidden frame...\n", count));
			for (uint32 loop = 0; loop < count; loop++) {
				visual = visuals[loop];
				frame->AddVisual((IUnknown*)visual);
				target->hiddenFrame->DeleteVisual((IUnknown*)visual);
			}

			//			Mem_Free(visuals);
		}

		target->flags &= ~ContainerFlags::CONTAINER_FLAG_MESHSWAPPED;

	}
}

// <LegoRR.exe @00474bb0>
uint32 __cdecl Gods98::Container_Mesh_AddGroup(Container* cont, uint32 vertexCount,
								uint32 faceCount, uint32 vPerFace, const uint32* faceData)
{
	uint32 groupID;
	Container_MeshAppData* appdata;
	Mesh* transmesh;

	Container_DebugCheckOK(cont);

	Error_Fatal(cont->type!=Container_Type::Container_Mesh, "Container_Mesh_AddGroup() called with non mesh object");

	if (transmesh = cont->typeData->transMesh) {

		groupID = Mesh_AddGroup(transmesh, vertexCount, faceCount, vPerFace, faceData);

		return groupID;

	}
	else {

		Error_Fatal(0 == vertexCount, "Don't know if that will work!!!");

		IDirect3DRMMesh* mesh = cont->typeData->mesh;
		Error_Fatal(!mesh, "Container has no mesh object");

		if (appdata = (Container_MeshAppData*)mesh->GetAppData()) {

			if (!appdata->firstAddGroup) {

				// Make sure it doesn't crash...
				if (appdata->usedCount == appdata->listSize) return -1;

				// Using seperate meshes instead of groups...
				if (lpD3DRM()->CreateMesh(&mesh) == D3DRM_OK) {
					Container_NoteCreation(mesh);

					appdata->meshList[appdata->usedCount++] = mesh;
					cont->activityFrame->AddVisual((IUnknown*)mesh);

					{
						// Set the msb if the group is added as a visual...
						uint32 value = appdata->usedCount;
						value |= 0x80000000;
						mesh->SetAppData(value);
					}

					if (appdata->usedCount == appdata->listSize) {
						IDirect3DRMMesh** newList;
						if (newList = (IDirect3DRMMesh**)Mem_ReAlloc(appdata->meshList, sizeof(IDirect3DRMMesh*) * (appdata->listSize + CONTAINER_MESHGROUPBLOCKSIZE))) {
							appdata->listSize += CONTAINER_MESHGROUPBLOCKSIZE;
							appdata->meshList = newList;
						}
					}

				}
				else {
					return -1;
					Error_Warn(true, "Cannot add seperated mesh as group");
				}
			}
			else appdata->firstAddGroup = false;

		}

		if (mesh->AddGroup(vertexCount, faceCount, vPerFace, const_cast<uint32*>(faceData), (D3DRMGROUPINDEX*)&groupID) == D3DRM_OK) {

			if (appdata) return appdata->usedCount;
			else return groupID;

		}
		else {
			Error_Fatal(true, "Unable to AddGroup");
			return -1;
		}
	}
	return -1;
}

// <LegoRR.exe @00474ce0>
uint32 __cdecl Gods98::Container_Mesh_GetGroupCount(Container* cont)
{
	Container_MeshAppData* appdata;
	Mesh* transmesh;

	Container_DebugCheckOK(cont);
	Error_Fatal(cont->type!=Container_Type::Container_Mesh && cont->type!=Container_Type::Container_LWO, "Container_Mesh_GetGroupCount() called with non mesh object");
	Error_Fatal(!cont->typeData, "Container has no typeData");

	if (transmesh = cont->typeData->transMesh) {

		return Mesh_GetGroupCount(transmesh);

	}
	else {

		IDirect3DRMMesh* mesh = cont->typeData->mesh;
		Error_Fatal(!mesh, "Container has no mesh object");

		if (appdata = (Container_MeshAppData*)cont->typeData->mesh->GetAppData()) {
			return appdata->usedCount + 1;
		}
		else {
			return cont->typeData->mesh->GetGroupCount();
		}
	}
}

// <LegoRR.exe @00474d20>
void __cdecl Gods98::Container_Mesh_SetQuality(Container* cont, uint32 groupID, Container_Quality quality)
{
	Mesh* transmesh;

	Error_Fatal(cont->type!=Container_Type::Container_Mesh, "Container_Mesh_SetQuality() called with non mesh object");

	if (transmesh = cont->typeData->transMesh) {

		Error_Warn(true, "Not supported yet");

	}
	else {

		D3DRMRENDERQUALITY d3drmqual;
		if (quality == Container_Quality::Container_Quality_Wireframe) d3drmqual = D3DRMRENDER_WIREFRAME;
		if (quality == Container_Quality::Container_Quality_UnlitFlat) d3drmqual = D3DRMRENDER_UNLITFLAT;
		if (quality == Container_Quality::Container_Quality_Flat) d3drmqual = D3DRMRENDER_FLAT;
		if (quality == Container_Quality::Container_Quality_Gouraud) d3drmqual = D3DRMRENDER_GOURAUD;

		Container_DebugCheckOK(cont);
		Error_Fatal(!cont->typeData, "Container has no typeData");

		IDirect3DRMMesh* mesh = cont->typeData->mesh;
		Error_Fatal(!mesh, "Container has no mesh object");

		Container_Mesh_DebugCheckOK(cont, groupID);

		Container_Mesh_HandleSeperateMeshGroups(&mesh, &groupID);
		cont->typeData->mesh->SetGroupQuality(groupID, d3drmqual);
	}
}

// <LegoRR.exe @00474da0>
bool32 __cdecl Gods98::Container_Mesh_IsGroupHidden(Container* cont, uint32 group)
{
	bool32 hidden;
	Container_MeshAppData* appdata;
	Mesh* transmesh;

	Container_DebugCheckOK(cont);
	Error_Fatal(cont->type!=Container_Type::Container_Mesh, "Container_Mesh_IsGroupHidden() called with non mesh object");
	Error_Fatal(!cont->typeData, "Container has no typeData");

	if (transmesh = cont->typeData->transMesh) {

		hidden = Mesh_IsGroupHidden(transmesh, group);

	}
	else {
		IDirect3DRMMesh* mesh = cont->typeData->mesh;
		Error_Fatal(!mesh, "Container has no mesh object");

		Container_Mesh_DebugCheckOK(cont, group);

		IDirect3DRMMesh* groupmesh;
		if (appdata = (Container_MeshAppData*)mesh->GetAppData()) {

			if (0 != group) {
				groupmesh = appdata->meshList[group - 1];
				hidden = !(groupmesh->GetAppData() & 0x80000000);
			}
			else hidden = appdata->groupZeroHidden;

		}
		else Error_Fatal(true, "Wrong mesh type");

	}
	return hidden;
}

// <LegoRR.exe @00474df0>
void __cdecl Gods98::Container_Mesh_HideGroup(Container* cont, uint32 group, bool32 hide)
{
	//IDirect3DRMMesh* mesh, *groupmesh;
	Container_MeshAppData* appdata;
	Mesh* transmesh;

	Container_DebugCheckOK(cont);
	Error_Fatal(cont->type!=Container_Type::Container_Mesh, "Container_Mesh_HideGroup() called with non mesh object");
	Error_Fatal(!cont->typeData, "Container has no typeData");

	if (transmesh = cont->typeData->transMesh) {

		Mesh_HideGroup(transmesh, group, hide);

	}
	else {

		IDirect3DRMMesh* mesh = cont->typeData->mesh;
		Error_Fatal(!mesh, "Container has no mesh object");

		Container_Mesh_DebugCheckOK(cont, group);

		if (appdata = (Container_MeshAppData*)mesh->GetAppData()) {

			bool32 hidden;
			IDirect3DRMMesh* groupmesh;
			IDirect3DRMVisual* visual;
			if (group != 0) {
				groupmesh = appdata->meshList[group - 1];
				hidden = !(groupmesh->GetAppData() & 0x80000000);
				visual = (LPDIRECT3DRMVISUAL)groupmesh;
				group = 0;
			}
			else {
				groupmesh = nullptr;
				hidden = appdata->groupZeroHidden;
				visual = (LPDIRECT3DRMVISUAL)mesh;
			}

			if (hide && !hidden) cont->activityFrame->DeleteVisual((IUnknown*)visual);
			else if (!hide && hidden) cont->activityFrame->AddVisual((IUnknown*)visual);

			if (groupmesh) {
				uint32 keep = groupmesh->GetAppData() & 0x7fffffff;
				if (hide) groupmesh->SetAppData(keep);
				else groupmesh->SetAppData(keep | 0x80000000);
			}
			else appdata->groupZeroHidden = hide;
		}// else Error_Fatal(true, "Wrong mesh type");
	}
}

// <LegoRR.exe @00474ec0>
bool32 __cdecl Gods98::Container_Mesh_HandleSeperateMeshGroups(IN OUT IDirect3DRMMesh** mesh, IN OUT uint32* group)
{
	Container_MeshAppData* appdata;

	if (appdata = (Container_MeshAppData*)(*mesh)->GetAppData()) {

		// Leave the default mesh if the group is zero and the (group-1)'th off the
		// list if not...

		if ((*group) != 0) *mesh = appdata->meshList[(*group) - 1];
		(*group) = 0;

		return true;
	}

	return false;
}

// <LegoRR.exe @00474f00>
bool32 __cdecl Gods98::Container_Mesh_GetGroup(Container* cont, uint32 groupID,
								OUT uint32* vertexCount, OUT uint32* faceCount,
								OUT uint32* vPerFace, OUT uint32* faceDataSize,
								OUT uint32* faceData)
{
	Mesh* transmesh;

	Container_DebugCheckOK(cont);

	Error_Fatal(cont->type!=Container_Type::Container_Mesh && cont->type!=Container_Type::Container_LWO, "Container_Mesh_GetGroup() called with non mesh object");
	Error_Fatal(!cont->typeData, "Container has no typeData");

	if (transmesh = cont->typeData->transMesh) {

		Mesh_GetGroup(transmesh, groupID, vertexCount, faceCount, vPerFace, faceDataSize, faceData);
		return true;

	}
	else {
		IDirect3DRMMesh* mesh = cont->typeData->mesh;
		Error_Fatal(!mesh, "Container has no mesh object");

		Container_Mesh_DebugCheckOK(cont, groupID);

		Container_Mesh_HandleSeperateMeshGroups(&mesh, &groupID);

		if (mesh->GetGroup(groupID, vertexCount, faceCount, vPerFace, (DWORD*)faceDataSize, faceData) == D3DRM_OK) {

			return true;

		}
		else {
			Error_Warn(true, "Unable to GetGroup()");
			return false;
		}
	}
}

// <LegoRR.exe @00474f80>
uint32 __cdecl Gods98::Container_Mesh_GetVertices(Container* cont, uint32 groupID, uint32 index,
								uint32 count, OUT Vertex3F* retArray)
{
	Mesh* transmesh;

	Container_DebugCheckOK(cont);

	Error_Fatal(cont->type!=Container_Type::Container_Mesh && cont->type!=Container_Type::Container_LWO, "Container_Mesh_GetVertices() called with non mesh object");

	if (transmesh = cont->typeData->transMesh) {

		Mesh_GetVertices(transmesh, groupID, index, count, retArray);

	}
	else {

		IDirect3DRMMesh* mesh = cont->typeData->mesh;
		Error_Fatal(!mesh, "Container has no mesh object");

		Container_Mesh_DebugCheckOK(cont, groupID);

		Container_Mesh_HandleSeperateMeshGroups(&mesh, &groupID);

		if (mesh->GetVertices(groupID, index, count, (LPD3DRMVERTEX)retArray) == D3DRM_OK) {

		}
		else {
			Error_Fatal(true, "Unable to GetVertices");
			return CONTAINER_ULONG_NULL;
		}
	}

	return 0;
}

// <LegoRR.exe @00474ff0>
uint32 __cdecl Gods98::Container_Mesh_SetVertices(Container* cont, uint32 groupID, uint32 index,
								uint32 count, const Vertex3F* values)
{
	Mesh* transmesh;

	Container_DebugCheckOK(cont);

	Error_Fatal(cont->type!=Container_Type::Container_Mesh && cont->type!=Container_Type::Container_LWO, "Container_Mesh_SetVertices() called with non mesh object");

	if (transmesh = cont->typeData->transMesh) {

		Mesh_SetVertices(transmesh, groupID, index, count, values);

	}
	else {

		IDirect3DRMMesh* mesh = cont->typeData->mesh;
		Error_Fatal(!mesh, "Container has no mesh object");

		Container_Mesh_DebugCheckOK(cont, groupID);

		Container_Mesh_HandleSeperateMeshGroups(&mesh, &groupID);

		if (mesh->SetVertices(groupID, index, count, (LPD3DRMVERTEX)values) == D3DRM_OK) {

		}
		else {
			Error_Fatal(true, "Unable to SetVertices");
			return CONTAINER_ULONG_NULL;
		}
	}

	return 0;
}

// <LegoRR.exe @00475060>
void __cdecl Gods98::Container_Mesh_SetTexture(Container* cont, uint32 groupID, Container_Texture* itext)
{
	IDirect3DRMTexture3* texture;
	HRESULT r;
	Mesh* transmesh;

	if (itext) texture = itext->texture;
	else texture = nullptr;

	Container_DebugCheckOK(cont);

	Error_Fatal(cont->type!=Container_Type::Container_Mesh && cont->type!=Container_Type::Container_LWO, "Container_Mesh_SetTexture() called with non mesh object");

	if (transmesh = cont->typeData->transMesh) {

		Error_Fatal(itext && itext->surface != nullptr && itext->texture == nullptr, "Texture is required to be created by Mesh_LoadTexture()");
		Mesh_SetGroupTexture(transmesh, groupID, itext);

	}
	else {
		IDirect3DRMTexture* text1;

		Container_Mesh_DebugCheckOK(cont, groupID);

		IDirect3DRMMesh* mesh = cont->typeData->mesh;
		Error_Fatal(!mesh, "Container has no mesh object");

		Container_Mesh_HandleSeperateMeshGroups(&mesh, &groupID);

		if (texture) {
			r = texture->QueryInterface(Idl::IID_IDirect3DRMTexture, (void**)&text1);
			Error_Fatal(r, "Unable to query texture1");
		}
		else text1 = nullptr;

		if (mesh->SetGroupTexture(groupID, text1) == D3DRM_OK) {

// unused preprocessor in LegoRR
#ifndef CONTAINER_DISABLEFRAMESETTINGS
			cont->activityFrame->SetMaterialMode(D3DRMMATERIAL_FROMMESH);
#endif // CONTAINER_DISABLEFRAMESETTINGS

		}
		else Error_Fatal(true, "Unable to SetGroupTexture");

		if (text1) text1->Release();
	}
}

// <LegoRR.exe @004750f0>
void __cdecl Gods98::Container_Mesh_SetPerspectiveCorrection(Container* cont, uint32 groupID, bool32 on)
{
	Mesh* transmesh;

	Container_DebugCheckOK(cont);
	Error_Fatal(cont->type!=Container_Type::Container_Mesh && cont->type!=Container_Type::Container_LWO, "Container_Mesh_SetPerspectiveCorrection() called with non mesh object");

	if (transmesh = cont->typeData->transMesh) {

		Error_Warn(true, "Not yet implemented for immediate mode meshes");

	}
	else {

		IDirect3DRMMesh* mesh = cont->typeData->mesh;

		Error_Fatal(!mesh, "Container has no mesh object");

		Container_Mesh_DebugCheckOK(cont, groupID);

		Container_Mesh_HandleSeperateMeshGroups(&mesh, &groupID);

		if (on) mesh->SetGroupMapping(groupID, D3DRMMAP_PERSPCORRECT);
		else mesh->SetGroupMapping(groupID, (D3DRMMAPPING)0);
	}
}

// <LegoRR.exe @00475150>
bool32 __cdecl Gods98::Container_Mesh_Scale(Container* cont, real32 x, real32 y, real32 z)
{
	Container_MeshAppData* appdata;
	Mesh* transmesh;

	Container_DebugCheckOK(cont);
	Error_Fatal(cont->type!=Container_Type::Container_Mesh && cont->type!=Container_Type::Container_LWO, "Container_Mesh_Scale() called with non mesh object");

	if (transmesh = cont->typeData->transMesh) {

		Mesh_Scale(transmesh, x, y, z);

	}
	else {

		IDirect3DRMMesh* mesh = cont->typeData->mesh;
		Error_Fatal(!mesh, "'From' Container has no mesh object");

		mesh->Scale(x, y, z);

		if (appdata = (Container_MeshAppData*)mesh->GetAppData()) {
			// Scale all the other meshes...
			for (uint32 loop = 0; loop < appdata->usedCount; loop++) {
				mesh = appdata->meshList[loop];
				mesh->Scale(x, y, z);
			}
		}
	}

	return false;
}

// <LegoRR.exe @004751d0>
bool32 __cdecl Gods98::Container_Mesh_GetBox(Container* cont, OUT Box3F* box)
{
	IDirect3DRMMesh* mesh = cont->typeData->mesh;
	Container_MeshAppData* appdata;

	Container_DebugCheckOK(cont);
	Error_Fatal(cont->type != Container_Type::Container_Mesh, "Container_Mesh_GetBox() called with non mesh object");
	Error_Fatal(!mesh, "'From' Container has no mesh object");
	mesh->GetBox((LPD3DRMBOX)box);

	if (appdata = (Container_MeshAppData*)mesh->GetAppData()) {


		// Find if any of the others span outside this box...
		for (uint32 loop = 0; loop < appdata->usedCount; loop++) {
			mesh = appdata->meshList[loop];

			D3DRMBOX subbox;
			mesh->GetBox(&subbox);
			if (subbox.min.x < box->min.x) box->min.x = subbox.min.x;
			if (subbox.min.y < box->min.y) box->min.y = subbox.min.y;
			if (subbox.min.z < box->min.z) box->min.z = subbox.min.z;
			if (subbox.max.x > box->max.x) box->max.x = subbox.max.x;
			if (subbox.max.y > box->max.y) box->max.y = subbox.max.y;
			if (subbox.max.z > box->max.z) box->max.z = subbox.max.z;
		}
	}

	return false;
}

// <LegoRR.exe @004752b0>
void __cdecl Gods98::Container_Mesh_SetEmissive(Container* cont, uint32 groupID,
								real32 r, real32 g, real32 b)
{
// unused preprocessor in LegoRR
#ifndef CONTAINER_DISABLEFRAMESETTINGS
	IDirect3DRMMaterial2* material2;
	IDirect3DRMMaterial* material;
	IDirect3DRMMesh* mesh;
#endif // CONTAINER_DISABLEFRAMESETTINGS
	Mesh* transmesh;

	Container_DebugCheckOK(cont);
	Error_Fatal(cont->type!=Container_Type::Container_Mesh && cont->type!=Container_Type::Container_LWO, "Container_Mesh_SetEmissive() called with non mesh object");

	if (transmesh = cont->typeData->transMesh) {

		Mesh_SetGroupEmissive(transmesh, groupID, r, g, b);
			
	} else {

// unused preprocessor in LegoRR
#ifndef CONTAINER_DISABLEFRAMESETTINGS
		mesh = cont->typeData->mesh;
		Error_Fatal(!mesh, "Container has no mesh object");

		Container_Mesh_DebugCheckOK(cont, groupID);

		Container_Mesh_HandleSeperateMeshGroups(&mesh, &groupID);

		mesh->GetGroupMaterial(groupID, &material);
		if (material == nullptr) {
			lpD3DRM()->CreateMaterial(0.0f, &material2);
			Container_NoteCreation(material);
			material2->QueryInterface(Idl::IID_IDirect3DRMMaterial, (void**)&material);
			material2->Release();
			mesh->SetGroupMaterial(groupID, material);
		}

		material->SetEmissive(r, g, b);
		material->Release();
		cont->activityFrame->SetMaterialMode(D3DRMMATERIAL_FROMMESH);
#endif // CONTAINER_DISABLEFRAMESETTINGS
	}
}



// <LegoRR.exe @004752e0>
void __cdecl Gods98::Container_Mesh_SetColourAlpha(Container* cont, uint32 groupID,
								real32 r, real32 g, real32 b, real32 a)
{
	Mesh* transmesh;

	Container_DebugCheckOK(cont);
	Error_Fatal(cont->type!=Container_Type::Container_Mesh && cont->type!=Container_Type::Container_LWO, "Container_Mesh_SetColourAlpha() called with non mesh object");

	D3DCOLOR colour = Container_GetRGBAColour(r, g, b, a);

	if (transmesh = cont->typeData->transMesh) {

		Mesh_SetGroupDiffuse(transmesh, groupID, r, g, b);
		Mesh_SetGroupAlpha(transmesh, groupID, a);

	}
	else {

// unused preprocessor in LegoRR
#ifndef CONTAINER_DISABLEFRAMESETTINGS

		IDirect3DRMMesh* mesh = cont->typeData->mesh;
		Error_Fatal(!mesh, "Container has no mesh object");

		Container_Mesh_DebugCheckOK(cont, groupID);

		Container_Mesh_HandleSeperateMeshGroups(&mesh, &groupID);

		mesh->SetGroupColor(groupID, colour);
		cont->activityFrame->SetMaterialMode(D3DRMMATERIAL_FROMMESH);

#endif // CONTAINER_DISABLEFRAMESETTINGS

	}
}

// <LegoRR.exe @00475330>
void __cdecl Gods98::Container_Transform(Container* cont, OUT Vector3F* dest, const Vector3F* src)
{
	Container_DebugCheckOK(cont);

	cont->masterFrame->Transform((LPD3DVECTOR)dest, (LPD3DVECTOR)const_cast<Vector3F*>(src));
}

// <LegoRR.exe @00475350>
void __cdecl Gods98::Container_InverseTransform(Container* cont, OUT Vector3F* dest, const Vector3F* src)
{
	Container_DebugCheckOK(cont);

	cont->masterFrame->InverseTransform((LPD3DVECTOR)dest, (LPD3DVECTOR)const_cast<Vector3F*>(src));
}

// <LegoRR.exe @00475370>
void __cdecl Gods98::Container_SetColourAlpha(Container* cont, real32 r, real32 g, real32 b, real32 a)
{
	Container_DebugCheckOK(cont);
	Error_Fatal(!cont->typeData, "Container has no typeData");
	
	D3DCOLOR colour = Container_GetRGBAColour(r, g, b, a);

	if (cont->type == Container_Type::Container_Light){
		Error_Fatal(!cont->typeData->light, "typedata has no light");
		cont->typeData->light->SetColor(colour);

		/// NEW GODS98: Feature not present in LegoRR
		//if (D3DRMLIGHT_AMBIENT == cont->typeData->light->GetType()) Mesh_SetAmbientLight(r, g, b);

	} else {


#ifndef CONTAINER_DISABLEFRAMESETTINGS
		cont->activityFrame->SetColor(colour);
		cont->activityFrame->SetMaterialMode(D3DRMMATERIAL_FROMFRAME);
#endif // CONTAINER_DISABLEFRAMESETTINGS
	}

	/*
	if (cont->type == Container_Type::Container_Light){
		Error_Fatal(!cont->typeData->light, "typedata has no light");
		cont->typeData->light->SetColor(colour);
	} else if (cont->type == Container_Type::Container_Mesh){
//		Error_Fatal(!cont->typeData->mesh, "typedata has no mesh");
		cont->activityFrame->SetColor(colour);
		cont->activityFrame->SetMaterialMode(D3DRMMATERIAL_FROMFRAME);
//		cont->typeData->mesh->SetGroupColor(groupID, colour);
//	} else if (cont->type == Container_Type::Container_Mesh){  // yes, this was checking the same type again
//		Error_Fatal(!cont->typeData->meshbuilder, "typedata has no meshbuilder");
//		cont->typeData->meshbuilder->SetColor(colour);
//	} else if (cont->type == Container_Type::Container_FromActivity){
//		Error_Fatal(!cont->typeData->name, "typedata has no activity name");
	} else Error_Warn(true, Error_Format("Code not implemented for Container type #%d", cont->type));*/
}

// <LegoRR.exe @004753e0>
real32 __cdecl Gods98::Container_MoveAnimation(Container* cont, real32 delta)
{
	real32 time = Container_GetAnimationTime(cont);
	return Container_SetAnimationTime(cont, time + delta);
}

// <LegoRR.exe @00475400>
real32 __cdecl Gods98::Container_SetAnimationTime(Container* cont, real32 time)
{
	/// FIX APPLY: dummy init
	IDirect3DRMFrame3* frame = nullptr;
//	IDirect3DRMAnimationSet* animSet = nullptr;
	char* currAnimName;
	uint32 frameCount;
	AnimClone* animClone = nullptr;
	real32 overrun = 0.0f;

	Container_DebugCheckOK(cont);

	if (cont->type == Container_Type::Container_FromActivity){
		Error_Fatal(!cont->typeData, "Container has no typeData");

		currAnimName = cont->typeData->name;
		if (frame = Container_Frame_Find(cont, currAnimName, 0)){
//			animSet = Container_Frame_GetAnimSet(frame);
			animClone = Container_Frame_GetAnimClone(frame);
	
/*			if (cont->flags & ContainerFlags::CONTAINER_FLAG_TRIGGERSAMPLE){
#pragma message ( "Trigger the sample half way through if the SetTime is half way in" )
				// Trigger the sample... (The sample may be longer than the animset!)
				lpSound sound;
				if (sound = Container_Frame_GetSample(frame)){
					Sound_Play(sound, Sound_Once);
				}
			}*/

			if (cont->flags & ContainerFlags::CONTAINER_FLAG_TRIGGERSAMPLE){
				const char* sound;
				if (sound = Container_Frame_GetSample(frame)){
					if (containerGlobs.soundTriggerCallback && (containerGlobs.flags & Container_GlobFlags::CONTAINER_FLAG_TRIGGERENABLED)) containerGlobs.soundTriggerCallback(sound, cont, containerGlobs.soundTriggerData);
				}
				cont->flags &= ~ContainerFlags::CONTAINER_FLAG_TRIGGERSAMPLE;
			}

		} else Error_Warn(true, "Couldn't find frame (thus AnimationSet) to SetTime() on");

	} else if (cont->type == Container_Type::Container_Anim) {

//		animSet = Container_Frame_GetAnimSet(cont->activityFrame);
		animClone = Container_Frame_GetAnimClone(cont->activityFrame);
		frame = cont->activityFrame;

	}// else Error_Fatal(true, "Container_SetTime() called with non-animation type Object");

	if (animClone) {

		bool32 skipSetTime = (cont->flags & ContainerFlags::CONTAINER_FLAG_HIDDEN);
		real32 oldTime = Container_GetAnimationTime(cont);

		// If the container is hidden then don't bother updating the animation
		// unless the animation has ended...

		frameCount = Container_Frame_GetFrameCount(frame);
		Container_Frame_SetAppData(frame, nullptr, nullptr, nullptr, nullptr, nullptr, &time, nullptr, nullptr, nullptr, nullptr);
		if (frameCount){
			frameCount--;
			if (time > frameCount) {
				overrun = (time - frameCount);
				skipSetTime = false;
			}
		}

		if (skipSetTime == false) {
			real32 triggerFrame = (real32) Container_Frame_GetTrigger(frame);
#pragma message ( "Not restoring the time on original animation set" )
			if (frameCount != 1) {
				AnimClone_SetTime(animClone, time, nullptr);//&oldTime);
				if (containerGlobs.triggerFrameCallback && triggerFrame) {
					if (oldTime < triggerFrame && time >= triggerFrame) {
						containerGlobs.triggerFrameCallback(cont, containerGlobs.triggerFrameData);
					}
				}
			}
		} else {
			cont->flags |= ContainerFlags::CONTAINER_FLAG_ANIMATIONSKIPPED;
		}

	}

	return overrun;
}

// <LegoRR.exe @004755c0>
void __cdecl Gods98::Container_ForceAnimationUpdate(Container* cont)
{
	if (cont->flags & ContainerFlags::CONTAINER_FLAG_ANIMATIONSKIPPED) {

		/// FIX APPLY: dummy init
		AnimClone* animClone = nullptr;
		IDirect3DRMFrame3* frame;
		real32 time;

		if (cont->type == Container_Type::Container_FromActivity) {
			Error_Fatal(!cont->typeData, "Container has no typeData");
			if (frame = Container_Frame_Find(cont, cont->typeData->name, 0)) {
				animClone = Container_Frame_GetAnimClone(frame);
				time = Container_Frame_GetCurrTime(frame);
			}
			else Error_Warn(true, "Couldn't find frame (thus AnimationSet) to SetTime() on");
		}
		else if (cont->type == Container_Type::Container_Anim) {
			animClone = Container_Frame_GetAnimClone(cont->activityFrame);
			time = Container_Frame_GetCurrTime(cont->activityFrame);
		}

		if (animClone) AnimClone_SetTime(animClone, time, nullptr);
	}
}

// <LegoRR.exe @00475650>
real32 __cdecl Gods98::Container_GetAnimationTime(Container* cont)
{
	IDirect3DRMFrame3* frame;
	char* currAnimName;
	real32 time = 0.0f;

	Container_DebugCheckOK(cont);
	
	if (cont->type == Container_Type::Container_FromActivity) {
		Error_Fatal(!cont->typeData, "Container has no typeData");

		currAnimName = cont->typeData->name;
		if (frame = Container_Frame_Find(cont, currAnimName, 0)){
			time = Container_Frame_GetCurrTime(frame);
		} else Error_Warn(true, "Couldn't find frame (thus AnimationSet) to SetTime() on");
		
	} else if (cont->type == Container_Type::Container_Anim) {

		time = Container_Frame_GetCurrTime(cont->activityFrame);

	} //else Error_Fatal(true, "Container_SetTime() called with non-animation type container");

	return time;
}

// <LegoRR.exe @004756b0>
uint32 __cdecl Gods98::Container_GetAnimationFrames(Container* cont)
{
	IDirect3DRMFrame3* frame = nullptr;
	char* currAnimName;

	if (cont->type == Container_Type::Container_FromActivity) {
		Error_Fatal(!cont->typeData, "Container has no typeData");
		currAnimName = cont->typeData->name;
		frame = Container_Frame_Find(cont, currAnimName, 0);
	}
	else if (cont->type == Container_Type::Container_Anim) {
		frame = cont->activityFrame;
	}

	if (frame) return Container_Frame_GetFrameCount(frame);
	else return 0;
}

// <LegoRR.exe @004756f0>
void __cdecl Gods98::Container_SetPosition(Container* cont, OPTIONAL Container* ref,
								real32 x, real32 y, real32 z)
{
	IDirect3DRMFrame3* refFrame, *frame;
	Container_GetFrames(cont, ref, &frame, &refFrame);
	frame->SetPosition(refFrame, x, y, z);

// unused preprocessor in LegoRR
#ifdef CONTAINER_MATCHHIDDENHIERARCHY
	Container_GetFramesHidden(cont, ref, &frame, &refFrame);
	frame->SetPosition(refFrame, x, y, z);
#endif // CONTAINER_MATCHHIDDENHIERARCHY
}

// <LegoRR.exe @00475730>
void __cdecl Gods98::Container_SetOrientation(Container* cont, OPTIONAL Container* ref,
								real32 dirx, real32 diry, real32 dirz, real32 upx, real32 upy, real32 upz)
{
	IDirect3DRMFrame3* refFrame, *frame;
	Container_GetFrames(cont, ref, &frame, &refFrame);
	frame->SetOrientation(refFrame, dirx, diry, dirz, upx, upy, upz);

// unused preprocessor in LegoRR
#ifdef CONTAINER_MATCHHIDDENHIERARCHY
	Container_GetFramesHidden(cont, ref, &frame, &refFrame);
	frame->SetOrientation(refFrame, dirx, diry, dirz, upx, upy, upz);
#endif // CONTAINER_MATCHHIDDENHIERARCHY
}

// <LegoRR.exe @00475780>
void __cdecl Gods98::Container_GetPosition(Container* cont, OPTIONAL Container* ref, OUT Vector3F* pos)
{
	IDirect3DRMFrame3* refFrame, *frame;
	Container_GetFrames(cont, ref, &frame, &refFrame);
	frame->GetPosition(refFrame, (LPD3DVECTOR)pos);
}

// <LegoRR.exe @004757c0>
void __cdecl Gods98::Container_GetOrientation(Container* cont, OPTIONAL Container* ref, OUT Vector3F* dir, OUT Vector3F* up)
{
	IDirect3DRMFrame3* refFrame, *frame;
	Vector3F vdir, vup;

	Container_GetFrames(cont, ref, &frame, &refFrame);
	frame->GetOrientation(refFrame, (LPD3DVECTOR) &vdir, (LPD3DVECTOR) &vup);
	if (dir) *dir = vdir;
	if (up) *up = vup;
}

// <LegoRR.exe @00475840>
void __cdecl Gods98::Container_AddRotation(Container* cont, Container_Combine_Type combine,
								real32 x, real32 y, real32 z, real32 angle)
{
	IDirect3DRMFrame3* frame;
	Container_DebugCheckOK(cont);

	frame = cont->masterFrame;
	Error_Fatal(!frame, "Container has no masterFrame");

	frame->AddRotation((D3DRMCOMBINETYPE)combine, x, y, z, angle);

// unused preprocessor in LegoRR
#ifdef CONTAINER_MATCHHIDDENHIERARCHY
	if (Container_Reference != cont->type) {
		frame = cont->hiddenFrame;
		frame->AddRotation((D3DRMCOMBINETYPE)combine, x, y, z, angle);
	}
#endif // CONTAINER_MATCHHIDDENHIERARCHY
}

// <LegoRR.exe @00475870>
void __cdecl Gods98::Container_AddScale(Container* cont, Container_Combine_Type combine,
								real32 x, real32 y, real32 z)
{
	IDirect3DRMFrame3* frame;
	Container_DebugCheckOK(cont);

	frame = cont->masterFrame;
	Error_Fatal(!frame, "Container has no masterFrame");

	frame->AddScale((D3DRMCOMBINETYPE)combine, x, y, z);

// unused preprocessor in LegoRR
#ifdef CONTAINER_MATCHHIDDENHIERARCHY
	if (Container_Reference != cont->type) {
		frame = cont->hiddenFrame;
		frame->AddScale((D3DRMCOMBINETYPE)combine, x, y, z);
	}
#endif // CONTAINER_MATCHHIDDENHIERARCHY
}

// <LegoRR.exe @004758a0>
void __cdecl Gods98::Container_AddTranslation(Container* cont, Container_Combine_Type combine,
								real32 x, real32 y, real32 z)
{
	IDirect3DRMFrame3* frame;
	Container_DebugCheckOK(cont);

	frame = cont->masterFrame;
	Error_Fatal(!frame, "Container has no masterFrame");

	frame->AddTranslation((D3DRMCOMBINETYPE)combine, x, y, z);

// unused preprocessor in LegoRR
#ifdef CONTAINER_MATCHHIDDENHIERARCHY
	if (Container_Reference != cont->type) {
		frame = cont->hiddenFrame;
		frame->AddTranslation((D3DRMCOMBINETYPE)combine, x, y, z);
	}
#endif // CONTAINER_MATCHHIDDENHIERARCHY
}

// <LegoRR.exe @004758d0>
void __cdecl Gods98::Container_ClearTransform(Container* cont)
{
	Matrix4F mat;
	mat[0][0] = 1.0f;	mat[1][0] = 0.0f;	mat[2][0] = 0.0f;	mat[3][0] = 0.0f;
	mat[0][1] = 0.0f;	mat[1][1] = 1.0f;	mat[2][1] = 0.0f;	mat[3][1] = 0.0f;
	mat[0][2] = 0.0f;	mat[1][2] = 0.0f;	mat[2][2] = 1.0f;	mat[3][2] = 0.0f;
	mat[0][3] = 0.0f;	mat[1][3] = 0.0f;	mat[2][3] = 0.0f;	mat[3][3] = 1.0f;

	Container_AddTransform(cont, Container_Combine_Type::Container_Combine_Replace, mat);
}

// <LegoRR.exe @00475970>
void __cdecl Gods98::Container_AddTransform(Container* cont, Container_Combine_Type combine,
								const Matrix4F mat)
{
	IDirect3DRMFrame3* frame;
	Container_DebugCheckOK(cont);

	frame = cont->masterFrame;
	Error_Fatal(!frame, "Container has no masterFrame");

	frame->AddTransform((D3DRMCOMBINETYPE)combine, const_cast<D3DVALUE(*)[4]>(mat));

// unused preprocessor in LegoRR
#ifdef CONTAINER_MATCHHIDDENHIERARCHY
	if (Container_Reference != cont->type) {
		frame = cont->hiddenFrame;
		frame->AddTransform((D3DRMCOMBINETYPE)combine, mat);
	}
#endif // CONTAINER_MATCHHIDDENHIERARCHY
}

// <LegoRR.exe @00475990>
real32 __cdecl Gods98::Container_GetZXRatio(Container* cont)
{
	IDirect3DRMFrame3* frame, *parent;
	Matrix4F mat;
	Container_DebugCheckOK(cont);

	frame = cont->masterFrame;
	Error_Fatal(!frame, "Container has no masterFrame");

	frame->GetParent(&parent);
	frame->GetTransform(parent, mat);
	parent->Release();

	return mat[2][2] / mat[0][0];
}

// <LegoRR.exe @004759d0>
void __cdecl Gods98::Container_SetParent(Container* child, OPTIONAL Container* parent)
{
	// Pass NULL as the parent to unhook the Container...

	IDirect3DRMFrame3* parentFrame, *childFrame, *childFrameHidden;
//	IDirect3DRMFrame* tempFrame1;
	Container_DebugCheckOK(child);

	childFrame = child->masterFrame;
	childFrameHidden = child->hiddenFrame;
	Error_Fatal(!childFrame||!childFrameHidden, "Child has no master/hiddenFrame");
	
	if (parent == nullptr) {

		// Delete the Child from its existing parent...
		childFrame->GetParent(&parentFrame);
//		parentFrame = CUF(tempFrame1);
		if (parentFrame) {
			parentFrame->DeleteChild(childFrame);
			parentFrame->Release();
		}
		// else Frame was already unparented...

		// Do the same with the hidden frame...
		childFrame->GetParent(&parentFrame);
//		parentFrame = CUF(tempFrame1);
		if (parentFrame) {
			parentFrame->DeleteChild(childFrameHidden);
			parentFrame->Release();
		}

	} else {
	
		parentFrame = parent->masterFrame;
		Error_Fatal(!parentFrame, "Parent has no masterFrame");
//		if (parentFrame->AddChild(CDF(childFrame)) == D3DRM_OK){
//		} else Error_Fatal(true, "Unable to add childFrame to parentFrame");

		Container_Frame_SafeAddChild(parentFrame, childFrame);

		// Do the same with the hidden frame...
		parentFrame = parent->hiddenFrame;
		Container_Frame_SafeAddChild(parentFrame, childFrameHidden);
	}
}

// <LegoRR.exe @00475a60>
Gods98::Container* __cdecl Gods98::Container_GetParent(Container* child)
{
	IDirect3DRMFrame3* childFrame, *parentFrame;
//	IDirect3DRMFrame* tempFrame1;
	Container* parent;

	Container_DebugCheckOK(child);

	// Special condition - Root frame 'may' have a parent with no container attached
	// (one that is at a level above the Containers hierarchy scope)...
	if (child == containerGlobs.rootContainer) return nullptr;

	childFrame = child->masterFrame;
	Error_Fatal(!childFrame, "Child has no masterFrame");

	childFrame->GetParent(&parentFrame);

	if (parentFrame) {
		parent = Container_Frame_GetContainer(parentFrame);
		parentFrame->Release();
	}
	else parent = nullptr;

	return parent;
}

// <LegoRR.exe @00475ab0>
real32 __cdecl Gods98::Container_GetTransCoef(Container* cont)
{
	IDirect3DRMFrame3* actframe;

	Container_DebugCheckOK(cont);

	if (cont->type == Container_Type::Container_FromActivity){

		if (cont->typeData != nullptr) {
			if (cont->typeData->name != nullptr) {
				if (actframe = Container_Frame_Find(cont, cont->typeData->name, 0)){
					return Container_Frame_GetTransCo(actframe);
				}
			}
		}
	} else Error_Warn(true, "Container_GetTransCoef() called with non-activity object");

	return 0.0f;
}

// <LegoRR.exe @00475af0>
Gods98::Container* __cdecl Gods98::Container_SearchOwner(IDirect3DRMFrame3* frame)
{
	// Search upwards from a frame until a non-reference container is found...

	Container* owner = nullptr, *test;
	IDirect3DRMFrame3* parent;
	HRESULT r;

	while (owner == nullptr && frame != nullptr){
		if (test = Container_Frame_GetOwner(frame)){
			if (test->type != Container_Type::Container_Reference) owner = test;
		}
		frame->GetParent(&parent);
		if (parent) {
			frame = parent;
			r = parent->Release();
		} else frame = nullptr;
	}

	return owner;
}

// <LegoRR.exe @00475b40>
Gods98::Container* __cdecl Gods98::Container_Frame_GetContainer(IDirect3DRMFrame3* frame)
{
	// Unlike Container_Frame_GetOwner() this will create a Container if there is
	// none already assigned...

	Container* cont;
	HRESULT r;

	Container_DebugCheckOK(frame);

	if ((cont = Container_Frame_GetOwner(frame)) == nullptr) {

		// Create a new Container (along with redundant master frame).
		if (cont = Container_Create(nullptr)) {

			/*
			// Then move over the activity frame to the 'real' master frame,
			Container_Frame_SafeAddChild(frame, cont->activityFrame);
			// Release the unused master frame and replace it with the correct one.
			Container_Frame_RemoveAppData(cont->masterFrame);
*/
// Remove the redundant frames from the container...
			r = cont->masterFrame->Release();
			r = cont->activityFrame->Release();
			//			r = cont->hiddenFrame->Release();

			cont->masterFrame = frame;
			//			cont->hiddenFrame = nullptr;
			cont->activityFrame = nullptr;

			cont->type = Container_Type::Container_Reference;

			frame->AddDestroyCallback(Container_Frame_ReferenceDestroyCallback, nullptr);

			Container_Frame_SetAppData(frame, cont, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

		}
		else Error_Warn(true, "Couldn't create Container to surround frame");

	}

	return cont;
}


// This implementation is used for Gods98 compiler option: #ifndef CONTAINER_MATCHHIDDENHIERARCHY
// <LegoRR.exe @00475bc0>
void __cdecl Gods98::Container_GetFrames(Container* cont, OPTIONAL Container* ref, OUT IDirect3DRMFrame3** contFrame, OUT IDirect3DRMFrame3** refFrame)
{
	Container_DebugCheckOK(cont);
	*contFrame = cont->masterFrame;
	Error_Fatal(!(*contFrame), "Container has no masterFrame");
	if (ref == nullptr) *refFrame = containerGlobs.rootContainer->masterFrame;
	else *refFrame = ref->masterFrame;
}


// <LegoRR.exe @00475bf0>
Gods98::Container_Type __cdecl Gods98::Container_ParseTypeString(const char* str, OUT bool32* noTexture)
{
	// For now, just check against one type...

	char* argv[10];
	char string[40];

	Container_DebugCheckOK(CONTAINER_DEBUG_NOTREQUIRED);

	if (str != nullptr){
		std::strcpy(string, str);
		uint32 argc = Util_Tokenise(string, argv, ":");
		if (argc > 1 && ::_stricmp(argv[1], "NOTEXTURE") == 0) *noTexture = true;
		else *noTexture = false;

		for (uint32 loop=0 ; loop<Container_Type::Container_TypeCount ; loop++){
			if (containerGlobs.typeName[loop] != nullptr){
				if (::_stricmp(containerGlobs.typeName[loop], string) == 0) {
					return (Container_Type)loop;
				}
			}
		}
	} else Error_Fatal(true, "Null string passed to Container_ParseTypeString()");
	return Container_Type::Container_Invalid;
}

// <LegoRR.exe @00475cb0>
void __cdecl Gods98::Container_AddList(void)
{
	Container_DebugCheckOK(CONTAINER_DEBUG_NOTREQUIRED);

	Error_Fatal(containerGlobs.listCount+1 >= CONTAINER_MAXLISTS, "Run out of lists");

	uint32 count = 0x00000001 << containerGlobs.listCount;

	Container* list;
	if (list = containerGlobs.listSet[containerGlobs.listCount] = (Container*)Mem_Alloc(sizeof(Container) * count)){

		containerGlobs.listCount++;

		for (uint32 loop=1 ; loop<count ; loop++){

//			list[loop-1].flags = ContainerFlags::CONTAINER_FLAG_INITIALISED;

			list[loop-1].nextFree = &list[loop];
		}
		list[count-1].nextFree = containerGlobs.freeList;
		containerGlobs.freeList = list;

	} else Error_Fatal(true, Error_Format("Unable to allocate %d bytes of memory for new list.\n", sizeof(Container) * count));
}

// <LegoRR.exe @00475d30>
uint32 __cdecl Gods98::Container_GetActivities(Container* cont, OUT IDirect3DRMFrame3** frameList, OUT AnimClone** acList, OUT char** nameList)
{
	// Either as List or nameList may be passed as NULL in which case they will not
	// be filled in (if both are NULL the result is the size of array required to hold them)...
	// If nameList is not passed as NULL, the LPUCHAR pointers returned MUST BE FREED
	// AFTER USE!!!!

	IDirect3DRMFrameArray* children;
	/// FIX APPLY: dummy init
	IDirect3DRMFrame3* sourceFrame = nullptr;
	IDirect3DRMFrame3* childFrame;
	IDirect3DRMFrame* frame1;
	uint32 listSize=0;
	DWORD nameLen = 0;
	char* name;
	HRESULT r;

	if (cont->type == Container_Type::Container_FromActivity){
		for (uint32 source=0 ; source<2 ; source++){

			if (source == 0) sourceFrame = cont->activityFrame;
			if (source == 1) sourceFrame = cont->hiddenFrame;

			if (sourceFrame->GetChildren(&children) == D3DRM_OK){
				uint32 count = children->GetSize();
//				Error_Warn(!count, "Can't find any children on frame");
				for (uint32 loop=0 ; loop<count ; loop++){
					children->GetElement(loop, &frame1);

					r = frame1->QueryInterface(Idl::IID_IDirect3DRMFrame3, (void**)&childFrame);
					Error_Fatal(r, "Cannot query frame3");
					frame1->Release();

					childFrame->GetName(&nameLen, nullptr);
					if (nameLen){
						name = (char*)Mem_Alloc(nameLen);
						childFrame->GetName(&nameLen, name);

						if (::_strnicmp(name, CONTAINER_ACTIVITYFRAMEPREFIX, std::strlen(CONTAINER_ACTIVITYFRAMEPREFIX)) == 0){
							if (frameList != nullptr) frameList[listSize] = childFrame;
							if (acList != nullptr) acList[listSize] = Container_Frame_GetAnimClone(childFrame);
							if (nameList != nullptr) nameList[listSize] = name;
							listSize++;
		
							if (nameList == nullptr) Mem_Free(name);
						} else Mem_Free(name);
					}
					r = childFrame->Release();
				}
				r = children->Release();

			} else Error_Fatal(true, "GetChildren() call failed");
		}
	} else Error_Fatal(true, "Container_GetActivities() supplied with a non-activity object");

	return listSize;
}

// <LegoRR.exe @00475ec0>
void __cdecl Gods98::Container_SetTypeData(Container* cont, OPTIONAL char* name, OPTIONAL IDirect3DRMLight* light, OPTIONAL IDirect3DRMMesh* mesh, OPTIONAL Mesh* transMesh)
{
	Container_DebugCheckOK(cont);

	if (cont->typeData) {
		if (name != nullptr) cont->typeData->name = name;
		if (light != nullptr) cont->typeData->light = light;
		if (mesh != nullptr) cont->typeData->mesh = mesh;
		if (transMesh != nullptr) cont->typeData->transMesh = transMesh;
	}
	else {
		cont->typeData = (Container_TypeData*)Mem_Alloc(sizeof(Container_TypeData));
		cont->typeData->name = name;
		cont->typeData->light = light;
		cont->typeData->mesh = mesh;
		cont->typeData->transMesh = transMesh;
	}
}

// <LegoRR.exe @00475f40>
void __cdecl Gods98::Container_FreeTypeData(Container* cont)
{
	HRESULT r;

	if (cont->typeData) {
		if (cont->type == Container_Type::Container_FromActivity) {
			Mem_Free(cont->typeData->name);
		}
		else if (cont->type == Container_Type::Container_Light) {
			r = cont->typeData->light->Release();
		}
		else if (cont->type == Container_Type::Container_Mesh) {
			if (cont->typeData->mesh) {
				cont->activityFrame->DeleteVisual((IUnknown*)cont->typeData->mesh);
				r = cont->typeData->mesh->Release();
			}
		}
		else if (cont->type == Container_Type::Container_LWO) {
			if (cont->typeData->transMesh)
				Mesh_Remove(cont->typeData->transMesh, cont->activityFrame);
		}
		else Error_Warn(true, Error_Format("Code not implemented for Container type #%d", cont->type));
		Mem_Free(cont->typeData);
		cont->typeData = nullptr;
	}
}

// <LegoRR.exe @00475fd0>
bool32 __cdecl Gods98::Container_AddActivity2(Container* cont, const char* filename, const char* actname, real32 transCo, uint32 trigger, const char* sample, AnimClone* origClone, bool32 lws, bool32 looping)
{
	IDirect3DRMFrame3* newFrame;
//	IDirect3DRMAnimationSet* animSet;
	char xFile[UTIL_DEFSTRINGLEN];
	uint32 frameCount;
	AnimClone* animClone;

	std::sprintf(xFile, "%s.%s", filename, containerGlobs.extensionName[Container_Type::Container_Anim]);

	if (lpD3DRM()->CreateFrame(cont->hiddenFrame, &newFrame) == D3DRM_OK){
		Container_NoteCreation(newFrame);

		// Set the name of the parent to the animation set.
		// Freed when the container is destroyed

		// This will also set the appData's pointer to the name... 
		Container_Frame_FormatName(newFrame, "%s_%s", CONTAINER_ACTIVITYFRAMEPREFIX, actname);

		// Load in the AnimationSet.
//		if (animSet = Container_LoadAnimSet(xFile, newFrame, fref, &frameCount)){
		if (origClone == nullptr) {
			animClone = Container_LoadAnimSet(xFile, newFrame, &frameCount, lws, looping);
		} else {
			animClone = AnimClone_Make(origClone, newFrame, &frameCount);
		}

			Container_Frame_SetAppData(newFrame, cont, animClone, filename, &frameCount, nullptr, nullptr, &transCo, sample, nullptr, &trigger);

			return true;
			
//		} else Error_Warn(true, Error_Format("Unable to load animset %s", xFile));
		
	} else Error_Warn(true, "Unable to create frame for new activity");

	return false;
}

// <LegoRR.exe @004760d0>
void __cdecl Gods98::Container_Frame_ReferenceDestroyCallback(LPDIRECT3DRMOBJECT lpD3DRMobj, LPVOID lpArg)
{
	IDirect3DRMFrame3* frame = (IDirect3DRMFrame3*)lpD3DRMobj;
	Container* cont;

	Error_Warn(true, "Reference container's frame is being destroyed");

	if (cont = Container_Frame_GetOwner(frame)) {
		cont->flags |= ContainerFlags::CONTAINER_FLAG_DEADREFERENCE;
		Container_Frame_RemoveAppData(frame);
	}
}

// <LegoRR.exe @00476100>
IDirect3DRMFrame3* __cdecl Gods98::Container_Frame_Find(Container* cont, const char* findName, bool32 /*uint32*/ hidden)
{
	IDirect3DRMFrame3* frame, *childFrame, *foundFrame = nullptr;
	IDirect3DRMFrame* frame1;
	IDirect3DRMFrameArray* children;
	DWORD nameLen;
	char tempString[UTIL_DEFSTRINGLEN];
	char* name;
	HRESULT r;

	if (hidden) frame = cont->hiddenFrame;
	else frame = cont->activityFrame;

	if (frame->GetChildren(&children) == D3DRM_OK){
		uint32 count = children->GetSize();
		Error_Warn(!count, "Can't find any children on frame");
		for (uint32 loop=0 ; loop<count ; loop++){
			children->GetElement(loop, &frame1);

			r = frame1->QueryInterface(Idl::IID_IDirect3DRMFrame3, (void**)&childFrame);
			Error_Fatal(r, "Error getting frame3");
			frame1->Release();

			childFrame->GetName(&nameLen, nullptr);
			if (nameLen) {
				name = (char*)Mem_Alloc(nameLen+1);
				childFrame->GetName(&nameLen, name);

				std::sprintf(tempString, "%s_%s", CONTAINER_ACTIVITYFRAMEPREFIX, findName);
				if (::_stricmp(name, tempString) == 0) foundFrame = childFrame;
				Mem_Free(name);
			}

			r = childFrame->Release();

			if (foundFrame) break;
		}
		r = children->Release();

	} else Error_Fatal(true, "GetChildren() call failed");

	return foundFrame;
}

// <LegoRR.exe @00476230>
void __cdecl Gods98::Container_Frame_SetAppData(IDirect3DRMFrame3* frame, Container* owner,
	OPTIONAL AnimClone* animClone, OPTIONAL const char* asfname, OPTIONAL uint32* frameCount,
	OPTIONAL const char* frameName, OPTIONAL real32* currTime, OPTIONAL real32* transCo,
	OPTIONAL const char* actSample, OPTIONAL void* soundRecord, OPTIONAL uint32* trigger)
{
	Container_AppData* appData;

	// If the AppData already exists then change the data (if specified)
	// Otherwise, create the new AppData and set the values..

//#pragma message ( "Sound3D removal changes" )

	if (appData = (Container_AppData*)frame->GetAppData()){
		if (owner != nullptr) appData->ownerContainer = owner;
		if (animClone != nullptr) appData->animClone = animClone;
		if (asfname != nullptr && appData->animSetFileName != nullptr) Mem_Free(appData->animSetFileName);
		if (frameCount != nullptr) appData->frameCount = *frameCount;
		if (frameName != nullptr) appData->frameName = frameName;
		if (currTime != nullptr) appData->currTime = *currTime;
		if (transCo != nullptr) appData->transCo = *transCo;
		if (actSample != nullptr) appData->activitySample = actSample;
		/// OLD LEGORR: Uncommented assignment of SoundRecord, used by LegoRR
		if (soundRecord != nullptr) appData->soundList = (Sound3D_SoundFrameRecord*)soundRecord;
		if (trigger != nullptr) appData->trigger = *trigger;
	} else {
		appData = (Container_AppData*)Mem_Alloc(sizeof(Container_AppData));
		appData->ownerContainer = owner;
		appData->animClone = animClone;
		appData->animSetFileName = nullptr;
		if (frameCount) appData->frameCount = *frameCount;
		else appData->frameCount = 0;
		appData->frameName = frameName;
		if (currTime) appData->currTime = *currTime;
		else appData->currTime = 0.0f;
		if (transCo) appData->transCo = *transCo;
		else appData->transCo = 0.0f;
		if (trigger) appData->trigger = *trigger;
		else appData->trigger = 0;
		appData->activitySample = actSample;
		/// OLD LEGORR: Uncommented assignment of SoundRecord, used by LegoRR
		appData->soundList = (Sound3D_SoundFrameRecord*)soundRecord;
		frame->SetAppData((DWORD)appData);
	}

	if (asfname != nullptr){
		appData->animSetFileName = (char*)Mem_Alloc(std::strlen(asfname)+1);
		std::strcpy(appData->animSetFileName, asfname);
	}
}

// <LegoRR.exe @004763a0>
void __cdecl Gods98::Container_Frame_RemoveAppData(IDirect3DRMFrame3* frame)
{
	Container_AppData* appData = (Container_AppData*)frame->GetAppData();
	Error_Fatal(!appData, "AppData not set on frame");
	if (appData) {
		if (appData->animSetFileName != nullptr) Mem_Free(appData->animSetFileName);
		Mem_Free(appData);
	}
	frame->SetAppData((DWORD)nullptr);
}

// <LegoRR.exe @004763e0>
Gods98::Container* __cdecl Gods98::Container_Frame_GetOwner(IDirect3DRMFrame3* frame)
{
	Container_AppData* appData = (Container_AppData*)frame->GetAppData();
//	Error_Warn(!appData, Error_Format("AppData not set on frame 0x%0.8x", frame));
	if (appData) return appData->ownerContainer;
	return nullptr;
}

// <LegoRR.exe @00476400>
const char* __cdecl Gods98::Container_Frame_GetAnimSetFileName(IDirect3DRMFrame3* frame)
{
	Container_AppData* appData = (Container_AppData*)frame->GetAppData();
	Error_Fatal(!appData, "AppData not set on frame");
	if (appData) return appData->animSetFileName;
	return nullptr;
}

// <LegoRR.exe @00476420>
Gods98::AnimClone* __cdecl Gods98::Container_Frame_GetAnimClone(IDirect3DRMFrame3* frame)
{
	Container_AppData* appData = (Container_AppData*)frame->GetAppData();
	Error_Fatal(!appData, "AppData not set on frame");
	if (appData) return appData->animClone;
	return nullptr;
}

// <LegoRR.exe @00476440>
uint32 __cdecl Gods98::Container_Frame_GetFrameCount(IDirect3DRMFrame3* frame)
{
	Container_AppData* appData = (Container_AppData*)frame->GetAppData();
	Error_Fatal(!appData, "AppData not set on frame");
	if (appData) return appData->frameCount;
	return 0;
}

// <LegoRR.exe @00476460>
real32 __cdecl Gods98::Container_Frame_GetCurrTime(IDirect3DRMFrame3* frame)
{
	Container_AppData* appData = (Container_AppData*)frame->GetAppData();
	Error_Fatal(!appData, "AppData not set on frame");
	if (appData) return appData->currTime;
	return 0.0f;
}

// <LegoRR.exe @00476480>
real32 __cdecl Gods98::Container_Frame_GetTransCo(IDirect3DRMFrame3* frame)
{
	Container_AppData* appData = (Container_AppData*)frame->GetAppData();
	Error_Fatal(!appData, "AppData not set on frame");
	if (appData) return appData->transCo;
	return 0.0f;
}

// <LegoRR.exe @004764a0>
const char* __cdecl Gods98::Container_Frame_GetSample(IDirect3DRMFrame3* frame)
{
	Container_AppData* appData = (Container_AppData*)frame->GetAppData();
	Error_Fatal(!appData, "AppData not set on frame");
	if (appData) return appData->activitySample;
	return nullptr;
}

// <LegoRR.exe @004764c0>
uint32 __cdecl Gods98::Container_Frame_GetTrigger(IDirect3DRMFrame3* frame)
{
	Container_AppData* appData = (Container_AppData*)frame->GetAppData();
	Error_Fatal(!appData, "AppData not set on frame");
	if (appData) return appData->trigger;
	return 0;
}

// <LegoRR.exe @004764e0>
void __cdecl Gods98::Container_Frame_SafeAddChild(IDirect3DRMFrame3* parent, IDirect3DRMFrame3* child)
{
	// Stop addchild from corrupting the transformation matrix...
	D3DRMMATRIX4D mat; // same as Matrix4F
	IDirect3DRMFrame3* oldParent;

	child->GetParent(&oldParent);
	child->GetTransform(oldParent, mat);
	if (oldParent) oldParent->Release();
	parent->AddChild(child);
	child->AddTransform(D3DRMCOMBINE_REPLACE, mat);
}

// <LegoRR.exe @00476530>
void __cdecl Gods98::Container_Frame_FormatName(IDirect3DRMFrame3* frame, const char* msg, ...)
{
	char buffer[1024];
	std::va_list args;
	
	va_start(args, msg);
	uint32 len = std::vsprintf(buffer, msg, args);
	va_end(args);

	char* name = (char*)Mem_Alloc(len+1);
	va_start(args, msg);
	std::vsprintf(name, msg, args);
	va_end(args);

	frame->SetName(name);
	Container_Frame_SetAppData(frame, nullptr, nullptr, nullptr, nullptr, name, nullptr, nullptr, nullptr, nullptr, nullptr);
}

// <LegoRR.exe @004765b0>
void __cdecl Gods98::Container_Frame_FreeName(IDirect3DRMFrame3* frame)
{
	char* name;
	if (name = const_cast<char*>(Container_Frame_GetName(frame))){
		Mem_Free(name);
	}
}

// <LegoRR.exe @004765d0>
const char* __cdecl Gods98::Container_Frame_GetName(IDirect3DRMFrame3* frame)
{
	Container_AppData* appData = (Container_AppData*)frame->GetAppData();
	Error_Fatal(!appData, "AppData not set on frame");
	if (appData) return appData->frameName;
	return nullptr;
}

// <LegoRR.exe @004765f0>
bool32 __cdecl Gods98::Container_Frame_WalkTree(IDirect3DRMFrame3* frame, uint32 level,
									ContainerWalkTreeCallback Callback, void* data)
{
	IDirect3DRMFrameArray* children;
	IDirect3DRMFrame3* child;
	IDirect3DRMFrame* child1;
	bool32 finished = false;
	HRESULT r;

	if (Callback(frame, data)) return true;

	frame->GetChildren(&children);
	uint32 count = children->GetSize();
	for (uint32 loop=0 ; loop<count ; loop++){
		children->GetElement(loop, &child1);
		child1->QueryInterface(Idl::IID_IDirect3DRMFrame3, (void**)&child);
		child1->Release();
		if (Container_Frame_WalkTree(child, level+1, Callback, data)) {
			finished = true;
			r = child->Release();
			break;
		}
		r = child->Release();
	}
	r = children->Release();

	return finished;
}

// <LegoRR.exe @004766d0>
bool32 __cdecl Gods98::Container_Frame_SearchCallback(IDirect3DRMFrame3* frame, void* data)
{
	Container_SearchData* search = (Container_SearchData*)data;
	DWORD len;

	frame->GetName(&len, nullptr);

	if (len-1 == search->stringLen){
		char* name = (char*)Mem_Alloc(len);
		name[0] = '\0';
		frame->GetName(&len, name);

		// Replace any characters in the name string with '?' if their position
		// corresponds to a '?' in the search string...
		for (uint32 loop=0 ; loop<len ; loop++) if (search->string[loop] == '?') name[loop] = '?';

		if (search->mode == Container_SearchMode::Container_SearchMode_FirstMatch) {
			search->resultFrame = nullptr;
			if (search->caseSensitive) {
				if (std::strcmp(name, search->string) == 0) search->resultFrame = frame;
			} else {
				if (::_stricmp(name, search->string) == 0) search->resultFrame = frame;
			}
		} else if (search->mode == Container_SearchMode::Container_SearchMode_MatchCount) {
			if (search->caseSensitive) {
				if (std::strcmp(name, search->string) == 0) search->count++;
			} else {
				if (::_stricmp(name, search->string) == 0) search->count++;
			}
		} else if (search->mode == Container_SearchMode::Container_SearchMode_NthMatch){
			search->resultFrame = nullptr;
			if (search->caseSensitive) {
				if (std::strcmp(name, search->string) == 0) search->count++;
			} else {
				if (::_stricmp(name, search->string) == 0) search->count++;
			}
			if (search->count == search->matchNumber + 1) search->resultFrame = frame;
		} else Error_Fatal(true, "Unknown search type");

		Mem_Free(name);

		if (search->resultFrame) return true;

	}
	return false;
}

// <LegoRR.exe @00476880>
Gods98::AnimClone* __cdecl Gods98::Container_LoadAnimSet(const char* fname, IDirect3DRMFrame3* frame, OUT uint32* frameCount, bool32 lws, bool32 looping)
{
	IDirect3DRMAnimationSet2* animSet = nullptr;
	IDirect3DRMFrame3* rootFrame;
	D3DRMLOADMEMORY buffer;
	Container_TextureData tData;
	AnimClone* animClone = nullptr;
	uint32 fc;

#ifdef _DEBUG_2
/*	{
		static char nameList[1024][128];
		static uint32 nameCount = 0;

		Error_Fatal(strlen(fname) > 128, "Name too long");
		Error_Fatal(nameCount > 1024, "name list overflowed");
		for (uint32 loop=0 ; loop<nameCount ; loop++) {
			Error_Fatal(0 == stricmp(fname, nameList[loop]), Error_Format("Cannot load the same x animation file twice\n(%s)", fname));
		}
		std::strcpy(nameList[nameCount++], fname);
	}*/
#endif // _DEBUG_2

	Error_Debug(Error_Format("Attempting to load animation from xfile \"%s\"\n", fname));


	if (lws) {

		Lws_Info* scene;
		char file[FILE_MAXPATH];

		std::strcpy(file, fname);
		file[std::strlen(file)-2] = '\0';
		if (scene = Lws_Parse(file, looping)) {
			Lws_LoadMeshes(scene, frame);
			Lws_SetTime(scene, 0.0f);
			if (frameCount) *frameCount = Lws_GetFrameCount(scene);
			animClone = AnimClone_RegisterLws(scene, frame, *frameCount);
		} else Error_Warn(true, "Cannot load file");

	} else {

		if (buffer.lpMemory = File_LoadBinary(fname, (uint32*)&buffer.dSize)){
			if (lpD3DRM()->CreateFrame(frame, &rootFrame) == D3DRM_OK){
				Container_NoteCreation(rootFrame);
				if (lpD3DRM()->CreateAnimationSet(&animSet) == D3DRM_OK){
					Container_NoteCreation(animSet);
					
					tData.xFileName = fname;
					tData.flags = Container_TextureFlags::CONTAINER_TEXTURE_NONE;
					
					if (animSet->Load(&buffer, nullptr, D3DRMLOAD_FROMMEMORY, Container_TextureLoadCallback, &tData, rootFrame) == D3DRM_OK){
	//				if (animSet->Load(&buffer, nullptr, D3DRMLOAD_FROMMEMORY, nullptr, nullptr, rootFrame) == D3DRM_OK){
						animSet->SetTime(0.0f);

	/*					{	// Set the animations to use linear interpolation...
							IDirect3DRMAnimationArray* anims;
							IDirect3DRMAnimation2* anim;
							D3DRMANIMATIONOPTIONS options;

							if (D3DRM_OK == animSet->GetAnimations(&anims)){
								uint32 animCount = anims->GetSize();
								for (uint32 loop=0 ; loop<animCount ; loop++) {
									anims->GetElement(loop, &anim);
									options = anim->GetOptions();
									anim->SetOptions(D3DRMANIMATION_LINEARPOSITION|D3DRMANIMATION_OPEN|D3DRMANIMATION_POSITION);
									anim->Release();
								}
								anims->Release();
							}
						}*/

						fc = Container_GetAnimFileFrameCount((const char*)buffer.lpMemory);
						if (frameCount) *frameCount = fc;
						animClone = AnimClone_Register(animSet, rootFrame, fc);
					}
					Mem_Free(buffer.lpMemory);
				}
				rootFrame->Release();
			}
		}
	}

	return animClone;
}

// <LegoRR.exe @00476a30>
uint32 __cdecl Gods98::Container_GetAnimFileFrameCount(const char* fileData)
{
	uint32 count = 0;

	if (fileData = std::strstr(fileData, "DDiScene")){
		while(!std::isdigit(*fileData)) fileData++;
		count = std::atoi(fileData);
	}

	if (count == 0) count = 1;
	return count;
}

// <LegoRR.exe @00476aa0>
bool32 __cdecl Gods98::Container_FrameLoad(const char* fname, IDirect3DRMFrame3* frame)
{
	bool32 res = false;
	D3DRMLOADMEMORY buffer;
	Container_TextureData tData;

	if (buffer.lpMemory = File_LoadBinary(fname, (uint32*)&buffer.dSize)){

		tData.xFileName = fname;
		tData.flags = Container_TextureFlags::CONTAINER_TEXTURE_NONE;

		if (frame->Load(&buffer, nullptr, D3DRMLOAD_FROMMEMORY, Container_TextureLoadCallback, &tData) == D3DRM_OK){
//		if (frame->Load(&buffer, nullptr, D3DRMLOAD_FROMMEMORY, nullptr, nullptr) == D3DRM_OK){
			res = true;
		}
		Mem_Free(buffer.lpMemory);
	}

	return res;
}

// <LegoRR.exe @00476b10>
IDirect3DRMMesh* __cdecl Gods98::Container_MeshLoad(void* file_data, uint32 file_size, const char* file_name, IDirect3DRMFrame3* frame, bool32 noTexture)
{
	IDirect3DRMMeshBuilder3* mb;
	IDirect3DRMMesh* mesh;
	HRESULT r;
	D3DRMLOADMEMORY buffer;
	Container_TextureData tData;

	buffer.lpMemory = file_data;
	buffer.dSize = file_size;

	if (lpD3DRM()->CreateMeshBuilder(&mb) == D3DRM_OK){
		Container_NoteCreation(mb);

		tData.xFileName = file_name;
		tData.flags = Container_TextureFlags::CONTAINER_TEXTURE_NONE;
		if (noTexture) tData.flags |= Container_TextureFlags::CONTAINER_TEXTURE_NOLOAD;

		if (mb->Load(&buffer, nullptr, D3DRMLOAD_FROMMEMORY, Container_TextureLoadCallback, &tData) == D3DRM_OK){
//		if (mb->Load(&buffer, nullptr, D3DRMLOAD_FROMMEMORY, nullptr, nullptr) == D3DRM_OK){
			mb->CreateMesh(&mesh);
			Container_NoteCreation(mesh);
			r = mb->Release();
			if (frame->AddVisual((IUnknown*)mesh) == D3DRM_OK){

				return mesh;

			} else Error_Fatal(true, "Unable to add visual to frame");
		} else Error_Fatal(true, Error_Format("Unable to load MeshBuilder from memory (%s)", file_name));
		r = mb->Release();
	} else Error_Fatal(true, "Unable create MeshBuilder");

	return nullptr;
}

// <LegoRR.exe @00476bc0>
HRESULT __cdecl Gods98::Container_TextureLoadCallback(char* name, void* data, LPDIRECT3DRMTEXTURE3* texture)
{
	Container_TextureData* textureData = (Container_TextureData*)data;
	Container_Texture* text;

	if (!(textureData->flags & Container_TextureFlags::CONTAINER_TEXTURE_NOLOAD)){

		char path[1024];
		char* tag = nullptr;
	//	Image* image;
	//	IDirect3DRMTexture3* text;

		const char* s = textureData->xFileName;
		char* p = path;

		for (sint32 location=0 ; location<(containerGlobs.sharedDir?2:1) ; location++) {

			if (location == 0) {
				// Strip the xfilename off the end of the path...
				while (*s != '\0') {
					if (*s == '\\') {
						if (tag) *tag = '\\';
						*p = '\0';
						tag = p;
					} else *p = *s;
					s++;
					p++;
				}
				if (tag){
					std::strcat(path, "\\");
					std::strcat(path, name);
				} else std::strcpy(path, name);
			} else {
				std::sprintf(path, "%s\\%s", containerGlobs.sharedDir, name);
			}

			*texture = nullptr;

			for (uint32 loop=0 ; loop<containerGlobs.textureCount ; loop++){

				if (containerGlobs.textureSet[loop].fileName && (std::strcmp(containerGlobs.textureSet[loop].fileName, path) == 0)){
					*texture = containerGlobs.textureSet[loop].texture;
					(*texture)->AddRef();

					return D3DRM_OK;
				}
			}

			if (text = Container_LoadTexture(path, nullptr, nullptr)){

				*texture = text->texture;

	#ifndef CONTAINER_DONTFLIPTEXTURES
				Container_YFlipTexture(*texture);
	#endif
				if (Main_MIPMapEnabled()) {
					HRESULT r = (*texture)->GenerateMIPMap(0);
				}

				if (containerGlobs.textureCount == CONTAINER_MAXTEXTURES) {

					Error_Warn(true, "Sorting texture list");
					std::qsort(containerGlobs.textureSet, containerGlobs.textureCount, sizeof(Container_TextureRef), Container_TextureSetSort);
					/// FIX APPLY: Infinite for-loop with unsigned `>= 0`, change to `> 0`
					for ( ; containerGlobs.textureCount>0 ; containerGlobs.textureCount--){
						if (containerGlobs.textureSet[containerGlobs.textureCount-1].fileName) break;
					}
					Error_Fatal(containerGlobs.textureCount == CONTAINER_MAXTEXTURES, "CONTAINER_MAXTEXTURES overflowed");

				}

				if (containerGlobs.textureCount < CONTAINER_MAXTEXTURES) {
					Container_TextureRef* textRef = &containerGlobs.textureSet[containerGlobs.textureCount];
					
					(*texture)->AddDestroyCallback(Container_TextureDestroyCallback, textRef);

					textRef->fileName = (char*)Mem_Alloc(strlen(path)+1);
					std::strcpy(textRef->fileName, path);
					textRef->texture = *texture;

					containerGlobs.textureCount++;
				}
				return D3DRM_OK;
			}
		}
	}

	return D3DRMERR_NOTFOUND;
}

// <LegoRR.exe @00476eb0>
void __cdecl Gods98::Container_YFlipTexture(IDirect3DRMTexture3* texture)
{
	D3DRMIMAGE* image;
	if (image = texture->GetImage()){
		sint32 byteWidth = (image->depth * image->width) / 8;

		uint8* buffer;
		if (buffer = (uint8*)Mem_Alloc(byteWidth)){
			uint8* topline = (uint8*) image->buffer1;
			uint8* bottomline = &((uint8*) image->buffer1)[(image->height-1) * image->bytes_per_line];
			
			for (sint32 y=0 ; y<image->height/2 ; y++){
				std::memcpy(buffer, bottomline, byteWidth);
				std::memcpy(bottomline, topline, byteWidth);
				std::memcpy(topline, buffer, byteWidth);
				topline += image->bytes_per_line;
				bottomline -= image->bytes_per_line;
			}

			Mem_Free(buffer);
		} else Error_Warn(true, "Cannot allocate buffer for yflip");
	}
}

// std::qsort callback
// <LegoRR.exe @00476fa0>
int __cdecl Gods98::Container_TextureSetSort(const void* a, const void* b)
{
	if (((Container_TextureRef*)a)->fileName != nullptr && ((Container_TextureRef*)b)->fileName == nullptr) return -1;
	if (((Container_TextureRef*)a)->fileName == nullptr && ((Container_TextureRef*)b)->fileName != nullptr) return 1;
	return 0;
}
//int __cdecl Gods98::Container_TextureSetSort(Container_TextureRef* a, Container_TextureRef* b);

// <LegoRR.exe @00476fd0>
void __cdecl Gods98::Container_TextureDestroyCallback(LPDIRECT3DRMOBJECT lpD3DRMobj, LPVOID lpArg)
{
	Container_TextureRef* textRef = (Container_TextureRef*)lpArg;
	Container_Texture* text;

	Error_Debug(Error_Format("Removing %s from texture list\n", textRef->fileName));

	text = (Container_Texture*)textRef->texture->GetAppData();
	//  (Gods98 source comment)
	// This is deliberatly not Mem_Free()

	legacy::free(text);

	Mem_Free(textRef->fileName);
	textRef->fileName = nullptr;
	textRef->texture = nullptr;
}

#pragma endregion

