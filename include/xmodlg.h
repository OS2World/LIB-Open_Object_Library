#ifndef __XMLESSDIALOG_H__
#define __XMLESSDIALOG_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XModelessDialog                       */
/*                       derived from: XDialog                               */
/*                        last update: 4/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "XDialog.h"
class XResource;
class XResourceLibrary;

class _export_ XModelessDialog: public XDialog
{
   friend MRESULT EXPENTRY ModelessDProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
      void CreateDialog( const ULONG id, const XResourceLibrary * res, const XWindow * owner=NULL, const BOOL center = TRUE);
   public:
      XModelessDialog( const XResource * res, const XWindow * owner=NULL, const BOOL center = TRUE);
      XModelessDialog( const ULONG id, const XWindow * owner=NULL, const BOOL center = TRUE);
};

#endif
