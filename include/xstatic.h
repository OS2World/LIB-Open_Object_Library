#ifndef __OOL_XSTATIC_H__
#define __OOL_XSTATIC_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XStatic                               */
/*                       derived from: XWindow                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xcontrol.h"

#define ST_AUTOSIZE   SS_AUTOSIZE

class _export_ XStatic: public XWindow
{
      friend MRESULT StaticProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
      PFNWP oldfunc;
   protected:
      XStatic( const OOL_WINDOWHANDLE hwnd);
      XStatic( const ULONG style, const char * title, const PSZ classname, const XWindow * ow, const XRect& r, const LONG id = 0, const char * font = NULL);
   public:
      virtual ~XStatic() {;}
};

#endif
