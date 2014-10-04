#include "xmodlg.h"
#include "xmodal.h"
#include "xres.h"
#include "xreslib.h"
#include "xexcept.h"
#include "xapp.h"

extern LONG MAXX;
extern LONG MAXY;
extern SHORT cxDlgFrame;
extern SHORT cyDlgFrame;
extern SHORT cxBorder;
extern SHORT cyBorder;
extern SHORT cxSizeBorder;
extern SHORT cySizeBorder;

void GetScreenValues();
void BuildChilds(HWND dlgHandle);
MRESULT HandleDefault(XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL & handled);


void SetCentered( HWND winhandle)
{
     if (MAXX == 0)
      GetScreenValues();

   SWP swp;
   WinQueryWindowPos(winhandle, &swp);
   WinSetWindowPos( winhandle, HWND_TOP, MAXX / 2 - swp.cx / 2, MAXY / 2 - swp.cy / 2, swp.cx, swp.cy, SWP_MOVE);
}


MRESULT EXPENTRY ModelessDProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   XModelessDialog *w = (XModelessDialog *) WinQueryWindowPtr(hwnd, 0);

   if (w)
   {
      if(msg == WM_CLOSE )
      {
         WinSetWindowULong(hwnd, 0, 0);
         delete w;
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
      }
      else if(msg == WM_COMMAND )
      {
         if( w->DoCommand( SHORT1FROMMP(mp1) ) == TRUE)
         {
            WinSetWindowULong(hwnd, 0, 0);
            delete w;
            return WinDefDlgProc(hwnd, msg, mp1, mp2);
         }
         else
            return (MRESULT) FALSE;
      }
      else if(msg == WM_SYSCOMMAND)
      {
         if( w->DoSysCommand( SHORT1FROMMP(mp1)) == TRUE)
            return WinDefDlgProc(hwnd, msg, mp1, mp2);
         return 0;
      }
      else if(msg == WM_MOVE)
      {
         MRESULT mr = WinDefDlgProc(hwnd, msg, mp1, mp2);
         w->DoMove();
         return mr;
      }
      else if(msg == WM_ADJUSTWINDOWPOS )
//      else if(msg == WM_WINDOWPOSCHANGED )
      {
//         XProcess::Beep(200,200);
         MRESULT mr = WinDefDlgProc(hwnd, msg, mp1, mp2);
         PSWP pswp = (PSWP) mp1;
         XSize size( pswp->cx, pswp->cy);
         w->DoSize(&size);
         return mr;
      } /* end if */

      BOOL handeld = FALSE;
      MRESULT mr = HandleDefault(w, msg, mp1, mp2, handeld);

      if (handeld)
         return mr;
   }
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


XDialog :: ~XDialog()
{
   HENUM enumWindow = WinBeginEnumWindows(winhandle);
   HWND hwnd;

   while ((hwnd = WinGetNextWindow(enumWindow)) != NULLHANDLE)
   {

      XWindow *w = (XWindow *) WinQueryWindowPtr(hwnd, 0);

      if (w && w != this)
      {
         delete w;
      }
   }
   WinEndEnumWindows(enumWindow);

   XApplication::GetApplication()->RemoveWindow(this);
}


/*@
@class XDialog
@parent XWindow
@type overview
@symbol _
*/


/*@
@class XModelessDialog
@parent XDialog
@type overview
@symbol _
*/


/*@ XModelessDialog :: XModelessDialog( const XResource * id, const XWindow * owner, const BOOL center)
@group constructors/destructors
@remarks Construct a modeless dialog.
@parameters
<t 'ø' c=2>
øXResource *   øa resource which describes the dialog template
øXWindow *       øa pointer to the owner-window (can be NULL)
øBOOL            øTRUE=show centered<BR>FALSE=not centered<BR>(default is TRUE)
</t>
*/
XModelessDialog :: XModelessDialog( const XResource * id, const XWindow * owner, const BOOL center)
{
   CreateDialog( id->GetID(), id->GetResourceLibrary(), owner, center);
}


/*@ XModelessDialog :: XModelessDialog( const ULONG id, const XWindow * owner, const BOOL center)
@group constructors/destructors
@remarks Construct a modeless dialog.
@parameters
<t 'ø' c=2>
øULONG         øID of the dialog template, the dialog-resources mus be linked to the exe-file
øXWindow *       øa pointer to the owner-window (can be NULL)
øBOOL            øTRUE=show centered<BR>FALSE=not centered<BR>(default is TRUE)
</t>
*/
XModelessDialog :: XModelessDialog( const ULONG id, const XWindow * owner, const BOOL center)
{
   CreateDialog( id, NULL, owner, center);
}


void XModelessDialog :: CreateDialog( const ULONG id, const XResourceLibrary * lib = NULL, const XWindow * owner, const BOOL center)
{
   HWND hwnd = HWND_DESKTOP;
   if(owner)
      hwnd = owner->GetHandle();
   if(lib)
      winhandle = WinLoadDlg( HWND_DESKTOP, hwnd, (PFNWP) ModelessDProc, lib->GetModuleHandle(), id, NULL);
   else
      winhandle = WinLoadDlg( HWND_DESKTOP, hwnd, (PFNWP) ModelessDProc, XApplication::GetApplication()->GetResourceLibrary()->GetModuleHandle(), id, NULL);

   if (winhandle == 0)
   {
      OOLThrow("could not create dialog from resources!", 0);
      return;
   }
   WinSetWindowPtr(winhandle, 0, this);

   if(center)
      SetCentered( winhandle );

   BuildChilds(winhandle);
   XApplication::GetApplication()->AddWindow(this);
}


MRESULT EXPENTRY ModalDProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   XModalDialog *w = (XModalDialog *) WinQueryWindowPtr(hwnd, 0);

   if (w)
   {
       if(msg == WM_CLOSE )
      {
         WinSetWindowULong(hwnd, 0, 0);
         WinDismissDlg( w->winhandle, -1 );
         delete w;
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
      }
      if(msg == WM_COMMAND )
      {
         if( w->DoCommand( SHORT1FROMMP(mp1) ) == TRUE)
         {
            WinSetWindowULong(hwnd, 0, 0);
            WinDismissDlg( w->winhandle, SHORT1FROMMP(mp1) );
            delete w;
            return WinDefDlgProc(hwnd, msg, mp1, mp2);
         }
         else
            return (MRESULT) FALSE;
      }
      else if(msg == WM_SYSCOMMAND)
      {
         if( w->DoSysCommand( SHORT1FROMMP(mp1)) == TRUE)
            return WinDefDlgProc(hwnd, msg, mp1, mp2);
         return 0;
      }
      else if(msg == WM_MOVE)
      {
         MRESULT mr = WinDefDlgProc(hwnd, msg, mp1, mp2);
         w->DoMove();
         return mr;
      }
      else if(msg == WM_ADJUSTWINDOWPOS )
      {
         MRESULT mr = WinDefDlgProc(hwnd, msg, mp1, mp2);
         PSWP pswp = (PSWP) mp1;
         XSize size( pswp->cx, pswp->cy);
         w->DoSize(&size);
         return mr;
      } /* end if */

      BOOL handeld = FALSE;
      MRESULT mr = HandleDefault(w, msg, mp1, mp2, handeld);

      if (handeld)
         return (MRESULT) mr;
   }
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


/*@
@class XModalDialog
@parent XDialog
@type overview
@symbol _
*/

/*@ XModalDialog :: XModalDialog( const XResource * id, const XWindow * owner, const BOOL center)
@group constructors/destructors
@remarks Construct a modal dialog. You must call XModalDialog::Start() after
constructing an instance to make the dialog work. When constructing the dialog you can initialize
the dialog controls.
@parameters
<t 'ø' c=2>
øXResource *       øA resource which describes the dialog template
øXWindow *          øA pointer to the owner-window. If now owner is given an error occures.
øBOOL            øTRUE=show centered<BR>FALSE=not centered<BR>(default is TRUE)
</t>
*/
XModalDialog :: XModalDialog( const XResource * id, const XWindow * owner, const BOOL center)
{
   CreateDialog( id->GetID(), id->GetResourceLibrary(), owner, center);
}


/*@ XModalDialog :: XModalDialog( const ULONG id, const XWindow * owner, const BOOL center)
@group constructors/destructors
@remarks Construct a modal dialog. You must call XModalDialog::Start() after
constructing an instance to make the dialog work. When constructing the dialog you can initialize
the dialog controls.
@parameters
<t 'ø' c=2>
øULONG          øID of the dialog template. The dialog-resources must be linked to the exe-file.
øXWindow *          øA pointer to the owner-window. If now owner is given an error occures.
øBOOL            øTRUE=show centered<BR>FALSE=not centered<BR>(default is TRUE)
</t>
*/
XModalDialog :: XModalDialog( const ULONG id, const XWindow * owner, const BOOL center)
{
   CreateDialog( id, NULL, owner, center);
}


void XModalDialog :: CreateDialog( const ULONG id, const XResourceLibrary * lib, const XWindow * owner, BOOL center)
{
   HWND hwnd = HWND_DESKTOP;

   if(owner)
      hwnd = owner->GetHandle();
   else
      OOLThrow("XModalDialog need a parent window!", 0);

   if( lib )
      winhandle = WinLoadDlg( HWND_DESKTOP, hwnd, (PFNWP) ModalDProc, lib->GetModuleHandle(), id, NULL);
   else
      winhandle = WinLoadDlg( HWND_DESKTOP, hwnd, (PFNWP) ModalDProc, XApplication::GetApplication()->GetResourceLibrary()->GetModuleHandle(), id, NULL);

   if (winhandle == 0)
   {
      OOLThrow("could not create dialog from resources!", 0);
      return;
   }
   WinSetWindowPtr(winhandle, 0, this);

   if( center )
      SetCentered(winhandle);

   BuildChilds(winhandle);

   XApplication::GetApplication()->AddWindow(this);
}



/*@ XModalDialog :: Start( void )
@group misc
@remarks To make a modal dialog work you must call Start(). this functions returns the value
of the last command.
@returns LONG command-value, -1 if the dialog was closed with the system-menu
@example
XModalDialog * dlg = new XModalDialog(...)
int command = dlg->Start();
*/
LONG XModalDialog :: Start( void )
{
   return WinProcessDlg( winhandle);
}


//Docs only
/*@
@class XDialog
@type overview
@symbol _
@remarks XDialog is the base class for XModalDialog and XModelessDialog.
You cannot construct an instance of XDialog directly, you have to chose
one of the child-classes.
*/


/*@ XDialog::DoCommand(LONG)
@remarks If a command was received by a dialog, eg. from a button, you must
decide if the dialog can be destroyed or not. Override the XDialog::DoCommand() function,
return TRUE if the dialog can be destroyed, otherwise return FALSE.
@new
@parameters LONG command
@returns BOOL TRUE=destroy, FALSE=do not destroy
*/
