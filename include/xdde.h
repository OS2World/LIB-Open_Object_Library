#ifndef __OOL_XDDE_H__
#define __OOL_XDDE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDDE                                  */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xobject.h"

class XWindow;
class ddehandler;

#define DDEF_TEXT  DDEFMT_TEXT

class _export_ XDDE: public XObject
{
      friend ddehandler;
   private:
      HWND server;
      SHORT ack;
      BOOL connected;
      PDDESTRUCT MakeDDEMsg( const USHORT usFormat, const char * pszItemName, const void * Data, const USHORT usDataSize);
      ddehandler * handler;
      XWindow * window;
      HWND * clientList;
      LONG clients;
      BOOL isServer;
   public:
      XDDE( const XWindow * handleFor, const BOOL isServer = FALSE );
      ~XDDE();
      virtual void Acknowledgement(LONG status) { ;}
      BOOL Advise( const char * itemName = "", const SHORT format = DDEF_TEXT );
      virtual void AdviseRequested( char *, SHORT, OOL_WINDOWHANDLE ) {;}
      virtual void Connected( OOL_WINDOWHANDLE ) { ;}
      virtual void DataReceived( ULONG , void * , char * , SHORT ) { ;}
      virtual void DataRequested( char * , SHORT , OOL_WINDOWHANDLE ) { ;}
      virtual void DisConnected() { ;}
      virtual void ConnectionRequested( char * app, char * topic, OOL_WINDOWHANDLE handle) { ;}
      virtual void DoCommand( char*) {;}
      void AcceptConnection( char * app, char * topic, OOL_WINDOWHANDLE hwnd );
      BOOL Init( char * app, char * topic);
      virtual BOOL InitPossible(char * , char * ) { return TRUE; }
      BOOL IsConnected(void) const { return connected; }
      BOOL PokeData( const OOL_WINDOWHANDLE client, const char * itemName, const void * data, const ULONG size=0, const SHORT format = DDEF_TEXT);
      SHORT GetAck(void) const { return ack; }
      BOOL GetData( const char * itemName, const SHORT format=DDEF_TEXT);
      XWindow * GetOwner(void) const { return window; }
      OOL_WINDOWHANDLE GetServerHandle(void) const { return server; }
      void SetAcknoledgment( const LONG a) {ack = a; }
      BOOL SendCommand( const char * itemName, const char * theCommand);
      BOOL SendData( const OOL_WINDOWHANDLE client, const char * itemName, const void * data, const ULONG size=0, const SHORT format = DDEF_TEXT);
      void Terminate(void);
      BOOL UnAdvise( const char * itemName = "");
      virtual void UnAdviseRequested( OOL_WINDOWHANDLE ) {;}
};

#endif
