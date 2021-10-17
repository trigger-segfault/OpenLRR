// Lwt.cpp : 
//

#include "../../platform/windows.h"
#include "../../platform/d3drm.h"

#include "../geometry.h"

#include "../core/Errors.h"
#include "../core/Files.h"
#include "../core/Memory.h"

#include "Lwt.h"


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0048c300>
const char* __cdecl Gods98::lwExtractString(const char* str1, const char* str2, OPTIONAL OUT uint32* pos, OPTIONAL OUT uint32* len)
{
	uint32 len1= std::strlen(str1);
	uint32 len2=std::strlen(str2);

	if(!len1 || !len2)
		return nullptr;

	for(uint32 i=0;i<len1;i++)
	{
		const char* str=str1+i;
		if(!::_strnicmp(str,str2,len2))
		{
			if(pos)
				*pos=i;
			if(len)
				*len=len2;
			return str;
		}
	}

	return nullptr;
}

// "YASD" Yet another `char* ::_strdup(const char*)`
// <LegoRR.exe @0048c380>
char* __cdecl Gods98::stringAlloc(const char* str)
{
	uint32 len=std::strlen(str)+1;
	len += (len & 1); // round up to units of 2
	char* sa = (char*)Mem_Alloc(len);
	std::memset(sa, 0, len);
	std::strcpy(sa, str);
	return sa;
}

// <LegoRR.exe @0048c3e0>
Gods98::LightWave_TexType __cdecl Gods98::texMapType(const char* str)
{
	const char* tTypes[]={"PLANAR","CYLINDRICAL","SPHERICAL"};

	for(uint32 ret=0; ret<(uint32)LightWave_TexType::MT_MAX; ret++)
	{
		uint32 len = std::strlen(tTypes[ret]);
		if (!::_strnicmp(str, tTypes[ret], len))
			return (LightWave_TexType)ret;
	}

	return LightWave_TexType::MT_PLANAR;// (LightWave_TexType)0; default type?
}

// if (flag) Mem_Free(srf);
// <LegoRR.exe @0048c440>
void __cdecl Gods98::surfFree(LWSURFACE* srf, bool32 flag)
{
	char* ptr;
	LWSURFACE* srfN;

	ptr = srf->srfName;
	if (ptr)
		Mem_Free(ptr);
	ptr = srf->srfPath;
	if (ptr)
		Mem_Free(ptr);
	srfN = srf->srfNextSurf;
	if (srfN)
		surfFree(srfN, true);
	if (flag)
		Mem_Free(srf);
}

// <LegoRR.exe @0048c490>
bool32 __cdecl Gods98::LWD3D(uint8** p, real32* f)
{
	uint8* fv = (uint8*)f;

	fv[3]=*(*p)++;
	fv[2]=*(*p)++;
	fv[1]=*(*p)++;
	fv[0]=*(*p)++;

	return true;
}

// <LegoRR.exe @0048c4d0>
bool32 __cdecl Gods98::PNTSprc(File* file, LWSIZE* sizeData, sint32 csize, real32** verts, bool32 dflag)
{
	char cbuff[10]={0}; // unused, keeping here in-case it's a bad safeguard for buffer overrun
	char buff[256];
	//D3DVECTOR pos;
	Vector3F pos;

	uint8* sp=(uint8*)Mem_Alloc(csize+16);
	uint8* cp=sp;

	File_Read(sp, csize, 1, file);
	sizeData->lwVertCount=csize/12;

	if(!verts)
	{
		Mem_Free(sp);
		return true;

	}
	*verts=(real32 *)Mem_Alloc(csize+4);
	real32* vptr=*verts;

	sint32 i = 0;
	while(csize>0)
	{
		LWD3D(&cp,&pos.x);LWD3D(&cp,&pos.y);LWD3D(&cp,&pos.z);
		*vptr++=pos.x;
		*vptr++=pos.y;
		*vptr++=pos.z;
		if(dflag)
		{
			std::sprintf(buff, "Point %d > %f %f %f\n",i, pos.x,pos.y,pos.z);
			Error_Debug(buff);
		}
		i++;
		csize-=12; // sizeof(Vector3F)
	}
	Mem_Free(sp);
	return true;
}

// <LegoRR.exe @0048c620>
bool32 __cdecl Gods98::CRVSprc(File* file, sint32 csize, bool32 dflag)
{
	uint16 vcount,vcur,vsurf,vflags;

	uint8* sp=(uint8*)Mem_Alloc(csize+16);
	uint8* cp=sp;
	uint16* swp = (uint16*)sp;
	File_Read(sp, csize, 1, file);

	while(csize>0)
	{
		vcount=LWSPVALUE(swp);
		swp++;
		csize-=2;
		while(vcount)
		{
			vcur=LWSPVALUE(swp);
			swp++;
			csize-=2;
			vcount--;
		}
		vsurf = LWSPVALUE(swp);
		vflags= LWSPVALUE(swp);
		swp++;
		swp++;
		csize-=4;
	}
	Mem_Free(sp);

	return true;
}

// <LegoRR.exe @0048c6a0>
bool32 __cdecl Gods98::POLSprc(File* file, LWSIZE* sizeData, LWPOLY** polys, sint32 csize, bool32 dflag)
{
	//uint8 *sp, *cp;
	//uint16 *swp;
	uint16 vcount,vcur,vsurf;
	LWPOLY *ply;
	uint16*plyd;
	char buff[256], buff2[64];

	uint8* sp=(uint8 *)Mem_Alloc(csize+16);
	uint8* cp=sp;
	uint16* swp=(uint16*)sp;
	File_Read(sp, csize, 1, file);
	uint16 pc = 0;
	while(csize>0)
	{
		vcount=LWSPVALUE(swp);
		swp++;
		csize-=2;

		if(polys)
		{
			if(!pc)
			{
				ply=(LWPOLY *)Mem_Alloc(sizeof(LWPOLY));
				*polys = ply;
			}
			else
			{
				ply=(LWPOLY *)Mem_Alloc((pc+1)*sizeof(LWPOLY));
				std::memcpy(ply,*polys,(pc * sizeof(LWPOLY)));
				Mem_Free(*polys);
				*polys = ply;
				ply += pc;
			}
			plyd = (uint16*)Mem_Alloc(vcount * sizeof(uint16));
			if(dflag)
			{
				std::sprintf(buff, "Poly %d, Vertex Count %d > ",pc, vcount);
			}
			ply->plyCount = vcount;
			ply->plyData = plyd;
			while(vcount)
			{
				vcur=LWSPVALUE(swp);
				if(dflag)
				{
					std::sprintf(buff2, "%d ", vcur);
					std::strcat(buff, buff2);
				}
				*plyd++=vcur;
				swp++;
				csize-=2;
				vcount--;
			}
			vsurf = LWSPVALUE(swp);
			vsurf--;
			ply->plySurface = vsurf;
			if(dflag)
			{
				std::sprintf(buff2, "Surface :- %d\n",vsurf);
				std::strcat(buff, buff2);

				Error_Debug(buff);
			}
		}
		swp++;
		csize-=2;
		pc++;
	}
	sizeData->lwPolyCount = pc;
	Mem_Free(sp);

	return true;
}

// if (dflag) ... Error_Debug(...);
// <LegoRR.exe @0048c950>
bool32 __cdecl Gods98::SRFSprc(File* file, LWSIZE* sizeData, LWSURFLIST** srfl, sint32 csize, OUT uint32* srflCount, bool32 dflag)
{
	//uint8* sp;
	//char* cp;
	char	buff[512];
	uint8	cbuff[10]={0};
	sint32 nc=0;
	/// FIX APPLY: dummy init
	LWSURFLIST* srfb = nullptr;

	char* sp=(char*)Mem_Alloc(csize+16);
	char* cp=sp;
	File_Read(sp, csize, 1, file);

	if(srfl)
	{
		srfb=(LWSURFLIST *)Mem_Alloc(sizeof(LWSURFLIST));
		std::memset(srfb, 0, sizeof(LWSURFLIST));
	}

	while(csize>0)
	{
		if(dflag)
		{
			std::sprintf(buff, "Surface %d > %s\n", nc, cp);
			Error_Debug(buff);
		}
		uint32 len = std::strlen(cp) + 1;
		if(len&1) // round up to unit of two
			len++;
		if(srfl)
		{
			char* snp   = (char*)Mem_Alloc(len);
			char** snpp = (char**)Mem_Alloc((nc+1)*sizeof(char *));
			std::memcpy(snp,cp,len);
			if(nc)
			{
				std::memcpy(snpp,srfb->srflName,nc*sizeof(char*));
				Mem_Free(srfb->srflName);
			}
			*(snpp+nc)=snp;
			srfb->srflName = snpp;
			*srfl=srfb;
			srfb->srflCount = nc+1;
		}
		nc++;
		*srflCount = nc;
		cp+=len;
		csize-=len;
	}
	Mem_Free(sp);
	return true;
}

// if (dflag) ... Error_Debug(...);
// <LegoRR.exe @0048cae0>
bool32 __cdecl Gods98::SURFprc(File* file, LWSIZE* sizeData, LWSURFACE** surf, sint32 csize, bool32 dflag)
{
	char *sp, *cp;
	char	buff[512];
	char	buff2[512];
	char	cbuff[10]={0};
	uint32 len;
	/// FIX APPLY: dummy init
	LWSURFACE *srfb = nullptr;
	sint32 sc;
	uint32 setVal;
	sp=(char *)Mem_Alloc(csize+16);
	cp=sp;
	File_Read(sp, csize, 1, file);

	sc=sizeData->lwSurfaceCount;

	if(surf)
	{
		if(!sc)
		{
			srfb = (LWSURFACE *)Mem_Alloc(sizeof(LWSURFACE));
			*surf = srfb;
			std::memset(srfb, 0, sizeof(LWSURFACE));
			srfb->srfName = stringAlloc(cp);
		}
		else
		{
			srfb = (LWSURFACE *)Mem_Alloc(sizeof(LWSURFACE) * (sc+1));
			std::memcpy(srfb,*surf,sc * sizeof(LWSURFACE));
			Mem_Free(*surf);
			*surf = srfb;
			srfb += sc;
			std::memset(srfb, 0, sizeof(LWSURFACE));
			srfb->srfName = stringAlloc(cp);
		}
	}

	if(dflag)
	{
		std::sprintf(buff, "Surface %d :- %s",sc,cp);
		Error_Debug(buff);
	}
	len = std::strlen(cp) + 1;
	if(len&1)
		len++;
	cp+=len;
	csize-=len;
	while(csize>0)
	{
		std::strncpy(cbuff,cp,4);
		cp+=4;
		len = *(uint16 *)cp;
		len = LWSVALUE(len);
		std::sprintf(buff, " %s (%d)",cbuff,len);
		cp+=2;
		if(!::_strnicmp(cbuff,"TIMG",4))
		{
			std::strcat(buff,cp);
			if(surf)
			{
				uint32 pos,len;
				char cps[128];

				std::strcpy(cps,cp);
				if(lwExtractString(cps,"(sequence)",&pos,&len))
				{
					*(cps+pos-1)=0;		//Get rid of sequence
					srfb->srfTexFlags |= LightWave_TexFlags::TFM_SEQUENCE;
				}
				srfb->srfPath = stringAlloc(cps);
			}
		}
		else if(!::_strnicmp(cbuff,"COLR",4))
		{
			std::sprintf(buff2, " RGBA %d,%d,%d,%d",cp[0],cp[1],cp[2],cp[3]);
			std::strcat(buff,buff2);
			if(surf)
			{
				srfb->srfCol.colRed=cp[0];
				srfb->srfCol.colGreen=cp[1];
				srfb->srfCol.colBlue=cp[2];
				srfb->srfCol.colAlpha=cp[3];
			}
		}
		else if(!::_strnicmp(cbuff,"CTEX",4))
		{
			std::strcat(buff,cp);
			if(surf)
			{
				srfb->srfTexType = texMapType(cp);
			}
		}
		else if(!::_strnicmp(cbuff,"TTEX",4))
		{
			std::strcat(buff,cp);
			if(surf)
			{
				LWSURFACE *srfbN;

				srfbN = (LWSURFACE *)Mem_Alloc(sizeof(LWSURFACE));
				srfb->srfNextSurf = srfbN;
				srfb = srfbN;
				std::memset(srfb, 0, sizeof(LWSURFACE));

				srfb->srfTexType = texMapType(cp);
			}
		}
		else if(!::_strnicmp(cbuff,"TFLG",4))
		{
			uint16 sval,i,f=0;
			if(surf)
			{
				const char* ttexName[]={
					"Axis_x",
					"Axis_y",
					"Axis_z",
					"World Coord",
					"Negative Image",
					"Pixel Blending",
					"Anti Aliasing",
				};

				sval = LWSPVALUE((uint16 *)cp);
				srfb->srfTexFlags |= (LightWave_TexFlags)sval;
				std::sprintf(buff2,"%xH - ",sval);
				for(i=0;i<LightWave_TexBits::TF_MAX;i++)
				{
					if(sval & (1<<i))
					{
						if(f)
							std::strcat(buff2, ",");
						f++;
						std::strcat(buff2, ttexName[i]);
					}
				}

				std::strcat(buff,buff2);
			}
		}
		else if(!::_strnicmp(cbuff,"FLAG",4))
		{
			uint16 sval,i,f=0;
			if(surf)
			{
				const char* tName[]={
					"Luminous",
					"Outline",
					"Smoothing",
					"Color Highlights",
					"Color Filter",
					"Opaque Edge",			//\ these two should not be set together
					"Transparent Edge",		///
					"Sharp Terminator",
					"Double Sided",
					"Additive"
				};

				sval = LWSPVALUE((uint16 *)cp);
				srfb->srfFlags=(LightWave_SurfFlags)sval;
				std::sprintf(buff2,"%xH - ",sval);
				for(i=0;i< LightWave_SurfBits::SF_MAX;i++)
				{
					if(sval & (1<<i))
					{
						if(f)
							std::strcat(buff2, ",");
						f++;
						std::strcat(buff2, tName[i]);
					}
				}

				std::strcat(buff,buff2);
			}
		}
		else if(!::_strnicmp(cbuff,"TSIZ",4))
		{
			TEXDATA td;
			if(surf)
			{
				uint8* ccp=(uint8*)cp;
				LWD3D(&ccp,&td.tdX);LWD3D(&ccp,&td.tdY);LWD3D(&ccp,&td.tdZ);
				std::sprintf(buff2," (%f,%f,%f)",td.tdX,td.tdY,td.tdZ);
				srfb->srfTexSize.tdX=td.tdX;
				srfb->srfTexSize.tdY=td.tdY;
				srfb->srfTexSize.tdZ=td.tdZ;
				std::strcat(buff,buff2);
			}
		}
		else if(!::_strnicmp(cbuff,"TCTR",4))
		{
			TEXDATA td;
			if(surf)
			{
				uint8* ccp=(uint8*)cp;
				LWD3D(&ccp,&td.tdX);LWD3D(&ccp,&td.tdY);LWD3D(&ccp,&td.tdZ);
				std::sprintf(buff2," (%f,%f,%f)",td.tdX,td.tdY,td.tdZ);
				srfb->srfTexCentre.tdX=td.tdX;
				srfb->srfTexCentre.tdY=td.tdY;
				srfb->srfTexCentre.tdZ=td.tdZ;
				std::strcat(buff,buff2);
			}
		}
		setVal=0;
		if(!::_strnicmp(cbuff,"TRAN",4))
			setVal=1;
		else if(!::_strnicmp(cbuff,"DIFF",4))
			setVal=2;
		else if(!::_strnicmp(cbuff,"LUMI",4))
			setVal=3;
		else if(!::_strnicmp(cbuff,"REFL",4))
			setVal=4;
		else if(!::_strnicmp(cbuff,"SPEC",4))
			setVal=5;
		else if(!::_strnicmp(cbuff,"GLOS",4))
			setVal=6;

		if(setVal)
		{
			uint16 sval;
			real32 fval;
			if(surf)
			{
				sval=LWSPVALUE((uint16 *)cp);
				fval=(real32)sval/256;
				switch(setVal)
				{
				case 1:
					srfb->srfTransparent=fval;
					break;
				case 2:
					srfb->srfDiffuse=fval;
					break;
				case 3:
					srfb->srfLuminous=fval;
					break;
				case 4:
					srfb->srfReflect=fval;
					break;
				case 5:
					srfb->srfSpecular=fval;
					break;
				case 6:
					srfb->srfSpecPower=fval;
					break;
				}
				std::sprintf(buff2,"%f",fval);
				std::strcat(buff,buff2);
			}
		}
		if(dflag)
		{
			Error_Debug(buff);
		}
		cp+=len;

		csize-=(len+6);
	}
	if(dflag)
	{
		Error_Debug("\n");
	}
	sizeData->lwSurfaceCount++;
	Mem_Free(sp);
	return true;
}

// if (dflag) ... Error_Debug(...);
// <LegoRR.exe @0048d580>
bool32 __cdecl Gods98::LoadLWOB(const char* fn, LWSIZE* sd, real32** verts, LWPOLY** polys, LWSURFACE** surfs, File** fileUV, bool32 dflag)
{
	char buff[1024];
	char cbuff[10]={0};
	char fname[LBUF_LEN];
	uint32 size, csize;
	sint32 length;
	uint32 srflCount=0;
	/// FIX APPLY: dummy init
	LWSURFLIST* srfl = nullptr;
	uint32 slCount=0;


	std::sprintf(fname,"%s.lwo",fn);
	File* file=File_Open(fname, "rb");
	if(!file)
	{	Error_Warn(true, Error_Format("Cannot load Lightwave object file %s", fname) );
		return false;
	}
	else
		Error_Debug( Error_Format("Loading Lightwave Object file %s\n", fname) );

	LWSIZE* sizeData=(LWSIZE*)Mem_Alloc(sizeof(LWSIZE));
	sizeData->lwVertCount=sizeData->lwPolyCount=sizeData->lwSurfaceCount=0;

	if( fileUV )
	{
		std::sprintf(fname,"%s.uv",fn);

		if( File_Exists(fname) )
		{
			*fileUV = File_Open(fname, "rb");
			if( *fileUV )
				Error_Debug( Error_Format("UV file \"%s\" found\n", fname) );

		}
		else
			*fileUV = nullptr;
	}

	File_Read(cbuff, 4, 1, file);
	if(::_stricmp(cbuff, "FORM"))
		goto err;
	File_Read(&size, 4, 1, file);
	size = LWVALUE(size);
	File_Read(cbuff, 4, 1, file);
	if(::_stricmp(cbuff, "LWOB"))
		goto err;
	length = size-4;
	while(length > 0)
	{
		File_Read(cbuff, 4, 1, file);
		File_Read(&csize, 4, 1, file);
		csize = LWVALUE(csize);

		if(dflag)
		{
			std::sprintf(buff, "LW OBJECT %s %d\n",cbuff, csize);
			Error_Debug(buff);
		}

		if(!::_stricmp(cbuff, "PNTS"))
			PNTSprc(file, sizeData, csize, verts, dflag);
		else if(!::_stricmp(cbuff, "SRFS"))
			SRFSprc(file, sizeData, &srfl, csize, &srflCount, dflag);
		else if(!::_stricmp(cbuff, "SURF"))
			SURFprc(file, sizeData, surfs, csize, dflag);
		else if(!::_stricmp(cbuff, "CRVS"))
			CRVSprc(file, csize, dflag);
		else if(!::_stricmp(cbuff, "POLS"))
			POLSprc(file, sizeData, polys, csize, dflag);
		else
			File_Seek(file, csize, SeekOrigin::Current);

		length-=(8+csize);
	}

	slCount = sizeData->lwSurfaceCount;

	//***********************************************************************
	//**
	//** Place surfaces in correct order for polys using surface list
	//**
	//***********************************************************************
	if(surfs && srfl && sizeData)
	{
		uint32 n,n2;
		char *sn;
		LWSURFACE *surfb,*sb;

		surfb  = (LWSURFACE *)Mem_Alloc(srflCount * sizeof(LWSURFACE));
		sb = surfb;
		std::memset(surfb, 0, sizeof(LWSURFACE) * srflCount);
		for(n=0; n < srflCount; n++)
		{
			sn=srfl->srflName[n];
			for(n2=0;n2<slCount;n2++)
			{
				if(!::_stricmp((*surfs+n2)->srfName,sn))
				{
					*sb=*(*surfs+n2);
					break;
				}
			}
			if(n2==slCount)
			{
				sb->srfName = stringAlloc("Default");
				sb->srfCol.colRed=255;
				sb->srfCol.colGreen=255;
				sb->srfCol.colBlue=255;
				slCount = srflCount;
			}
			sb++;
		}
		Mem_Free(*surfs);
		*surfs = surfb;
	}
	sizeData->lwSurfaceCount = slCount;
	if(sd)
	{
		*sd=*sizeData;
	}
	Mem_Free(sizeData);

//****************************************
	// Free surface list
	if(srflCount)
	{
		for(uint32 i=0;i<srflCount;i++)
		{
			Mem_Free(srfl->srflName[i]);
		}
		Mem_Free(srfl->srflName);
		Mem_Free(srfl);
	}

//****************************************

	File_Close(file);

	return true;
err:
	Mem_Free(sizeData);
	File_Close(file);
	std::sprintf(buff, "Error in Lightwave file %s",fn);
	Error_Warn(true, buff);
	return false;
}

// if (dflag) ... Error_Debug(...);
// <LegoRR.exe @0048da80>
bool32 __cdecl Gods98::LoadAppObj(const char* fn, APPOBJ** ao, bool32 flag)
{
	/// FIXME: Buffer is really small for a filepath
	char path[128]; // isn't this a bit small?

	(*ao)=(APPOBJ*)Mem_Alloc(sizeof(APPOBJ));
	std::memset((*ao),0,sizeof(APPOBJ));
	::_splitpath(fn, nullptr,path,nullptr, nullptr);
	(*ao)->aoPath = stringAlloc(path);
	if( LoadLWOB(fn, &(*ao)->aoSize, &(*ao)->aoVerts, &(*ao)->aoPoly, &(*ao)->aoSurface, &(*ao)->aoFileUV, flag) )
		return true;
	else
	{
		Mem_Free( (*ao)->aoPath );
		Mem_Free( *ao );

		return false;
	}
}

// flag -> if (dFlag) ... Error_Debug(...);
// <LegoRR.exe @0048db30>
bool32 __cdecl Gods98::FreeLWOB(APPOBJ* ao)
{
	if(!ao)
		return false;

	if (ao->aoFileUV) File_Close(ao->aoFileUV);

	LWSURFACE* srf = ao->aoSurface;
	if(srf)
	{
		uint32 n=ao->aoSize.lwSurfaceCount;
		for(uint32 i=0;i<n;i++)
		{
			surfFree(srf, false);
			srf++;
		}
		Mem_Free(ao->aoSurface);
	}
	if(ao->aoPoly)
	{
		uint32 n=ao->aoSize.lwPolyCount;
		for(uint32 i=0;i<n;i++)
		{
			Mem_Free((ao->aoPoly+i)->plyData);
		}
		Mem_Free(ao->aoPoly);
	}
	Mem_Free(ao->aoVerts);
	Mem_Free(ao->aoPath);
	Mem_Free(ao);
	return true;
}

#pragma endregion
