#ifndef __OOL_XMARKER_H__
#define __OOL_XMARKER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMarker                               */
/*                       derived from: XGraphicObject                        */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xgraphob.h"
#include "xpoint.h"
#include "xsize.h"

class _export_ XMarker: public XGraphicObject
{
   private:
      char type;
      //SHORT cx, cy;
      XSize size;
   public:
      XMarker( const XPoint * point, const SHORT t = MARKSYM_SOLIDSQUARE);
      virtual ~XMarker() {;}
      void Draw( XGraphicDevice * dev );
      void GetSize( XSize * s) { *s = size; }
      void SetSize( const XSize * s) { size = *s; }
      void SetupDevice( XGraphicDevice * dev);
};


#endif
