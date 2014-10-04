#
# Makefile for OOL sample 2 - Watcom C/C++
#

!include ..\Common.inc

sample2.exe: sample2.obj 
        @$(LD) name sample2.exe $(LDFLAGS) file sample2.obj libr OOL23W10

