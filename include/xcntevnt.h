#ifndef __OOL_XCONTAINEREVENT_H__
#define __OOL_XCONTAINEREVENT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XContainerEvent                       */
/*                       derived from: XEvent                                */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xcntrevn.h"


#ifdef OOL_FOR_OS2_X86

   //message-idïs

   #define CON_BEGINEDIT    CN_BEGINEDIT
   #define CON_COLLAPSTREE  CN_COLLAPSTREE
   #define CON_CONTEXTMENU  CN_CONTEXTMENU
   #define CON_DRAGOVER     CN_DRAGOVER
   #define CON_DROP         CN_DROP
   #define CON_EMPHASIS     CN_EMPHASIS
   #define CON_ENDEDIT      CN_ENDEDIT
   #define CON_ENTER        CN_ENTER
   #define CON_EXPANDTREE   CN_EXPANDTREE
   #define CON_INITDRAG     CN_INITDRAG
   #define CON_PAINTBACKGOUND CM_PAINTBACKGROUND
   #define CON_REALLOC      CN_REALLOCPSZ
#endif


class XContainerObject;


class XContainerControl;


class _export_ XContainerEvent: public XControlEvent
{
      friend MRESULT HandleFrameDefault( XFrameWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   protected:
      RECORDCORE * core;
      XContainerEvent( const XContainerControl * w, const MPARAM mp1, const MPARAM mp2);
   public:
      XContainerControl * GetWindow(void) const { return (XContainerControl *) XControlEvent::GetWindow(); }
      XContainerObject * GetObject( void );
};


#endif
