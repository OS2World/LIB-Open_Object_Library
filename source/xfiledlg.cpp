#include "xfiledlg.h"
#include "xbitmap.h"
#include "xreslib.h"
#include "xwindow.h"
#include "xglfldlg.h"

#include <stdlib.h>


/*@
@class XFileDialog
@parent XSystemDialog
@type overview
@symbol _
*/


/*@ XFileDialog :: XFileDialog(const XWindow * owner, const char *f, const char *title, const char *fileType, const LONG options, FNWP* dlgProc, ULONG ulUser)
@group constructors/destructors
@remarks Open the system defined file-dialog
@parameters <t 'ø' c=2>
            øXWindow * øowner-window
            øchar * øinital value for the filename entryfield (default is <*>)
            øchar * øtitle of the dialog (default is NULL)
            øchar * øfile-type to be displayed (default is NULL).
            øLONG øoptions, valid values are:
<P>FD_OPEN                   open-dialog
<P>FD_CENTER                 dialog is centered
<P>FD_SAVEAS                 saveas-dialog
<P>FD_MULTIPLESEL            multiple file-selection
<P>FD_HELP                   the dialog has a help-button. To display help the owner-window must be associated with a help-instance and in th help-file there must be an entry with an id of 267 (res=267)
<P>FD_MODELESS               show modeless
<P>FD_EAS                    use EAs
<P>FD_FILTERUNION            use filter from string-filter and EAs
<P>FD_HELPBUTTON             show a help-button
<P>default is FD_OPEN | FD_CENTER, can be or-ed
            øchar ** øList of filters for extended attributes (default is NULL). The list must contain a NULL, eg <PRE>char * list[] = {"C Code", "DIB", NULL};</PRE>
            øFNWP * øan user-defined dialog-procedure. Use it to implement your own file-filter or redefine the apply-button. See oolglib.c for an example.
            øULONG øoptional parameter for your own dialog-procedure
            </t>
*/

ULONG XFileDialog :: Start( const XWindow * owner, const char *f, const char *title, const char *fileType, const LONG options, char ** typeList, FNWP* dlgProc, ULONG ulUser )
{
   FILEDLG fild;

   memset(&fild, 0, sizeof(FILEDLG));
   fild.cbSize = sizeof(FILEDLG);
   fild.fl =  options;
   fild.pszTitle = (PSZ) title;
   fild.pszIType = (PSZ) fileType;
   fild.papszITypeList = (APSZ*) typeList;
   fild.pfnDlgProc = dlgProc;
   fild.ulUser = ulUser;
   if(f)
      strcpy(fild.szFullFile, f);
   WinFileDlg(HWND_DESKTOP, owner->GetHandle(), &fild);
   pap = fild.papszFQFilename;
   numbers = fild.ulFQFCount;
   command = fild.lReturn;
   fileName = fild.szFullFile;

   return fild.ulUser;
}


XFileDialog :: XFileDialog(const XWindow * owner, const char *f, const char *title, const char *fileType, const LONG options, char ** typeList, FNWP* dlgProc, ULONG ulUser )
{
   Start( owner, f, title, fileType, options, typeList, dlgProc, ulUser );
}


/*@ XFileDialog::GetFileName()
@group misc
@remarks If the file-dialog is proceeded you can ask for the
selected filename. If FD_MULTIPLESEL is specified, at first ask with GetFileCount()
for the number of files selected.
@parameters
<t c=2 'ø'>
øXString * øthe buffer which will contain the filename
øSHORT øzero-based index of filename to get (default is 0)
</t>
*/



XFileDialog :: ~XFileDialog()
{
   WinFreeFileDlgList(pap);
}


/*@
@class XGLibFileDialog
@parent XFileDialog
@type overview
@symbol _
*/

/*@ XGLibFileDialog :: XGLibFileDialog( const XWindow * owner, const char * filename, const char * title, const char * fileType, const LONG options)
@group constructors/destructors
@remarks Open a special file-dialog for bitmap-files supported by OOLGLIB.<P><I>If this method is used, GBM.DLL and OLLRES.DLL must be in the LIBPATH.</I>
@parameters <t 'ø' c=2>
            øXWindow * øowner-window
            øchar * øinital value for the filename entryfield (default is <*>)
            øchar * øtitle of the dialog (default is NULL)
            øchar * øfile-type to be displayed (default is NULL).
            øLONG øoptions, valid values are:
<P>FD_OPEN                   open-dialog
<P>FD_CENTER                 dialog is centered
<P>FD_SAVEAS                 saveas-dialog
<P>FD_MULTIPLESEL            multiple file-selection
<P>FD_HELP                   the dialog has a help-button. To display help the owner-window must be associated with a help-instance and in th help-file there must be an entry with an id of 267 (res=267)
<P>FD_MODELESS               show modeless
<P>FD_OPTIONSBUTTON          options-button for filter-specific parameters. Replaces FD_APPLYBUTTON.
<P>FD_EAS                    use EAs
<P>FD_FILTERUNION            use filter from string-filter and EAs
<P>FD_HELPBUTTON             show a help-button
<P>default is FD_OPEN | FD_CENTER, can be or-ed
            </t>
*/

XGLibFileDialog :: XGLibFileDialog( const XWindow * owner, const char * filename, const char* title, const char * fileType, const LONG options): XFileDialog ()
{
   pResLib  = new XResourceLibrary ("OOLGLIB");
   pDlgProc = pResLib->LoadFunction ("OOL_GLIB_FDLG_PROC");

   pFilter = (XBitmapFilter*) Start(owner, fileType, title, fileType, options,
      XBitmap :: GetFilterNames(), pDlgProc, 0L);

   pResLib->UnLoadFunction (pDlgProc);
   delete pResLib;
}

XGLibFileDialog :: ~XGLibFileDialog () {
   XBitmap :: ReleaseFilterStruct (pFilter);
}


/*@ XBitmap :: GetFilter ()
@group loading/saving a bitmap
@remarks Returns the bitmap-filter created by the dialog behind the options-button.
@returns XBitmapFilter *
*/

