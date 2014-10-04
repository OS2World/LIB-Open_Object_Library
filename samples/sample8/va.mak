!include ..\va.inc

all: client.exe server.exe


client.exe: client.obj client.res
   $(LINK) /Fe"client.exe" client.obj $(LIBS) client.def
   rc client.res client.exe

client.res: client.rc
   rc -r client.rc

client.obj: client.cpp\
   client.h

server.exe: server.obj server.res
   $(LINK) /Fe"server.exe" server.obj $(LIBS) server.def
   rc server.res server.exe

server.res: server.rc
   rc -r server.rc

server.obj: server.cpp\
   server.h


