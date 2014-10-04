#
# Makefile for the OOL Course  (Watcom C/C++)
#

!include ..\Watcom.inc

hello2.exe: hello2.obj
        @$(LD) name hello2.exe $(LDFLAGS) file hello2.obj

