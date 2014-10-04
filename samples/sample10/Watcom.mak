#
# Makefile for OOL sample 7 - Watcom C/C++
#

!include ..\Common.inc

sample10.exe: sample10.obj
        @$(LD) name sample10.exe $(LDFLAGS) file sample10.obj

