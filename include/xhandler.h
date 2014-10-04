#ifndef __OOL_XHANDLER_H__
#define __OOL_XHANDLER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XHandler                              */
/*                       derived from: XObject                               */
/*                        last update: 9/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"

class XWindow;


#define OOL_DRAGHANDLER                  1
#define OOL_MOUSEHANDLER                 2
#define OOL_CONTAINERHANDLER             3
#define OOL_KEYBHANDLER                  4
#define OOL_ITMDRAWHANDLER               5
#define OOL_BACKDRAWHANDLER              6
#define OOL_STYLEHANDLER                 7
#define OOL_DEFAULTHANDLER               8
#define OOL_TIMER                        9
#define OOL_NOTEBOOKHANDLER             10
#define OOL_LAYOUTHANDLER               11

class _export_ XHandler: public XObject
{
      friend XWindow;
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   private:
      USHORT id;
      XWindow * handleFor;
   protected:
      XHandler( const USHORT id, const XWindow * handleWindow);
   public:
      USHORT GetID( void ) const { return id; }
      virtual ~XHandler();
      XWindow * GetWindow(void) const { return handleFor; }
};


#endif
