!include ..\va.inc

#Object files/libraries to link
OBJS=sample11.obj


#All files to build
all:  sample11.exe

#Rules to build the application
sample11.exe: $(OBJS)
   $(LINK) /Fe"sample11.exe" $(OBJS) $(LIBS) $(OOL)\LIB\OOLJ3I10.LIB sample11.def

#Dependencies
sample11.obj: sample11.cpp\
   sample11.h

