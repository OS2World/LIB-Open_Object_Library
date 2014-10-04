#ifndef __OOL_XLED_H__
#define __OOL_XLED_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XLED                                  */
/*                       derived from: XUserWindow                           */
/*                        last update: 7/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "XfrmWnd.h"
#include "xuserwnd.h"
#include "xtext.h"

class XBitmap;
class XGraphicDevice;


class _export_ XLED: public XUserWindow
{
   private:
      BOOL state;
      XGraphicDevice * dev;
      XBitmap * map;
   public:
      virtual void Draw( void );
      XLED( const XWindow *, const XPoint& = XPoint(), const USHORT id = 0);
      ~XLED();
      void Enable( const BOOL on = TRUE );
      BOOL GetState() { return state; }
};

#endif
