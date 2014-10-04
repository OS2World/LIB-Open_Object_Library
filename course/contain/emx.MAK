include ..\emx.inc

#Object files/libraries to link
OBJS=contain.obj

#All files to build
all: contain.exe

#Rules to build the application
contain.exe: $(OBJS)
	$(LINK) contain.exe $(OBJS) $(LIBS)

#Dependencies
contain.obj: contain.cpp




