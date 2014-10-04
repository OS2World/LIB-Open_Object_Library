#ifndef __OOL_XICON_H__
#define __OOL_XICON_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XIcon                                 */
/*                       derived from: XObject                               */
/*                        last update: 6/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"

class XResource;

class _export_ XIcon: public XObject
{
      friend class XContainerObject;
   private:
      OOL_ICONHANDLE handle;
      BOOL loaded;
   public:
      XIcon() { handle = 0; loaded = FALSE;}
      ~XIcon();
      OOL_ICONHANDLE GetHandle( void ) const { return handle; }
      BOOL Load( const XResource *);
      BOOL Load( const ULONG id);
      BOOL Load( const char * file, const BOOL copy = FALSE );
      XIcon operator =(const XIcon& da)
         {
            loaded = da.loaded;
            handle = da.handle;
            return *this;
         }
};


#endif
