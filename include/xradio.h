#ifndef __OOL_XRADIO_H__
#define __OOL_XRADIO_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XRadioButton                          */
/*                       derived from: XSettingButton                        */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xsetbtn.h"

class _export_ XRadioButton: public XSettingButton{
      friend void BuildChilds( HWND );
   protected:
      XRadioButton( const HWND hwnd): XSettingButton(hwnd) {;}
   public:
      XRadioButton( const XWindow * owner, const XRect& rec=XRect(), const USHORT id = 0, const LONG style = WIN_VISIBLE, const char * s = "", const char*font=NULL);
      virtual ~XRadioButton() {;}
};


#endif
