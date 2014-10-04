#ifndef __OOL_XCIRCSLIDER_H__
#define __OOL_XCIRCSLIDER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XCircularSlider                       */
/*                       derived from: XControl                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xcontrol.h"

#ifdef OOL_FOR_OS2_X86
   #define CS_NOBUTTON            0x0001
   #define CS_NOTEXT               0x0002
   #define CS_NONUMBER            0x0004
   #define CS_POINTSELECT         0x0008
   #define CS_360                  0x0010
   #define CS_MIDPOINT            0x0020
   #define CS_PROPORTIONALTICKS   0x0040
   #define CS_NOTICKS            0x0080
   #define CS_CIRCULARVALUE      0x0100
#endif

class _export_ XCircularSlider: public XControl{
      friend void BuildChilds( HWND );
   private:
      XCircularSlider( const OOL_WINDOWHANDLE hwnd): XControl(hwnd) {;}
   public:
      XCircularSlider( const XWindow * owner, const XRect& rec = XRect(), const USHORT id = 0, const ULONG style=WIN_VISIBLE|CS_MIDPOINT, const char*font=NULL);
      virtual ~XCircularSlider() {;}
      SHORT GetValue( void );
      void GetIncremets( SHORT& scrollIncrement, SHORT& tickIncrement );
      void GetLimits( SHORT& lowerLimit, SHORT& upperLimit );
      BOOL SetIncrements( const SHORT ticks, const SHORT steps) const ;
      BOOL SetLimits( const LONG lowerLimit=0, const LONG upperLimit=100) const ;
      BOOL SetValue( const LONG val) const ;
};


#endif
