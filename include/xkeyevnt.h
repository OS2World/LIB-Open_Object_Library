#ifndef __OOL_XKEYBEVENT_H__
#define __OOL_XKEYBEVENT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XKeyboardEvent                        */
/*                       derived from: XEvent                                */
/*                        last update: 8/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xevent.h"

#ifdef OOL_FOR_OS2X86
   #define XKC_KEYUP   KC_KEYUP
   #define XKC_PREVDOWN   KC_PREVDOWN
   #define XKC_LONEKEY   KC_LONEKEY
   #define XKC_SHIFT   KC_SHIFT
   #define XKC_ALT   KC_ALT
   #define XKC_CTRL   KC_CTRL
#endif


class _export_ XKeyboardEvent: public XEvent
{
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   private:
      SHORT virtualKey, scanCode, flags;
      XKeyboardEvent( const SHORT key, const SHORT vKey, const SHORT scan, const SHORT flag): XEvent( key ) { virtualKey = vKey; scanCode = scan; flags = flag;}
   public:
      SHORT GetVirtualKey( void ) const { return virtualKey; }
      SHORT GetScanCode(void) const {return scanCode;}
      SHORT GetFlags(void) const { return flags; }
};


#endif
