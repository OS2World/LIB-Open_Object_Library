#include "XGraphOb.h"
#include "XGraphDv.h"
#include "XLine.h"
#include "XColor.h"
#include "XRect.h"
#include "XFont.h"
#include "XCircle.h"
#include "XText.h"
#include "XBox.h"
#include "XMarker.h"
#include "XBitmap.h"
#include "XFile.h"
#include "XFileInf.h"
#include "XRes.h"
#include "XResLib.h"
#include "XArc.h"
#include "xmsgbox.h"
#include "xexcept.h"
#include <stdlib.h>
#include <string.h>
#include "xapp.h"
#include <stdlib.h>


/*@
@class XGraphicObject
@parent XObject
@type overview
@symbol _
*/

/*@ XGraphicObject::Draw( XGraphicDevice * dev )
@group drawing
@remarks Draw the graphic primitive on a screen or printer
@parameters XGraphicDevice * the device to draw on
*/

XGraphicObject :: XGraphicObject(const XPoint * point)
{
   settings = 0;
   if (point)
      p = *point;
   color.SetColor(0);
}


XGraphicObject :: XGraphicObject(const LONG x, const LONG y)
{
   p.SetX(x);
   p.SetY(y);
   color.SetColor(0);
   settings = 0;
}


/*@ XGraphicObject::Show(const BOOL show)
@group misc
@remarks Show/hide the object
@parameters   BOOL show      TRUE=show, FALSE=hide
*/
void XGraphicObject::Show(const BOOL show)
{
   if (show)
   {
      if (settings & GO_HIDE)
         settings ^= GO_HIDE;
   }
   else
      settings |= GO_HIDE;
//   Draw();
}


XMarker :: XMarker(const XPoint * point, const SHORT t):XGraphicObject( point)
{
   type = t;
   size.SetWidth(10);
   size.SetHeight(10);
}


void XMarker::SetupDevice( XGraphicDevice * dev)
{
   XGraphicObject::SetupDevice(dev);
   if (dev->markerType != type)
   {
      dev->markerType = type;
      GpiSetMarker(dev->hps, type);
   }
   SIZEF s;

   s.cx = MAKEFIXED(size.GetWidth(), 0);
   s.cy = MAKEFIXED(size.GetHeight(), 0);
   GpiSetMarkerBox(dev->hps, &s);
}


void XMarker::Draw( XGraphicDevice * dev)
{
   if (!(settings & GO_HIDE))
   {
      SetupDevice(dev);
      POINTL po;
      po.x = p.x;
      po.y = p.y;
      GpiMove(dev->hps, &po);
      GpiMarker(dev->hps, &po);
   }
}


void XLine::SetupDevice( XGraphicDevice * dev)
{
   XGraphicObject::SetupDevice(dev);
   if (lineJoin != dev->aktLineJoining)
      dev->SetLineJoin(lineJoin);
   if (lineEnd != dev->aktLineEnding)
      dev->SetLineEnd(lineEnd);
   if (lineWidth != dev->aktLineWidth)
      dev->SetLineWidth(lineWidth);
   if (lineType != dev->aktLineType)
      dev->SetLineType(lineType);
}


void XLine::Draw( XGraphicDevice * dev)
{
   if (!(settings & GO_HIDE))
   {
      POINTL po;
      po.x = p.x;
      po.y = p.y;
      SetupDevice(dev);
      GpiBeginPath(dev->hps, 1);
      GpiMove(dev->hps, &po);
      //POINTL ptl;// = po;

      po.x = p.x + width;
      po.y = p.y + height;
      GpiLine(dev->hps, &po);
      GpiEndPath(dev->hps);
      GpiOutlinePath(dev->hps, 1, 0);
   }
}


/*@
@class XCircle
@parent XLine
@type overview
@symbol _
*/


/*@ XCircle :: XCircle(const XPoint * pp1, const ULONG wight, const BOOL fill, const USHORT hight, const SHORT xOff, const SHORT yOff)
@group constructors/destructors
@remarks Construct a circle
@parameters   <t 'ø' c=2>
            øXPoint * øposition
            øULONG øwidth
            øBOOL øTRUE=fill circle, FALSE=donït fill
            øULONG øif zero, height becomes width, otherwise an elipse is drawn (default is zero)
            øSHORT øx-offset (default is zero)
            øSHORT øy-offset (default is zero)
            </t>
*/
XCircle :: XCircle(const XPoint * pp1, const ULONG wight, const BOOL fill, const USHORT hight, const SHORT xOff, const SHORT yOff):XLine( pp1)
{
   w = wight;
   h = (hight > 0 ? hight : wight);
   x = xOff;
   y = yOff;
   filled = fill;
}


/*@
@class XArc
@parent XLine
@type overview
@symbol _
*/

/*@ XArc :: XArc(const XPoint * pp1, const SHORT wight, const SHORT startAngle, const SHORT sweepAngle, const BOOL showLines, const BOOL fill, const SHORT hight, const SHORT xOff, const SHORT yOff)
@group constructors/destructors
@remarks Construct an arc
@parameters   <t 'ø' c=2>
            øXPoint * øposition
            øSHORT øwidth
            øSHORT østarting angle
            øSHORT øending angle
            øBOOL øTRUE=fill circle, FALSE=donït fill
            øULONG øif zero, height becomes width, otherwise an elipse is drawn (default is zero)
            øSHORT øx-offset (default is zero)
            øSHORT øy-offset (default is zero)
            </t>
*/
XArc :: XArc(const XPoint * pp1, const SHORT wight, const SHORT startAngle, const SHORT sweepAngle, const BOOL showLines, const BOOL fill, const SHORT hight, const SHORT xOff, const SHORT yOff):XLine(pp1)
{
   settings = 0;
   st = startAngle;
   en = sweepAngle;
   vis = showLines;
   w = wight;
   h = (hight > 0 ? hight : wight);
   x = xOff;
   y = yOff;
   filled = fill;
}


void XArc::Draw( XGraphicDevice * dev)
{
   if (!(settings & GO_HIDE))
   {
      SetupDevice(dev);

      POINTL po;
      po.x = p.x;
      po.y = p.y;
      GpiMove(dev->hps, &po);
      ARCPARAMS a;

      a.lQ = w;
      a.lP = h;
      a.lR = x;
      a.lS = y;

      GpiSetArcParams(dev->hps, &a);

      if (vis)
      {
         GpiBeginPath(dev->hps, 1);
         GpiPartialArc(dev->hps, &po, MAKEFIXED(1, 0), MAKEFIXED(st, 0), MAKEFIXED(en, 0));
         GpiLine(dev->hps, &po);
         GpiEndPath(dev->hps);
         if (filled)
            GpiFillPath(dev->hps, 1, FPATH_WINDING);
         else
            GpiOutlinePath(dev->hps, 1, 0);
      }
      else
      {
         POINTL pt[3];

//berechnen!
         pt[0].x = pt[0].y = 0;
         GpiMove(dev->hps, &pt[0]);
         pt[1].x = pt[1].y = 100;
         pt[2].x = 200;
         pt[2].y = 0;
         GpiPointArc(dev->hps, &pt[1]);
      }
   }
}


void XCircle::Draw( XGraphicDevice * dev)
{
   if (!(settings & GO_HIDE))
   {
//       SHORT modify = ( w> 10 ? 10 : 1);

      SetupDevice(dev);
      POINTL po;
      po.x = p.x;
      po.y = p.y;
      GpiMove(dev->hps, &po);
      ARCPARAMS a;

      a.lQ = w / 10;
      a.lP = h / 10;
      a.lR = x / 10;
      a.lS = y / 10;
      GpiSetArcParams(dev->hps, &a);
      GpiFullArc(dev->hps, (filled == FALSE ? DRO_OUTLINE : DRO_FILL), MAKEFIXED(10, 0));
   }
}


/*@
@class XLine
@parent XGraphicObject
@type overview
@symbol _
*/


/*@ XLine :: XLine(const XPoint * pp1, const XPoint * pp2)
@group constructors/destructors
@remarks Construct a line object
@parameters   <t 'ø' c=2>
            øXPoint * p1                  østart point
            øXPoint * p2                  øend point
            </t>
*/
XLine :: XLine(const XPoint * pp1, const XPoint * pp2):XGraphicObject(pp1)
{
   lineEnd = LINEEND_DEFAULT;
   lineJoin = LINEJOIN_DEFAULT;
   lineWidth = LINEWIDTH_DEFAULT;
   lineType = LINETYPE_DEFAULT;
   if (pp2)
   {
      width = pp2->x - p.x;
      height = pp2->y - p.y;
   }
   else
      width = height = 0;
}


/*@
@class XFont
@parent XObject
@type overview
@symbol _
*/

/*@ XFont :: XFont(XGraphicDevice * dev, const char *fontName, const SHORT size, const LONG options)
@group constructors/destructors
@remarks Construct a font
@parameters   <t 'ø' c=2>
            øXGraphicDevice * device      øowner
            øchar * fontName            øname of the font
            øSHORT size                  øsize
            øLONG options               øoptions
            </t>
*/
XFont :: XFont(XGraphicDevice * dev, const char *fontName, const SHORT size, const LONG options)
{
   // dev->fontId+=1;
   fontId = dev->fonts + 1;
   fontSize = size;
   LONG cFont;               /* Fonts Count   */
   LONG lFontsTotal = 0L;      /* Fonts Total Count   */
   LONG lXDeviceRes;         /* x Device Resolution  */
   LONG lYDeviceRes;         /* y Device Resolution  */
   PFONTMETRICS pfmSelect;      /* Font Metrics Pointer  */
   FATTRS fat;               /* Font Attributes   */
   register INT i;            /* Loop Counter   */

   LONG lNominalPointSize = size * 10;

   DevQueryCaps(dev->hdc, CAPS_HORIZONTAL_FONT_RES, 1L, &lXDeviceRes);
   DevQueryCaps(dev->hdc, CAPS_VERTICAL_FONT_RES, 1L, &lYDeviceRes);
   realSize = size;
   if ((cFont = GpiQueryFonts(dev->hps, QF_PUBLIC, (PSZ) fontName, &lFontsTotal, sizeof(FONTMETRICS), (PFONTMETRICS) NULL)) != 0L)
   {
      DosAllocMem((PPVOID) (PVOID) & pfmSelect, (ULONG) (sizeof(FONTMETRICS) * cFont), PAG_READ | PAG_WRITE | PAG_COMMIT);

      GpiQueryFonts(dev->hps, QF_PUBLIC, (PSZ) fontName, &cFont, sizeof(FONTMETRICS), pfmSelect);

      for (i = 0; i < (INT) cFont; i++)
         if ((pfmSelect[i].sXDeviceRes == (SHORT) lXDeviceRes) && (pfmSelect[i].sYDeviceRes == (SHORT) lYDeviceRes) && ((LONG) pfmSelect[i].sNominalPointSize == lNominalPointSize))
         {
            memset(&fat, 0, sizeof(FATTRS));
            fat.usRecordLength = sizeof(FATTRS);
            strcpy(fat.szFacename, fontName);
            fat.lMatch = pfmSelect[i].lMatch;
            fat.fsSelection = (USHORT) options;
            DosFreeMem((PVOID) pfmSelect);
            fixed = TRUE;
            GpiCreateLogFont(dev->hps, (PSTR8) NULL, fontId, &fat);
            dev->RegisterFont(this);
            dev->SetFont(this);
            return;
         }

      for (i = 0; i < (INT) cFont; i++)
         if ((pfmSelect[i].sXDeviceRes == 1000) && (pfmSelect[i].sYDeviceRes == 1000))
         {
            memset(&fat, 0, sizeof(FATTRS));
            fat.usRecordLength = sizeof(FATTRS);
            strcpy(fat.szFacename, fontName);
            fat.lMatch = pfmSelect[i].lMatch;
            fat.usCodePage = 850;
            fat.fsFontUse = FATTR_FONTUSE_OUTLINE | FATTR_FONTUSE_TRANSFORMABLE;
            fat.fsSelection = (USHORT) options;

            DosFreeMem((PVOID) pfmSelect);
            GpiCreateLogFont(dev->hps, (PSTR8) NULL, fontId, &fat);
            fontSize = lNominalPointSize;
            fixed = FALSE;
            dev->RegisterFont(this);
            dev->SetFont(this);
            return;
         }
      DosFreeMem((PVOID) pfmSelect);
   }
}


/*@
@class XText
@parent XGraphicObject
@type overview
@symbol _
*/


/*@ XText :: XText(const XFont * f, const XRect * rect, const char *s, const SHORT sty)
@group constructors/destructors
@remarks Construct a text object, this constructor should only be used with XGraphicDevice, not with XPrinterDevice
@parameters   <t 'ø' c=2>
            øXFont * font               øfont to use
            øXRect * rect               ørectangle
            øchar * text                  øthe text to print
            øSHORT style               østyle, possible values are:
<BR>
TX_TOP
<BR>
TX_VCENTER
<BR>
TX_BOTTOM
<BR>
TX_LEFT
<BR>
TX_CENTER
<BR>
TX_RIGHT
<BR>
TX_WORDBREAK
            </t>
*/
XText :: XText(const XFont * f, const XRect * rect, const char *s, const SHORT sty):XGraphicObject(rect->GetX(), rect->GetY())
{
   font = (XFont *) f;
//   dev->SetFont(font);
   sh.x = sh.y = 0;
   an.x = an.y = 0;
   style = sty;
   if (s)
      text = s;
   p2.x = rect->cx + rect->x;
   p2.y = rect->cy + rect->y;
}


/*@ XText :: XText(const XFont * f, const XPoint * p, const char *s)
@group constructors/destructors
@remarks Construct a text object.
@parameters   <t 'ø' c=2>
            øXGraphicDevice * device      øowner
            øXFont * font               øfont to use
            øXPoint * point               øbase point
            øchar * text                  øthe text to print
            </t>
*/
XText :: XText(const XFont * f, const XPoint * p, const char *s):XGraphicObject(p)
{
   settings = 0;
   font = (XFont *) f;

   sh.x = sh.y = 0;
   an.x = an.y = 0;
   style = 0;
   p2.x = p2.y = 0;
   if (s)
      text = s;
}


void XText::GetTextBox(XGraphicDevice * dev, XRect * r)
{
   POINTL ptl[TXTBOX_COUNT];

   SetupDevice(dev);
   GpiQueryTextBox(dev->hps, text.GetLength(), (PSZ) (char*) text, TXTBOX_COUNT, ptl);
   r->SetX(ptl[TXTBOX_BOTTOMLEFT].x);
   r->SetWidth(ptl[TXTBOX_BOTTOMRIGHT].x - ptl[TXTBOX_BOTTOMLEFT].x);
   r->SetY(ptl[TXTBOX_BOTTOMLEFT].y);
   r->SetWidth(ptl[TXTBOX_TOPLEFT].y - ptl[TXTBOX_BOTTOMLEFT].y);
}


void XText::SetupDevice(XGraphicDevice * dev)
{
   XGraphicObject::SetupDevice(dev);
//////   if (dev->aktFont != font->fontId)
      dev->SetFont(font);

   GpiSetCharShear(dev->hps, &sh);
   GpiSetCharAngle(dev->hps, &an);
}


void XText::Draw( XGraphicDevice * dev)
{
   if (!(settings & GO_HIDE))
   {
      SetupDevice(dev);

      if (p2.GetX() == 0)
      {
         POINTL pnt;

         pnt.x = p.GetX();
         pnt.y = p.GetY();
         GpiCharStringAt(dev->hps, &pnt, text.GetLength(), (PSZ) (char*) text);
      }
      else
      {
         RECTL rc;

         rc.yBottom = p.y + dev->yOffset;
         rc.yTop = p2.y + dev->yOffset;
         rc.xLeft = p.x + dev->xOffset;
         rc.xRight = p2.x + dev->xOffset;
         char *t = text;

         if (style & DT_WORDBREAK)
         {
            int drawn = 0, totalDrawn, length = text.GetLength();

            for (totalDrawn = 0; totalDrawn < length; rc.yTop -= (LONG) (font->realSize))
            {
               drawn = WinDrawText(dev->hps, length - totalDrawn, (PSZ) t + totalDrawn, &rc, color.GetColor(), 0, style);
               if (drawn)
                  totalDrawn += drawn;
               else
                  break;
            }
         }
         else
            WinDrawText(dev->hps, text.GetLength(), (PCH) t, &rc, color.GetColor(), 0, style);
      }
      return;
   }
}


/*@
@class XBox
@parent XLine
@type overview
@symbol _
*/


/*@ XBox :: XBox(const XPoint * pp1, const XPoint * pp2, const BOOL f)
@group constructors/destructors
@remarks Construct a visible rectangle/box
@parameters   <t 'ø' c=2>
            øXPoint * p1                  øleft-lower corner
            øXPoint * p2                  øright-upper corner
            øBOOL fill                  øTRUE=fill box, FALSE=donït fill
            </t>
*/
XBox :: XBox(const XPoint * pp1, const XPoint * pp2, const BOOL f):XLine(pp1, pp2)
{
   settings = 0;
   fill = f;
}


void XBox::Draw(XGraphicDevice * dev)
{
   if (!(settings & GO_HIDE))
   {
      XLine::SetupDevice(dev);

      POINTL ptl[5], po;

      po.x = p.x;
      po.y = p.y;
      ptl[0].x = ptl[4].x = p.x;
      ptl[0].y = ptl[4].y = p.y;
      ptl[1].x = p.x;
      ptl[1].y = ptl[2].y = height + p.y;
      ptl[3].x = ptl[2].x = width + p.x;
      ptl[3].y = p.y;

      GpiBeginPath(dev->hps, 1);
      GpiMove(dev->hps, &po);
      GpiPolyLine(dev->hps, 5, ptl);
      GpiEndPath(dev->hps);

      if (fill)
         GpiFillPath(dev->hps, 1, FPATH_ALTERNATE);
      else
         GpiOutlinePath(dev->hps, 1, 0);
//          GpiStrokePath( dev->hps, 1, 0);
   }
}


char *pszdata[8] = {"Display", NULL, NULL, NULL, NULL, NULL, NULL};
SIZEL g = {0, 0};



/*@
@class XBitmap
@parent XGraphicObject
@type overview
@symbol _
*/


/*@ XBitmap: : XBitmap(const XWindow * w)
@group constructors/destructors
@remarks Construct a bitmap. Dont use this constructor for usage with a graphic device.
@parameters   XWindow * owner      window to draw to
*/
XBitmap :: XBitmap(const XWindow * w):XGraphicObject(NULL)
{
   owner = (XWindow *) w;
   hps = 0;
   hdc = 0;
   hbm = 0;
   mode = ROP_SRCCOPY;
   width = height = cx = cy = 0;
}


/*@ XBitmap :: XBitmap(const XPoint * pp1, const LONG m)
@group constructors/destructors
@remarks Construct a bitmap
@parameters   <t 'ø' c=2>
            øXPoint * p                  øleft-lower corner
            øLONG mode                  ømode, default is ROP_SRCCOPY
            </t>
@exceptions   If the method fails an exception of the type XException is thrown.
*/
XBitmap :: XBitmap(const XPoint * pp1, const LONG m):XGraphicObject(pp1)
{
   mode = m;
   owner = NULL;
   hdc = DevOpenDC(XApplication::GetApplication()->GetAnchorBlock(), OD_MEMORY, (PSZ) "*", 0, (PDEVOPENDATA) NULL, 0L);
   if (hdc == 0)
      OOLThrow("error creating device context", 4);
   hps = GpiCreatePS(XApplication::GetApplication()->GetAnchorBlock(), hdc, &g, PU_PELS | GPIA_ASSOC);
   if (hps == 0)
      OOLThrow("error creating presentation space", 5);
   width = height = cx = cy = 0;
}


void XBitmap::Draw( XGraphicDevice * dev)
{
   if (!(settings & GO_HIDE))
   {
      POINTL aptl[4];

      aptl[0].x = p.GetX() - dev->xOffset;
      aptl[0].y = p.GetY() - dev->yOffset;
      aptl[1].x = aptl[0].x + width;
      aptl[1].y = aptl[0].y + height;
      aptl[2].x = aptl[2].y = 0;
      aptl[3].x = cx;
      aptl[3].y = cy;
      GpiSetBitmap(hps, 0);
      GpiWCBitBlt(dev->hps, hbm, 4L, (PPOINTL) aptl, mode, BBO_IGNORE);
   }
}


XBitmap :: ~XBitmap()
{
   GpiDeleteBitmap(hbm);
   GpiDestroyPS(hps);
   DevCloseDC(hdc);
   hbm = 0;
   hps = 0;
   hdc = 0;
}


/*@ XBitmap::GetDimensions(XSize*)
@group misc
@remarks Returns the dimensions
@parameters   XSize * size      buffer to hold the size
*/
void XBitmap::GetDimensions( XSize * size) const
{
   BITMAPINFOHEADER phd;

   GpiQueryBitmapParameters(hbm, &phd);
   size->SetWidth( phd.cx );
   size->SetHeight( phd.cy );
}


void XBitmap::Load( const ULONG id, const XResourceLibrary * lib)
{
   if (hbm)
      GpiDeleteBitmap(hbm);
   if (owner && hps == 0)
      hps = WinGetPS(owner->GetHandle());
   hbm = GpiLoadBitmap(hps, lib->GetModuleHandle(), id, 0, 0);

   if (hbm == GPI_ERROR)
   {
       ULONG error = WinGetLastError(XApplication::GetApplication()->GetAnchorBlock());
      OOLThrow("couldnït load bitmap", error);
   }

   XSize s;
   GetDimensions(&s);
   width = cx = s.GetWidth();
   height = cy = s.GetHeight();
}


/*@ XBitmap::Load(const XResource * r)
@group loading a bitmap
@remarks load a bitmap from a resource library
@parameters   XResource * resource
*/
void XBitmap::Load(const XResource * r)
{
   Load( r->GetID(), r->GetResourceLibrary());
}


void XBitmap::Load(const ULONG id)
{
   Load( id, XApplication::GetApplication()->GetResourceLibrary());
}


/*@ XBitmap :: Fill( XColor * color)
@remarks Does not work
*/
void XBitmap :: Fill( XColor * color)
{
//   LONG line[15];

//   for( int i = 0; i < 15; i++)
//      line[i] = color->GetColor();
   GpiSetBitmap( hps, hbm);
   GpiFloodFill( hps, FF_SURFACE, color->GetColor());
}


XBitmap XBitmap::operator = (const XBitmap & b)
{
/****
   BITMAPINFOHEADER2 * head;

   head = (BITMAPINFOHEADER2 *) malloc( sizeof(BITMAPINFOHEADER2));
   owner = b.owner;

   if (owner && hps == 0)
      hps = WinGetPS(owner->GetHandle());

   if (hbm != 0)
      GpiDeleteBitmap(hbm);

   head->cbFix = sizeof(BITMAPINFOHEADER2);

   GpiSetBitmap(b.hps, b.hbm);
   if( GpiQueryBitmapInfoHeader(b.hbm, head) == FALSE)
      OOLThrow("error copying bitmap", 3);

   LONG offBits = sizeof(BITMAPINFO2) + (sizeof(RGB2) * (1 << head->cBitCount));

   LONG size = (((head->cBitCount * head->cx) + 31) / 32) * 4 * head->cPlanes * head->cy;

   head = (BITMAPINFOHEADER2 *) realloc(head, offBits);

   void * buffer = malloc(size);
   if( GpiQueryBitmapBits(b.hps, 0, head->cy, (PBYTE) buffer, (BITMAPINFO2 *) head) == GPI_ALTERROR)
      OOLThrow("error query bitmap data", 3);

//   hbm = GpiCreateBitmap(hps, (PBITMAPINFOHEADER2) &p->bmp, CBM_INIT, (PBYTE) p + p->offBits, (PBITMAPINFO2) &p->bmp);
   hbm = GpiCreateBitmap(hps, (PBITMAPINFOHEADER2) head, CBM_INIT, (PBYTE) buffer, (PBITMAPINFO2) head);
   if(hbm == 0)
      XProcess::Beep(300,300);

   GpiSetBitmap(hps, hbm);

   XSize s;
   b.GetDimensions(&s);
   cx = s.GetWidth();
   cy = s.GetHeight();

   width = b.width;
   height = b.height;
   p = b.p;
   free(head);
   free(buffer);
   return *this;
****/
   BITMAPFILEHEADER2 * head;

   owner = b.owner;

   if (owner && hps == 0)
      hps = WinGetPS(owner->GetHandle());

   if (hbm != 0)
      GpiDeleteBitmap(hbm);

   head = (BITMAPFILEHEADER2 *) malloc(sizeof(BITMAPFILEHEADER2));
   head->bmp2.cbFix = sizeof(BITMAPINFOHEADER2);

   GpiQueryBitmapInfoHeader(b.hbm, &head->bmp2);
   head->offBits = sizeof(BITMAPFILEHEADER2) + (sizeof(RGB2) * (1 << head->bmp2.cBitCount));
   LONG size = head->offBits + (((head->bmp2.cBitCount * head->bmp2.cx) + 31) / 32) * 4 * head->bmp2.cPlanes * head->bmp2.cy;

   head = (BITMAPFILEHEADER2 *) realloc(head, size);

   head->usType = BFT_BMAP;
   head->xHotspot = head->yHotspot = 0;
   head->cbSize = sizeof(BITMAPFILEHEADER2);
   GpiSetBitmap(hps, b.hbm);
/***
head->bmp2.cbFix=16;
head->bmp2.cPlanes =1;
head->bmp2.cBitCount = 1;
****/
   GpiQueryBitmapBits(hps, 0, head->bmp2.cy, (PBYTE) head + head->offBits, (BITMAPINFO2 *) &head->bmp2);
   hbm = GpiCreateBitmap(hps, (PBITMAPINFOHEADER2) &head->bmp2, CBM_INIT, (PBYTE) head + head->offBits, (PBITMAPINFO2) &head->bmp2);

   GpiSetBitmap(hps, hbm);

   XSize s;
   b.GetDimensions(&s);
   cx = s.GetWidth();
   cy = s.GetHeight();

   width = b.width;
   height = b.height;
   p = b.p;
   free(head);

   return *this;
}


LONG XBitmap::SetData(const BITMAPINFOHEADER2 * p, const LONG offset)
{
   LONG cScans;
   BITMAPINFOHEADER2 *data;

   LONG size = (((((BITMAPINFOHEADER2 *) p)->cBitCount * ((BITMAPINFOHEADER2 *) p)->cx) + 31) / 32) * 4 * ((BITMAPINFOHEADER2 *) p)->cPlanes * ((BITMAPINFOHEADER2 *) p)->cy;

   data = (PBITMAPINFOHEADER2) malloc(size);
   memcpy(data, p, size);

   if (data->cbFix == sizeof(BITMAPINFOHEADER))
      cScans = (ULONG) ((PBITMAPINFOHEADER) data)->cy;
   else
      cScans = data->cy;

   if (hbm != 0)
      GpiDeleteBitmap(hbm);

   hbm = GpiCreateBitmap(hps, data, CBM_INIT, (PBYTE) p + offset, (PBITMAPINFO2) data);

   if (hbm == GPI_ERROR)
      OOLThrow("error creating bitmap", 3);

   free(data);

   return 0;
}


/*@ XBitmap::Copy(const XGraphicDevice * d, const XPoint * p)
@group misc
@remarks copy the content of the bitmap to a graphic device
@parameters
<t 'ø' c=2>
øXGraphicDevice * øtarget
øXPoint * øpoint to copy to
</t>
*/
void XBitmap::Copy(const XGraphicDevice * d, const XPoint * p)
{
   POINTL aptl[4];

   aptl[0].x = 0;
   aptl[0].y = 0;
   aptl[1].x = width;
   aptl[1].y = height;
   aptl[2].x = p->GetX();
   aptl[2].y = p->GetY();
   GpiBitBlt(hps, d->hps, 3, aptl, mode, BBO_IGNORE);
}

BOOL XBitmap::HitTest(const LONG x, const LONG y)
{
   if (x > p.GetX() && x < p.GetX() + width && y > p.GetY() && y < p.GetY() + height)
      return TRUE;
   else
      return FALSE;
}


/*@ XText::SetText()
@group misc
@remarks Set text to draw
@parameters   char * theText
*/


/*@ XText::SetStyle()
@group style
@remarks Set the style to use (not with XPrinterDevice)
@parameters   LONG style, possible values are:
<UL>
<LI>TX_TOP
<LI>TX_VCENTER
<LI>TX_BOTTOM
<LI>TX_LEFT
<LI>TX_CENTER
<LI>TX_RIGHT
<LI>TX_WORDBREAK
</UL>
*/


/*@ XText::GetStyle()
@group style
@remarks Query the current style
@returns LONG theStyle
*/


/*@ XBitmap::SetOutputSize()
@group misc
@remarks Set the size of the bitmap to draw. If it is diferent from it physical size, the bitmap will be stretched.
@parameters   XRect * rect   new rect (only width and height will be used)
*/


/*@ XGraphicObject::Move()
@group position/size
@remarks Move the object to a new place
@parameters   XPoint * p                  point to move to
*/


/*@ XGraphicObject::SetPattern()
@group misc
@remarks Set a fill-patern for the object
@parameters   SHORT patternIndex      index of fill-patern (0-255)
*/


/*@ XGraphicObject::SetColor()
@group color
@remarks Set the color
@parameters   XColor * color      the new color
*/


/*@ XGraphicObject::GetColor()
@group color
@remarks Get the current color
@parameters   XColor * color      buffer to hold the color
*/


/*@ XFont::GetPixelSize()
@group misc
@remarks Returns the used count of pixels for the height of a single character. This size may differ
from the logical size, escpacialy on a printer-device.
@returns SHORT pixels
*/



/*@ XLine::SetLineWidth()
@group settings
@remarks Set the width of the line.
@parameters SHORT width
*/


/*@ XLine::SetLineJoin()
@group settings
@remarks Set the t<pe of the joining of lines.
@parameters CHAR type
*/

/*@ XLine::SetLineType()
@group settings
@remarks Set the type of the line.
@parameters CHAR type
*/

/*@ XLine::SetLineEnd()
@group settings
@remarks Set the type of line ending.
@parameters CHAR type
*/

/*@ XBitmap::LoadBMP(const char* filename)
@group loading a bitmap
@remarks Load a bitmap from a file. This method works faster than Load() but can only load bitmpas in OS2-BMP format
@parameters   char * fileName   filename of the file to load
@exceptions   If the method fails to create a new bitmap an exception of the type XException is thrown.
*/
void XBitmap :: LoadBMP(const char* filename)
{
   if (hbm)
      GpiDeleteBitmap(hbm);
   hbm = 0;

   XFile file;
   PBITMAPFILEHEADER p;
   if (file.Open(filename, XFILE_FAIL_IF_NEW | XFILE_OPEN_EXISTING, XFILE_READONLY, XFILE_SHARE_DENYNONE ) == 0)
   {
      XFileInfo info;
      file.GetFileInfo(&info);
      file.Seek(0, XFILE_BEGIN);
      p = (PBITMAPFILEHEADER) malloc(info.GetFileSize());
      file.Read(p, info.GetFileSize());
      file.Close();
   }
   else
      OOLThrow( "couldnït open file!", -1);

   if (owner )
   {
      hps = WinGetPS(owner->GetHandle());
      hbm = GpiCreateBitmap(hps, (PBITMAPINFOHEADER2) &p->bmp, CBM_INIT, (PBYTE) p + p->offBits, (PBITMAPINFO2) &p->bmp);
      if(hbm == 0)
         OOLThrow("error creating bitmap", 3);
   }
   else
      SetData((BITMAPINFOHEADER2 *) &p->bmp, p->offBits);
   free(p);
   XSize s;
   GetDimensions(&s);
   width = cx = s.GetWidth();
   height = cy = s.GetHeight();

   if (hbm == GPI_ERROR)
   {
      ULONG error = WinGetLastError(XApplication::GetApplication()->GetAnchorBlock());
      OOLThrow("couldnït load bitmap", error);
   }

   return;
}


XBitmapFilterProfile* __XBMFProfile__ = NULL;
UCHAR                 __XBMFErrorMsg__ [1024];

#ifndef __IBMCPP__
typedef ULONG _System _export_ XGLibProcError (PULONG, XBitmapFilter*, PSZ, ULONG);
typedef ULONG _System _export_ XGLibProcProfInit (XBitmapFilterProfile*, ULONG, ULONG, ULONG);
typedef ULONG _System _export_ XGLibProcFltInit (const char*, XBitmapFilterProfile*, XBitmapFilter*, ULONG);
typedef ULONG _System _export_ XGLibProcSetup (HWND, const char*, XBitmapFilterProfile*, XBitmapFilter*);
typedef ULONG _System _export_ XGLibProcIO (const char*, XBitmapFilterProfile*, XBitmapFilter*, ULONG);
#else
typedef ULONG _System XGLibProcError (PULONG, XBitmapFilter*, PSZ, ULONG);
typedef ULONG _System XGLibProcProfInit (XBitmapFilterProfile*, ULONG, ULONG, ULONG);
typedef ULONG _System XGLibProcFltInit (const char*, XBitmapFilterProfile*, XBitmapFilter*, ULONG);
typedef ULONG _System XGLibProcSetup (HWND, const char*, XBitmapFilterProfile*, XBitmapFilter*);
typedef ULONG _System XGLibProcIO (const char*, XBitmapFilterProfile*, XBitmapFilter*, ULONG);
#endif
/*@ XBitmap::Load(const char* filename, XBitmapFilter * params, BOOL releaseParams)
@group loading a bitmap
@remarks Load a bitmap from a file. <P><I>If this method is used, GBM.DLL and OLLGLIB.DLL must be in the LIBPATH.</I> <P>Due a bug (?) in OS/2-GPI you should specify params->cBitCount = BFP_BPP_TRUECOLOR if you want to modify or resave the bitmap.
@parameters    <t 'ø' c=2>
               øchar * fileName           øfilename of the file to load
               øXBitmapFilter * params    øfiletype specific filter informations. An initialized structure has to be created by XBitmap :: GetImportFilterStruct () or XGLibFileDialog. (default is NULL)
               øBOOL releaseParams        øfilter informations will be released after usages
               <\t>
@exceptions   If the method fails to create a new bitmap an exception of the type XException is thrown.
*/
void XBitmap :: Load (const char* filename, XBitmapFilter* params, BOOL releaseParams)
{
   if (hbm) GpiDeleteBitmap(hbm); hbm = NULLHANDLE;

   XResourceLibrary* lib = new XResourceLibrary ("OOLGLIB");
   XGLibProcIO* func = (XGLibProcIO*) lib->LoadFunction ("OOL_GLIB_IMP");
   DosSleep(100);

   if (NULL != func) {
      if (owner) hps = WinGetPS (owner->GetHandle());

      ULONG rc;
      if (NULL == params) params = GetImportFilterStruct (filename);
      params->hps = hps;
      params->hbm = hbm;
      params->hab = XApplication :: GetApplication ()->GetAnchorBlock ();
      if (BFE_OK != (rc = func (filename, __XBMFProfile__, params, 0))) {
         if (releaseParams) ReleaseFilterStruct (params);
         OOLThrow (GetLastErrorMessage (lib, &rc, params), rc);
      }
      hbm = params->hbm;
      if (releaseParams) ReleaseFilterStruct (params);
      lib->UnLoadFunction ((PFNWP) func);
   } else {
      if (releaseParams) ReleaseFilterStruct (params);
      ULONG error = WinGetLastError (XApplication :: GetApplication ()->GetAnchorBlock ());
      OOLThrow("Could not load function \"OOL_GLIB_IMP\" from library \"OOLGLIB.DLL\".", error);
   }

   delete lib;
   XSize s;
   GetDimensions(&s);
   width = cx = s.GetWidth();
   height = cy = s.GetHeight();
}

/*@ XBitmap::Save(const char* filename, XBitmapFilter * params, BOOL releaseParams)
@group saving a bitmap
@remarks Save a bitmap from a file. <P><I>If this method is used, GBM.DLL and OLLGLIB.DLL must be in the LIBPATH.</I> <P>Due a bug (?) in OS/2-GPI the bitmap should be in 24-bit-format.
@parameters    <t 'ø' c=2>
               øchar * fileName           øfilename of the file to load
               øXBitmapFilter * params    øfiletype specific filter informations. An initialized structure has to be created by XBitmap :: GetImportFilterStruct () or XGLibFileDialog. (default is NULL)
               øBOOL releaseParams        øfilter informations will be released after usages
               <\t>
@exceptions   If the method fails to create a new bitmap an exception of the type XException is thrown.
*/
void XBitmap :: Save (const char* filename, XBitmapFilter* params, BOOL releaseParams)
{
   XResourceLibrary* lib = new XResourceLibrary ("OOLGLIB");
   XGLibProcIO* func = (XGLibProcIO*) lib->LoadFunction ("OOL_GLIB_EXP");
   DosSleep(100);

   if (NULL != func) {
      ULONG rc;
      if (NULL == params) params = GetExportFilterStruct (filename);
      params->hps = hps;
      params->hbm = hbm;
      params->hab = XApplication :: GetApplication ()->GetAnchorBlock ();
      if (BFE_OK != (rc = func (filename, __XBMFProfile__, params, 0))) {
         if (releaseParams) ReleaseFilterStruct (params);
         OOLThrow (GetLastErrorMessage (lib, &rc, params), rc);
      }
      if (releaseParams) ReleaseFilterStruct (params);
      lib->UnLoadFunction ((PFNWP) func);
   } else {
      if (releaseParams) ReleaseFilterStruct (params);
      ULONG error = WinGetLastError (XApplication :: GetApplication ()->GetAnchorBlock ());
      OOLThrow("Could not load function \"OOL_GLIB_EXP\" from library \"OOLGLIB.DLL\".", error);
   }

   delete lib;
}

/*@ XBitmap :: SetupImportFilter (const XWindow * owner, const char * filename, XBitmapFilter * params)
@group loading a bitmap
@remarks Opens an dialog which allows you to modify the parameters of bitmap-import-filters interactivly. <P><I>If this method is used, GBM.DLL, OLLGLIB.DLL and OOLRES.DLL must be in the LIBPATH.</I>
@parameters    <t 'ø' c=2>
               øXWindow * owner           øowner of the dialog
               øchar * filename           øfilename of the file to load (later)
               øXBitmapFilter * params    øfiletype specific filter informations. An initialized structure has to be created by XBitmap :: GetImportFilterStruct () or XGLibFileDialog. (default is NULL)
               <\t>
@exceptions   If the method fails to create a new bitmap an exception of the type XException is thrown.
*/
void XBitmap :: SetupImportFilter (const XWindow * owner, const char * filename, XBitmapFilter * params) {
   CheckFilterProfile ();

   if (NULL == owner)    OOLThrow("invalid parameters", 1);
   if (NULL == filename) OOLThrow("invalid parameters", 2);
   if (NULL == params)   OOLThrow("invalid parameters", 3);

   XResourceLibrary* lib = new XResourceLibrary ("OOLGLIB");
   XGLibProcSetup* func = (XGLibProcSetup*) lib->LoadFunction ("OOL_GLIB_IMP_SETUP");
   DosSleep(100);

   if (NULL != func) {
      ULONG rc;

      params->hab = XApplication :: GetApplication ()->GetAnchorBlock ();
      if (BFE_OK != (rc = func (owner->GetHandle (), filename, __XBMFProfile__, params)))
         OOLThrow (GetLastErrorMessage (lib, &rc, params), rc);

      lib->UnLoadFunction ((PFNWP) func);
   } else {
      ULONG error = WinGetLastError (XApplication :: GetApplication ()->GetAnchorBlock ());
      OOLThrow("Could not load function \"OOL_GLIB_IMP_SETUP\" from library \"OOLGLIB.DLL\".", error);
   }

   delete lib;
}

/*@ XBitmap :: SetupExportFilter (const XWindow * owner, const char * filename, XBitmapFilter * params)
@group saving a bitmap
@remarks Opens an dialog which allows you to modify the parameters of bitmap-export-filters interactivly. <P><I>If this method is used, GBM.DLL, OLLGLIB.DLL and OOLRES.DLL must be in the LIBPATH.</I>
@parameters    <t 'ø' c=2>
               øXWindow * owner           øowner of the dialog
               øchar * filename           øfilename of the file to load (later)
               øXBitmapFilter * params    øfiletype specific filter informations. An initialized structure has to be created by XBitmap :: GetImportFilterStruct () or XGLibFileDialog. (default is NULL)
               <\t>
@exceptions   If the method fails to create a new bitmap an exception of the type XException is thrown.
*/

void XBitmap :: SetupExportFilter (const XWindow* owner, const char* filename, XBitmapFilter* params) {
   CheckFilterProfile ();

   if (NULL == owner)    OOLThrow("invalid parameters", 1);
   if (NULL == filename) OOLThrow("invalid parameters", 2);
   if (NULL == params)   OOLThrow("invalid parameters", 3);

   XResourceLibrary* lib = new XResourceLibrary ("OOLGLIB");
   XGLibProcSetup* func = (XGLibProcSetup*) lib->LoadFunction ("OOL_GLIB_EXP_SETUP");
   DosSleep(100);

   if (NULL != func) {
      ULONG rc;

      params->hab = XApplication :: GetApplication ()->GetAnchorBlock ();
      if (BFE_OK != (rc = func (owner->GetHandle (), filename, __XBMFProfile__, params)))
         OOLThrow (GetLastErrorMessage (lib, &rc, params), rc);

      lib->UnLoadFunction ((PFNWP) func);
   } else {
      ULONG error = WinGetLastError (XApplication :: GetApplication ()->GetAnchorBlock ());
      OOLThrow("Could not load function \"OOL_GLIB_EXP_SETUP\" from library \"OOLGLIB.DLL\".", error);
   }

   delete lib;
}

/*@ XBitmap :: GetFilterExtensions ()
@group loading/saving a bitmap
@remarks Returns a NULL-terminated list of the file-extensions of all bitmap-formats supported by OOLGLIB. <P><I>If this method is used, GBM.DLL and OOLGLIB.DLL must be in the LIBPATH.</I>
@exceptions   If the method fails an exception of the type XException is thrown.
*/

char** XBitmap :: GetFilterExtensions () {
   CheckFilterProfile ();
   return (char**) __XBMFProfile__->pszExtension;
}

/*@ XBitmap :: GetFilteNames ()
@group loading/saving a bitmap
@remarks Returns a NULL-terminated list of the names of all bitmap-formats supported by OOLGLIB. <P><I>If this method is used, GBM.DLL and OOLGLIB.DLL must be in the LIBPATH.</I>
@exceptions   If the method fails an exception of the type XException is thrown.
*/

char** XBitmap :: GetFilterNames () {
   CheckFilterProfile ();
   return (char**) __XBMFProfile__->pszName;
}

/*@ XBitmap :: GetImportFilterStruct (const char* filename)
@group loading a bitmap
@remarks Returns the best matching import-filter for the file you specified. The structure returned is used a paramter for several OOLGLIB-functions. To setup the structure manually use the declarations found in oolglib.h.<P><I>If this method is used, GBM.DLL and OLLRES.DLL must be in the LIBPATH.</I>
@parameters    <t 'ø' c=2>
               øchar * filename           øfilename of the file to load (later)
               <\t>
@exceptions   If the method fails to create a new bitmap an exception of the type XException is thrown.
*/

XBitmapFilter* XBitmap :: GetImportFilterStruct (const char* filename) {
   XBitmapFilter* filter;
   XBitmapFilter  dummy;
   dummy.cbFix = 0;
   CheckFilterProfile ();

   XResourceLibrary* lib = new XResourceLibrary ("OOLGLIB");
   XGLibProcFltInit* func = (XGLibProcFltInit*) lib->LoadFunction ("OOL_GLIB_IMP_INIT");
   DosSleep(100);

   if (NULL != func) {
      ULONG rc;
      if (0 == (rc = func (filename, __XBMFProfile__, &dummy, 0)))
         OOLThrow (GetLastErrorMessage (lib, &rc, &dummy), rc);
      DosAllocMem ((PPVOID) &filter, rc, PAG_READ | PAG_WRITE | PAG_COMMIT);

      filter->cbFix = rc;
      filter->hab = XApplication :: GetApplication ()->GetAnchorBlock ();
      if (BFE_OK != (rc = func (filename, __XBMFProfile__, filter, 0)))
         OOLThrow (GetLastErrorMessage (lib, &rc, filter), rc);

      lib->UnLoadFunction ((PFNWP) func);
   } else {
      ULONG error = WinGetLastError (XApplication :: GetApplication ()->GetAnchorBlock ());
      OOLThrow("Could not load function \"OOL_GLIB_IMP_INIT\" from library \"OOLGLIB.DLL\".", error);
   }

   delete lib;
   return filter;
}

/*@ XBitmap :: GetExportFilterStruct (const char* filename)
@group saving a bitmap
@remarks Returns the best matching export-filter for the file you specified. The structure returned is used a paramter for several OOLGLIB-functions. To setup the structure manually use the declarations found in oolglib.h.<P><I>If this method is used, GBM.DLL and OLLRES.DLL must be in the LIBPATH.</I>
@parameters    <t 'ø' c=2>
               øchar * filename           øfilename of the file to load (later)
               <\t>
@exceptions   If the method fails to create a new bitmap an exception of the type XException is thrown.
*/

XBitmapFilter* XBitmap :: GetExportFilterStruct (const char* filename) {
   XBitmapFilter* filter;
   XBitmapFilter dummy;
   dummy.cbFix = 0;
   CheckFilterProfile ();

   XResourceLibrary* lib = new XResourceLibrary ("OOLGLIB");
   XGLibProcFltInit* func = (XGLibProcFltInit*) lib->LoadFunction ("OOL_GLIB_EXP_INIT");
   DosSleep(100);

   if (NULL != func) {
      ULONG          rc;
      if (0 == (rc = func (filename, __XBMFProfile__, NULL, 0)))
         OOLThrow (GetLastErrorMessage (lib, &rc, &dummy), rc);
      DosAllocMem ((PPVOID) &filter, rc, PAG_READ | PAG_WRITE | PAG_COMMIT);

      filter->cbFix = rc;
      filter->hab = XApplication :: GetApplication ()->GetAnchorBlock ();
      if (BFE_OK != (rc = func (filename, __XBMFProfile__, filter, 0)))
         OOLThrow (GetLastErrorMessage (lib, &rc, filter), rc);

      lib->UnLoadFunction ((PFNWP) func);
   } else {
      ULONG error = WinGetLastError (XApplication :: GetApplication ()->GetAnchorBlock ());
      OOLThrow("Could not load function \"OOL_GLIB_EXP_INIT\" from library \"OOLGLIB.DLL\".", error);
   }

   delete lib;
   return filter;
}

void XBitmap :: CheckFilterProfile () {
   if (NULL == __XBMFProfile__)  {
      XResourceLibrary*   lib = new XResourceLibrary ("OOLGLIB");
      XGLibProcProfInit* func = (XGLibProcProfInit*) lib->LoadFunction ("OOL_GLIB_INIT");
      XBitmapFilter     dummy;
      dummy.cbFix = 0;
      DosSleep(100);

      if (NULL != func) {
         ULONG rc;
         if (0 == (rc = func (NULL, 0, 0, 0))) {
            ULONG error = WinGetLastError (XApplication :: GetApplication ()->GetAnchorBlock ());
            OOLThrow (GetLastErrorMessage (lib, &rc, &dummy), rc);
         }
         DosAllocMem ((PPVOID) &__XBMFProfile__, rc, PAG_READ | PAG_WRITE | PAG_COMMIT);

         __XBMFProfile__->cbFix = rc;
         if (BFE_OK != (rc = func (__XBMFProfile__, 0, 0, 0))) {
            ULONG error = WinGetLastError (XApplication :: GetApplication ()->GetAnchorBlock ());
            OOLThrow (GetLastErrorMessage (lib, &rc, &dummy), rc);
         }
         lib->UnLoadFunction ((PFNWP) func);
      } else {
         ULONG error = WinGetLastError (XApplication :: GetApplication ()->GetAnchorBlock ());
         OOLThrow("Could not load function \"OOL_GLIB_INIT\" from library \"OOLGLIB.DLL\".", error);
      }
      delete lib;
   }
}

char* XBitmap :: GetLastErrorMessage (XResourceLibrary* lib, ULONG* pRC, XBitmapFilter* params) {
   XGLibProcError*  func = (XGLibProcError*) lib->LoadFunction ("OOL_GLIB_ERR");
   DosSleep(100);

   if (NULL != func) {
      ULONG rc;
      if (BFE_OK != (rc = func (pRC, params, (PSZ) __XBMFErrorMsg__, 1024)))
         OOLThrow ("Could not recive last error message from OOLGLIB.", rc);
      lib->UnLoadFunction ((PFNWP) func);
   } else {
      ULONG error = WinGetLastError (XApplication :: GetApplication ()->GetAnchorBlock ());
      OOLThrow("Could not load function \"OOL_GLIB_ERR\" from library \"OOLGLIB.DLL\".", error);
   }
   return (char*) __XBMFErrorMsg__;
}

XBitmapFilterProfile* XBitmap :: GetProfile () {
   CheckFilterProfile ();
   return __XBMFProfile__;
}

/*@ XBitmap :: ReleaseFilterStruct (XBitmapFilter * params)
@group loading/saving a bitmap
@remarks Released bitmap-filters created by XBitmap :: GetImportFilter/GetExportFilter and XGLibFileDialog
@parameters    <t 'ø' c=2>
               øchar * filename           øfilename of the file to load (later)
               <\t>
@exceptions   If the method fails to create a new bitmap an exception of the type XException is thrown.
*/
void XBitmap :: ReleaseFilterStruct (XBitmapFilter * params) {
   int n;
   if(params)
   {
   if (BFT_TIFF == params->cFilterID)
      for (n = 0; n < BFP_TIFF_TAGCOUNT; n++) {
         free (((XTIFFExportFilter*) params)->pszTagValue [n]);
         ((XTIFFExportFilter*) params)->pszTagValue [n] = NULL;
      }
   DosFreeMem (params);
   } /* end if */
   params = NULL;
}
