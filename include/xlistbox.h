#ifndef __OOL_XLISTBOX_H__
#define __OOL_XLISTBOX_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XListBox                              */
/*                       derived from: XControl                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xcontrol.h"


#ifdef OOL_FOR_OS2_X86
   #define LS_NONE          -1
   #define LS_FIRST         -1
   #define LS_LAST          -1
   #define LS_ASCENDING     -2
   #define LS_DESCENDING    -3
   #define LS_PREFIX        0x0002
   #define LS_CASESENSITIVE 0x0004
   #define LS_SUBSTRING     0x0001
   #define LS_SCROLL        LS_HORZSCROLL
   #define LS_MULTI         LS_MULTIPLESEL
   #define LS_EXTENDED      LS_EXTENDEDSEL
#endif

class _export_ XListBox: public XControl
{
      friend void BuildChilds( HWND );
   protected:
      XListBox( const HWND hwnd): XControl(hwnd) {;}
      XListBox( const XRect * rec, const ULONG style, const XWindow * owner, const USHORT id, const PSZ className, const char* font=NULL);
   public:
      XListBox( const XWindow * owner, const XRect& rec= XRect(), const USHORT id = 0, const ULONG style = WIN_VISIBLE, const char* font=NULL);
      virtual ~XListBox() {;}
      SHORT InsertItem( const char * item, const SHORT pos = LS_LAST) const ;
      SHORT GetCount(void) const ;
      LONG GetItemHandle( const SHORT pos) const ;
      SHORT GetItemText( const SHORT pos, XString * buffer);
      SHORT GetSelection( const SHORT start = LS_FIRST) const ;
      SHORT GetTopIndex( void ) const ;
      BOOL RemoveAll(void) const ;
      void RemoveItem( const SHORT pos) const ;
      SHORT SearchString ( const char * p, const SHORT start=LS_FIRST, const SHORT matchCode=LS_CASESENSITIVE) const ;
      BOOL SelectItem( const SHORT pos, const BOOL select = TRUE) const ;
      BOOL SetItemHandle( const SHORT pos, const LONG handle) const ;
      void SetItemHeight( const SHORT height ) const ;
      BOOL SetItemText( const SHORT pos, const char * p) const ;
      void SetItemWidth( const SHORT width ) const ;
      void SetTopIndex( const SHORT index ) const ;
};

#endif
