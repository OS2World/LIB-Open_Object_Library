include ..\emx.inc

#Object files/libraries to link
OBJS=sample14.obj

#All files to build
all: sample14.exe

#Rules to build the application
sample14.exe: $(OBJS) sample14.res
	$(LINK) sample14.exe $(OBJS) $(LIBS) -Zlinker /PM:PM
	rc sample14.res sample14.exe

#Dependencies
sample14.obj: sample14.cpp

#Rules to build the resource-files
sample14.res: sample14.rc\
	resource.h\
	sample14.dlg
	rc -r sample14.rc

