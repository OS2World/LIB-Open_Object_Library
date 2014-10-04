!include ..\va.inc

#Object files/libraries to link
OBJS=sample7.obj

#All files to build
all:  sample7.exe

#Rules to build the application
sample7.exe: $(OBJS)
   $(LINK) /Fe"sample7.exe" $(OBJS) $(LIBS) sample7.def

#Dependencies
sample7.obj: sample7.cpp\
   sample7.h

