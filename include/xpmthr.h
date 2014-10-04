#ifndef __OOL_XPMTHREAD_H__
#define __OOL_XPMTHREAD_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XPMThread                             */
/*                       derived from: XProcess                              */
/*                        last update: 9/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xthread.h"


class _export_ XPMThread: public XThread
{
   private:
      HMQ queue;
      HAB hab;
      QMSG messagequeue;
      virtual void ThreadEntry( void );
   public:
      XPMThread( const LONG stack = 128000 ):XThread(stack) {;}
      ~XPMThread();
};


#endif

