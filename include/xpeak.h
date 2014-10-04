#ifndef __OOL_PEAKM_H__
#define __OOL_PEAKM_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XPeakMeter                            */
/*                       derived from: XUserWindow                           */
/*                        last update: 7/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "XfrmWnd.h"
#include "xuserwnd.h"
#include "xtext.h"

class XBitmap;
class XGraphicDevice;

class _export_ XPeakMeter: public XUserWindow
{
   private:
      USHORT level;
      XGraphicDevice * dev;
      XBitmap * mapGreen, * mapRed;
      BOOL vert;
   public:
      virtual void Draw( void );
      XPeakMeter( const XWindow *, const XPoint& = XPoint(), const USHORT id = 0, const BOOL vertical = TRUE);
      ~XPeakMeter();
      void SetLevel( USHORT );
      SHORT GetLevel( ) { return level; }
};

#endif
