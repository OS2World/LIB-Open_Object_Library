include ..\emx.inc

all: client.exe server.exe

client.exe: client.obj
	gcc -Zmt -Zcrtdll -Zomf -Zlinker /PM:VIO -o client.exe client.obj $(LIBS) $(OOL)\lib\OOLT3E10.LIB -lstdcpp

client.obj: client.cpp


server.exe: server.obj
	gcc -Zmt -Zcrtdll -Zomf -Zlinker /PM:VIO -o server.exe server.obj $(LIBS) $(OOL)\lib\OOLT3E10.LIB -lstdcpp

server.obj: server.cpp

