#
# Makefile for OOL - Watcom C/C++
#

#
# Directories
#
OOLLIB     = $(OOL)\lib
OOLINC     = $(OOL)\include
OOLSRC     = $(OOL)\source

#
# Compiler and linker definitions
#
CC              = wpp386
LD              = wlink

CCDBGFLGS       = -bd -bm -bt=os2 -d2 -5s -otexan -xst -zq
CCNODBGFLGS     = -bd -bm -bt=os2 -5s -otexan -xst -zq
CCFLAGS         = $(CCDBGFLGS)

OBJFILE         = watcom.lnk
LDDBGFLGS       = debug all format os2 flat dll initinstance terminstance option protmode, many, stack=128000, quiet, int @$(OBJFILE)
LDNODBGFLGS     = format os2 flat dll initi termi option protmode, many, stack=128000, quiet, int @$(OBJFILE)
LDFLAGS         = $(LDDBGFLGS)

OBJ_MAIN        = xapp.obj xbookpg.obj xbutton.obj xchart.obj xcons.obj xcolor.obj xcircsld.obj xclipbrd.obj xcont.obj xcontain.obj xcontrol.obj xdde.obj xdevice.obj xdialog.obj xentry.obj xevent.obj xesem.obj xexcept.obj xfile.obj xfileinf.obj xfiledlg.obj xfilefi.obj xfontdlg.obj xframe.obj xfrmcnt.obj xfrmwnd.obj xglibdlg.obj xgraphdv.obj xgraphob.obj xhandler.obj xhelp.obj xlayer.obj xlistbox.obj xmdi.obj xmenu.obj xmessage.obj xmle.obj xmsgbox.obj xmusem.obj xnotebk.obj xnpipe.obj xpipe.obj xprofile.obj xprint.obj xrexx.obj xreslib.obj xscrlbar.obj xscrlwnd.obj xslider.obj xspinbtn.obj xstatic.obj xstring.obj xthread.obj xtime.obj xtoolbar.obj xvaluset.obj xuserbtn.obj xuserwnd.obj xwindow.obj xwindrag.obj xwizard.obj xmemchk.obj
LOBJ_MAIN       = xapp.obj,xbookpg.obj,xbutton.obj,xchart.obj,xcons.obj,xcolor.obj,xcircsld.obj,xclipbrd.obj,xcont.obj,xcontain.obj,xcontrol.obj,xdde.obj,xdevice.obj,xdialog.obj,xentry.obj,xevent.obj,xesem.obj,xexcept.obj,xfile.obj,xfileinf.obj,xfiledlg.obj,xfilefi.obj,xfontdlg.obj,xframe.obj,xfrmcnt.obj,xfrmwnd.obj,xglibdlg.obj,xgraphdv.obj,xgraphob.obj,xhandler.obj,xhelp.obj,xlayer.obj,xlistbox.obj,xmdi.obj,xmenu.obj,xmessage.obj,xmle.obj,xmsgbox.obj,xmusem.obj,xnotebk.obj,xnpipe.obj,xpipe.obj,xprofile.obj,xprint.obj,xrexx.obj,xreslib.obj,xscrlbar.obj,xscrlwnd.obj,xslider.obj,xspinbtn.obj,xstatic.obj,xstring.obj,xthread.obj,xtime.obj,xtoolbar.obj,xvaluset.obj,xuserbtn.obj,xuserwnd.obj,xwindow.obj,xwindrag.obj,xwizard.obj,xmemchk.obj
OBJ_MEDIA       = xmedia.obj xgraphbt.obj xvirtscr.obj xbltwnd.obj xpal.obj xblitter.obj
LOBJ_MEDIA      = xmedia.obj,xgraphbt.obj,xvirtscr.obj,xbltwnd.obj,xpal.obj,xblitter.obj
#OBJ_MEDIA       = xmedia.obj xgraphbt.obj
#LOBJ_MEDIA      = xmedia.obj,xgraphbt.obj
OBJ_TCP         = xsocket.obj
LOBJ_TCP        = xsocket.obj
OBJ_DBASE       = xdbase.obj
LOBJ_DBASE      = xdbase.obj
OBJ_JAVA        = xjavaenv.obj xjvcmsgh.obj xjavaapp.obj xjvsmsgh.obj xjvsurlh.obj
LOBJ_JAVA       = xjavaenv.obj, xjvcmsgh.obj, xjavaapp.obj, xjvsmsgh.obj, xjvsurlh.obj
OBJ_CRYPT	= xcrypt.obj crypt.obj crypt_util.obj md5.obj
LOBJ_CRYPT	= xcrypt.obj,crypt.obj,crypt_util.obj,md5.obj

#
# how to make object files
#
.cpp.obj:
        @echo Compiling $*.cpp
        @$(CC) $(CCFLAGS) $*

.c.obj:
        @echo Compiling $*.c
        @$(CC) $(CCFLAGS) $*

#
# make the libraries
#
all: OOLW3W10.dll OOLM3W10.dll OOLT3W10.dll OOLJ3W10.dll OOL23W10.dll OOLY3W10.dll OOLRES.DLL copydlls

OOLRES.RES: $(OOL)\resource\oolres.rc
        @echo Compiling OOL resource code
	@rc -r $(OOL)\resource\oolres.rc

OOLRES.DLL: OOLRES.RES empty.obj
        @echo Linking OOL resource library
        @$(LD) name OOLRES.DLL file empty.obj format os2 flat dll initinstance terminstance option protmode, many, stack=36000, quiet
        @rc $(OOL)\resource\oolres.res OOLRES.DLL
        @copy OOLRES.dll $(OOLLIB)>NUL

OOLW3W10.dll: $(OBJ_MAIN)
        @echo linking OOLW3W10.DLL
        @echo segment type data nonshared readwrite loadoncall >$(OBJFILE)
        @echo segment type code nonshared loadoncall >>$(OBJFILE)
        @echo file $(LOBJ_MAIN)>>$(OBJFILE)
        @$(LD) name OOLW3W10.dll import WinMessageBox2 PMWIN.1015 $(LDFLAGS) libr rxstring.lib libr rexx.lib
        @wlib -b -n -q OOLW3W10.lib +OOLW3W10.dll
        @copy OOLW3W10.lib $(OOLLIB)>NUL
        @copy OOLW3W10.dll $(OOLLIB)>NUL
        @del $(OBJFILE)

OOLM3W10.dll: $(OBJ_MEDIA)
        @echo linking OOLM3W10.DLL
        @echo segment type data nonshared readwrite loadoncall >$(OBJFILE)
        @echo segment type code nonshared loadoncall >>$(OBJFILE)
        @echo file $(LOBJ_MEDIA)>>$(OBJFILE)
        @$(LD) name OOLM3W10.dll $(LDFLAGS) libr OOLW3W10.lib libr mmpm2.lib
        @wlib -b -n -q OOLM3W10.lib +OOLM3W10.dll
        @copy OOLM3W10.lib $(OOLLIB)>NUL
        @copy OOLM3W10.dll $(OOLLIB)>NUL
        @del $(OBJFILE)

OOLT3W10.dll: $(OBJ_TCP)
        @echo linking OOLT3W10.DLL
        @echo segment type data nonshared readwrite loadoncall >$(OBJFILE)
        @echo segment type code nonshared loadoncall >>$(OBJFILE)
        @echo file $(LOBJ_TCP)>>$(OBJFILE)
        @$(LD) name OOLT3W10.dll $(LDFLAGS) libr OOLW3W10 libr so32dll libr tcp32dll
        @wlib -b -n -q OOLT3W10.lib +OOLT3W10.dll
        @copy OOLT3W10.lib $(OOLLIB)>NUL
        @copy OOLT3W10.dll $(OOLLIB)>NUL
        @del $(OBJFILE)

OOLJ3W10.dll: $(OBJ_JAVA)
        @echo linking OOLJ3W10.DLL
        @echo segment type data nonshared readwrite loadoncall >$(OBJFILE)
        @echo segment type code nonshared loadoncall >>$(OBJFILE)
        @echo file $(LOBJ_JAVA)>>$(OBJFILE)
        @$(LD) name OOLJ3W10.dll $(LDFLAGS) libr OOLW3W10 libr jempo10
        @wlib -b -n -q OOLJ3W10.lib +OOLJ3W10.dll
        @copy OOLJ3W10.lib $(OOLLIB)>NUL
        @copy OOLJ3W10.dll $(OOLLIB)>NUL
        @del $(OBJFILE)

OOL23W10.dll: $(OBJ_DBASE)
        @echo linking OOL23W10.DLL
        @echo segment type data nonshared readwrite loadoncall >$(OBJFILE)
        @echo segment type code nonshared loadoncall >>$(OBJFILE)
        @echo file $(LOBJ_DBASE)>>$(OBJFILE)
        @$(LD) name OOL23W10.dll $(LDFLAGS) libr OOLW3W10
        @wlib -b -n -q OOL23W10.lib +OOL23W10.dll
        @copy OOL23W10.lib $(OOLLIB)>NUL
        @copy OOL23W10.dll $(OOLLIB)>NUL
        @del $(OBJFILE)

OOLY3W10.dll: $(OBJ_CRYPT)
        @echo linking OOLY3W10.DLL
        @echo segment type data nonshared readwrite loadoncall >$(OBJFILE)
        @echo segment type code nonshared loadoncall >>$(OBJFILE)
        @echo file $(LOBJ_DBASE)>>$(OBJFILE)
        @$(LD) name OOLY3W10.dll $(LDFLAGS) libr OOLW3W10
        @wlib -b -n -q OOLY3W10.lib +OOLY3W10.dll
        @copy OOLY3W10.lib $(OOLLIB)>NUL
        @copy OOLY3W10.dll $(OOLLIB)>NUL
        @del $(OBJFILE)



#
# uncomment the line below, in order to copy the dll files to \os2\dll
#
copydlls:
#        @copy *.dll f:\os2\dll>NUL

clean:
        @if exist *.obj del *.obj>NUL
        @if exist *.err del *.err>NUL
        @if exist *.dll del *.dll>NUL
        @if exist *.lib del *.lib>NUL



#
# dependencies
#
empty.obj: empty.cpp
xwizard.obj: xwizard.cpp
xcons.obj: xcons.cpp
xmemchk.obj: xmemchk.cpp


xapp.obj: xapp.cpp\
   $(OOL)\include\xapp.h\
   $(OOL)\include\xreslib.h\
   $(OOL)\include\xrect.h\
   $(OOL)\include\xframe.h\
   $(OOL)\include\xfrmwnd.h\
   $(OOL)\include\xstring.h\
   $(OOL)\include\xmsgbox.h\
   $(OOL)\include\xcountry.h

xbookpg.obj: xbookpg.cpp\
   $(OOL)\include\XNoteBk.h\
   $(OOL)\include\XBookPg.h\
   $(OOL)\include\xcntevnt.h\
   $(OOL)\include\xcolor.h\
   $(OOL)\include\xexcept.h\
   $(OOL)\include\xres.h\
   $(OOL)\include\xreslib.h\
   $(OOL)\include\xbitmap.h

xbutton.obj: xbutton.cpp\
   $(OOL)\include\xcheckbx.h\
   $(OOL)\include\xpushbtn.h\
   $(OOL)\include\xradio.h\
   $(OOL)\include\xuserbtn.h\
   $(OOL)\include\xstring.h

xchart.obj: xchart.cpp\
   $(OOL)\include\XPie.h\
   $(OOL)\include\XColor.h\
   $(OOL)\include\xrect.h\
   $(OOL)\include\xmsgbox.h\
   $(OOL)\include\xstring.h\
   $(OOL)\include\xbar.h

xcircsld.obj: xcircsld.cpp\
   $(OOL)\include\xcircsld.h

xclipbrd.obj: xclipbrd.cpp\
   $(OOL)\include\XClipBrd.h\
   $(OOL)\include\XBitmap.h\
   $(OOL)\include\XString.h

xcolor.obj: xcolor.cpp\
   $(OOL)\include\xcolor.h

xcont.obj: xcont.cpp\
   $(OOL)\include\XContInf.h\
   $(OOL)\include\XContObj.h\
   $(OOL)\include\XString.h\
   $(OOL)\include\XContain.h\
   $(OOL)\include\XContCol.h\
   $(OOL)\include\XDate.h\
   $(OOL)\include\XTime.h\
   $(OOL)\include\XBitmap.h\
   $(OOL)\include\XIcon.h

xcontain.obj: xcontain.cpp\
   $(OOL)\include\XContain.h\
   $(OOL)\include\XContInf.h\
   $(OOL)\include\XContObj.h\
   $(OOL)\include\XControl.h\
   $(OOL)\include\XString.h\
   $(OOL)\include\XMsgBox.h\
   $(OOL)\include\XContCol.h

xcontrol.obj: xcontrol.cpp\
   $(OOL)\include\XControl.h\
   $(OOL)\include\XRect.h\
   $(OOL)\include\XPushBtn.h\
   $(OOL)\include\XDragHdl.h\
   $(OOL)\include\XDragEvn.h\
   $(OOL)\include\XMousHdl.h\
   $(OOL)\include\XMousEvn.h\
   $(OOL)\include\XFrame.h\
   $(OOL)\include\XNoteBk.h\
   $(OOL)\include\xcolor.h\
   $(OOL)\include\xmsgbox.h\
   $(OOL)\include\xexcept.h\
   $(OOL)\include\XFrmWnd.h\
   $(OOL)\include\XObjWnd.h\
   $(OOL)\include\xrect.h\
   $(OOL)\include\XColor.h

xdbase.obj: xdbase.cpp

xdde.obj: xdde.cpp\
   $(OOL)\include\XWindow.h\
   $(OOL)\include\XDDE.h\
   $(OOL)\include\XDefHdl.h\
   $(OOL)\include\xprocess.h

xdevice.obj: xdevice.cpp

xentry.obj: xentry.cpp\
   $(OOL)\include\xentry.h

xevent.obj: xevent.cpp\
   $(OOL)\include\XCntrevn.h\
   $(OOL)\include\XDragEvn.h\
   $(OOL)\include\XWindow.h\
   $(OOL)\include\XPoint.h\
   $(OOL)\include\XBitmap.h\
   $(OOL)\include\XDragItm.h\
   $(OOL)\include\XMousevn.h\
   $(OOL)\include\XCntEvnt.h\
   $(OOL)\include\XKeyEvnt.h\
   $(OOL)\include\XContObj.h\
   $(OOL)\include\XcnDrEvn.h\
   $(OOL)\include\XCnEdEvn.h\
   $(OOL)\include\xString.h\
   $(OOL)\include\XContCol.h\
   $(OOL)\include\XItmDEvn.h\
   $(OOL)\include\XBackEvn.h

xfile.obj: xfile.cpp\
   $(OOL)\include\XFileInf.h\
   $(OOL)\include\XFile.h

xfiledlg.obj: xfiledlg.cpp\
   $(OOL)\include\xfiledlg.h\
   $(OOL)\include\xwindow.h

xfilefi.obj: xfilefi.cpp\
   $(OOL)\include\XFileFi.h

xfileinf.obj: xfileinf.cpp\
   $(OOL)\include\xfileinf.h\
   $(OOL)\include\XDate.h\
   $(OOL)\include\XTime.h

xfontdlg.obj: xfontdlg.cpp\
   $(OOL)\include\xfontdlg.h

xframe.obj: xframe.cpp\
   $(OOL)\include\XFrmWnd.h\
   $(OOL)\include\XFrame.h\
   $(OOL)\include\XFrmCnt.h\
   $(OOL)\include\XScrlBar.h\
   $(OOL)\include\XRect.h

xfrmcnt.obj: xfrmcnt.cpp\
   $(OOL)\include\XFrmCnt.h\
   $(OOL)\include\XFrmwnd.h\
   $(OOL)\include\XFrame.h

xfrmwnd.obj: xfrmwnd.cpp\
   $(OOL)\include\XFrmwnd.h\
   $(OOL)\include\xexcept.h\
   $(OOL)\include\XResLib.h\
   $(OOL)\include\XMsgBox.h\
   $(OOL)\include\XRes.h\
   $(OOL)\include\XRect.h\
   $(OOL)\include\XString.h\
   $(OOL)\include\XFrame.h\
   $(OOL)\include\XColor.h\
   $(OOL)\include\XCombo.H\
   $(OOL)\include\XMLE.h\
   $(OOL)\include\XSlider.h\
   $(OOL)\include\XEntry.h\
   $(OOL)\include\XCheckBx.h\
   $(OOL)\include\XPushBtn.h\
   $(OOL)\include\XScrlBar.h\
   $(OOL)\include\XSpinBtn.h\
   $(OOL)\include\XListBox.h\
   $(OOL)\include\XCircSld.h\
   $(OOL)\include\XGroupBx.h\
   $(OOL)\include\XSBitmap.h\
   $(OOL)\include\XSIcon.h\
   $(OOL)\include\XSText.h\
   $(OOL)\include\XSFrame.h\
   $(OOL)\include\XValuSet.h\
   $(OOL)\include\XIcon.h\
   $(OOL)\include\XRadio.h\
   $(OOL)\include\XMenuBar.h\
   $(OOL)\include\XContain.h\
   $(OOL)\include\XProcess.h\
   $(OOL)\include\Xcntrevn.h\
   $(OOL)\include\XMedia.h\
   $(OOL)\include\XTimer.h\
   $(OOL)\include\XDragHdl.h\
   $(OOL)\include\XDragEvn.h\
   $(OOL)\include\XMousHdl.h\
   $(OOL)\include\XMousevn.h\
   $(OOL)\include\XWindow.h\
   $(OOL)\include\XContHdl.h\
   $(OOL)\include\XItmDHdl.h\
   $(OOL)\include\XBackHdl.h\
   $(OOL)\include\XStylHdl.h\
   $(OOL)\include\XdefHdl.h\
   $(OOL)\include\XTimer.h\
   $(OOL)\include\XScrlWnd.h\
   $(OOL)\include\XCntEvnt.h\
   $(OOL)\include\XCnEdEvn.h\
   $(OOL)\include\XCndrEvn.h\
   $(OOL)\include\xkeyevnt.h\
   $(OOL)\include\xkeybhdl.h\
   $(OOL)\include\XItmDEvn.h\
   $(OOL)\include\XBackEvn.h\
   $(OOL)\include\XNoteBk.h

xgraphbt.obj: xgraphbt.cpp\
   $(OOL)\include\xgraphbt.h\
   $(OOL)\include\XColor.h

xgraphdv.obj: xgraphdv.cpp\
   $(OOL)\include\XGraphDv.h\
   $(OOL)\include\XGraphOb.h\
   $(OOL)\include\XWindow.h\
   $(OOL)\include\XColor.h\
   $(OOL)\include\XMsgBox.h\
   $(OOL)\include\XFont.h

xgraphob.obj: xgraphob.cpp\
   $(OOL)\include\XGraphOb.h\
   $(OOL)\include\XGraphDv.h\
   $(OOL)\include\XLine.h\
   $(OOL)\include\XColor.h\
   $(OOL)\include\XRect.h\
   $(OOL)\include\XFont.h\
   $(OOL)\include\XCircle.h\
   $(OOL)\include\XText.h\
   $(OOL)\include\XBox.h\
   $(OOL)\include\XMarker.h\
   $(OOL)\include\XBitmap.h\
   $(OOL)\include\XFile.h\
   $(OOL)\include\XFileInf.h\
   $(OOL)\include\XRes.h\
   $(OOL)\include\XResLib.h\
   $(OOL)\include\XArc.h\
   $(OOL)\include\xmsgbox.h\
   $(OOL)\include\xexcept.h\
   $(OOL)\include\XUserBtn.h

xglibdlg.obj: xglibdlg.cpp

xhandler.obj: xhandler.cpp\
   $(OOL)\include\XDragHdl.h\
   $(OOL)\include\XDragEvn.h\
   $(OOL)\include\XMousHdl.h\
   $(OOL)\include\XMousevn.h\
   $(OOL)\include\XWindow.h\
   $(OOL)\include\XContHdl.h\
   $(OOL)\include\XItmDHdl.h\
   $(OOL)\include\XBackHdl.h\
   $(OOL)\include\XStylHdl.h\
   $(OOL)\include\XTimer.h

xhelp.obj: xhelp.cpp\
   $(OOL)\include\XHelp.h\
   $(OOL)\include\XFrmwnd.H\
   $(OOL)\include\XResLib.h\
   $(OOL)\include\XRes.h\
   $(OOL)\include\XProcess.h

xjavaapp.obj: xjavaapp.cpp\
   $(OOL)\include\xheaders.h\
   $(OOL)\include\XJavaApp.h

xjavaenv.obj: xjavaenv.cpp\
   $(OOL)\include\xheaders.h\
   $(OOL)\include\xmsgbox.h\
   $(OOL)\include\xjavaenv.h\
   $(OOL)\include\xjvcmsgh.h\
   $(OOL)\include\xjvsurlh.h\
   $(OOL)\include\xjvsmsgh.h

xjvcmsgh.obj: xjvcmsgh.cpp\
   $(OOL)\include\xheaders.h\
   $(OOL)\include\XJavaEnv.h\
   $(OOL)\include\XJvCMsgH.h

xjvsmsgh.obj: xjvsmsgh.cpp

xjvsurlh.obj: xjvsurlh.cpp

xlayer.obj: xlayer.cpp

xlistbox.obj: xlistbox.cpp\
   $(OOL)\include\xlistbox.h\
   $(OOL)\include\xstring.h\
   $(OOL)\include\xcombo.h

xmdi.obj: xmdi.cpp

xmedia.obj: xmedia.cpp\
   $(OOL)\include\XVideo.h\
   $(OOL)\include\XFrmWnd.h\
   $(OOL)\include\XMsgBox.h\
   $(OOL)\include\XSound.h\
   $(OOL)\include\XCntrEvn.h

xmenu.obj: xmenu.cpp\
   $(OOL)\include\XPopMenu.h\
   $(OOL)\include\XMenuBar.h\
   $(OOL)\include\XMenuItm.h\
   $(OOL)\include\XPoint.h\
   $(OOL)\include\XRes.h\
   $(OOL)\include\XResLib.h\
   $(OOL)\include\XFrmWnd.h\
   $(OOL)\include\XString.h\
   $(OOL)\include\xexcept.h

xmessage.obj: xmessage.cpp

xmle.obj: xmle.cpp\
   $(OOL)\include\xmle.h\
   $(OOL)\include\xmlesear.h\
   $(OOL)\include\xstring.h

xmsgbox.obj: xmsgbox.cpp\
   $(OOL)\include\XMsgBox.h\
   $(OOL)\include\XxtMsgBx.h\
   $(OOL)\include\XString.h\
   $(OOL)\include\XResLib.h\
   $(OOL)\include\XIcon.h

xnotebk.obj: xnotebk.cpp\
   $(OOL)\include\xnotebk.h\
   $(OOL)\include\XBookPg.h

xnpipe.obj: xnpipe.cpp\
   $(OOL)\include\XNPipe.h\
   $(OOL)\include\XcPipe.h

xpipe.obj: xpipe.cpp\
   $(OOL)\include\XPipe.h

xprint.obj: xprint.cpp\
   $(OOL)\include\XCheckBx.h\
   $(OOL)\include\XListBox.h\
   $(OOL)\include\XEntry.h\
   $(OOL)\include\XCntrevn.h\
   $(OOL)\include\xfont.h\
   $(OOL)\include\XFrmwnd.h\
   $(OOL)\include\xmsgbox.h\
   $(OOL)\include\XReslib.h\
   $(OOL)\include\XRes.h\
   $(OOL)\include\XExcept.h\
   $(OOL)\include\xcountry.h\
   $(OOL)\include\xprint.h\
   $(OOL)\include\xgraphob.h

xprofile.obj: xprofile.cpp\
   $(OOL)\include\xprofile.h

xreslib.obj: xreslib.cpp\
   $(OOL)\include\XRes.h\
   $(OOL)\include\XResLib.h\
   $(OOL)\include\XApp.h\
   $(OOL)\include\XWindow.h\
   $(OOL)\include\XString.h\
   $(OOL)\include\XMsgBox.h\
   $(OOL)\include\XIcon.h\
   $(OOL)\include\xexcept.h

xrexx.obj: xrexx.cpp\
   $(OOL)\include\xrexx.h\
   $(OOL)\include\xexcept.h

xscrlbar.obj: xscrlbar.cpp\
   $(OOL)\include\xscrlbar.h

xscrlwnd.obj: xscrlwnd.cpp\
   $(OOL)\include\XScrlWnd.h\
   $(OOL)\include\xrect.h\
   $(OOL)\include\XScrlBar.h\
   $(OOL)\include\XFrame.h\
   $(OOL)\include\XDefhdl.h\
   $(OOL)\include\xexcept.h

xslider.obj: xslider.cpp\
   $(OOL)\include\xslider.h\
   $(OOL)\include\xstring.h\
   $(OOL)\include\xindicat.h

xsocket.obj: xsocket.cpp

xspinbtn.obj: xspinbtn.cpp\
   $(OOL)\include\xspinbtn.h

xstatic.obj: xstatic.cpp\
   $(OOL)\include\XSIcon.h\
   $(OOL)\include\XSFrame.h\
   $(OOL)\include\XSBitmap.h\
   $(OOL)\include\XGroupBx.h\
   $(OOL)\include\XSText.h\
   $(OOL)\include\xcontrol.h

xstring.obj: xstring.cpp\
   $(OOL)\include\XString.h

xthread.obj: xthread.cpp\
   $(OOL)\include\xthread.h\
   $(OOL)\include\xmsgbox.h\
   $(OOL)\include\xexcept.h

xtime.obj: xtime.cpp\
   $(OOL)\include\xtime.h\
   $(OOL)\include\xstring.h\
   $(OOL)\include\xdate.h

xtoolbar.obj: xtoolbar.cpp\
   $(OOL)\include\XToolBar.h\
   $(OOL)\include\XFrmWnd.h\
   $(OOL)\include\XRect.h\
   $(OOL)\include\XFrame.h\
   $(OOL)\include\XColor.h\
   $(OOL)\include\XGraphDv.H\
   $(OOL)\include\XLine.h\
   $(OOL)\include\XCntrEvn.h\
   $(OOL)\include\XMousHdl.h\
   $(OOL)\include\XMousevn.h\
   $(OOL)\include\XReslib.h\
   $(OOL)\include\xmsgbox.h

xuserbtn.obj: xuserbtn.cpp\
   $(OOL)\include\xuserbtn.h\
   $(OOL)\include\xdefhdl.h\
   $(OOL)\include\xmoushdl.h\
   $(OOL)\include\xmousevn.h\
   $(OOL)\include\xbitmap.h\
   $(OOL)\include\XLine.h\
   $(OOL)\include\xbox.h\
   $(OOL)\include\xfont.h

xuserwnd.obj: xuserwnd.cpp\
   $(OOL)\include\xuserwnd.h\
   $(OOL)\include\xrect.h\
   $(OOL)\include\xexcept.h\
   $(OOL)\include\xcntrevn.h\
   $(OOL)\include\xreslib.h\
   $(OOL)\include\xbubble.h\
   $(OOL)\include\xtimer.h\
   $(OOL)\include\xbitmap.h\
   $(OOL)\include\xgraphdv.h\
   $(OOL)\include\xres.h\
   $(OOL)\include\xprocess.h\
   $(OOL)\include\xmoushdl.h\
   $(OOL)\include\xfont.h\
   $(OOL)\include\xmousevn.h\
   $(OOL)\include\XText.h

xvaluset.obj: xvaluset.cpp\
   $(OOL)\include\xvaluset.h\
   $(OOL)\include\xstring.h\
   $(OOL)\include\xcolor.h\
   $(OOL)\include\xicon.h\
   $(OOL)\include\xbitmap.h

xwebview.obj: xwebview.cpp\
   $(OOL)\include\XControl.h\
   $(OOL)\include\XWebView.h\
   $(OOL)\include\xmsgbox.h\
   $(OOL)\include\webexwin.h\
   $(OOL)\include\xstring.h\
   $(OOL)\include\XDefHdl.h

xwindow.obj: xwindow.cpp\
   $(OOL)\include\XWindow.h\
   $(OOL)\include\XString.h\
   $(OOL)\include\XRect.h\
   $(OOL)\include\XColor.h\
   $(OOL)\include\XHandler.h\
   $(OOL)\include\XPoint.h

xwindrag.obj: xwindrag.cpp\
   $(OOL)\include\XWindow.h\
   $(OOL)\include\XWinDrag.h\
   $(OOL)\include\XDragItm.h\
   $(OOL)\include\XBitmap.h\
   $(OOL)\include\XIcon.h

xcrypt.obj: xcrypt.cpp\
   $(OOL)\include\xcrypt.h

crypt.obj: crypt.c
crypt_util.obj: crypt_util.c
md5.obj: md5.c



