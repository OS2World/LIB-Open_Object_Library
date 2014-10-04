#include "XControl.h"
#include "XRect.h"
#include "XPushBtn.h"
#include "XDragHdl.h"
#include "XDragEvn.h"
#include "XMousHdl.h"
#include "XMousEvn.h"
#include "XFrame.h"
#include "XNoteBk.h"
#include "xcolor.h"
#include "xmsgbox.h"
#include "xexcept.h"
#include "XFrmWnd.h"
#include "XObjWnd.h"
#include "xrect.h"
#include "XColor.h"
#include "xsize.h"
#include "xcolcont.h"
#include "xdefhdl.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


MRESULT HandleDefault(XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL & handled);


MRESULT EXPENTRY ControlProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   XControl *w = (XControl *) WinQueryWindowULong(hwnd, QWL_USER);
   if(msg == WM_CLOSE)
      return 0;

   if (msg == WM_QUERYFRAMECTLCOUNT || msg == WM_FORMATFRAME || msg == WM_SYSCOMMAND)
      return ((XFrame *) w)->HandleMessage(msg, mp1, mp2);

   if (w)
   {
      if ( msg == WM_FOCUSCHANGE )
      {
         MRESULT res = w->oldfunc(hwnd, msg, mp1, mp2);
         w->FocusChanged( SHORT1FROMMP(mp2));
         return res;
      }
      BOOL handled = FALSE;
/*****************
      if( msg == WM_SIZE )
      {
         MRESULT r = w->oldfunc(hwnd, msg, mp1, mp2);
           XSize s(SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
         w->DoSize( &s);
         return r;
      }
********************/
      MRESULT mr = HandleDefault(w, msg, mp1, mp2, handled);

      if (handled)
         return mr;

      return w->oldfunc(hwnd, msg, mp1, mp2);
   }
   else
      return WinDefWindowProc(hwnd, msg, mp1, mp2);
}


/*@
@class XControl
@parent XWindow
@type overview
@symbol _
@remarks XControl is the basic class for user input/output windows. You cannot create
a XControl directly.
*/


XControl :: XControl(const HWND hwnd)
{
   oldfunc = NULL;
   winhandle = hwnd;
   if (hwnd)
   {
      WinSetWindowULong(winhandle, QWL_USER, (ULONG) this);
      oldfunc = WinSubclassWindow(winhandle, (PFNWP) ControlProc);
   }
}


XControl :: XControl(const XRect * rec, const ULONG style, const XWindow * ow, const char *string, const PSZ cl, const USHORT id, const char *font)
{
   PRES pres;
   HWND hwnd = 0;
   char *s = (char *) string;

   if (ow)
      hwnd = ow->GetHandle();
   ULONG st = style;
   void *p = NULL;

   if ((cl == WC_BUTTON && (style & BS_BITMAP || style & BS_ICON || style & BS_MINIICON)) || font)
   {
      pres.fcparam.id = PP_FOREGROUNDCOLORINDEX;
      pres.fcparam.cb = sizeof(pres.fcparam.ulColor);
      pres.fcparam.ulColor = 16;
      pres.fntparam.id = PP_FONTNAMESIZE;
      pres.fntparam.cb = 20;
      pres.cb = sizeof(pres.fcparam) + sizeof(pres.fntparam);
      p = (void *) &pres;

      if (!font)
      {
         strcpy(pres.fntparam.szFontNameSize, "7.Helv");
         char ti[50];

         if (style & BS_TEXT)
            sprintf(ti, "#%i\t%s", id, string);
         else
            sprintf(ti, "#%i", id);
         s = ti;
      }
      else
         strcpy(pres.fntparam.szFontNameSize, font);
   }
   if (cl == WC_BUTTON && st & BS_USERBUTTON && st & 0x8000)
      st ^= 0x8000;

   if (rec == NULL)
      rec = new XRect();
   winhandle = WinCreateWindow(hwnd, cl, (PSZ) s, st, rec->x, rec->y, rec->cx, rec->cy, hwnd, HWND_TOP, id, NULL, p);

   if (winhandle == 0)
      OOLThrow("error creating control", -10);

   WinSetWindowULong(winhandle, QWL_USER, (ULONG) this);
   oldfunc = WinSubclassWindow(winhandle, (PFNWP) ControlProc);
   if (ow && cl != WC_SCROLLBAR)
      WinPostMsg(hwnd, OOL_ADDCLIENT, (MPARAM) winhandle, 0);
}


XObjectWindow :: XObjectWindow(const XFrameWindow * win)
{
   owner = (XFrameWindow *) win;

   if (WinRegisterClass( WinQueryAnchorBlock(win->frame), (PSZ) "OOL_OBJECTWND", (PFNWP) WindowProc, 0, 4) == FALSE)
      OOLThrow("error registering objectwindow-class", -10);

   winhandle = WinCreateWindow(HWND_OBJECT, (PSZ) "OOL_OBJECTWND", (PSZ) "", 0, 0, 0, 0, 0, HWND_OBJECT, HWND_BOTTOM, 0, this, 0);
}


/*@
@class XColorControl
@parent XControl
@type overview
@symbol _
@remarks XColorControl is a color selection control. Only use one control of this type
in a window. The height of the control should be aprox. 60% of its width.
*/

class ColorDefaultHandler: public XDefaultHandler
{
   public:
      ColorDefaultHandler(XWindow * w): XDefaultHandler(w) {;}
      BOOL HandleEvent( ULONG msg, void * mp1, void *, LONG * retVal);
};


/*@ XColorControl :: GetColor( XColor* color)
@group set/get the color
@remarks Get the color
@parameters XColor* buffer for the data
*/



/*@ XColorControl :: ColorSelected( const XColor& newColor)
@group set/get the color
@remarks Override this method if you need information when the user selected a new color
@parameters XColor& the new color
*/


/*@ XColorControl :: SetColor( XColor& color)
@group set/get the color
@remarks Set the color
@parameters XColor& the pre-selected color
*/
void XColorControl :: SetColor( const XColor& color) const
{
   WinSendMsg( winhandle, 0x0602, (MPARAM) color.GetColor(), 0);
}


MRESULT EXPENTRY ColorControlProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   XControl *w = (XControl *) WinQueryWindowULong(hwnd, QWL_USER+4);
   if(msg == WM_CLOSE)
      return 0;
   if (w)
   {
      return w->oldfunc(hwnd, msg, mp1, mp2);
   }
   else
      return WinDefWindowProc(hwnd, msg, mp1, mp2);
}


/*@ XColorControl :: XColorControl( XWindow * owner, XRect& rect)
@group constructor
@remarks Create the control
@parameters
<t 'ø' c=2>
øXWindow*   øowner window
øXRect&     øposition/size
</t>
*/
XColorControl :: XColorControl( const XWindow * owner, const XRect& rect):lib("WPCONFIG.DLL"), XControl(0)
{
   winhandle = WinCreateWindow( owner->GetHandle(), "ColorSelectClass", "", WIN_VISIBLE, rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight(), owner->GetHandle(), HWND_TOP, 0, NULL, NULL);
   if(winhandle == 0)
   {
      OOLThrow( "error creating XColorControl", -1);
   }
   WinSetWindowULong(winhandle, QWL_USER+4, (ULONG) this);
   oldfunc = WinSubclassWindow(winhandle, (PFNWP) ColorControlProc);
}
