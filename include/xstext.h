#ifndef __OOL_XSTEXT_H__
#define __OOL_XSTEXT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XStaticText                           */
/*                       derived from: XStatic                               */
/*                        last update: 1/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xstatic.h"

#define TX_TOP        DT_TOP
#define TX_VCENTER    DT_VCENTER
#define TX_BOTTOM     DT_BOTTOM
#define TX_LEFT       DT_LEFT
#define TX_CENTER     DT_CENTER
#define TX_RIGHT      DT_RIGHT
#define TX_WORDBREAK  DT_WORDBREAK

class _export_ XStaticText: public XStatic
{
      friend void BuildChilds( HWND );
   private:
      LONG color;
      XStaticText( const HWND hwnd):XStatic(hwnd) {;}
   public:
      XStaticText( const XWindow * w, const char * text = NULL, const XRect& p=XRect(), const LONG style = TX_LEFT | ST_AUTOSIZE | TX_VCENTER | WS_VISIBLE, const LONG id = 0, char * font = NULL);
      virtual ~XStaticText() {;}
};

#endif
