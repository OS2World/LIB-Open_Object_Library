Readme file for the OPEN OBJECT LIBRARY 1.0

1. What is the OPEN OBJECT LIBRARY?
===================================

The Open Object Library (OOL) is a C++ class library for Warp 3 or higher.
It contains classes for
- control-windows, menus, dialogs and other types of windows
- files, (named) pipes, DDE, sockets
- graphic primitives and printer-support
- semaphores and other useful things like interfaces to Java (TM), REXX and DIVE.

The OOL works with following compilers:
- IBM Visual Age C++ 3.0
- EMX/GCC 0.9b or higher
- Watcom C/C++ 10.0 (Currently we have some problems with Watcom 10.x so the
  librarys and DLLs for this compiler are not distributed, the makefiles for
  the compiler are part of the package so you are able to build these files
  yourself).


The OOL is copyrighted freeware.


2. Files of the OOL
===================

The OOL is shipped in four packages:
OOL_MAIN.ZIP   source, header-files, samples, course, documentation in
               INF-format
OOL_GLIB.ZIP   modules to rebuild the graphic import libarary of the OOL
OOL_EMX.ZIP    DLLs and import librarys for EMX 0.9b (or higher)
OOL_IBM.ZIP    DLLs and import librarys for Visual Age C++


3. How to install the OOL
=========================

To install the OOL you need a drive wich supports long filenames (if you don't
need the HTML-documentation you can install it on a FAT-drive).

Copy the zipped file to a place you want to and unzip the file:
>unzip ool.zip

For the first step you should read OOL.INF (directory OOL\DOC) where you find
detailed instructions how to work with the OOL.
