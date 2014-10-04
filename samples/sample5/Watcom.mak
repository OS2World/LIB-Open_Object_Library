#
# Makefile for OOL sample 5 - Watcom C/C++
#

!include ..\Common.inc

sample5.exe: sample5.obj sample5.dll
        @$(LD) name sample5.exe $(LDFLAGS) file sample5.obj

sample5.dll: sample5.res
        @echo making resource dll
        @wpp386 -bm -bd -bt=os2 -mf -4r -otexan -zq empty.cpp
        @wlink name sample5.dll format os2 lx dll option protmode, quiet file empty.obj
        @rc -n sample5.res sample5.dll

