!include ..\va.inc

OBJS=hello1.obj

all:  hello1.exe

#Rules to build the application
hello1.exe: $(OBJS)
   $(LINK) /Fe"hello1.exe" $(OBJS) $(LIBS) hello1.def

#Dependencies
hello1.obj: hello1.cpp

