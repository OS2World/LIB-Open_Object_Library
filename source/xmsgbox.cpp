#include "XMsgBox.h"
#include "XxtMsgBx.h"
#include "XString.h"
#include "XResLib.h"
#include "XIcon.h"

#include <stdlib.h>


LONG _export_ XShowMessage( const char * s, const char * s2, const ULONG style, const XWindow * owner)
{
   XMessageBox mbox(s, s2, style, owner);
   return mbox.GetCommand();
}


LONG _export_ XShowMessage( const char * s, const ULONG style, const XWindow * owner)
{
   XMessageBox mbox(s, NULL, style, owner);
   return mbox.GetCommand();
}

/*@
@class XMessageBox
@type overview
@symbol _
*/

/*@ XMessageBox::GetCommand()
@group misc
@remarks Returns the ID of the users action.
@returns USHORT id    The ID of the button the user pressed:
                        <t 'ø' c=1>
                        øMDID_OK
                        øMDID_CANCEL
                        øMDID_ABORT
                        øMDID_RETRY
                        øMDID_IGNORE
                        øMDID_YES
                        øMDID_NO
                        øMDID_HELP
                        øMDID_ENTER
                        øMDID_ERROR
                     </t>
*/


/*@ XMessageBox :: XMessageBox(const char *s1, const char *s2, const ULONG style, const XWindow * owner, const ULONG id)
@group constructors/destructors
@remarks Construct a message box.
@parameters <t 'ø' c=2>
            øchar * content     øthe content of the message box
           øchar * title       øthe title of the message box
           øULONG style        øthe style, possible values are:
                                 <t 'ø' c=1>
                                    øMBOX_OK
                                    øMBOX_OKCANCEL
                                    øMBOX_RETRYCANCEL
                                    øMBOX_ABORTRETRYIGNORE
                                    øMBOX_YESNO
                                    øMBOX_YESNOCANCEL
                                    øMBOX_CANCEL
                                    øMBOX_ENTER
                                    øMBOX_ENTERCANCEL
                                    øMBOX_NOICON
                                    øMBOX_CUANOTIFICATION
                                    øMBOX_CUAWARNING
                                    øMBOX_ICONQUESTION
                                    øMBOX_ICONEXCLAMATION
                                    øMBOX_ICONASTERISK
                                    øMBOX_ICONHAND
                                    øMBOX_CUACRITICAL
                                    øMBOX_QUERY
                                    øMBOX_WARNING
                                    øMBOX_INFORMATION
                                    øMBOX_CRITICAL
                                    øMBOX_ERROR
                                    øMBOX_CUSTOMICON
                                    øMBOX_DEFBUTTON1
                                    øMBOX_DEFBUTTON2
                                    øMBOX_DEFBUTTON3
                                    øMBOX_APPLMODAL
                                    øMBOX_SYSTEMMODAL
                                    øMBOX_HELP
                                    øMBOX_MOVEABLE
                                    øMBOX_NONMODAL
                              </t>
                              Can be or-ed.
           øXWindow * owner    øowner-window of the message box. If owner is set, the
                              message box is modal for the owner-window.
           øULONG id           øID for the message-box. If MBOX_HELP is set this value identifies the help-resource in your help-file.
                              Therefore the messagebox must have an owner-window and the owner-window must be related with an instance
                              of XHelpInstance.
            </t>
*/
XMessageBox :: XMessageBox(const char *s1, const char *s2, const ULONG style, const XWindow * owner, const ULONG id)
{
//   if(id) style |= MB_HELP;
   res = WinMessageBox(HWND_DESKTOP, owner ? owner->GetHandle() : HWND_DESKTOP, (PSZ) s1, (PSZ) (s2 ? s2 : ""), id, style);
}


/*@ XMessageBox :: XMessageBox(const XResource * ms1, const XResource * ms2, const ULONG style, const XWindow * owner, const ULONG id)
@group constructors/destructors
@remarks Construct a message box.
@parameters <t 'ø' c=2>
            øXResource * content     øresource which point to a string in the application resources to load and display as the messagebox content
           øXResource * title       øresource which point to a string in the application resources to load and display as the messagebox title
           øULONG style        øthe style, possible values are:
                                 <t 'ø' c=1>
                                    øMBOX_OK
                                    øMBOX_OKCANCEL
                                    øMBOX_RETRYCANCEL
                                    øMBOX_ABORTRETRYIGNORE
                                    øMBOX_YESNO
                                    øMBOX_YESNOCANCEL
                                    øMBOX_CANCEL
                                    øMBOX_ENTER
                                    øMBOX_ENTERCANCEL
                                    øMBOX_NOICON
                                    øMBOX_CUANOTIFICATION
                                    øMBOX_CUAWARNING
                                    øMBOX_ICONQUESTION
                                    øMBOX_ICONEXCLAMATION
                                    øMBOX_ICONASTERISK
                                    øMBOX_ICONHAND
                                    øMBOX_CUACRITICAL
                                    øMBOX_QUERY
                                    øMBOX_WARNING
                                    øMBOX_INFORMATION
                                    øMBOX_CRITICAL
                                    øMBOX_ERROR
                                    øMBOX_CUSTOMICON
                                    øMBOX_DEFBUTTON1
                                    øMBOX_DEFBUTTON2
                                    øMBOX_DEFBUTTON3
                                    øMBOX_APPLMODAL
                                    øMBOX_SYSTEMMODAL
                                    øMBOX_HELP
                                    øMBOX_MOVEABLE
                                    øMBOX_NONMODAL
                              </t>
                              Can be or-ed.
           øXWindow * owner    øowner-window of the message box. If owner is set, the
                              message box is modal for the owner-window.
           øULONG id           øID for the message-box. If MBOX_HELP is set this value identifies the help-resource in your help-file.
                              Therefore the messagebox must have an owner-window and the owner-window must be related with an instance
                              of XHelpInstance.
            </t>
*/
XMessageBox :: XMessageBox(const XResource * ms1, const XResource * ms2, const ULONG style, const XWindow * owner, const ULONG id)
{
   XString s1, s2;

   ms1->GetResourceLibrary()->LoadString(&s1, ms1->GetID());
   if (ms2)
   {
      if (ms2->GetID() > 0)
         ms2->GetResourceLibrary()->LoadString(&s2, ms2->GetID());
   }
   res = WinMessageBox(HWND_DESKTOP, owner ? owner->GetHandle() : HWND_DESKTOP, (PSZ) (char *) s1, (PSZ) (char *) s2, id, style);
}


XExtendedMessageBox :: XExtendedMessageBox(const char *s1, const char *s2, const ULONG style, const SHORT buttonCount, const XMessageButton ** buttonList, const XIcon * icon, const XWindow * owner, const ULONG id)
{
   MB2INFO *info = (MB2INFO *) malloc(sizeof(MB2INFO) + (buttonCount - 1) * sizeof(MB2D));

   memset(info, 0, sizeof(info));
   info->cb = sizeof(MB2INFO) + (buttonCount > 0 ? (buttonCount - 1) * sizeof(MB2D) : 0);
   info->flStyle = style;
   info->cButtons = buttonCount;

   if (buttonCount)
   {
      int i;

      for (i = 0; i < buttonCount; i++)
         memcpy(&info->mb2d[i], &buttonList[i]->info, sizeof(MB2D));
   }

   if (icon)
   {
      info->flStyle |= MB_CUSTOMICON;
      info->hIcon = icon->GetHandle();
   }

   res = WinMessageBox2(HWND_DESKTOP, owner ? owner->GetHandle() : HWND_DESKTOP, (PSZ) s1, (PSZ) (s2 ? s2 : ""), id, info);

   free(info);
}


XExtendedMessageBox :: XExtendedMessageBox(const XResource * ms1, const XResource * ms2, const ULONG style, const SHORT buttonCount, const XMessageButton ** buttonList, const XIcon * icon, const XWindow * owner, const ULONG id)
{
   XString s1, s2 = "";

   MB2INFO *info = (MB2INFO *) malloc(sizeof(MB2INFO) + (buttonCount - 1) * sizeof(MB2D));

   memset(info, 0, sizeof(info));
   info->cb = sizeof(MB2INFO) + (buttonCount > 0 ? (buttonCount - 1) * sizeof(MB2D) : 0);
   info->flStyle = style;
   info->cButtons = buttonCount;

   if (buttonCount)
   {
      int i;

      for (i = 0; i < buttonCount; i++)
         memcpy(&info->mb2d[i], &buttonList[i]->info, sizeof(MB2D));
   }

   if (icon)
   {
      info->flStyle |= MB_CUSTOMICON;
      info->hIcon = icon->GetHandle();
   }

   ms1->GetResourceLibrary()->LoadString(&s1, ms1->GetID());

   if (ms2->GetID() > 0)
      ms2->GetResourceLibrary()->LoadString(&s2, ms2->GetID());

   res = WinMessageBox2(HWND_DESKTOP, owner ? owner->GetHandle() : HWND_DESKTOP, (PSZ) (char *) s1, (PSZ) (char *) s2, id, info);
   free(info);
}
