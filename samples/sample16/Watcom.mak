#
# Makefile for OOL sample 16 - Watcom C/C++
#

all: client.exe server.exe

client.exe: client.cpp
	wcl386 -xst -d2 -k128000 client.cpp OOLW3W10.lib OOLT3W10.lib
	
server.exe: server.cpp
        wcl386 -xst -d2 -k128000 server.cpp OOLW3W10.lib OOLT3W10.lib


