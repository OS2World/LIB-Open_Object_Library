#
# Makefile for OOL - Watcom C/C++
#

all:
        @cls
        @echo Compiling OOL...
        @cd source
        @nmake -nologo -f Watcom.mak
        @cd ..
        @echo OOL compiled successfully.
        @echo .
        @echo Compiling samples...
        @cd samples
        @nmake -nologo -f Watcom.mak
        @cd ..

clean:
        @cls
        @echo cleaning up...
        @cd source
        @nmake -nologo -f Watcom.mak clean
        @cd ..
        @cd samples
        @nmake -nologo -f Watcom.mak clean
        @cd ..
        @echo Directories are clean now.
