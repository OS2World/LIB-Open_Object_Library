#ifndef __OOL_XNOTEBOOKHANDLER_H__
#define __OOL_XNOTEBOOKHANDLER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XNoteBookHandler                      */
/*                       derived from: XHandler                              */
/*                        last update: 5/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xhandler.h"


class XNoteBookEvent;
class XNoteBook;

class _export_ XNoteBookHandler: public XHandler
{
   public:
      XNoteBookHandler( const XWindow * w): XHandler( OOL_NOTEBOOKHANDLER, w) { ;}
      virtual BOOL HandleEvent( XNoteBookEvent *) { return FALSE;}
};


#endif
