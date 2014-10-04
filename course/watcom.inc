#
# Common definitions for the OOL samples - Watcom C/C++
#

#
# compiler and linker settings
#
CC      = wpp386
LD      = wlink

CCDBGFLGS       = -bm -bt=os2 -d2 -4s -otexan -xs -zq
CCNODBGFLGS     = -bm -bt=os2 -4s -otexan -xs -zq
CCFLAGS         = $(CCDBGFLGS)

LDDBGFLGS       = debug all format os2 flat pm option symfile option protmode, stack=36000, quiet, int libr OOLW3W04
LDNODBGFLGS     = format os2 flat pm option protmode, stack=36000, quiet, int libr OOLW3W04
LDFLAGS         = $(LDDBGFLGS)


#
# how to make .obj and .res files
#
.cpp.obj:
        @echo Compiling $*.cpp
        @$(CC) $(CCFLAGS) $*

.rc.res:
        @echo Compiling $*.rc
        @rc -n -r $*




