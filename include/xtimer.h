#ifndef __OOL_XTIMER_H__
#define __OOL_XTIMER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XTimer                                */
/*                       derived from: XHandler                              */
/*                        last update: 9/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xhandler.h"

// TBO
class _export_ XTimer: public XHandler
{
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   private:
      int timerID;
      ULONG timeOut;
   public:
      XTimer( const XWindow *);
//      ~XTimer() { Stop(); }
      ~XTimer() { }
      ULONG GetTimeOut(void) const { return timeOut; }
      BOOL Start( const ULONG time );
      BOOL Stop( void ) const;
      virtual void TimerEvent(void) { ;}
};

#endif
