#include "xobject.h"

#ifndef __OOL_XEXIT_H__
#define __OOL_XEXIT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XExitHandler                          */
/*                       derived from: XObject                               */
/*                        last update: 10/97                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

class XExitHandler: public XObject
{
   public:
      virtual BOOL QueryForQuit() { return TRUE; }
};

#endif
