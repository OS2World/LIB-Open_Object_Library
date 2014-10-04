!include ..\va.inc

#Object files/libraries to link
OBJS=sample10.obj

#All files to build
all:  sample10.exe

#Rules to build the application
sample10.exe: $(OBJS) sample10.res
   $(LINK) /Fe"sample10.exe" $(OBJS) $(LIBS) sample10.def
   rc.exe sample10.res sample10.exe

#Dependencies
sample10.obj: sample10.cpp\
   sample10.h

sample10.res: sample10.rc
   rc.exe -r sample10.rc

