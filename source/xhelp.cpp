#include "XHelp.h"
#include "XFrmwnd.H"
#include "XResLib.h"
#include "XRes.h"
#include "XApp.h"
#include "xdialog.h"

#include <string.h>

/*@ 
@class XHelpInstance
@type overview
@symbol _
@remarks XHelpInstance handles help functions for you. Therefore you have
to write your help-text for your controls and give the text the same id
(res=XXX in the IPF-file) like you have defined for the requested control.<BR>
You can associate a XHelpInstance to one ore more framewindows.
<p>The help-instance also supports help for a message-box, refer to XMessageBox for details.
*/


BOOL HelpHook(HAB hab, ULONG mode, ULONG idT, ULONG idS, PRECTL rect)
{
	if (mode)					// == HLPM_MESSAGE)
	{
		HWND hwnd = WinQueryFocus(HWND_DESKTOP);

		hwnd = WinQueryWindow(hwnd, QW_OWNER);
		if (idT == 0)
			idT = WinQueryWindowUShort(hwnd, QWS_ID);
		if (hwnd)
		{
			WinSendMsg(WinQueryHelpInstance(hwnd), HM_DISPLAY_HELP, MPFROMSHORT(idT), MPFROMSHORT(HM_RESOURCEID));
			return TRUE;
		}
	}
	return FALSE;
}


/*@ XHelpInstance :: XHelpInstance(const char *path, const char *title, const XResource * helpTable, const XResourceLibrary * actionBarLib)
@group constructors/destructors
@remarks Construct a helpinstance
@parameters    <t 'ø' c=2>
					øchar * path                        øpath for the help-file
               øchar * title                       øwindow -title (default is NULL)
               øXResource * helpTableLib           øresource for the help-table (default is NULL)
               øXResourceLibrary * actionBarLib    ølibrary for action-bar (defaul tis NULL)
					</t>
*/
XHelpInstance :: XHelpInstance(const char *path, const char *title, const XResource * helpTable, const XResourceLibrary * actionBarLib)
{
	HELPINIT helpinit;

	memset(&helpinit, 0, sizeof(helpinit));

	helpinit.cb = sizeof(HELPINIT);
	helpinit.ulReturnCode = 0;
	helpinit.pszTutorialName = NULL;
	if (helpTable)
		helpinit.phtHelpTable = (PHELPTABLE) MPFROM2SHORT(helpTable->GetID(), 0xFFFF);

	helpinit.idAccelTable = 0;
	helpinit.idActionBar = 0;

	helpinit.pszHelpWindowTitle = (PSZ) title;
	helpinit.fShowPanelId = CMIC_HIDE_PANEL_ID;
	helpinit.fShowPanelId = 0;
	helpinit.pszHelpLibraryName = (PSZ) path;
	if (actionBarLib)
		helpinit.hmodAccelActionBarModule = actionBarLib->GetModuleHandle();
	if (helpTable)
		if (helpTable->GetResourceLibrary())
			helpinit.hmodHelpTableModule = helpTable->GetResourceLibrary()->GetModuleHandle();
	helpInstance = WinCreateHelpInstance(XApplication::GetApplication()->GetAnchorBlock(), &helpinit);
	WinSetHook( XApplication::GetApplication()->GetAnchorBlock(), HMQ_CURRENT, HK_HELP, (PFN) HelpHook, NULLHANDLE);
}


/*@ XHelpInstance::ShowHelpForHelp(void)
@group help funcions
@remarks Shows how to use help
*/
void XHelpInstance::ShowHelpForHelp(void) const
{
	WinSendMsg(helpInstance, HM_DISPLAY_HELP, NULL, NULL);
}



/*@ XHelpInstance::ShowHelpContents(void)
@group help funcions
@remarks Show the contents
*/
void XHelpInstance::ShowHelpContents(void) const
{
	WinSendMsg(helpInstance, HM_HELP_CONTENTS, NULL, NULL);
}


/*@ XHelpInstance::ShowHelpIndex(void)
@group help funcions
@remarks Shows the index
*/
void XHelpInstance::ShowHelpIndex(void) const
{
	WinSendMsg(helpInstance, HM_HELP_INDEX, NULL, NULL);
}


/*@ XHelpInstance::ShowHelpForId(const LONG id)
@group help funcions
@remarks Shows the help for a given ID
@parameters LONG id     the requested id
*/
void XHelpInstance::ShowHelpForId(const LONG id) const
{
	WinSendMsg(helpInstance, HM_DISPLAY_HELP, MPFROMSHORT(id), MPFROMSHORT(HM_RESOURCEID));
}


XHelpInstance :: ~XHelpInstance()
{
	if (helpInstance)
		WinDestroyHelpInstance(helpInstance);
}


/*@ XHelpInstance::AssociateWindow(const XFrameWindow * fWin)
@group window functions
@remarks Associate a frame-window with the help-instance
@parameters XFrameWindow * window
*/
void XHelpInstance::AssociateWindow(const XFrameWindow * fWin) const
{
	WinAssociateHelpInstance(helpInstance, fWin->frame);
}


/*@ XHelpInstance::AssociateWindow(const XDialog * fWin)
@group window functions
@remarks Associate a dialog with the help-instance
@parameters XDialog * dialog
*/
void XHelpInstance::AssociateWindow(const XDialog * dlg) const
{
	WinAssociateHelpInstance(helpInstance, dlg->GetHandle());
}
