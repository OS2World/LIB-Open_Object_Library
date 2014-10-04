#ifndef __OOL_XBACKDRAWEVENT_H__
#define __OOL_XBACKDRAWEVENT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XBackgroundDrawEvent                  */
/*                       derived from: XEvent                                */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xcntevnt.h"

class XBitmap;

class _export_ XBackgroundDrawEvent: public XEvent
{
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   private:
      POWNERBACKGROUND back;
      XBackgroundDrawEvent( const LONG l, const void * p): XEvent( l) { back = (POWNERBACKGROUND) p;}
   public:
      void Draw( const XBitmap *);
};


#endif
