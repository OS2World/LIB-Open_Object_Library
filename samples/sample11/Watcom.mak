#
# Makefile for OOL sample 7 - Watcom C/C++
#

!include ..\Common.inc

sample11.exe: sample11.obj
        @$(LD) name sample11.exe $(LDFLAGS) file sample11.obj libr OOLJ3W10

