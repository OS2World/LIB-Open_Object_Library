#ifndef __OOL_XDRAGEVENT_H__
#define __OOL_XDRAGEVENT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDragEvent                            */
/*                       derived from: XEvent                                */
/*                        last update: 7/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xevent.h"

#ifdef OOL_FOR_OS2_X86
   //events
   #define DRG_DROPPED   1
   #define DRG_DRAGOVER  2

   //target accept-mode
   #define DRG_DROP      DOR_DROP
   #define DRG_NODROP    DOR_NODROP
   #define DRG_NODROPOP  DOR_NODROPOP
   #define DRG_NEVERDROP DOR_NEVERDROP

   //target operation
   #define DRG_COPY            DO_COPY
   #define DRG_MOVE            DO_MOVE
   #define DRG_LINK            DO_LINK
   #define DRG_ENDCONVERSATION DM_ENDCONVERSATION
   #define DRG_DISCARDOBJECT   DM_DISCARDOBJECT
   #define DRG_PRINTOBJECT     DM_PRINTOBJECT
#endif


class XPoint;
class XDragItem;


class _export_ XDragEvent: public XEvent
{
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   private:
      SHORT accept;
      SHORT operation;
      DRAGINFO * dragInfo;
      ~XDragEvent();
      PPRINTDEST printer;
   public:
      XDragEvent( const LONG, const void*);
      BOOL GetDragItem( XDragItem * itm, const SHORT index);
      SHORT GetDragItemCount( void ) const { return dragInfo ? dragInfo->cditem : -1; }
      BOOL GetDropPos( XPoint * p) const;
      PRINTDEST * GetPrinterInfo(void) const { return printer; }
      OOL_WINDOWHANDLE GetSourceWindow( void ) const { return dragInfo ? dragInfo->hwndSource : 0; }
      SHORT GetDragOperation(void) const { return dragInfo->usOperation; }
      void SetAcceptMode( const SHORT mode) { accept = mode; }
      void SetOperation( const SHORT op) { operation = op; }
};


#endif
