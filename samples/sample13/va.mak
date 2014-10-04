!include ..\va.inc

#Object files/libraries to link
OBJS=sample13.obj


#All files to build
all:  sample13


#Rules to build the application
sample13: $(OBJS)
   $(LINK) /Fe"sample13" $(OBJS) $(LIBS) sample13.def

#Dependencies
sample13.obj: sample13.cpp\
   sample13.h

