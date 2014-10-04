include ..\emx.inc

#Object files/libraries to link
OBJS=notebook.obj

#All files to build
all: notebook.exe

#Rules to build the application
notebook.exe: $(OBJS)
	$(LINK) notebook.exe $(OBJS) $(LIBS)

#Dependencies
notebook.obj: notebook.cpp




