#ifndef __OOL_XCRYPT_H__
#define __OOL_XCRYPT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XCrypt                                */
/*                       derived from: XObject                               */
/*                        last update: 1/98                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"
#include "xstring.h"

class XCrypt: public XObject
{
   public:
      static XString Crypt16( const XString& data, const XString& key);
      static XString BuildMD5Key( const XString& userData);
};

#endif