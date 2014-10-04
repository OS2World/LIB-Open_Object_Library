#ifndef __OOL_XVIDEO_H__
#define __OOL_XVIDEO_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XVideo                                */
/*                       derived from: XMediaWindow                          */
/*                        last update: 2/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xmedia.h"

class XFrameWindow;


class _export_ XVideo: public XMediaWindow{
   public:
      XVideo( const XFrameWindow *);
      LONG Restore( void ) const;
      LONG SetViewPort( const XFrameWindow * w) const;
};

#endif
