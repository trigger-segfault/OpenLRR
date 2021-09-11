# Generating D3DRM.lib

Although the `D3DRM.lib` file is already provided, instructions for how to re-generate it are included below.

In order to generate a usable `.lib` file for `D3DRM.dll`, you'll need the following:

* `D3DRM.dll` *(provided in `../bin/`)*
* Visual Studio Developer command prompt
* Hex editor

The primary issue is the common `undecorated` name type that you need to work with Microsoft DLLs. The `lib.exe` utility is not able to generate this name type. However, this can actually be solved by a quick hex edit for the flags byte that determines this name type (and other fields).

## Steps

1. The `.def` file was initially created using [this StackOverflow answer](https://stackoverflow.com/questions/9360280/how-to-make-a-lib-file-when-have-a-dll-file-and-a-header-file/16127548#16127548).
2. The `@#` postfixes for each method were copy-pasted off of another existing `d3drm.lib` file found on GitHub, however they can be determined by hand, based on the amount of bytes passed as arguments onto the stack. `Direct3DRMCreate` is currently the only function we need access to, and is such the only function we need to worry about getting right.
3. Use the command `lib /def:d3drm.def /machine:x86 /out:d3drm.lib` in the VS Dev Command Prompt to generate the initial `.lib` file.
4. Once generated, open up the file in a hex editor and navigate down to the final usage of the required function names: `_Direct3DRMCreate@4`.
5. Two bytes before the underscore `_` will be the byte `08`, which determines the name type and other flags. Change this value to `0C` to get an `undecorated` function name type. This has to be done for all functions you plan on using in the DLL.
6. That's all, no extra Visual C DLL project required! :D
