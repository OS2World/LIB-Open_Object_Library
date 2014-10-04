	include ..\emx.inc

all: client.exe server.exe

client.exe: client.res client.obj
	gcc -Zmtd -Zcrtdll -Zomf -o client.exe client.obj $(LIBS) client.def
	rc client.res client.exe


client.res: client.rc
	rc -r client.rc

client.obj: client.cpp\
	server.h



server.exe: server.res server.obj
	gcc -Zmtd -Zcrtdll -Zomf -o server.exe server.obj $(LIBS) server.def
	rc server.res server.exe

server.res: server.rc
	rc -r server.rc

server.obj: server.cpp
