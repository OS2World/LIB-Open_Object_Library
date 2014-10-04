!include ..\va.inc

#Object files/libraries to link
OBJS=sample5.obj

#All files to build
all: sample5.exe sample5.dll

#Rules to build the application
sample5.exe: $(OBJS)
   $(LINK) /Fe"sample5.exe" $(OBJS) $(LIBS) sample5.def

#Rules to build the DLL
sample5.dll: sample5.res empty.obj
   $(LINK) /Fe"sample5.dll" empty.obj dll.def
   rc sample5.res sample5.dll

#Rules to build the resource-files
sample5.res: sample5.rc
   rc -r sample5.rc


#Dependencies of the related files
sample5.obj: sample5.cpp\
   sample5.h

empty.obj: empty.c
