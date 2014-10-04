#include "XGraphDv.h"
#include "XGraphOb.h"
#include "XWindow.h"
#include "XColor.h"
#include "XMsgBox.h"
#include "XFont.h"
#include "xexcept.h"
#include "xapp.h"
#include <stdlib.h>


/*@
@class XGraphicDevice
@type overview
@symbol _
@remarks XGraphicDevice is a context in which graphic object can be created and drawn.
There are two methods to use XGraphicDevice
<OL>
<LI><B>Using one graphic device</B>
<P>The most simple way is to use one graphic device
in this case you only need to create the objects
to draw in this device, every time it has to be
redrawn (when the method Draw() of the owning
window is called) you call the method Draw() of
the device.
<LI><B>Using two graphic devices</B>
<P>If you have a litle bit more objects you should use
this method because it works faster. Create one
device in memory (the second parameter of the
constructor must be TRUE) and create the objects
to draw on this device. Only call Draw() if you
have inserted/removed one or more objcts. Create
a second graphic device associated to the owning
window. If the method Draw() of the owning window
is called, copy the content of the memory-device
to the window-associated device with Copy().
</OL>
You can draw on a XGraphicDevice (and derived classes) with following classes:
<UL>
<LI>XArc
<LI>XBitmap
<LI>XBox
<LI>XCircle
<LI>XLine
<LI>XMarker
<LI>XText
</UL>
@seealso XLayer
*/


void XGraphicDevice::RegisterFont(XFont * font)
{
   fonts++;
   fontList = (XFont **) realloc(fontList, fonts * sizeof(void *));
   fontList[fonts - 1] = font;
}


/*@ XGraphicDevice::Copy(const XGraphicDevice * s, const XRect * target, const XRect * source)
@group drawing
@remarks Copy the content of one graphic to another
@parameters   <t 'ø' c=2>
            øXGraphicDevice * source      øthe source from which to copy
            øXRect * targetRect         ørectangle which defines the target
            øXRect * sourceRect         ørectangle which defines the source (default is NULL).
                                       If the width of the rect is set greater zero, the
                                       content will be stretched
            </t>
*/
void XGraphicDevice::Copy(const XGraphicDevice * s, const XRect * target, const XRect * source)
{
   POINTL p[4];
   LONG m = 3;

   p[0].x = target->GetX();
   p[0].y = target->GetY();
   p[1].x = p[0].x + target->GetWidth();
   p[1].y = p[0].y + target->GetHeight();
   if (source)
   {
      p[2].x = source->GetX();
      p[2].y = source->GetY();
      if (source->GetWidth() > 0)
      {
         m = 4;
         p[3].x = p[2].x + source->GetWidth();
         p[3].y = p[2].x + source->GetHeight();
      }
   }
   else
      p[2].x = p[2].y = 0;
   GpiBitBlt(hps, s->hps, m, p, ROP_SRCCOPY, BBO_IGNORE);
}



XGraphicDevice :: XGraphicDevice(const LONG r)
{
   fonts = 0;
   fontList = NULL;
   // fontId = 0;
   xOffset = yOffset = 0;
   width = height = 0;
   aktPattern = 0;
   hwnd = NULL;
   hab = NULL;
   mode = r;
   aktLineEnding = LINEEND_DEFAULT;
   aktLineJoining = LINEJOIN_DEFAULT;
   aktLineWidth = LINEWIDTH_DEFAULT;
   aktLineType = LINETYPE_DEFAULT;
}


/*@ XGraphicDevice :: XGraphicDevice(const XWindow * w, const BOOL memory, const BOOL fillBack, LONG m)
@remarks Constructs a grphic device where objects like XLine, XText etc can be drawn
@parameters   <t 'ø' c=2>
            øXWindow * window      øwindow to draw to (if XGraphicDevice is not constructed in memory)
            øBOOL memory            øconstruct in memory (default is FALSE)
            øBOOL fillBackground   øfill the background
            øLONG resolution      øpossible values:
<BR>
XGRAPH_PELS
<BR>
XGRAPH_LOMETRIC
<BR>
XGRAPH_HIMETRIC
<BR>
XGRAPH_LOENGLISH
<BR>
XGRAPH_HIENGLISH
<BR>
XGRAPH_TWIPS
            øLONG mode            ømode, see OS/2 API documentation ( GPIA_ASSOC and related )
            </t>
*/
extern char *pszdata[9];

XGraphicDevice :: XGraphicDevice(const XWindow * w, const BOOL memory, const BOOL fillBack, LONG r, LONG m)
{
   fonts = 0;
   fontList = NULL;
   fill = fillBack;
   // fontId = 0;
   xOffset = yOffset = 0;
   width = height = 0;
   SIZEL g = {0, 0};
   mode = r;

   hwnd = w->GetHandle();//Client
   hab = WinQueryAnchorBlock(hwnd);

   if (!(memory))
   {
      hdc = WinOpenWindowDC(hwnd);
      hps = GpiCreatePS(hab, hdc, &g, mode | m); //| GPIF_LONG | GPIA_ASSOC| GPIF_LONG | GPIA_ASSOC
      if( hps == GPI_ERROR )
         OOLThrow( "couldnït create HPS", GPI_ERROR);
   }
   else
   {
      hdc = DevOpenDC(hab, OD_MEMORY, (PSZ) "*", 4, (PDEVOPENDATA) pszdata, 0L);
      hps = GpiCreatePS(hab, hdc, &g, mode | m);//| PU_PELS | GPIA_ASSOC);

      if( hps == GPI_ERROR )
         OOLThrow( "couldnït create HPS", GPI_ERROR);
      BITMAPINFOHEADER2 h;

      memset(&h, 0, sizeof(BITMAPINFOHEADER2));
      h.cbFix = sizeof(BITMAPINFOHEADER2);
      width = h.cx = 2000;
      height = h.cy = 2000;
      h.cBitCount = 8;
      h.cPlanes = 1;
      HBITMAP hbm = GpiCreateBitmap(hps, &h, 0, NULL, NULL);

      GpiSetBitmap(hps, hbm);
   }

   GpiCreateLogColorTable(hps, LCOL_RESET, LCOLF_RGB, 0, 1, &backCol);

   backCol = 16777216;
   aktColor = 0;
   aktLineEnding = LINEEND_DEFAULT;
   aktLineJoining = LINEJOIN_DEFAULT;
   aktLineWidth = LINEWIDTH_DEFAULT;
   aktLineType = LINETYPE_DEFAULT;
   aktPattern = 0;
   aktFont = 0;
   GpiSetCharMode(hps, CM_MODE2);
   if (memory)
      hwnd = 0;
}


/*@ XGraphicDevice::SetBackgroundColor(const XColor * c)
@remarks Set the background color (only used if fillBackgound is set TRUE in the constructor)
@parameters   XColor * color      color to fill the background with
*/
void XGraphicDevice::SetBackgroundColor(const XColor * c)
{
   backCol = c->GetColor();
   GpiSetBackColor(hps, backCol);
}


XGraphicDevice :: ~XGraphicDevice()
{
   for (int i = 0; i < fonts; i++)
   {
      GpiDeleteSetId(hps, fontList[i]->fontId);
      delete fontList[i];
   }
   if(fontList)
      free(fontList);

   if (hps)
      GpiDestroyPS(hps);
}



/*@ XGraphicDevice::FillBackground(void)
@remarks Fill the background
*/
void XGraphicDevice::FillBackground(void)
{
   if (hwnd)
   {
      if (fill)
      {
         RECTL rect;
         WinQueryUpdateRect(hwnd, &rect);
         WinFillRect(hps, &rect, backCol);
      }
   }
   else
   {
      if (fill)
      {
         RECTL rect = {0, 0, width, height};
         WinFillRect(hps, &rect, backCol);
      }
   }
}


void XGraphicDevice::SetLineWidth(const LONG width)
{
   GpiSetLineWidth(hps, width);
   aktLineWidth = width;
}


void XGraphicDevice::ResetFont(void)
{
   GpiSetCharSet(hps, 0);
   aktFont = 0;
}


void XGraphicDevice::SetFont(XFont * font)
{
   EnableFont(font, (SHORT) (font->fontSize * 1.8 / 10));
}


void XGraphicDevice::EnableFont( XFont * font, LONG fontSize)
{
   aktFont = font->fontId;
   GpiSetCharSet(hps, aktFont);
   SIZEF sc = {0, 0};
   SIZEL s;

   if (font->fixed == FALSE)
   {
      switch(mode)
      {
         case PU_LOMETRIC:
            fontSize = fontSize * 2 * 254 / 1440;
            break;
         case PU_HIMETRIC:
            fontSize = fontSize * 2 * 2540 / 1440;
            break;
         case PU_LOENGLISH:
            fontSize = fontSize * 2 * 100 / 1440;
            break;
         case PU_HIENGLISH:
            fontSize = fontSize * 2 * 1000 / 1440;
            break;
         case PU_PELS:
            break;
         default:
            fontSize = fontSize * 2;
      }

      sc.cx = sc.cy = MAKEFIXED(fontSize, 0);
      GpiSetCharBox(hps, &sc);
   }
   FONTMETRICS fm;

   GpiQueryFontMetrics(hps, sizeof(fm), &fm);
   font->realSize = fm.lMaxBaselineExt;
   font->pixelSize = fontSize;
}
