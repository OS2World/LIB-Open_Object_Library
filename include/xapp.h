#ifndef __OOL_XAPPLICATION_H__
#define __OOL_XAPPLICATION_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XApplication                          */
/*                       derived from: XProcess                              */
/*                        last update: 10/97                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xprocess.h"
#include "XString.h"

class XResourceLibrary;
class XExitHandler;

class _export_ XApplication: public XProcess
{
      friend class XFrameWindow;
      friend class XUserWindow;
      friend class XModelessDialog;
      friend class XModalDialog;
      friend class XDialog;
      friend class XDummyApplication;
   private:
      XResourceLibrary * resLib;
      int argCount;
      XString ** argValues;
      BOOL ex;
      HAB hab;
      HMQ queue;
      QMSG messagequeue;
//public:
      SHORT windows;
      XWindow ** frames;
      void AddWindow( const XWindow * );
      void RemoveWindow( const XWindow*);
      void Clean( void );
   protected:
      XApplication();
      ~XApplication();
   public:
      static XApplication * GetApplication();
      static void SetExitHandler( XExitHandler *);
      HAB GetAnchorBlock() { return hab; }
      USHORT GetArgumentCount() { return argCount; }
      BOOL GetArgumentValue( USHORT index, XString * buffer);
      XResourceLibrary * GetResourceLibrary(void) const { return resLib; }
      void Terminate( void );
      void SetArguments( int argc, void ** argv);
      void SetResourceLibrary( const XResourceLibrary *);
      void Start( void );
};

#endif
