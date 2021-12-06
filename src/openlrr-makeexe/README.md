# OpenLRR-MakeExe

Tool to generate `OpenLRR.exe` for distribution and attaching to the VS Debugger.


### Usage

```cmd
Create an OpenLRR executable from a LegoRR masterpiece executable.
usage: OpenLRR-MakeExe [-h] [-d] <LEGORREXE> [ICOFILE]

arguments:
  -h,--help   Show this help message
  -d,--debug  Output a debug executable that imports a debug build of OpenLRR
              "OpenLRR-d.exe" and "openlrr-d.dll"
  LEGORREXE   Filepath to LegoRR masterpiece executable
  ICOFILE     Optional filepath to replacement icon
              Currently only supports same dimensions/bit-depth


Output file is written to working directory as "OpenLRR.exe" or "OpenLRR-d.exe".
```


***

## Steps

The goal is to import the OpenLRR codebase contained in `openlrr.dll`, and replace the original `LegoRR.exe!WinMain` entrypoint with a call to `openlrr.dll!StartOpenLRR`.

1. Insert a new Import Directory PE section (`.idata2`) after `.idata` and before the final section `.rsrc`.
2. Append a new import for `openlrr.dll` and include the function `StartOpenLRR`.
3. Overwrite `LegoRR.exe!WinMain` with a call to `int __cdecl openlrr.dll!StartOpenLRR` that pushes the same 4 parameters, then cleans up the stack and returns (register `EBP` must be preserved).
4. Update the Import and Resource directory RVAs to point to their new location.
5. Update the address offsets within `.rsrc` to match their new location.
6. Replace the main icon resource with *"teal-OR"* and include any other desired resources.

**Note:** The reason `.idata2` isn't being appended after `.rsrc` is so that anyone can freely modify executable resources without potentially breaking our hook into `openlrr.dll!StartOpenLRR`. This ensures our import will be placed in a fixed position that won't unexpectedly change.


***

### PE fields of importance

#### Fields to read

* OPTIONAL HEADER: FileAlignment
* OPTIONAL HEADER: VirtualAlignment


#### Fields to modify

* FILE HEADER: NumberOfSections (+1 for `.idata2`)
* OPTIONAL HEADER: SizeOfInitializedData (+file size diff of `.rsrc`, round up to file alignment)
* OPTIONAL HEADER: SizeOfImage (+virtual size diff of `.rsrc`, and +virtual size of `.idata2`, round up to virtual alignment)
* DATA DIRECTORY\[1\] Import directory
    * VirtualAddress (change to address of `.idata2`)
    * Size (+0x14 for added dll import `openlrr.dll`)
* DATA DIRECTORY\[2\] Resource directory
    * VirtualAddress (+offset in address caused by `.idata2`)
    * Size (+file size diff of `.rscr`)

