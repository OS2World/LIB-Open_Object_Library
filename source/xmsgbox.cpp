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
                        <t '�' c=1>
                        �MDID_OK
                        �MDID_CANCEL
                        �MDID_ABORT
                        �MDID_RETRY
                        �MDID_IGNORE
                        �MDID_YES
                        �MDID_NO
                        �MDID_HELP
                        �MDID_ENTER
                        �MDID_ERROR
                     </t>
*/


/*@ XMessageBox :: XMessageBox(const char *s1, const char *s2, const ULONG style, const XWindow * owner, const ULONG id)
@group constructors/destructors
@remarks Construct a message box.
@parameters <t '�' c=2>
            �char * content     �the content of the message box
           �char * title       �the title of the message box
           �ULONG style        �the style, possible values are:
                                 <t '�' c=1>
                                    �MBOX_OK
                                    �MBOX_OKCANCEL
                                    �MBOX_RETRYCANCEL
                                    �MBOX_ABORTRETRYIGNORE
                                    �MBOX_YESNO
                                    �MBOX_YESNOCANCEL
                                    �MBOX_CANCEL
                                    �MBOX_ENTER
                                    �MBOX_ENTERCANCEL
                                    �MBOX_NOICON
                                    �MBOX_CUANOTIFICATION
                                    �MBOX_CUAWARNING
                                    �MBOX_ICONQUESTION
                                    �MBOX_ICONEXCLAMATION
                                    �MBOX_ICONASTERISK
                                    �MBOX_ICONHAND
                                    �MBOX_CUACRITICAL
                                    �MBOX_QUERY
                                    �MBOX_WARNING
                                    �MBOX_INFORMATION
                                    �MBOX_CRITICAL
                                    �MBOX_ERROR
                                    �MBOX_CUSTOMICON
                                    �MBOX_DEFBUTTON1
                                    �MBOX_DEFBUTTON2
                                    �MBOX_DEFBUTTON3
                                    �MBOX_APPLMODAL
                                    �MBOX_SYSTEMMODAL
                                    �MBOX_HELP
                                    �MBOX_MOVEABLE
                                    �MBOX_NONMODAL
                              </t>
                              Can be or-ed.
           �XWindow * owner    �owner-window of the message box. If owner is set, the
                              message box is modal for the owner-window.
           �ULONG id           �ID for the message-box. If MBOX_HELP is set this value identifies the help-resource in your help-file.
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
@parameters <t '�' c=2>
            �XResource * content     �resource which point to a string in the application resources to load and display as the messagebox content
           �XResource * title       �resource which point to a string in the application resources to load and display as the messagebox title
           �ULONG style        �the style, possible values are:
                                 <t '�' c=1>
                                    �MBOX_OK
                                    �MBOX_OKCANCEL
                                    �MBOX_RETRYCANCEL
                                    �MBOX_ABORTRETRYIGNORE
                                    �MBOX_YESNO
                                    �MBOX_YESNOCANCEL
                                    �MBOX_CANCEL
                                    �MBOX_ENTER
                                    �MBOX_ENTERCANCEL
                                    �MBOX_NOICON
                                    �MBOX_CUANOTIFICATION
                                    �MBOX_CUAWARNING
                                    �MBOX_ICONQUESTION
                                    �MBOX_ICONEXCLAMATION
                                    �MBOX_ICONASTERISK
                                    �MBOX_ICONHAND
                                    �MBOX_CUACRITICAL
                                    �MBOX_QUERY
                                    �MBOX_WARNING
                                    �MBOX_INFORMATION
                                    �MBOX_CRITICAL
                                    �MBOX_ERROR
                                    �MBOX_CUSTOMICON
                                    �MBOX_DEFBUTTON1
                                    �MBOX_DEFBUTTON2
                                    �MBOX_DEFBUTTON3
                                    �MBOX_APPLMODAL
                                    �MBOX_SYSTEMMODAL
                                    �MBOX_HELP
                                    �MBOX_MOVEABLE
                                    �MBOX_NONMODAL
                              </t>
                              Can be or-ed.
           �XWindow * owner    �owner-window of the message box. If owner is set, the
                              message box is modal for the owner-window.
           �ULONG id           �ID for the message-box. If MBOX_HELP is set this value identifies the help-resource in your help-file.
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
