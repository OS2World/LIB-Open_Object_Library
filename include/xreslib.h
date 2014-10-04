#ifndef __OOL_XRESLIB_H__
#define __OOL_XRESLIB_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XResourceLibrary                      */
/*                       derived from: XObject                               */
/*                        last update: 6/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"

class XWindow;
class XString;

class _export_ XResourceLibrary: public XObject
{
      friend class XDummyApplication;
   private:
public:
      HMODULE moduleHandle;
   public:
      XResourceLibrary( const char * path = NULL);
      ~XResourceLibrary();
      ULONG GetModuleHandle( void ) const { return moduleHandle; }
      PFNWP LoadFunction( char * name);
      BOOL LoadString( XString * string, const unsigned long resId);
      OOL_WINDOWHANDLE LoadMenu( const XWindow * f, const unsigned long id);
      BOOL UnLoadFunction( PFNWP pfnwp);
};


#endif
