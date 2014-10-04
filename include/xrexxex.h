#ifndef __OOL_XREXXEXCEPT_H__
#define __OOL_XREXXEXCEPT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XRexxException                        */
/*                       derived from: XException                            */
/*                        last update: 3/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xexcept.h"

class _export_ XRexxException: public XException
{
   public:
      XRexxException(char * errorMsg, LONG errorCode, LONG lineNumber=0, char * fileName=""):XException(errorMsg, errorCode, lineNumber, fileName)
         {;}
};

#define OOLRexxThrow(a, b) throw( XRexxException(a, b, __LINE__, __FILE__))

#endif
