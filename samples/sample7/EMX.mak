	include ..\emx.inc

OBJS=sample7.obj

all: sample7.exe

sample7.exe: $(OBJS)
	$(LINK) sample7.exe $(OBJS) $(LIBS) sample7.def


sample7.obj: sample7.cpp\
	sample7.h

