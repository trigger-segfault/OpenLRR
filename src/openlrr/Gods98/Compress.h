// Compress.h : 
//
/// APIS: -
/// DEPENDENCIES: -
/// DEPENDENTS: Wad

#pragma once

#include "../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Typedefs
 **********************************************************************************/

#pragma region Typedefs

/// Informative typedefs for integers stored in Big Endian byte order.
typedef uint16 be_uint16;
typedef uint32 be_uint32;

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define COMPRESSION_UNPACK_BUFFER_EXTEND 20000

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

namespace _ns_RNCError {
enum RNCError : sint16
{
	RNC_INVALIDCOMPRESSION = -2,
	RNC_INVALIDFILE        = -1,
	RNC_OK                 = 0,
};
static_assert(sizeof(RNCError) == 0x2, "");
} using RNCError = _ns_RNCError::RNCError;


namespace _ns_RNCCompression {
enum RNCCompression : uint8
{
	RNC_COMPRESS_STORE = 0,
	RNC_COMPRESS_BEST  = 1,
	RNC_COMPRESS_FAST  = 2,
};
static_assert(sizeof(RNCCompression) == 0x1, "");
} using RNCCompression = _ns_RNCCompression::RNCCompression;

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

#pragma pack(push, 1)

// <https://segaretro.org/Rob_Northen_compression>
// BIG ENDIAN byte order!!!!
struct RNC_Header
{
	/*00,3*/ char signature[3]; // "RNC"
	/*03,1*/ RNCCompression compression; // 1 or 2
	/*04,4*/ be_uint32 beOrigSize;
	/*08,4*/ be_uint32 bePackedSize;
	/*0c,2*/ be_uint16 beOrigCRC;
	/*0e,2*/ be_uint16 bePackedCRC; // Checksum of the RNC data
	/*10,1*/ uint8 leeway; // Difference between compressed and uncompressed data in largest pack chunk (if larger than decompressed data)
	/*11,1*/ uint8 packChunks; // Amount of pack chunks
	///*....*/ uint8 data[1]; // (dummy)
	/*12*/
};
static_assert(sizeof(RNC_Header) == 0x12, "");

#pragma pack(pop)


struct HuffmanLeaf
{
	/*00,4*/ uint32 value_1;
	/*04,2*/ uint16 count_1;
	/*06,2*/ //<uint16 padding1;>
	/*08,4*/ uint32 value_2;
	/*0c,2*/ uint16 count_2; // bit_depth
	/*0e,2*/ //<uint16 padding2;>
	/*10*/
};
static_assert(sizeof(HuffmanLeaf) == 0x10, "");


struct RNC_Globs
{
	/*000,1*/ uint8 BitCount;
	/*003,3*/ //<uint8 padding1[3];>
	/*004,14*/ uint32 reserved1[5]; // probably used for compression methods
	/*018,100*/ HuffmanLeaf HuffmanTable_Raw[16];
	/*118,100*/ HuffmanLeaf HuffmanTable_Dst[16];
	/*218,100*/ HuffmanLeaf HuffmanTable_Len[16];
	/*318,4*/ const uint8* Input; /// TODO: See if extra changes are needed to support const
	/*31c,4*/ uint8* Output;
	/*320,1*/ uint8 ByteBuffer; // Method 2 exclusive: uint16 RNC_M2_BitStreamAdvance(uint8 bits)
	/*323,3*/ //<uint8 padding2[3];>
	/*324,4*/ uint32 BitBuffer;
	/*328,4*/ const uint8* OutputEnd;
	/*32c*/
};
static_assert(sizeof(RNC_Globs) == 0x32c, "");

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00558d68>
extern RNC_Globs & rncGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// bufferOut is allocated by this function, and must be freed using the same memory management method.
// <LegoRR.exe @0049ca80>
uint32 __cdecl RNC_Uncompress(IN void* bufferIn, OUT void** bufferOut);


// Expects an allocated bufferOut of the required size
// <LegoRR.exe @0049cb00>
RNCError __cdecl _RNC_Uncompress(IN void* bufferIn, OUT void* bufferOut);

// <LegoRR.exe @0049cba0>
RNCError __cdecl _RNC_Uncompress_Method1(IN void* bufferIn, OUT void* bufferOut);

// <LegoRR.exe @0049cd20>
RNCError __cdecl _RNC_Uncompress_Method2(IN void* bufferIn, OUT void* bufferOut);

// <LegoRR.exe @0049cf30>
void __cdecl RNC_BitStreamInit(void* bufferIn, void* bufferOut);

// Method 1 exclusive
// <LegoRR.exe @0049cf80>
uint32 __cdecl RNC_M1_BitStreamAdvance(uint8 bits);

// Method 2 exclusive
// <LegoRR.exe @0049cff0>
uint32 __cdecl RNC_M2_BitStreamAdvance(uint8 bits);

// Method 1 exclusive
// <LegoRR.exe @0049d050>
void __cdecl RNC_M1_ReadHuffmanTable(HuffmanLeaf* table, uint8 bits);

// Method 1 exclusive
// <LegoRR.exe @0049d0c0>
uint16 __cdecl RNC_M1_ReadHuffman(HuffmanLeaf* table);

// Method 2 exclusive
// <LegoRR.exe @0049d130>
uint16 __cdecl RNC_M2_ReadWindow(void);

// Method 2 exclusive
// <LegoRR.exe @0049d170>
uint16 __cdecl RNC_M2_ReadCode(void);

// Method 1 exclusive
// <LegoRR.exe @0049d210>
void __cdecl RNC_M1_HuffmanReset(HuffmanLeaf* table, uint8 bits);

// Method 1 exclusive
// <LegoRR.exe @0049d250>
void __cdecl RNC_M1_HuffmanAssignValues(HuffmanLeaf* table, uint8 bits);

// Method 1 exclusive
// <LegoRR.exe @0049d2c0>
uint32 __cdecl RNC_M1_MirrorBits(uint32 value, uint8 bits);

#pragma endregion

}
