// openlrr-injector.cpp : Launcher program for running "LegoRR.exe" with the injected "openlrr.dll"
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <windows.h>
#include <string>


/////////////////////////////////////////////////////////////////////
// Defines

#define DLL_NAME _T("OpenLRR.dll")
#define EXE_NAME _T("LegoRR.exe")
//#define LRR_DIR _T("C:\\Program Files (x86)\\LEGO Media\\Games\\Rock Raiders")
// entrypoint for LegoRR.exe (masterpiece)
#define PROCESS_EIP 0x48F2C0

// (unused)
// WinMain function for LegoRR.exe (masterpiece)
//#define PROCESS_WINMAIN 0x477A60
// entrypoint instruction that calls WinMain for LegoRR.exe (masterpiece)
//#define PROCESS_WINMAIN_CALL 0x48F3FA

#define WAIT_TIME_MS 100


/////////////////////////////////////////////////////////////////////
// Macro functions

// (secondary macro needed for macro expansion)
#define nameof_(symbol) #symbol
#define nameof(symbol) nameof_(symbol)

#define tstrsizeof(s) ((_tcslen((s)) + 1) * sizeof(TCHAR))

// keep the console window open on error
#define return_error(r) ::system("pause"); return (r)


/////////////////////////////////////////////////////////////////////
// Typedefs

using tstring = std::basic_string<TCHAR>;


/////////////////////////////////////////////////////////////////////
// Utility functions

/*++
Source:   https://blogs.msdn.microsoft.com/twistylittlepassagesallalike/2011/04/23/everyone-quotes-command-line-arguments-the-wrong-way/
Archived: https://web.archive.org/web/20180811234026/https://blogs.msdn.microsoft.com/twistylittlepassagesallalike/2011/04/23/everyone-quotes-command-line-arguments-the-wrong-way/
Routine Description:
	This routine appends the given argument to a command line such
	that CommandLineToArgvW will return the argument string unchanged.
	Arguments in a command line should be separated by spaces; this
	function does not add these spaces.
Arguments:
	Argument - Supplies the argument to encode.
	CommandLine - Supplies the command line to which we append the encoded argument string.
	Force - Supplies an indication of whether we should quote
			the argument even if it does not contain any characters that would
			ordinarily require quoting.
--*/
static void ArgvQuote(const tstring& Argument, IN OUT tstring& CommandLine, bool Force = false) {
	// Unless we're told otherwise, don't quote unless we actually
	// need to do so --- hopefully avoid problems if programs won't
	// parse quotes properly

	// Separate arguments with a space.
	if (!CommandLine.empty())
		CommandLine.append(_T(" "));

	if (!Force && !Argument.empty() && Argument.find_first_of(_T(" \t\n\v\"")) == Argument.npos) {
		CommandLine.append(Argument);
	}
	else {
		CommandLine.push_back(_T('"'));

		for (auto It = Argument.begin(); ; ++It) {
			unsigned NumberBackslashes = 0;

			while (It != Argument.end() && *It == _T('\\')) {
				++It;
				++NumberBackslashes;
			}

			if (It == Argument.end()) {
				// Escape all backslashes, but let the terminating
				// double quotation mark we add below be interpreted
				// as a metacharacter.

				CommandLine.append(NumberBackslashes * 2, _T('\\'));
				break;
			}
			else if (*It == _T('"')) {
				// Escape all backslashes and the following
				// double quotation mark.

				CommandLine.append(NumberBackslashes * 2 + 1, _T('\\'));
				CommandLine.push_back(*It);
			}
			else {
				// Backslashes aren't special here.

				CommandLine.append(NumberBackslashes, _T('\\'));
				CommandLine.push_back(*It);
			}
		}

		CommandLine.push_back(_T('"'));
	}
}

int BuildFullPaths(const tstring& Exe, OUT tstring& FullPath, OUT tstring& WorkingDir)
{
	TCHAR cwdBuff[1024] = { 0 };

	//_tcscpy(currentDir, LRR_DIR);
	if (!_tgetcwd(cwdBuff, _countof(cwdBuff))) {
		_tprintf(_T("getcwd failed\n"));
		return -1;
	}

	// Strip unexpected trailing slashes.
	size_t cwdlen = _tcslen(cwdBuff);
	if (cwdlen > 0 && (cwdBuff[cwdlen - 1] == _T('\\') || cwdBuff[cwdlen - 1] == _T('/'))) {
		cwdBuff[cwdlen - 1] = _T('\0');
	}

	WorkingDir.assign(cwdBuff);
	FullPath.assign(WorkingDir).append(_T("\\")).append(Exe);

	return 0;
}

int BuildCommandLine(int argc, TCHAR* argv[], const tstring& FullPath, OUT tstring& CommandLine)
{
	CommandLine.clear();

	// Add all command line arguments, and escape/quote them if needed.
	ArgvQuote(FullPath, CommandLine, false);
	for (int i = 1; i < argc; i++) {
		ArgvQuote(argv[i], CommandLine, false);
	}

	return 0;
}

// Confirm we're using the expected LegoRR.exe masterpiece executable
int ConfirmEntryPoint(const tstring& FullPath, DWORD EntryPoint)
{
	IMAGE_DOS_HEADER dos;
	IMAGE_NT_HEADERS32 nt32;
	FILE* file;

	if (!(file = _tfopen(FullPath.c_str(), _T("rb")))) {
		_tprintf(_T("EXE fopen failed\n"));
		return -1; // return here, we don't need to close the file
	}

	int result = 0;
	if (!fread(&dos, sizeof(dos), 1, file)) {
		_tprintf(_T("IMAGE_DOS_HEADER fread failed\n"));
		result = -1;
	}
	else if (fseek(file, dos.e_lfanew, 0) != 0 || ftell(file) != dos.e_lfanew) {
		_tprintf(_T("IMAGE_DOS_HEADER.e_lfanew fseek failed\n"));
		result = -1;
	}
	else if (!fread(&nt32, sizeof(nt32), 1, file)) {
		_tprintf(_T("IMAGE_NT_HEADERS32 fread failed\n"));
		result = -1;
	}
	else {
		DWORD nt32EntryPoint = nt32.OptionalHeader.AddressOfEntryPoint + nt32.OptionalHeader.ImageBase;
		if (nt32EntryPoint != EntryPoint) {
			_tprintf(_T("nt32EntryPoint != EntryPoint\n"));
			_tprintf(_T("Found 0x%08x, but expected 0x%08x\n"), nt32EntryPoint, EntryPoint);
			result = -1;
		}
	}

	fclose(file);
	return result;
}


/////////////////////////////////////////////////////////////////////
// Entry point

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	PROCESS_INFORMATION ProcessInfo = { 0 }; // (init added)
	STARTUPINFO StartupInfo = { 0 };
	int r;
	
	_tprintf(_T("Target Program: %s EIP: 0x%x\n"), EXE_NAME, PROCESS_EIP);
	_tprintf(_T("Hook DLL: %s\n"), DLL_NAME);
	_tprintf(_T("\n"));


	tstring fullPath = _T("");
	tstring commandLine = _T("");
	tstring workingDir = _T("");

	// Get absolute paths for our exe, and the target workingDir (which is our current workingDir).
	if ((r = BuildFullPaths(EXE_NAME, fullPath, workingDir)) != 0) {
		return_error(r);
	}
	_tprintf(_T("fullPath=%s\n"), fullPath.c_str());
	_tprintf(_T("workingDir=%s\n"), workingDir.c_str());

	// We need to pass all arguments through OpenLRR.exe into LegoRR.exe, handle escaping here.
	// fullPath is added as the first argument.
	if ((r = BuildCommandLine(argc, argv, fullPath, commandLine)) != 0) {
		return_error(r);
	}
	_tprintf(_T("commandLine=%s\n"), commandLine.c_str());

	// Check just in-case the user isn't running the LegoRR.exe masterpiece executable
	if ((r = ConfirmEntryPoint(fullPath, PROCESS_EIP)) != 0) {
		_tprintf(_T("EXE check failed\n The %s Masterpiece executable is required."), EXE_NAME);
		return_error(r);
	}

	// Allocate this because CreateProcess expects a non-const lpCommandLine argument,
	// Let's play it safe and assume this isn't just a joke.
	LPTSTR lpCommandLine = new TCHAR[commandLine.length() + 1];
	_tcscpy(lpCommandLine, commandLine.c_str());

	StartupInfo.cb = sizeof(StartupInfo);

	if (!::CreateProcess(fullPath.c_str(), lpCommandLine, NULL, NULL, FALSE,
		CREATE_SUSPENDED, NULL, workingDir.c_str(), &StartupInfo, &ProcessInfo))
	{
		_tprintf(_T("CreateProcess failed\n"));
		return_error(-1);
	}
	delete[] lpCommandLine;


	_tprintf(_T("CREATE_SUSPENDED %s OK\n"), EXE_NAME);

	BYTE eipBackup[2] = { 0 };

	if (!::ReadProcessMemory(ProcessInfo.hProcess, (LPVOID)PROCESS_EIP, &eipBackup, sizeof(eipBackup), NULL)) {
		_tprintf(_T("EIP Backup failed\n"));
		return_error(-1);
	}

	_tprintf(_T("EIP Backup OK\n"));

	static const constexpr BYTE eipPatch[2] = { 0xEB, 0xFE };    // infinite jmp to itself

	if (!::WriteProcessMemory(ProcessInfo.hProcess, (LPVOID)PROCESS_EIP, &eipPatch, sizeof(eipPatch), NULL)) {
		_tprintf(_T("EIP Patch failed\n"));
		return_error(-1);
	}

	_tprintf(_T("EIP Patch OK\n"));

	if (::ResumeThread(ProcessInfo.hThread) == (DWORD)-1) {
		_tprintf(_T("ResumeThread failed\n"));
		return_error(-1);
	}

	_tprintf(_T("ResumeThread OK\n"));

	_tprintf(_T("Waiting %dms for program... "), WAIT_TIME_MS);
	::Sleep(WAIT_TIME_MS); // wait for program to get stuck at eip loop
	_tprintf(_T("OK\n"));

	const auto addrLoadLibrary = (LPTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("kernel32.dll")), nameof(LoadLibrary));
	if (!addrLoadLibrary) {
		_tprintf(_T("GetProcAddress(kernel32.dll, %s) failed\n"), _T(nameof(LoadLibrary)));
		return_error(-1);
	}

	_tprintf(_T("kernel32.dll!%s @ 0x%p\n"), _T(nameof(LoadLibrary)), addrLoadLibrary);

	static constexpr LPCTSTR dllPathBuf = DLL_NAME;

	const LPVOID vAllocMem = ::VirtualAllocEx(ProcessInfo.hProcess, NULL, tstrsizeof(dllPathBuf),
												MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!vAllocMem) {
		_tprintf(_T("VirtualAllocEx failed\n"));
		return_error(-1);
	}

	_tprintf(_T("VirtualAllocEx OK\n"));


	if (!::WriteProcessMemory(ProcessInfo.hProcess, vAllocMem, dllPathBuf, tstrsizeof(dllPathBuf), NULL)) {
		_tprintf(_T("WriteProcessMemory failed\n"));
		return_error(-1);
	}

	_tprintf(_T("WriteProcessMemory OK\n"));

	const HANDLE hInjectedThread = ::CreateRemoteThread(ProcessInfo.hProcess, NULL, 0, addrLoadLibrary, vAllocMem, NULL, NULL);
	if (!hInjectedThread) {
		_tprintf(_T("CreateRemoteThread failed\n"));
		return_error(-1);
	}

	_tprintf(_T("CreateRemoteThread OK\n"));

	// (commented out for now)
	//_tprintf(_T("\nInjection Success!\nAttach any debuggers now\n"));
	//::system("pause");
	_tprintf(_T("Restoring EIP... "));

	if (!::WriteProcessMemory(ProcessInfo.hProcess, (LPVOID)PROCESS_EIP, &eipBackup, sizeof(eipBackup), NULL)) {
		_tprintf(_T("Failed\n"));
	}

	_tprintf(_T("OK\n"));

	::system("pause"); // optional pause at end to check log output (will not pause startup of OpenLRR)

	return 0;
}
