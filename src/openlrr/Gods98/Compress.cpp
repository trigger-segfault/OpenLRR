// Compress.cpp : 
//

#include "Compress.h"


/**********************************************************************************
 ******** Macro Functions
 **********************************************************************************/

#pragma region Macros

// <https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/byteswap-uint64-byteswap-ulong-byteswap-ushort?view=msvc-160>
#define BE_uint16(val) ((uint16)_byteswap_ushort((val)))
#define BE_uint32(val) ((uint32)_byteswap_ulong((val)))

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00558d68>
Gods98::RNC_Globs & Gods98::rncGlobs = *(Gods98::RNC_Globs*)0x00558d68;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0049ca80>
uint32 __cdecl Gods98::RNC_Uncompress(IN void* bufferIn, OUT void** bufferOut)
{
	// There are a lot of potential issues implementing this, and little benefit.
	// 
	// No existing WAD files EVER use this method, and no tools are available to
	//  easily create WADs using this compression method.
	//return ((uint32(__cdecl*)(void*, void**))0x0049ca80)(bufferIn, bufferOut);

	const RNC_Header* hdr = (const RNC_Header*)bufferIn;

	uint32 origSize = BE_uint32(hdr->beOrigSize);
	if (bufferOut == nullptr)
		return origSize; // return orig data size, allowing user to query the size of uncompressed data
	
	uint8* newBufferOut = (uint8*)std::malloc(origSize);
	if (newBufferOut != nullptr) {
		RNCError rstatus = _RNC_Uncompress(bufferIn, newBufferOut);
		if (rstatus == RNCError::RNC_OK /*0*/) {
			*bufferOut = newBufferOut;
			return origSize;
		}
		std::free(newBufferOut);
	}

	return 0;
}


// Expects an allocated bufferOut of the required size
// <LegoRR.exe @0049cb00>
Gods98::RNCError __cdecl Gods98::_RNC_Uncompress(IN void* bufferIn, OUT void* bufferOut)
{
	//return ((Gods98::RNCError(__cdecl*)(void*, void*))0x0049cb00)(bufferIn, bufferOut);

	const RNC_Header* hdr = (const RNC_Header*)bufferIn;

	// check compression signature
	if (std::strncmp(hdr->signature, "RNC", 3) != 0)
		return RNCError::RNC_INVALIDFILE /*-1*/;

	switch (hdr->compression) {
	case RNCCompression::RNC_COMPRESS_STORE /*0*/:
		std::memcpy(bufferOut, (const uint8*)bufferIn + 0x12 /*hdr->data*/, BE_uint32(hdr->beOrigSize));
		return RNCError::RNC_OK /*0*/;

	case RNCCompression::RNC_COMPRESS_BEST /*1*/:
		return _RNC_Uncompress_Method1(bufferIn, bufferOut);
	
	case RNCCompression::RNC_COMPRESS_FAST /*2*/:
		return _RNC_Uncompress_Method2(bufferIn, bufferOut);

	default:
		return RNCError::RNC_INVALIDCOMPRESSION /*-2*/;
	}
}

// <LegoRR.exe @0049cba0>
Gods98::RNCError __cdecl Gods98::_RNC_Uncompress_Method1(IN void* bufferIn, OUT void* bufferOut)
{
	return ((Gods98::RNCError(__cdecl*)(void*, void*))0x0049cba0)(bufferIn, bufferOut);
}

// <LegoRR.exe @0049cd20>
Gods98::RNCError __cdecl Gods98::_RNC_Uncompress_Method2(IN void* bufferIn, OUT void* bufferOut)
{
	return ((Gods98::RNCError(__cdecl*)(void*, void*))0x0049cd20)(bufferIn, bufferOut);
}

// Everything beyond this point is not reachable in the call tree yet.
//  _RNC_Uncompress_Method1 and _RNC_Uncompress_Method2 are both pretty complicated.
//  And hooking individual functions for testing may cause side effects as I believe
//  this module is written in assembly, and some weird optimizations may make this a pain.

// <LegoRR.exe @0049cf30>
void __cdecl Gods98::RNC_BitStreamInit(void* bufferIn, void* bufferOut)
{
	const RNC_Header* hdr = (const RNC_Header*)bufferIn;

	rncGlobs.BitCount = 0;
	rncGlobs.Input = (const uint8*)bufferIn + 0x12; //hdr->data;
	rncGlobs.Output = (uint8*)bufferOut;
	rncGlobs.OutputEnd = (const uint8*)bufferOut + BE_uint32(hdr->beOrigSize);
}

// Method 1 exclusive
// <LegoRR.exe @0049cf80>
uint32 __cdecl Gods98::RNC_M1_BitStreamAdvance(uint8 bits)
{
	uint32 result = 0;
	for (sint32 k = 0; k < bits; k++) {
		if (rncGlobs.BitCount == 0) {
			// Refill BitBuffer with current Input WORD ptr, then increment
			// Although this is loaded as a dword ptr, there should
			//  logically never be a case where BitBuffer > USHORT_MAX.
			// Some of these awkward decompilations convince me this was written in assembly.
			rncGlobs.BitBuffer = *(uint32*)((uint16*)rncGlobs.Input++); // get DWORD ptr, then increment as WORD ptr
			//rncGlobs.Input = (rncGlobs.Input + 2); // (uint16*)Input++
			rncGlobs.BitCount = 16;
		}

		if (rncGlobs.BitBuffer & 0x1) {
			result |= (1 << k);
		}
		rncGlobs.BitBuffer >>= 1;
		rncGlobs.BitCount--;
	}

	return result;
}

// Method 2 exclusive
// <LegoRR.exe @0049cff0>
uint32 __cdecl Gods98::RNC_M2_BitStreamAdvance(uint8 bits)
{
	uint32 result = 0;
	for (sint32 k = 0; k < bits; k++) {
		if (rncGlobs.BitCount == 0) {
			// Refill ByteBuffer with current Input BYTE ptr, then increment
			rncGlobs.ByteBuffer = *rncGlobs.Input++;
			rncGlobs.BitCount = 8;
		}

		result <<= 1;
		if ((rncGlobs.ByteBuffer & 0x80) != 0) {
			result |= 0x1; // result++; // I think this is just |= 0x1;
		}
		rncGlobs.ByteBuffer <<= 1;
		rncGlobs.BitCount--;
	}

	return result;
}

// Method 1 exclusive
// <LegoRR.exe @0049d050>
void __cdecl Gods98::RNC_M1_ReadHuffmanTable(HuffmanLeaf* table, uint8 bits)
{
	RNC_M1_HuffmanReset(table, bits);

	uint8 bits2 = (uint8)RNC_M1_BitStreamAdvance(5);
	if (bits2 != 0) {
		if (bits2 > 16) bits2 = 16;

		for (sint32 k = 0; k < bits2; k++) {
			table[k].count_2 = (uint16)RNC_M1_BitStreamAdvance(4);
		}

		RNC_M1_HuffmanAssignValues(table, bits2);
	}
}

// Method 1 exclusive
// <LegoRR.exe @0049d0c0>
uint16 __cdecl Gods98::RNC_M1_ReadHuffman(HuffmanLeaf* table)
{
	uint8 k = 0;
	while (!table[k].count_2 || (((1 << (uint8)table[k].count_2) - 1U) & rncGlobs.BitBuffer) != table[k].value_2) {
		k++;
	}

	RNC_M1_BitStreamAdvance((uint8)table->count_2);
	if (k < 2) {
		return (uint16)k; // no additional data to shift/add in, return k
	}

	return (uint16)(1 << (k - 1)) | (uint16)RNC_M1_BitStreamAdvance(k - 1);
}

// Method 2 exclusive
// <LegoRR.exe @0049d130>
uint16 __cdecl Gods98::RNC_M2_ReadWindow(void)
{
	// returns value in the range: 4 -> 9
	uint16 num1 = (uint16)RNC_M2_BitStreamAdvance(1); // A (bit 0)

	uint16 bNoExtra = (uint16)RNC_M2_BitStreamAdvance(1); // B (bit 1)
	if (!bNoExtra) {
		// 0b0000010A
		return 0x4 | num1; // 4,5
	}

	// 0b0000100C if (A)
	// 0b0000011C if (!A)
	// 0 = 0xfffe, 1 = 0xffff
	sint16 num2 = (sint16)RNC_M2_BitStreamAdvance(1); // C (bit 2)
	//return 0x4 | (num1 << 1) | 
	return ((sint16)num2 - 2) + ((num1 | 0x4) << 1);
}

// Method 2 exclusive
// <LegoRR.exe @0049d170>
uint16 __cdecl Gods98::RNC_M2_ReadCode(void)
{
	uint16 value = 0;

	uint16 bIsWord = (uint16)RNC_M2_BitStreamAdvance(1); // (bit 0)
	if (bIsWord) {
		value = (uint16)RNC_M2_BitStreamAdvance(1); // A (bit 1)

		uint16 bNoExtra = (uint16)RNC_M2_BitStreamAdvance(1); // B (bit 2)
		if (!bNoExtra) {
			if (value == 0) {
				// 0b0000001C
				value = 0x2 | (uint16)RNC_M2_BitStreamAdvance(1); // C (bit 3)
			}
			//else {
			//	// 0b0000000A
			//}
		}
		else {
			// 0b00000BAC
			value = 0x4 | (value << 1) | (uint16)RNC_M2_BitStreamAdvance(1); // C (bit 3)

			bNoExtra = (uint16)RNC_M2_BitStreamAdvance(1); // D (bit 4)
			if (!bNoExtra) {
				// 0b0000BACE
				value = (value << 1) | (uint16)RNC_M2_BitStreamAdvance(1); // E (bit 5)
			}
		}
	}

	// +1 for minimum of 1
	return (value << 8) + 1 + (uint16)*rncGlobs.Input++;
}

// Method 1 exclusive
// <LegoRR.exe @0049d210>
void __cdecl Gods98::RNC_M1_HuffmanReset(HuffmanLeaf* table, uint8 bits)
{
	for (sint32 k = 0; k < bits; k++) {
		table[k].value_1 = 0;
		table[k].count_1 = 0xffff; // USHRT_MAX
		table[k].value_2 = 0;
		table[k].count_2 = 0;
	}
}

// Method 1 exclusive
// <LegoRR.exe @0049d250>
void __cdecl Gods98::RNC_M1_HuffmanAssignValues(HuffmanLeaf* table, uint8 bits)
{
	uint32 value = 0;
	uint32 bitmask = 0x80000000;

	for (uint16 k = 1; k <= 16; k++) {
		for (uint32 i = 0; i < (uint32)bits; i++) {
			if (table[i].count_2 == (uint16)k) {
				// There may be a bitwise-way to implement the division and addition,
				//  but I'm not too sure.
				table[i].value_2 = RNC_M1_MirrorBits(value / bitmask, (uint8)k);
				value += bitmask;
			}
		}
		bitmask >>= 1;
	}
}

// Method 1 exclusive
// <LegoRR.exe @0049d2c0>
uint32 __cdecl Gods98::RNC_M1_MirrorBits(uint32 value, uint8 bits)
{
	uint32 result = 0;
	for (sint32 k = 0; k < bits; k++) {
		result <<= 1;
		if (value & 0x1)
			result |= 0x1;
		value >>= 1;
	}
	return result;
}

#pragma endregion
