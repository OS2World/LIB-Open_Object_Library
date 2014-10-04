#ifndef __OOL_XDEFHANDLER_H__
#define __OOL_XDEFHANDLER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDefaultHandler                       */
/*                       derived from: XHandler                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xhandler.h"



class _export_ XDefaultHandler: public XHandler
{
   public:
      XDefaultHandler( const XWindow * w): XHandler( OOL_DEFAULTHANDLER, w ) { ;}
      virtual BOOL HandleEvent( ULONG, void *, void *, LONG * retVal) { return FALSE; }
};


#endif
