#
# Makefile for the OOL Course  (Watcom C/C++)
#

!include ..\Watcom.inc

hello1.exe: hello1.obj
        @$(LD) name hello1.exe $(LDFLAGS) file hello1.obj

