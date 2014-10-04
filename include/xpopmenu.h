#ifndef __OOL_XOPOPUP_H__
#define __OOL_XOPOPUP_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XPopupMenu                            */
/*                       derived from: XMenu                                 */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xmenu.h"

class XResource;
class XResourceLibrary;
class XPoint;
class XWindow;

class _export_ XPopupMenu: public XMenu
{
   private:
     XWindow * owner;
     void CreateMenu( const XResourceLibrary*, const ULONG id, const XWindow *);
   public:
     XPopupMenu( const XResource *, const XWindow *);
     XPopupMenu( const ULONG, const XWindow *);
     virtual ~XPopupMenu() {;}
     void Display( const XPoint * p, const USHORT def = 0, const ULONG style= PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_NONE | PU_MOUSEBUTTON1) const;
     BOOL IsPopup(void) const { return TRUE; }
};

#endif
