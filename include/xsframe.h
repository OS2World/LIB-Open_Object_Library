#ifndef __OOL_XSFRAME_H__
#define __OOL_XSFRAME_H__


/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XStaticFrame                          */
/*                       derived from: XStatic                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xstatic.h"


class _export_ XStaticFrame: public XStatic
{
      friend class XFrameWindow;
   private:
      XStaticFrame( const HWND hwnd):XStatic(hwnd) {;}
   public:
      XStaticFrame( const XWindow * w, const XRect& p=XRect(), const LONG id=0);
      virtual ~XStaticFrame() {;}
};

#endif
