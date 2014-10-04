include ..\emx.inc

#Object files/libraries to link
OBJS=dialog.obj

#All files to build
all: dialog.exe

#Rules to build the application
dialog.exe: $(OBJS) dialog.res
	$(LINK) dialog.exe $(OBJS) $(LIBS)
	rc dialog.res dialog.exe

#Dependencies
dialog.obj: dialog.cpp

dialog.res: dialog.rc
	rc -r dialog.rc




