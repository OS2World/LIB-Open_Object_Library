#ifndef __OOL_XEXTMSGBOX_H__
#define __OOL_XEXTMSGBOX_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XExtendedMessageBox                   */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xobject.h"
#include "string.h"

class XResource;
class XWindow;
class XIcon;

class _export_ XMessageButton: public XObject
{
      friend class XExtendedMessageBox;
      MB2D info;
   public:
      XMessageButton(const char * title, const ULONG id=0, const ULONG style = BS_PUSHBUTTON)
            {strcpy(info.achText, title); info.flStyle = style; info.idButton = id; }
};


class _export_ XExtendedMessageBox: public XObject
{
   private:
     ULONG res;
   public:
     XExtendedMessageBox( const char * s1, const char * s2, const ULONG style, const SHORT buttonCount, const XMessageButton ** buttonList, const XIcon * icon = NULL, const XWindow * owner = NULL, const ULONG id = 0);
     XExtendedMessageBox( const XResource * ms1, const XResource * ms2, const ULONG style, const SHORT buttonCount, const XMessageButton ** buttonList, const XIcon * icon = NULL, const XWindow * owner = NULL, const ULONG id = 0);
     virtual ~XExtendedMessageBox() {;}
     ULONG GetCommand( void ) const { return res; }
};

#endif
