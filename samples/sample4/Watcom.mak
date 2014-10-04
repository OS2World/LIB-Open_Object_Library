#
# Makefile for OOL sample 4 - Watcom C/C++
#

!include ..\Common.inc

sample4.exe: sample4.obj sample4.res
        @$(LD) name sample4.exe $(LDFLAGS) file sample4.obj
        @rc -n sample4.res sample4.exe

