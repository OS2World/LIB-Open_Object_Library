#include "xuserbtn.h"
#include "xdefhdl.h"
#include "xmoushdl.h"
#include "xmousevn.h"
#include "xbitmap.h"
#include "XLine.h"
#include "xbox.h"
#include "xfont.h"


class userbtnhandler:public XDefaultHandler
{
   public:
      userbtnhandler(XWindow * w):XDefaultHandler(w) {;}
      BOOL HandleEvent(ULONG, void *, void *, LONG*);
};


class userbtnmousehandler:public XMouseHandler
{
   public:
      userbtnmousehandler(XWindow * w):XMouseHandler(w) {;}
      BOOL HandleEvent(XMouseEvent *);
};


/*@
@class XUserButton
@parent XSettingButton
@type overview
@symbol _
*/


/*@ XUserButton::XUserButton( const XWindow * w, const XRect& r, const LONG id, const LONG st, const char * t, const SHORT yOff)
@group constructors/destructors
@remarks Constructs a userbuttom
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner
            øXRect& rect         øthe rectangle
            øUSHORT id            øid of the window
            øULONG style          østyle, valid values are:
<BR>BU_HELP
<BR>BU_DEFAULT
<BR>BU_NOPOINTERFOCUS
<BR>BU_NOCURSORSELECT
<BR>BU_NOBORDER
<BR>BU_TWOSTATE
<BR>
(can be or-ed).
            øconst char * string  øtext to display (default is NULL)
            øSHORT yOffset        øan y-offset for the bitmap (useful if you use bimaps and text, default is NULL)
            </t>
*/
XUserButton :: XUserButton(const XWindow * w, const XRect& r, const LONG id, const LONG st, const char *t, const SHORT yOff):XSettingButton(w, &r, id, st | BS_PUSHBUTTON | BS_USERBUTTON, (const char *) "", (unsigned char *) WC_BUTTON)
{
   graph = NULL;
   bmp[0] = bmp[1] = NULL;
   isPressed = 0;
   style = st;
   graph = new XGraphicDevice(this, FALSE, TRUE, PU_PELS, GPIF_LONG | GPIA_ASSOC);
   XColor colPaleGray(COL_PALEGRAY);

   graph->SetBackgroundColor(&colPaleGray);
   bmp[0] = new XBitmap();
   bmp[1] = new XBitmap();

   XPoint p1(1, 2), p2(1, 0), p3, p4(1, 2), pa(0, 0), pb;

   p2.SetY(r.GetHeight() - 1);
   p3.SetY(p2.GetY());
   p3.SetX(r.GetWidth() - 1);
   p4.SetX(p3.GetX());
   pb.SetX(r.GetWidth());
   pb.SetY(r.GetHeight());
   XFont *font = new XFont(graph, "Helvetica", 9);

   XRect rect;

   GetSize(&rect);
   rect.SetX(2);
   rect.SetY(2);
   rect.SetWidth(rect.GetWidth() - 2);
   rect.SetHeight(rect.GetHeight() - 2);
   text = new XText(font, &rect, t, DT_BOTTOM | DT_CENTER);

   if (style & BU_TWOSTATE)
      userbtnmousehandler *m = new userbtnmousehandler(this);

   else
      userbtnhandler * u = new userbtnhandler(this);
}


void XUserButton::GetText(XString * buffer) const
{
   text->GetText(buffer);
}


void XUserButton::SetText(const char *t)
{
   text->SetText(t);
}

//#include "XProcess.h"
XUserButton :: ~XUserButton()
{
   delete bmp[0];
   delete bmp[1];
   WinSetWindowULong(winhandle, QWL_USER, 0);
   delete graph;
}


void XUserButton::Draw(void)
{
   if (graph)
   {
      graph->FillBackground();
      bmp[0]->Draw(graph);
      bmp[1]->Draw(graph);
      text->Draw(graph);
   } /* end if */
//      graph->Draw();
   RECTL rect;
   WinQueryWindowRect(winhandle, &rect);

   if (!(style & BS_NOBORDER)) {
      HPS hps = graph->GetHPS();
      if(isPressed)
         WinDrawBorder(hps, &rect, 2,2,0,0,DB_PATCOPY|DB_DEPRESSED);
      else
         WinDrawBorder(hps, &rect, 2,2,0,0,DB_PATCOPY|DB_RAISED);
   }
}


/*@ XUserButton :: IsSelected( void )
@group misc
@remarks Query if the button is selected or not
@returns BOOL selection
*/
BOOL XUserButton::IsSelected(void) const
{
   if (!(style & BU_TWOSTATE))
      return XSettingButton::IsSelected();
   else
      return isPressed;
}


/*@ XUserButton::Toggle(void)
@group misc
@remarks Toggles the button
*/
void XUserButton::Toggle(void)
{
   if (!(style & BU_TWOSTATE))
   {
      BOOL res = (XSettingButton::IsSelected() ? FALSE : TRUE);
      XSettingButton::Select(res);
   }
   else
   {
      isPressed = (isPressed ? FALSE : TRUE);
      Invalidate();
   }
}


/*@ XUserButton :: Select( const BOOL sel )
@group misc
@remarks Select te button
@parameters BOOL selection      TRUE=select, FALSE=unselect
*/
void XUserButton::Select( const BOOL sel)
{
   if (!(style & BU_TWOSTATE))
      XSettingButton::Select(sel);
   else
   {
      isPressed = sel;
      Invalidate();
   }
}


/*@ XUserButton::GetBitmapPointer()
@group style
@remarks Returns a pointer to one of two used bitmaps.
@parameters   <t 'ø' c=2>
            øUCHAR      index      øindex of the bitmap:
<BR>
                              0 = the bitmap to display if the button is not pressed
<BR>
                              1 = the bitmap to display if the button is pressed
            </t>
@returns   XBitmap * bmp         the requested bitmap
*/


/*@ XUserButton::GetGraphPointer()
@group style
@remarks Returns a pointer to the graphic-device of the button.
@returns   XGraphicDevice * dev         the graphic device
*/


/*@ XUserButton::GetTextPointer()
@group style
@remarks Returns a pointer to used text-object of the button.
@returns   XText * text         the text object
*/


/*@ XUserButton::SetBitmap()
@group style
@remarks Set a bitmap to the button. The bitmap is copied so the destructor of the bitmap given in
the argument can be called when this functions returns.
@parameters   <t 'ø' c=2>
            øXBitmap * bmp      øthe bitmap
            øUCHAR      index      øindex of the bitmap:
<BR>
                              0 = the bitmap to display if the button is not pressed
<BR>
                              1 = the bitmap to display if the button is pressed
            /t>
*/
void XUserButton::SetBitmap(const XBitmap * b, const UCHAR index)
{
   if (index > 1)
      return;
   *bmp[index] = *b;
   XPoint p(1, 1);

   bmp[index]->Move(&p);

   if (index == 1)
   {
      bmp[1]->Show(FALSE);
      bmp[0]->Show(TRUE);
   }
   Invalidate();
}


void XUserButton::Setup(void)
{
/*
   XColor colWhite(COL_WHITE);
   XColor colGray(COL_DARKGRAY);
*/
   if (isPressed)
   {
      XPoint p;

      text->GetPos(&p);
      p.SetX(p.GetX() + 1);
      p.SetY(p.GetY() - 1);
      text->Move(&p);

      if (bmp[1]->cx == 0)
      {
         bmp[0]->GetPos(&p);
         p.SetX(p.GetX() + 1);
         p.SetY(p.GetY() - 1);
         bmp[0]->Move(&p);
      }
      else
      {
         bmp[0]->Show(FALSE);
         bmp[1]->Show();
      }
/*
      line1->SetColor(&colGray);
      line2->SetColor(&colGray);
      line3->SetColor(&colWhite);
      line4->SetColor(&colWhite);
*/
   }
   else
   {
      XPoint p;

      text->GetPos(&p);
      p.SetX(p.GetX() - 1);
      p.SetY(p.GetY() + 1);
      text->Move(&p);

      if (bmp[1]->cx == 0)
      {
         bmp[0]->GetPos(&p);
         p.SetX(p.GetX() - 1);
         p.SetY(p.GetY() + 1);
         bmp[0]->Move(&p);
      }
      else
      {
         bmp[0]->Show();
         bmp[1]->Show(FALSE);
      }
/*
      line1->SetColor(&colWhite);
      line2->SetColor(&colWhite);
      line3->SetColor(&colGray);
      line4->SetColor(&colGray);
*/
   }
   Invalidate();
}


BOOL userbtnhandler::HandleEvent(ULONG msg, void *mp1, void *, LONG * retVal)
{
   if (msg == BM_SETHILITE)
   {
      XUserButton *b = (XUserButton *) GetWindow();

      b->isPressed = SHORT1FROMMP(mp1);
      b->Setup();
      b->Invalidate();
   }
   return FALSE;
}


BOOL userbtnmousehandler::HandleEvent(XMouseEvent * event)
{
   if (event->GetEventID() == MOU_BTN1UP)
   {
      XUserButton *b = (XUserButton *) GetWindow();

      b->isPressed = (b->buffer ? FALSE : TRUE);
      if (b->isPressed != b->buffer)
         b->Setup();
   }
   else if (event->GetEventID() == MOU_BTN1DOWN)
   {
      XUserButton *b = (XUserButton *) GetWindow();

      b->buffer = b->isPressed;
      b->isPressed = (b->buffer ? TRUE : FALSE);
      if (b->isPressed != b->buffer)
         b->Setup();
   }
   return FALSE;
}
