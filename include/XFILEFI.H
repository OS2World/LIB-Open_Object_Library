#ifndef __OOL_XFILEFIND_H__
#define __OOL_XFILEFIND_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XFileFind                             */
/*                       derived from: XObject                               */
/*                        last update: 8/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xstring.h"
#include "xobject.h"


class XString;
class XFileInfo;

class _export_ XFileFind: public XObject
{
   private:
      HDIR dir;
      XString mask;
      BOOL init;
   public:
      XFileFind( const char * file);
      ~XFileFind();
      BOOL Find( XString *, XFileInfo *);
};

#endif
