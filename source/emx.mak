#this is the makfile for the Open Object Library (OS/2 version, EMX compiler)
#nessesary tools:
#gnu make
#emx 0.9c, emx multimedia package, jempower
#librarys for rexx and java must be build first, see documentation

COMPILE=gcc -c -Zmtd -Zcrtdll -Zdll -Zomf -fhandle-exceptions -frtti -b -s
LINK=gcc -Zmtd -Zcrtdll -Zomf -Zdll -Zlinker /PACKC -Zlinker /PACKD -o

#DLLs to build
all: OOLW3E10.DLL OOLM3E10.DLL OOLJ3E10.DLL OOL23E10.DLL OOLY3E10.DLL OOLT3E10.DLL

#the object-files for window stuff
OBJ_MAIN=xapp.obj xbookpg.obj xbutton.obj xchart.obj xcolor.obj xcircsld.obj xclipbrd.obj xcont.obj\
	xcontain.obj xcontrol.obj xdde.obj xdevice.obj xdialog.obj xentry.obj xevent.obj xesem.obj\
	xexcept.obj xfile.obj xfileinf.obj xfiledlg.obj xfilefi.obj xfontdlg.obj xframe.obj xfrmcnt.obj\
	xfrmwnd.obj xgraphdv.obj xgraphob.obj xhandler.obj xhelp.obj xlayer.obj xlistbox.obj xmdi.obj xmenu.obj\
	xmessage.obj xmle.obj xmsgbox.obj xmusem.obj xnotebk.obj xnpipe.obj xpipe.obj xprofile.obj\
	xprint.obj xrexx.obj xreslib.obj xscrlbar.obj xscrlwnd.obj xslider.obj xspinbtn.obj xstatic.obj\
	xstring.obj xthread.obj xtime.obj xtoolbar.obj xvaluset.obj xuserbtn.obj xuserwnd.obj xwindow.obj xwizard.obj xwindrag.obj xmemchk.obj xcons.obj


OBJ_MEDIA=xmedia.obj xgraphbt.obj xvirtscr.obj xbltwnd.obj xpal.obj xblitter.obj
OBJ_JAVA=xjavaenv.obj xjvcmsgh.obj xjavaapp.obj xjvsmsgh.obj xjvsurlh.obj
OBJ_DBASE=xdbase.obj
OBJ_CRYPT=xcrypt.obj crypt.obj crypt_util.obj md5.obj
OBJ_TCP=xsocket.obj
#OBJ_GLIB=oolglib.obj xglib.obj debug.obj

#how to build the objects
%.obj: %.c
	$(COMPILE) $<

%.obj: %.cpp
	$(COMPILE) $<

#how to build the window-related dll
#the library rexx2emx.lib must be build from the developers toolkit librarys,
#see ool.htm for details
OOLW3E10.DLL: $(OBJ_MAIN)
	@echo ;DEF-file for EMX (window-stuff) > oolw3e10.def
	@echo LIBRARY OOLW3E10 INITINSTANCE TERMINSTANCE >> oolw3e10.def
	@echo DATA NONSHARED MULTIPLE READWRITE LOADONCALL >> oolw3e10.def
	@echo CODE LOADONCALL >> oolw3e10.def
	@echo PROTMODE >> oolw3e10.def
	@echo EXPORTS >> oolw3e10.def
	emxexp -n -o $(OBJ_MAIN) >> oolw3e10.def
#	copy emx_w.def + imp.tmp OOLW3E10.def
	$(LINK) OOLW3E10.DLL $(OBJ_MAIN) c:\emx\lib\rexx2emx.lib OOLW3E10.def -lstdcpp
	emximp -o OOLW3E10.LIB OOLW3E10.DEF
	copy OOLW3E10.lib $(OOL)\lib
	copy OOLW3E10.DLL $(OOL)\lib


#how to build the multimedia-related dll
OOLM3E10.DLL: $(OBJ_MEDIA)
	@echo ;DEF-file for EMX (multimedia-stuff) > oolm3e10.def
	@echo LIBRARY OOLM3E10 INITINSTANCE TERMINSTANCE >> oolm3e10.def
	@echo DATA NONSHARED MULTIPLE READWRITE LOADONCALL >> oolm3e10.def
	@echo CODE LOADONCALL >> oolm3e10.def
	@echo PROTMODE >> oolm3e10.def
	@echo EXPORTS >> oolm3e10.def
	emxexp -n -o $(OBJ_MEDIA) >> oolm3e10.def
	$(LINK) OOLM3E10.DLL $(OBJ_MEDIA) $(OOL)\lib\OOLW3E10.lib c:\emx\lib\mm2emx.lib c:\emx\lib\oolimp.lib OOLM3E10.def -lstdcpp
	emximp -o OOLM3E10.LIB OOLM3E10.DEF
	copy OOLM3E10.lib $(OOL)\lib
	copy OOLM3E10.DLL $(OOL)\lib

#how to build the crypt- dll
OOLY3E10.DLL: $(OBJ_CRYPT)
	@echo ;DEF-file for EMX (crypt-stuff) > ooly3e10.def
	@echo LIBRARY OOLY3E10 INITINSTANCE TERMINSTANCE >> ooly3e10.def
	@echo DATA NONSHARED MULTIPLE READWRITE LOADONCALL >> ooly3e10.def
	@echo CODE LOADONCALL >> ooly3e10.def
	@echo PROTMODE >> ooly3e10.def
	@echo EXPORTS >> ooly3e10.def
	emxexp -n -o $(OBJ_CRYPT) >> ooly3e10.def
	$(LINK) OOLY3E10.DLL $(OBJ_CRYPT) $(OOL)\lib\OOLW3E10.lib OOLY3E10.def -lstdcpp
	emximp -o OOLY3E10.LIB OOLY3E10.DEF
	copy OOLY3E10.lib $(OOL)\lib
	copy OOLY3E10.DLL $(OOL)\lib

#how to build the TCPIP- dll
OOLT3E10.DLL: $(OBJ_TCP)
	@echo ;DEF-file for EMX (TCP-stuff) > oolt3e10.def
	@echo LIBRARY OOLT3E10 INITINSTANCE TERMINSTANCE >> oolt3e10.def
	@echo DATA NONSHARED MULTIPLE READWRITE LOADONCALL >> oolt3e10.def
	@echo CODE LOADONCALL >> oolt3e10.def
	@echo PROTMODE >> oolt3e10.def
	@echo EXPORTS >> oolt3e10.def
	emxexp -n -o $(OBJ_TCP) >> oolt3e10.def
	$(LINK) OOLT3E10.DLL $(OBJ_TCP) $(OOL)\lib\OOLW3E10.lib OOLT3E10.def -lstdcpp -lsocket
	emximp -o OOLT3E10.LIB OOLT3E10.DEF
	copy OOLT3E10.lib $(OOL)\lib
	copy OOLT3E10.DLL $(OOL)\lib

#how to build the java-related dll
OOLJ3E10.DLL: $(OBJ_JAVA)
	@echo ;DEF-file for EMX (Java-stuff) > oolj3e10.def
	@echo LIBRARY OOLJ3E10 INITINSTANCE TERMINSTANCE >> oolj3e10.def
	@echo DATA NONSHARED MULTIPLE READWRITE LOADONCALL >> oolj3e10.def
	@echo CODE LOADONCALL >> oolj3e10.def
	@echo PROTMODE >> oolj3e10.def
	@echo EXPORTS >> oolj3e10.def
	emxexp -n -o $(OBJ_JAVA) >> oolj3e10.def
	$(LINK) OOLJ3E10.DLL $(OBJ_JAVA) $(OOL)\lib\OOLW3E10.lib c:\emx\lib\java2emx.lib OOLJ3E10.def -lstdcpp
	emximp -o OOLJ3E10.LIB OOLJ3E10.DEF
	copy OOLJ3E10.lib $(OOL)\lib
	copy OOLJ3E10.DLL $(OOL)\lib

#dbase
OOL23E10.DLL: $(OBJ_DBASE)
	@echo ;DEF-file for EMX (DBase-stuff) > ool23e10.def
	@echo LIBRARY OOL23E10 INITINSTANCE TERMINSTANCE >> ool23e10.def
	@echo DATA NONSHARED MULTIPLE READWRITE LOADONCALL >> ool23e10.def
	@echo CODE LOADONCALL >> ool23e10.def
	@echo PROTMODE >> ool23e10.def
	@echo EXPORTS >> ool23e10.def
	emxexp -n -o $(OBJ_DBASE) >> ool23e10.def
	$(LINK) OOL23E10.DLL $(OBJ_DBASE) $(OOL)\lib\OOLW3E10.lib OOL23E10.def -lstdcpp
	emximp -o OOL23E10.LIB OOL23E10.DEF
	copy OOL23E10.lib $(OOL)\lib
	copy OOL23E10.DLL $(OOL)\lib

#OOLGLIB.DLL: $(OBJ_GLIB)
#	@echo Generating definition file...
#	@echo LIBRARY OOLGLIB INITINSTANCE TERMINSTANCE > oolglib.def
#	@echo DATA NONSHARED MULTIPLE READWRITE LOADONCALL >> oolglib.def
#	@echo CODE LOADONCALL >> oolglib.def
#	@echo PROTMODE >> oolglib.def
#	@echo EXPORTS >> oolglib.def
#	@echo	 OOL_GLIB_IMP >> oolglib.def
#	@echo	 OOL_GLIB_EXP >> oolglib.def
#	emxexp -n -o $(OBJ_GLIB) >> ool23e10.def
#	$(LINK) OOLGLIB.DLL $(OBJ_GLIB) $(OOL)\lib\OOLW3E10.lib c:\graphics\gbm\gbm.lib OOLGLIB.def -lstdcpp
#	emximp -o OOLGLIB.LIB OOLGLIB.DEF
#	copy OOLGLIB.lib $(OOL)\lib
#	copy OOLGLIB.DLL $(OOL)\lib

#dependencies
xexcept.obj: xexcept.cpp
oolglib.obj: oolglib.c
xglib.obj: xglib.cpp
debug.obj: debug.cpp
xwizard.obj: xwizard.cpp
xmemchk.obj: xmemchk.cpp
xcons.obj: xcons.cpp

xapp.obj: xapp.cpp\
	c:\source\ool\include\xapp.h\
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
	$(OOL)\include\XWindow.h\
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

xdde.obj: xdde.cpp\
	$(OOL)\include\XWindow.h\
	$(OOL)\include\XDDE.h\
	$(OOL)\include\XDefHdl.h\
	$(OOL)\include\xprocess.h

xdbase.obj: xdbase.cpp

xdialog.obj: xdialog.cpp

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

clean:
	del *.s *.map *.tmp *.lib *.dll

