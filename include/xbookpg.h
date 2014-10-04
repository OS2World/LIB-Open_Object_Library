#ifndef __OOL_BOOKPAGE_H__
#define __OOL_BOOKPAGE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XBookPage                             */
/*                       derived from: XWindow                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include  "xnotebk.h"

class XColor;
class XBitmap;
class XResource;
class XResourceLibrary;

#ifdef OOL_FOR_OS2_X86
   #define BP_LAST         0x0002
   #define BP_FIRST         0x0004
   #define BP_NEXT         0x0008
   #define BP_PREV         0x0010
   #define BP_TOP            0x0020

   #define BP_MAJORTAB      0x0001
   #define BP_MINORTAB      0x0002
   #define BP_PAGEBUTTON   0x0100

   #define BP_STATUSTEXTON   0x0001
   #define BP_MAJOR         0x0040
   #define BP_MINOR         0x0080
#endif


class _export_ XNoteBookPage: public XWindow
{
   protected:
      XNoteBook * owner;
      ULONG id;  
   private:
      void CreateBookPage( const XNoteBook*, const ULONG pageID, const USHORT style, const USHORT order, const char * title, const XNoteBookPage * insertBehind, const XResourceLibrary * res, BOOL build);
   public:
      XNoteBookPage( const XNoteBook*, const ULONG pageID, const USHORT style = BP_MAJOR|BP_STATUSTEXTON, const USHORT order = BKA_LAST, const char * title = NULL, const XNoteBookPage * insertBehind=NULL, BOOL build = FALSE);
      XNoteBookPage( const XNoteBook*, const XResource * res, const USHORT style = BP_MAJOR|BP_STATUSTEXTON, const USHORT order = BKA_LAST, const char * title = NULL, const XNoteBookPage * insertBehind=NULL);
      ~XNoteBookPage();
      void GetBackgroundColor ( XColor * col ) const;
      ULONG GetID(void) const { return id; }
      SHORT GetPageCount( void )const ;
      void SetBitmap( const XBitmap * b);
      void SetStatusText( const char * text) const ;
      void SetText( const char * text);
      void SetTop( void ) const;
};


#endif
