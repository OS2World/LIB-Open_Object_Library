#ifndef __OOL_XWINDOW_H__
#define __OOL_XWINDOW_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XWindow                               */
/*                       derived from: XObject                               */
/*                        last update: 3/98                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"
#include "XRect.h"
#include "XColor.h"
#include "XString.h"
#include "xHandler.h"
#include "XPoint.h"
#include "XCntrEvn.H"

class XFrameWindow;
class XSize;

#define OOL_ADDCLIENT  WM_USER + 9987

#ifdef OOL_FOR_OS2_X86
   #define WIN_VISIBLE  WS_VISIBLE
   #define WIN_TABSTOP  WS_TABSTOP
   #define WIN_BORDER   FS_BORDER
   #define WIN_ANIMATE  WS_ANIMATE
   #define WIN_NEXT     QW_NEXT
   #define WIN_PREV     QW_PREV
   #define WIN_TOP      QW_TOP
   #define WIN_BOTTOM   QW_BOTTOM
   #define WIN_OWNER      QW_OWNER
   #define WIN_PARENT   QW_PARENT
   #define WIN_NEXTTOP   QW_NEXTTOP
   #define WIN_PREVTOP   QW_PREVTOP
   #define WIN_FRAMEOWNER   QW_FRAMEOWNER

   #define PTR_ARROW    SPTR_ARROW
   #define PTR_TEXT     SPTR_TEXT
   #define PTR_WAIT     SPTR_WAIT
   #define PTR_SIZE     SPTR_SIZE
   #define PTR_SIZENWSE SPTR_SIZENWSE
   #define PTR_SIZENESW SPTR_SIZENESW
   #define PTR_SIZEWE   SPTR_SIZEWE
   #define PTR_SIZENS   SPTR_SIZENS
   #define PTR_MOVE     SPTR_MOVE
   #define PTR_ILLEGAL  SPTR_ILLEGAL

#endif

class _export_ XWindow: public XObject
{
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
      friend MRESULT HandleFrameDefault( XFrameWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
      friend class XHandler;
      friend class XFrameWindow;
      friend class XScrollWindow;
      friend class XFrame;
      friend class XNoteBook;
   private:
      XHandler ** regHandlers;
      USHORT handlers;
   protected:
      OOL_WINDOWHANDLE winhandle;
      XWindow( )
         {
            winhandle = 0;
            handlers = 0;
            regHandlers = NULL;
         }
   public:
      virtual ~XWindow();
      virtual void Activate( void ) const;
      virtual void Close() const;
      virtual BOOL DoCommand( LONG ) { return FALSE;}
      virtual void DoControl( XControlEvent *);
      virtual void DoMove( void ) { ;}
      virtual void DoSize( XSize* ) { ;}
      virtual BOOL DoSysCommand( USHORT syscom) { return TRUE;}
      virtual void Draw( void ) { ;}
      virtual void Enable( const BOOL enable = TRUE);
      virtual void EnableWindowUpdate ( const BOOL update = TRUE) const ;
      virtual void FocusChanged( BOOL ) { ;}
      virtual void GetBackgroundColor( XColor * rgbColor) const;
      virtual LONG GetClientHandle( void ) const { return winhandle; }
      void GetForegroundColor( XColor * rgbColor) const;
      virtual OOL_WINDOWHANDLE GetHandle(void) const {return winhandle;}
      virtual void GetSize( XRect* rect) const;
      static LONG GetSysValue( LONG valueID );
      virtual void GetText( XString * buffer ) const;
      virtual void GetUpdateRect( XRect*) const;
      virtual XWindow * XWindow :: GetWindow( const ULONG id );
      virtual SHORT GetWindowID( void ) const;
      virtual void Invalidate( BOOL invalidateChilds = FALSE, XRect * rect = NULL );
      virtual BOOL IsVisible( void ) const;
      virtual BOOL IsEnabled( void ) const;
      void GetFontName( XString *) const;
      static void GetPointerPos( XPoint * p);
      static OOL_POINTERHANDLE GetPointerHandle( void );
      virtual ULONG GetStyle(void) const;
      static OOL_POINTERHANDLE GetSystemPointerHandle( const LONG theID, const BOOL copy = FALSE );
      virtual LONG GetTextLength( void) const;
      virtual XWindow * QueryWindow( const ULONG id ) const;
      static void * SendMsg( const OOL_WINDOWHANDLE handle, const ULONG msg, void * mp1, void * mp2) { return WinSendMsg( handle, msg, mp1, mp2); }
      virtual BOOL SetBackgroundColor( const XColor * rgbColor);
      virtual void SetBottom( void ) const ;
      BOOL SetFocus( void ) const ;
      virtual BOOL SetForegroundColor( const XColor * rgbColor) const;
      BOOL SetFont(const char * fontName, const unsigned short fontSize) const;
      BOOL SetFont(const char * fontNameSize) const;
      virtual void SetOwner( const XWindow * w ) const;
      virtual void SetParent( const XWindow * w, const BOOL redraw = FALSE) const;
      static BOOL SetPointer( const OOL_POINTERHANDLE thePointer);
      virtual void SetSize( const XRect * rect) const;
      virtual void SetSize( const ULONG xPos, const ULONG yPos, const ULONG width, const ULONG height) const;
      virtual void SetStyle( const ULONG style);
      virtual void SetText(const char * text);
      virtual void SetTop( void ) const;
      virtual void Show( const BOOL show = TRUE);
};

#endif
