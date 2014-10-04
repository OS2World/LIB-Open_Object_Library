include ..\emx.inc

#Object files/libraries to link
OBJS=menu1.obj

#All files to build
all: menu1.exe

#Rules to build the application
menu1.exe: $(OBJS) menu1.res
	$(LINK) menu1.exe $(OBJS) $(LIBS)
	rc menu1.res menu1.exe

#Dependencies
menu1.obj: menu1.cpp

menu1.res: menu1.rc
	rc -r menu1.rc




