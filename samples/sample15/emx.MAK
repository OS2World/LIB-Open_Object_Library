include ..\emx.inc

#Object files/libraries to link
OBJS=sample15.obj

#All files to build
all: sample15.exe

#Rules to build the application
sample15.exe: $(OBJS) sample15.res
	$(LINK) sample15.exe $(OBJS) $(LIBS) $(OOL)\lib\OOLM3E10.LIB
	rc sample15.res sample15.exe

#Dependencies
sample15.obj: sample15.cpp

#Rules to build the resource-files
sample15.res: sample15.rc\
	resource.h\

sample15.rc:
	rc -r sample15.rc

