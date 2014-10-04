#ifndef __OOL_XITMDRAWHANDLER_H__
#define __OOL_XITMDRAWHANDLER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XItemDrawHandler                      */
/*                       derived from: XHandler                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xhandler.h"


class XItemDrawEvent;

class _export_ XItemDrawHandler: public XHandler
{
      friend MRESULT HandleFrameDefault( XFrameWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   private:
      SHORT height, width, type;
   public:
      XItemDrawHandler( const XWindow * w, const SHORT itemWidth, const SHORT itemHeight);
      virtual BOOL HandleEvent( XItemDrawEvent *) {return FALSE; }
};


#endif
