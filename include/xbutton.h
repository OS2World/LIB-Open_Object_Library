#ifndef __OOL_XBUTTON_H__
#define __OOL_XBUTTON_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XButton                               */
/*                       derived from: XWindow                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xcontrol.h"

#ifdef OOL_FOR_OS2_X86
   #define BU_3STATE         5
   #define BU_AUTO3STATE     6
   #define BU_USERBUTTON     7

   #define BU_PRIMARYSTYLES  0x000f
   #define BU_TEXT           0x0010
   #define BU_MINIICON       0x0020
   #define BU_BITMAP         0x0040
   #define BU_ICON           0x0080
   #define BU_HELP           0x0100
   #define BU_DEFAULT        0x0400
   #define BU_NOPOINTERFOCUS 0x0800
   #define BU_NOBORDER       0x1000
   #define BU_NOCURSORSELECT 0x2000
   #define BU_AUTOSIZE       0x4000
#endif

class _export_ XButton: public XControl
{
   protected:
       XButton(  const XWindow * owner, const XRect * rec, const USHORT id, const ULONG style, const char * s , const unsigned char *classStyle, const char * font=NULL);
       XButton( const HWND hwnd ): XControl(hwnd) {;}
       virtual ~XButton() {;}
};


#endif
