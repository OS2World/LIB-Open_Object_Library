!include ..\va.inc

#Object files/libraries to link
OBJS=event2.obj

#All files to build
all:  event2.exe

#Rules to build the application
event2.exe: $(OBJS)
   $(LINK) /Fe"event2.exe" $(OBJS) $(LIBS) event2.def

#Dependencies
event2.obj: event2.cpp

