	include ..\emx.inc

OBJS=sample5.obj

all: sample5.exe sample5.dll


sample5.exe: $(OBJS)
	gcc -Zmtd -Zcrtdll -Zomf -fhandle-exceptions -o sample5.exe $(OBJS) $(LIBS) sample5.def


sample5.dll: sample5.rc sample5.dlg
	rc -r sample5.rc sample5.res
	link386 \emx\lib\res0, sample5.dll, nul, ,dll.def
	rc sample5.res sample5.dll


sample5.obj: sample5.cpp\
	sample5.h

