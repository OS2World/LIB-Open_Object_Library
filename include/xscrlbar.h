#ifndef __OOL_XSCROLBAR_H__
#define __OOL_XSCROLBAR_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XScrollBar                            */
/*                       derived from: XControl                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xcontrol.h"

#define SC_HORZ   SBS_HORZ
#define SC_VERT   SBS_VERT
#define SC_AUTO   SBS_AUTOTRACK


class _export_ XScrollBar: public XControl{
      friend void BuildChilds( HWND );
   private:
      XScrollBar( const HWND hwnd): XControl(hwnd) {;}
   public:
      XScrollBar( const XWindow * owner, const XRect& rec=XRect(), const USHORT id = 0, const ULONG style = SC_VERT | SC_AUTO);
      virtual ~XScrollBar() {;}
      SHORT GetPos(void) const ;
      void GetRange( SHORT& first, SHORT& last);
      BOOL SetPos( const LONG pos) const ;
      BOOL SetRange( const SHORT first, const SHORT last, const SHORT sliderPos) const ;
      BOOL SetSliderSize( const SHORT visible, const SHORT total) const ;
};

#endif
