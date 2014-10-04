#ifndef __OOL_XSCROLLWND_H__
#define __OOL_XSCROLLWND_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XScrollWindow                         */
/*                       derived from: XFrameWindow                          */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xfrmwnd.h"


class XScrollBar;
class XResource;
class XSize;

class _export_ XScrollWindow: public XFrameWindow
{
      friend class scrollhandler;
   private:
      void AddDummy(void);
      void DeleteDummy( void);
   protected:
      void CalcHorzSize( void );
      void CalcVertSize( void );
      LONG virtualX, virtualY, scrollPosX, scrollPosY;
      SHORT xStep, yStep, xPage, yPage;
//      SWP clientSwp;
      BOOL scrollEnabled;
   private:
      void Setup();
   public:
      XScrollWindow( const XResource * resource,
                 const char * title=NULL,
                 const ULONG createstyle = XFrameWindow::defaultStyle,
                 const XRect& rec = XRect(),
                 const XFrameWindow * parent = NULL,
                 const BOOL buildFromResource = FALSE,
                 const BOOL animate = FALSE);
      XScrollWindow( const ULONG id,
                 const char * title=NULL,
                 const ULONG createstyle = XFrameWindow::defaultStyle,
                 const XRect& rec = XRect(),
                 const XFrameWindow * parent = NULL,
                 const BOOL buildFromResource = FALSE,
                 const BOOL animate = FALSE);
      virtual ~XScrollWindow();
      void AddVertScroller( void );
      void AddHorzScroller( void );
      void DeleteVertScroller( void );
      void DeleteHorzScroller( void );
//      void DoSize( XSize*);
      void EnableAutoScroll( BOOL b) {scrollEnabled = b;}
      LONG GetHorzPos( void ) { return scrollPosX; }
      XScrollBar * GetHorzScroller(void) { return horz; }
      LONG GetVertPos( void ) { return scrollPosY; }
      XScrollBar * GetVertScroller(void) { return vert; }
      virtual void HScroll( LONG );
      void SetVirtualX( const LONG x);
      void SetVirtualY( const LONG y);
      void SetXPageSize( const SHORT p) { xPage = p; 	CalcHorzSize(); }
      void SetYPageSize( const SHORT p) { yPage = p; 	CalcVertSize(); }
      void SetXStepSize( const SHORT p) { xStep = p; }
      void SetYStepSize( const SHORT p) { yStep = p; }
      void SetXScrollPos( const USHORT s);
      void SetYScrollPos( const USHORT s);
      virtual void VScroll( LONG );
};

#endif
