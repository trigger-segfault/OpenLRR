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

// shorthand for an N-bit mask applied to a value
#define RNC_Mask(bits, value) (((1 << ((bits) & 0x1f)) - 1U) & (value))

// Extra safety bounds checking (only half-implemented for Method1)
#define RNC_BOUNDS_CHECKING

// Call the original uncompress functions when defined
//#define RNC_NATIVE_UNCOMPRESS

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

//#pragma optimize("", off)

// <LegoRR.exe @0049ca80>
uint32 __cdecl Gods98::RNC_Uncompress(IN const void* bufferIn, OUT void** bufferOut)
{
	log_firstcall();

	const RNC_Header* hdr = (const RNC_Header*)bufferIn;

	uint32 origSize = BE_uint32(hdr->beOrigSize);
	if (bufferOut == nullptr)
		return origSize; // return orig data size, allowing user to query the size of uncompressed data
	
	uint8* newBufferOut = (uint8*)std::malloc(origSize);
	if (newBufferOut != nullptr) {
		RNCError rstatus = _RNC_Uncompress(bufferIn, newBufferOut);
		if (rstatus == RNCError::OK /*0*/) {
			*bufferOut = newBufferOut;
			return origSize;
		}
		std::free(newBufferOut);
	}

	return 0;
}


// Expects an allocated bufferOut of the required size
// <LegoRR.exe @0049cb00>
Gods98::RNCError __cdecl Gods98::_RNC_Uncompress(IN const void* bufferIn, OUT void* bufferOut)
{
	log_firstcall();

	const RNC_Header* hdr = (const RNC_Header*)bufferIn;

	// check compression signature
	if (std::strncmp(hdr->signature, "RNC", 3) != 0)
		return RNCError::INVALIDFILE /*-1*/;

	switch (hdr->compression) {
	case RNCCompression::RNC_COMPRESS_STORE /*0*/:
		//std::printf("RNC_COMPRESS_STORE\n");
		std::memcpy(bufferOut, (const uint8*)hdr->data, BE_uint32(hdr->beOrigSize));
		return RNCError::OK /*0*/;

	case RNCCompression::RNC_COMPRESS_BEST /*1*/:
		//std::printf("RNC_COMPRESS_BEST\n");
		return RNC_M1_Uncompress(bufferIn, bufferOut);

	case RNCCompression::RNC_COMPRESS_FAST /*2*/:
		//std::printf("RNC_COMPRESS_FAST\n");
		return RNC_M2_Uncompress(bufferIn, bufferOut);

	default:
		return RNCError::INVALIDCOMPRESSION /*-2*/;
	}
}

//#pragma optimize("", on)

// <LegoRR.exe @0049cba0>
Gods98::RNCError __cdecl Gods98::RNC_M1_Uncompress(IN const void* bufferIn, OUT void* bufferOut)
{
	log_firstcall();

#ifdef RNC_NATIVE_UNCOMPRESS
	return ((Gods98::RNCError(__cdecl*)(const void*, void*))0x0049cba0)(bufferIn, bufferOut);

#else

	RNC_BitStreamInit(bufferIn, bufferOut);

	RNC_M1_BitStreamAdvance(2); // skip the first 2 bits

	while (rncGlobs.Output < rncGlobs.OutputEnd) {
		RNC_M1_ReadHuffmanTable(rncGlobs.HuffmanTable_Raw, 16);
		RNC_M1_ReadHuffmanTable(rncGlobs.HuffmanTable_Dst, 16);
		RNC_M1_ReadHuffmanTable(rncGlobs.HuffmanTable_Len, 16);

		uint16 runCount = (uint16)RNC_M1_BitStreamAdvance(16);

		assert(runCount != 0);
		bool isDataRun = true;
		while (runCount) {
		//for (sint32 j = 0; j < runCount; j++) {
			if (isDataRun) {
				////// DATA RUN //////
				uint16 runDataLength = (uint16)RNC_M1_ReadHuffman(rncGlobs.HuffmanTable_Raw);

				#ifdef BOUNDS_CHECKING
				if (rncGlobs.Output + runDataLength > rncGlobs.OutputEnd)
					return RNCError::INVALIDCOMPRESSION;
				#endif

				//std::memcpy(rncGlobs.Output, rncGlobs.Input, runDataLength);
				for (sint32 i = 0; i < runDataLength; i++) {
					*rncGlobs.Output++ = *rncGlobs.Input++;
				}

				// no idea what the purpose of this is
				uint32 input24 = ((uint32)rncGlobs.Input[2]<<16) | ((uint32)rncGlobs.Input[1]<<8) | (uint32)rncGlobs.Input[0];
				rncGlobs.BitBuffer = (input24 << (rncGlobs.BitCount & 0x1f)) | RNC_Mask(rncGlobs.BitCount, rncGlobs.BitBuffer);

				runCount--;
			}
			else {
				////// COPY RUN //////
				uint16 prevRunOffset = (uint16)RNC_M1_ReadHuffman(rncGlobs.HuffmanTable_Dst);
				const uint8* prevOutput = rncGlobs.Output - (uint32)prevRunOffset - 1; // minimum of -1 offset
				uint16 runCopyLength = (uint16)RNC_M1_ReadHuffman(rncGlobs.HuffmanTable_Len);

				// This is an overlapped copy, meaning any new data copied into
				//  rncGlobs.Output can still be used, as a sort-of auto-repeat mechanism.
				// Overlapped copy is a common occurance in compression algorithms.

				// The assembly for this is really muddled, but the condition is that
				//  the for loop will NEVER iterate for more than SHRT_MAX times
				//  as the original `if (runCopyLength != -2)` ensures that the
				//  `(runCopyLength+1 & 0xffff) + 1` count will never be 0x10000.

				runCopyLength += 2; // copy a minimum of 2 bytes
				
				#ifdef BOUNDS_CHECKING
				if (rncGlobs.Output + runCopyLength > rncGlobs.OutputEnd)
					return RNCError::INVALIDCOMPRESSION;
				#endif

				//overlapped_memcpy(rncGlobs.Output, prevOutput, runCopyLength);
				for (sint32 i = 0; i < runCopyLength; i++) {
					*rncGlobs.Output++ = *prevOutput++;
				}

				// runCount is not decremented on the copy command, I'm not sure how that's supposed to work
			}

			isDataRun = !isDataRun;
		}
	}
	return RNCError::OK;
#endif
}

// <LegoRR.exe @0049cd20>
Gods98::RNCError __cdecl Gods98::RNC_M2_Uncompress(IN const void* bufferIn, OUT void* bufferOut)
{
	log_firstcall();

#ifdef RNC_NATIVE_UNCOMPRESS
	return ((Gods98::RNCError(__cdecl*)(const void*, void*))0x0049cd20)(bufferIn, bufferOut);

#else

	RNC_BitStreamInit(bufferIn, bufferOut);

	RNC_M2_BitStreamAdvance(2); // skip the first 2 bits

	while (rncGlobs.Output < rncGlobs.OutputEnd) {

		if (!RNC_M2_BitStreamAdvance(1)) { // single byte data run //
			*rncGlobs.Output++ = *rncGlobs.Input++;
		}
		else if (!RNC_M2_BitStreamAdvance(1)) { // data run or copy run //
			uint16 runCode;

			if ((runCode = RNC_M2_ReadLengthCode()) == 9) { // 9 is max return from RNC_M2_ReadLengthCode
				// data run //
				uint16 runDataLength = (uint16)RNC_M2_BitStreamAdvance(4);

				runDataLength = (uint16)((uint32)runDataLength * 4 + 12);

				//std::memcpy(rncGlobs.Output, rncGlobs.Input, runDataLength);
				for (sint32 i = 0; i < runDataLength; i++) {
					*rncGlobs.Output++ = *rncGlobs.Input++;
				}
			}
			else {
				// basic copy run //
				uint16 runCopyLength = runCode;
				uint16 prevRunOffset = RNC_M2_ReadOffsetCode();
				const uint8* prevOutput = rncGlobs.Output - (uint32)prevRunOffset;

				//overlapped_memcpy(rncGlobs.Output, prevOutput, runCopyLength);
				for (sint32 i = 0; i < runCopyLength; i++) {
					*rncGlobs.Output++ = *prevOutput++;
				}
			}
		}
		else { // other copy runs -or- no copy run //
			uint16 runCopyLength;
			uint16 prevRunOffset;

			if (!RNC_M2_BitStreamAdvance(1)) {
				runCopyLength = 2;
				prevRunOffset = (*rncGlobs.Input++) + 1;
			}
			else if (!RNC_M2_BitStreamAdvance(1)) {
				runCopyLength = 3;
				prevRunOffset = (uint32)RNC_M2_ReadOffsetCode();
			}
			else if ((runCopyLength = (*rncGlobs.Input++)) != 0) {
				runCopyLength += 8;
				prevRunOffset = (uint32)RNC_M2_ReadOffsetCode();
			}
			else {
				// no copy run //
				RNC_M2_BitStreamAdvance(1); // skip next 1 bit
				continue;
			}

			const uint8* prevOutput = rncGlobs.Output - (uint32)prevRunOffset;

			//overlapped_memcpy(rncGlobs.Output, prevOutput, runLength);
			for (sint32 i = 0; i < runCopyLength; i++) {
				*rncGlobs.Output++ = *prevOutput++;
			}
		}
	}
	return RNCError::OK;
#endif
}

// <LegoRR.exe @0049cf30>
void __cdecl Gods98::RNC_BitStreamInit(const void* bufferIn, void* bufferOut)
{
	const RNC_Header* hdr = (const RNC_Header*)bufferIn;

	rncGlobs.BitCount = 0;
	rncGlobs.Input = (const uint8*)hdr->data; // bufferIn + 0x12;
	rncGlobs.Output = (uint8*)bufferOut;
	rncGlobs.OutputEnd = (const uint8*)bufferOut + BE_uint32(hdr->beOrigSize);
}

// <LegoRR.exe @0049cf80>
uint32 __cdecl Gods98::RNC_M1_BitStreamAdvance(uint8 bits)
{
	uint32 result = 0;
	for (sint32 k = 0; k < bits; k++) {
		if (rncGlobs.BitCount == 0) {
			// Refill BitBuffer with current Input DWORD ptr
			rncGlobs.BitBuffer = *(uint32*)rncGlobs.Input; // get DWORD ptr
			rncGlobs.Input += 2; // then increment as WORD ptr...
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

// <LegoRR.exe @0049cff0>
uint32 __cdecl Gods98::RNC_M2_BitStreamAdvance(uint8 bits)
{
	uint32 result = 0;
	for (sint32 k = 0; k < bits; k++) {
		if (rncGlobs.BitCount == 0) {
			// Refill ByteBuffer with current Input BYTE ptr
			rncGlobs.ByteBuffer = *rncGlobs.Input; // get BYTE ptr
			rncGlobs.Input += 1; // then increment as BYTE ptr
			rncGlobs.BitCount = 8;
		}

		result <<= 1;
		if (rncGlobs.ByteBuffer & 0x80) {
			result |= 0x1;
		}

		rncGlobs.ByteBuffer <<= 1;
		rncGlobs.BitCount--;
	}

	return result;
}

// <LegoRR.exe @0049d050>
void __cdecl Gods98::RNC_M1_ReadHuffmanTable(HuffmanLeaf* table, uint8 bits)
{
	// Probably reset the last (or max) number of bits used by the table
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

// <LegoRR.exe @0049d0c0>
uint16 __cdecl Gods98::RNC_M1_ReadHuffman(HuffmanLeaf* table)
{
	uint8 k = 0;
	
	#ifdef RNC_BOUNDS_CHECKING
	for (k = 0; k < 16; k++) {
		// count_2 shouldn't be zero, as that would result in a mask of 0x0
		if (table[k].count_2 && RNC_Mask(table[k].count_2, rncGlobs.BitBuffer) == table[k].value_2)
			break;
	}
	assert(k != 16);

	#else
	while (!table[k].count_2 || RNC_Mask(table[k].count_2, rncGlobs.BitBuffer) != table[k].value_2) {
		k++;
	}

	#endif

	// under current circumstances, count_2 should never be > 16. So we don't apply the hiword mask.
	uint32 hiword = RNC_M1_BitStreamAdvance((uint8)table[k].count_2);
	if (k < 2) {
		//return hiword & 0xffff0000 | (uint32)k;
		return (uint16)k; // no additional data to shift/add in, return k
	}

	return (uint16)RNC_M1_BitStreamAdvance(k - 1) | (uint16)(1 << ((k - 1) & 0x1f));
}

// <LegoRR.exe @0049d130>
uint16 __cdecl Gods98::RNC_M2_ReadLengthCode(void)
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
	return (uint16)( ((sint16)num2 - 2) + ((num1 | 0x4) << 1) );
}

// <LegoRR.exe @0049d170>
uint16 __cdecl Gods98::RNC_M2_ReadOffsetCode(void)
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
	return (value << 8) + 1 + (uint16)(*rncGlobs.Input++);
}

// <LegoRR.exe @0049d210>
void __cdecl Gods98::RNC_M1_HuffmanReset(HuffmanLeaf* table, uint8 bits)
{
	for (sint32 i = 0; i < bits; i++) {
		table[i].value_1 = 0;      // (unused in decompression)
		table[i].count_1 = 0xffff; // (unused in decompression) USHRT_MAX

		table[i].value_2 = 0;
		table[i].count_2 = 0;
	}
}

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

//#pragma optimize("", on)

#pragma endregion
