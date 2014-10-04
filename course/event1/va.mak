!include ..\va.inc

OBJS=event1.obj

#All files to build
all:  event1.exe

#Rules to build the application
event1.exe: $(OBJS)
   $(LINK) /Fe"event1.exe" $(OBJS) $(LIBS) event1.def

#Dependencies
event1.obj: event1.cpp

