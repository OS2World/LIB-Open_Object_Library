#ifndef __XSEMAPHORE_H__
#define __XSEMAPHORE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XSemaphore                            */
/*                       derived from: XObject                               */
/*                        last update: 3/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"

class _export_ XSemaphore:public XObject
{
   protected:
      ULONG hev;
      XSemaphore() { hev = 0; }
};

#endif
