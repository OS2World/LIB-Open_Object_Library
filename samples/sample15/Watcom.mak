#
# Makefile for OOL sample 15 - Watcom C/C++
#

!include ..\Common.inc

sample15.exe: sample15.obj
        @$(LD) name sample15.exe $(LDFLAGS) file sample15.obj


