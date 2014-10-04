!include ..\va.inc

#Object files/libraries to link
OBJS=sample2.obj

#All files to build
all:  sample2.exe


#Rules to build the application
sample2.exe: $(OBJS)
   $(LINK) /Fe"sample2.exe" $(OBJS) $(LIBS) OOL23I10.LIB sample2.def

#Dependencies
sample2.obj: sample2.cpp
