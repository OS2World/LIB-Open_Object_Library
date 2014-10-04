#ifndef __OOL_XTABLEEXCEPT_H__
#define __OOL_XTABLEEXCEPT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XTableException                       */
/*                       derived from: XException                            */
/*                        last update: 3/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xexcept.h"

class _export_ XTableException: public XException
{
   public:
      XTableException(char * errorMsg, LONG errorCode, LONG lineNumber=0, char * fileName=""):XException(errorMsg, errorCode, lineNumber, fileName)
         {;}
};

#define OOLTableThrow(a, b) throw( XTableException(a, b, __LINE__, __FILE__))

#endif
