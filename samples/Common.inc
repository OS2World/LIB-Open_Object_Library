#
# Common definitions for the OOL samples - Watcom C/C++
#

#
# compiler and linker settings
#
CC      = wpp386
LD      = wlink

CCDBGFLGS       = -bm -bt=os2 -d2 -5s -otexan -xst -zq
CCNODBGFLGS     = -bm -bt=os2 -5s -otexan -xst -zq
CCFLAGS         = $(CCDBGFLGS)

LDDBGFLGS       = debug all format os2 flat pm option symfile option protmode, stack=136000, quiet, int libr OOLW3W10
LDNODBGFLGS     = format os2 flat pm option protmode, stack=136000, quiet, int libr OOLW3W10
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




