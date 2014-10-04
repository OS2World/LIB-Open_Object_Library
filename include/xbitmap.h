#ifndef __OOL_XBITMAP_H__
#define __OOL_XBITMAP_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XBitmap                               */
/*                       derived from: XGraphicObject                        */
/*                        last update: 3/98                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*                         OOLGLIB by: Mathias Hasselmann                    */
/*                                     mhasselm@arco.met.fu-berlin.de        */
/*===========================================================================*/


#include "xgraphob.h"
#include "XPoint.h"
#include "XRes.h"
#include "XWindow.h"
#include "xsize.h"
#include "oolglib.h"

class XResourceLibrary;
class XBitmapFilterParameter;

class _export_  XBitmap: public XGraphicObject
{
      friend class XClipBoard;
      friend class XItemDrawEvent;
      friend class XUserButton;
      friend class XContainerObject;
      friend class userbtnhandler;
      friend class userbtnmousehandler;
   private:
      XWindow * owner;
      HBITMAP hbm;
      HDC hdc;
      HPS hps;
      LONG SetData( const BITMAPINFOHEADER2 *, const LONG );
      BOOL HitTest( const LONG x, const LONG y);
      LONG cx, cy, mode, width, height;
      void Load( const ULONG, const XResourceLibrary *);
   public:
      XBitmap( const XPoint * p = NULL, const LONG mode = ROP_SRCCOPY);
      XBitmap( const XWindow *);
      ~XBitmap();
      void Copy( const XGraphicDevice *, const XPoint * );
      void Draw( XGraphicDevice * dev );
      ULONG GetHandle( void ) const { return hbm; }
      void Load( const XResource * r);
      void Load( const ULONG);
      void LoadBMP(const char* filename);
      void Load(const char* filename, XBitmapFilter* param = NULL, BOOL releaseParams = TRUE);
      void Save(const char* filename, XBitmapFilter* param = NULL, BOOL releaseParams = TRUE);
      static char**           GetFilterNames ();
      static char**           GetFilterExtensions ();
      static XBitmapFilter*   GetImportFilterStruct (const char* filename);
      static XBitmapFilter*   GetExportFilterStruct (const char* filename);
      static void             ReleaseFilterStruct (XBitmapFilter* param);
      static void             SetupImportFilter (const XWindow* owner, const char* filename, XBitmapFilter* params);
      static void             SetupExportFilter (const XWindow* owner, const char* filename, XBitmapFilter* params);

      void GetDimensions( XSize *) const;
      void SetOutputSize( const XRect *r) { width = r->GetWidth(); height = r->GetHeight(); }
      XBitmap operator =(const XBitmap& da);
      void Fill( XColor * color);

friend class ABitmap;
friend class XGLibFileDialog;
   private:
      static void  CheckFilterProfile ();
      static char* GetLastErrorMessage (XResourceLibrary*, ULONG*, XBitmapFilter*);
      static XBitmapFilterProfile* GetProfile ();
};

#endif
