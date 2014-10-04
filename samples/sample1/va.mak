!include ..\va.inc

#Object files/libraries to link
OBJS=sample1.obj

#All files to build
all:  sample1.exe


#Rules to build the application
sample1.exe: $(OBJS) sample1.res
   $(LINK) /Fe"sample1.exe" $(OBJS) $(LIBS) sample1.def
   rc sample1.res sample1.exe

#Rules to build the resource-files
sample1.res: sample1.rc\
   resource.h\
   sample1.dlg\
   sample1.ico
   rc -r sample1.rc

#Dependencies
sample1.obj: sample1.cpp\
   sample1.h
