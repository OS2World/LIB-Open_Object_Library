#include "xfontdlg.h"
#include <stdlib.h>

/*@
@class XFontDialog
@parent XSystemDialog
@type overview
@symbol _
@remarks Class for the system defined font dialog
*/


/*@ XFontDialog :: XFontDialog(const XWindow * owner, const char *fontName, const SHORT fontSize, const LONG style, const LONG attr, const char *title, const char *preView)
@group constructors/destructors
@remarks Open the stystem-defined dialog to select a font.
@parameters  <t 'ø' c=2>
            øXWindow * owner   øowner of the dialog
            øchar * fontName   ødefault font name
            øSHORT fontSize    ødefault font size
            øLONG style        østyle of the font, valid values are:

            øLONG atrributes   ødialog attributes, valid values are:
<P>
FDLG_CENTER
<P>
FDLG_HELPBUTTON
<P>
FDLG_APPLYBUTTON
<P>
FDLG_RESETBUTTON
<P>
FDLG_MODELESS
<P>
FDLG_INITFROMFATTRS
<P>
FDLG_BITMAPONLY
<P>
FDLG_VECTORONLY
<P>
FDLG_FIXEDWIDTHONLY
<P>
FDLG_PROPORTIONALONLY
<P>
FDLG_NOSYNTHESIZEDFONTS
<P>
FDLG_NATIONAL_LANGUAGE
            øchar * preview    øtext for the preview-window of the dialog
            </t>
*/
XFontDialog :: XFontDialog(const XWindow * owner, const char *fontName, const SHORT fontSize, const LONG style, const LONG attr, const char *title, const char *preView)
{
   memset(&fdlg, 0, sizeof(fdlg));
   fdlg.cbSize = sizeof(fdlg);
   fdlg.pszFamilyname = (PSZ) malloc(FACESIZE);
   fdlg.usFamilyBufLen = 100;
   fdlg.hpsScreen = WinGetPS(owner->GetHandle());
   fdlg.pszTitle = (PSZ) title;
   fdlg.pszPreview = (PSZ) preView;
   fdlg.fl = attr;
   fdlg.clrFore = CLR_BLACK;
   fdlg.clrBack = CLR_WHITE;
   fdlg.fAttrs.usCodePage = 437;
   strcpy((char *) fdlg.pszFamilyname, fontName);
   fdlg.fxPointSize = MAKEFIXED(fontSize, 0);
   fdlg.flStyle = style;
   WinFontDlg(HWND_DESKTOP, owner->GetHandle(), &fdlg);
   command = fdlg.lReturn;
}


/*@ XFontDialog::GetFontName(XString * buffer)
@group misc
@remarks Query the name of the selected font.
@parameters XString * buffer to hold the data
*/
void XFontDialog::GetFontName(XString * buffer) const
{
   SHORT l = strlen((char *) fdlg.fAttrs.szFacename) + 1;

   memcpy(buffer->GetBuffer(l), fdlg.fAttrs.szFacename, l);
   buffer->ReleaseBuffer(l - 1);
}


/*@ XFontDialog::GetFontSize()
@group misc
@remarks Query the selected font-size
@returns SHORT size
*/

XFontDialog :: ~XFontDialog()
{
   free(fdlg.pszFamilyname);
}


/*@
@class XSystemDialog
@parent XObject
@type overview
@symbol _
@remarks XSystemDialog is a basic class for sytem defined dialogs like XFileDialog and XFontDialog
*/

/*@ XSystemDialog::GetCommand()
@group misc
@remarks Return the users action.
@returns SHORT ID of the users action:
<t 'ø' c=2>
øUSER_OK    øuser selected ok
øUSER_CANCEL   øuser canceled
</t>
*/

