// openlrr-makeexe.cpp : Program to build "OpenLRR.exe" binary.
//

#include "common.h"
#include "pe/PEFile.h"
#include "pe/PESectionStream.h"


/////////////////////////////////////////////////////////////////////
// Defines

#define OUTEXE_NAME_RELEASE		_T("OpenLRR.exe")
#define OUTEXE_NAME_DEBUG		_T("OpenLRR-d.exe")

// Section where import descriptors are redefined with more room.
#define NEW_SECTION_NAME		".idata2"

// Import DLL containing thunk functions:
#define DLL_NAME_RELEASE		"openlrr.dll"
#define DLL_NAME_DEBUG			"openlrr-d.dll"

// Old LRR:CE import method (kept for backwards compatibility):
#define DUMMY_THUNK_NAME		"Dummy"
#define DUMMY_THUNK_HINT		((uint16_t)0)

// New OpenLRR import method:
#define START_THUNK_NAME		"StartOpenLRR"
#define START_THUNK_HINT		((uint16_t)0)

// Resource IDs to replace:
#define REPLACE_ICON_NAME		((uint16_t)1)
#define REPLACE_ICON_LANG		((uint16_t)2057)

// WinMain address for LegoRR.exe (masterpiece)
#define PROCESS_WINMAIN			0x477a60


/////////////////////////////////////////////////////////////////////

#pragma region Replace resource icon

struct IconResHeader
{
	uint32_t size; // size of this IconRes header struct
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bitCount;
	uint32_t compression;
	uint32_t sizeImage;
	uint32_t XpelsPerMeter;
	uint32_t YpelsPerMeter;
	uint32_t clrUsed;
	uint32_t clrImportant;
};

struct IconRes
{
	IconResHeader hdr;
	std::vector<uint8_t> ColorMap;	// size: ((hdr.bitCount==8) ? 1024 : 0)
	std::vector<uint8_t> ImageData;	// size: hdr.sizeImage
	std::vector<uint8_t> BitMask;	// size: ((hdr.bitCount==8) ? 128  : 0) ????

	inline IconRes() : hdr({ 0 }), ColorMap(), ImageData(), BitMask() { }
};

static bool ReadIconResFromFile(const tstring& filename, IconRes& icon)
{
	std::memset(&icon.hdr, 0, sizeof(icon.hdr));
	icon.ColorMap.clear();
	icon.ImageData.clear();
	icon.BitMask.clear();

	FILE* file = _tfopen(filename.c_str(), _T("rb"));
	if (file == nullptr)
		return false;

	// start of IconResHeader (bad assuption based on single BMP icon format)
	std::fseek(file, 0x16, SEEK_SET);

	constexpr uint32_t sizeSize = sizeof(icon.hdr.size);
	std::fread(&icon.hdr.size, sizeSize, 1, file);
	const uint32_t minSize = std::min(icon.hdr.size, sizeof(icon.hdr));
	std::fread((uint8_t*)&icon.hdr + sizeSize, minSize - sizeSize, 1, file);

	if (icon.hdr.size > sizeof(icon.hdr)) {
		// seek past remaining header to data
		std::fseek(file, icon.hdr.size - sizeof(icon.hdr), SEEK_CUR);
	}

	if (icon.hdr.bitCount == 8) {
		// NOTE: Resource icons do not include the palette in their sizeImage field,
		// but file icons do include it.
		// Strip the extra size away to conform when working with resource icons.
		assert(icon.hdr.sizeImage >= 1024);
		icon.hdr.sizeImage -= 1024;
		icon.ColorMap.resize(1024);
		std::fread(icon.ColorMap.data(), icon.ColorMap.size(), 1, file);
	}

	icon.ImageData.resize(icon.hdr.sizeImage);
	std::fread(icon.ImageData.data(), icon.ImageData.size(), 1, file);

	if (icon.hdr.bitCount == 8) {
		icon.BitMask.resize(128);
		std::fread(icon.BitMask.data(), icon.BitMask.size(), 1, file);
	}

	std::fclose(file);

	return true;
}

static bool ReadIconResFromRsrc(PE::PESectionStream& rsrcStream, IconRes& icon, rva32_t offset, uint32_t size)
{
	std::memset(&icon.hdr, 0, sizeof(icon.hdr));
	icon.ColorMap.clear();
	icon.ImageData.clear();
	icon.BitMask.clear();

	rsrcStream.SeekRVA(offset);

	constexpr uint32_t sizeSize = sizeof(icon.hdr.size);
	rsrcStream.Read(&icon.hdr.size, sizeSize);
	const uint32_t minSize = std::min(icon.hdr.size, sizeof(icon.hdr));
	rsrcStream.Read((uint8_t*)&icon.hdr + sizeSize, minSize - sizeSize);

	if (icon.hdr.size > sizeof(icon.hdr)) {
		// seek past remaining header to data
		rsrcStream.Seek(icon.hdr.size - sizeof(icon.hdr), SEEK_CUR);
	}

	if (icon.hdr.bitCount == 8) {
		icon.ColorMap.resize(1024);
		rsrcStream.Read(icon.ColorMap.data(), icon.ColorMap.size());
	}

	icon.ImageData.resize(icon.hdr.sizeImage);
	rsrcStream.Read(icon.ImageData.data(), icon.ImageData.size());

	if (icon.hdr.bitCount == 8) {
		icon.BitMask.resize(128);
		rsrcStream.Read(icon.BitMask.data(), icon.BitMask.size());
	}


	return true;
}

static void ReplaceRsrcIcon(PE::PESectionStream& rsrcStream, IconRes& newIcon, rva32_t offset, uint32_t size)
{
	IconRes icon;
	ReadIconResFromRsrc(rsrcStream, icon, offset, size);
	
	if (std::memcmp(&icon.hdr, &newIcon.hdr, sizeof(icon.hdr)) != 0) {
		throw std::exception("Icon resources differ, advanced icon replace not supported yet");
	}

	// Skip past header, as we've confirmed they're the same.
	rsrcStream.SeekRVA(offset + icon.hdr.size);

	if (newIcon.hdr.bitCount == 8) {
		rsrcStream.Write(newIcon.ColorMap.data(), newIcon.ColorMap.size());
	}

	rsrcStream.Write(newIcon.ImageData.data(), newIcon.ImageData.size());

	if (newIcon.hdr.bitCount == 8) {
		rsrcStream.Write(newIcon.BitMask.data(), newIcon.BitMask.size());
	}
	_tprintf(_T("Icon replaced   @ 0x%08x\n"), (uint32_t)offset);
}

#pragma endregion

#pragma region Update resource section

static void UpdateRsrcDirectory(PE::PESectionStream& rsrcStream, sectionoff32_t offset, rva_t virtDist, std::vector<int32_t>& resIdStack, IconRes* newIcon = nullptr, int32_t iconName = -1, int32_t iconLang = -1)
{
	PE::IMAGE_RESOURCE_DIRECTORY dir;
	rsrcStream.SeekSection(offset);
	rsrcStream.Read(&dir, sizeof(dir));

	uint32_t count = dir.NumberOfNamedEntries + dir.NumberOfIdEntries;
	PE::IMAGE_RESOURCE_DIRECTORY_ENTRY* entries = new PE::IMAGE_RESOURCE_DIRECTORY_ENTRY[count];

	rsrcStream.Read(entries, count * sizeof(*entries));

	for (uint32_t i = 0; i < count; i++) {
		auto& entry = entries[i];

		resIdStack.push_back((!entry.NameIsString ? entry.Id : -1)); // names not supported

		if (entry.DataIsDirectory) {
			UpdateRsrcDirectory(rsrcStream, entry.OffsetToData, virtDist, resIdStack, newIcon, iconName, iconLang);
		}
		else {
			bool replaceIcon = false;
			int32_t replaceIconName = -1;
			int32_t replaceIconLang = -1;
			if ((newIcon != nullptr && resIdStack.size() == 3) &&
				(resIdStack[0] != -1 && (resIdStack[0] == (int32_t)RT_ICON)) &&
				(resIdStack[1] != -1 && (resIdStack[1] == iconName || iconName == -1)) &&	// -1 for any name
				(resIdStack[2] != -1 && (resIdStack[2] == iconLang || iconLang == -1)))		// -1 for any language
			{
				replaceIcon = true;
				replaceIconName = resIdStack[1]; // in the scenario either of these fields were -1 for "any"
				replaceIconLang = resIdStack[2];
			}

			PE::IMAGE_RESOURCE_DATA_ENTRY data;
			rsrcStream.SeekSection(entry.OffsetToData);
			rsrcStream.Read(&data, sizeof(data));

			data.OffsetToData += (int32_t)virtDist; // Shift our data offset

			// Correct offset to data acquired, move to new icon location and modify
			if (replaceIcon) {
				ReplaceRsrcIcon(rsrcStream, *newIcon, data.OffsetToData, data.Size);
			}

			rsrcStream.SeekSection(entry.OffsetToData);
			rsrcStream.Write(&data, sizeof(data));
		}

		resIdStack.pop_back();
	}

	delete[] entries;
}

static void UpdateRsrcSection(PE::PEFile& pefile, PE::PESectionSPtr rsrc, fileoff_t dist, IconRes* newIcon = nullptr, int32_t iconName = -1, int32_t iconLang = -1)
{
	auto& resourcesDir = pefile.ResourceDirectory();

	auto& header = rsrc->Header;
	fileoff_t fileDist = pefile.FileAlign(header.PointerToRawData + dist) - header.PointerToRawData;
	rva_t virtDist = pefile.VirtualAlign(header.VirtualAddress + fileDist) - header.VirtualAddress;

	header.PointerToRawData += (int32_t)fileDist;
	header.VirtualAddress += (int32_t)virtDist;

	std::vector<int32_t> resIdStack;

	PE::PESectionStream rsrcStream = PE::PESectionStream(rsrc, false);
	UpdateRsrcDirectory(rsrcStream, 0, virtDist, resIdStack, newIcon, iconName, iconLang);

	// Update address of resource directory
	resourcesDir.VirtualAddress = header.VirtualAddress;


	_tprintf(_T("Resources moved @ 0x%08x\n"), header.VirtualAddress);
}

#pragma endregion


/////////////////////////////////////////////////////////////////////

#pragma region Add new imports

struct ImportThunk
{
	uint16_t Hint;
	std::string Name;
	rva32_t VirtualAddress; // RVA -> start of Hint

	ImportThunk(uint16_t hint, const std::string& name)
		: Hint(hint), Name(name), VirtualAddress(0)
	{
	}
};

struct ImportDll
{
	std::string Name;
	std::vector<ImportThunk> Thunks;
	PE::IMAGE_IMPORT_DESCRIPTOR Desc;

	ImportDll(const std::string& name, std::vector<ImportThunk> thunks)
		: Name(name), Thunks(thunks), Desc({ 0 })
	{
	}
};


static PE::PESectionSPtr AddNewImportsSection(PE::PEFile& pefile, PE::PESectionSPtr rsrc, std::vector<ImportDll>& addImportsList)
{
	// We don't want to be adding a section that already exists.
	assert(!pefile.ContainsSection(NEW_SECTION_NAME));

	// We MUST operate under the assumption that ".rsrc" is the last section.
	assert(pefile.IndexOfSection(rsrc) == (pefile.NumberOfSections() - 1));

	auto newIdata = pefile.CreateSection(NEW_SECTION_NAME);
	auto& header = newIdata->Header;

	// We're inserting ourselves before ".rsrc", so steal it's position information
	header.PointerToRawData = rsrc->Header.PointerToRawData;
	header.VirtualAddress = rsrc->Header.VirtualAddress;
	header.Characteristics = (IMAGE_SCN_CNT_CODE|IMAGE_SCN_MEM_EXECUTE|IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE);
	// size updates are handled by PESectionStream, so no need to set them here

	auto& importsDir = pefile.ImportDirectory();

	// We're expecting a valid size multiple of IMAGE_IMPORT_DESCRIPTOR.
	assert((importsDir.Size % sizeof(PE::IMAGE_IMPORT_DESCRIPTOR)) == 0);

	std::vector<PE::IMAGE_IMPORT_DESCRIPTOR> importsList;
	importsList.resize(importsDir.Size / sizeof(PE::IMAGE_IMPORT_DESCRIPTOR));

	//// We're expecting the importsDir to be in ".idata"
	//auto idata = pefile.GetSection(".idata");
	//assert(idata == pefile.GetSectionAtRVA(importsDir.VirtualAddress));

	//// This stream ensures the importsDir is in ".idata"
	PE::PESectionSPtr origIdata = pefile.GetSectionAtRVA(importsDir.VirtualAddress);
	PE::PESectionStream origStream = PE::PESectionStream(origIdata, false);
	origStream.SeekRVA(importsDir.VirtualAddress);
	origStream.Read(importsList.data(), importsDir.Size);



	PE::PESectionStream newStream = PE::PESectionStream(newIdata, true);


	// Skip import descriptors
	const uint32_t newImportsSize = (importsList.size() + addImportsList.size()) * sizeof(importsList[0]);
	newStream.SeekSection(newImportsSize);

	// Calculate the RVAs for our Original First Thunk lists
	for (auto& addImport : addImportsList) {
		addImport.Desc.OriginalFirstThunk = (rva32_t)newStream.TellRVA();
		//                                       +1 for empty entry to signify end of list
		size_t listSize = (addImport.Thunks.size() + 1) * sizeof(addImport.Thunks[0].VirtualAddress);
		newStream.Seek(listSize, SEEK_CUR);
	}

	// Calculate the RVAs for our First Thunk lists
	for (auto& addImport : addImportsList) {
		addImport.Desc.FirstThunk = (rva32_t)newStream.TellRVA();
		// (same calculation as above)
		//                                       +1 for empty entry to signify end of list
		size_t listSize = (addImport.Thunks.size() + 1) * sizeof(addImport.Thunks[0].VirtualAddress);
		newStream.Seek(listSize, SEEK_CUR);
	}


	// Now start actually writing the name data, no more skipping over.


	// Flattened list of thunk Virtual Addresses, with each import dll terminated by a zero-entry.
	std::vector<rva32_t> flatThunkAddrList;

	// Write all strings for our thunk names and dll names
	for (auto& addImport : addImportsList) {
		// Write all our dll thunk names, and update the RVA to that name
		for (auto& thunk : addImport.Thunks) {
			thunk.VirtualAddress = newStream.TellRVA(); // update the RVA where this name is written

			newStream.Write(&thunk.Hint, sizeof(thunk.Hint));
			newStream.WriteCString(thunk.Name);
			newStream.PutC(0); // +1 extra zero-byte after thunk name

			// Add this address to a flat list to simplify writing afterwards.
			flatThunkAddrList.push_back(thunk.VirtualAddress);
		}

		flatThunkAddrList.push_back(0); // add empty entry to signify end of list

		// After thunk names, write our dll name and update the RVA to that
		addImport.Desc.Name = (rva32_t)newStream.TellRVA();
		newStream.WriteCString(addImport.Name);
		newStream.PutC(0); // +1 extra zero-byte after dll name


		// Insert our now-complete import descriptor into the list, before last entry which is empty.
		importsList.insert(importsList.end() - 1, addImport.Desc);
	}

	// Go back and write the updated RVAs
	newStream.SeekSection(0);
	newStream.Write(importsList.data(), (importsList.size() * sizeof(importsList[0])));

	// Write our Original First Thunk lists
	newStream.Write(flatThunkAddrList.data(), (flatThunkAddrList.size() * sizeof(flatThunkAddrList[0])));

	// Write our First Thunk lists
	newStream.Write(flatThunkAddrList.data(), (flatThunkAddrList.size() * sizeof(flatThunkAddrList[0])));


	// Fill the remainder of the section with zeros, aligned to file
	newStream.SetLength(pefile.FileAlign(newStream.Length()));


	// Finally add our new section to the list in the PE,
	// then update the imports directory to point to it.
	pefile.InsertSectionAt(pefile.IndexOfSection(rsrc), newIdata);

	// Overwrite imports directory with our new one
	importsDir.VirtualAddress = header.VirtualAddress;
	importsDir.Size = newImportsSize; // +1 import (the size here is only the descriptors list, not the entire section)


	// Unsure if needed (left out for now)
	//pefile.OptionalHeader().SizeOfInitializedData += header.SizeOfRawData;


	_tprintf(_T("Imports updated @ 0x%08x\n"), header.VirtualAddress);

	return newIdata; // return our newly created section so we can tell how much the size has been increased by
}

#pragma endregion

#pragma region Patch WinMain call

// Patch in call to StartOpenLRR from WinMain
static void PatchWinMain(PE::PEFile& pefile, PE::PESectionSPtr text, rva32_t thunkAddr)
{
	uint8_t ASM_PUSHEBP[]  = { 0x55 };                                // PUSH EBP
	uint8_t ASM_MOVEBP[]   = { 0x8b,0xec };                           // MOV  EBP,ESP
	uint8_t ASM_PUSHARG[]  = { 0xff,0x74,0x24,0x14 };                 // PUSH dword ptr[ESP+0x14]
	uint8_t ASM_CALL[]     = { 0xff,0x15/*,0x00,0x00,0x00,0x00*/ };   // CALL dword ptr[indirectAddr]
	uint32_t indirectAddr  = (uint32_t)pefile.RVA2Address(thunkAddr); // (Address of function pointer)
	uint8_t ASM_ADDESP10[] = { 0x83,0xc4,0x10 };                      // ADD  ESP,0x10
	uint8_t ASM_POPEBP[]   = { 0x5d };                                // POP  EBP
	uint8_t ASM_RET10[]    = { 0xc2,0x10,0x00 };                      // RET  0x10


	PE::PESectionStream textStream = PE::PESectionStream(text, false);
	textStream.SeekRVA(pefile.Address2RVA(PROCESS_WINMAIN));


	textStream.Write(ASM_PUSHEBP,   sizeof(ASM_PUSHEBP));  // Preserve base stack pointer for parent function.
	textStream.Write(ASM_MOVEBP,    sizeof(ASM_MOVEBP));   // This is a critical step that causes some nasty
	                                                       //  unwanted side effects if skipped (see: Issue #27).
	for (uint32_t i = 0; i < 4; i++) {
		textStream.Write(ASM_PUSHARG, sizeof(ASM_PUSHARG));// Push the last 4 arguments onto the stack again.
	}
	textStream.Write(ASM_CALL,      sizeof(ASM_CALL));     // Call an indirect absolute address.
	textStream.Write(&indirectAddr, sizeof(indirectAddr)); // Address of CALL instruction.
	textStream.Write(ASM_ADDESP10,  sizeof(ASM_ADDESP10)); // Restore stack position for called __cdecl function.
	textStream.Write(ASM_POPEBP,    sizeof(ASM_POPEBP));   // Restore base stack pointer for parent function.
	textStream.Write(ASM_RET10,     sizeof(ASM_RET10));    // Restore stack position for this __stdcall function.

	_tprintf(_T("WinMain patched @ 0x%08x\n"), pefile.Address2RVA(PROCESS_WINMAIN));
}

#pragma endregion


/////////////////////////////////////////////////////////////////////
// Entry point

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	tstring inFilename, outFilename;
	bool showHelp = false, useDebug = false, replaceIcon = false;
	IconRes newIcon;
	PE::PEFile pefile;

	int argOff = 1;

	// Parse: [-h|--help] "Show help" (check all arguments, unlike other ordered arguments)
	for (int i = argOff; i < argc; i++) {
		if (tstring(argv[i]) == _T("-h") || tstring(argv[i]) == _T("--help")) {
			showHelp = true;
			break;
		}
	}

	// Parse: [-d|--debug] "Debug exe build" (consume before we check for enough arguments)
	if (!showHelp && argOff < argc && (tstring(argv[argOff]) == _T("-d") || tstring(argv[argOff]) == _T("--debug"))) {
		useDebug = true;
		argOff++;
	}

	// Do we have enough arguments? If not, show usage.
	if (showHelp || argc <= argOff) {
		_tprintf(_T("Create an OpenLRR executable from a LegoRR masterpiece executable.\n"));
		_tprintf(_T("usage: OpenLRR-MakeExe [-h] [-d] <LEGORREXE> [ICOFILE]\n"));
		if (showHelp) {
			_tprintf(_T("\n"));
			_tprintf(_T("arguments:\n"));
			_tprintf(_T("  -h,--help   Show this help message\n"));
			_tprintf(_T("  -d,--debug  Output a debug executable that imports a debug build of OpenLRR\n"));
			_tprintf(_T("              \"OpenLRR-d.exe\" and \"openlrr-d.dll\"\n"));
			_tprintf(_T("  LEGORREXE   Filepath to LegoRR masterpiece executable\n"));
			_tprintf(_T("  ICOFILE     Optional filepath to replacement icon\n"));
			_tprintf(_T("              Currently only supports same dimensions/bit-depth\n"));

			_tprintf(_T("\n"));
			_tprintf(_T("\n"));
			_tprintf(_T("Output file is written to working directory as \"OpenLRR.exe\" or \"OpenLRR-d.exe\".\n"));
		}
		return 1;
	}


	// Parse: <LEGORREXE> "Input Masterpiece LegoRR.exe file"
	inFilename = argv[argOff++];
	std::replace(inFilename.begin(), inFilename.end(), _T('\\'), _T('/'));

	// Output executable in current working directory.
	outFilename = (useDebug ? OUTEXE_NAME_DEBUG : OUTEXE_NAME_RELEASE); // "OpenLRR(-d).exe"

	// Optional: Get working directory for clarity on where file is being written to.
	TCHAR cwdBuff[MAX_PATH];
	if (_tgetcwd(cwdBuff, _countof(cwdBuff))) {
		size_t cwdLen = _tcsclen(cwdBuff);
		if (cwdLen && (cwdBuff[cwdLen - 1] == _T('\\') || cwdBuff[cwdLen - 1] == _T('/')))
			cwdBuff[cwdLen - 1] = _T('\0');

		outFilename = tstring(cwdBuff) + _T("/") + outFilename;
		std::replace(outFilename.begin(), outFilename.end(), _T('\\'), _T('/'));
	}


	// Parse: [ICOFILE] "Input Icon resource to replace (very finicky)"
	if (argOff < argc) {
		// Set to true on success
		tstring iconFilename = argv[argOff++];
		replaceIcon = ReadIconResFromFile(iconFilename, newIcon);
		if (!replaceIcon) {
			_tprintf(_T("Failed to read ICOFILE icon\n"));
			_tprintf(_T("%s\n"), iconFilename.c_str());
			return 1;
		}
	}



	// Begin reading PE file and performing modifications.

	if (!pefile.Load(inFilename)) {
		_tprintf(_T("Failed to read LEGORREXE executable: %hs\n"), pefile.ErrorMessage().c_str());
		_tprintf(_T("%s\n"), inFilename.c_str());
		return 1;
	}


	PE::PESectionSPtr rsrc = pefile.GetSection(".rsrc");
	PE::PESectionSPtr text = pefile.GetSection(".text");


	// Our list of new import dlls and function thunks to add (just 1)
	const std::string dllName = (useDebug ? DLL_NAME_DEBUG : DLL_NAME_RELEASE); // "openlrr(-d).dll"

	_tprintf(_T("Using %s import %hs\n"), (useDebug ? _T("Debug") : _T("Release")), dllName.c_str());

	std::vector<ImportDll> addImportsList {
		ImportDll(dllName, { // "openlrr.dll" (or "openlrr-d.dll" for DEBUG mode)
			ImportThunk(START_THUNK_HINT, START_THUNK_NAME), // "StartOpenLRR"
		}),
	};
	ImportDll& startDll = addImportsList[0];
	size_t startThunkIndex = 0;


	// Build a new section for imports, and insert it before ".rsrc".
	PE::PESectionSPtr newIdata = AddNewImportsSection(pefile, rsrc, addImportsList);


	// Get the address assigned for our "StartOpenLRR" thunk, so we can patch it into WinMain.
	rva32_t startThunkAddr = startDll.Desc.FirstThunk + (startThunkIndex * sizeof(rva32_t)); // thunk addr table[thunk index]

	// Patch WinMain function to call our "openlrr.dll" function "StartOpenLRR".
	PatchWinMain(pefile, text, startThunkAddr);


	// Shift resource directory/section by amount taken up by new idata section.
	// Also handles updating icon resource if non-null.
	UpdateRsrcSection(pefile, rsrc, newIdata->Header.SizeOfRawData,
					  (replaceIcon ? &newIcon : nullptr),
					  REPLACE_ICON_NAME, REPLACE_ICON_LANG);


	pefile.UpdateSizeOfImage();

	// Write changes to new file
	pefile.Save(outFilename);

	_tprintf(_T("Saved to %s\n"), outFilename.c_str());
	//::system("pause");

	return 0;
}
