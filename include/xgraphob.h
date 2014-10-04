#ifndef __OOL_XGRAPHICOBJ_H__
#define __OOL_XGRAPHICOBJ_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XGraphicObject                        */
/*                       derived from: XObject                               */
/*                        last update: 7/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xobject.h"
#include "xpoint.h"
#include "XColor.h"
#include "XGraphDv.h"

class XRect;

#define GO_HIDE 1


class _export_ XGraphicObject: public XObject
{
      friend XGraphicDevice;
      friend class XLayer;
      friend class XPrinterDevice;
   protected:
      XColor color;
      XPoint p;
      char settings;
      virtual BOOL HitTest( const LONG , const LONG ) { return FALSE;}
      XGraphicObject() { ;}
      SHORT pattern;
   public:
      XGraphicObject( const XPoint * p = NULL);
      XGraphicObject( const LONG x, const LONG y);
      virtual ~XGraphicObject() { ;}
      virtual void Draw( XGraphicDevice * dev ) { ;}
      void GetColor( XColor * c) { *c = color; }
      void GetPos( XPoint * point) { *point = p; }
      void Move( const XPoint * point) { p = *point; }
      void SetColor( const XColor * c) { color = *c;}
      void SetPattern( SHORT p) { pattern = p; }
      virtual void SetupDevice ( XGraphicDevice * dev ) { LONG c = color.GetColor();   if( dev->aktColor != c)   dev->SetColor(c); if(dev->aktPattern != pattern) dev->SetPattern(pattern);}
      void Show( const BOOL show = TRUE );
};

#endif
