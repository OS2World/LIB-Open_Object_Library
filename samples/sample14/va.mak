!include ..\va.inc

#Object files/libraries to link
OBJS=sample14.obj


#All files to build
all:  sample14.exe

#Rules to build the application
sample14.exe: $(OBJS) sample14.res
   $(LINK) /Fe"sample14.exe" $(OBJS) $(LIBS) sample14.def
   rc sample14.res sample14.exe

#Rules to build the resource-files
sample14.res: sample14.rc\
   resource.h\
   sample14.dlg
   rc -r sample14.rc

#Dependencies
sample14.obj: sample14.cpp\
   sample14.h\
   sample14.h

