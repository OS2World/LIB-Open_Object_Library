#include "XPie.h"
#include "XColor.h"
#include "xrect.h"
#include "xmsgbox.h"
#include "xstring.h"
#include "xbar.h"
#include "xtimesl.h"
#include <stdlib.h>


XChart::~XChart()
{
   int i;

   if (itemCount)
   {
      for (i = 0; i < itemCount; i++)
         free(item[i]);
      free(item);
   }
   delete graph;
}


XChart :: XChart(const XWindow * owner, const XRect& r, const LONG id, const LONG s, const char *t):XUserWindow(owner->GetHandle())
{
   WinSetWindowUShort(winhandle, QWS_ID, id);
   style = s;
   item = NULL;
   itemCount = 0;
   graph = new XGraphicDevice(this);
   if (t)
   {
      XRect titleRect( 3, 3, r.GetWidth() - 6, r.GetHeight() - 6);
      titleFont = new XFont(graph, "Helvetica", 10);
      title = new XText( titleFont, &titleRect, t, DT_CENTER | DT_BOTTOM);
      XColor col(COL_BLACK);
      title->SetColor(&col);
   }
   else
   {
      titleFont = NULL;
      title = NULL;
   }

   if (style & FS_BORDER)
   {
      XPoint p, p2;
      box = new XBox( &p, &p2, FALSE);
   }
   else
      box = NULL;

   SetSize(&r);
}


BOOL XChart::SetBackgroundColor(const XColor * c)
{
   graph->SetBackgroundColor(c);
   return TRUE;
}


void XChart::GetBackgroundColor(XColor * c) const
{
   graph->GetBackgroundColor(c);
}


void XChart::SetText(const char *text)
{
   title->SetText(text);
}


void XChart::SetSize(const XRect * r) const
{
   if (box)
   {
      box->SetHeight(r->GetHeight() - 1);
      box->SetWidth(r->GetWidth() - 1);
   }
   if (title)
   {
      XRect titleRect( 3, 3, r->GetWidth() - 6, r->GetHeight() - 6);
      title->SetSize(&titleRect);
   }
   XWindow::SetSize(r);
}


void XChart::SetSize(const ULONG xPos, const ULONG yPos, const ULONG width, const ULONG height) const
{
   if (box)
   {
      box->SetHeight(height - 1);
      box->SetWidth(width - 1);
   }
   if (title)
   {
      XRect titleRect(3, 3, width - 6, height - 6);
      title->SetSize(&titleRect);
   }
   XWindow::SetSize(xPos, yPos, width, height);
}


void XPie::SetItemColor(const USHORT index, const XColor * col)
{
   if (index < itemCount)
      arc[index]->SetColor(col);
}


void XChart::SetItemValue(const USHORT index, const double value)
{
   if (index < itemCount)
      item[index]->value = value;
}


double XChart::GetItemValue(const USHORT index)
{
   if (index < itemCount)
      return item[index]->value;
   return 0;
}


void XChart::SetItemCount(const USHORT count)
{
   int i;

   if (itemCount)
   {
      for (i = 0; i < itemCount; i++)
         free(item[i]);
      free(item);
   }
   itemCount = count;
   item = (chartItem **) malloc(count * sizeof(void *));
   for (i = 0; i < itemCount; i++)
   {
      item[i] = (chartItem *) malloc(sizeof(chartItem));
      item[i]->value = 0;
   }
}


XPie :: XPie(const XWindow * w, const XRect& r, const LONG id, const LONG s, const char *t):XChart(w, r, id, s, t)
{
   arc = NULL;
}


void XPie::GetItemColor(const USHORT index, XColor * col)
{
   if (index < itemCount)
      arc[index]->GetColor(col);
}


void XPie::SetItemCount(const USHORT count)
{
   int i;
   XRect r;

   GetSize(&r);

   if (arc)
      layer.Empty(TRUE);

   XChart::SetItemCount(count);

   arc = (XArc **) realloc(arc, itemCount * sizeof(void *));
   for (i = 0; i < itemCount; i++)
   {
      XPoint p(r.GetWidth() / 2, r.GetHeight() / 2 + 10);

      arc[i] = new XArc( &p, r.GetWidth() / 2 - 15, 0, 0, TRUE, TRUE);
      XColor col(i + 3);
      arc[i]->SetColor(&col);
      layer.AddObject(arc[i]);
   }
}


void XPie::Calculate(void)
{
   double all = 0;
   int i;

   for (i = 0; i < itemCount; i++)
      all += item[i]->value;
   if (all == 0)
      return;

   float start = 0, akt;

   for (i = 0; i < itemCount; i++)
   {
      akt = (item[i]->value * 360) / all;
      arc[i]->SetStart((USHORT) start);
      arc[i]->SetWidth((USHORT) akt);
      start += akt;
   }
}


XBar :: XBar(const XWindow * w, const XRect& rec, const LONG id, const LONG s, const char *t):XChart(w, rec, id, s, t)
{
   box = NULL;
}


void XBar::SetItemCount(const USHORT count)
{
   int i;
   XRect r;

   GetSize(&r);

   if (box)
      layer.Empty(TRUE);

   XChart::SetItemCount(count);

   box = (XBox **) realloc(box, itemCount * sizeof(void *));
   SHORT width = (r.GetWidth() - 2) / itemCount;

   for (i = 0; i < itemCount; i++)
   {
      XPoint p(i * width + 1, 1);
      XPoint p2((i + 1) * width, r.GetHeight() - 2);

      box[i] = new XBox( &p, &p2, TRUE);
      XColor col(i + 1);
      layer.AddObject(box[i]);
      box[i]->SetColor(&col);
   }
}


void XBar::SetItemColor(const USHORT index, const XColor * col)
{
   if (index < itemCount)
      box[index]->SetColor(col);
}


void XBar::GetItemColor(const USHORT index, XColor * col)
{
   if (index < itemCount)
      box[index]->GetColor(col);
}


void XBar::Calculate(void)
{
   int i;
   float max = 0;

   XRect r;

   GetSize(&r);

   for (i = 0; i < itemCount; i++)
      if (item[i]->value > max)
         max = item[i]->value;

   for (i = 0; i < itemCount; i++)
      box[i]->SetHeight((SHORT) (item[i]->value / max * (r.GetHeight() - 21)));
}


XTimeSlice :: XTimeSlice(const XWindow * w, const XRect& r, const LONG id, const LONG s, const char *t):XChart(w, r, id, s, t)
{
   lines = NULL;
   maxValue = 100;
   width = 1;
}


void XTimeSlice::SetItemCount(const USHORT count)
{
   int i;
   XRect r;

   GetSize(&r);

   if(lines)
      layer.Empty();

   XChart::SetItemCount(count);

   free( lines );
   lines = (XLine**) malloc( (count-1) * sizeof(XLine*));

   LONG wid = r.GetWidth() / (itemCount-1);
   for(i=0; i < count-1; i++)
   {
      XPoint p1(wid * i, 10), p2(wid * (i+1), 10);
      lines[i] = new XLine( &p1, &p2);
      lines[i]->SetColor( &color );
      lines[i]->SetLineWidth( width );
      layer.AddObject(lines[i]);
   }
}


void XTimeSlice::Calculate(void)
{
   int i;
   XRect r;

   GetSize(&r);
   LONG lastY;
   for (i = 0; i < itemCount; i++)
   {
       XPoint p( r.GetWidth() / (itemCount-1) * i, (SHORT) (item[i]->value / maxValue * (r.GetHeight()-20) + 20));
      if( i < itemCount -1 )
         lines[i]->Move( &p );
      if( i > 0)
         lines[i-1]->SetHeight( (SHORT) ((item[i]->value / maxValue * (r.GetHeight()-20)) + 20 - lastY));
      lastY = p.GetY();
   }
}


void XTimeSlice::SetLineColor( const XColor * col)
{
   color = *col;
   if(lines)
   {
      for(int i = 0; i < itemCount-1; i++)
         lines[i]->SetColor( col );
   }
}


void XTimeSlice::SetLineWidth(const LONG w)
{
   width = w;
   if(lines)
   {
      for(int i = 0; i < itemCount-1; i++)
         lines[i]->SetLineWidth( w );
   }
}
