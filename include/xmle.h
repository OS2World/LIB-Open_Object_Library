#ifndef __OOL_XMLE_H__
#define __OOL_XMLE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMultiLineEdit                        */
/*                       derived from: XControl                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xcontrol.h"

#ifdef OOL_FOR_OS2_X86
   #define MLE_BORDER      0x0002
   #define MLE_READONLY    0x0010
   #define MLE_WORDWRAP    0x0001
   #define MLE_HORZSCROLL  0x0008
   #define MLE_VERTSCROLL  0x0004
   #define MLE_IGNORETAB   0x0020
   #define MLE_DISABLEUNDO 0x0040
#endif

class XMLESearch;
class XString;

class _export_ XMultiLineEdit: public XControl{
      friend void BuildChilds( HWND );
   private:
      XMultiLineEdit( const HWND hwnd): XControl(hwnd) {;}
   public:
      XMultiLineEdit( const XWindow * owner, const XRect& rec=XRect(), const USHORT id = 0, const ULONG style=MLE_BORDER|MLE_WORDWRAP|WIN_VISIBLE, const char * title = "", const char*font=NULL);
      virtual ~XMultiLineEdit() {;}
      BOOL CanUndo( void ) const ;
      void Clear( void ) const ;
      void Copy( void ) const ;
      void Cut( void ) const ;
      void EnableOverWrite( const BOOL overWrite = TRUE) const ;
      void EnableRefresh( const BOOL refresh = TRUE) const ;
      void EnableWrap( const BOOL wrap = TRUE) const ;
      virtual void GetText( XString * buffer ) const;
      BOOL IsChanged( void ) const ;
      void InsertString( const char * p) const ;
      void Paste( void ) const ;
      LONG GetFirstChar( void ) const ;
      LONG GetLineCount( void ) const ;
      LONG GetLineFromChar( const LONG point) const ;
      LONG GetLineLength( const LONG point ) const ;
      LONG GetLineStart( const LONG line ) const ;
      BOOL IsReadOnly( void ) const ;
      void GetSelection(LONG& start, LONG& end);
      LONG GetTextLength( void) const;
      BOOL IsWrap( void ) const ;
      void ResetUndo( void ) const ;
      BOOL Search( const XMLESearch * s) const ;
      void SelectText( const LONG start, const LONG end) const ;
      void SetLimit( const LONG len) const ;
      void SetReadOnly( const BOOL readOnly = TRUE) const ;
      virtual void SetText(const char * text);
      void ShowFirstChar( const LONG firstChar = 0) const ;
      BOOL Undo( void ) const ;
};


#endif
