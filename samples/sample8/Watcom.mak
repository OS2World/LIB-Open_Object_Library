#
# Makefile for OOL sample 8 - Watcom C/C++
#

!include ..\Common.inc

all: server.exe client.exe

client.exe: client.obj client.res
        @$(LD) name client.exe $(LDFLAGS) file client.obj
        @rc -n client.res client.exe

server.exe: server.obj server.res
        @$(LD) name server.exe $(LDFLAGS) file server.obj
        @rc -n server.res server.exe


