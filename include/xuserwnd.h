#ifndef __OOL_USERWND_H__
#define __OOL_USERWND_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XUserWindow                           */
/*                       derived from: XWindow                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xwindow.h"


class XResource;

class _export_ XUserWindow: public XWindow
{
      friend MRESULT userProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
   protected:
      XUserWindow ( const LONG handle);
      XUserWindow ( void );
      XUserWindow ( const XWindow *);
      virtual ~XUserWindow();
};


#endif
