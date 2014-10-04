#ifndef __OOL_XCONTROLEVENT_H__
#define __OOL_XCONTROLEVENT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XControlEvent                         */
/*                       derived from: XEvent                                */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xevent.h"

//standard control events
#define WIN_CHANGED   1
#define WIN_DBLCLICK  2
#define WIN_PAINT     3
#define WIN_ENTER     4
#define WIN_VSCROLL   5
#define WIN_HSCROLL   6
#define WIN_SETFOCUS  7
#define WIN_KILLFOCUS 8
#define WIN_SELECTED  9
#define WIN_SHOWLIST  10
#define WIN_TRACK     11
#define WIN_UPARROW   12
#define WIN_DOWNARROW 13
#define WIN_ENDTRACK  14

//media control events
#define MEDIA_PLAYED   300
#define MEDIA_PAUSED   301
#define MEDIA_REWINDED 302
#define MEDIA_STOPED   303



#define WIN_FONTCHANGED  15
#define WIN_BACKCOLCHANGED   16
#define WIN_FRONTCOLCHANGED  17


class XWindow;
class XFrameWindow;

class _export_ XControlEvent: public XEvent
{
      friend MRESULT HandleFrameDefault( XFrameWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
      friend MRESULT EXPENTRY userProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
      friend MRESULT EXPENTRY bookProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
   protected:
      XControlEvent( const LONG event): XEvent(event) {;}
      XControlEvent( XWindow *, const void*, const void*);
      LONG windowID;
      XWindow * window;
   public:
      XWindow * GetWindow(void) const { return window; }
      LONG GetWindowID( void ) const { return windowID; }
};

#endif
