#ifndef __OOL_XSBITMAP_H__
#define __OOL_XSBITMAP_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XStaticBitmap                         */
/*                       derived from: XStatic                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xstatic.h"

class XBitmap;
class XResource;

class _export_ XStaticBitmap: public XStatic
{
      friend void BuildChilds( HWND );
   private:
      XStaticBitmap( const OOL_WINDOWHANDLE hwnd):XStatic(hwnd) {;}
      XBitmap * bmp;
   public:
      XStaticBitmap( const XWindow * w, const char * , const XRect& p=XRect(), const LONG id = 0, const LONG style=ST_AUTOSIZE);
      virtual ~XStaticBitmap();
      void Load(char * path);
      void Load(XResource*);
      void Load(ULONG id);
};

#endif