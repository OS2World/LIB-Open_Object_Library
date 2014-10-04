#
# Makefile for OOL sample 17 - Watcom C/C++
#

!include ..\Common.inc

sample17.exe: sample17.obj sample17.res
        @$(LD) name sample17.exe $(LDFLAGS) file sample17.obj
	rc sample17.res sample17.exe

