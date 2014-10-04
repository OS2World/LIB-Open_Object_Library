#ifndef __OOL_XCIRCLE_H__
#define __OOL_XCIRCLE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XCircle                               */
/*                       derived from: XLine                                 */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xline.h"


class _export_ XCircle: public XLine
{
      BOOL filled;
      USHORT w;
      USHORT h;
      SHORT x;
      SHORT y;
   public:
      XCircle( const XPoint * center, const ULONG wight, const BOOL fill=FALSE, const USHORT hight=0, const SHORT xOff = 0, const SHORT yOff = 0);
      virtual ~XCircle() {;}
      void Draw( XGraphicDevice * dev);
};


#endif
