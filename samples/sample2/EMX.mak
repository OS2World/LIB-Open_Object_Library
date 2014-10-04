	include ..\emx.inc

OBJS=sample2.obj


all: sample2.exe


sample2.exe: $(OBJS)
	$(LINK) sample2.exe $(OBJS) $(LIBS) $(OOL)\lib\OOL23E10.LIB sample2.def


sample2.obj: sample2.cpp


sample2.res: sample2.rc

