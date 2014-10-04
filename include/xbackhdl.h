#ifndef __OOL_XBACKDRAWHANDLER_H__
#define __OOL_XBACKDRAWHANDLER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XBackgroundDrawHandler                */
/*                       derived from: XHandler                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xhandler.h"


#include "XBackevn.h"

class _export_  XBackgroundDrawHandler: public XHandler
{
   public:
      XBackgroundDrawHandler( const XWindow * w): XHandler( OOL_BACKDRAWHANDLER, w ) { ;}
      virtual BOOL HandleEvent( XBackgroundDrawEvent *) {return FALSE;}
};


#endif
