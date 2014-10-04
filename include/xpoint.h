#ifndef __OOL_XPOINT_H__
#define __OOL_XPOINT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XPoint                                */
/*                       derived from: XObject                               */
/*                        last update: 3/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xobject.h"


class _export_ XPoint: public XObject
{
      friend class XBox;
      friend class XText;
      friend class XCircle;
      friend class XArc;
      friend class XLine;
      friend class XMarker;
   protected:
      LONG x, y;
   public:
      XPoint( const LONG xPoint = 0, const LONG yPoint = 0)
            { x = xPoint; y = yPoint;}
      XPoint( const XPoint * pp) { x=pp->x; y=pp->y; }
      virtual ~XPoint() {;}
      LONG GetX( void ) const { return x; }
      LONG GetY( void ) const { return y; }
      void Set( const LONG x_1, const LONG y_1) { x=x_1; y=y_1; }
      void SetX( const LONG newX ) {x = newX; }
      void SetY( const LONG newY ) {y = newY; }
      XPoint operator =(const XPoint& p) { x=p.x; y=p.y; return *this; }
};

#endif
