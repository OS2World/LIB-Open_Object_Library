#ifndef __XEVENTSEM_H__
#define __XEVENTSEM_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XEventSemaphore                       */
/*                       derived from: XSemaphore                            */
/*                        last update: 3/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xsem.h"

class _export_ XEventSemaphore:public XSemaphore
{
   public:
      XEventSemaphore(const char * name, const BOOL shared, const BOOL isSet=FALSE);
      XEventSemaphore(const char * name);
      ~XEventSemaphore();
      ULONG Reset();
      void Close();
      void Post();
      void Wait( const LONG timeOut);
};

#endif
