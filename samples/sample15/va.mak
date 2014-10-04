!include ..\va.inc

#Object files/libraries to link
OBJS=sample15.obj


#All files to build
all:  sample15.exe

#Rules to build the application
sample15.exe: $(OBJS) sample15.res
   $(LINK) /Fe"sample15.exe" $(OBJS) $(LIBS) sample15.def
   rc sample15.res sample15.exe

#Rules to build the resource-files
sample15.res: sample15.rc\
   resource.h\
   sample15.ico
   rc -r sample15.rc

#Dependencies
sample15.obj: sample15.cpp\
   sample15.h