#ifndef __OOL_XCOMBO_H__
#define __OOL_XCOMBO_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XComboBox                             */
/*                       derived from: XListBox                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/




#include "xlistbox.h"

#ifdef OOL_FOR_OS2_X86

   #define CB_SIMPLE        CBS_SIMPLE
   #define CB_DROPDOWN      0x0002
   #define CB_DROPDOWNLIST  0x0004

#endif

class _export_ XComboBox: public XListBox{
      friend void BuildChilds( HWND );
   private:
      XComboBox( const HWND hwnd): XListBox(hwnd) {;}
   public:
      XComboBox( const XWindow * owner, const XRect& rec = XRect(), const USHORT id = 0, const ULONG style = CB_DROPDOWN | WIN_VISIBLE, const char*font=NULL );
      virtual ~XComboBox() { ;}
      void EnableOverWrite( const BOOL overWrite=TRUE) const ;
      BOOL HasChanged(void) const ;
      void Hilite( const BOOL hilite = TRUE) const ;
      BOOL IsListShowing( void ) const ;
      SHORT GetFirstChar( void ) const ;
      BOOL IsReadOnly( void ) const ;
      void GetSel( SHORT& start, SHORT& end);
      void SetLimit( const USHORT len) const ;
      void SetReadOnly( const BOOL readOnly = TRUE) const ;
      void ShowList( const BOOL show = TRUE) const ;
};


#endif
