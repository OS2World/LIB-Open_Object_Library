	include ..\emx.inc


all: client.exe server.exe


client.exe: client.obj
	$(LINK) client.exe client.obj $(LIBS)

client.obj: client.cpp



server.exe: server.obj
	$(LINK) server.exe server.obj $(LIBS)

server.obj: server.cpp

