# Makefile for OOL - IBM Visual C++
# build all samples

all: sample1 sample3 sample4 sample5 sample6 sample7 sample8 sample9 sample11 sample12 sample13 sample14 sample15 sample16

sample1:
        @echo .
        @echo Compiling sample1...
        @cd sample1
        @nmake -nologo -f va.mak
        @cd ..

sample3:
        @echo .
        @echo Compiling sample3...
        @cd sample3
        @nmake -nologo -f va.mak
        @cd ..

sample4:
        @echo .
        @echo Compiling sample4...
        @cd sample4
        @nmake -nologo -f va.mak
        @cd ..

sample5:
        @echo .
        @echo Compiling sample5...
        @cd sample5
        @nmake -nologo -f va.mak
        @cd ..

sample6:
        @echo .
        @echo Compiling sample6...
        @cd sample6
        @nmake -nologo -f va.mak
        @cd ..

sample7:
        @echo .
        @echo Compiling sample7...
        @cd sample7
        @nmake -nologo -f va.mak
        @cd ..

sample8:
        @echo .
        @echo Compiling sample8...
        @cd sample8
        @nmake -nologo -f va.mak
        @cd ..

sample9:
        @echo .
        @echo Compiling sample9...
        @cd sample9
        @nmake -nologo -f va.mak
        @cd ..

sample11:
        @echo .
        @echo Compiling sample11...
        @cd sample11
        @nmake -nologo -f va.mak
        @cd ..

sample12:
        @echo .
        @echo Compiling sample12...
        @cd sample12
        @nmake -nologo -f va.mak
        @cd ..

sample13:
        @echo .
        @echo Compiling sample13...
        @cd sample13
        @nmake -nologo -f va.mak
        @cd ..

sample14:
        @echo .
        @echo Compiling sample14...
        @cd sample14
        @nmake -nologo -f va.mak
        @cd ..

sample15:
        @echo .
        @echo Compiling sample15...
        @cd sample15
        @nmake -nologo -f va.mak
        @cd ..

sample16:
        @echo .
        @echo Compiling sample16...
        @cd sample16
        @nmake -nologo -f va.mak
        @cd ..

clean:
        @cd sample1
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL
        @cd ..\sample2
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL
        @cd ..\sample3
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL
        @cd ..\sample4
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL
        @cd ..\sample5
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL
        @cd ..\sample6
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL
        @cd ..\sample7
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL
        @cd ..\sample8
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL
        @cd ..\sample9
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL
        @cd ..\sample10
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL
        @cd ..\sample11
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL
        @cd ..\sample12
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL
        @cd ..\sample13
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL
        @cd ..\sample14
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.res del *.res>NUL
        @if exist *.exe del *.exe>NUL


