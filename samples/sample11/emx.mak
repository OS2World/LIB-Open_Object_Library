	include ..\emx.inc

OBJS=sample11.obj

all: sample11.exe


sample11.exe: $(OBJS)
	$(LINK) sample11.exe $(OBJS) $(LIBS) $(OOL)\LIB\OOLJ3E10.lib sample11.def


sample11.obj: sample11.cpp\
	sample11.h

