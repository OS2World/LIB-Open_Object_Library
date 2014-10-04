#include "xthread.h"
#include "xpmthr.h"
#include "xmsgbox.h"
#include "stdio.h"
#include "xexcept.h"

#include <stdlib.h>
#include <process.h>


#ifndef __WATCOMC__
int XThread::threadsRunning = 0;
#else
static int threadsRunning = 0;
#endif


ULONG XThread::Resume()
{
   return DosResumeThread(tid);
}


ULONG XThread::Suspend()
{
   return DosSuspendThread(tid);
}

/*@
@class XThread
@parent XProcess
@type overview
@symbol _
*/

/*@ XThread::Terminate(void)
@group initiate/terminate a process
@remarks With this method a thread can be terminated. If you
call Terminate() the method QueryForQuit() will not be called.
@fixed
*/
void XThread::Terminate(void)
{
   threadsRunning -= 1;
   delete this;
   _endthread();
}


void XThread::ThreadEntry(void)
{
   Init();
}


/*@ XThread::XThread(LONG)
@group constructors/destructors
@remarks Contructs a thread. After a thread is constructed, call Run() to make
the thread work. You must override method XThread::Init() where you have full controll
of the thread. To stop the thread call Terminate().<P>
<B>If your thread should have access to windows, use XPMThread instead.</B>
@parameters: LONG stackSize    size of the stack, default is 128000 KB
*/
#ifndef __WATCOMC__
void _Optlink StartUp(void * v) //void _Optlink StartUp(void *v)
#else
void StartUp(void *v)
#endif
{
   XThread *t = (XThread *) v;

   #ifndef __WATCOMC__
      XThread :: threadsRunning++;
   #else
      threadsRunning++;
   #endif
   XThread::RunThreads(t);
}


void XThread::Run()
{
#ifdef __BCPLUSPLUS__
   tid = _beginthread(StartUp(this), stackSize, this);
#else
   tid = _beginthread(StartUp, 0, stackSize, (void *) this);
#endif
}


void XThread::RunThreads(XThread * t)
{
   #ifndef __WATCOMC__
   if(XThread::threadsRunning != 1)
   #else
   if(threadsRunning != 1)
   #endif
   {
      t->ThreadEntry();
      return;
   } /* end if */

   BOOL init = FALSE;

   while (1)
   {
      TID id = 0;
      if(init==FALSE )
      {
         init = TRUE;
         t->ThreadEntry();
      } /* end if */
      DosWaitThread(&id, DCWW_WAIT);
      #ifndef __WATCOMC__
      if (XThread::threadsRunning <= 0)
      #else
      if( threadsRunning <= 0 )
      #endif
         break;
      DosSleep(0);
   }
}

/*@
@class XPMThread
@parent XThread
@type overview
@symbol _
*/


/*@ XPMThread::XPMThread(LONG)
@group constructors/destructors
@remarks Contructs a thread which have access to windows. After a thread is constructed, call Run() to make
the thread work. You must override method XThread::Init() where you can construct
windows and so on. Init() is the funcion where you enter the thread and have full controll of it.
To stop the thread call Terminate().
@parameters: LONG stackSize    size of the stack, default is 128000 KB
*/

void XPMThread :: ThreadEntry( void )
{
//to prevent timing problems
XProcess::Sleep(200);
   hab = WinInitialize(0);
   if(hab == NULL)
   {
      throw(XException("couldn't initialize PM-thread", - 100));
   } /* end if */
   queue = WinCreateMsgQueue(hab, 0);

   Init();

   while (1)
   {
      WinGetMsg(hab, &messagequeue, 0, 0, 0);
      if (messagequeue.msg == WM_QUIT)
         break;
      WinDispatchMsg(hab, &messagequeue);
   };
}


XPMThread :: ~XPMThread()
{
//to prevent timing problems
XProcess::Sleep(200);
   WinDestroyMsgQueue(queue);
   WinTerminate(hab);
}

