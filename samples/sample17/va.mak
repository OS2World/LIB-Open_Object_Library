!include ..\va.inc

OBJS=sample17.obj


#All files to build
all:  sample17.EXE sample17.hlp

sample17.EXE: $(OBJS) sample17.res
   $(LINK) /Fe"sample17.EXE" $(OBJS) $(LIBS) sample17.def
   rc sample17.res sample17.EXE


#Rules to build the resource-files
sample17.res: sample17.rc\
   resource.h\
   sample17.ico
   rc -r sample17.rc

#Rules to build the help-files
sample17.hlp: sample17.ipf
   ipfc sample17.ipf

#Dependencies
sample17.obj: sample17.cpp\
   sample17.h