#ifndef __OOL_MDICLIENT_H__
#define __OOL_MDICLIENT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMDIClient                            */
/*                       derived from: XFrameWindow                          */
/*                        last update: 5/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xscrlwnd.h"
class XMDIServer;

class _export_ XMDIClient: public XScrollWindow
{
      friend XMDIServer;
      XMDIServer * owner, * test;
      LONG GetID() { return ids; }
      LONG ids;
      void SetID(LONG i) { ids = i; }
      void Setup( const XMDIServer * p, const XRect * rect);
   public:
      XMDIClient( const XMDIServer * p, const XResource * r, const char * title="", const LONG style = XFrameWindow::defaultStyle | FRM_ICON, const XRect& rec = XRect(), const BOOL build=FALSE): XScrollWindow( r, title, XFrameWindow::defaultStyle | FRM_ICON, rec, (XFrameWindow*) p, build)
         { Setup(p, &rec);   }
      XMDIClient( const XMDIServer * p, const ULONG id, const char * title="", const LONG style = XFrameWindow::defaultStyle | FRM_ICON, const XRect& rec = XRect(), const BOOL build=FALSE): XScrollWindow( id, title, XFrameWindow::defaultStyle | FRM_ICON, rec, (XFrameWindow*)p, build)
         { Setup(p, &rec);   }
      ~XMDIClient();
      virtual BOOL DoSysCommand( USHORT syscom);
      virtual void FocusChanged( BOOL );
      virtual void UpdateMenu(XMenuBar*) {;}
      virtual void SetText( const char *);
      XMDIServer * GetOwner() { return owner; }
};

#endif
