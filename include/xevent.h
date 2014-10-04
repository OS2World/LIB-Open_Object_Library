#ifndef __OOL_XEVENT_H__
#define __OOL_XEVENT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XEvent                                */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"

class XWindow;

class _export_ XEvent: public XObject
{
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   protected:
      ULONG eventID;
      XEvent( const ULONG event = 0) { eventID = event; }
   public:
      ULONG GetEventID(void) const { return eventID; }
};


#endif
