#ifndef __OOL_XINDICATOR_H__
#define __OOL_XINDICATOR_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XIndicator                            */
/*                       derived from: XSlider                               */
/*                        last update: 1/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xslider.h"

class _export_ XIndicator: public XSlider{
   public:
      XIndicator( const XWindow * owner, const XRect& rec= XRect(), const USHORT id = 0, const BOOL showScale = TRUE, const BOOL showText = TRUE, const char*font=NULL);
      virtual ~XIndicator() {;}
};


#endif
