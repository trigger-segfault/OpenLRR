// PETypes.h : PEFile raw structure and enum types.
//

#pragma once

#include "../common.h"


/////////////////////////////////////////////////////////////////////
// Typedefs

// Typedefs used to help determine current unit of offset.

// Generic unsigned 32-bit size integer.
typedef uint32_t size32_t;

// PE unsigned offset typedefs.
typedef uint32_t fileoff32_t;		// Raw file offset.
typedef uint32_t rva32_t;			// Image base offset (VirtualAddress).
typedef uint32_t address32_t;		// Full virtual address offset (VirtualAddress + ImageBase).
typedef uint32_t sectionoff32_t;	// Offset relative to section.

// PE signed variants of *32_t typedefs used for offset calculations and conversions.
typedef int32_t fileoff_t;		// Raw file offset.
typedef int32_t rva_t;			// Image base offset (VirtualAddress).
typedef int32_t address_t;		// Full virtual address offset (VirtualAddress + ImageBase).
typedef int32_t sectionoff_t;	// Offset relative to section.


namespace PE
{; // !<---

/////////////////////////////////////////////////////////////////////
// Enumerations

#ifdef IMAGE_SCN_TYPE_NO_PAD
// Assume all other flags are defined.
typedef uint32_t SectionFlags;

#else
// Define the flags ourselves.

#if 1
typedef uint32_t SectionFlags;

#define IMAGE_SCN_TYPE_NO_PAD             0x8
#define IMAGE_SCN_RESERVED_0001           0x10
#define IMAGE_SCN_CNT_CODE                0x20
#define IMAGE_SCN_CNT_INITIALIZED_DATA    0x40
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA  0x80
#define IMAGE_SCN_LNK_OTHER               0x100
#define IMAGE_SCN_LNK_INFO                0x200
#define IMAGE_SCN_RESERVED_0040           0x400
#define IMAGE_SCN_LNK_REMOVE              0x800
#define IMAGE_SCN_LNK_COMDAT              0x1000
#define IMAGE_SCN_GPREL                   0x8000
#define IMAGE_SCN_MEM_16BIT               0x20000
#define IMAGE_SCN_MEM_PURGEABLE           0x20000
#define IMAGE_SCN_MEM_LOCKED              0x40000
#define IMAGE_SCN_MEM_PRELOAD             0x80000
#define IMAGE_SCN_ALIGN_1BYTES            0x100000
#define IMAGE_SCN_ALIGN_2BYTES            0x200000
#define IMAGE_SCN_ALIGN_4BYTES            0x300000
#define IMAGE_SCN_ALIGN_8BYTES            0x400000
#define IMAGE_SCN_ALIGN_16BYTES           0x500000
#define IMAGE_SCN_ALIGN_32BYTES           0x600000
#define IMAGE_SCN_ALIGN_64BYTES           0x700000
#define IMAGE_SCN_ALIGN_128BYTES          0x800000
#define IMAGE_SCN_ALIGN_256BYTES          0x900000
#define IMAGE_SCN_ALIGN_512BYTES          0xa00000
#define IMAGE_SCN_ALIGN_1024BYTES         0xb00000
#define IMAGE_SCN_ALIGN_2048BYTES         0xc00000
#define IMAGE_SCN_ALIGN_4096BYTES         0xd00000
#define IMAGE_SCN_ALIGN_8192BYTES         0xe00000
#define IMAGE_SCN_LNK_NRELOC_OVFL         0x1000000
#define IMAGE_SCN_MEM_DISCARDABLE         0x2000000
#define IMAGE_SCN_MEM_NOT_CACHED          0x4000000
#define IMAGE_SCN_MEM_NOT_PAGED           0x8000000
#define IMAGE_SCN_MEM_SHARED              0x10000000
#define IMAGE_SCN_MEM_EXECUTE             0x20000000
#define IMAGE_SCN_MEM_READ                0x40000000
#define IMAGE_SCN_MEM_WRITE               0x80000000

#else
enum SectionFlags : uint32_t
{
	IMAGE_SCN_TYPE_NO_PAD            = 0x8,
	IMAGE_SCN_RESERVED_0001          = 0x10,
	IMAGE_SCN_CNT_CODE               = 0x20,
	IMAGE_SCN_CNT_INITIALIZED_DATA   = 0x40,
	IMAGE_SCN_CNT_UNINITIALIZED_DATA = 0x80,
	IMAGE_SCN_LNK_OTHER              = 0x100,
	IMAGE_SCN_LNK_INFO               = 0x200,
	IMAGE_SCN_RESERVED_0040          = 0x400,
	IMAGE_SCN_LNK_REMOVE             = 0x800,
	IMAGE_SCN_LNK_COMDAT             = 0x1000,
	IMAGE_SCN_GPREL                  = 0x8000,
	IMAGE_SCN_MEM_16BIT              = 0x20000,
	IMAGE_SCN_MEM_PURGEABLE          = 0x20000,
	IMAGE_SCN_MEM_LOCKED             = 0x40000,
	IMAGE_SCN_MEM_PRELOAD            = 0x80000,
	IMAGE_SCN_ALIGN_1BYTES           = 0x100000,
	IMAGE_SCN_ALIGN_2BYTES           = 0x200000,
	IMAGE_SCN_ALIGN_4BYTES           = 0x300000,
	IMAGE_SCN_ALIGN_8BYTES           = 0x400000,
	IMAGE_SCN_ALIGN_16BYTES          = 0x500000,
	IMAGE_SCN_ALIGN_32BYTES          = 0x600000,
	IMAGE_SCN_ALIGN_64BYTES          = 0x700000,
	IMAGE_SCN_ALIGN_128BYTES         = 0x800000,
	IMAGE_SCN_ALIGN_256BYTES         = 0x900000,
	IMAGE_SCN_ALIGN_512BYTES         = 0xa00000,
	IMAGE_SCN_ALIGN_1024BYTES        = 0xb00000,
	IMAGE_SCN_ALIGN_2048BYTES        = 0xc00000,
	IMAGE_SCN_ALIGN_4096BYTES        = 0xd00000,
	IMAGE_SCN_ALIGN_8192BYTES        = 0xe00000,
	IMAGE_SCN_LNK_NRELOC_OVFL        = 0x1000000,
	IMAGE_SCN_MEM_DISCARDABLE        = 0x2000000,
	IMAGE_SCN_MEM_NOT_CACHED         = 0x4000000,
	IMAGE_SCN_MEM_NOT_PAGED          = 0x8000000,
	IMAGE_SCN_MEM_SHARED             = 0x10000000,
	IMAGE_SCN_MEM_EXECUTE            = 0x20000000,
	IMAGE_SCN_MEM_READ               = 0x40000000,
	IMAGE_SCN_MEM_WRITE              = 0x80000000,
};
DEFINE_ENUM_FLAG_OPERATORS(SectionFlags);
#endif

#endif


#ifndef RT_ICON
// Assume none of the resource types have been defined

#define RT_CURSOR			1
#define RT_BITMAP			2
#define RT_ICON				3
#define RT_MENU				4
#define RT_DIALOG			5
#define RT_STRING			6
#define RT_FONTDIR			7
#define RT_FONT				8
#define RT_ACCELERATOR		9
#define RT_RCDATA			10
#define RT_MESSAGETABLE		11

#define RT_GROUP_CURSOR		12
#define RT_GROUP_ICON		13
#define RT_VERSION			16
#define RT_DLGINCLUDE		17

#define RT_PLUGPLAY			19
#define RT_VXD				20
#define RT_ANICURSOR		21
#define RT_ANIICON			22

#define RT_HTML				23
#define RT_MANIFEST			24

#endif


/////////////////////////////////////////////////////////////////////
// Structures

struct IMAGE_DOS_HEADER
{
	ascii e_magic[2];			// Magic number
	uint16_t e_cblp;			// Bytes of last page
	uint16_t e_cp;				// Pages in file
	uint16_t e_crlc;			// Relocations
	uint16_t e_cparhdr;			// Size of header in paragraphs
	uint16_t e_minalloc;		// Minimum extra paragraphs needed
	uint16_t e_maxalloc;		// Maximum extra paragraphs needed
	uint16_t e_ss;				// Initial (relative) SS value
	uint16_t e_sp;				// Initial SP value
	uint16_t e_csum;			// Checksum
	uint16_t e_ip;				// Initial IP value
	uint16_t e_cs;				// Initial (relative) CS value
	uint16_t e_lfarlc;			// File address of relocation table
	uint16_t e_ovno;			// Overlay number
	uint16_t e_res[4];			// Reserved words
	uint16_t e_oemid;			// OEM identifier (for e_oeminfo)
	uint16_t e_oeminfo;			// OEM information; e_oemid specific
	uint16_t e_res2[10];		// Reserved words
	uint32_t e_lfanew;			// File address of new exe header
	uint8_t e_program[64];		// Actual DOS program
};

struct IMAGE_FILE_HEADER
{
	uint16_t Machine;				// 332
	uint16_t NumberOfSections;
	uint32_t TimeDateStamp;
	uint32_t PointerToSymbolTable;	// 0
	uint32_t NumberOfSymbols;		// 0
	uint16_t SizeOfOptionalHeader;	// 0xe0
	uint16_t Characteristics;		// 0x10f
};

struct IMAGE_DATA_DIRECTORY
{
	rva32_t VirtualAddress;
	uint32_t Size;
};

struct IMAGE_OPTIONAL_HEADER32
{
	uint16_t Magic; // "MZ"
	uint8_t MajorLinkerVersion;
	uint8_t MinorLinkerVersion;
	uint32_t SizeOfCode;
	uint32_t SizeOfInitializedData;
	uint32_t SizeOfUninitializedData;
	rva32_t AddressOfEntryPoint;
	rva32_t BaseOfCode;
	rva32_t BaseOfData;
	address32_t ImageBase;
	uint32_t SectionAlignment;
	uint32_t FileAlignment;
	uint16_t MajorOperatingSystemVersion;
	uint16_t MinorOperatingSystemVersion;
	uint16_t MajorImageVersion;
	uint16_t MinorImageVersion;
	uint16_t MajorSubsystemVersion;
	uint16_t MinorSubsystemVersion;
	uint32_t Win32VersionValue;
	uint32_t SizeOfImage;
	uint32_t SizeOfHeaders;
	uint32_t CheckSum;
	uint16_t Subsystem;
	uint16_t DllCharacteristics;
	uint32_t SizeOfStackReserve;
	uint32_t SizeOfStackCommit;
	uint32_t SizeOfHeapReserve;
	uint32_t SizeOfHeapCommit;
	uint32_t LoaderFlags;
	uint32_t NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[16];
};

struct IMAGE_NT_HEADERS32
{
	ascii Signature[4]; // "PE"
	IMAGE_FILE_HEADER FileHeader;
	IMAGE_OPTIONAL_HEADER32 OptionalHeader;
};

struct IMAGE_SECTION_HEADER
{
	ascii Name[8];					// Not guaranteed to be null terminated!!!
	uint32_t VirtualSize; //union Misc Misc;
	rva32_t VirtualAddress;
	uint32_t SizeOfRawData;
	fileoff32_t PointerToRawData;
	uint32_t PointerToRelocations;
	uint32_t PointerToLinenumbers;
	uint16_t NumberOfRelocations;
	uint16_t NumberOfLinenumbers;
	SectionFlags Characteristics;
};


struct IMAGE_IMPORT_DESCRIPTOR
{
	rva32_t OriginalFirstThunk;
	uint32_t TimeDateStamp;
	uint32_t ForwarderChain;
	rva32_t Name;
	rva32_t FirstThunk;
};

struct IMAGE_IMPORT_BY_NAME
{
	uint16_t Hint;
	ascii Name[1]; // dummy size
};


struct IMAGE_RESOURCE_DIRECTORY_ENTRY
{
	union
	{
		uint16_t Id;					// uint16 ID number.
		struct
		{
			sectionoff32_t OffsetToName : 31;	// Pointer to string ID name. (offset type unknown)
			uint32_t NameIsString : 1;			// Non-zero if name is used to determine resource ID, do not use Id field.
		};
	};
	sectionoff32_t OffsetToData : 31;	// Section-relative offset to next directory, or IMAGE_RESOURCE_DATA_ENTRY.
	uint32_t DataIsDirectory : 1;		// Non-zero if next offset is directory.
};

struct IMAGE_RESOURCE_DATA_ENTRY
{
	rva32_t OffsetToData;
	uint32_t Size;
	uint32_t CodePage;
	uint32_t Reserved;
};

struct IMAGE_RESOURCE_DIRECTORY
{
	uint32_t Characteristics;
	uint32_t TimeDateStamp;
	uint16_t MajorVersion;
	uint16_t MinorVersion;
	uint16_t NumberOfNamedEntries;
	uint16_t NumberOfIdEntries;
};

}
