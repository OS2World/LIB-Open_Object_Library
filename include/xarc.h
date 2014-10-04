#ifndef __OOL_XARC_H__
#define __OOL_XARC_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XArc                                  */
/*                       derived from: XLine                                 */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xline.h"


class _export_ XArc: public XLine
{
      BOOL filled;
      BOOL vis;
      USHORT w;
      USHORT h;
      SHORT x;
      SHORT y;
      SHORT st;
      SHORT en;
   public:
      XArc( const XPoint * pp1, const SHORT w, const SHORT startAngle, const SHORT sweepAngle, const BOOL showLines=FALSE, const BOOL fill=FALSE, const SHORT wight=0, const SHORT xOff=0, const SHORT yOff=0);
      virtual ~XArc() {;}
      void Draw( XGraphicDevice * dev );
      USHORT GetStart( void ) const { return st; }
      USHORT GetWidth( void ) const { return en; }
      void SetStart( const USHORT degrees ) { st = degrees; }
      void SetWidth( const USHORT degrees ) { en = degrees; }
};


#endif
