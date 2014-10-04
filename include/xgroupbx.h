#ifndef __OOL_XGROUPBOX_H__
#define __OOL_XGROUPBOX_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XGroupBox                             */
/*                       derived from: XStatic                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xstatic.h"

class _export_ XGroupBox: public XStatic{
      friend void BuildChilds( HWND );
   private:
      XGroupBox( const HWND hwnd):XStatic(hwnd) {;}
   public:
      XGroupBox( const XWindow * owner, const XRect& rec= XRect(), const char * string = "", const LONG id = 0);
      virtual ~XGroupBox() {;}
};


#endif