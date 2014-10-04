#ifndef __OOL_XCHECKBOX_H__
#define __OOL_XCHECKBOX_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XCheckBox                             */
/*                       derived from: XSettingButton                        */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/




#include "xsetbtn.h"

class _export_ XCheckBox: public XSettingButton{
      friend void BuildChilds( HWND );
   protected:
      XCheckBox( const HWND hwnd): XSettingButton(hwnd) {;}
   public:
      XCheckBox( const XWindow * owner, const XRect& rect = XRect(), const USHORT id = 0, const LONG style = WIN_VISIBLE, const char * s = "", const char*font=NULL);
      virtual ~XCheckBox() {;}
};


#endif
