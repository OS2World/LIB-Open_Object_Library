#include "xnotebk.h"
#include "XBookPg.h"

/*@
@class XNoteBook
@parent XControl
@type overview
@symbol _
@changed
*/

/*@ XNoteBook :: XNoteBook(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, char *font)
@group constructors/destructors
@remarks Create a notebook
@parameters <t 'ø' c=2>
            øXWindow * owner   øthe owner-window
            øXRect& rect      øsize and position
            øUSHORT id         øID of the window
            øULONG style       østyle of the notebook, valid values are:<BR>
NB_BACKPAGESBR<BR>
NB_BACKPAGESBL<BR>
NB_BACKPAGESTR<BR>
NB_BACKPAGESTL<BR>
NB_MAJORTABRIGHT<BR>
NB_MAJORTABLEFT<BR>
NB_MAJORTABTOP<BR>
NB_MAJORTABBOTTOM<BR>
NB_SQUARETABS<BR>
NB_ROUNDEDTABS<BR>
NB_POLYGONTABS<BR>
NB_SOLIDBIND<BR>
NB_SPIRALBIND<BR>
NB_STATUSTEXTLEFT<BR>
NB_STATUSTEXTRIGHT<BR>
NB_STATUSTEXTCENTER<BR>
NB_TABTEXTLEFT<BR>
NB_TABTEXTRIGHT<BR>
NB_TABTEXTCENTER<BR>
NB_TABBEDDIALOG   (Warp4 only)<BR>
                              (can be or-ed, default is NB_SOLIDBIND|NB_BACKPAGESBR|NB_SQUARETABS|NB_TABTEXTCENTER|NB_STATUSTEXTLEFT)
            øchar * font         øfont to use, eg "8.Helv" (default is NULL)
            </t>
*/
XNoteBook :: XNoteBook(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, char *font):XControl(&rec, style, owner, "", WC_NOTEBOOK, id, font)
{
   color = XColor(COL_WHITE).GetColor();
}


XNoteBook :: ~XNoteBook()
{
   ULONG id = (ULONG) WinSendMsg( winhandle, BKM_QUERYPAGEID, 0, MPFROM2SHORT( BKA_FIRST, 0));
   HWND hwnd = (HWND) WinSendMsg( winhandle, BKM_QUERYPAGEWINDOWHWND, (MPARAM) id, 0);

   while (hwnd != 0 && id != 0)
   {
      XWindow *w = (XWindow *) WinQueryWindowPtr(hwnd, 0);
      if( w )
         w->Close();
      WinSendMsg( winhandle, BKM_DELETEPAGE, (MPARAM) id, (MPARAM) BKA_SINGLE);

      id = (ULONG) WinSendMsg( winhandle, BKM_QUERYPAGEID, 0, MPFROM2SHORT( BKA_FIRST, 0));
      if( id )
         hwnd = (HWND) WinSendMsg( winhandle, BKM_QUERYPAGEWINDOWHWND, (MPARAM) id, 0);
      if( hwnd == BOOKERR_INVALID_PARAMETERS )
         hwnd = 0;
   }
}


/*@ XNoteBook :: RemovePage( XNoteBookPage * p)
@remarks Remove a page from the notebook
@parameters XNoteBookPage* thePage
@returns BOOL success
@new
*/
BOOL XNoteBook :: RemovePage( XNoteBookPage * p)
{
   return (BOOL) WinSendMsg( winhandle, BKM_DELETEPAGE, (MPARAM) p->GetID(), (MPARAM) BKA_SINGLE);
}


/*@ XNoteBook :: GetPage( USHORT order, ULONG id )
@remarks Retrieve a page from th notebook
@parameters
<t 'ø' c=2>
øUSHORT øorder of searching:<BR>
BKA_FIRST<BR>
BKA_NEXT<BR>
BKA_PREV<BR>
BKA_LAST<BR>
BKA_TOP
øULONG   øID of a relative page. This id is <B>not</B> the window-ID, this ID is automatical
          created from the operating system, you can get this ID with XNoteBookPage::GetID().
</t>
@returns XNoteBookPage* pointer to the page
@example
   //how to enumerate all pages
   XNoteBookPage * last = NULL, * page = noteBook->GetPage( BKA_FIRST );

   while( page )
   {
      last = page;
      page = noteBook->GetPage( BKA_NEXT, last->GetID());
   };
*/
XNoteBookPage * XNoteBook :: GetPage( USHORT order, ULONG id )
{
   ULONG i = (ULONG) WinSendMsg( winhandle, BKM_QUERYPAGEID, (MPARAM) id, MPFROM2SHORT( order, 0));
   HWND hwnd = (HWND) WinSendMsg( winhandle, BKM_QUERYPAGEWINDOWHWND, (MPARAM) i, 0);
   return (XNoteBookPage*) WinQueryWindowULong( hwnd, 0);
}


/*@ XNoteBook::SetMajorTabSize(const SHORT width, const SHORT height)
@group size
@remarks Set the size of major tabs.
@parameters
<t 'ø' c=2>
øSHORT øwidth
øSHORT øhight
</t>
*/
void XNoteBook::SetMajorTabSize(const SHORT width, const SHORT height) const
{
   WinSendMsg(winhandle, BKM_SETDIMENSIONS, MPFROM2SHORT(width, height), (MPARAM) BKA_MAJORTAB);
}


/*@ XNoteBook::SetMinorTabSize(const SHORT width, const SHORT height)
@group size
@remarks Set the size of minor tabs.
@parameters
<t 'ø' c=2>
øSHORT øwidth
øSHORT øhight
</t>
*/
void XNoteBook::SetMinorTabSize(const SHORT width, const SHORT height) const
{
   WinSendMsg(winhandle, BKM_SETDIMENSIONS, MPFROM2SHORT(width, height), (MPARAM) BKA_MINORTAB);
}


/*@ XNoteBook::SetBackgroundColor(const XColor * col)
@group colors
@remarks Set the background color.
@parameters XColor * color    the new color
*/
BOOL XNoteBook::SetBackgroundColor(const XColor * col)
{
   color = col->GetColor();
   WinSendMsg(winhandle, BKM_SETNOTEBOOKCOLORS, MPFROMLONG(col->GetColor()), MPFROMLONG(BKA_BACKGROUNDPAGECOLOR));
   return TRUE;
}


/*@ XNoteBook::SetMajorBackgroundColor(const XColor * col)
@group colors
@remarks Set the background color of major tabs.
@parameters XColor * theColor
*/
void XNoteBook::SetMajorBackgroundColor(const XColor * col) const
{
   WinSendMsg(winhandle, BKM_SETNOTEBOOKCOLORS, MPFROMLONG(col->GetColor()), MPFROMLONG(BKA_BACKGROUNDMAJORCOLOR));
}


/*@ XNoteBook::SetMajorForegroundColor(const XColor * col)
@group colors
@remarks Set the foreground color of major tabs.
@parameters XColor * theColor
*/
void XNoteBook::SetMajorForegroundColor(const XColor * col) const
{
   WinSendMsg(winhandle, BKM_SETNOTEBOOKCOLORS, MPFROMLONG(col->GetColor()), MPFROMLONG(BKA_FOREGROUNDMAJORCOLOR));
}


/*@ XNoteBook::SetMinorBackgroundColor(const XColor * col)
@group colors
@remarks Set the backgroundcolor of minor tabs.
@parameters XColor * theColor
*/
void XNoteBook::SetMinorBackgroundColor(const XColor * col) const
{
   WinSendMsg(winhandle, BKM_SETNOTEBOOKCOLORS, MPFROMLONG(col->GetColor()), MPFROMLONG(BKA_BACKGROUNDMINORCOLOR));
}


/*@ XNoteBook::SetMinorForegroundColor(const XColor * col)
@group colors
@remarks Set the foreground color of minor tabs.
@parameters XColor * theColor
*/
void XNoteBook::SetMinorForegroundColor(const XColor * col) const
{
   WinSendMsg(winhandle, BKM_SETNOTEBOOKCOLORS, MPFROMLONG(col->GetColor()), MPFROMLONG(BKA_FOREGROUNDMINORCOLOR));
}


/*@ XNoteBook::GetBackgroundColor(XColor*)
@group colors
@remarks Returns the background color.
@parameters XColor * buffer   buffer to hold the data
*/


/*@ XNoteBook::GetPageCount(void)
@group misc
@remarks Returns the count of pages.
@returns SHORT numberOfPages
@new
*/
SHORT XNoteBook::GetPageCount(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, BKM_QUERYPAGECOUNT, (MPARAM) 0, (MPARAM) BKA_END));
}


/*@ XNoteBook::CalcClientRect(XRect * out)
@group misc
@remarks Calculate the client-region of the notebook.
@parameters XRect * buffer
@new
*/
void XNoteBook::CalcClientRect(XRect * out, BOOL page)
{
   RECTL rect;
   XRect r;
   GetSize(&r);
   rect.xLeft = r.GetX();
   rect.yBottom = r.GetY();
   rect.xRight = r.GetWidth() + r.GetX();
   rect.yTop = r.GetY() + r.GetHeight();
   WinSendMsg(winhandle, BKM_CALCPAGERECT, MPFROMP(&rect), (MPARAM) page);
   out->SetX( rect.xLeft );
   out->SetY( rect.yBottom );
   out->SetWidth( rect.xRight - rect.xLeft );
   out->SetHeight( rect.yTop - rect.yBottom );
}


