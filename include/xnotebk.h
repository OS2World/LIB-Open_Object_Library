#ifndef __OOL_XNOTEBOOK_H__
#define __OOL_XNOTEBOOK_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XNoteBook                             */
/*                       derived from: XControl                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include  "xcontrol.h"

#ifdef OOL_FOR_OS2_X86
   #define NB_BACKPAGESBR         0x0001
   #define NB_BACKPAGESBL         0x0002
   #define NB_BACKPAGESTR         0x0004
   #define NB_BACKPAGESTL         0x0008
   #define NB_MAJORTABRIGHT      0x0010
   #define NB_MAJORTABLEFT          0x0020
   #define NB_MAJORTABTOP         0x0040
   #define NB_MAJORTABBOTTOM      0x0080
   #define NB_SQUARETABS         0x0000
   #define NB_ROUNDEDTABS         0x0100
   #define NB_POLYGONTABS         0x0200
   #define NB_SOLIDBIND            0x0000
   #define NB_SPIRALBIND         0x0400
   #define NB_STATUSTEXTLEFT      0x0000
   #define NB_STATUSTEXTRIGHT      0x1000
   #define NB_STATUSTEXTCENTER   0x2000
   #define NB_TABTEXTLEFT         0x0000
   #define NB_TABTEXTRIGHT         0x4000
   #define NB_TABTEXTCENTER      0x8000
   #define NB_TABBEDDIALOG         0x800
#endif

class XNoteBookPage;

class _export_ XNoteBook: public XControl{
      friend void BuildChilds( HWND );
      friend class XBookPage;
      LONG color;
   protected:
      XNoteBook( const HWND hwnd):XControl(hwnd) {;}
   public:
      XNoteBook( const XWindow * owner, const XRect& rec=XRect(), const USHORT id = 0, const ULONG style = NB_SOLIDBIND|NB_BACKPAGESBR|NB_SQUARETABS|NB_TABTEXTCENTER|NB_STATUSTEXTLEFT, char * font = NULL);
      virtual ~XNoteBook();
      void CalcClientRect(XRect * out, BOOL client);
      void GetBackgroundColor( XColor * col ) const { col->SetColor( color ); }
      SHORT GetPageCount( void ) const ;
      XNoteBookPage * GetPage( USHORT order = BKA_TOP, ULONG id = 0);
      BOOL RemovePage( XNoteBookPage*);
      BOOL SetBackgroundColor( const XColor * col );
      void SetMajorTabSize( const SHORT width, const SHORT height) const ;
      void SetMinorTabSize( const SHORT width, const SHORT height) const ;
      void SetMajorBackgroundColor( const XColor * col ) const ;
      void SetMajorForegroundColor( const XColor * col ) const ;
      void SetMinorBackgroundColor( const XColor * col ) const ;
      void SetMinorForegroundColor( const XColor * col ) const ;
};

#endif
