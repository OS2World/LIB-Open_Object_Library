#ifndef __OOL_XRECT_H__
#define __OOL_XRECT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XRect                                 */
/*                       derived from: XObject                               */
/*                        last update: 3/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xpoint.h"
#include "XSize.h"

class _export_ XRect: public XPoint
{
      friend class XControl;
      friend class XFrameWindow;
      friend class XWindow;
      friend class XText;
   private:
      LONG cx, cy;
   public:
      XRect( const long width, const long height)
            { x=y=0; cx = width; cy = height;}
      XRect( const long x1 = 0, const long y1 = 0, const long x_2 = 0, const long y_2 = 0):XPoint(x1,y1)
            { cx=x_2; cy=y_2;}
      virtual ~XRect() {;}
      long GetHeight( void ) const { return cy;}
      long GetWidth( void ) const { return cx;}
      void SetWidth( const LONG w ) { cx = w;}
      void SetHeight( const LONG h)  { cy = h;}
      XRect operator =(const XRect& r)
         {
            x=r.x;
            y=r.y;
            cx=r.cx;
            cy=r.cy;
            return *this;
         }
      XRect operator =(const XSize& s)
         {
            cx=s.GetWidth();
            cy=s.GetHeight();
            return *this;
         }
};



#endif
