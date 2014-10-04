#ifndef __OOL_XKEYBHANDLER_H__
#define __OOL_XKEYBHANDLER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XKeyboardHandler                      */
/*                       derived from: XHandler                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xhandler.h"


class XKeyboardEvent;

class _export_ XKeyboardHandler: public XHandler
{
   public:
      XKeyboardHandler( const XWindow * w): XHandler( OOL_KEYBHANDLER, w ) { ;}
      virtual BOOL HandleEvent( XKeyboardEvent *) { return TRUE;} //return TRUE if the key-event can be processed
};


#endif
