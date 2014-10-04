include ..\emx.inc

#Object files/libraries to link
OBJS=hello2.obj

#All files to build
all: hello2.exe

#Rules to build the application
hello2.exe: $(OBJS)
	$(LINK) hello2.exe $(OBJS) $(LIBS)

#Dependencies
hello2.obj: hello2.cpp




