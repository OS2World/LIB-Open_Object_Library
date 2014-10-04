#include file for va-makefiles

LIBS=$(OOL)\LIB\OOLW3I10.LIB $(OOL)\LIB\OOLC3I10.LIB OS2386.LIB

#Default compiler switches
CC=icc.exe
DFLAGS= /C /Q

#Rules how to build the application
!ifdef debug
CFLAGS= /Ge+ /O- /Fb- /Gm+ /G5 /Ti /W3 /Gn+
LINK=icc.exe $(CFLAGS) /B" /De /e:2 /NOD"
!else
!ifdef beta
CFLAGS= /Ge+ /O+ /Fb- /Gm+ /G5 /W3 /Gn+
LINK=icc.exe $(CFLAGS) /B" /e:2 /NOD"
!else
CFLAGS= /Ge+ /O+ /Fb- /Gm+ /Gd+ /G5 /W3 /Gn+
LINK=icc.exe $(CFLAGS) /B"/NOD"
!endif
!endif

#Rules to build the object-files
.cpp.obj:
   $(CC) $(CFLAGS) $(DFLAGS) %s
.c.obj:
   $(CC) $(CFLAGS) $(DFLAGS) %s
