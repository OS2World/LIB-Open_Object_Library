#ifndef __OOL_XTEXT_H__
#define __OOL_XTEXT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XText                                 */
/*                       derived from: XGraphicObject                        */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xgraphob.h"
#include "stdlib.h"
#include "xstring.h"
class XRect;
class XFont;

class _export_ XText:public XGraphicObject
{
   private:
      XFont * font;
      XString text;
      SHORT style;
      POINTL sh;
      GRADIENTL an;
      XPoint p2;
   public:
      XText( const XFont * font, const XRect * r, const char * text, const SHORT style = DT_LEFT);
      XText( const XFont * font, const XPoint * p, const char * text);
      void Draw( XGraphicDevice * dev);
      LONG GetStyle( void ) const { return style; }
      void GetText( XString * t) { *t=text; }
      void GetTextBox(XGraphicDevice * dev, XRect * r);
      void SetAngle( const SHORT dX, const SHORT dY ) { an.x = dX; an.y = dY; }
      void SetFont( XFont * f) { font = f;}
      void SetShear( const SHORT dX, const SHORT dY ) { sh.x = dX; sh.y = dY; }
      void SetSize( const XRect * rect) { p.SetX(rect->GetX()); p.SetY(rect->GetY()); p2.SetX( rect->GetWidth() + rect->GetX()); p2.SetY( rect->GetY() + rect->GetHeight());}
      void SetStyle( const LONG s) { style = s; }
      void SetText( const char * t) { text = t; }
      void SetupDevice( XGraphicDevice * dev );
};

#endif
