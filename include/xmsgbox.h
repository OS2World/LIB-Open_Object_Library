#ifndef __OOL_XMSGBOX_H__
#define __OOL_XMSGBOX_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMessageBox                           */
/*                       derived from: XObject                               */
/*                        last update: 9/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xobject.h"
#include "XRes.h"
#include "XWindow.h"

#define MBOX_OK               0x0000
#define MBOX_OKCANCEL         0x0001
#define MBOX_RETRYCANCEL      0x0002
#define MBOX_ABORTRETRYIGNORE 0x0003
#define MBOX_YESNO            0x0004
#define MBOX_YESNOCANCEL      0x0005
#define MBOX_CANCEL           0x0006
#define MBOX_ENTER            0x0007
#define MBOX_ENTERCANCEL      0x0008

#define MBOX_NOICON           0x0000
#define MBOX_CUANOTIFICATION  0x0000
#define MBOX_CUAWARNING       0x0020
#define MBOX_ICONQUESTION     0x0010
#define MBOX_ICONEXCLAMATION  0x0020
#define MBOX_ICONASTERISK     0x0030
#define MBOX_ICONHAND         0x0040
#define MBOX_CUACRITICAL      0x0040
#define MBOX_QUERY            MBOX_ICONQUESTION
#define MBOX_WARNING          MBOX_CUAWARNING
#define MBOX_INFORMATION      MBOX_ICONASTERISK
#define MBOX_CRITICAL         MBOX_CUACRITICAL
#define MBOX_ERROR            MBOX_CRITICAL
#define MBOX_CUSTOMICON       0x0080

#define MBOX_DEFBUTTON1       0x0000
#define MBOX_DEFBUTTON2       0x0100
#define MBOX_DEFBUTTON3       0x0200

#define MBOX_APPLMODAL        0x0000
#define MBOX_SYSTEMMODAL      0x1000
#define MBOX_HELP             0x2000
#define MBOX_MOVEABLE         0x4000
#define MBOX_NONMODAL         0x8000

#define MDID_OK               1
#define MDID_CANCEL           2
#define MDID_ABORT            3
#define MDID_RETRY            4
#define MDID_IGNORE           5
#define MDID_YES              6
#define MDID_NO               7
#define MDID_HELP             8
#define MDID_ENTER            9
#define MDID_ERROR            0xffff


class _export_ XMessageBox: public XObject
{
   private:
     USHORT res;
   public:
     XMessageBox( const char *, const char * = NULL, const ULONG style = MBOX_OK , const XWindow * owner = NULL, const ULONG id = 0);
     XMessageBox( const XResource * ms1, const XResource * ms2 = NULL, const ULONG style = MB_OK , const XWindow * owner = NULL, const ULONG id = 0);
     virtual ~XMessageBox() {;}
     USHORT GetCommand( void ) const { return res; }
};


LONG XShowMessage( const char * s, const char * s2 = NULL, const ULONG style = MB_OK|MB_ERROR, const XWindow * owner=NULL);
LONG XShowMessage( const char * s, const ULONG style = MB_OK|MB_ERROR, const XWindow * owner=NULL);

#endif
