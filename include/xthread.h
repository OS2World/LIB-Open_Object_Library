#ifndef __OOL_XTHREAD_H__
#define __OOL_XTHREAD_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XThread                               */
/*                       derived from: XProcess                              */
/*                        last update: 2/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xprocess.h"


class _export_ XThread: public XProcess
{
      #ifndef __WATCOMC__
      friend void _Optlink StartUp(void*);
      #else
      friend void StartUp( void* v );
      #endif
   private:
      LONG stackSize;
   protected:
      #ifndef __WATCOMC__
      static int threadsRunning;
      #endif
      virtual void ThreadEntry( void );
   public:
      void Run();
      virtual void Init( void ) { ;}
   private:
      static void RunThreads( XThread *);
   public:
      XThread( const LONG stack = 128000 ) { stackSize = stack; }
      ULONG Resume();
      ULONG Suspend();
      void  Kill()   { DosKillThread( tid ); }
      void Terminate( void );
};

#endif
