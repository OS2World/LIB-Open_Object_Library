#ifndef __OOL_XSLIDER_H__
#define __OOL_XSLIDER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XSlider                               */
/*                       derived from: XControl                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xcontrol.h"

#ifdef OOL_FOR_OS2_X86
   #define SL_HORIZONTAL         0x0000
   #define SL_VERTICAL            0x0001
   #define SL_CENTER               0x0000
   #define SL_BOTTOM               0x0002
   #define SL_TOP                  0x0004
   #define SL_LEFT               0x0002
   #define SL_RIGHT               0x0004
   #define SL_SNAPTOINCREMENT      0x0008
   #define SL_BUTTONSBOTTOM      0x0010
   #define SL_BUTTONSTOP         0x0020
   #define SL_BUTTONSLEFT         0x0010
   #define SL_BUTTONSRIGHT         0x0020
   #define SL_READONLY            0x0080
   #define SL_RIBBONSTRIP         0x0100
   #define SL_HOMEBOTTOM         0x0000
   #define SL_HOMETOP            0x0200
   #define SL_HOMELEFT            0x0000
   #define SL_HOMERIGHT              0x0200
   #define SL_PRIMARYSCALE1      0x0000
   #define SL_PRIMARYSCALE2      0x0400
#endif


class _export_ XSlider: public XControl{
      friend void BuildChilds( HWND );
   private:
      XSlider( const HWND hwnd): XControl(hwnd) {;}
   public:
      XSlider( const XWindow * owner, const XRect& rec=XRect(), const USHORT id = 0, const ULONG style = WIN_VISIBLE|SL_HOMELEFT|SL_LEFT|SL_HORIZONTAL, const char*font=NULL);
      virtual ~XSlider() {;}
      LONG AddDetent( const SHORT detent) const ;
      USHORT GetDetentPos( const LONG detentID) const ;
      void GetScaleText( const SHORT tickNumber, XString * buffer) const;
      LONG GetSliderPos( void ) const ;
      void RemoveDetent( const LONG detendID ) const ;
      void SetScales( const USHORT scale1Incr, const USHORT scale1Space, const USHORT scale2Incr = 0, const USHORT scale2Space = 0);
      void SetScaleText( const SHORT tickNumber, const char * text) const ;
      void SetShaftSize( const SHORT size ) const ;
      void SetSliderPos( const LONG pos) const ;
      void SetSliderSize( const SHORT vert, const SHORT horz ) const ;
      void SetTickSize( const SHORT tickNumber = SMA_SETALLTICKS, const SHORT tickLength = 10) const ;
};


#endif
