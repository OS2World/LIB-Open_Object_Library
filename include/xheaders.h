#ifndef __OOL_HEADER_H__
#define __OOL_HEADER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/* because often names of header-files seams to be cryptonized, here is a    */
/* simple method wich makes including headers very easy. You just need to    */
/* include the classname with the suffix _i like >>#include XControlEvent_i<<*/
/* instead of >>#include "xcntrlevn.h"<<                                     */
/*===========================================================================*/

#include "xinclude.h"


#define XObject_i           "xobject.h"
#define XClipBoard_i        "xclipbrd.h"
#define XColor_i            "xcolor.h"
#define XContainerColumn_i  "xcontcol.h"
#define XContainerInfo_i    "xcontinf.h"
#define XContainerObject_i  "xcontobj.h"
#define XEvent_i            "xevent.h"
#define XControlEvent_i     "xcntrevn.h"
#define XFile_i             "xfile.h"
#define XFileDialog_i       "xfiledlg.h"
#define XGraphicDevice_i    "xgraphdv.h"
#define XGraphicObject_i    "xgraphob.h"
#define XBox_i              "xbox.h"
#define XElipse_i           "xelipse.h"
#define XLine_i             "xline.h"
#define XMarker_i           "xmarker.h"
#define XHelpInstance_i     "xhelp.h"
#define XMenuItem_i         "xmenuitm.h"
#define XPoint_i            "xpoint.h"
#define XProcess_i          "xprocess.h"
#define XApplication_i      "xapp.h"
#define XThread_i           "xthread.h"
#define XRect_i             "xrect.h"
#define XResource_i         "xres.h"
#define XResourceLibrary_i  "xreslib.h"
#define XString_i           "xstring.h"
#define XWindow_i           "xwindow.h"
#define XControl_i          "xcontrol.h"
#define XButton_i           "xbutton.h"
#define XSettingButton_i    "xsetbtn.h"
#define XCheckBox_i         "xcheckbx.h"
#define XRadioButton_i      "xradio.h"
#define XGraphicButton_i    "xgraphbt.h"
#define XPushButton_i       "xpushbtn.h"
#define XCircularSlider_i   "xcircsld.h"
#define XContainerControl_i "xcontain.h"
#define XEntryField_i       "xentry.h"
#define XFrame_i            "xframe.h"
#define XListBox_i          "xlistbox.h"
#define XComboBox_i         "xcombo.h"
#define XMultiLineEdit_i    "xmle.h"
#define XScrollBar_i        "xscrlbar.h"
#define XSlider_i           "xslider.h"
#define XSpinButton_i       "xspinbtn.h"
#define XValueSet_i         "xvaluset.h"
#define XFrameWindow_i      "xfrmwnd.h"
#define XScrollWindow_i     "xscrlwnd.h"
#define XMenu_i             "xmenu.h"
#define XMenuBar_i          "xmenubar.h"
#define XPopupMenu_i        "xpopmenu.h"
#define XObjectWindow_i     "xobjwnd.h"
#define XMediaWindow_i      "xmedia.h"
#define XSound_i            "xsound.h"
#define XVideo_i            "xvideo.h"
#define XStatic_i           "xstatic.h"
#define XGroupBox_i         "xgroupbx.h"
#define XStaticBitmap_i     "xsbitmap.h"
#define XStaticFrame_i      "xsframe.h"
#define XStaticIcon_i       "xsicon.h"
#define XStaticText_i       "xstext.h"
#define XUserWindow_i       "xuserwnd.h"
#define XFrameControl_i     "xfrmcnt.h"
#define XStatusWindow_i     "xstatwin.h"
#define XToolBar_i          "xtoolbar.h"
#define XMessageBox_i       "xmsgbox.h"
#define XDate_i             "xdate.h"
#define XTime_i             "xtime.h"
#define XFont_i             "xfont.h"
#define XText_i             "xtext.h"
#define XHandler_i          "xhandler.h"
#define XDragHandler_i      "xdraghdl.h"
#define XDragEvent_i        "xdragevn.h"
#define XDragItem_i         "xdragitm.h"
#define XMouseEvent_i       "xmousevn.h"
#define XMouseHandler_i     "xmoushdl.h"
#define XFile_i             "xfile.h"
#define XContainerHandler_i   "xconthdl.h"
#define XContainerEvent_i   "xcntevnt.h"
#define XKeyboardHandler_i  "xkeybhdl.h"
#define XKeyboardEvent_i    "xkeyevnt.h"
#define XContainerDragEvent_i   "xcndrevn.h"
#define XWindowDrag_i       "xwindrag.h"
#define XContainerEditEvent_i   "xcnedevn.h"
#define XItemDrawEvent_i    "xitmdevn.h"
#define XItemDrawHandler_i    "xitmdhdl.h"
#define XBackgroundDrawHandler_i  "xbackhdl.h"
#define XBackgroundDrawEvent_i  "xbackevn.h"
#define XStyleHandler_i  "xstylhdl.h"
#define XBitmap_i  "xbitmap.h"
#define XCollection_i   "XCollect.h"
#define XFileFind_i    "xfilefi.h"
#define XUserButton_i   "xuserbtn.h"
#define XDefaultHandler_i   "xdefhdl.h"
#define XFileInfo_i   "xfileinf.h"
#define XMLESearch_i   "xmlesear.h"
#define XExtendedMessageBox_i   "xxtmsgbx.h"
#define XCircle_i   "xcircle.h"
#define XArc_i   "xarc.h"
#define XChart_i   "xchart.h"
#define XPie_i   "xpie.h"
#define XNoteBook_i   "xnotebk.h"
#define XNoteBookPage_i  "xbookpg.h"
#define XDDE_i   "xdde.h"
#define XTimer_i   "xtimer.h"
#define XBubbleHelp_i   "xbubble.h"
#define XIO_i   "xio.h"
#define XPipe_i   "xpipe.h"
#define XIcon_i   "xicon.h"
#define XProfile_i   "xprofile.h"
#define XNamedPipeServer_i   "xnpipe.h"
#define XNamedPipeClient_i   "xcpipe.h"
#define XFontDialog_i   "xfontdlg.h"
#define XWebView_i         "xwebview.h"
#define XVirtualScreen_i     "xvirtscr.h"
#define XBar_i             "xbar.h"
#define XException_i       "xexcept.h"
#define XTable_i           "xtable.h"
#define XRexxInterface_i       "xrexx.h"
#define XPrinterDevice_i     "xprint.h"
#define XCountryInfo_i        "xcountry.h"
#define XIndicator_i           "xindicat.h"
#define XJavaEnvironment_i      "xjavaenv.h"
#define XJavaConsoleMessageHandler_i     "xjvcmsgh.h"
#define XJavaApplet_i           "xjavaapp.h"
#define XJavaShowURLHandler_i     "xjvsurlh.h"
#define XJavaStatusMessageHandler_i   "xjvsmsgh.h"
#define XSemaphore_i         "xsem.h"
#define XEventSemaphore_i    "xesem.h"
#define XMutexSemaphore_i    "xmusem.h"
#define XSize_i              "xsize.h"
#define XSemaphoreException_i   "xsemex.h"
#define XRexxException_i      "xrexxex.h"
#define XTableException_i      "xtabex.h"
#define XBlitter_i            "xblitter.h"
#define XGameException_i      "xgamexc.h"
#define XPalette_i            "xpal.h"
#define XBlitterWindow_i      "xbltwnd.h"
#define XModelessDialog_i      "xmodlg.h"
#define XModalDialog_i         "xmodal.h"
#define XDBaseFile_i            "xdbasefi.h"
#define XDBaseException_i      "xdbaseex.h"
#define XDBaseRecord_i         "xdbasere.h"
#define XNoteBookEvent_i      "xnbevent.h"
#define XNoteBookHandler_i      "xnbhdlr.h"
#define XMDIServer_i            "xmdiserv.h"
#define XMDIClient_i            "xmdicli.h"
#define XDateEntryField_i      "xdentry.h"
#define XTimeEntryField_i     "xtentry.h"
#define XSocket_i             "xsocket.h"
#define XMessage_i            "xmessage.h"
#define XTrafficLight_i       "xtraffic.h"
#define XLED_i                "xled.h"
#define XCalendarControl_i    "xcal.h"
#define XCDPlayer_i           "xcd.h"
#define XPMThread_i           "xpmthr.h"
#define XMemoryCheck_i          "xmemchek.h"
#define XExitHandler_i        "xexit.h"
#define XCursor_i             "xcursor.h"
#define XLayout_i             "xlayout.h"
#define XDragTransfer_i       "xdragtra.h"
#define XAtom_i               "XAtom.h"
#define XCrypt_i              "xcrypt.h"
#define XResourceWindow_i     "xreswin.h"
#define XWizClientWindow_i    "xcwiz.h"
#define XWizard_i             "XWizard.h"
#define XConsole_i            "xcons.h"
#define XGLibFileDialog_i     "xglfldlg.h"
#define XCoordinateSystem_i   "xcosys.h"
#define XColorControl_i       "xcolcont.h"
#endif