#ifndef __OOL_XUSERBUTTON_H__
#define __OOL_XUSERBUTTON_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XUserButton                           */
/*                       derived from: XSettingButton                        */
/*                        last update: 03/98                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xsetbtn.h"
#include "xtext.h"
#include "xgraphdv.h"

class XBitmap;
class XLine;

#ifndef BU_TWOSTATE
   #define BU_TWOSTATE         0x8000
#endif

#ifndef BU_NOPOINTERFOCUS
   #define BU_NOPOINTERFOCUS   0x0800
#endif

#ifndef BU_NOCURSORSELECT
   #define BU_NOCURSORSELECT   0x1000
#endif

#ifndef BU_NOBORDER
   #define BU_NOBORDER         0x2000
#endif

class _export_ XUserButton: public XSettingButton
{
      friend XBitmap;
      //for internal use only:
      friend class userbtnhandler;
      friend class userbtnmousehandler;
      char isPressed;
      XText * text;
      XGraphicDevice * graph;
      LONG style;
      char buffer;
//      XLine * line1, *line2, *line3, *line4;
   protected:
      XBitmap * bmp[2];
      void Draw( void );
      void Setup( );
   public:
      XUserButton( const XWindow * w, const XRect& r=XRect(), const LONG id = 0, const LONG style = WIN_VISIBLE, const char * text = NULL, const SHORT yOff = 0);
      virtual ~XUserButton();
      virtual void GetText( XString * buffer ) const;
      XBitmap * GetBitmap( const UCHAR index) const { if(index == 0 || index == 1) return bmp[index]; else return NULL; }
      XGraphicDevice * GetGraphicDevice() const { return graph; }
      XText * GetTextPointer(void) const { return text; }
      virtual BOOL IsSelected( void ) const ;
      void SetBitmap( const XBitmap *, const UCHAR index = 0);
      virtual void Select( const BOOL select = TRUE);
      virtual void SetText( const char * text);
      void SetTextStyle( const LONG t) { ;}
      virtual void Toggle(void);
};

#endif
