!include ..\va.inc

#Object files/libraries to link
OBJS=sound.obj


#All files to build
all:  sound.exe

#Rules to build the application
sound.exe: $(OBJS) sound.res
   $(LINK) /Fe"sound.exe" $(OBJS) $(LIBS) $(OOL)\LIB\OOLM3I10.LIB sound.def
   rc sound.res sound.exe

sound.res: sound.rc
   rc -r sound.rc

#Dependencies
sound.obj: sound.cpp