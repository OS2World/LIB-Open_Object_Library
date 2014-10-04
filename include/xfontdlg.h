#ifndef __XFONTDLG_H__
#define __XFONTDLG_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XFontDialog                           */
/*                       derived from: XObject                               */
/*                        last update: 9/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/




#include "xsysdlg.h"
#include "XWindow.h"
#include "XString.h"

#ifdef OOL_FOR_OS2_X86
   #define FDLG_CENTER                  0x00000001
   #define FDLG_OWNERDRAWPREVIEW        0x00000004
   #define FDLG_HELPBUTTON              0x00000008
   #define FDLG_APPLYBUTTON             0x00000010
   #define FDLG_RESETBUTTON             0x00000020
   #define FDLG_MODELESS                0x00000040
   #define FDLG_INITFROMFATTRS          0x00000080
   #define FDLG_BITMAPONLY              0x00000100
   #define FDLG_VECTORONLY              0x00000200
   #define FDLG_FIXEDWIDTHONLY          0x00000400
   #define FDLG_PROPORTIONALONLY        0x00000800
   #define FDLG_NOSYNTHESIZEDFONTS      0x00001000
   #define FDLG_NATIONAL_LANGUAGE       0x80000000
#endif


class _export_ XFontDialog: public XSystemDialog
{
   private:
      FONTDLG fdlg;
   public:
      ~XFontDialog( );
      XFontDialog( const XWindow * owner, const char * fontName= "Helv", const SHORT fontSize = 10, const LONG fontStyle = 0, const LONG attr = FDLG_CENTER, const char * title = NULL, const char * preView = NULL );
      void GetFontName( XString * buffer ) const ;
      SHORT GetFontSize(void) const { return fdlg.fxPointSize / 65536 ;}
};

#endif
