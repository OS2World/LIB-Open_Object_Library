#
# Makefile for OOL sample 14 - Watcom C/C++
#

!include ..\Common.inc

sample14.exe: sample14.obj
        @$(LD) name sample14.exe $(LDFLAGS) file sample14.obj


