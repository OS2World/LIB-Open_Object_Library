#ifndef __OOL_XPROCES_H__
#define __OOL_XPROCES_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XProcess                              */
/*                       derived from: XObject                               */
/*                        last update: 4/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xobject.h"

class XWindow;
class XString;

#ifdef OOL_FOR_OS2_X86
   #define EXE_SYNC       EXEC_SYNC
   #define EXE_ASYNC      EXEC_ASYNC

   #define XPRTY_PROCESS       PRTYS_PROCESS
   #define XPRTY_PROCESSTREE   PRTYS_PROCESSTREE
   #define XPRTY_THREAD      PRTYS_THREAD

   #define XPRTY_NOCHANGE       PRTYC_NOCHANGE
   #define XPRTY_IDLETIME       PRTYC_IDLETIME
   #define XPRTY_REGULAR        PRTYC_REGULAR
   #define XPRTY_TIMECRITICAL    PRTYC_TIMECRITICAL
   #define XPRTY_FOREGROUNDSERVER   PRTYC_FOREGROUNDSERVER
#endif

class _export_ XProcess: public XObject
{
   protected:
public:
      TID   tid;
   public:
      XProcess();
      virtual ~XProcess() {;}
      static void Beep( const LONG f, const LONG d);
      static void EnterCriticSection();
      static LONG ExecuteProg( const char * path, const char * args=NULL, const char * env=NULL, const ULONG flag=EXEC_ASYNC, LONG * result=NULL);
      static void LeaveCriticSection();
//////      virtual BOOL QueryForQuit( void );
      static BOOL ScanEnvironment( const char * valueToSearch, XString * buffer);
      static void Sleep( const LONG d);
      void SetPriority( LONG delta, ULONG scope=XPRTY_THREAD, ULONG clss=XPRTY_NOCHANGE )
      { DosSetPriority( scope, clss, delta, tid ); }
};

#endif



