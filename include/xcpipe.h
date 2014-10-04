#ifndef __OOL_XCPIPE_H__
#define __OOL_XCPIPE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XNamedPipeClient                      */
/*                       derived from: XIO                                   */
/*                        last update: 9/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xio.h"
#include "xfile.h"

class _export_ XNamedPipeClient: public XIO
{
   public:
      XNamedPipeClient() { ;}
      ~XNamedPipeClient() { if(handle) Close(); }
      LONG GetState( void );
      LONG Open(char * name, ULONG modeopen = OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYNONE, char * server = NULL);
      LONG WaitForServer( char * name, ULONG timeOut, char * server = NULL );
};

#endif
