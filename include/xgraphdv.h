#ifndef __OOL_XGRAPHDEV_H__
#define __OOL_XGRAPHDEV_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XGraphicDevice                        */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xcolor.h"

class XWindow;
class XGraphicObject;
class XColor;
class XPoint;
class XFont;
class XRect;

#ifdef OOL_FOR_OS2_X86
   #define XGRAPH_PELS         PU_PELS
   #define XGRAPH_LOMETRIC      PU_LOMETRIC
   #define XGRAPH_HIMETRIC      PU_HIMETRIC
   #define XGRAPH_LOENGLISH   PU_LOENGLISH
   #define XGRAPH_HIENGLISH   PU_HIENGLISH
   #define XGRAPH_TWIPS         PU_TWIPS
#endif

class _export_ XGraphicDevice: public XObject
{
      friend class XLine;
      friend class XCircle;
      friend XGraphicObject;
      friend class XArc;
      friend class XFont;
      friend class XText;
      friend class XBox;
      friend class XMarker;
      friend class XBitmap;
   private:
      HAB hab;
      HWND hwnd;
      LONG backCol;
      char fonts;
      XFont ** fontList;
      void RegisterFont( XFont* );
   protected:
      XGraphicDevice( const LONG res);
      void EnableFont( XFont*, LONG);
      HPS hps;
      HDC hdc;
      LONG mode;
      LONG aktColor, xOffset, yOffset;
      char aktLineEnding;
      char aktLineJoining;
      LONG aktLineWidth;
      SHORT aktLineType;
      SHORT aktFontSize;
      SHORT aktPattern;
      char aktFont;
      SHORT markerType;
      LONG width, height;
      BOOL fill;
   public:
      XGraphicDevice( const XWindow*, const BOOL createInMemory = FALSE, const BOOL fillBackground = TRUE, LONG res = PU_PELS, LONG mode = GPIT_MICRO | GPIA_ASSOC);
      virtual ~XGraphicDevice();
//      void AddObject ( const XGraphicObject*);
      void Copy( const XGraphicDevice *, const XRect *, const XRect * = NULL);
      void FillBackground( void );
//      void Empty(void);
//      XGraphicObject * FindObject( const LONG x, const LONG y, const XGraphicObject * searchBehind = NULL);
      void GetBackgroundColor( XColor * c) const { c->SetColor( backCol); }
      HDC GetHDC() const { return hdc; }
      HPS GetHPS() const { return hps; }
//      BOOL RemoveObject( XGraphicObject*, const BOOL destroyObject = TRUE);
      void ResetFont( void );
      void SetBackgroundColor( const XColor * c);
      virtual void SetColor( const LONG c) { aktColor = c; GpiSetColor( hps, c); }
      virtual void SetFont( XFont * );
      void SetHeight( const LONG h) { height = h; }
      void SetLineEnd( const char lineEnd) { GpiSetLineEnd( hps, lineEnd ); aktLineEnding = lineEnd; }
      void SetLineJoin( const char lineJoin ) { GpiSetLineJoin( hps, lineJoin); aktLineJoining = lineJoin; }
      void SetLineWidth( const LONG width);
      void SetLineType( const SHORT type ) { GpiSetLineType( hps, type); aktLineType = type; }
      void SetPattern( SHORT p) { aktPattern = p; GpiSetPattern(hps, p);}
      void SetWidth( const LONG h) { width = h; }
      void SetXOffset( const LONG x) { xOffset = x; }
      void SetYOffset( const LONG y) { yOffset = y; }
};

#endif
