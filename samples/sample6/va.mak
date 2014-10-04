!include ..\va.inc

#Object files/libraries to link
OBJS=sample6.obj


#All files to build
all:  sample6.exe


#Rules to build the application
sample6.exe: $(OBJS) sample6.res
   $(LINK) /Fe"sample6.exe" $(OBJS) $(LIBS) sample6.def
   rc sample6.res sample6.exe

#Rules to build the resource-files
sample6.res: sample6.rc\
   resource.h
   rc -r sample6.rc

#Dependencies
sample6.obj: sample6.cpp\
   sample6.h

