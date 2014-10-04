!include ..\va.inc

#Object files/libraries to link
OBJS=notebook.obj


#All files to build
all:  notebook.exe

#Rules to build the application
notebook.exe: $(OBJS)
   $(LINK) /Fe"notebook.exe" $(OBJS) $(LIBS) notebook.def

#Dependencies
notebook.obj: notebook.cpp

