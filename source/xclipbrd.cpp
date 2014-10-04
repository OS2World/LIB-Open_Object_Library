#include "XClipBrd.h"
//#include "XWindow.h"
#include "XBitmap.h"
#include "XString.h"
#include "xapp.h"


/*@
@class XClipBoard
@parent XObject
@type overview
@symbol _
*/
void XClipBoard :: Empty(void) const
{
   WinEmptyClipbrd ( XApplication::GetApplication()->GetAnchorBlock());
}


/*@ XClipBoard :: IsFormatAvaible( ULONG format)
@group misc
@remarks Query if a specific format is avaible
@parameters ULONG format of interest (see OS/2 documentation)
@returns BOOL
*/
BOOL XClipBoard :: IsFormatAvaible( ULONG format)
{
   ULONG form;
   return WinQueryClipbrdFmtInfo(XApplication::GetApplication()->GetAnchorBlock(), format, &form);
}


/*@ XClipBoard :: XClipBoard( )
@group constructors/destructors
@remarks Open the clipboard
*/
XClipBoard :: XClipBoard()
{
   isOpen = FALSE;
   if (WinOpenClipbrd( XApplication::GetApplication()->GetAnchorBlock() ) == FALSE)
      return;
   isOpen = TRUE;
}


/*@ XClipBoard :: GetText( XString * buffer )
@group get data
@remarks Get the text in the clipboard
@parameters    XString * buffer to hold the data
@returns BOOL success
*/
BOOL XClipBoard::GetText(XString * buffer)
{
   char * res = (char*) WinQueryClipbrdData( XApplication::GetApplication()->GetAnchorBlock(), CF_TEXT);
   if(res)
   {
      *buffer = res;
      return TRUE;
   } /* end if */
   else
   {
      *buffer = "";
      return FALSE;
   }
}


/*@ XClipBoard :: GetBitmap( XBitmap * bmp )
@group get data
@remarks Get a bitmap from the clipboard
@parameters XBitmap * buffer to hold the data
@returns TRUE success
*/
BOOL XClipBoard::GetBitmap(XBitmap * bmp)
{
   HBITMAP hbm = WinQueryClipbrdData( XApplication::GetApplication()->GetAnchorBlock(), CF_BITMAP);
   if(hbm)
   {
      bmp->hbm = hbm;
      XSize size;
      bmp->GetDimensions(&size);
      bmp->width = bmp->cx = size.GetWidth();
      bmp->height = bmp->cy = size.GetHeight();
      return TRUE;
   } /* end if */
   else
      return FALSE;
}


/*@ XClipBoard :: SetBitmap( const XBitmap * bmp)
@group set data
@remarks Set a bitmap to the clipboard
@parameters XBitmap * the bitmap
@returns    BOOL success
*/
BOOL XClipBoard::SetBitmap(const XBitmap * bmp)
{
   return WinSetClipbrdData(XApplication::GetApplication()->GetAnchorBlock(), (ULONG) bmp->GetHandle(), CF_BITMAP, CFI_HANDLE);
}


/*@ XClipBoard :: SetText( const char * p, const ULONG len)
@group set data
@remarks Set text to the clipboard
@parameters
<t 'ø' c=2>
øchar* øtheText
øULONG ølength of the text (default is 0)
</t>
@returns    BOOL                         success
*/
BOOL XClipBoard::SetText(const char *p, const ULONG len)
{
   void *pp;

   ULONG l = (len == 0 ? strlen(p) + 1 : len + 1);

   DosAllocSharedMem(&pp, NULL, l, PAG_READ | PAG_WRITE | PAG_COMMIT | OBJ_GIVEABLE);
   strcpy((char *) pp, p);
   return WinSetClipbrdData(XApplication::GetApplication()->GetAnchorBlock(), (ULONG) pp, CF_TEXT, CFI_POINTER);
}


/*@ XClipBoard :: ~XClipBoard()
@group constructor/destructor
@remarks Never forget to close the clipboard by calling the destructor!
*/
XClipBoard :: ~XClipBoard()
{
   if (isOpen)
      WinCloseClipbrd(XApplication::GetApplication()->GetAnchorBlock());
}


/*@ XClipBoard :: SetData( const void * p, const XAtom&)
@group set data
@remarks Set data in a application defined format to the clipboard
@parameters
<t 'ø' c=2>
øvoid* øpointer to shared memory which contains the data
øconst XAtom& øatom which identifies the format of the data
</t>
@returns    BOOL                         success
*/
BOOL XClipBoard :: SetData(const void *p, const XAtom& atom)
{
   return WinSetClipbrdData( XApplication::GetApplication()->GetAnchorBlock(), (ULONG) p, atom.GetAtom(), CFI_POINTER);
}


/*@ XClipBoard :: GetData( const XAtom&)
@group get data
@remarks Get data in a application defined format to the clipboard
@parameters const XAtom&  atom which describes the format of the data
@returns    void * pointer to the data (NULL if no data avaible)
*/
void * XClipBoard :: GetData(const XAtom& atom)
{
   return (void *) WinQueryClipbrdData( XApplication::GetApplication()->GetAnchorBlock(), atom.GetAtom());
}

