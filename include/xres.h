#ifndef __OOL_XRESOURCE_H__
#define __OOL_XRESOURCE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XResource                             */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xobject.h"

class XResourceLibrary;
class XWindow;

class _export_ XResource: public XObject
{
   private:
      XResourceLibrary * library;
      unsigned long id;
   public:
      XResource( const unsigned long resId, const XResourceLibrary * lib) { id = resId; library = (XResourceLibrary*) lib; }
      XResourceLibrary * GetResourceLibrary( void ) const { return library; }
      virtual ~XResource() {;}
      ULONG GetID(void) const { return id; }
      void SetID( const LONG i) { id = i; }
};

#endif
