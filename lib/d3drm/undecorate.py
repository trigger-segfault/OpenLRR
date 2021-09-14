#!/usr/bin/env python3
#-*- coding: utf-8 -*-
"""Patch a `.lib` file and change its symbol names from noprefix to undecorated.

WARNING: A LOT OF REALLY BAD/DUMB ASSUMPTIONS ARE BEING MADE FOR THIS PATCH,
         BUT ONLY BECAUSE IT WORKS FOR D3DRM.DLL.
"""

__version__ = '1.0.0'
__date__    = '2021-09-13'
__author__  = 'Robert Jordan'

__all__ = ['patch_file_undecorate', 'patch_data_undecorate', 'apply_patch_undecorate']

#######################################################################################

import re
import traceback
from collections import namedtuple
from struct import Struct
from typing import Optional


#######################################################################################

#region ## CONSTANTS ##

### ASSUMPTION: All functions names start with '_' (0x5f), and the first hint is 0
# search for the first 0x08|0x0c "no prefix"|"undecorate" type flag and function name start
SEARCH_FIRST  = re.compile(rb"\x00\x00[\x08\x0c]\x00_", re.DOTALL)
# search for all 0x08|0x0c "no prefix"|"undecorate" type flags and function name starts
SEARCH_DAT    = re.compile(rb"..[\x08\x0c]\x00_", re.DOTALL)

# Type: code   Name type: no prefix
FLAG_NO_PREFIX   = 0x08
# Type: code   Name type: undecorate
FLAG_UNDECORATE  = 0x0c


# for printing visual information only
LABEL_HINT       = "Hint"
LABEL_TYPE       = "Name type" #"Type"
LABEL_DLL        = "Dll"
LABEL_SYMBOL     = "Symbol"
LABEL_NO_PREFIX  = "no prefix"
LABEL_UNDECORATE = "undecorate"

#endregion

#######################################################################################

#region ## HELPERS ##

def cstring(buf:bytes, offset:int=0) -> bytes:
    idx = buf.index(b'\x00', offset)
    return bytes(buf[offset:idx])

# Structure: uint16 hint,  uint16 typeFlag
HintType = Struct('<HH')

# Structure: uint16 hint,  uint16 typeFlag,  cstring symbol,  cstring dll
class LibSignature(namedtuple('_LibSignature', ('offset', 'hint', 'typeFlag', 'symbol', 'dll'))):
    @classmethod
    def read(cls, buffer:bytes, offset:int=0) -> 'LibSignature':
        hint, typeFlag = HintType.unpack_from(buffer, offset)

        symbol = cstring(buffer, offset + HintType.size)
        dll    = cstring(buffer, offset + HintType.size + len(symbol)+1)

        return cls(offset, hint, typeFlag, symbol, dll)
    
    def patch_typeFlag(self, buffer:bytearray, typeFlag:int) -> None:
        HintType.pack_into(buffer, self.offset, self.hint, typeFlag)

#endregion

#######################################################################################

#region ## APPLY PATCH ##

def apply_patch(data:bytearray) -> None:
    if not isinstance(data, bytearray):
        raise TypeError(f'apply_patch() argument must be bytearray, not {data.__class__.__name__}')
    
    # keep track of length to ensure everything went correctly
    orig_len = len(data)
    
    # make sure we found our data,
    # and that there's only one appearance of the starting pattern we're searching for
    match = SEARCH_FIRST.search(data)
    assert(match is not None and len(SEARCH_FIRST.findall(data)) == 1)

    # find all lib functions starting at the first match or after
    offset = match.span()[0]
    matches = [m for m in SEARCH_DAT.finditer(data) if m.span()[0] >= offset]
    sigs = [LibSignature.read(data, m.span()[0]) for m in matches]


    # calulate some visual output info
    maxsym_len = max(len(LABEL_SYMBOL), max(len(s.symbol) for s in sigs))
    maxdll_len  = max(len(LABEL_DLL),  max(len(s.dll)  for s in sigs))
    maxhint = max(s.hint for s in sigs)
    maxtype = max(s.typeFlag for s in sigs)
    maxhint_len = max(len(LABEL_HINT), len(str(maxhint)))
    maxtype_len = max([len(LABEL_TYPE), len(LABEL_NO_PREFIX), len(LABEL_UNDECORATE), 4]) # len(str(maxtype)))

    hr = '-' * (maxhint_len + 2 + maxtype_len + 2 + maxdll_len + 2 + maxsym_len + 2)


    # modify the data and show the results as we go
    print(f' {LABEL_HINT.ljust(maxhint_len)}  {LABEL_TYPE.ljust(maxtype_len)}  {LABEL_DLL.ljust(maxdll_len)}  {LABEL_SYMBOL}')
    print(hr)

    changed = 0

    # print all found entries and show which ones were modified with a '*' prefix
    for sig in sigs:
        off, hint, typeFlag, symbol, dll = sig

        typeFlagStr = f'{typeFlag:02x}'.ljust(maxtype_len)
        if typeFlag == FLAG_NO_PREFIX:
            typeFlagStr = LABEL_NO_PREFIX
        elif typeFlag == FLAG_UNDECORATE:
            typeFlagStr = LABEL_UNDECORATE

        dochange = (typeFlag == FLAG_NO_PREFIX) # 0x08
        print(f'{"*" if dochange else " "}{str(hint).ljust(maxhint_len)}  {typeFlagStr.ljust(maxtype_len)}  {dll.decode().ljust(maxdll_len)}  {symbol.decode()}')
        
        if dochange:
            sig.patch_typeFlag(data, FLAG_UNDECORATE) # 0x0c
            changed += 1
    
    print(hr)
    print(f'Found: {len(matches)}, Changed: {changed}')


    # ensure the length of the file never changed
    assert(len(data) == orig_len)

    # done!
    
#endregion

#######################################################################################

#region ## PATCH FILE/DATA ##

def patch_data(data:bytes) -> bytearray:
    data = bytearray(data)
    apply_patch(data)
    return data

def patch_file(src_file:str, dst_file:Optional[str]=None) -> None:
    # overwrite src_file if dst_file was not specified
    if dst_file is None:
        dst_file = src_file
    
    # Read source file
    with open(src_file, 'rb') as f:
        data = f.read()
    
    data = patch_data(data)
    
    with open(dst_file, 'wb+') as f:
        f.write(data)
        f.flush()

#endregion

#######################################################################################

# assign import * names
apply_patch_sfxgroup = apply_patch
patch_data_sfxgroup = patch_data
patch_file_sfxgroup = patch_file

#######################################################################################

#region ## MAIN FUNCTION ##

def main(argv:Optional[list]=None) -> int:
    import os

    BACKUP_CONST = '_orig'

    #region ## PARSER SETUP ##
    import argparse
    parser = argparse.ArgumentParser(
        description='Patch a `.lib` file and change its symbol names from noprefix to undecorated.',
        add_help=True)
    
    parser.add_argument('input', metavar='INPUTLIB', help='original library to patch (*.lib)')
    oparser = parser.add_argument_group('output options')
    ogroup = oparser.add_mutually_exclusive_group(required=False)
    ogroup.add_argument('-o', '--output', metavar='OUTPUTEXE', default=None, help='write to new lib output file')
    ogroup.add_argument('-n', '--name', metavar='OUTPUTNAME', dest='rename', default=None, help='write to new lib name in same directory')
    ogroup.add_argument('-r', '--replace', action='store_true', default=False, help='overwrite original lib')
    ogroup.add_argument('-b', '--backup', action='store_true', default=False, help=f'overwrite original lib and create *{BACKUP_CONST}.lib backup')
    parser.add_argument('-q', '--quiet', action='store_true', default=False, help='don\'t print actions being performed')
    parser.add_argument('-T', '--test', action='store_true', default=False, help='test patch without saving any changes')

    #endregion

    ###########################################################################

    #region ## PARSER VALIDATION ##

    args = parser.parse_args(argv)

    input = args.input
    output = None
    backup = None
    if args.output is not None:
        output = args.output
    elif args.rename is not None:
        output = os.path.join(os.path.dirname(input), args.rename)
    elif args.replace or args.backup:
        output = input
        if args.backup:
            basepath, ext = os.path.splitext(input)
            backup = basepath + BACKUP_CONST + ext
    
    test = args.test
    if not test and not output:
        parser.error('destination lib file must be specified!')
    if not input:
        parser.error('input lib file path is empty!')
    if not os.path.isfile(input):
        parser.error('input lib file not found!')
    if output and os.path.isdir(output):
        parser.error('destination lib file cannot be directory!')
    
    verbose = args.test or not args.quiet

    #endregion

    ###########################################################################

    if verbose: print(f'Loading: {input}')
    with open(input, 'rb') as f:
        data = f.read()
    
    if backup:
        if verbose: print(f'Backup:  {backup}')
        if not test:
            with open(backup, 'wb+') as f:
                f.write(data)
                f.flush()

    if verbose: print('Applying patch...')
    try:
        data = patch_data(data)
    except AssertionError as ex:
        print('Erroring patching lib, unexpected data found!')
        traceback.print_exc()
        return 1

    if output:
        if verbose: print(f'Saving:  {output}')
        if not test:
            with open(output, 'wb+') as f:
                f.write(data)
                f.flush()

    if verbose: print('Done!')

    return 0

#endregion


###########################################################################

del Optional  # cleanup declaration-only imports


## MAIN CONDITION ##

if __name__ == '__main__':
    exit(main())
