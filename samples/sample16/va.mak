#makefile sample16 vor VisualAge C++

!include ..\va.inc

all: client.exe server.exe

client.exe: client.obj
   $(LINK) /Fe"client.exe" client.obj $(LIBS) OOLT3I10.LIB

client.obj: client.cpp\
   c:\source\ool\include\xheaders.h\
   c:\emx\include\stdlib.h


server.exe: server.obj
   $(LINK) /Fe"server.exe" server.obj $(LIBS) OOLT3I10.LIB

server.obj: server.cpp