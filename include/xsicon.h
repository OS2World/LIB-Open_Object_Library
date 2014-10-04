#ifndef __OOL_XSICON_H__
#define __OOL_XSICON_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XStaticIcon                           */
/*                       derived from: XStatic                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xstatic.h"


class _export_ XStaticIcon: public XStatic
{
      friend void BuildChilds( HWND );
   private:
      XStaticIcon( const HWND hwnd):XStatic(hwnd){;}
   public:
      XStaticIcon( const XWindow * w, const char * , const XRect& p=XRect(), const LONG id = 0, const LONG style=ST_AUTOSIZE);
      virtual ~XStaticIcon() { ;}
};

#endif
