#include "XSIcon.h"
#include "XSFrame.h"
#include "XSBitmap.h"
#include "XBitmap.h"
#include "XGroupBx.h"
#include "XSText.h"
#include "xcontrol.h"
#include <stdio.h>

XStatic :: XStatic(const HWND hwnd)
{
   winhandle = hwnd;
   WinSetWindowPtr(hwnd, 0, this);
}


XStatic :: XStatic(const ULONG style, const char *title, const PSZ classname, const XWindow * ow, const XRect& r, const LONG id, const char *font)
{
   PRES pres;
/*****
   LONG x = 0, y = 0, cx = 0, cy = 0;

   x = r.GetX();
   y = r.GetY();
   cx = r.GetWidth();
   cy = r.GetHeight();
****/
   char buffer[100], *ch;

   void *p = NULL;

   if (font)
   {
      pres.fcparam.id = PP_FOREGROUNDCOLORINDEX;
      pres.fcparam.cb = sizeof(pres.fcparam.ulColor);
      pres.fcparam.ulColor = 16;
      pres.fntparam.id = PP_FONTNAMESIZE;
      pres.fntparam.cb = 20;
      pres.cb = sizeof(pres.fcparam) + sizeof(pres.fntparam);
      p = (void *) &pres;
      strcpy(pres.fntparam.szFontNameSize, font);
   }

   if ((style & SS_ICON || style & SS_BITMAP) && id > 0 && (!(style & SS_TEXT)))
   {
      sprintf(buffer, "#%i", (int) id);
      ch = buffer;
   }
   else
      ch = (char *) title;

   OOL_WINDOWHANDLE handle = ow->GetHandle();

   winhandle = WinCreateWindow(handle, classname, (PSZ) ch, style, r.GetX(), r.GetY(), r.GetWidth(), r.GetHeight(), handle, HWND_TOP, id, NULL, p);
   WinSetWindowPtr(winhandle, 0, this);

   XColor color;

   ow->GetBackgroundColor(&color);
   SetBackgroundColor(&color);
}


/*@
@class XStaticText
@parent XStatic
@type overview
@symbol _
*/

/*@ XStaticText :: XStaticText( const XWindow * w, const char *text, const XRect& r, const LONG style, const LONG id, char *font)
@group constructors/destructors
@remarks Constructs a static text
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner of the icon
            øchar * text            øtext to display
            øXRect& rect         øthe rectangle
            øLONG style           øthe style
            øLONG id              øthe id of the icon in the program resources
            øchar * font            øfont to use, eg "8.Helv" (default is NULL)
            </t>
*/
XStaticText :: XStaticText(const XWindow * w, const char *text, const XRect& r, const LONG style, const LONG id, char *font):XStatic(SS_TEXT | style, (char *) text, WC_STATIC, w, r, id, font)
{
}


/*@
@class XStatic
@parent XWindow
@type overview
@symbol _
*/

/*@
@class XStaticIcon
@parent XStatic
@type overview
@symbol _
*/

/*@ XStaticIcon :: XStaticIcon( const XWindow * w, const XRect * r, const LONG id, const LONG style)
@group constructors/destructors
@remarks Constructs a static icon
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner of the icon
            øXRect& rect         øthe rectangle
            øLONG id              øthe id of the icon in the program resources
            øLONG style           øthe style
            </t>
*/
XStaticIcon :: XStaticIcon( const XWindow * w, const char * s, const XRect& r, const LONG id, const LONG style):XStatic(SS_ICON | WIN_VISIBLE | style, s, WC_STATIC, w, r, id)
{
}


/*@
@class XStaticBitmap
@parent XStatic
@type overview
@symbol _
*/

/*@ XStaticBitmap :: XStaticBitmap(const XWindow * w, const XRect& r, const LONG id, const LONG style)
@group constructors/destructors
@remarks Constructs a static bitmap
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner of the bitmap
            øXRect& rect         øthe rectangle
            øLONG id              øthe id of the bitmap in the program resources
            øLONG style           øthe style
            </t>
*/
XStaticBitmap :: XStaticBitmap( const XWindow * w, const char * s, const XRect& r, const LONG id, const LONG style):XStatic(SS_BITMAP | WIN_VISIBLE | style, s, WC_STATIC, w, r, id)
{
   bmp = NULL;
}


void XStaticBitmap :: Load(char * path)
{
   bmp = new XBitmap(this);
   bmp->Load(path);
   WinSendMsg( GetHandle(), SM_SETHANDLE, (MPARAM) bmp->GetHandle(), 0);
}


void XStaticBitmap :: Load(XResource*r)
{
   bmp = new XBitmap(this);
   bmp->Load(r);
   WinSendMsg( GetHandle(), SM_SETHANDLE, (MPARAM) bmp->GetHandle(), 0);
}


void XStaticBitmap :: Load(ULONG id)
{
   bmp = new XBitmap(this);
   bmp->Load(id);
   WinSendMsg( GetHandle(), SM_SETHANDLE, (MPARAM) bmp->GetHandle(), 0);
}


XStaticBitmap :: ~XStaticBitmap()
{
   if(bmp)
      delete bmp;
}

/*@
@class XStaticFrame
@parent XStatic
@type overview
@symbol _
*/

/*@ XStaticFrame :: XStaticFrame( const XWindow * w, const XRect& r, LONG id)
@group constructors/destructors
@remarks Constructs a static frame
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner of the frame
            øXRect& rect         øthe rectangle
            øLONG id              øid for the frame
            </t>
*/
XStaticFrame :: XStaticFrame( const XWindow * w, const XRect& r, const LONG id):XStatic(SS_FGNDFRAME | WIN_VISIBLE, "", WC_STATIC, w, r, id)
{
}


/*@
@class XGroupBox
@parent XStatic
@type overview
@symbol _
*/

/*@ XGroupBox :: XGroupBox( const XWindow * owner, const XRect& rec, const char *string, const LONG id)
@group constructors/destructors
@remarks Constructs a group-box
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner of the box
            øXRect& rect         øthe rectangle
            øconst char* title    øthe title of the group-box
            </t>
*/
XGroupBox :: XGroupBox( const XWindow * owner, const XRect& rec, const char *string, const LONG id):XStatic(SS_GROUPBOX | WS_GROUP|WS_VISIBLE, (char *) string, WC_STATIC, owner, rec, id)
{
}


