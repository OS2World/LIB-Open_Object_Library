#ifndef __XCURSOR_H__
#define __XCURSOR_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XCursor                               */
/*                       derived from: XObject                               */
/*                        last update: 11/97                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xwindow.h"

class XCursor: public XObject
{
      XWindow * owner;
      char vis;
   public:
      XCursor( const XFrameWindow * w, const XPoint &p, const XSize& s = XSize(), const LONG type = CURSOR_SOLID|CURSOR_FLASH)
      {
         owner = (XWindow*) w;
         vis = 0;
         if( WinCreateCursor( owner->GetHandle(), p.GetX(), p.GetY(), s.GetWidth(), s.GetHeight(), type, NULL) == FALSE )
            OOLThrow( "couldnt create cursor", -1);
      }
      ~XCursor()
      {
         Show(FALSE);
         WinDestroyCursor( owner->GetHandle());
      }
      void SetPosition( const XPoint& p)
      {
         WinCreateCursor( owner->GetHandle(), p.GetX(), p.GetY(), 0, 0, CURSOR_SETPOS, NULL);
      }
      void SetPosition( const LONG x, const LONG y)
      {
         WinCreateCursor( owner->GetHandle(), x, y, 0, 0, CURSOR_SETPOS, NULL);
      }
      void Show( const BOOL s = TRUE)
      {
         WinShowCursor( owner->GetHandle(), s);
      }
};

#endif
