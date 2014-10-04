#include "XToolBar.h"
#include "XFrmWnd.h"
#include "XRect.h"
#include "XFrame.h"
#include "XColor.h"
#include "XGraphDv.H"
#include "XLine.h"
#include "XCntrEvn.h"
#include "XMousHdl.h"
#include "XMousevn.h"
#include "XReslib.h"
#include "xmsgbox.h"
#include "stdlib.h"


/*@
@class XToolBar
@parent XWindow
@type overview
@symbol _
@remarks XToolBar represents a toolbar for framewindows. You can attach every type of control to it.
*/


class ToolBarParent:public XFrameWindow
{
   friend XToolBar;
      XToolBar *toolBar;
      BOOL init;
   public:
      ToolBarParent(const XResource * r, const XRect& rec, XToolBar * t);
      ~ToolBarParent() { WinSetOwner(frame, HWND_DESKTOP); }
      BOOL QueryForClose()
      {
         Show(FALSE);
         return FALSE;
      }
      BOOL DoSysCommand(USHORT com)
         {
            if(com == SC_CLOSE)
               return (toolBar->style & TB_NOCLOSE ? FALSE : TRUE);
            else
               return TRUE;
         }
      void DoMove(void);
};


void ToolBarParent::DoMove(void)
{
   if (init)
      return;
   else
      init = TRUE;
   XRect rect1, rect3;

   toolBar->frame->GetSize(&rect1);
   GetSize(&rect3);
   if (rect3.GetX() > rect1.GetX() + 50 && rect3.GetX() < rect1.GetX() + rect1.GetWidth() - 50 && (rect3.GetY() + rect3.GetHeight()) < rect1.GetY() + rect1.GetHeight() && (rect3.GetY() + rect3.GetHeight()) > rect1.GetY() + rect1.GetHeight() - 50)
   {
      WinSetOwner(frame, NULLHANDLE);
      toolBar->style = TB_TOP;
      toolBar->Attach();
      clientWindow = NULL;
      delete this;
   }
   else if (rect3.GetX() > rect1.GetX() + 50 && rect3.GetX() < rect1.GetX() + rect1.GetWidth() - 50 && rect3.GetY() + rect3.GetHeight() > rect1.GetY() && rect3.GetY() + rect3.GetHeight() < rect1.GetY() + 50)
   {
      WinSetOwner(frame, NULLHANDLE);
      toolBar->style = TB_BOTTOM;
      toolBar->Attach();
      clientWindow = NULL;
      delete this;
   }
   else if (rect3.GetX() > rect1.GetX() && rect3.GetX() < rect1.GetX() + 50 && rect3.GetY() > rect1.GetY() && rect3.GetY() < rect1.GetY() + rect1.GetHeight())
   {
      WinSetOwner(frame, NULLHANDLE);
      toolBar->style = TB_LEFT;
      toolBar->Attach();
      clientWindow = NULL;
      delete this;
   }
   else if (rect3.GetX() < rect1.GetX() + rect1.GetWidth() && rect3.GetX() > rect1.GetX() + rect1.GetWidth() - 50 && rect3.GetY() > rect1.GetY() && rect3.GetY() < rect1.GetY() + rect1.GetHeight())
   {
      WinSetOwner(frame, NULLHANDLE);
      toolBar->style = TB_RIGHT;
      toolBar->Attach();
      clientWindow = NULL;
      delete this;
   }
   init = FALSE;
}


ToolBarParent :: ToolBarParent(const XResource * r, const XRect& rec, XToolBar * t):XFrameWindow(r, "", FRM_TITLEBAR | FRM_BORDER | FRM_SYSMENU | WIN_VISIBLE, rec)
{
   t->cutWindow = this;

   init = TRUE;
   toolBar = t;
   WinSetParent(t->GetHandle(), frameWin->GetHandle(), TRUE);
   WinSetOwner(frame, t->frame->frame);
   SetClient(t);
/******************************
 ULONG Frame_style = WinQueryWindowULong( frame, QWL_STYLE );
 Frame_style = ( Frame_style & ~FS_BORDER ) | FS_SIZEBORDER;
 WinSetWindowULong( frame, QWL_STYLE, Frame_style );
 WinSendMsg( frame, WM_UPDATEFRAME, MPFROMP( FCF_BORDER ), 0 );
********************************/
   Activate();
   SetText(t->title);
   init = FALSE;
}


class ToolBarHandler:public XMouseHandler
{
      BOOL btn;
      XPoint p;
   public:
      ToolBarHandler(const XWindow * w):XMouseHandler(w)   { btn = FALSE;   }
      BOOL HandleEvent(XMouseEvent *);
};


BOOL ToolBarHandler::HandleEvent(XMouseEvent * e)
{
   switch (e->GetEventID())
   {
      case MOU_BTN1DOWN:
         btn = TRUE;
         p = e->p;
         break;
      case MOU_MOVE:
         if (btn == TRUE)
         {
            if (e->p.GetX() > p.GetX() + 5 || e->p.GetX() < p.GetX() - 5)
            {
               XToolBar *t = (XToolBar *) GetWindow();
               if (t->GetCutWindow() == NULL)
                  t->Cut();
               btn = FALSE;
            }
         }
         break;
      default:
         btn = FALSE;
         break;
   }
   return FALSE;
}



void XToolBar::Attach(void)
{
   frame->frameWin->AddFrameControl(this);

   WinSetParent(winhandle, frame->frame, TRUE);
   WinSetOwner(winhandle, frame->frame);

   LONG x = borderSize, y = borderSize;
   XRect rect;
   int width = 0, height = 0;

   if (style & TB_RIGHT || style & TB_LEFT)
   {
      for (int i = 0; i < clients; i++)
      {
         clientArray[i]->clientWin->GetSize(&rect);
         if (clientArray[i]->group)
            x = borderSize;
         if (x + clientArray[i]->xOffset + rect.GetWidth() > width)
            width = x + clientArray[i]->xOffset + rect.GetWidth() + borderSize;
         x += clientArray[i]->xOffset + rect.GetWidth();
      }
      cx = width;
   }
   else
   {
      int yy = 0;

      for (int i = 0; i < clients; i++)
      {
         char className[10];

         WinQueryClassName(clientArray[i]->clientWin->GetHandle(), 10, (PCH) className);
         className[0] = ' ';
         if (atol(className) == 2)
         {
            if (yy < borderSize * 2 + 25)
               yy = borderSize * 2 + 25;
         }
         else
         {
            clientArray[i]->clientWin->GetSize(&rect);
            if (yy < rect.GetHeight() + borderSize * 2 + clientArray[i]->yOffset)
               yy = rect.GetHeight() + borderSize * 2 + clientArray[i]->yOffset;
         }
      }
      cy = yy;
   }

   cutWindow = NULL;

   XRect rect2;

   frame->GetSize(&rect2);
   rect2.SetHeight(rect2.GetHeight() - 1);
   frame->SetSize(&rect2);
}


void XToolBar::Cut(void)
{
   XRect rect2;

   frame->frameWin->RemoveFrameControl(this);

   frame->GetSize(&rect2);
   rect2.SetHeight(rect2.GetHeight() + 1);
   frame->SetSize(&rect2);

   XResource res(0, XApplication::GetApplication()->GetResourceLibrary());

   XPoint p;

   GetPointerPos(&p);
   GetSize(&rect2);
   rect2.SetX(p.GetX() - 30);

   ULONG width = 0, hight = 0, lowest = 0;
   XRect rect;

   for (int i = 0; i < clients; i++)
   {
      clientArray[i]->clientWin->GetSize(&rect);
      if (rect.GetX() + rect.GetWidth() > width)
         width = rect.GetX() + rect.GetWidth();
      if (lowest == 0)
         lowest = rect.GetY();
      if (rect.GetY() < lowest)
         lowest = rect.GetY();
   }

   rect2.SetWidth(width + borderSize);

   rect2.SetHeight(rect2.GetHeight() + borderSize - lowest);

   rect2.SetY(p.GetY() - rect2.GetHeight());

   LONG y = WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);

   y += WinQuerySysValue(HWND_DESKTOP, SV_CYBORDER);
   rect2.SetHeight(rect2.GetHeight() + y);
   rect2.SetY(rect2.GetY() - y);

   cutWindow = new ToolBarParent(&res, rect2, this);
}


/*@ XToolBar::Draw(void)
@group display
@remarks Redraw the toolbar.
*/
void XToolBar::Draw(void)
{
   RECTL rect;
   WinQueryWindowRect(winhandle, &rect);
   HPS hps = WinGetPS(winhandle);
   GpiCreateLogColorTable(hps, LCOL_RESET, LCOLF_RGB, 0,0,NULL);
   WinFillRect( hps, &rect, SYSCLR_BUTTONMIDDLE);
   WinDrawBorder(hps, &rect, 1,1,0,0,DB_PATCOPY|DB_RAISED);
   HENUM e;
   HWND hwnd;

   e = WinBeginEnumWindows(winhandle);
   while ((hwnd = WinGetNextWindow(e)) != 0)
      WinInvalidateRegion(hwnd, NULLHANDLE, TRUE);
   WinEndEnumWindows(e);
}


BOOL XToolBar::DoCommand(LONG com)
{
   return frame->DoCommand(com);
}

/*@ XToolBar::DoControl(XControlEvent * e)
@remarks If a control event is send from a client of the toolbar the
event will be send to the owner frame window.
*/
void XToolBar::DoControl(XControlEvent * e)
{
   frame->DoControl(e);
}


void XToolBar::DoSize(XSize * s)
{
   cx = s->GetWidth();
   ReSize();
}


/*@ XToolBar::GetBackgroundColor(XColor * col)
@group display
@remarks Returns the background color
@parameters XColor * colorBuffer         buffer which will get the color
*/
void XToolBar::GetBackgroundColor(XColor * col) const
{
   XColor color(COL_PALEGRAY);
   *col = color;
}



/*@ XToolBar::RemoveWindow(const XWindow * w, const BOOL redraw)
@group adding/removing windows
@remarks Removes a window attached to the toolbar
@parameters
<t 'ø' c=2>
øXWindow * øwindow to remove
øBOOL ørecalculate the position of other attached windows
</t>
@returns    BOOL                         success
*/
BOOL XToolBar::RemoveWindow(const XWindow * w, const BOOL redraw)
{
   SHORT i, j;
   XRect or;

   for (i = 0; i < clients; i++)
   {
      if (clientArray[i]->clientWin == w)
      {
         EnableWindowUpdate(FALSE);
         clientArray[i]->clientWin->GetSize(&or);
         clientArray[i]->clientWin->Show(FALSE);
         free(clientArray[i]);
         for (j = i; j < clients - 1; j++)
         {
            clientArray[j] = clientArray[j + 1];
            if (redraw)
            {
               XRect r;

               clientArray[j]->clientWin->GetSize(&r);
               r.SetX(r.GetX() - or.GetWidth());
               clientArray[j]->clientWin->SetSize(&r);
            }
         }
         clients -= 1;
         clientArray = (clientWin **) realloc(clientArray, clients * sizeof(clientWin *));
         EnableWindowUpdate();
         return TRUE;
      }
   }
   return FALSE;
}


/*@ XToolBar::AddWindow(const XWindow * w, const BOOL newGroup, const BOOL adjustSize, const XWindow * insertBehind, const LONG x, LONG y)
@group adding/removing windows
@remarks Attach a window to the toolbar.
@parameters  <t 'ø' c=2>
            øXWindow * theWindow          øwindow to add
            øBOOL newGroup                øwith this window a new group starts
            øBOOL adjustSize              ørecalculate the hight of the toolbar depending on the dimensions of theWindow
            øXWindow * insertBehind       øinsert theWindow behind this window
            øLONG xOffset                 øx-offset of the window
            øLONG yOffset                 øy-offset of the window (usefull for XComboBox)
            </t>
@returns     BOOL                         success
*/
void XToolBar::AddWindow(const XWindow * w, const BOOL newGroup, const BOOL adjustSize, const XWindow * insertBehind, const LONG x, LONG y)
{
   XRect r;
   LONG xo = borderSize;
   LONG y0 = 0, xMax = 0;
   SHORT ins = clients;

   EnableWindowUpdate(FALSE);

   clients += 1;
   clientArray = (clientWin **) realloc(clientArray, clients * sizeof(clientWin *));

   if (clients > 1)
   {
      if (insertBehind)
      {
         SHORT i, j;

         for (i = 0; i < clients - 1; i++)
         {
            if (clientArray[i]->clientWin == insertBehind)
            {
               clientArray[i]->clientWin->GetSize(&r);
               SHORT xDiff = r.GetX() + r.GetWidth() + x;

               if (newGroup)
               {
                  if (style & TB_LEFT || style & TB_RIGHT)
                     goto newG;
                  xDiff += spacing;
               }
               ins = i + 1;
               xo = xDiff;
               XRect r2;

               w->GetSize(&r2);
               if (style & TB_LEFT || style & TB_RIGHT)
                  y += clientArray[i]->yOffset;
               xDiff = r2.GetWidth();

               for (j = clients - 1; j > i + 1; j--)
               {
                  clientArray[j] = clientArray[j - 1];
                  clientArray[j]->clientWin->GetSize(&r2);
                  r2.SetX(r2.GetX() + xDiff);
                  if (r2.GetX() + r2.GetWidth() > xMax)
                     xMax = r2.GetX() + r2.GetWidth();
                  r2.SetY(y);
                  clientArray[j]->clientWin->SetSize(&r2);
               }

               goto mark1;
            }
         }
      }
      else
      {
   newG:
         if (style & TB_LEFT || style & TB_RIGHT)
         {
            y0 = 0;
            for (SHORT i = 0; i < clients - 1; i++)
            {
               clientArray[i]->clientWin->GetSize(&r);
               if (r.GetY() < y0)
                  y0 = r.GetY();
               xo = r.GetX() + r.GetWidth();
            }
            y0 += borderSize;
            if (newGroup)
            {
               xo = borderSize;
               y0 += r.GetHeight() + spacing;
            }
            else if (clients > 1)
               y0 += r.GetHeight();
         }
         else
         {
            if (clients > 1)
               clientArray[clients - 2]->clientWin->GetSize(&r);
            xo = r.GetX() + r.GetWidth();
         }
         if (newGroup)
         {
            if (style & TB_TOP || style & TB_BOTTOM)
               xo += spacing;
            else
               y0 -= spacing;
         }
         y += y0;
      }
   }
mark1:

   clientArray[ins] = (clientWin *) calloc(sizeof(clientWin), 1);

   clientArray[ins]->clientWin = (XWindow *) w;
   clientArray[ins]->yOffset = y;
   clientArray[ins]->xOffset = x;
   clientArray[ins]->group = newGroup;

   if (adjustSize)
   {
      XRect rec;

      w->GetSize(&rec);
      if (style & TB_TOP || style & TB_BOTTOM)
      {
         if (rec.GetHeight() > cy - 1 * borderSize)
         {
            cy = rec.GetHeight() + 2 * borderSize;
            SetHeight(cy);
            ReSize();
         }
      }
      else
      {
         if (rec.GetWidth() + x + xo > cx - borderSize || xMax > cy)
         {
            if (xMax)
               cx = xMax + borderSize;
            else
               cx = rec.GetWidth() + x + xo + borderSize;
            SetHeight(cy);
            ReSize();
         }
      }
   }

   w->GetSize(&r);

   r.SetX(xo + x);
   r.SetY(cy - borderSize + y - r.GetHeight());
   w->SetSize(&r);

   EnableWindowUpdate();
}


/*@ XToolBar::ReSize(void)
@group sizing
@remarks The positions of the attached windows are recalculated.
*/
void XToolBar::ReSize(void)
{
   int i;
   XRect rect2, rect;

   GetSize(&rect2);

   int x = borderSize;

   if (style & TB_RIGHT || style & TB_LEFT)
   {
      int y = rect2.GetHeight() - borderSize;

      for (i = 0; i < clients; i++)
      {
         clientArray[i]->clientWin->GetSize(&rect);
         if (clientArray[i]->group == TRUE)
         {
            x = borderSize;
            char className[10];

            WinQueryClassName(clientArray[i]->clientWin->GetHandle(), 10, (PCH) className);
            className[0] = ' ';
            if (atol(className) == 2)
            {
               if (i > 0)
                  y -= (25 + clientArray[i]->yOffset + spacing);
               else
                  y -= (25 + clientArray[i]->yOffset);
               rect.SetY(y + 25 + clientArray[i]->yOffset - rect.GetHeight());
            }
            else
            {
               if (i > 0)
                  y -= (rect.GetHeight() + clientArray[i]->yOffset + spacing);
               else
                  y -= (rect.GetHeight() + clientArray[i]->yOffset - borderSize);
               rect.SetY(y + clientArray[i]->yOffset);
            }
         }
         else
         {
            if (i == 0)
               y -= (rect.GetHeight() + clientArray[i]->yOffset);
            else
               rect.SetY(y + clientArray[i]->yOffset);
         }
         rect.SetX(x + clientArray[i]->xOffset);
         x += rect.GetWidth() + clientArray[i]->xOffset;
         clientArray[i]->clientWin->SetSize(&rect);
      }
   }
   else
   {
      for (i = 0; i < clients; i++)
      {
         clientArray[i]->clientWin->GetSize(&rect);
         rect.SetY(rect2.GetHeight() - rect.GetHeight() + clientArray[i]->yOffset - borderSize);

         if (clientArray[i]->group == TRUE && i > 0)
            x += spacing;

         rect.SetX(x + clientArray[i]->xOffset);
         x += rect.GetWidth() + clientArray[i]->xOffset;
         clientArray[i]->clientWin->SetSize(&rect);
      }
   }
}


/*@ XToolBar :: XToolBar(const XFrameWindow * fr, const ULONG sty, const ULONG id, const USHORT ySize, const USHORT groupSpace)
@group constructors/destructors
@remarks Creates a toolbar
@parameters <t 'ø' c=2>
            øXFrameWindow * parent        øframewindow wich gets the toolbar
            øULONG style                  øthe style of the toolbar, possible values are:
<BR>
TB_TOP      toolbar is on the top of the framwindow
<BR>
TB_BOTTOM   toolbar is on the bottom of the framwindow
<BR>
TB_LEFT     toolbar is on the left side of the framwindow
<BR>
TB_RIGHT    toolbar is on the right side of the framwindow
<BR>
TB_RIPABLE  the user can rip the toolbar
<BR>
                                         default is TB_TOP
            øULONG id                     øwindow-ID, default is 0
            øUSHORT ySize                 øhight of the toolbar in pixels, default is 20
            øUSHORT groupSpace            øsize between window-groups in pixels, default is 15
            </t>
*/
XToolBar :: XToolBar(const XFrameWindow * fr, const ULONG sty, const ULONG id, const USHORT ySize, const char groupSpace):XFrameControl(fr, sty, id)
{
   style = sty;
   cx = 30;
   if (style & TB_RIPABLE)
      hdl = new ToolBarHandler(this);
   else
      hdl = NULL;

   cutWindow = NULL;
   spacing = groupSpace;
   frame = (XFrameWindow *) fr;
   borderSize = 5;
   clients = 0;
   clientArray = NULL;
   cy = ySize;

   SetHeight(cy);
}


/*@ XToolBar::SetBorderSize(const USHORT border)
@group sizing
@remarks Set the size between the border of the toolbar and the attached windows
@parameters USHORT borderSize            size of border in pixels
*/
void XToolBar::SetBorderSize(const USHORT border)
{
   borderSize = border;
}


/*@ XToolBar::Show(const BOOL show)
@group display
@remarks Show/hide the toolbar
@parameters BOOL show            TRUE=show, FALSE=hide
*/
void XToolBar::Show(const BOOL show)
{
   if (cutWindow)
   {
      cutWindow->Show(show);
      return;
   }
   if (show)
   {
      WinShowWindow(winhandle, TRUE);
      SetHeight(cy);
   }
   else
   {
      WinShowWindow(winhandle, FALSE);
      SetHeight(0);
   }
}


/*@ XToolBar::SetHeight(const USHORT y)
@group sizing
@remarks Set the hight of the toolbar
@parameters USHORT ySize                 the new hight in pixels
*/
BOOL XToolBar::SetHeight(const USHORT y)
{
   SWP swp[200];

   if (IsVisible())
      cy = y;
/*
   if (line1)
   {
      line1->SetHeight(y - 2);
      XPoint p(0, y - 2);

      line2->Move(&p);
      p.Set(cx - 1, 0);
      line3->Move(&p);
      line3->SetHeight(y - 2);
   }
*/
   if (frame && frame->frameWin)
   {
      SWP swp2;

      WinQueryWindowPos(frame->winhandle, &swp2);
      WinSetWindowPos(frame->winhandle, HWND_TOP, 0, 0, swp2.cx - cx, swp2.cy - y, SWP_SIZE | SWP_SHOW | SWP_ZORDER);

      SHORT res = SHORT1FROMMR(WinSendMsg(frame->frameWin->winhandle, WM_QUERYFRAMECTLCOUNT, 0, 0));

      WinSendMsg(frame->frameWin->winhandle, WM_FORMATFRAME, MPFROMP(swp), 0);

      SHORT i;

      for (i = 0; i < res; i++)
      {
         if (swp[i].hwnd != 0)
            WinSetWindowPos(swp[i].hwnd, swp[i].hwndInsertBehind, swp[i].x, swp[i].y, swp[i].cx, swp[i].cy, swp[i].fl);
      }

      if (frame->clientWindow == NULL)
         WinInvalidateRegion(frame->winhandle, NULLHANDLE, TRUE);

      return TRUE;
   }
   else
      return FALSE;
}


/*@ XToolBar :: ~XToolBar()
@group constructors/destructors
@remarks Destructor, every attached window is destroyed and the destructors of the
windows are called.
*/
XToolBar :: ~XToolBar()
{
   if( style & TB_NOCLOSE )
      style ^= TB_NOCLOSE;

   if(cutWindow)
      cutWindow->Close();
   else
      WinDestroyWindow(winhandle);
   for (int i = 0; i < clients; i++)
   {
      delete clientArray[i]->clientWin;
      free( clientArray[i] );
   } /* end for */
   free(clientArray);
//   delete graph;
}
