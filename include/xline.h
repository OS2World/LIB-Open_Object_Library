#ifndef __OOL_XLINE_H__
#define __OOL_XLINE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XLine                                 */
/*                       derived from: XGraphicObject                        */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xgraphob.h"
#include "xpoint.h"

class XRect;

class _export_ XLine: public XGraphicObject
{
   protected:
      LONG width, height;
      char lineType;
      LONG lineWidth;
      char lineJoin;
      char lineEnd;
   public:
      XLine( const XPoint *, const XPoint * p = NULL);
      virtual ~XLine() {;}
      void Draw( XGraphicDevice * dev );
      LONG GetWidth( void ) { return width; }
      LONG GetHeight( void ) { return height; }
      void SetLineEnd( const char end ) { lineEnd = end; }
      void SetLineJoin( const char join ) { lineJoin = join; }
      void SetLineType( const char type ) { lineType = type; }
      void SetLineWidth( const LONG width ) { lineWidth = width; }
      void SetHeight( const LONG y) { height = y; }
      void SetWidth( const LONG x) { width = x; }
      void SetupDevice( XGraphicDevice * dev );
};


#endif
