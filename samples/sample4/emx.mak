	include ..\emx.inc

OBJS=sample4.obj

all: sample4.exe

sample4.exe: $(OBJS) sample4.res
	$(LINK) sample4.exe $(OBJS) $(LIBS) sample4.def
	rc sample4.res sample4.exe

sample4.res: sample4.rc
	rc -r sample4.rc

sample4.obj: sample4.cpp
