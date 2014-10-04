	include ..\emx.inc

OBJS=sample10.obj


all: sample10.exe


sample10.exe: $(OBJS) sample10.res
	$(LINK) sample10.exe $(OBJS) $(LIBS) sample10.def
	rc.exe sample10.res sample10.exe

sample10.obj: sample10.cpp\
	sample10.h

sample10.res: sample10.rc
	rc.exe -r sample10.rc
