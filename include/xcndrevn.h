#ifndef __OOL_XCONTDRAGEVENT_H__
#define __OOL_XCONTDRAGEVENT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XContainerDragEvent                   */
/*                       derived from: XContainerEvent                       */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xcntevnt.h"
#include "xdragitm.h"

class _export_  XContainerDragEvent: public XContainerEvent
{
      friend MRESULT HandleFrameDefault( XFrameWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   private:
      XContainerDragEvent( const XWindow *, const MPARAM, const MPARAM);
      SHORT accept;
      SHORT operation;
      DRAGINFO * dragInfo;
      ~XContainerDragEvent();
      PPRINTDEST printer;
   public:
      BOOL GetDragItem( XDragItem * itm, const SHORT index);
      SHORT GetDragItemCount( void ) { return dragInfo ? dragInfo->cditem : -1;}
      BOOL GetDropPos( XPoint * p);
      PRINTDEST * GetPrinterInfo(void) { return printer; }
      OOL_WINDOWHANDLE GetSourceWindow( void ) const { return dragInfo ? dragInfo->hwndSource : 0; }
      SHORT GetDragOperation(void) const { return dragInfo->usOperation; }
      void SetAcceptMode( const SHORT mode) { accept = mode; }
      void SetOperation( const SHORT op) { operation = op; }
};


#endif
