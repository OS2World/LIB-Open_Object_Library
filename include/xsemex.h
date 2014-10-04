#ifndef __OOL_XSEMEXCEPT_H__
#define __OOL_XSEMEXCEPT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XSemaphoreException                   */
/*                       derived from: XException                            */
/*                        last update: 3/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xexcept.h"

class _export_ XSemaphoreException: public XException
{
   public:
      XSemaphoreException(char * errorMsg, LONG errorCode, LONG lineNumber=0, char * fileName=""):XException(errorMsg, errorCode, lineNumber, fileName)
         {;}
};

#define OOLSemThrow(a, b) throw( XSemaphoreException(a, b, __LINE__, __FILE__))

#endif
