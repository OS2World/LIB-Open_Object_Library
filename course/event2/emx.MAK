include ..\emx.inc

#Object files/libraries to link
OBJS=event2.obj

#All files to build
all: event2.exe

#Rules to build the application
event2.exe: $(OBJS)
	$(LINK) event2.exe $(OBJS) $(LIBS)

#Dependencies
event2.obj: event2.cpp




