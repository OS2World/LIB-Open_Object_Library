	include ..\emx.inc

OBJS=sample6.obj

all: sample6.exe

sample6.exe: $(OBJS) sample6.res
	$(LINK) sample6.exe $(OBJS) $(LIBS) sample6.def
	rc sample6.res sample6.exe


sample6.res: sample6.rc
	rc -r sample6.rc

sample6.obj: sample6.cpp\
	sample6.h

