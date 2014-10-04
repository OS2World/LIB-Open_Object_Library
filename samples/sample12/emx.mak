	include ..\emx.inc

OBJS=sample12.obj

all:  sample12.exe


#Rules to build the application
sample12.exe: $(OBJS)
	$(LINK) sample12.exe $(OBJS) $(LIBS)

#Dependencies
sample12.obj: sample12.cpp

