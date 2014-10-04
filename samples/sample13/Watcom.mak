#
# Makefile for OOL sample 13 - Watcom C/C++
#

!include ..\Common.inc

sample13.exe: sample13.obj
        @$(LD) name sample13.exe $(LDFLAGS) file sample13.obj libr OOLM3W10


