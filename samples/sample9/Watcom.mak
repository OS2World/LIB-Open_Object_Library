#
# Makefile for OOL sample 9 - Watcom C/C++
#

!include ..\Common.inc

all: server.exe client.exe

client.exe: client.obj
        @$(LD) name client.exe debug all format os2 flat option protmode, stack=116384, quiet libr OOLW3W10 file client.obj

server.exe: server.obj
        @$(LD) name server.exe debug all format os2 flat option protmode, stack=116384, quiet libr OOLW3W10 file server.obj

