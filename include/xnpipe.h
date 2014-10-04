#ifndef __OOL_XNPIPE_H__
#define __OOL_XNPIPE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XNamedPipeServer                      */
/*                       derived from: XIO                                   */
/*                        last update: 9/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xio.h"

#ifdef OOL_FOR_OS2_X86
   #define XNPIPE_DEFAULTSIZE            4096
   #define XNPIPE_INDEFINITE_WAIT   (-1)
   #define XNPIPE_DEFAULT_WAIT      0
   #define XNPIPE_STATE_DISCONNECTED   1
   #define XNPIPE_STATE_LISTENING   2
   #define XNPIPE_STATE_CONNECTED   3
   #define XNPIPE_STATE_CLOSING   4
   #define XNPIPE_ACCESS_INBOUND   0x0000
   #define XNPIPE_ACCESS_OUTBOUND   0x0001
   #define XNPIPE_ACCESS_DUPLEX   0x0002
   #define XNPIPE_INHERIT      0x0000
   #define XNPIPE_NOINHERIT      0x0080
   #define XNPIPE_WRITEBEHIND      0x0000
   #define XNPIPE_NOWRITEBEHIND   0x4000

   #define XNPIPE_READMODE_BYTE   0x0000
   #define XNPIPE_READMODE_MESSAGE   0x0100
   #define XNPIPE_TYPE_BYTE      0x0000
   #define XNPIPE_TYPE_MESSAGE      0x0400
   #define XNPIPE_END_CLIENT      0x0000
   #define XNPIPE_END_SERVER      0x4000
   #define XNPIPE_WAIT         0x0000
   #define XNPIPE_NOWAIT      0x8000
   #define XNPIPE_UNLIMITED_INSTANCES   0x00ff

#endif


class _export_ XNamedPipeServer: public XIO
{
      BOOL isOpen;
   public:
      XNamedPipeServer() { isOpen = FALSE; }
      ~XNamedPipeServer() { if(isOpen) Close();  }
      LONG Connect(void);
      LONG DisConnect( void );
      LONG Open( char * name, ULONG openMode = XNPIPE_ACCESS_DUPLEX, ULONG pipeMode = XNPIPE_WAIT|XNPIPE_TYPE_BYTE|XNPIPE_READMODE_BYTE, char pipeCount = 1, LONG outSize = XNPIPE_DEFAULTSIZE, long inSize = XNPIPE_DEFAULTSIZE, ULONG timeOut = 500 );
      LONG GetState( void );
      LONG ResetBuffer( void ) { return DosResetBuffer(handle); }
};

#endif
