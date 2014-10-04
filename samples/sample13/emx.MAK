	include ..\emx.inc

OBJS=sample13.obj
#LIBS=$(OOL)\LIB\OOLW3E03.LIB $(OOL)\LIB\OOLM3E03.LIB c:\emx\lib\mm2emx.lib c:\emx\lib\oolimp.lib

#All files to build
all: sample13.exe


#Rules to build the application
sample13.exe: $(OBJS)
	$(LINK) sample13.exe $(OBJS) $(LIBS) $(OOL)\LIB\OOLM3E10.LIB sample13.def

#Dependencies of the related files
sample13.obj: sample13.cpp\
	sample13.h
