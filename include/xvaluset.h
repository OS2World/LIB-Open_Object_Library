#ifndef __OOL_XVALUESET_H__
#define __OOL_XVALUESET_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XValueSet                             */
/*                       derived from: XControl                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xcontrol.h"

#ifdef OOL_FOR_OS2_X86
   #define VA_BITMAP          0x0001
   #define VA_ICON            0x0002
   #define VA_TEXT            0x0004
   #define VA_COLOR           0x0008
   #define VA_BORDER          0x0020
   #define VA_ITEMBORDER      0x0040
   #define VA_SCALEBITMAPS    0x0080
   #define VA_RIGHTTOLEFT     0x0100
#endif

class XIcon;
class XBitmap;
class XSize;

class _export_ XValueSet: public XControl{
      friend void BuildChilds( HWND );
   private:
      XValueSet( const OOL_WINDOWHANDLE hwnd): XControl(hwnd){;}
   public:
      XValueSet( const XWindow * owner, const XRect& rec=XRect(), const USHORT id = 0, const ULONG style = VA_TEXT|WIN_VISIBLE|VA_BORDER, const char * font = NULL);
      virtual ~XValueSet() { ;}
      void GetSelection( SHORT& row, SHORT& col) const;
      void GetItem( const SHORT row, const SHORT col, XString * s) const;
      void GetItem( const SHORT row, const SHORT col, LONG& ) const;
      void GetItem( const SHORT row, const SHORT col, XColor * color) const;
      LONG GetItemAttribute( const SHORT row, const SHORT col) const ;
      void GetItemSize( XSize*) const;
      void GetItemSpacing( XSize*) const;
      void SelectItem( const SHORT row, const SHORT col) const ;
      void SetDimensions( const SHORT rows, const SHORT cols);
      void SetItem( const SHORT row, const SHORT col, const char * text) const ;
      void SetItem( const SHORT row, const SHORT col, const XColor * color) const ;
      void SetItem( const SHORT row, const SHORT col, const XIcon *) const ;
      void SetItem( const SHORT row, const SHORT col, const XBitmap *) const ;
      void SetItemAttribute( const SHORT row, const SHORT col, const SHORT attr, const BOOL enable = TRUE) const ;
      void SetItemSize( const XSize*) const ;
      void SetItemSpacing( const XSize*) const ;
};

#endif
