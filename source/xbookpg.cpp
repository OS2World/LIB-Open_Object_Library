#include "XNoteBk.h"
#include "XBookPg.h"
#include "xcntevnt.h"
#include "xcolor.h"
#include "xexcept.h"
#include "xres.h"
#include "xreslib.h"
#include "xbitmap.h"
#include "xapp.h"

MRESULT HandleDefault(XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL & handled);
void BuildChilds(HWND dlgHandle);


MRESULT EXPENTRY bookProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   XNoteBookPage *w = (XNoteBookPage *) WinQueryWindowPtr(hwnd, 0);

   if (w)
   {
      if(msg == WM_CLOSE)
      {
         WinSetWindowPtr( hwnd, 0, 0);
         delete w;
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
      }
      BOOL handeld = FALSE;
      MRESULT mr = HandleDefault(w, msg, mp1, mp2, handeld);

      if (handeld)
         return mr;
   }
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


/*@
@class XNoteBookPage
@parent XWindow
@type overview
@symbol _
@changed
*/

/*@
@class XNoteBookPage
@type overview
@symbol _
@remarks XNoteBookPage is a page of XNoteBook. You can add one ore more pages to
a notebook. The behaviour of a XNoteBookPage is like any other window so you can use
all functions like DoCommand, DoControl etc.
*/


/*@ XNoteBookPage :: GetBackgroundColor ( XColor * col )
@group colors
@remarks Returns the background color.
@parameters XColor * buffer
*/
void XNoteBookPage::GetBackgroundColor(XColor * col) const
{
   owner->GetBackgroundColor(col);
}


/*@ XNoteBookPage :: XNoteBookPage(const XNoteBook * o, const XResource * r, const USHORT style, const USHORT order, const char *title, const XNoteBookPage * insertBehind)
@group contructors/destructors
@remarks Construct a notebook-page and adds it to a given notebook
@parameters <t 'ø' c=2>
            øXNoteBook * notebook       ønotebook which gets the page
            øXResource * resource       øGive here a XResource which contains the id of the dialog to load and the resourcelibrary
                                       where to load the dialog from. If you have created one or more pages from resources, you
                                       should resize the notebook.
            øUSHORT style               østyle of the page:
<BR>
BP_MAJORTAB
<BR>
BP_MINORTAB
<BR>
BP_PAGEBUTTON
<BR>
BP_STATUSTEXTON
<BR>
BP_MAJOR
<BR>
BP_MINOR
<BR>
(can be or-ed, default is BP_MAJOR|BP_STATUSTEXTON)
            øUSHORT order               øwhere to insert:
<BR>
BP_LAST
<BR>
BP_FIRST
<BR>
BP_NEXT
<BR>
BP_PREV
<BR>
BP_TOP
<BR>
                                       if BP_LAST or BP_FIRST, insertBehind is ignored (default BP_LAST)
            øXNoteBookPage * insertBehind   øinsert behind this page (default is NULL)
            </t>
@exceptions   If the method fails an exception of the type XException is thrown.
*/
XNoteBookPage :: XNoteBookPage(const XNoteBook * o, const XResource * r, const USHORT style, const USHORT order, const char *title, const XNoteBookPage * insertBehind)
{
   CreateBookPage(o, r->GetID(), style, order, title, insertBehind, r->GetResourceLibrary(), TRUE);
}


/*@ XNoteBookPage :: XNoteBookPage(const XNoteBook * o, const ULONG pageID, const USHORT style, const USHORT order, const char *title, const XNoteBookPage * insertBehind, BOOL build)
@group contructors/destructors
@remarks Construct a notebook-page and adds it to a given notebook
@parameters <t 'ø' c=2>
            øXNoteBook * notebook       ønotebook which gets the page
            øULONG                      øpage-id
            øUSHORT style               østyle of the page:
<BR>
BP_MAJORTAB
<BR>
BP_MINORTAB
<BR>
BP_PAGEBUTTON
<BR>
BP_STATUSTEXTON
<BR>
BP_MAJOR
<BR>
BP_MINOR
<BR>
(can be or-ed, default is BP_MAJOR|BP_STATUSTEXTON)
            øUSHORT order               øwhere to insert:
<BR>
BP_LAST
<BR>
BP_FIRST
<BR>
BP_NEXT
<BR>
BP_PREV
<BR>
BP_TOP
<BR>
                                       if BP_LAST or BP_FIRST, insertBehind is ignored (default BP_LAST)
            øXNoteBookPage * insertBehind   øinsert behind this page (default is NULL)
            øBOOL                     øTRUE=build from resource-dialog<BR>FALSE=donït build<BR>If TRUE the dialog-resource must be linked to the exe-file and have the id given in pageID. (default is FALSE)
            </t>
@exceptions   If the method fails an exception of the type XException is thrown.
*/
XNoteBookPage :: XNoteBookPage(const XNoteBook * o, const ULONG pageID, const USHORT style, const USHORT order, const char *title, const XNoteBookPage * insertBehind, BOOL build)
{
   CreateBookPage(o, pageID, style, order, title, insertBehind, NULL, build);
}


void XNoteBookPage :: CreateBookPage(const XNoteBook * o, const ULONG pageID, const USHORT style, const USHORT order, const char *title, const XNoteBookPage * insertBehind, const XResourceLibrary * res, BOOL build)
{
   if (build)
   {
      XColor col(COL_PALEGRAY);

      SetBackgroundColor(&col);
      if( res )
         winhandle = WinLoadDlg(o->GetHandle(), o->GetHandle(), (PFNWP) bookProc, res->GetModuleHandle(), pageID, NULL);
      else
         winhandle = WinLoadDlg(o->GetHandle(), o->GetHandle(), (PFNWP) bookProc, XApplication::GetApplication()->GetResourceLibrary()->GetModuleHandle(), pageID, NULL);
      if (winhandle == 0)
         OOLThrow("error loading notebook-template", -10);

      WinSetWindowPtr(winhandle, 0, this);
      BuildChilds(winhandle);
   }
   else
   {
      if (WinRegisterClass(WinQueryAnchorBlock(o->GetHandle()), (PSZ) "OOL_BOOKPAGE", (PFNWP) userProc, 0, 4) == FALSE)
         OOLThrow("error registering notebook-class", -10);

      winhandle = WinCreateWindow(o->GetHandle(), (PSZ) "OOL_BOOKPAGE", NULL, 0, 0, 0, 0, 0, o->GetHandle(), HWND_TOP, pageID, this, 0);
      WinSetWindowPtr(winhandle, 0, this);
   }

   ULONG p = (insertBehind ? insertBehind->id : 0);

   owner = (XNoteBook *) o;
   id = LONGFROMMR(WinSendMsg(o->GetHandle(), BKM_INSERTPAGE, (MPARAM) p, MPFROM2SHORT(style | BKA_AUTOPAGESIZE, order)));
   WinSendMsg(o->GetHandle(), BKM_SETPAGEWINDOWHWND, MPFROMLONG(id), MPFROMHWND(winhandle));

   if (title)
      SetText(title);
}


XNoteBookPage :: ~XNoteBookPage()
{
   HENUM enumWindow = WinBeginEnumWindows(winhandle);
   HWND hwnd;

   while ((hwnd = WinGetNextWindow(enumWindow)) != NULLHANDLE)
   {
      XWindow *w = (XWindow *) WinQueryWindowPtr(hwnd, 0);

      if (w && w != this)
         delete w;
   }
   WinEndEnumWindows(enumWindow);
}


/*@ XNoteBookPage :: SetStatusText( const char * t)
@group text functions
@remarks Set the text of the status-line (if the page has one)
@parameters char * text    the text to display
*/
void XNoteBookPage::SetStatusText(const char *t) const
{
   WinSendMsg(owner->GetHandle(), BKM_SETSTATUSLINETEXT, MPFROMLONG(id), MPFROMP(t));
}


/*@ XNoteBookPage :: SetText( const char * t)
@group text functions
@remarks Set the text of the tab
@parameters char * text    the text to display
*/
void XNoteBookPage::SetText(const char *t)
{
   WinSendMsg(owner->GetHandle(), BKM_SETTABTEXT, MPFROMLONG(id), MPFROMP(t));
}


/*@ XNoteBookPage :: SetBitmap( const XBitmap * b)
@group text functions
@remarks Set the bitmap of the tab
@parameters XBitmap * bitmap    the bitmap to display
*/
void XNoteBookPage::SetBitmap(const XBitmap * b)
{
   WinSendMsg(owner->GetHandle(), BKM_SETTABBITMAP, MPFROMLONG(id), MPFROMLONG(b->GetHandle()));
}


/*@ XNoteBookPage :: SetTop( void )
@group misc
@remarks Set the page to the top of the notebook
*/
void XNoteBookPage::SetTop(void) const
{
   WinSendMsg(owner->GetHandle(), BKM_TURNTOPAGE, MPFROMLONG(id), 0);
}


/*@ XNoteBookPage :: GetPageCount( void )
@group misc
@remarks Returns the count of minor-pages behind this page up to the next major-page
@returns SHORT numberOfPages
*/
SHORT XNoteBookPage::GetPageCount(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, BKM_QUERYPAGECOUNT, (MPARAM) id, (MPARAM) BKA_MAJOR));
}
