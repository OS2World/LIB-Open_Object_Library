#include "XWindow.h"

#include "XString.h"
#include "XRect.h"
#include "XColor.h"
#include "XHandler.h"
#include "XPoint.h"
#include "xreswin.h"
#include "xapp.h"
#include "xexcept.h"

#include <stdio.h>
#include <stdlib.h>

void BuildChilds(HWND dlgHandle);

/*@ XWindow :: GetSysValue( LONG valueID )
@group misc
@remarks Query a system-value
@parameters LONG id of the value (SV_* see OS/2 documentation)
@returns LONG value
*/
LONG XWindow :: GetSysValue( LONG valueID )
{
   return WinQuerySysValue( HWND_DESKTOP, valueID);
}


/*@
@class XObject
@type overview
@symbol _
@remarks The base of the Open Object Library
*/

/*@
@class XWindow
@type overview
@parent XObject
@symbol _
@remarks XWindow is the base class for several window classes.
*/


/*@ XWindow :: Close()
@group misc
@remarks Close the window
*/
void XWindow :: Close() const
{
   WinSendMsg( winhandle, WM_CLOSE, 0, 0);
}


/*@ XWindow::SetParent(const XWindow * w, const BOOL redraw)
@group misc
@remarks Set a new parent for  this window
@parameters
<t 'ø' c=2>
øXWindow * øthe new parent
øBOOL øredraw the window
</t>
*/
void XWindow::SetParent(const XWindow * w, const BOOL redraw) const
{
   WinSetParent(winhandle, w->GetHandle(), redraw);
}


/*@ XWindow::SetOwner(const XWindow * w)
@group misc
@remarks Set a new owner for  this window
@parameters
<t 'ø' c=2>
øXWindow * øthe new parent
</t>
*/
void XWindow::SetOwner(const XWindow * w) const
{
   WinSetOwner(winhandle, w->GetHandle());
}


/*@ XWindow::SetStyle(const ULONG style)
@group style
@remarks Set the style of a window
@parameters ULONG style    the new syle (depending on the type of window)
*/
void XWindow::SetStyle(const ULONG style)
{
   WinSetWindowULong(winhandle, QWL_STYLE, style);
}


/*@ XWindow::GetStyle(void)
@group style
@remarks Query the current style of the window.
@returns ULONG style
*/
ULONG XWindow::GetStyle(void) const
{
   return WinQueryWindowULong(winhandle, QWL_STYLE);
}


/*@ XWindow::DoControl(XControlEvent * e)
@group user input
@remarks If the user has done some interaction with a window this function is called.
To get information about the event, override this function, the parameter holds
information of your interest.
@parameters   XControlEvent * pointerOfEvent   a pointer to an instance of XControlEvent
*/
void XWindow::DoControl(XControlEvent * e)
{
   XWindow *w = QueryWindow(QW_PARENT);

   if (w)
      w->DoControl(e);
}


/*@ XWindow::SetPointer(const OOL_POINTERHANDLE thePointer)
@group pointer
@remarks Set the actual pointer. You can receive a pointer-handle by calling GetSystemPointer()
or load a pointer from a resource with XResourceLibrary::LoadIcon.
@parameters OOL_POINTERHANDLE handle   the pointer
*/
BOOL XWindow::SetPointer(const OOL_POINTERHANDLE thePointer)
{
   return WinSetPointer(HWND_DESKTOP, thePointer);
}


/*@ XWindow::GetPointerHandle(void)
@group pointer
@remarks Returns the handle of the actual pointer.
@returns OOL_POINTERHANDLE handle
*/
OOL_POINTERHANDLE XWindow::GetPointerHandle(void)
{
   return WinQueryPointer(HWND_DESKTOP);
}


/*@ XWindow::GetSystemPointerHandle(LONG theID, BOOL copy)
@group pointer
@remarks Returns the handle of the pointer identified by theID.
@parameters   <t 'ø' c=2>
               øLONG theID             øID of the needed pointer. Valid values are:
<BR>
PTR_ARROW
<BR>
PTR_TEXT
<BR>
PTR_WAIT
<BR>
PTR_SIZE
<BR>
PTR_SIZENWSE
<BR>
PTR_SIZENESW
<BR>
PTR_SIZEWE
<BR>
PTR_SIZENS
<BR>
PTR_MOVE
<BR>
PTR_ILLEGAL
                  øBOOL copy              øIf you need a copy of the pointer (to modify
                                         it) set copy TRUE, otherwise FALSE.<BR>
                                      Default is FALSE.
               </t>
@returns OOL_POINTERHANDLE handle
*/
OOL_POINTERHANDLE XWindow :: GetSystemPointerHandle( const LONG theID, const BOOL copy)
{
   return WinQuerySysPointer(HWND_DESKTOP, theID, copy);
}


/*@ XWindow::GetPointerPos(XPoint * p)
@group pointer
@remarks Returns the position of the pointer
@parameters XPoint * point    buffer to hold the datas
*/
void XWindow::GetPointerPos(XPoint * p)
{
   POINTL pointl;

   WinQueryPointerPos(HWND_DESKTOP, &pointl);
   p->Set(pointl.x, pointl.y);
}


/*@ XWindow::Activate(void)
@group misc
@remarks Activate the window
*/
void XWindow::Activate(void) const
{
   WinSetActiveWindow(HWND_DESKTOP, winhandle);
}


/*@ XWindow::IsVisible(void)
@group misc
@remarks Query if the window is visible or not
@returns BOOL result
*/
BOOL XWindow::IsVisible(void) const
{
   return WinIsWindowVisible(winhandle);
}


/*@ XWindow::GetSize(XRect * rect)
@group size/position/order
@remarks Query the size and position of the window
@parameters XRect * rect   buffer to hold the datas
*/
void XWindow::GetSize(XRect * rect) const
{
   SWP swp;

   WinQueryWindowPos(winhandle, &swp);
   rect->x = swp.x;
   rect->y = swp.y;
   rect->cx = swp.cx;
   rect->cy = swp.cy;
}


/*@ XWindow::GetWindowID(void)
@group misc
@remarks Query the ID of the window
@returns SHORT theID
*/
SHORT XWindow::GetWindowID(void) const
{
   return WinQueryWindowUShort(winhandle, QWS_ID);
}


/*@ XWindow::QueryWindow(const ULONG id)
@group misc
@remarks Find a window with the given relationship
            ULONG relationship:
                                 <t 'ø' c=1>
                                 øWIN_NEXT
                                 øWIN_PREV
                                 øWIN_TOP
                                 øWIN_BOTTOM
                                 øWIN_OWNER
                                 øWIN_PARENT
                                 øWIN_NEXTTOP
                                 øWIN_PREVTOP
                                 øWIN_FRAMEOWNER
                                 </t>
@returns XWindow * pointer
*/
XWindow * XWindow::QueryWindow(const ULONG id) const
{
   HWND hwnd = WinQueryWindow(winhandle, id);
   return (XWindow *) WinQueryWindowPtr(hwnd, 0);
}


/*@ XWindow::GetWindow(const ULONG id)
@group misc
@remarks Find a child-window with the given ID
@parameters ULONG theID    ID of the window to find
@returns XWindow * pointer
*/
XWindow * XWindow::GetWindow(const ULONG id)
{
   HWND hwnd = WinWindowFromID(GetHandle(), id);

   return (XWindow *) WinQueryWindowPtr(hwnd, 0);
}


/*@ XWindow::SetTop(void)
@group size/position/order
@remarks The window is set to the top in z-order
*/
void XWindow::SetTop(void) const
{
   WinSetWindowPos(winhandle, HWND_TOP, 0, 0, 0, 0, SWP_ZORDER);
}


/*@ XWindow::Enable(const BOOL enable)
@group misc
@remarks Enable/disable the window
@parameters BOOL enabe     TRUE=enable, FALSE=disable
*/
void XWindow::Enable(const BOOL enable)
{
   WinEnableWindow(winhandle, enable);
}


/*@ XWindow::EnableWindowUpdate(const BOOL enable)
@group misc
@remarks Enable/disable window-update
@parameters BOOL enabe     TRUE=enable, FALSE=disable window-update
*/
void XWindow::EnableWindowUpdate(const BOOL enable) const
{
   WinEnableWindowUpdate(winhandle, enable);
}


/*@ XWindow::GetText(XString * buffer)
@group text functions
@remarks Query the text of the window
@parameters XString * buffer     buffer to hold the text
*/
void XWindow::GetText(XString * buffer) const
{
   SHORT r = WinQueryWindowTextLength(winhandle);

   r = WinQueryWindowText(winhandle, r + 1, (PCH) buffer->GetBuffer(r));
   buffer->ReleaseBuffer(r);
}


/*@ XWindow::GetUpdateRect(XRect * r)
@group size
@remarks Get the rectangle which should be redrawn
@parameters XRect * rect      buffer
*/
void XWindow::GetUpdateRect(XRect * r) const
{
   RECTL rect;

   WinQueryUpdateRect(winhandle, &rect);
   r->SetX(rect.xLeft);
   r->SetY(rect.yBottom);
   r->SetWidth(rect.xRight - rect.xLeft);
   r->SetHeight(rect.yTop - rect.yBottom);
}


/*@ XWindow::SetSize(const XRect * rect)
@group size
@remarks Set the size and position of the window
@parameters XRect * rect   new size and position
*/
void XWindow::SetSize(const XRect * rect) const
{
   WinSetWindowPos(winhandle, 0, rect->x, rect->y, rect->cx, rect->cy, SWP_SHOW | SWP_MOVE | SWP_SIZE);
}


/*@ XWindow::SetSize(SetSize( const ULONG xPos, const ULONG yPos, const ULONG width, const ULONG height)
@group size/position/order
@remarks Set the size and/or position of a window
@parameters
<t 'ø' c=2>
øULONG øxPosition
øULONG øyPosition
øULONG øwidth
øULONG øheight
</t>
*/
void XWindow::SetSize(const ULONG xPos, const ULONG yPos, const ULONG width, const ULONG height) const
{
   WinSetWindowPos(winhandle, 0, xPos, yPos, width, height, SWP_SHOW | SWP_MOVE | SWP_SIZE);
}


/*@ XWindow::SetText(const char *text)
@group text functions
@remarks Set the text of the window
@parameters char * text    text to display
*/
void XWindow::SetText(const char *text)
{
   WinSetWindowText(winhandle, (PSZ) text);
}


/*@ XWindow::Show(const BOOL show)
@group size/position/order
@remarks Show/hide the window
@parameters BOOL show            TRUE=show, FALSE=hide
*/
void XWindow::Show(const BOOL show)
{
   if (show)
      WinSetWindowPos(winhandle, HWND_TOP, 0, 0, 0, 0, SWP_SHOW | SWP_ZORDER | SWP_ACTIVATE | SWP_RESTORE);
   else
      WinSetWindowPos(winhandle, 0, 0, 0, 0, 0, SWP_HIDE);
}


/*@ XWindow::SetFocus(void)
@group focus
@remarks Set the focus to this window
@returns BOOL result
*/
BOOL XWindow::SetFocus(void) const
{
   return WinSetFocus(HWND_DESKTOP, winhandle);
}


/*@ XWindow::GetTextLength(void)
@group text functions
@remarks Query the length of the windows text
@returns LONG length
*/
LONG XWindow::GetTextLength(void) const
{
   return WinQueryWindowTextLength(winhandle);
}


/*@ XWindow::Invalidate(BOOL invalidateChilds)
@group misc
@remarks Invalidates the window content, it will be redrawn
@parameters BOOL childs    TRUE=childs of the window are invalidated too
                           FALSE=childs are not invalidated
*/
void XWindow::Invalidate(BOOL invalidateChilds, XRect * rect )
{
   if( rect == NULL)
      WinInvalidateRegion(winhandle, NULLHANDLE, invalidateChilds);
   else
   {
      RECTL r;
      r.xLeft = rect->GetX();
      r.yBottom = rect->GetY();
      r.yTop = r.yBottom + rect->GetHeight();
      r.xRight = r.xLeft + rect->GetWidth();
      WinInvalidateRect(winhandle, &r, invalidateChilds);
   }
}


/*@ XWindow::SetBottom(void)
@group size/position/order
@remarks The window is set to the bottom in z-order
*/
void XWindow::SetBottom(void) const
{
   WinSetWindowPos(winhandle, HWND_BOTTOM, 0, 0, 0, 0, SWP_ZORDER);
}


/*@ XWindow::GetForegroundColor(XColor * rgb)
@group colors
@remarks Query the foreground-color of the window
@parameters XColor * color buffer to hold data
*/
void XWindow::GetForegroundColor(XColor * rgb) const
{
   LONG c;

   WinQueryPresParam(winhandle, PP_FOREGROUNDCOLOR, 0, NULL, 4, &c, QPF_PURERGBCOLOR);
   rgb->SetColor(c);
}


/*@ XWindow::GetBackgroundColor(XColor * rgb)
@group colors
@remarks Query the background-color of the window
@parameters XColor * color buffer to hold data
*/
void XWindow::GetBackgroundColor(XColor * rgb) const
{
   LONG c;

   WinQueryPresParam(winhandle, PP_BACKGROUNDCOLOR, 0, NULL, 4, &c, QPF_PURERGBCOLOR);
   rgb->SetColor(c);
}


/*@ XWindow::SetForegroundColor(const XColor * rgb)
@group colors
@remarks Set the foregrund-color of the window
@parameters XColor * color   the new color
@returns BOOL success
*/
BOOL XWindow::SetForegroundColor(const XColor * rgb) const
{
   LONG c = rgb->GetColor();

   return WinSetPresParam(winhandle, PP_FOREGROUNDCOLOR, 4, (PVOID) (ULONG) & c);
}


/*@ XWindow::SetBackgroundColor(const XColor * rgb)
@group colors
@remarks Set the foregrund-color of the window
@parameters XColor * color   the new color
@returns BOOL success
*/
BOOL XWindow::SetBackgroundColor(const XColor * rgb)
{
   LONG c = rgb->GetColor();

   return WinSetPresParam(winhandle, PP_BACKGROUNDCOLOR, 4, (PVOID) (ULONG) & c);
}


/*@ XWindow::SetFont(const char *fontName, const unsigned short fontSize)
@group fonts
@remarks Set the font of the window
@parameters char * fontName   the name of the font<BR>
            USHORT fontSize   the size of the font
@returns BOOL success
*/
BOOL XWindow::SetFont(const char *fontName, const unsigned short fontSize) const
{
   short l;
   char st[FACESIZE + 3];

   sprintf(st, "%i.%s", fontSize, fontName);
   l = strlen(st);

   return WinSetPresParam(winhandle, PP_FONTNAMESIZE, l, (PVOID) st);
}


/*@ XWindow::SetFont(const char *fontNameSize)
@group fonts
@remarks Set the font of the window
@parameters char * fontName   the name of the font in format "10.Helvetica"
@returns BOOL success
*/
BOOL XWindow::SetFont(const char *fontNameSize) const
{
   return WinSetPresParam(winhandle, PP_FONTNAMESIZE, strlen(fontNameSize), (PVOID) fontNameSize);
}


/*@ XWindow :: ~XWindow()
@group constructors/destructors
@remarks Destructors of windows are called automaticaly when they are closed. All destructors
of child-windows are called too. All handlers registered at the window aredestructed.
You can destroy a window by calling the destructor.
*/
XWindow :: ~XWindow()
{
   SHORT i;
   WinSetWindowPtr(winhandle, 0, NULL);
   WinDestroyWindow(winhandle);
   for (i = 0; i < handlers; i++)
   {
      regHandlers[i]->handleFor = NULL;
      delete regHandlers[i];
   }
   handlers = 0;
   free(regHandlers);
}


BOOL XWindow::IsEnabled(void) const
{
   return WinIsWindowEnabled(winhandle);
}


/*@ XWindow::GetFontName(XString * font)
@group fonts
@remarks Query the font of the window
@parameters XString * buffer   the name of the font in format "10.Helvetica"
*/
void XWindow::GetFontName(XString * font) const
{
   WinQueryPresParam(winhandle, PP_FONTNAMESIZE, 0, NULL, 100, font->GetBuffer(100), QPF_NOINHERIT);
   font->ReleaseBuffer();
}


/*@ XWindow::DoCommand()
@group user input
@remarks If the user selected a XPopupMenu or if the
user pressed a button which is attached to a window, this function
is called. To get the ID of the command the user requested, override this function.
Return TRUE if you have handled the command, otherwise return FALSE. If you return FALSE
the command is posted to the owner of this window.
@parameters   LONG theCommandID    the ID of the menuitem/toolbar-button
@returns      BOOL handled
*/


/*@ XWindow::DoSize()
@group size/position/order
@remarks DoSize informs the application if the window is resized. Override
this function if you need this information.
@parameters XSize * size
*/


/*@ XWindow::DoMove()
@group size/position/order
@remarks DoMove informs the application if the window is moved. Override
this function if you need this information.
*/


/*@ XWindow::FocusChanged()
@group focus
@remarks Informs the window if the focus a moved
Override
this function if you need this information.
@parameters BOOL focus is received
*/

/*@ XWindow::IsVisible()
@group size/position/order
@remarks Query if the window is visible
@returns BOOL
*/

/*@ XWindow::DoSysCommand()
@group user input
@remarks If the user selected a command from the sytem-menu or chosed a button
of the titlebar a syscommand is send to the window. Return TRUE if the action can be performed
by the OS, otherwise return FALSE.
@parameters   USHORT theCommandID    SC_CLOSE, SC_RESTORE etc, see OS/2 API documentation
@returns      BOOL handled
*/


MRESULT EXPENTRY RProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   XWindow *w = (XWindow *) WinQueryWindowPtr(hwnd, 0);

   if (w)
   {
      if(msg == WM_COMMAND )
      {
         if( w->DoCommand( SHORT1FROMMP(mp1) ) == TRUE)
         {
            WinSetWindowULong(hwnd, 0, 0);
            return WinDefDlgProc(hwnd, msg, mp1, mp2);
         }
         else
            return (MRESULT) FALSE;
      }
      else if(msg == WM_MOVE)
      {
         MRESULT mr = WinDefDlgProc(hwnd, msg, mp1, mp2);
         w->DoMove();
         return mr;
      }
      else if(msg == WM_ADJUSTWINDOWPOS )
      {
         MRESULT mr = WinDefDlgProc(hwnd, msg, mp1, mp2);
         PSWP pswp = (PSWP) mp1;
         XSize size( pswp->cx, pswp->cy);
         w->DoSize(&size);
         return mr;
      } /* end if */

      BOOL handeld = FALSE;
      MRESULT mr = HandleDefault(w, msg, mp1, mp2, handeld);

      if (handeld)
         return mr;
   }
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


/*@
@class XResourceWindow
@parent XWindow
@type overview
@symbol _
@remarks XResourceWindow is a window class which is loaded from the resources. Usualy you
create a dialog-template with your dialog editor, disable the styles for titlebar and frame.
Then create one or more XResourceWindows with a dialog or framewindow as owner.
*/


/*@ XResourceWindow :: XResourceWindow(XWindow * w, ULONG id)
@group constructor/destructor
@remarks Create a resource-window
@parameters
<t 'ø' c=2>
øXWindow *  øowner
øULONG øID of the window-resource (the resource must be linked to the exe-file)
</t>
*/
XResourceWindow :: XResourceWindow(XWindow * w, ULONG id)
{
   CreateWindow( w, id, NULL);
}


/*@ XResourceWindow :: XResourceWindow(XWindow * w, XResource * r)
@group constructor/destructor
@remarks Create a resource-window
@parameters
<t 'ø' c=2>
øXWindow *  øowner
øXResource * øresource where to load the window from
</t>
*/
XResourceWindow :: XResourceWindow(XWindow * w, XResource * r)
{
   CreateWindow( w, r->GetID(), r->GetResourceLibrary());
}


void XResourceWindow :: CreateWindow(XWindow * w, ULONG id, XResourceLibrary* lib)
{
   if(lib)
      winhandle = WinLoadDlg(w->GetHandle(), w->GetHandle(), (PFNWP) RProc, lib->GetModuleHandle(), id, NULL);
   else
      winhandle = WinLoadDlg(w->GetHandle(), w->GetHandle(), (PFNWP) RProc, XApplication::GetApplication()->GetResourceLibrary()->GetModuleHandle(), id, NULL);
   if (winhandle == 0)
   {
      OOLThrow("could not create window from resources!", 0);
      return;
   }
   WinSetWindowPtr(winhandle, 0, this);
   BuildChilds(winhandle);
}

