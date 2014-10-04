#ifndef __OOL_XOBJWND_H__
#define __OOL_XOBJWND_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XObjectWindow                         */
/*                       derived from: XWindow                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xwindow.h"

class XFrameWindow;

class _export_ XObjectWindow: public XWindow{
      friend MRESULT EXPENTRY ObjectProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
   protected:
      XFrameWindow * owner;
      XObjectWindow( const XFrameWindow * id);
      virtual ~XObjectWindow() { ;}
};

#endif
