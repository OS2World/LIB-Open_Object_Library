#
# Makefile for OOL sample 6 - Watcom C/C++
#

!include ..\Common.inc

sample6.exe: sample6.obj sample6.res
        @$(LD) name sample6.exe $(LDFLAGS) file sample6.obj
        @rc -n sample6.res sample6.exe

