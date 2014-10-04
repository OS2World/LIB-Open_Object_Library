!include ..\va.inc

OBJS=dialog.obj

#All files to build
all: dialog.exe

#Rules to build the application
dialog.exe: $(OBJS) dialog.res
   $(LINK) /Fe"dialog.exe" $(OBJS) $(LIBS) dialog.def
   rc dialog.res dialog.exe

#Rules to build the resource-files
dialog.res: dialog.rc\
   resource.h\
   dialog.dlg
   rc -r dialog.rc

#Dependencies
dialog.obj: dialog.cpp\
   dialog.h

