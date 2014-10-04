#ifndef __OOL_XDRAGHANDLER_H__
#define __OOL_XDRAGHANDLER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDragHandler                          */
/*                       derived from: XHandler                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xhandler.h"


class XDragEvent;
class XDragTransfer;

class _export_ XDragHandler: public XHandler
{
   public:
      XDragHandler( const XWindow*w): XHandler( OOL_DRAGHANDLER, w ) { ;}
      virtual void HandleEvent( XDragEvent *) {;}
      virtual BOOL HandleRenderEvent( XDragTransfer*) { return 0;}
};


#endif
