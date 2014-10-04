#ifndef __XDIALOG_H__
#define __XDIALOG_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDialog                                */
/*                       derived from: XWindow                               */
/*                        last update: 6/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "XWindow.h"


class _export_ XDialog: public XWindow
{
   protected:
      XDialog( ) { winhandle = 0; }      
   public:
      virtual BOOL DoCommand( LONG ) { return TRUE; }
      virtual BOOL DoSysCommand( USHORT) { return TRUE;}
      virtual ~XDialog();
};

#endif
