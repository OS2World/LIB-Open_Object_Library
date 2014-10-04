#ifndef __OOL_XPUSHBTN_H__
#define __OOL_XPUSHBTN_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XPushButton                           */
/*                       derived from: XButton                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include  "xbutton.h"


class _export_ XPushButton: public XButton{
      friend void BuildChilds( HWND );
   protected:
      XPushButton( const HWND hwnd): XButton(hwnd) { ;}
   public:
      XPushButton( const XWindow * owner, const XRect& rec=XRect(), const USHORT id = 0, const ULONG style = WIN_VISIBLE, const char * s = "", const char*font=NULL);
      virtual ~XPushButton() {;}
};


#endif
