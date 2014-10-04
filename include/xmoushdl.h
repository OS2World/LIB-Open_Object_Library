#ifndef __OOL_XMOUSEHANDLER_H__
#define __OOL_XMOUSEHANDLER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMouseHandler                         */
/*                       derived from: XHandler                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xhandler.h"


class XMouseEvent;

class _export_ XMouseHandler: public XHandler
{
   public:
      XMouseHandler( const XWindow * w): XHandler( OOL_MOUSEHANDLER, w ) { ;}
      virtual BOOL HandleEvent( XMouseEvent *) { return FALSE;}
};


#endif
