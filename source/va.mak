#makefile for OOL - VisualAge C++

#files: xapp.cpp xbookpg.cpp xbutton.cpp xchart.cpp xcolor.cpp xcircsld.cpp xclipbrd.cpp xcont.cpp xcontain.cpp xcontrol.cpp xdde.cpp xdevice.cpp xdialog.cpp xentry.cpp xevent.cpp xesem.cpp xexcept.cpp xfile.cpp xfileinf.cpp xfiledlg.cpp xfilefi.cpp xfontdlg.cpp xframe.cpp xfrmcnt.cpp xfrmwnd.cpp xgraphdv.cpp xgraphob.cpp xhandler.cpp xhelp.cpp xlistbox.cpp xmdi.cpp xmenu.cpp xmessage.cpp xmle.cpp xmsgbox.cpp xmusem.cpp xnotebk.cpp xnpipe.cpp xpipe.cpp xprofile.cpp xprint.cpp xrexx.cpp xreslib.cpp xscrlbar.cpp xscrlwnd.cpp xslider.cpp xspinbtn.cpp xstatic.cpp xstring.cpp xthread.cpp xtime.cpp xtoolbar.cpp xvaluset.cpp xuserbtn.cpp xuserwnd.cpp xwindow.cpp xwindrag.cpp xmedia.cpp xgraphbt.cpp xvirtscr.cpp xbltwnd.cpp xpal.cpp xblitter.cpp xwebview.cpp xsocket.cpp xtable.cpp xdbase.cpp xjavaenv.cpp xjvcmsgh.cpp xjavaapp.cpp xjvsmsgh.cpp xjvsurlh.cpp xwindow.cpp

OBJ_MAIN= xapp.obj xbookpg.obj xbutton.obj xchart.obj xcons.obj xcolor.obj xcircsld.obj xclipbrd.obj xcont.obj xcontain.obj xcontrol.obj xdde.obj xdevice.obj xdialog.obj xentry.obj xevent.obj xesem.obj xexcept.obj xfile.obj xfileinf.obj xfiledlg.obj xfilefi.obj xfontdlg.obj xframe.obj xfrmcnt.obj xfrmwnd.obj xgraphdv.obj xgraphob.obj xhandler.obj xhelp.obj xlayer.obj xlistbox.obj xmdi.obj xmenu.obj xmessage.obj xmle.obj xmsgbox.obj xmusem.obj xnotebk.obj xnpipe.obj xpipe.obj xprofile.obj xprint.obj xrexx.obj xreslib.obj xscrlbar.obj xscrlwnd.obj xslider.obj xspinbtn.obj xstatic.obj xstring.obj xthread.obj xtime.obj xtoolbar.obj xvaluset.obj xuserbtn.obj xuserwnd.obj xwindow.obj xwindrag.obj xwizard.obj xmemchk.obj
OBJ_TCP=xsocket.obj
OBJ_DBASE=xdbase.obj
OBJ_JAVA=xjavaenv.obj xjvcmsgh.obj xjavaapp.obj xjvsmsgh.obj xjvsurlh.obj
OBJ_CRYPT=xcrypt.obj crypt.obj crypt_util.obj md5.obj
OBJ_C=oolc3i03.c
OBJ_MEDIA=xmedia.obj xgraphbt.obj xvirtscr.obj xbltwnd.obj xpal.obj xblitter.obj
DEF_LIBS=$(OOL)\LIB\oolc3i10.lib os2386.lib


COMPILE=icc.exe /Ge- /Tdp /Q /O+ /G5 /Gm+ /Gn+ /Gd+ /C /Fi /Si /Ti-
LINK=/Ge- /Tdp /Gm+ /B"/noe /e:2 /NOD"
#LINK_IP=/Ge- /Tdp /Gm+ /B"/noe /e:2 /IGNORECASE"

all: OOLC3I10.DLL OOLRES.DLL OOLW3I10.dll OOLM3I10.DLL OOLT3I10.dll OOL23I10.dll OOLJ3I10.dll OOLY3I10.DLL clean

OOLC3I10.DLL: empty.c
   icc.exe /Fe"OOLC3I10.DLL" /Ge- /Gm+ empty.c oolc3i10.def
   implib /NOI oolc3i10.lib oolc3i10.def
   ilib /CONV /NOBR /NOE oolc3i10.lib +cppom30o.lib
   @copy OOLC3I10.LIB $(OOL)\LIB\OOLC3I10.LIB
   @copy OOLC3I10.DLL $(OOL)\LIB\OOLC3I10.DLL

.cpp.obj:
   $(COMPILE) %s
.c.obj:
   $(COMPILE) %s

.\OOLW3I10.LIB: $(OBJ_MAIN)
  @echo build library
  @if exist OOLW3I10.LIB erase OOLW3I10.LIB
  ilib /nol /noi @<<
.\OOLW3I10.LIB
y
xapp.obj &
xbookpg.obj &
xbutton.obj &
xchart.obj &
xcons.obj &
xcolor.obj &
xcircsld.obj &
xclipbrd.obj &
xcont.obj &
xcontain.obj &
xcontrol.obj &
xdde.obj &
xdevice.obj &
xdialog.obj &
xentry.obj &
xesem.obj &
xevent.obj &
xexcept.obj &
xfile.obj &
xfileinf.obj &
xfiledlg.obj &
xfilefi.obj &
xfontdlg.obj &
xframe.obj &
xfrmcnt.obj &
xfrmwnd.obj &
xgraphdv.obj &
xgraphob.obj &
xhandler.obj &
xhelp.obj &
xlayer.obj &
xlistbox.obj &
xmdi.obj &
xmenu.obj &
xmessage.obj &
xmle.obj &
xmsgbox.obj &
xmusem.obj &
xnotebk.obj &
xnpipe.obj &
xpipe.obj &
xprofile.obj &
xprint.obj &
xrexx.obj &
xreslib.obj &
xscrlbar.obj &
xscrlwnd.obj &
xslider.obj &
xspinbtn.obj &
xstatic.obj &
xstring.obj &
xthread.obj &
xtime.obj &
xtoolbar.obj &
xvaluset.obj &
xuserbtn.obj &
xuserwnd.obj &
xwindow.obj &
xwizard.obj &
xwindrag.obj &
xmemchk.obj, .\OOLW3I10.LST;
<<

.\OOLW3I10.DEF: .\OOLW3I10.LIB
  @echo Generating definition file...
  @echo LIBRARY OOLW3I10 INITINSTANCE TERMINSTANCE > OOLW3I10.DEF
  @echo DESCRIPTION 'OOL window-DLL for VA' >> OOLW3I10.DEF
  @echo DATA MULTIPLE NONSHARED READWRITE LOADONCALL >> OOLW3I10.DEF
  @echo CODE LOADONCALL >> OOLW3I10.DEF
  @echo PROTMODE >> OOLW3I10.DEF
  @echo IMPORTS >> OOLW3I10.DEF
  @echo EXPORTS >> OOLW3I10.DEF
  @cppfilt -b -p -q -n -o OOLW3I10.LIB >> OOLW3I10.DEF
  @if exist OOLW3I10.LIB erase OOLW3I10.LIB
  implib OOLW3I10.LIB OOLW3I10.DEF
  @copy OOLW3I10.LIB $(OOL)\LIB\OOLW3I10.LIB


.\OOLW3I10.DLL: $(OBJ_MAIN) .\OOLW3I10.DEF
  @echo Linking ...
  @icc @<<
$(LINK) /Fe.\OOLW3I10.DLL $(OBJ_MAIN) $(DEF_LIBS) mmpm2.lib rexx.lib .\OOLW3I10.DEF
<<
   @copy OOLW3I10.DLL $(OOL)\LIB\OOLW3I10.DLL


OOLM3I10.DEF: $(OBJ_MEDIA) OOLM3I10.LIB
  @echo Generating definition file...
  @echo LIBRARY OOLm3I10 INITINSTANCE TERMINSTANCE > OOLM3I10.DEF
  @echo DESCRIPTION 'OOL multimedia-DLL for VA' >> OOLM3I10.DEF
  @echo DATA MULTIPLE NONSHARED READWRITE LOADONCALL >> OOLM3I10.DEF
  @echo CODE LOADONCALL >> OOLM3I10.DEF
  @echo PROTMODE >> OOLM3I10.DEF
  @echo IMPORTS >> OOLM3I10.DEF
  @echo EXPORTS >> OOLM3I10.DEF
  @cppfilt -b -p -q -n -o OOLM3I10.LIB >> OOLM3I10.DEF
  @if exist OOLM3I10.LIB erase OOLM3I10.LIB
  implib OOLM3I10.LIB OOLM3I10.DEF
  @copy OOLM3I10.LIB $(OOL)\LIB\OOLM3I10.LIB


.\OOLM3I10.LIB: $(OBJ_MEDIA)
  @echo build library
  @if exist OOLM3I10.LIB erase OOLM3I10.LIB
  ilib /nol /noi @<<
.\OOLM3I10.LIB
y
$(OBJ_MEDIA) , .\OOLM3I10.LST;
<<


.\OOLM3I10.DLL: $(OBJ_MEDIA) oolM3i10.def
  @echo Linking ...
  @icc @<<
$(LINK) /Fe.\OOLM3I10.DLL $(OBJ_MEDIA) OOLW3I10.lib $(DEF_LIBS) mmpm2.lib .\OOLM3I10.DEF
<<
  @copy OOLM3I10.DLL $(OOL)\LIB\OOLM3I10.DLL


OOLT3I10.DEF: $(OBJ_TCP) OOLT3I10.LIB
  @echo Generating definition file...
  @echo LIBRARY OOLT3I10 INITINSTANCE TERMINSTANCE > OOLT3I10.DEF
  @echo DESCRIPTION 'OOL TCP/IP-DLL for VA' >> OOLT3I10.DEF
  @echo DATA MULTIPLE NONSHARED READWRITE LOADONCALL >> OOLT3I10.DEF
  @echo CODE LOADONCALL >> OOLT3I10.DEF
  @echo PROTMODE >> OOLT3I10.DEF
  @echo IMPORTS >> OOLT3I10.DEF
  @echo EXPORTS >> OOLT3I10.DEF
  @cppfilt -b -p -q -n -o OOLT3I10.LIB >> OOLT3I10.DEF
  @if exist OOLT3I10.LIB erase OOLT3I10.LIB
  implib OOLT3I10.LIB OOLT3I10.DEF
  @copy OOLT3I10.LIB $(OOL)\LIB\OOLT3I10.LIB


.\OOLT3I10.LIB: $(OBJ_TCP)
  @echo build library
  @if exist OOLT3I10.LIB erase OOLT3I10.LIB
  ilib /nol /noi @<<
.\OOLT3I10.LIB
y
$(OBJ_TCP) , .\OOLT3I10.LST;
<<


.\OOLT3I10.DLL: $(OBJ_TCP) oolT3i10.def
  @echo Linking ...
  @icc @<<
$(LINK) /Fe.\OOLT3I10.DLL $(OBJ_TCP) OOLW3I10.lib $(DEF_LIBS) $(OOL)\LIB\oolip.lib .\OOLT3I10.DEF
<<
  @copy OOLT3I10.DLL $(OOL)\LIB\OOLT3I10.DLL




OOL23I10.DEF: $(OBJ_DBASE) OOL23I10.LIB
  @echo Generating definition file...
  @echo LIBRARY OOL23I10 INITINSTANCE TERMINSTANCE > OOL23I10.DEF
  @echo DESCRIPTION 'OOL DBASEIII-DLL for VA' >> OOL23I10.DEF
  @echo DATA MULTIPLE NONSHARED READWRITE LOADONCALL >> OOL23I10.DEF
  @echo CODE LOADONCALL >> OOL23I10.DEF
  @echo PROTMODE >> OOL23I10.DEF
  @echo IMPORTS >> OOL23I10.DEF
  @echo EXPORTS >> OOL23I10.DEF
  @cppfilt -b -p -q -n -o OOL23I10.LIB >> OOL23I10.DEF
  @if exist OOL23I10.LIB erase OOL23I10.LIB
  implib OOL23I10.LIB OOL23I10.DEF
  @copy OOL23I10.LIB $(OOL)\LIB\OOL23I10.LIB

.\OOL23I10.LIB: $(OBJ_DBASE)
  @echo build library
  @if exist OOL23I10.LIB erase OOL23I10.LIB
  ilib /nol /noi @<<
.\OOL23I10.LIB
y
$(OBJ_DBASE) , .\OOL23I10.LST;
<<


.\OOL23I10.DLL: $(OBJ_DBASE) ool23i10.def
  @echo Linking ...
  @icc @<<
$(LINK) /Fe.\OOL23I10.DLL $(OBJ_DBASE) $(DEF_LIBS) OOLW3I10.lib .\OOL23I10.DEF
<<
  @copy OOL23I10.DLL $(OOL)\LIB\OOL23I10.DLL



OOLJ3I10.DEF: $(OBJ_JAVA) OOLJ3I10.LIB
  @echo Generating definition file...
  @echo LIBRARY OOLJ3I10 INITINSTANCE TERMINSTANCE > OOLJ3I10.DEF
  @echo DESCRIPTION 'OOL Java-DLL for VA' >> OOLJ3I10.DEF
  @echo DATA MULTIPLE NONSHARED READWRITE LOADONCALL >> OOLJ3I10.DEF
  @echo CODE LOADONCALL >> OOLJ3I10.DEF
  @echo PROTMODE >> OOLJ3I10.DEF
  @echo IMPORTS >> OOLJ3I10.DEF
  @echo EXPORTS >> OOLJ3I10.DEF
  @cppfilt -b -p -q -n -o OOLJ3I10.LIB >> OOLJ3I10.DEF
  @if exist OOLJ3I10.LIB erase OOLJ3I10.LIB
  implib OOLJ3I10.LIB OOLJ3I10.DEF
  @copy OOLJ3I10.LIB $(OOL)\LIB\OOLJ3I10.LIB


.\OOLJ3I10.LIB: $(OBJ_JAVA)
  @echo build library
  @if exist OOLJ3I10.LIB erase OOLJ3I10.LIB
  ilib /nol /noi @<<
.\OOLJ3I10.LIB
y
$(OBJ_JAVA) , .\OOLJ3I10.LST;
<<


.\OOLJ3I10.DLL: $(OBJ_JAVA) oolJ3i10.def
  @echo Linking ...
  @icc @<<
$(LINK) /Fe.\OOLJ3I10.DLL $(OBJ_JAVA) jempo10.lib $(DEF_LIBS) OOLW3I10.lib .\OOLJ3I10.DEF
<<
  @copy OOLJ3I10.DLL $(OOL)\LIB\OOLJ3I10.DLL


OOLY3I10.DEF: $(OBJ_CRYPT) OOLY3I10.LIB
  @echo Generating definition file...
  @echo LIBRARY OOLY3I10 INITINSTANCE TERMINSTANCE > OOLY3I10.DEF
  @echo DESCRIPTION 'OOL Crypt-DLL for VA' >> OOLY3I10.DEF
  @echo DATA MULTIPLE NONSHARED READWRITE LOADONCALL >> OOLY3I10.DEF
  @echo CODE LOADONCALL >> OOLY3I10.DEF
  @echo PROTMODE >> OOLY3I10.DEF
  @echo IMPORTS >> OOLY3I10.DEF
  @echo EXPORTS >> OOLY3I10.DEF
  @cppfilt -b -p -q -n -o OOLY3I10.LIB >> OOLY3I10.DEF
  @if exist OOLY3I10.LIB erase OOLY3I10.LIB
  implib OOLY3I10.LIB OOLY3I10.DEF
  @copy OOLY3I10.LIB $(OOL)\LIB\OOLY3I10.LIB


.\OOLY3I10.LIB: $(OBJ_CRYPT)
  @echo build library
  @if exist OOLY3I10.LIB erase OOLY3I10.LIB
  ilib /nol /noi @<<
.\OOLY3I10.LIB
y
$(OBJ_CRYPT) , .\OOLY3I10.LST;
<<


.\OOLY3I10.DLL: $(OBJ_CRYT) oolY3i10.def
  @echo Linking ...
  @icc @<<
$(LINK) /Fe.\OOLY3I10.DLL $(OBJ_CRYPT) $(DEF_LIBS) OOLW3I10.lib .\OOLY3I10.DEF
<<
  @copy OOLY3I10.DLL $(OOL)\LIB\OOLY3I10.DLL

OOLRES.DLL: empty.obj $(OOL)\resource\OOLRES.RES
   @echo ;OOL resource-library  > oolres.def
   @echo LIBRARY OOLRES >> oolres.def
   icc $(LINK) /Fe"OOLRES.DLL" empty.obj OOLRES.DEF $(DEF_LIBS)
   rc $(OOL)\resource\oolres.res OOLRES.DLL
   @copy OOLRES.DLL $(OOL)\LIB\OOLRES.DLL

empty.obj: empty.c

OOLRES.RES: $(OOL)\resource\oolres.rc
   rc -r $(OOL)\resource\oolres.rc



clean:
   @if exist *.lst erase *.lst

empty.obj: empty.c
xwizard.obj: xwizard.cpp
xcons.obj: xcons.cpp
xmemchk.obj: xmemchk.cpp


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



