include ..\emx.inc

#Object files/libraries to link
OBJS=sound.obj

#All files to build
all: sound.exe

#Rules to build the application
sound.exe: $(OBJS) sound.res
	$(LINK) sound.exe $(OBJS) $(OOL)\LIB\oolm3e10.lib $(LIBS)
	rc sound.res sound.exe

#Dependencies
sound.obj: sound.cpp

sound.res: sound.rc
	rc -r sound.rc




