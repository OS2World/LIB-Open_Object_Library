#ifndef __XDBASEEXCEPTION_H__
#define __XDBASEEXCEPTION_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDBaseException                       */
/*                       derived from: XException                            */
/*                        last update: 5/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "Xheaders.h"
#include XException_i


class _export_ XDBaseException: public XException
{
   public:
      XDBaseException(char * errorMsg, LONG errorCode, LONG lineNumber=0, char * fileName=""):XException(errorMsg, errorCode, lineNumber, fileName)
         {;}
};

#define OOLDbaseThrow(a, b) throw( XDBaseException(a, b, __LINE__, __FILE__))

#endif
