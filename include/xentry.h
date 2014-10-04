#ifndef __OOL_XENTRYFIELD_H__
#define __OOL_XENTRYFIELD_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XEntryField                           */
/*                       derived from: XControl                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/




#include "xcontrol.h"

#ifdef OOL_FOR_OS2_X86
   #define EN_LEFT       ES_LEFT
   #define EN_CENTER     ES_CENTER
   #define EN_RIGHT      ES_RIGHT
   #define EN_AUTOSCROLL ES_AUTOSCROLL
   #define EN_MARGIN     ES_MARGIN
   #define EN_AUTOTAB    ES_AUTOTAB
   #define EN_READONLY   ES_READONLY
   #define EN_UNREADABLE ES_UNREADABLE
   #define EN_AUTOSIZE   ES_AUTOSIZE
   #define EN_BORDER     ES_MARGIN
#endif

class _export_ XEntryField: public XControl{
      friend void BuildChilds( HWND );
   private:
      XEntryField( const HWND hwnd): XControl(hwnd) {;}
   public:
      XEntryField( const XWindow * owner, const XRect& rec= XRect(), const USHORT id = 0, const ULONG style = EN_BORDER | EN_LEFT, const char * string = "", const char*font=NULL);
      virtual ~XEntryField() {;}
      void Clear( void ) const ;
      void Copy( void ) const ;
      void Cut( void ) const ;
      void EnableOverWrite( const BOOL overWrite = TRUE) const ;
      BOOL IsChanged(void ) const ;
      void Paste( void ) const ;
      SHORT GetFirstChar( void ) const ;
      BOOL IsReadOnly( void ) const ;
      void GetSelection( SHORT& start, SHORT& end);
      void SelectText( const USHORT start, const USHORT end) const ;
      void SetLimit( const USHORT len) const ;
      void SetReadOnly( const BOOL readOnly = TRUE) const ;
      void ShowFirstChar( const SHORT firstChar = 0) const ;
};

#endif
