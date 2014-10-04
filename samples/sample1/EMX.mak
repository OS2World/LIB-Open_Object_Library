	include ..\emx.inc

OBJS=sample1.obj


all: sample1.exe


sample1.exe: $(OBJS) sample1.res
	$(LINK) sample1.exe $(OBJS) $(LIBS) sample1.def
	rc.exe sample1.res sample1.exe

sample1.obj: sample1.cpp\
	sample1.h

sample1.res: sample1.rc
	rc.exe -r sample1.rc
