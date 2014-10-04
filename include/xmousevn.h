#ifndef __OOL_XMOUSEEVENT_H__
#define __OOL_XMOUSEEVENT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMouseEvent                           */
/*                       derived from: XEvent                                */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xevent.h"
#include "xpoint.h"


#ifdef OOL_FOR_OS2_X86
   #define MOU_BTN1CLICK                 1
   #define MOU_BTN1DBLCLICK              2
   #define MOU_BTN1DOWN                  3
   #define MOU_BTN1UP                    4
   #define MOU_BTN2CLICK                 5
   #define MOU_BTN2DBLCLICK              6
   #define MOU_BTN2DOWN                  7
   #define MOU_BTN2UP                    8
   #define MOU_BTN3CLICK                 9
   #define MOU_BTN3DBLCLICK              10
   #define MOU_BTN3DOWN                  11
   #define MOU_BTN3UP                    12
   #define MOU_INITDRAG                  WM_INITDRAG
   #define MOU_MOVE                      WM_MOUSEMOVE
   #define MOU_ENTER                     1054
   #define MOU_EXIT                      1055
#endif


class _export_ XMouseEvent: public XEvent
{
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   private:
      XMouseEvent( const LONG, const void*, const void*);
      SHORT keyboardState;
   public:
      XPoint p;
      SHORT GetKeyInfo(void ) const { return keyboardState; }
};


#endif
