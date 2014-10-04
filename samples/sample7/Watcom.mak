#
# Makefile for OOL sample 7 - Watcom C/C++
#

!include ..\Common.inc

sample7.exe: sample7.obj
        @$(LD) name sample7.exe $(LDFLAGS) file sample7.obj

