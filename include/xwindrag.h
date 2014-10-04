#ifndef __OOL_XWINDOWDRAG_H__
#define __OOL_XWINDOWDRAG_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XWindowDrag                           */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"

#ifdef OOL_FOR_OS2_X86

   //image type and options
   #define IMG_ICON     DRG_ICON
   #define IMG_BITMAP   DRG_BITMAP
   #define IMG_STRETCH  DRG_STRETCH

#endif

class XWindow;
class XDragItem;
class XBitmap;
class XIcon;

class _export_ XWindowDrag: public XObject
{
   private:
      PDRAGINFO pInfo;
      SHORT count;
      DRAGIMAGE * pImage;
      SHORT imageCount;
      XWindow * ownerWindow;
   public:
      XWindowDrag( const XWindow * owner, const SHORT itemCount );
      ~XWindowDrag();
      void AddImage( const XIcon *, const SHORT style = 0, const SHORT xOffset=0, const SHORT yOffset=0, const SHORT stretch=0);
      void AddImage( const XBitmap *, const SHORT style = 0, const SHORT xOffset=0, const SHORT yOffset=0, const SHORT stretch=0);
      OOL_WINDOWHANDLE PerformDrag( void );
      void GetDragItem( XDragItem * item, const SHORT number);
      SHORT GetItemCount(void) { return pInfo->cditem; }
      LONG GetSupportedOperation( void ) { return pInfo->usOperation; }
      void SetDragItem( const XDragItem * item, const SHORT number);
};

#endif
