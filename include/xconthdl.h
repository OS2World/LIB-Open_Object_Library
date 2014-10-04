#ifndef __OOL_XCONTAINERHANDLER_H__
#define __OOL_XCONTAINERHANDLER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XContainerHandler                     */
/*                       derived from: XHandler                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xhandler.h"


class XContainerEvent;
class XContainerControl;

class _export_ XContainerHandler: public XHandler
{
   public:
      XContainerHandler( const XContainerControl * w): XHandler( OOL_CONTAINERHANDLER, (XWindow*) w ) { ;}
      virtual BOOL HandleEvent( XContainerEvent *) { return FALSE;}
};


#endif
