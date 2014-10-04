LIBS=$(OOL)\lib\OOLW3E10.LIB

CC=gcc.exe
DFLAGS=-Zomf -c -fhandle-exceptions

CFLAGS= -Zmt -Zcrtdll
LINK=gcc.exe -Zomf -Zcrtdll -Zlinker /PACKD -Zlinker /PACKC -Zlinker /PM:PM -o

%.obj: %.cpp
	$(CC) $(CFLAGS) $(DFLAGS) $<
