#ifndef __OOL_XPIPE_H__
#define __OOL_XPIPE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XPipe                                 */
/*                       derived from: XIO                                   */
/*                        last update: 9/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xio.h"

#ifdef OOL_FOR_OS2_X86
   #define XPIPE_DEFAULTSIZE            4096
   #define XPIPE_STDIN           0x00000000
   #define XPIPE_STDOUT          0x00000001
   #define XPIPE_STDERROR        0x00000002
#endif


class _export_ XPipe: public XIO
{
   private:
public:
      HFILE handle2;
   public:
      XPipe() { handle2 = -1;}
      ~XPipe() { if(handle2 != -1) Close(); }
      void Close();
      static void CloseHandle( HFILE h ) { DosClose( h );}
      static ULONG DuplicateHandle( const ULONG h1, ULONG& h2);
      LONG Open( const LONG size = XPIPE_DEFAULTSIZE );
      LONG GetReadHandle(void) const { return handle; }
      LONG GetWriteHandle(void) const { return handle2; }
      ULONG _Write( const void * buffer, const ULONG sizeofbuffer) const
         {
            ULONG r;
            DosWrite(handle2, (PVOID) buffer, sizeofbuffer, &r);
            return r;
         }
};

#endif
