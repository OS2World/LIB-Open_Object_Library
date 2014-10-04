#ifndef __OOL_MDISERVER_H__
#define __OOL_MDISERVER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMDIServer                            */
/*                       derived from: XFrameWindow                          */
/*                        last update: 9/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xfrmwnd.h"

class XMDIClient;

class _export_ XMDIServer: public XFrameWindow
{     
      friend XMDIClient;
      void SetClientFocus( XMDIClient *);
      void AddWindow( XMDIClient*);
      void RemoveWindow(XMDIClient*);
      void UpdateMenuText( const int, const char*);
      XMDIClient ** windowList;
      XMDIClient * focus;
      SHORT counter;
      LONG ids;
      void Setup(void);
      BOOL max;
   public:
      XMDIServer( const XResource *, const char* s = "", const LONG style = XFrameWindow::defaultStyle | FRM_TASKLIST | FRM_CENTER | FRM_MENU, const XRect& rec=XRect());
      XMDIServer( const ULONG id, const char* s = "", const LONG style = XFrameWindow::defaultStyle | FRM_TASKLIST | FRM_CENTER | FRM_MENU, const XRect& rec=XRect());
      ~XMDIServer();
      void CascadeWindows();
      virtual BOOL DoCommand( LONG command);
//      virtual BOOL DoSysCommand( USHORT command);
      void DoSize( XSize * s);
      void Draw( void );
      USHORT GetClientCount() { return counter; }
//        XMDIClient * GetClientWindow(USHORT index) { return (index > 0 && index < counter ? windowList[index] : NULL); }
      void FocusChanged( BOOL set);
      XMDIClient * GetClientWindow(USHORT index) { return (index < counter ? windowList[index] : NULL); }
      XMDIClient * GetFocusWindow() { return focus; }
      void Init(void);
      void InitMenu( XMenu * menu);
      void NextWindow(void);
      void PrevWindow(void);
      BOOL QueryForClose();
      virtual void SetEmptyMenu( XMenuBar*) {;}
      void ShowClient( XMDIClient *, BOOL canSize = FALSE);
      void TileWindows();
};


#endif
