	include ..\emx.inc

OBJS=sample3.obj

all: sample3.exe

sample3.exe: $(OBJS) sample3.res
	$(LINK) sample3.exe $(OBJS) $(LIBS) $(OOL)\LIB\OOLM3E10.lib sample3.def
	rc sample3.res sample3.exe

sample3.res: sample3.rc
	rc -r sample3.rc

sample3.obj: sample3.cpp\
	sample3.h

