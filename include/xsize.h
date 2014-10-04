#ifndef __OOL_XSIZE_H__
#define __OOL_XSIZE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XSize                                 */
/*                       derived from: XObject                               */
/*                        last update: 3/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xobject.h"
#include "string.h"

class _export_ XSize: public XObject
{
   private:
      LONG width, height;
   public:
      XSize( const LONG x=0, const LONG y=0) {width = x; height = y; }
      LONG GetWidth() const { return width; }
      LONG GetHeight() const { return height; }
      void SetWidth( const LONG x) { width = x; }
      void SetHeight( const LONG y ) { height = y; }
      void Set(const LONG x, const LONG y) { height = y; width = x; }
      XSize operator =(const XSize& s)
         {
            width=s.width;
            width=s.height;
            return *this;
         }
};

#endif
