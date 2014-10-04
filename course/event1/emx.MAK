include ..\emx.inc

#Object files/libraries to link
OBJS=event1.obj

#All files to build
all: event1.exe

#Rules to build the application
event1.exe: $(OBJS)
	$(LINK) event1.exe $(OBJS) $(LIBS)

#Dependencies
event1.obj: event1.cpp




