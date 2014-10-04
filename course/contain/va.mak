!include ..\va.inc

OBJS=contain.obj

#All files to build
all:  contain.exe


#Rules to build the application
contain.exe: $(OBJS)
   $(LINK) /Fe"contain.exe" $(OBJS) $(LIBS) contain.def

#Dependencies
contain.obj: contain.cpp

