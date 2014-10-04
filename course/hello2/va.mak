!include ..\va.inc

#Object files/libraries to link
OBJS=hello2.obj
all:  hello2.exe

#Rules to build the application
hello2.exe: $(OBJS)
   $(LINK) /Fe"hello2.exe" $(OBJS) $(LIBS) hello2.def

#Dependencies
hello2.obj: hello2.cpp

