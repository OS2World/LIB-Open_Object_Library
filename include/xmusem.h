#ifndef __XMUTEXSEM_H__
#define __XMUTEXSEM_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMutexSemaphore                       */
/*                       derived from: XSemaphore                            */
/*                        last update: 3/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xsem.h"

class _export_ XMutexSemaphore:public XSemaphore
{
   public:
      XMutexSemaphore(char * name, BOOL shared, BOOL owned=TRUE);
      XMutexSemaphore(char * name);
      ~XMutexSemaphore();
      void Close();
      void Release();
      void Request(LONG timeOut);
};

#endif
