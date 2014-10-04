#ifndef __OOL_XFRAMEWND_H__
#define __OOL_XFRAMEWND_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XFrameWindow                          */
/*                       derived from: XWindow                               */
/*                        last update: 6/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xwindow.h"
#include "XRect.h"
#include "XColor.h"
#include "XString.h"
#include "XHandler.h"
#include "XPoint.h"
#include "XCntrEvn.h"
#include "XRes.h"
#include "XScrlbar.h"
#include "XMenu.h"
#include "XApp.h"
#include "XIcon.h"

#ifdef OOL_FOR_OS2_X86
   #define FRM_TITLEBAR         0x00000001
   #define FRM_SYSMENU          0x00000002
   #define FRM_SIZEBORDER       0x00000008
   #define FRM_MINBUTTON        0x00000010
   #define FRM_MAXBUTTON        0x00000020
   #define FRM_CENTER           0x00080000
   #define FRM_DIALOGBORDER     0x00000100
   #define FRM_BORDER           0x00000200
   #define FRM_TASKLIST         0x00000800
   #define FRM_NOBYTEALIGN      0x00001000
   #define FRM_NOMOVEWITHOWNER  0x00002000
   #define FRM_ICON             0x00004000
   #define FRM_ACCELTABLE       0x00008000
   #define FRM_SYSMODAL         0x00010000
   #define FRM_SCREENALIGN      0x00020000
   #define FRM_MOUSEALIGN       0x00040000
   #define FRM_HIDEBUTTON       0x01000000
   #define FRM_HIDEMAX          0x01000020
   #define FRM_AUTOICON         0x40000000
   #define FRM_MENU             FCF_MENU
#endif


class _export_ XFrameWindow : public XWindow{
      friend class XMenuBar;
      friend class XPopupMenu;
      friend class XFrameControl;
      friend class XHelpInstance;
      friend class XObjectWindow;
      friend class XFrame;
      friend class XToolBar;
      friend class ToolBarParent;
      friend MRESULT EXPENTRY DProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
      friend MRESULT EXPENTRY WindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
      friend MRESULT HandleFrameDefault( XFrameWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
      friend MRESULT EXPENTRY ScrollProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
   protected:
      XScrollBar * vert, *horz;
   public:
      enum __WindowStyles__ {
                defaultStyle = FRM_TITLEBAR | FRM_SYSMENU | FRM_SIZEBORDER | FRM_MINBUTTON | FRM_MAXBUTTON | WIN_VISIBLE,
                defaultDialogStyle = FRM_TITLEBAR | FRM_SYSMENU | FRM_DIALOGBORDER | WIN_VISIBLE,
                defaultClientStyle = FRM_BORDER | WIN_VISIBLE
      };
   protected:
      HSWITCH sEntry;
      OOL_WINDOWHANDLE dummy;
      XFrame * frameWin;
      XWindow * clientWindow;
      XFrameWindow * lockingWin;
      OOL_WINDOWHANDLE frame, dlgHandle;
      BOOL enabled;
      PFNWP oldfunc;
      LONG backCol;
      void CreateWindow( const ULONG id, const XResourceLibrary * lib,
                     const char * title =NULL,
                     const ULONG createstyle = defaultStyle,
                     const XRect * rec = NULL,
                     const XFrameWindow * parent=NULL,
                     const BOOL buildFromResources = FALSE,
                     const BOOL animate = FALSE);
   public:
      XFrameWindow( const XResource * resource,
                     const char * title =NULL,
                     const ULONG createstyle = defaultStyle,
                     const XRect& rec = XRect(),
                     const XFrameWindow * parent=NULL,
                     const BOOL buildFromResources = FALSE,
                     const BOOL animate = FALSE);
      XFrameWindow( const ULONG id,
                     const char * title =NULL,
                     const ULONG createstyle = defaultStyle,
                     const XRect& rec = XRect(),
                     const XFrameWindow * parent=NULL,
                     const BOOL buildFromResources = FALSE,
                     const BOOL animate = FALSE);
      virtual ~XFrameWindow();
      void Activate( void ) const;
      virtual void DoControl( XControlEvent*) { ;}
      virtual void Draw ( void ) { ;}
      void Enable( const BOOL enable = TRUE);
      void EnableWindowUpdate ( const BOOL enable = TRUE ) const ;
      void FillBackground( void );
      virtual void GetBackgroundColor( XColor * rgbColor) const;
      void GetClientSize( XRect *) const ;
      OOL_WINDOWHANDLE GetHandle() const;
      void GetSize( XRect* rect) const;
      void GetText( XString * buffer ) const;
      XWindow * GetWindow( const ULONG id);
      virtual void InitMenu( XMenu *) {;}
      virtual BOOL QueryForClose( void ) { return TRUE; }
      XMenuBar * GetMenu( void ) const ;
      virtual BOOL SetBackgroundColor( const XColor*);
      void SetBottom(void) const ;
      BOOL SetClient( const XWindow * c);
      void SetIcon( const XIcon *);
      void SetSize( const XRect * rect) const;
      void SetSize( const ULONG xPos, const ULONG yPos, const ULONG width, const ULONG height) const;
      void SetText(const char * buffer);
      void SetTop(void) const;
      void Show( const BOOL show=TRUE);
      void ShowModal( XFrameWindow *  );
      void Minimize() const;
      void Maximize() const;
      void Restore() const;
};


#endif
