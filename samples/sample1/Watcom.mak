#
# Makefile for OOL sample 1 - Watcom C/C++
#

!include ..\Common.inc

sample1.exe: sample1.obj sample1.res
        @$(LD) name sample1.exe $(LDFLAGS) file sample1.obj
        @rc -n sample1.res sample1.exe

