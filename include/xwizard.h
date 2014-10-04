#ifndef __OOL_XWIZARD_H__
#define __OOL_XWIZARD_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XWizard                               */
/*                       derived from: XModalDialog                          */
/*                        last update: 2/98                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xmodal.h"
#include "xcwiz.h"

class _export_ XWizard: public XModalDialog
{
   private:
      XWizClientWindow * start, * current;
      void SetClientSize();
   public:
      XWizard(const XWindow*,const ULONG);
      void SetStartPage( XWizClientWindow * s) { start = s; current = start; SetClientSize();}
      BOOL ShowNextPage();
      BOOL ShowPrevPage();
      void Init();
      XWizClientWindow * GetCurrentPage() { return current; }
};

#endif