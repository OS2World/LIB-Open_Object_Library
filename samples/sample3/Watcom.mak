#
# Makefile for OOL sample 3 - Watcom C/C++
#

!include ..\Common.inc

sample3.exe: sample3.obj sample3.res
        @$(LD) name sample3.exe $(LDFLAGS) file sample3.obj libr OOLM3W10
        @rc -n sample3.res sample3.exe

