#ifndef __OOL_XITMDRAWEVENT_H__
#define __OOL_XITMDRAWEVENT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XItemDrawEvent                        */
/*                       derived from: XEvent                                */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xevent.h"

class XBitmap;
class XContainerObject;
class XContainerColumn;

class _export_ XItemDrawEvent: public XEvent
{
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
      friend MRESULT HandleFrameDefault( XFrameWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   private:
      XItemDrawEvent( const LONG, const void*, const void*, const SHORT t);
      POWNERITEM item;
      LONG windowID;
      SHORT type;
      LONG color, bColor;
      LONG textStyle;
   public:
      void DrawItem( const XBitmap * bitmap, const char * title = "", const BOOL drawOver = FALSE);
      LONG GetWindowID( void ) const { return windowID; }
      XContainerObject * GetObject(void);
      XContainerColumn * GetColumn(void) const ;
      LONG GetItemID( void ) const { return item->idItem; }
      ULONG GetItemHandle(void) const { return item->hItem; }
      OOL_WINDOWHANDLE GetWindowHandle( void ) const { return item->hwnd; }
      void SetTextColor( const XColor * col) { color = col->GetColor(); }
      void SetBackgroundColor( const XColor * col) { bColor = col->GetColor(); }
      void SetTextStyle( const LONG t) { textStyle = t; }
};


#endif
