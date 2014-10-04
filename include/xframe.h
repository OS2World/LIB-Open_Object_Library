#ifndef __OOL_XFRAME_H__
#define  __OOL_XFRAME_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XFrame                                */
/*                       derived from: XControl                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xcontrol.h"

class XFrameControl;

class _export_ XFrame: public XControl{
      friend class XFrameWindow;
      friend class XScrollWindow;
      friend class XToolBar;
      friend class XMenuBar;
      friend class XProcess;
   private:
      USHORT adds;
      USHORT controls;
      XFrameControl ** cont;
      XFrameWindow * frame;
   public:
      MRESULT HandleMessage( ULONG msg, MPARAM mp1, MPARAM mp2 );
      XFrame( const XFrameWindow * );
      XFrame( const HWND hwnd);
      virtual ~XFrame();
      void AddFrameControl( XFrameControl * f);
      virtual XWindow * QueryWindow( const ULONG id ) const ;
      void RemoveFrameControl( XFrameControl * f);
};

#endif
