include ..\emx.inc

#Object files/libraries to link
OBJS=hello1.obj

#All files to build
all: hello1.exe

#Rules to build the application
hello1.exe: $(OBJS)
	$(LINK) hello1.exe $(OBJS) $(LIBS)

#Dependencies
hello1.obj: hello1.cpp




