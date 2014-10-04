#ifndef __OOL_XNOTEBOOKEVENT_H__
#define __OOL_XNOTEBOOKEVENT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XNoteBookEvent                        */
/*                       derived from: XEvent                                */
/*                        last update: 5/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xcntrevn.h"


#ifdef OOL_FOR_OS2_X86

   //message-idïs
      #define XNOTEBOOK_PAGESELECTED            BKN_PAGESELECTED      
      #define XNOTEBOOK_NEWPAGESIZE               BKN_NEWPAGESIZE
      #define XNOTEBOOK_HELP                     BKN_HELP         
      #define XNOTEBOOK_PAGEDELETED               BKN_PAGEDELETED
      #define XNOTEBOOK_PAGESELECTEDPENDING      BKN_PAGESELECTEDPENDING

#endif


class XNoteBook;
class XNoteBookPage;

class _export_ XNoteBookEvent: public XControlEvent
{
      friend MRESULT HandleFrameDefault( XFrameWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   protected:
      XNoteBookPage * bookPage;
      XNoteBookEvent( const XWindow * w, const void *mp1, const void * mp2);
   public:
      XNoteBook * GetWindow(void) const { return (XNoteBook *) XControlEvent::GetWindow(); }
      XNoteBookPage * GetPage() { return bookPage; }
};


#endif
