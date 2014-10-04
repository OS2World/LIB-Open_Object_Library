#ifndef __OOL_XMENUBAR_H__
#define __OOL_XMENUBAR_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMenuBar                              */
/*                       derived from: XMenu                                 */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xmenu.h"

class XFrameWindow;
class XDialog;
class XResource;
class XResourceLibrary;

class _export_ XMenuBar: public XMenu{
      friend void BuildChilds( HWND );
      friend XFrameWindow;
   private:
      XMenuBar( const LONG hwnd);
      void CreateMenu( const OOL_WINDOWHANDLE, const ULONG id, const XResourceLibrary*);
   public:
      XMenuBar( const XFrameWindow *, const XResource*);
      XMenuBar( const XFrameWindow *, const ULONG);
      XMenuBar( const XDialog *, const XResource*);
      XMenuBar( const XDialog *, const ULONG);
      virtual ~XMenuBar();
      void Show( const BOOL show = TRUE);
};


#endif
