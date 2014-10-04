#ifndef __OOL_XSETTINGBTN_H__
#define __OOL_XSETTINGBTN_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XSettingButton                        */
/*                       derived from: XButton                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xbutton.h"


class _export_ XSettingButton: public XButton
{
   protected:
      XSettingButton( const HWND hwnd): XButton(hwnd) {;}
      XSettingButton( const XWindow * owner, const XRect * rec, const USHORT id, const ULONG style, const char * s , const unsigned char *classStyle, const char*font=NULL);
   public:
      virtual ~XSettingButton() {;}
      virtual BOOL IsSelected( void ) const ;
      virtual void Select( const BOOL select = TRUE);
};


#endif
