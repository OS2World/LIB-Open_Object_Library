#ifndef __OOL_XEXCEPT_H__
#define __OOL_XEXCEPT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XException                            */
/*                       derived from: XObject                               */
/*                        last update: 5/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"
#include "xstring.h"
#include <stdio.h>

class XWindow;


class _export_ XException
{
      XString msg, file;
      LONG code, line;
   public:
      XException(const char * errorMsg, const LONG errorCode, const LONG lineNumber=0, const char * fileName="")
            { msg = errorMsg; code = errorCode; line = lineNumber; file = fileName; }
      char * GetErrorMessage(void) const { return (char*) msg; }
      LONG GetErrorCode(void) const { return code; }
      void ShowError( const XWindow * w=NULL) const ;
      void PrintError(void) const ;
};

#define OOLThrow(a, b) throw( XException(a, b, __LINE__, __FILE__))

#endif
