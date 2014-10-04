#ifndef __XMODALDIALOG_H__
#define __XMODALDIALOG_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XModalDialog                          */
/*                       derived from: XDialog                               */
/*                        last update: 4/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "XDialog.h"
class XResource;
class XResourceLibrary;


class _export_ XModalDialog: public XDialog
{
   friend MRESULT EXPENTRY ModalDProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
      void CreateDialog( const ULONG id, const XResourceLibrary * res, const XWindow * owner, BOOL center = TRUE);
   public:
      XModalDialog( const XResource * res, const XWindow * owner, const BOOL center = TRUE);
      XModalDialog( const ULONG id, const XWindow * owner, const BOOL center = TRUE);
      LONG Start (void);
};

#endif
