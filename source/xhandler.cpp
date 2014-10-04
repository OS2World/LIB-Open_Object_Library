#include "XDragHdl.h"
#include "XDragEvn.h"
#include "XMousHdl.h"
#include "XMousevn.h"
#include "XWindow.h"
#include "XContHdl.h"
#include "XItmDHdl.h"
#include "XBackHdl.h"
#include "XStylHdl.h"
#include "XTimer.h"

#include <stdlib.h>


/*@
@class XHandler
@type overview
@symbol _
@remarks Events like mouse-moving, keyboard input) are typicaly send in the OOL in the form
of XEvent and derived classes. Except XControlEvent (see XWindow::DoControl()) this events
must be caught with classes derived from XHandler, each of this handlers is corresponding
to a special event-class, eg. to catch a XKeyboardEvent you need a XKeyboardHandler. <P>
To catch these event you must attach the needed handler to the window from which the
events should be caught (see the constructors of the handlers for details). Therefor you have
to derive a class from the needed handler and override the member-function HandleEvent(). With
your overridden function you catch the events and handle it.<P>
Usualy the method HandleEvent() must return a boolean value. The return value indicates if the
event can be processed by the operating system (return TRUE) or if the event should not be
processed (return FALSE).
You can install:
<UL>
<LI>XBackgroundDrawHandler
<LI>XContainerHandler
<LI>XDragHandler
<LI>XKeyboardHandler
<LI>XItemDrawHandler
<LI>XMouseHandler
<LI>XNoteBookHandler
<LI>XStyleHandler
<LI>XTimer
</UL>
*/


/*@ XHandler :: HandleEvent(XEvent*)
@remarks Override this method to catch event.
@parameters XEvent * event   the event-class, in derived classes of XHandler there will be usualy caught
events derived from XEvent.
@returns BOOL TRUE=your handler handled the event, FALSE=your handler didn't handle the event.
*/
XHandler :: XHandler(const USHORT ide, const XWindow * handleWindow)
{
   id = ide;
   handleFor = (XWindow *) handleWindow;

   handleFor->handlers += 1;
   handleFor->regHandlers = (XHandler **) realloc(handleFor->regHandlers, handleFor->handlers * sizeof(XHandler *));
   handleFor->regHandlers[handleFor->handlers - 1] = this;
}


XHandler :: ~XHandler()
{
   if (handleFor)
   {
      BOOL swap = FALSE;

      for (int i = 0; i < handleFor->handlers - 1; i++)
      {
         if (handleFor->regHandlers[i] == this)
            swap = TRUE;
         if (swap)
            handleFor->regHandlers[i] = handleFor->regHandlers[i + 1];
      }
      handleFor->handlers -= 1;
      handleFor->regHandlers = (XHandler **) realloc(handleFor->regHandlers, handleFor->handlers * sizeof(XHandler *));
   }
}


/*@
@class XItemDrawHandler
@parent XHandler
@type overview
@symbol _
@remarks XItemDrawHandler is used for controls which can draw the items itself, eg. menus or listboxes
with the style OWNERDRAW. To do so derive a class from XItemDrawHandler and override the method HandleEvent().
If you catch a XItemDrawEvent you can draw the item by calling XItemDrawEvent::DrawItem().
*/

/*@ XItemDrawHandler :: XItemDrawHandler(const XWindow * w, const SHORT itemWidth, const SHORT itemHeight)
@remarks   Constructor of XItemDrawHandler
@parameters
<t 'ø' c=2>
øXWindow * w            øthe window
øSHORT itemWidth      øthe width of the items
øSHORT itemHeight      øthe height of the items
</t>
*/
XItemDrawHandler :: XItemDrawHandler(const XWindow * w, const SHORT itemWidth, const SHORT itemHeight):XHandler(OOL_ITMDRAWHANDLER, w)
{
   width = itemWidth;
   height = itemHeight;

   char str[5];

   WinQueryClassName(w->GetHandle(), 5, (PCH) str);
   str[0] = ' ';
   type = atol(str);

   if (type == 2 || type == 7)
   {
      WinSendMsg(w->GetHandle(), LM_SETITEMWIDTH, MPFROMSHORT(width), 0);
      WinSendMsg(w->GetHandle(), LM_SETITEMHEIGHT, MPFROMSHORT(height), 0);
   }
}


int tid = 0;

/*@
@class XTimer
@parent XHandler
@type overview
@symbol _
@remarks XTimer is a timer which is activated in time-intervals given in XTimer::Start().
Time-intervals shouldnt be smaller than 1/18 second. To handle time-events derive a class from XTimer
and override TimeEvent().
*/


/*@ XTimer::TimeEvent()
@group misc
@remarks The timer got a timeout. Override this function to handle timer-events.
*/


/*@ XTimer::XTimer(XWindow*)
@group constructors/destructors
@remarks Constructs a timer.
@parameters XWindow * owner   the owner window
*/
XTimer :: XTimer(const XWindow * w):XHandler(OOL_TIMER, w)
{
   timerID = tid;
   tid++;
}


/*@ XTimer::Start()
@group misc
@remarks Start the timer.
@parameters ULONG time  time-interval in milli-seconds.
*/
BOOL XTimer::Start(const ULONG time)
{
   timeOut = time;
   HWND hwnd = GetWindow()->GetHandle();

   if (WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd, timerID, time) == 0)
      return FALSE;
   else
      return TRUE;
}


/*@ XTimer::Stop(void)
@group misc
@remarks Stop the timer.
*/
BOOL XTimer::Stop(void) const
{
   HWND hwnd = GetWindow()->GetHandle();

   return WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, timerID);
}


/*@ XTimer::QueryTimeOut()
@group misc
@remarks Query the time-interval
@returns ULONG time-interval in milliseconds
*/


/*@
@class XContainerHandler
@parent XHandler
@type overview
@symbol _
@remarks To catch events from a container you need to derive a class from XContainerHandler
and override the method HandleEvent where you will get the events.<P>
In the case of drag-events XContainerDragEvent is posted, in the case of edit-events XContainerEditEvent is posted, in the method HandleEvent
you can typecast in this case, see XContainerEvent for details.
*/


/*@ XContainerHandler::XContainerHandler(XContainerControl*)
@parameters XContainerControl * window       The window to handle the events for.
*/


/*@
@class XDragHandler
@parent XHandler
@type overview
@symbol _
@remarks To catch drag-events you need to derive a class from XDragHandler
and override the method HandleEvent() where you will get the events (XDragEvent).
*/


/*@ XDragHandler::XDragHandler(XWindow*)
@parameters XWindow * window       The window to handle the events for.
*/


/*@
@class XMouseHandler
@parent XHandler
@type overview
@symbol _
@remarks To catch events from the mouse like moving using mouse-buttons etc you must
register a XMouseHandler. Therefor you derive a class from XMouseHandler
and override the method HandleEvent() where you will get events (XMouseEvent) posted
from the mouse.
*/


/*@
@class XKeyboardHandler
@parent XHandler
@type overview
@symbol _
@remarks To catch events from the keyboard  you must
register a XKeyboardHandler. Therefor you derive a class from XKeyboardHandler
and override the method HandleEvent() where you will get events (XKeyboardEvent) posted
from the mouse.
*/


/*@ XKeyboardHandler::XKeyboardHandler(XWindow*)
@parameters XWindow * window       The window to handle the keyboard-events for.
*/


/*@ XMouseHandler::XMouseHandler(XWindow*)
@parameters XWindow * window       The window to handle the mouseevents for.
*/


/*@
@class XStyleHandler
@parent XHandler
@type overview
@symbol _
@remarks XStyleHandler catch events when the style of a window has changed (e.g. if the user
droped a font or color on a window). If you want to catch these events derive a class of
XStyleHandler and override HandleEvent(). In HandleEvent you get a XEvent where you can
get the ID of the event. Valid values are:
<t 'ø' 2>
øSTY_FONT                      øfont changed
øSTY_FOREGROUNDCOLOR           øforeground-color changed
øSTY_BACKGROUNDCOLOR           øbackground-color changed
</t>
*/

/*@ XStyleHandler::XStyleHandler(XWindow*)
@parameters XWindow * window       The window to handle the events for.
*/


/*@
@class XNoteBookHandler
@parent XHandler
@type overview
@symbol _
@remarks To catch events from a notebook you need to derive a class from XNoteBookHandler
and override the method HandleEvent where you will get the events.
@new
*/


/*@ XNoteBookHandler::XNoteBookHandler(XNoteBook*)
@parameters XNoteBook * window       The window to handle the events for.
*/


/*@
@class XBackgroundDrawHandler
@parent XHandler
@type overview
@symbol _
@remarks Use this class to catch a XBackgroundDrawEvent. Can be used in XContainerControl.
*/
