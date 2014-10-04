#ifndef __OOL_XFRAMECNT_H__
#define __OOL_XFRAMECNT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XFrameControl                         */
/*                       derived from: XUserWindow                           */
/*                        last update: 9/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xuserwnd.h"

#define TB_TOP       1
#define TB_BOTTOM    2
#define TB_LEFT      4
#define TB_RIGHT     8
#define TB_NOBORDER   32

class XFrameWindow;

class _export_ XFrameControl: public XUserWindow{
      friend class XFrame;
   protected:
      XFrameControl( const XFrameWindow * w, const LONG style, const ULONG id=0);
      virtual ~XFrameControl() {;}
      SHORT style;
      USHORT cy, cx;
   public:
      virtual void AdjustSize( PSWP pswpToolbar, PSWP pswpClient);
};

#endif
