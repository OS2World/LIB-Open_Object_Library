#ifndef __OOL_XWIZCLIWND_H__
#define __OOL_XWIZCLIWND_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XWizClientWindow                      */
/*                       derived from: XResourceWindow                       */
/*                        last update: 2/98                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xreswin.h"

class _export_ XWizClientWindow: public XResourceWindow
{
   friend class XWizard;
      XWizClientWindow *next, *prev;
   public:
      XWizClientWindow(XWizard * w, ULONG id):XResourceWindow((XWindow*)w,id) { Show(FALSE); next = prev = NULL;}
      XWizClientWindow(XWizard * w, XResource*r):XResourceWindow((XWindow*) w,r) { Show(FALSE); next = prev = NULL;}
      virtual BOOL DoCommand(LONG com) { QueryWindow(QW_PARENT)->DoCommand(com); return FALSE; }
      virtual void DoControl(XControlEvent*e) { QueryWindow(QW_PARENT)->DoControl(e); }
      void SetNextPage(XWizClientWindow*w) { next = w; w->prev = this; }
      void SetPrevPage(XWizClientWindow*w) { prev = w; w->next = this; }
};

#endif