// Camera.cpp : 
//

#include "../../engine/core/Errors.h"
#include "../../engine/core/Maths.h"
#include "../../engine/core/Memory.h"
#include "../../engine/gfx/Containers.h"
#include "../../engine/gfx/Viewports.h"

#include "../object/Object.h"
#include "../Game.h"

#include "Map3D.h"

#include "Camera.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005570a0>
LegoRR::Camera_Globs & LegoRR::cameraGlobs = *(LegoRR::Camera_Globs*)0x005570a0;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00435a50>
LegoRR::LegoCamera* __cdecl LegoRR::Camera_Create(Gods98::Container* contRoot, LegoCamera_Type camType)
{
	LegoCamera* cam = (LegoCamera*)Gods98::Mem_Alloc(sizeof(LegoCamera));
	if (cam != nullptr) {
		cam->type = camType;
		cam->trackObj = nullptr;
		cam->trackRadarZoom = 0.0f;
		cam->trackRadarZoomSpeed = 0.0f;
		cam->flags = CameraFlags::CAMERA_FLAG_NONE;
		cam->rotation = 0.0f;
		cam->tilt = 0.0f;
		cam->zoom = 200.0f;
		cam->trackFPCameraFrame = 0;
		cam->moveVector.x = 0.0f;
		cam->moveVector.y = 0.0f;
		cam->moveVector.z = 0.0f;
		cam->moveSpeed = 0.0f;
		cam->shakeTimer = 0.0f;
		cam->shakeDuration = 0.0f;
		cam->shakeIntensity = 0.0f;
		cam->shakeVector.z = 0.0f;
		cam->shakeVector.y = 0.0f;
		cam->shakeVector.x = 0.0f;

		if ((cam->contCam = Gods98::Container_Create(nullptr)) != nullptr) {
			Gods98::Container_SetPosition(cam->contCam, nullptr, 0.0f, 0.0f, -cam->zoom);
			Gods98::Container_SetOrientation(cam->contCam, nullptr, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

			if (camType == LegoCamera_Type::LegoCamera_FP) {
				Gods98::Container_SetParent(cam->contCam, contRoot);
				return cam;
			}

			if ((cam->cont2 = Gods98::Container_Create(contRoot)) != nullptr) {
				if ((cam->cont3 = Gods98::Container_Create(contRoot)) != nullptr) {
					if ((cam->cont4 = Gods98::Container_Create(cam->cont2)) != nullptr) {

						if (camType == LegoCamera_Type::LegoCamera_Top) {

							if ((cam->contListener = Gods98::Container_Create(cam->cont4)) != nullptr) {
								Gods98::Container_SetPosition(cam->contListener, nullptr, 0.0f, 0.0f, -80.0f);
							}
						}

						Gods98::Container_SetParent(cam->contCam, cam->cont4);
						return cam;
					}
					Gods98::Container_Remove(cam->cont3);
				}
				Gods98::Container_Remove(cam->cont2);
			}
			Gods98::Container_Remove(cam->contCam);
		}
		Gods98::Mem_Free(cam);
	}
	return nullptr;
}

// <LegoRR.exe @00435cc1>
void __cdecl LegoRR::Camera_EnableFreeMovement(LegoCamera* cam, bool32 on)
{
	if (on) cam->flags |= CameraFlags::CAMERA_FLAG_FREEMOVEMENT;
	else    cam->flags &= ~CameraFlags::CAMERA_FLAG_FREEMOVEMENT;
}

// <LegoRR.exe @00435cf8>
void __cdecl LegoRR::Camera_Free(LegoCamera* cam)
{
	Gods98::Container_Remove(cam->contCam);
	if (cam->type != LegoCamera_Type::LegoCamera_FP) {
		Gods98::Container_Remove(cam->cont2);
		Gods98::Container_Remove(cam->cont3);

		// I think cam->cont4 and cam->contListener don't need to be removed,
		// since they're always parented to one of the containers removed above(?)
	}
	Gods98::Mem_Free(cam);
}

// <LegoRR.exe @00435d3e>
void __cdecl LegoRR::Camera_InitCameraMovements(real32 maxSpeed, real32 deceleration, real32 acceleration, sint32 mouseScrollIndent)
{
	cameraGlobs.maxSpeed = maxSpeed;
	cameraGlobs.deceleration = deceleration;
	cameraGlobs.acceleration = acceleration;
	cameraGlobs.mouseScrollIndent = mouseScrollIndent;
}

// <LegoRR.exe @00435d65>
void __cdecl LegoRR::Camera_CopyFPPositionOrientation(LegoCamera* dstCamera, LegoCamera* srcCamera)
{
	if (dstCamera->type == LegoCamera_Type::LegoCamera_FP) {
		Vector3F vectorPos;
		Vector3F vectorD, vectorU;
		Gods98::Container_GetPosition(srcCamera->contCam, nullptr, &vectorPos);
		Gods98::Container_GetOrientation(srcCamera->contCam, nullptr, &vectorD, &vectorU);

		Gods98::Container_SetPosition(dstCamera->contCam, nullptr, vectorPos.x, vectorPos.y, vectorPos.z);
		Gods98::Container_SetOrientation(dstCamera->contCam, nullptr, vectorD.x, vectorD.y, vectorD.z, vectorU.x, vectorU.y, vectorU.z);
	}
}

// <LegoRR.exe @00435deb>
void __cdecl LegoRR::Camera_TrackObject(LegoCamera* cam, LegoObject* liveObj, real32 trackZoomSpeed, real32 trackZoom, real32 trackTilt, real32 trackRotationSpeed)
{
	cam->trackObj = liveObj;
	cam->trackRadarZoomSpeed = trackZoomSpeed;
	cam->trackRadarZoom = trackZoom;
	Camera_SetTilt(cam, trackTilt);
	cam->trackRadarRotationSpeed = trackRotationSpeed;
}

// <LegoRR.exe @00435e24>
void __cdecl LegoRR::Camera_SetFPObject(LegoCamera* cam, LegoObject* liveObj, sint32 cameraFrame)
{
	cam->trackObj = liveObj;
	cam->trackFPCameraFrame = cameraFrame;
}

// <LegoRR.exe @00435e3b>
sint32 __cdecl LegoRR::Camera_GetFPCameraFrame(LegoCamera* cam)
{
	return cam->trackFPCameraFrame;
}

// <LegoRR.exe @00435e46>
void __cdecl LegoRR::Camera_StopMovement(LegoCamera* cam)
{
	cam->moveSpeed = 0.0f;
}

// <LegoRR.exe @00435e58>
sint32 __cdecl LegoRR::Camera_GetMouseScrollIndent(void)
{
	return cameraGlobs.mouseScrollIndent;
}

// <LegoRR.exe @00435e62>
void __cdecl LegoRR::Camera_Shake(LegoCamera* cam, real32 intensity, real32 duration)
{
	cam->shakeIntensity = intensity;
	cam->shakeDuration = duration;
	cam->shakeTimer = 0.0f;
}

// <LegoRR.exe @00435e8c>
void __cdecl LegoRR::Camera_Update(LegoCamera* cam, Lego_Level* level, real32 elapsedAbs, real32 elapsedGame)
{
	/*real32 fVar1;
	real32 fVar2;
	real32 fVar3;
	real32 fVar4;
	real32 fVar5;
	real32 fVar6;
	real32 fVar7;
	real32 fVar8;
	real32 fVar9;
	//bool32 bInside;
	//Map3D* map;
	float10 fVar10;
	float10 fVar11;*/

	// Remove current shake translation
	Gods98::Container_AddTranslation(cam->contCam, Gods98::Container_Combine::Before, -cam->shakeVector.x, -cam->shakeVector.y, -cam->shakeVector.z);

	//fVar1 = cameraGlobs.deceleration;
	if (cam->type == LegoCamera_Type::LegoCamera_Top) {
		Vector3F top_vecPos;
		Vector3F top_vecDir;
		Vector3F top_vecUp;

		// Decelerate camera movement.
		Gods98::Maths_Vector3DScale(&cam->moveVector, &cam->moveVector, cameraGlobs.deceleration);
		//cam->moveVector.x = cam->moveVector.x * cameraGlobs.deceleration;
		//cam->moveVector.y = cam->moveVector.y * fVar1;
		//cam->moveVector.z = cam->moveVector.z * fVar1;

		Gods98::Container_AddTranslation(cam->cont2, Gods98::Container_Combine::After, cam->moveVector.x, cam->moveVector.y, cam->moveVector.z);
		Gods98::Container_GetPosition(cam->cont2, nullptr, &top_vecPos);
		Gods98::Container_GetOrientation(cam->cont2, nullptr, &top_vecDir, &top_vecUp);

		uint32 blockX = 0, blockY = 0; // dummy inits
		if (!Map3D_WorldToBlockPos_NoZ(level->map, top_vecPos.x, top_vecPos.y, (sint32*)&blockX, (sint32*)&blockY)) {
			// Cancel out moveVector ??
			Gods98::Container_AddTranslation(cam->cont2, Gods98::Container_Combine::After, -cam->moveVector.x, -cam->moveVector.y, -cam->moveVector.z);
		}
		else {
			if ((blockX < (uint32)cameraGlobs.mouseScrollIndent) ||
				((uint32)(level->map->blockWidth - cameraGlobs.mouseScrollIndent) <= blockX))
			{
				Gods98::Container_AddTranslation(cam->cont2, Gods98::Container_Combine::After, -cam->moveVector.x, 0.0f, 0.0f);
			}
			if ((blockY < (uint32)cameraGlobs.mouseScrollIndent) ||
				((uint32)(level->map->blockHeight - cameraGlobs.mouseScrollIndent) <= blockY))
			{
				Gods98::Container_AddTranslation(cam->cont2, Gods98::Container_Combine::After, 0.0f, -cam->moveVector.y, 0.0f);
			}
			real32 zPos = Map3D_GetWorldZ(level->map, top_vecPos.x, top_vecPos.y);
			Gods98::Container_SetPosition(cam->cont3, nullptr, top_vecPos.x, top_vecPos.y, (real32)zPos);
			Gods98::Container_SetOrientation(cam->cont3, nullptr, top_vecUp.x, top_vecUp.y, 0.0f, 0.0f, 0.0f, -1.0f);


			zPos = Map3D_UnkCameraXYFunc_RetZunk(Lego_GetMap(), top_vecPos.x, top_vecPos.y);
			Gods98::Container_SetPosition(cam->contCam, cam->cont4, 0.0f, 0.0f, -(cam->zoom - zPos));
		}
		//goto LAB_004368f0;
	}
	else if (cam->type == LegoCamera_Type::LegoCamera_Radar) {
		Gods98::Container* radar_objCont;
		Vector3F radar_vecCont1Pos2;
		Vector3F radar_vecCont2Pos;
		Vector3F radar_vecCont1Pos;
		Vector3F radar_vecObjPos;

		if (cam->trackObj == nullptr) {
			Game_SetFlag1_20000_unkCameraRadarHasTrackObj(true);
		}
		else if ((radar_objCont = LegoObject_GetActivityContainer(cam->trackObj)) != nullptr) {
			Game_SetFlag1_20000_unkCameraRadarHasTrackObj(false);


			Gods98::Container_GetPosition(cam->contCam, nullptr, &radar_vecCont1Pos);
			Gods98::Container_GetPosition(radar_objCont, nullptr, &radar_vecObjPos);

			radar_vecObjPos.z -= cam->trackRadarZoom / 10.0f;
			real32 oldZoom = cam->zoom;
			cam->zoom = cam->trackRadarZoom;

			Gods98::Container_SetPosition(cam->contCam, cam->cont4, 0.0f, 0.0f, -cam->zoom);

			Gods98::Container_GetPosition(cam->cont2, nullptr, &radar_vecCont2Pos);
			Gods98::Container_SetPosition(cam->cont2, nullptr, radar_vecObjPos.x, radar_vecObjPos.y, radar_vecObjPos.z);

			Gods98::Container_GetPosition(cam->contCam, nullptr, &radar_vecCont1Pos2);

			real32 dist1 = Gods98::Maths_Vector3DDistance(&radar_vecCont1Pos2, &radar_vecCont1Pos);
			//real32 dist1 = std::sqrt((double)((radar_vecCont1Pos2.z - radar_vecCont1Pos.z) *
			//							(radar_vecCont1Pos2.z - radar_vecCont1Pos.z) +
			//							(radar_vecCont1Pos2.y - radar_vecCont1Pos.y) *
			//							(radar_vecCont1Pos2.y - radar_vecCont1Pos.y) +
			//							(radar_vecCont1Pos2.x - radar_vecCont1Pos.x) *
			//							(radar_vecCont1Pos2.x - radar_vecCont1Pos.x)));

			real32 float8Elapsed = cam->trackRadarZoomSpeed * elapsedGame;

			if ((dist1 > float8Elapsed) && (dist1 < 120.0f)) {

				Vector3F radar_vecObjDir;

				real32 fVar3 = (real32)dist1 / float8Elapsed;
				Gods98::Container_GetOrientation(radar_objCont, nullptr, &radar_vecObjDir, nullptr);
				//Gods98::Maths_Vector3DScale(&radar_vecObjDir, &radar_vecObjDir, fVar3);
				// radar_vecObjDir isn't actually modified here, but its value is unused afterwards
				//Gods98::Maths_Vector3DAdd(&radar_vecObjPos, &radar_vecObjPos, &radar_vecObjDir);
				Gods98::Maths_RayEndPoint(&radar_vecObjPos, &radar_vecObjPos, &radar_vecObjDir, fVar3);
				//radar_vecObjPos.x = radar_vecObjPos.x + radar_vecObjDir.x * fVar3;
				//radar_vecObjPos.y = radar_vecObjPos.y + radar_vecObjDir.y * fVar3;
				//radar_vecObjPos.z = radar_vecObjPos.z + radar_vecObjDir.z * fVar3;


				real32 fVar4 = radar_vecObjPos.x - radar_vecCont2Pos.x; // end - start
				real32 fVar5 = radar_vecObjPos.y - radar_vecCont2Pos.y;
				real32 fVar6 = radar_vecObjPos.z - radar_vecCont2Pos.z;

				Vector3F vecDist;
				Gods98::Maths_Vector3DSubtract(&vecDist, &radar_vecObjPos, &radar_vecCont2Pos);
				real32 dist2 = Gods98::Maths_Vector3DModulus(&vecDist);

				//real32 dist2 = Gods98::Maths_Vector3DDistance(&radar_vecObjPos, &radar_vecCont2Pos);

				//dist2 = std::sqrt((double)(fVar6 * fVar6 + fVar5 * fVar5 + fVar4 * fVar4));
				real32 zoomDist = (real32)std::fabs((double)(cam->trackRadarZoom - oldZoom));

				//fVar3 = (real32)zoomDist;
				real32 zoomAddDist2 = dist2 + zoomDist;
				//dist2_again = std::sqrt((double)(fVar6 * fVar6 + fVar5 * fVar5 + fVar4 * fVar4));
				//real32 fVar8 = 1.0f / dist2; // normalize


				//real32 fVar9 = (dist2 / zoomAddDist2) * float8Elapsed;
				real32 fVar9 = (dist2 / (dist2 + zoomDist)) * float8Elapsed;

				//Gods98::Maths_Vector3DNormalize(&vecDist);
				//Gods98::Maths_Vector3DScale(&vecDist, fVar9);
				//Gods98::Maths_Vector3DAdd(&radar_vecCont2Pos, &radar_vecCont2Pos, &vecDist);
				Gods98::Maths_RayEndPoint(&radar_vecCont2Pos, &radar_vecCont2Pos, &vecDist, fVar9);

				//real32 delta; // ??
				//Gods98::Maths_Vector3DInterpolate(&radar_vecCont2Pos, &radar_vecObjPos, &radar_vecCont2Pos, delta);
				//Gods98::Maths_Vector3DSubtract(&radar_vecCont2Pos, &radar_vecObjPos, &radar_vecCont2Pos);
				//Gods98::Maths_Vector3DScale(&radar_vecCont2Pos, &radar_vecCont2Pos, delta);
				//Gods98::Maths_Vector3DAdd(&radar_vecCont2Pos, &radar_vecObjPos, &radar_vecCont2Pos);


				//radar_vecCont2Pos.x = radar_vecCont2Pos.x + fVar4 * fVar8 * fVar9;
				//radar_vecCont2Pos.y = radar_vecCont2Pos.y + fVar5 * fVar8 * fVar9;
				//radar_vecCont2Pos.z = radar_vecCont2Pos.z + fVar6 * fVar8 * fVar9;

				Gods98::Container_SetPosition(cam->cont2, nullptr, radar_vecCont2Pos.x, radar_vecCont2Pos.y, radar_vecCont2Pos.z);

				if (oldZoom > cam->trackRadarZoom) {
					//cam->zoom = oldZoom - (zoomDist / zoomAddDist2) * float8Elapsed;
					cam->zoom = oldZoom - (zoomDist / (zoomDist + dist2)) * float8Elapsed;
				}
				else if (oldZoom != cam->trackRadarZoom) {
					//cam->zoom = oldZoom + (zoomDist / zoomAddDist2) * float8Elapsed;
					cam->zoom = oldZoom + (zoomDist / (zoomDist + dist2)) * float8Elapsed;
				}
				Gods98::Container_SetPosition(cam->contCam, cam->cont4, 0.0f, 0.0f, -cam->zoom);
			}

			Camera_AddRotation(cam, cam->trackRadarRotationSpeed * elapsedAbs);
		}
		//goto LAB_004368f0;
	}
	else if (cam->type == LegoCamera_Type::LegoCamera_FP && (cam->trackObj != nullptr && elapsedAbs != 0.0f)) {
		Vector3F fp_vecObjPos;
		Vector3F fp_vecObjDir;
		Vector3F fp_vecCont1Pos;
		Vector3F fp_vecCont1Dir;
		/// FIX APPLY: Assign local variables, that would potentially be used without initialization
		real32 fp_distOrient = 0.0f;
		real32 fp_distPos = 0.0f;
		
		Gods98::Container_GetPosition(cam->contCam, nullptr, &fp_vecCont1Pos);
		/// REFACTOR: fp_vecObjDir used as dummy, but it's not necessary since both arguments are optional
		Gods98::Container_GetOrientation(cam->contCam, nullptr, &fp_vecCont1Dir, nullptr);// &fp_vecObjDir);

		LegoObject_FP_GetPositionAndHeading(cam->trackObj, cam->trackFPCameraFrame, &fp_vecObjPos, &fp_vecObjDir);

		if (!(cam->flags & CameraFlags::CAMERA_FLAG_FPSETUP)) {

			fp_vecCont1Pos = fp_vecObjPos;
			fp_vecCont1Dir = fp_vecObjDir;
		}
		else {
			fp_distPos = Gods98::Maths_Vector3DDistance(&fp_vecObjPos, &fp_vecCont1Pos);
			//fVar10 = std::sqrt((double)((fp_vecObjPos.z - fp_vecCont1Pos.z) *
			//							(fp_vecObjPos.z - fp_vecCont1Pos.z) +
			//							(fp_vecObjPos.y - fp_vecCont1Pos.y) *
			//							(fp_vecObjPos.y - fp_vecCont1Pos.y) +
			//							(fp_vecObjPos.x - fp_vecCont1Pos.x) *
			//							(fp_vecObjPos.x - fp_vecCont1Pos.x)));
			//fp_distPos = (real32)fVar10;

			if ((fp_distPos > 100.0f) ||
				((fp_distOrient = Gods98::Maths_Vector3DDotProduct(&fp_vecObjDir, &fp_vecCont1Dir)) < 0.0f))
			{
				fp_vecCont1Pos = fp_vecObjPos;
				fp_vecCont1Dir = fp_vecObjDir;
			}
		}

		real32 fVar1 = (1.0f / elapsedAbs) * 4.0f;
		Gods98::Maths_Vector3DSetLength(&fp_vecCont1Dir, &fp_vecCont1Dir, fVar1);
		//Gods98::Maths_Vector3DNormalize(&fp_vecCont1Dir);
		//Gods98::Maths_Vector3DScale(&fp_vecCont1Dir, &fp_vecCont1Dir, fVar1);

		//fVar10 = std::sqrt((double)(fp_vecCont1Dir.z * fp_vecCont1Dir.z +
		//							fp_vecCont1Dir.y * fp_vecCont1Dir.y +
		//							fp_vecCont1Dir.x * fp_vecCont1Dir.x));
		//fVar2 = 1.0f / (real32)fVar10;
		//fp_vecCont1Dir.x *= fVar2;
		//fp_vecCont1Dir.y *= fVar2;
		//fp_vecCont1Dir.z *= fVar2;

		//fp_vecCont1Dir.x *= fVar1;
		//fp_vecCont1Dir.y *= fVar1;
		//fp_vecCont1Dir.z *= fVar1;

		Gods98::Maths_Vector3DNormalize(&fp_vecObjDir);
		Gods98::Maths_Vector3DAdd(&fp_vecObjDir, &fp_vecObjDir, &fp_vecCont1Dir);


		//fVar10 = std::sqrt((double)(fp_vecObjDir.z * fp_vecObjDir.z +
		//							fp_vecObjDir.y * fp_vecObjDir.y + fp_vecObjDir.x * fp_vecObjDir.x));
		//fVar2 = 1.0f / (real32)fVar10;
		//fp_vecObjDir.x = fp_vecObjDir.x * fVar2 + fp_vecCont1Dir.x;
		//fp_vecObjDir.y = fp_vecObjDir.y * fVar2 + fp_vecCont1Dir.y;
		//fp_vecObjDir.z = fp_vecObjDir.z * fVar2 + fp_vecCont1Dir.z;


		Gods98::Maths_Vector3DScale(&fp_vecCont1Pos, &fp_vecCont1Pos, fVar1);
		//fp_vecCont1Pos.x *= fVar1;
		//fp_vecCont1Pos.y *= fVar1;
		//fp_vecCont1Pos.z *= fVar1;
		
		fVar1 = 1.0f / (fVar1 + 1.0f);
		Gods98::Maths_Vector3DAdd(&fp_vecObjPos, &fp_vecObjPos, &fp_vecCont1Pos);
		Gods98::Maths_Vector3DScale(&fp_vecObjPos, &fp_vecObjPos, fVar1);
		//fp_vecObjPos.x = (fp_vecObjPos.x + fp_vecCont1Pos.x) * fVar1;
		//fp_vecObjPos.y = (fp_vecObjPos.y + fp_vecCont1Pos.y) * fVar1;
		//fp_vecObjPos.z = (fp_vecObjPos.z + fp_vecCont1Pos.z) * fVar1;

		if (fp_distPos > 0.01f) {
			Gods98::Container_SetPosition(cam->contCam, nullptr, fp_vecObjPos.x, fp_vecObjPos.y, fp_vecObjPos.z);
		}
		if (fp_distOrient < 0.99999f) {
			Gods98::Container_SetOrientation(cam->contCam, nullptr, fp_vecObjDir.x, fp_vecObjDir.y, fp_vecObjDir.z, 0.0f, 0.0f, -1.0f);
		}

		cam->flags |= CameraFlags::CAMERA_FLAG_FPSETUP;

	}
	//if (cam->trackObj == nullptr || elapsedAbs == 0.0f)
	//	goto LAB_004368f0;

	

//LAB_004368f0:
	if (cam->shakeDuration <= cam->shakeTimer) {
		cam->shakeVector.z = 0.0f;
		cam->shakeVector.y = 0.0f;
		cam->shakeVector.x = 0.0f;
	}
	else {
		//fVar1 = cam->shakeDuration;
		//fVar2 = cam->shakeTimer;
		//fVar3 = cam->shakeDuration;
		//fVar4 = cam->shakeIntensity;
		real32 randRange = Gods98::Maths_RandRange(0.0f, 1.0f);
		real32 shakeCurrent = (cam->shakeDuration - cam->shakeTimer); // lower (earlier) timer -> higher intensity
		real32 shakeDelta = Gods98::Maths_InterpolationDelta(0.0f, cam->shakeDuration, shakeCurrent);
		real32 shakeScalar = shakeDelta * cam->shakeIntensity * randRange;
		//real32 shakeOff = ((cam->shakeDuration - cam->shakeTimer) / cam->shakeDuration) * cam->shakeIntensity * randRange;
		
		Gods98::Maths_Vector3DRandom(&cam->shakeVector);
		Gods98::Maths_Vector3DScale(&cam->shakeVector, &cam->shakeVector, shakeScalar);
		//cam->shakeVector.x = cam->shakeVector.x * shakeOff;
		//cam->shakeVector.y = cam->shakeVector.y * shakeOff;
		//cam->shakeVector.z = cam->shakeVector.z * shakeOff;
		
		cam->shakeTimer += elapsedGame;

		// I'm confused why the same shake operation is happening here too,
		// subtracting the shake vector, when that was previously done to negate the shake?...
		Gods98::Container_AddTranslation(cam->contCam, Gods98::Container_Combine::Before, -cam->shakeVector.x, -cam->shakeVector.y, -cam->shakeVector.z);
	}
	return;
}

// <LegoRR.exe @00436a53>
void __cdecl LegoRR::Camera_SetTiltRange(LegoCamera* cam, real32 minTilt, real32 maxTilt)
{
	cam->flags |= CameraFlags::CAMERA_FLAG_TILTRANGE;
	cam->tiltRange.min = minTilt;
	cam->tiltRange.max = maxTilt;
}

// <LegoRR.exe @00436a82>
void __cdecl LegoRR::Camera_SetTilt(LegoCamera* cam, real32 tilt)
{
	if (!(cam->flags & CameraFlags::CAMERA_FLAG_TILTRANGE) ||
		(cam->flags & CameraFlags::CAMERA_FLAG_FREEMOVEMENT) ||
		(tilt >= cam->tiltRange.min && tilt <= cam->tiltRange.max))
	{
		cam->tilt = tilt;
		Gods98::Container_SetOrientation(cam->cont4, cam->cont2, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		Gods98::Container_AddRotation(cam->cont4, Gods98::Container_Combine::Before, 1.0f, 0.0f, 0.0f, -tilt);
	}
}

// <LegoRR.exe @00436b22>
void __cdecl LegoRR::Camera_AddTilt(LegoCamera* cam, real32 tiltAmount)
{
	Camera_SetTilt(cam, cam->tilt + tiltAmount);
}

// <LegoRR.exe @00436b43>
void __cdecl LegoRR::Camera_SetRotationRange(LegoCamera* cam, real32 minRotation, real32 maxRotation)
{
	cam->flags |= CameraFlags::CAMERA_FLAG_ROTATIONRANGE;
	cam->rotationRange.min = minRotation;
	cam->rotationRange.max = maxRotation;
}

// <LegoRR.exe @00436b75>
void __cdecl LegoRR::Camera_SetRotation(LegoCamera* cam, real32 rotation)
{
	if (!(cam->flags & CameraFlags::CAMERA_FLAG_ROTATIONRANGE) ||
		(cam->flags & CameraFlags::CAMERA_FLAG_FREEMOVEMENT) ||
		(rotation >= cam->rotationRange.min && rotation <= cam->rotationRange.max))
	{
		cam->rotation = rotation;
		Gods98::Container_SetOrientation(cam->cont2, nullptr, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		Gods98::Container_AddRotation(cam->cont2, Gods98::Container_Combine::Before, 0.0f, 0.0f, 1.0f, -rotation);
	}
}

// <LegoRR.exe @00436c16>
void __cdecl LegoRR::Camera_AddRotation(LegoCamera* cam, real32 rotationAmount)
{
	Camera_SetRotation(cam, cam->rotation + rotationAmount);
}

// <LegoRR.exe @00436c3a>
void __cdecl LegoRR::Camera_SetZoomRange(LegoCamera* cam, real32 minZoom, real32 maxZoom)
{
	cam->flags |= CameraFlags::CAMERA_FLAG_ZOOMRANGE;
	cam->zoomRange.min = minZoom;
	cam->zoomRange.max = maxZoom;
}

// <LegoRR.exe @00436c6c>
void __cdecl LegoRR::Camera_SetZoom(LegoCamera* cam, real32 zoom)
{
	if (!(cam->flags & CameraFlags::CAMERA_FLAG_ZOOMRANGE) ||
		(cam->flags & CameraFlags::CAMERA_FLAG_FREEMOVEMENT) ||
		(zoom >= cam->zoomRange.min && zoom <= cam->zoomRange.max))
	{
		cam->zoom = zoom;
		// no Container transform like the other angles
	}
}

// <LegoRR.exe @00436cc7>
void __cdecl LegoRR::Camera_AddZoom(LegoCamera* cam, real32 zoomAmount)
{
	Camera_SetZoom(cam, cam->zoom + zoomAmount);
}

// <LegoRR.exe @00436ceb>
void __cdecl LegoRR::Camera_AddTranslation2D(LegoCamera* cam, real32 translateX, real32 translateY)
{
	Gods98::Container_AddTranslation(cam->cont2, Gods98::Container_Combine::Before, translateX, translateY, 0.0f);
}

// <LegoRR.exe @00436d0b>
void __cdecl LegoRR::Camera_GetTopdownPosition(LegoCamera* cam, Vector3F* out_position)
{
	if (cam->type == LegoCamera_Type::LegoCamera_Top) {
		Gods98::Container_GetPosition(cam->cont2, nullptr, out_position);
	}
}

// <LegoRR.exe @00436d2d>
void __cdecl LegoRR::Camera_SetTopdownPosition(LegoCamera* cam, real32 xPos, real32 yPos)
{
	if (cam->type == LegoCamera_Type::LegoCamera_Top) {
		Gods98::Container_SetPosition(cam->cont2, nullptr, xPos, yPos, 0.0f);
	}
}

// <LegoRR.exe @00436d55>
bool32 __cdecl LegoRR::Camera_GetTopdownWorldPos(LegoCamera* cam, Map3D* map, Vector3F* out_worldPos)
{
	if (cam->type == LegoCamera_Type::LegoCamera_Top) {
		Gods98::Container_GetPosition(cam->cont2, nullptr, out_worldPos);

		out_worldPos->z = Map3D_GetWorldZ(map, out_worldPos->x, out_worldPos->y);
	}
	return true;
}

// <LegoRR.exe @00436d9b>
real32 __cdecl LegoRR::Camera_GetRotation(LegoCamera* cam)
{
	return cam->rotation;
}

// <LegoRR.exe @00436da9>
void __cdecl LegoRR::Camera_Move(LegoCamera* cam, IN OUT Vector3F* dir, real32 elapsedAbs)
{
	Gods98::Maths_Vector3DNormalize(dir);
	//dir_dist = std::sqrt((double)(dir->z * dir->z +
	//							  dir->y * dir->y + dir->x * dir->x));
	//dir_norm = 1.0 / (real32)dir_dist;
	//dir->x = dir->x * dir_norm;
	//dir->y = dir->y * dir_norm;
	//dir->z = dir->z * dir_norm;

	// Add (acceleration*timeDelta) to current speed.
	//  (note that moveSpeed itself does not contain time info, which is the best way to handle it)
	cam->moveSpeed += cameraGlobs.acceleration * elapsedAbs;

	// Cap acceleration at max (maxSpeed)
	if (cam->moveSpeed > cameraGlobs.maxSpeed)
		cam->moveSpeed = cameraGlobs.maxSpeed;

	real32 moveDist = cam->moveSpeed * elapsedAbs;

	// Set length of direction vector to moveDist
	Gods98::Maths_Vector3DScale(dir, dir, moveDist);

	// Add directional movement to cam->moveVector
	Gods98::Maths_Vector3DAdd(&cam->moveVector, &cam->moveVector, dir);
}

#pragma endregion
