#include "xuserwnd.h"
#include "xrect.h"
#include "xexcept.h"
#include "xcntrevn.h"
#include "stdlib.h"
#include "xreslib.h"
#include "xbubble.h"
#include "xtimer.h"
#include "xbitmap.h"
#include "xgraphdv.h"
#include "xres.h"
#include "xprocess.h"
#include "xmoushdl.h"
#include "xfont.h"
#include "xmousevn.h"
#include "XText.h"
#include "xtraffic.h"
#include "xled.h"
#include "xpeak.h"
#include "xdate.h"
#include "xcal.h"
#include "xmoushdl.h"
#include "xmousevn.h"
#include "oolres.rh"

MRESULT EXPENTRY userProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   XWindow *win = (XUserWindow *) WinQueryWindowPtr(hwnd, 0);

   if (msg == WM_CREATE)
   {
      WinSetWindowPtr(hwnd, 0, (void *) mp1);
      mp1 = 0;
      return WinDefWindowProc(hwnd, msg, mp1, mp2);
   }

   if (win)
   {
      BOOL handled = FALSE;
      MRESULT mr = HandleDefault(win, msg, mp1, mp2, handled);

      if (handled)
         return mr;
   }

   return WinDefWindowProc(hwnd, msg, mp1, mp2);
}


/*@
@class XUserWindow
@parent XWindow
@type overview
@symbol _
*/

XUserWindow :: XUserWindow(const XWindow * w)
{
   if (w)
      XUserWindow(w->GetClientHandle());
   else
      XUserWindow(HWND_DESKTOP);
}


XUserWindow :: XUserWindow(const LONG handle)
{
   if (WinRegisterClass(WinQueryAnchorBlock(handle), (PSZ) "OOL_USERCLASS", (PFNWP) userProc, 0, 4) == FALSE)
      OOLThrow("error creating userwindow - couldnït register class", -10);

   winhandle = WinCreateWindow(handle, (PSZ) "OOL_USERCLASS", (PSZ) "", WS_VISIBLE | FS_SCREENALIGN,
                        0, 0, 0, 0, handle, HWND_TOP, 0, this, 0);
}


XUserWindow :: XUserWindow( void )
{
   if (WinRegisterClass(XApplication::GetApplication()->hab, (PSZ) "OOL_USERCLASS", (PFNWP) userProc, 0, 4) == FALSE)
      OOLThrow("error creating userwindow - couldnït register class", -10);

   winhandle = WinCreateWindow(HWND_DESKTOP, (PSZ) "OOL_USERCLASS", (PSZ) "", WS_VISIBLE | FS_SCREENALIGN,
                       0, 0, 0, 0, HWND_DESKTOP, HWND_TOP, 0, this, 0);
   if (winhandle == 0)
      OOLThrow("error creating userwindow", -10);
}


XUserWindow :: ~XUserWindow()
{
   HWND hwnd;

   HENUM enumWindow = WinBeginEnumWindows(winhandle);

   while ((hwnd = WinGetNextWindow(enumWindow)) != NULLHANDLE)
   {
      XWindow *w = (XWindow *) WinQueryWindowPtr(hwnd, 0);

      if (w && w != this)
         delete w;
   };
   WinEndEnumWindows(enumWindow);
   WinDestroyWindow(winhandle);
}


class bubbletimer:public XTimer
{
      XBubbleHelp *bubble;
   public:
      ULONG id;
      bubbletimer(XBubbleHelp * w):XTimer(w) { bubble = w; }
      void TimerEvent(void);
};


void bubbletimer::TimerEvent(void)
{
   POINTL p;
   POINTL buffer;

   if (!bubble->isActive)
      return;
   WinQueryPointerPos(HWND_DESKTOP, &p);
   buffer = p;
   SWP swp;

   WinMapWindowPoints(HWND_DESKTOP, bubble->aktWin->GetHandle(), &buffer, 1);
   WinQueryWindowPos(bubble->aktWin->GetHandle(), &swp);

   if (!(buffer.x > 0 && buffer.x < swp.cx && buffer.y > 0 && buffer.y < swp.cy))
      return;

   XRect rect;

   if (bubble->SetMsgText(id) == FALSE)
      return;
   rect.SetX(p.x);
   rect.SetY(p.y + 2);
   rect.SetWidth(174);
   rect.SetHeight(71);
   bubble->SetSize(&rect);
   bubble->Show();
   Stop();
}


class bhandler:public XMouseHandler
{
      XBubbleHelp *bubble;
   public:
      bhandler(XBubbleHelp * b, XWindow * w):XMouseHandler(w) { bubble = b; }
      BOOL HandleEvent(XMouseEvent *);
};


BOOL bhandler::HandleEvent(XMouseEvent *)
{
   if (bubble->IsVisible())
      bubble->Show(FALSE);
   bubble->timer->Stop();
   return FALSE;
}


class bubblehandler:public XMouseHandler
{
      XBubbleHelp *bubble;
   public:
      bubblehandler(XBubbleHelp * b, XWindow * w):XMouseHandler(w) {   bubble = b;   }
      BOOL HandleEvent(XMouseEvent *);
};


BOOL bubblehandler::HandleEvent(XMouseEvent * e)
{
   switch (e->GetEventID())
   {
      case MOU_MOVE:
      {
         POINTL buffer;

         HWND i = 0;
         SWP swp;

         WinQueryPointerPos(HWND_DESKTOP, &buffer);
         WinMapWindowPoints(HWND_DESKTOP, GetWindow()->GetHandle(), &buffer, 1);
         WinQueryWindowPos(GetWindow()->GetHandle(), &swp);

         if (buffer.x > 0 && buffer.x < swp.cx && buffer.y > 0 && buffer.y < swp.cy)
            i = GetWindow()->GetHandle();
         if (i)
         {
            if (!bubble->IsVisible() && bubble->isActive)
            {
               bubble->timer->id = WinQueryWindowUShort(GetWindow()->GetHandle(), QWS_ID);
               bubble->aktWin = GetWindow();
               bubble->timer->Start(1000);
            }
         }
         else
         {
            bubble->Show(FALSE);
            bubble->timer->Stop();
         }
         break;
      }
      default:
         if (bubble->IsVisible())
            bubble->Show(FALSE);
         bubble->timer->Stop();
         break;
   }
   return FALSE;
}


/*@ XBubbleHelp::SetMsgText()
@group text functions
@remarks Override this function to select the text to display with SetText()
@parameters ULONG id    the window-id from the window to display a helptext for
@returns    BOOL        TRUE=show the bubble, FALSE=dont display the bubble
*/


/*@ XBubbleHelp::SetText()
@group text functions
@remarks Set the text to display
@parameters char * theText
*/


/*@ XBubbleHelp::Enable()
@group misc
@remarks Enable/disable XBubbleHelp
@parameters BOOL enable    TRUE=enable, FALSE=disable (default is TRUE)
*/


/*@
@class XBubbleHelp
@parent XUserWindow
@type overview
@symbol _
@remarks XBubbleHelp is a window which can display short help-messages, it looks like
the bubbles on a mac.<P>
To use XBubbleHelp derive a class of it and override XBubbleHelp::SetMsgText().
See XBubbleHelp::AddWindow() and XBubbleHelp::XBubbleHelp() for details how to
select windows to show help-messages for.<P>
For applications which use XBubbleHelp it is nessecary that the OOL-resourcelibrary
OOLRES.DLL is in the libpath.<P>
The destructor is not called automaticaly when the window is hidden.<P>
XBubbleHelp does not work with menus!
*/


/*@ XBubbleHelp: : XBubbleHelp(XWindow * w)
@group constructors/destructors
@remarks Construct a bubblehelp-window
@parameters XWindow *   The owner-window.
*/
XBubbleHelp :: XBubbleHelp( XWindow * w):XUserWindow()
{
   dev = NULL;
   map1 = map2 = map3 = NULL;
   dev = new XGraphicDevice(this, FALSE, FALSE);

   dev->SetWidth(174);
   dev->SetHeight(71);

   XPoint p;

   map3 = new XBitmap();
   map3->Copy(dev, &p);

   map1 = new XBitmap(&p, ROP_SRCAND);
   map2 = new XBitmap(&p, ROP_SRCINVERT);

   XResourceLibrary lib("oolres");
   XResource res( IDR_BUBBLE1, &lib);

   map1->Load( &res );

   res.SetID(IDR_BUBBLE2);
   map2->Load( &res );

   XRect rec(12, 3, 152, 61);
   XFont *font = new XFont(dev, "Helv", 8);

   text = new XText(font, &rec, "", DT_LEFT | DT_WORDBREAK | DT_TOP);

   timer = new bubbletimer(this);
   text->Show();

   HWND hwnd;
   XWindow *win;
   HENUM henum = WinBeginEnumWindows(w->GetHandle());

   while ((hwnd = WinGetNextWindow(henum)) != 0)
   {
      win = (XWindow *) WinQueryWindowPtr(hwnd, 0);
      if (win)
         bubblehandler *h = new bubblehandler(this, win);
   }
   WinEndEnumWindows(henum);

   bhandler *b1 = new bhandler(this, w);
   bhandler *b2 = new bhandler(this, this);

   isActive = TRUE;
}


XBubbleHelp :: ~XBubbleHelp()
{
   delete map1;
   delete map2;
   delete map3;
   delete text;
   delete dev;
}


void XBubbleHelp::Draw(void)
{
   if (IsVisible())
   {
      map1->Draw(dev);
      map2->Draw(dev);
      map3->Draw(dev);
      text->Draw(dev);
   } /* end if */
}


void XBubbleHelp::Show(const BOOL show)
{
   if (!dev)
      return;
   if (show)
   {
      XPoint p;
      XWindow :: Show(TRUE);
      if (map3)
         map3->Copy(dev, &p);
   }
   else
      XWindow :: Show(FALSE);
}


/*@
@class XTrafficLight
@parent XUserWindow
@type overview
@symbol _
@remarks XTrafficLight is a class which creates a window that looks like a traffic light<P>
For applications which use XTrafficLight it is nessecary that the OOL-resourcelibrary
OOLRES.DLL is in the libpath.
*/
XTrafficLight :: XTrafficLight( const XWindow * w, const XPoint& p, const USHORT id):XUserWindow(w->GetHandle())
{
   dev = new XGraphicDevice(this);
   WinSetWindowUShort(winhandle, QWS_ID, id);
   state = 0;

   XRect rect( p.GetX(), p.GetY(), 32, 32);
   SetSize(&rect);

   dev->SetWidth(32);
   dev->SetHeight(32);

   XPoint p0;

   map = new XBitmap( &p0);

   SetState(0);
   Draw();
}


/*@ XTrafficLight::SetState( const SHORT st )
@remarks Set the current state
@parameters SHORT state, possible values are:
<OL>
<LI>TRAFFIC_OFF
<LI>TRAFFIC_GREEN
<LI>TRAFFIC_YELLOW
<LI>TRAFFIC_RED
<LI>TRAFFIC_YELLOWRED
</OL>
*/
void XTrafficLight::SetState( const SHORT st )
{
   XResourceLibrary lib("oolres");
   XResource res( IDR_TRAFFIC + st, &lib);

   map->Load( &res );

   state = st;
   Draw();
}


void XTrafficLight::Draw()
{
   map->Draw(dev);
}


XTrafficLight :: ~XTrafficLight()
{
   delete map;
   delete dev;
}

/*@
@class XLED
@parent XUserWindow
@type overview
@symbol _
@remarks XLED is a class which creates a window that looks like a LED<P>
For applications which use XLED it is nessecary that the OOL-resourcelibrary
OOLRES.DLL is in the libpath.
*/
XLED :: XLED( const XWindow * w, const XPoint& p, const USHORT id):XUserWindow(w->GetHandle())
{
   WinSetWindowUShort(winhandle, QWS_ID, id);
   dev = new XGraphicDevice(this);
   state = FALSE;

   XRect rect( p.GetX(), p.GetY(), 16, 16);
   SetSize(&rect);

   dev->SetWidth(16);
   dev->SetHeight(16);

   XPoint p0;

   map = new XBitmap( &p0);
   Enable(FALSE);
   Draw();
}


void XLED :: Draw( void )
{
   map->Draw(dev);
}

XLED :: ~XLED()
{
   delete map;
   delete dev;
}


/*@ XLED::Enable( const BOOL st )
@remarks Enable/Disable the LED
@parameters BOOL TRUE=enable, FALSE=disable, default is TRUE
*/
void XLED::Enable( const BOOL st )
{
   XResourceLibrary lib("oolres");
   XResource res(0, &lib);

   if(st == FALSE)
   {
      res.SetID(IDR_LED_OFF);
      map->Load(&res);
   }
   else
   {
      res.SetID(IDR_LED_ON);
      map->Load(&res);
   }
   state = st;
   Draw();
}


XPeakMeter :: XPeakMeter( const XWindow * w, const XPoint& p, const USHORT id, const BOOL v):XUserWindow(w->GetHandle())
{
   WinSetWindowUShort(winhandle, QWS_ID, id);
   dev = new XGraphicDevice(this);
   vert = v;

   if(vert)
   {
      XRect rect( p.GetX(), p.GetY(), 215, 15);
      SetSize(&rect);

      dev->SetWidth(215);
      dev->SetHeight(15);
   }
   else
   {
      XRect rect( p.GetX(), p.GetY(), 20, 145);
      SetSize(&rect);

      dev->SetWidth(20);
      dev->SetHeight(145);
   }

   XColor col(COL_BLACK);
   dev->SetBackgroundColor( &col );

   XPoint point(10, 5);

   XResourceLibrary lib("oolres");
   XResource res(IDR_PEAK_GREEN, &lib);

   mapGreen = new XBitmap( &point);
   mapGreen->Load(&res);
   mapGreen->Show();

   point.SetX( 200 );
   res.SetID(IDR_PEAK_RED);
   mapRed = new XBitmap( &point);
   mapRed->Load(&res);
   mapRed->Show();
   SetLevel(0);
}


void XPeakMeter :: Draw( void )
{
   dev->FillBackground();
   XPoint point(10, 5);
   if(vert)
   {
      for(int i = 0; i < 14; i++)
      {
         if(i < level)
         {
            point.SetX( 5 + i * 15);
            if(i < 10 )
            {
               mapGreen->Move( &point );
               mapGreen->Draw(dev);
            }
            else
            {
               mapRed->Move( &point );
               mapRed->Draw(dev);
            }
         } /* end if */
      }
   }
   else
   {
      point.SetX(5);

      for(int i = 0; i < 14; i++)
      {
         if(i < level)
         {
            point.SetY( 5 + i * 10);
            if(i < 10 )
            {
               mapGreen->Move( &point );
               mapGreen->Draw(dev);
            }
            else
            {
               mapRed->Move( &point );
               mapRed->Draw(dev);
            }
         } /* end if */
      }
   }
}


void XPeakMeter :: SetLevel( USHORT l)
{
   level = l;
   Draw();
}


XPeakMeter::~XPeakMeter()
{
   delete mapGreen;
   delete mapRed;
   delete dev;
}


class CalendarHandler: public XMouseHandler
{
   public:
      CalendarHandler(XWindow* w): XMouseHandler(w) {;}
      BOOL HandleEvent( XMouseEvent *);
};


BOOL CalendarHandler :: HandleEvent( XMouseEvent * e)
{
   if( e->GetEventID() == MOU_BTN1CLICK )
   {
      XCalendarControl * win = (XCalendarControl*) GetWindow();
      if( e->p.GetX() > 10 )
      {
         if(e->p.GetX() < 23 )
         {
              SHORT sel = (85 - e->p.GetY()) / 12 + 1 - win->date.GetWeekDay();
            if(sel > 0)
               win->DaySelected( sel );
         }
         else
         {
            SHORT x = (((e->p.GetX() - 23 ) / 18) + 2) * 7;
            SHORT y = (85 - e->p.GetY()) / 12 + 1;
            SHORT days = x + y - 7 - win->date.GetWeekDay();
            if(days <= win->date.GetDaysOfMonth())
               win->DaySelected( days );
         }
      }
      return TRUE;
   }
   return FALSE;
}


XCalendarControl :: ~XCalendarControl()
{
   for(int i = 0; i < count; i++)
      delete dayList[i];
   free(dayList);
   delete dev;
}


XCalendarControl :: XCalendarControl( const XWindow * w, const XPoint& p, const USHORT id, const XDate& startDate):XUserWindow(w->GetHandle())
{
   WinSetWindowUShort(winhandle, QWS_ID, id);
   dev = new XGraphicDevice(this);

   XRect rect( p.GetX(), p.GetY(), 120, 85);
   SetSize(&rect);

   dev->SetWidth(120);
   dev->SetHeight(85);

   XColor col(COL_PALEGRAY);
   dev->SetBackgroundColor( &col );

   XRect p0( 5, 0, 18, 12);

   p0.SetY( 73 - startDate.GetWeekDay() * 12);
   date = startDate;
   date.SetDays( 1 );

   XFont * font = new XFont( dev, "Helv", 7);
   XColor colRed( COL_RED );

   dayList = (XText**) malloc( 31 * sizeof(void*));
   count = startDate.GetDaysOfMonth() +1;
   for(int i = 0; i < count; i++)
   {
      XString string = (LONG) i+1;
      if(i > 0)
      {
         if( date.GetWeekDay() == 0 )
         {
            p0.SetY( 73 );
            p0.SetX( p0.GetX() + 18);
         }
         else
            p0.SetY( p0.GetY() - 12);
      }

      dayList[i] = new XText( font, &p0, (char*) string, DT_RIGHT);
      if(date.GetWeekDay() == 6 || date.IsCeremon())
         dayList[i]->SetColor( &colRed );

      date.AddDays(1);
   }

   date = startDate;
   CalendarHandler * h = new CalendarHandler(this);

   Draw();
}


void XCalendarControl :: Draw( void )
{
   dev->FillBackground();
   for(int i = 0; i < count; i++)
      dayList[i]->Draw(dev);
}


XText * XCalendarControl :: GetDayObject(const UCHAR day) const
{
   if( day > 0 && day <= date.GetDaysOfMonth())
      return dayList[day-1];
   else
      return NULL;
}
