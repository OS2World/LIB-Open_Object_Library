#ifndef __OOL_XGRAPHICBTN_H__
#define __OOL_XGRAPHICBTN_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XGraphicButton                        */
/*                       derived from: XSettingButton                        */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/




#include "xsetbtn.h"

#ifdef OOL_FOR_OS2_X86
   //syles
   #define GB_TWOSTATE         0x1000
   #define GB_AUTOTWOSTATE     0X2000
   #define GB_ANIMATION        0x4000
   #define GB_AUTOANIMATION    0x8000
   #define GB_DISABLEBITMAP    0x0010
   #define GB_HILITEBITMAP     0x0020
   #define GB_3D_TEXTRECESSED  0x0040
   #define GB_3D_TEXTRAISED    0x0080
#endif

class _export_ XGraphicButton: public XSettingButton{
       friend void BuildChilds( HWND );
     private:
       XGraphicButton(HWND hwnd);
     public:
       XGraphicButton( const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char * s, const SHORT bitmapIdCount, const SHORT * bitmapIdList);
       virtual ~XGraphicButton() {;}
       BOOL Animate( const BOOL start = TRUE, const BOOL fromBeginning = TRUE);
       BOOL IsAnimationActive( void );
       BOOL IsSelected( void ) const;
       LONG GetAnimationRate( void) const ;
       void Select( const BOOL select = TRUE);
       BOOL SetAnimationRate( const LONG rate) const ;
       void Toggle( void ) const ;
};


#endif
