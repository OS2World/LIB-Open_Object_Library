#ifndef __OOL_XRESWND_H__
#define __OOL_XRESWND_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XResourceWindow                       */
/*                       derived from: XWindow                               */
/*                        last update: 2/98                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xwindow.h"
#include "xreslib.h"
#include "xres.h"

class _export_ XResourceWindow: public XWindow
{
      void CreateWindow(XWindow * , ULONG id, XResourceLibrary*);
   public:
      XResourceWindow(XWindow *, ULONG id);
      XResourceWindow(XWindow *, XResource*);
};


#endif