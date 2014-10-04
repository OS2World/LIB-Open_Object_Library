!include ..\va.inc

#Object files/libraries to link
OBJS=sample12.obj


#All files to build
all:  sample12.exe

#Rules to build the application
sample12.exe: $(OBJS)
   $(LINK) /Fe"sample12.exe" $(OBJS) $(LIBS)

#Dependencies
sample12.obj: sample12.cpp

