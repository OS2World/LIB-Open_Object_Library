#include "XScrlWnd.h"
#include "xrect.h"
#include "XScrlBar.h"
#include "XFrame.h"
#include "XDefhdl.h"
#include "xexcept.h"

class scrollhandler:public XDefaultHandler
{
   XScrollWindow *win;
   public:
      scrollhandler(XScrollWindow * w):XDefaultHandler(w) {win = w;}
      BOOL HandleEvent(ULONG, void *, void *, LONG*);
};


/*@
@class XScrollWindow
@parent XFrameWindow
@type overview
@symbol _
*/


/*@ XScrollWindow :: XScrollWindow(const XResource * resource, const char *title, const ULONG style, const XRect * rec, const XFrameWindow * parent, const BOOL build, const BOOL animate)
@group constructors/destructors
@remarks Constructs a frame-window which does scrolling automaticaly.
Note that destructors of windows are called automaticaly when a window is closed! (see ~XFrameWindow)
@parameters <t 'ø' c=2>
            øXResource * theResourceID   øa XResource contains two informations, an ID and a pointer
                                        to a XResourceLibrary. If you want to create a window out of
                                        a resourcefile you must specify the ID (otherwise it can be zero)
                                        and the XResourceLibrary which contains the window-resource.
                                        The window which is created always belongs to the process who
                                        owns the resource library, so if you work with multiple processes
                                        every process must have its own resource library.
            øchar * theTitle             øThe title of the window which is displayed in the titlebar
            øULONG theStyleofWindow      øYou can specify the style of the window with the following defines,
                                        which can be or-ed:
<BR>
FRM_TITLEBAR the window gets a titlebar
<BR>
FRM_SYSMENU the window gets the system menu
<BR>
FRM_MINBUTTON the titlebar get a button to minimize the window
<BR>
                                           FRM_MAXBUTTON the titlebar get a button to maximize the window
<BR>
                                           FRM_CENTER the window is created in the midle of the workplace
<BR>
                                           FRM_SIZEBORDER the windowsize can be changed by the user
<BR>
                                           FRM_DIALOGBORDER the window gets a thick border
<BR>
                                           FRM_BORDER the window gets a thin border
<BR>
                                           FRM_TASKLIST the window is displayed in the tasklist
<BR>
                                           FRM_NOMOVEWITHOWNER the window dontït move when the parent is moved
<BR>
                                           FRM_ICON  the window get an icon wich is identified by theResourceID,                                                            if the icon is not found in the resource-library, an error ocurses
<BR>
                                           FRM_ACCELTABLE an acceltable will be loaded from the resources with the windows id.
<BR>
                                           FRM_SYSMODAL the window is displayed system-modal
<BR>
FRM_SCREENALIGN
<BR>
FRM_MOUSEALIGN
<BR>
FRM_HIDEBUTTON
<BR>
FRM_HIDEMAX
<BR>
FRM_AUTOICON
<BR>
there are three static member-variables for default styles
<BR>
defaultStyle default setting for a framewindow
<BR>
defaultClientStyle default setting for windows wich are displayed as a clientwindow of a framewindow
<BR>
defaultDialogStyle default setting for windows wich are displayed as a dialog
<BR>
                                        Default is defaultStyle.
            øXRect * theRectangle        øOn default a window is created with length and hight of zero. Windows
                                        which are created with an resource template get the size of the template.
                                        Default is NULL.<BR>
                                        If theRectangle is specified, the window gets the size of it.
            øXFrameWindow * parent       øIf parent is specified the window is a client of the parent. The
                                        behavior depends on the styles you have set.<BR>
                                        Default is NULL.
            øBOOL buildFromResource      øIf this variable is set OOL try to build the window with a resource
                                        template which is identified by theResourceID. If the template is
                                        not found, an error ocurses.<BR>
                                        Default is FALSE.
            øBOOL animate                øEnable/disable animation on window creation. Default is FALSE
         </t>
*/
XScrollWindow :: XScrollWindow(const XResource * resource, const char *title, const ULONG style, const XRect& rec, const XFrameWindow * parent, const BOOL build, const BOOL animate):XFrameWindow(resource, title, style, rec, parent, build, animate)
{
   Setup();
}


/*@ XScrollWindow :: XScrollWindow(const XResource * resource, const char *title, const ULONG style, const XRect * rec, const XFrameWindow * parent, const BOOL build, const BOOL animate)
@group constructors/destructors
@remarks Constructs a frame-window which does scrolling automaticaly.
Note that destructors of windows are called automaticaly when a window is closed! (see ~XFrameWindow)
@parameters <t 'ø' c=2>
            øULONG   øID of the window. If resources are used they must be linked to the exe-file.
            øchar *              øThe title of the window which is displayed in the titlebar
            øULONG    øYou can specify the style of the window with the following defines,
                                        which can be or-ed:
<BR>
FRM_TITLEBAR the window gets a titlebar
<BR>
FRM_SYSMENU the window gets the system menu
<BR>
FRM_MINBUTTON the titlebar get a button to minimize the window
<BR>
                                           FRM_MAXBUTTON the titlebar get a button to maximize the window
<BR>
                                           FRM_CENTER the window is created in the midle of the workplace
<BR>
                                           FRM_SIZEBORDER the windowsize can be changed by the user
<BR>
                                           FRM_DIALOGBORDER the window gets a thick border
<BR>
                                           FRM_BORDER the window gets a thin border
<BR>
                                           FRM_TASKLIST the window is displayed in the tasklist
<BR>
                                           FRM_NOMOVEWITHOWNER the window dontït move when the parent is moved
<BR>
                                           FRM_ICON  the window get an icon wich is identified by theResourceID,                                                            if the icon is not found in the resource-library, an error ocurses
<BR>
                                           FRM_ACCELTABLE an acceltable will be loaded from the resources with the windows id.
<BR>
                                           FRM_SYSMODAL the window is displayed system-modal
<BR>
FRM_SCREENALIGN
<BR>
FRM_MOUSEALIGN
<BR>
FRM_HIDEBUTTON
<BR>
FRM_HIDEMAX
<BR>
FRM_AUTOICON
<BR>
there are three static member-variables for default styles
<BR>
defaultStyle default setting for a framewindow
<BR>
defaultClientStyle default setting for windows wich are displayed as a clientwindow of a framewindow
<BR>
defaultDialogStyle default setting for windows wich are displayed as a dialog
<BR>
                                        Default is defaultStyle.
            øXRect *            øOn default a window is created with length and hight of zero. Windows
                                        which are created with an resource template get the size of the template.
                                        Default is NULL.<BR>
                                        If theRectangle is specified, the window gets the size of it.
            øXFrameWindow *        øParent-window, if parent is specified the window is a client of the parent. The
                                        behavior depends on the styles you have set.<BR>
                                        Default is NULL.
            øBOOL buildFromResource      øIf this variable is set OOL try to build the window with a resource
                                        template which is identified by theResourceID. If the template is
                                        not found, an error ocurses.<BR>
                                        Default is FALSE.
            øBOOL animate                øEnable/disable animation on window creation. Default is FALSE
         </t>
*/
XScrollWindow :: XScrollWindow(const ULONG id, const char *title, const ULONG style, const XRect& rec, const XFrameWindow * parent, const BOOL build, const BOOL animate):XFrameWindow(id, title, style, rec, parent, build, animate)
{
   Setup();
}


void XScrollWindow :: Setup()
{
   scrollEnabled = TRUE;

   scrollhandler *handler = new scrollhandler(this);

   scrollPosX = scrollPosY = 0;
   xStep = yStep = 20;
   xPage = yPage = 60;

   if (dlgHandle)
   {
      SWP swp;

      WinQueryWindowPos(dlgHandle, &swp);
      SetVirtualX(swp.cx);
      SetVirtualY(swp.cy);
   }
   else
   {
      SetVirtualX(0);
      SetVirtualY(0);
   }
}


/*@ XScrollWindow::SetVirtualX(const LONG x)
@group scroll-functions
@remarks Set the virtual x-size of the window
@parameters LONG x                       virtual x-size
*/
void XScrollWindow::SetVirtualX(const LONG x)
{
   virtualX = x;
   CalcHorzSize();
}


/*@ XScrollWindow::SetVirtualY(const LONG y)
@group scroll-functions
@remarks Set the virtual y-size of the window
@parameters LONG y                       virtual y-size
*/
void XScrollWindow::SetVirtualY(const LONG y)
{
   virtualY = y;
   CalcVertSize();
}


/*@ XScrollWindow::VScroll(LONG s)
@group scroll-functions
@remarks This function is called when the window-contents must be scrolled vertcal.
On default scrolling is done automaticaly
@parameters LONG s    how much pixels to scroll
*/
void XScrollWindow::VScroll(LONG s)
{
   if(scrollEnabled)
   {
      SWP swp;
      HWND cl = winhandle, hwnd;
      if (dlgHandle)
         cl = dlgHandle;

      HENUM enumWindow = WinBeginEnumWindows(cl);

      WinEnableWindowUpdate(cl, FALSE);
      char buffer[5];
      SHORT cor;
      ULONG style;

      while ((hwnd = WinGetNextWindow(enumWindow)) != 0)
      {
         WinQueryClassName(hwnd, 5, (PSZ) buffer);
         style = WinQueryWindowULong(hwnd, QWL_STYLE);
         if (style & ES_MARGIN && buffer[1] == '6')
            cor = 3;
         else
            cor = 0;
         WinQueryWindowPos(hwnd, &swp);
         swp.y -= s; //-
         WinSetWindowPos(hwnd, 0, swp.x + cor, swp.y + cor, 0, 0, SWP_MOVE);
      };
      WinEndEnumWindows(enumWindow);
      WinEnableWindowUpdate(cl, TRUE);
   }
   scrollPosY += s; //+
}


/*@ XScrollWindow::HScroll(LONG s)
@group scroll-functions
@remarks This function is called when the window-contents must be scrolled horizontal.
On default scrolling is done automaticaly
@parameters LONG s   how much pixels to scroll
*/
void XScrollWindow::HScroll(LONG s)
{
   if(scrollEnabled)
   {
   SWP swp;
   HWND cl = winhandle, hwnd;

   if (dlgHandle)
      cl = dlgHandle;

   HENUM enumWindow = WinBeginEnumWindows(cl);

   if (scrollPosX + s > virtualX)
      s = virtualX - scrollPosX;
   if (scrollPosX + s < 0)
      s = -scrollPosX;

   WinEnableWindowUpdate(cl, FALSE);

   char buffer[5];
   SHORT cor;
   ULONG style;

   while ((hwnd = WinGetNextWindow(enumWindow)) != 0)
   {
      WinQueryClassName(hwnd, 5, (PSZ) buffer);
      style = WinQueryWindowULong(hwnd, QWL_STYLE);
      if (style & ES_MARGIN && buffer[1] == '6')
         cor = 3;
      else
         cor = 0;

      WinQueryWindowPos(hwnd, &swp);
      swp.x -= s;
      WinSetWindowPos(hwnd, 0, swp.x + cor, swp.y + cor, 0, 0, SWP_MOVE);
   };
   WinEndEnumWindows(enumWindow);

   WinEnableWindowUpdate(cl, TRUE);
   }
   scrollPosX += s;
}


BOOL scrollhandler::HandleEvent(ULONG msg, MPARAM mp1, MPARAM mp2, LONG * retVal)
{
   SHORT s = 0;
   *retVal = TRUE;

   switch (msg)
   {
   case OOL_ADDCLIENT:
      SWP swp;
      WinQueryWindowPos((HWND) mp1, &swp);
      if (swp.cx + swp.x > win->virtualX)
      {
         win->virtualX = swp.x + swp.cx + 5;
         win->CalcHorzSize();
      }
      if (swp.cy + swp.y > win->virtualY)
      {
         win->virtualY = swp.y + swp.cy + 5;
         win->CalcVertSize();
      }
      return TRUE;
   case WM_HSCROLL:
      {
         switch (SHORT2FROMMP(mp2))
         {
         case SB_LINERIGHT:
            s = win->xStep;
            break;
         case SB_LINELEFT:
            s = -win->xStep;
            break;
         case SB_PAGELEFT:
            s = -win->xPage;
            break;
         case SB_PAGERIGHT:
            s = win->xPage;
            break;
         case SB_ENDSCROLL:
            return 0;
         default:
            s = SHORT1FROMMP(mp2) - win->scrollPosX;
         }
         win->HScroll(s);
         if (win->horz)
            win->horz->SetPos(win->scrollPosX);
      }
      return TRUE;
   case WM_VSCROLL:
      {
         switch (SHORT2FROMMP(mp2))
         {
         case SB_LINEUP:
            s = -win->yStep;
            break;
         case SB_LINEDOWN:
            s = win->yStep;
            break;
         case SB_PAGEDOWN:
            s = win->yPage;
            break;
         case SB_PAGEUP:
            s = -win->yPage;
            break;
         case SB_ENDSCROLL:
            return 0;
         default:
            s = SHORT1FROMMP(mp2) - win->scrollPosY;
         }
         win->VScroll(s);
         if (win->vert)
            win->vert->SetPos( win->scrollPosY);
      }
      return TRUE;
   }
   *retVal = FALSE;
   return FALSE;
}


void XScrollWindow::SetXScrollPos( const USHORT s)
{
   scrollPosX = s;
   if(horz)
      horz->SetPos( scrollPosX);
}


void XScrollWindow::SetYScrollPos( const USHORT s)
{
   scrollPosY = s;
   if(vert)
      vert->SetPos( scrollPosY);
}


void XScrollWindow::CalcHorzSize(void)
{
   if (horz)
   {
       if( xPage > virtualX)
         HScroll( -scrollPosX);
      horz->SetSliderSize(xPage, virtualX);
      horz->SetRange(1, virtualX - xPage, scrollPosX);
   }
}


/*@ XScrollWindow::AddHorzScroller(void)
@group scrollbars
@remarks Display a horizontal scroller
*/
void XScrollWindow::AddHorzScroller(void)
{
   if (horz != NULL)
      return;
   scrollPosX = 0;

   if (frameWin)
      frameWin->adds += 1;

   XRect r;

   horz = new XScrollBar(this, r, 0, SC_HORZ | WIN_VISIBLE);
   WinSetParent(horz->winhandle, frame, FALSE);
   WinSetOwner(horz->winhandle, frame);

   if (vert && dummy == 0)
      AddDummy();

   WinSendMsg(frame, WM_UPDATEFRAME, (MPARAM) FCF_HORZSCROLL, 0);
   CalcHorzSize();
}


void XScrollWindow::CalcVertSize(void)
{
   if (vert)
   {
       if( yPage > virtualY )
         VScroll( -scrollPosY);
      vert->SetSliderSize(yPage, virtualY);
      vert->SetRange(1, virtualY - yPage, scrollPosY);
   }
}

/*@ XScrollWindow::AddVertScroller(void)
@group scrollbars
@remarks Display a vertical scroller
*/
void XScrollWindow::AddVertScroller(void)
{
   if (vert != NULL)
      return;

   scrollPosY = 0;

   if (frameWin)
      frameWin->adds += 1;

   XRect r;

   vert = new XScrollBar(this, r, 0, SC_VERT | WIN_VISIBLE | SC_AUTO);
   WinSetParent(vert->winhandle, frame, FALSE);
   WinSetOwner(vert->winhandle, frame);

   if (horz && dummy == 0)
      AddDummy();

   WinSendMsg(frame, WM_UPDATEFRAME, (MPARAM) FCF_VERTSCROLL, 0);

   CalcVertSize();
}


/*@ XScrollWindow::DeleteVertScroller(void)
@group scrollbars
@remarks Removes the vertical scroller
*/
void XScrollWindow::DeleteVertScroller(void)
{
   if (vert == NULL)
      return;
   if (frameWin)
      frameWin->adds -= 1;

   WinDestroyWindow(vert->winhandle);
   delete vert;

   vert = NULL;
   scrollPosY = 0;
   if (dummy)
      DeleteDummy();
   WinSendMsg(frame, WM_UPDATEFRAME, (MPARAM) FCF_VERTSCROLL, 0);
   CalcHorzSize();
}


/*@ XScrollWindow::DeleteHorzScroller(void)
@group scrollbars
@remarks Removes the horizontal scroller
*/
void XScrollWindow::DeleteHorzScroller(void)
{
   if (horz == NULL)
      return;
   if (frameWin)
      frameWin->adds -= 1;
   WinDestroyWindow(horz->winhandle);
   delete horz;

   scrollPosX = 0;
   horz = NULL;
   if (dummy)
      DeleteDummy();
   WinSendMsg(frame, WM_UPDATEFRAME, (MPARAM) FCF_HORZSCROLL, 0);
   CalcVertSize();
}


MRESULT EXPENTRY dummyHandler(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   if (msg == WM_PAINT)
   {
      RECTL rec;
      HPS hps = WinBeginPaint(hwnd, NULLHANDLE, &rec);

      WinFillRect(hps, &rec, CLR_PALEGRAY);
      WinEndPaint(hps);
   }
   return WinDefWindowProc(hwnd, msg, mp1, mp2);
}


void XScrollWindow::AddDummy(void)
{
   if (WinRegisterClass(WinQueryAnchorBlock(frame), (PSZ) "OOL_DUMMY", (PFNWP) dummyHandler, 0, 0) == FALSE)
      OOLThrow("error registering internal class", -10);

   if (frameWin)
      frameWin->adds += 1;

   dummy = WinCreateWindow(frame, (PSZ) "OOL_DUMMY", (PSZ) "", WS_VISIBLE,
                     0, 0, 0, 0,
                     frame, HWND_TOP, 0, 0, 0);
}


void XScrollWindow::DeleteDummy(void)
{
   if (dummy == 0)
      return;

   if (frameWin)
      frameWin->adds -= 1;
   WinDestroyWindow(dummy);
   dummy = 0;
}


XScrollWindow :: ~XScrollWindow()
{
   DeleteHorzScroller();
   DeleteVertScroller();
}

