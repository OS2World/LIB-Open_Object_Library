#
# Makefile for OOL sample 12 - Watcom C/C++
#


sample12.exe: sample12.cpp
	wcl386 -d2 -xst -k128000 sample12.cpp OOLW3W10.lib

