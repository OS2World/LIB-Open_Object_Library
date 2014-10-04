#
# Makefile for the OOL Course  (Watcom C/C++)
#

!include ..\Watcom.inc

menu1.exe: menu1.obj menu1.res
        @$(LD) name menu1.exe $(LDFLAGS) file menu1.obj
        @rc -n menu1.res menu1.exe

