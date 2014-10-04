#include "xwizard.h"

/*@
@class XWizClientWindow
@type overview
@parent XResourceWindow
@symbol _
@remarks XWizClientWindow is a window which is used in XWizard. Unlike XResourceWindow
it posts events received in DoCommand() and DoControl() to the owning XWizard.<P>
*/

/*@ XWizClientWindow :: XWizClientWindow(XWizard * w, ULONG id)
@group constructor/destructor
@remarks Create a resource-window
@parameters
<t 'ø' c=2>
øXWizard *  øowner
øULONG øID of the window-resource (the resource must be linked to the exe-file)
</t>
*/


/*@ XWizClientWindow :: XWizClientWindow(XWizard * w, XResource*)
@group constructor/destructor
@remarks Create a resource-window
@parameters
<t 'ø' c=2>
øXWizard *  øowner
øXResource* øresource
</t>
*/

/*@ XWizClientWindow :: SetNextPage(XWizClientWindow*next)
@group navigation
@remarks Set the following page to display in the XWizard
@parameters
XWizClientWindow * the next window
*/

/*@ XWizClientWindow :: SetPrevPage(XWizClientWindow*next)
@group navigation
@remarks Set the previous page to display in the XWizard
@parameters
XWizClientWindow * the next window
*/


/*@
@class XWizard
@type overview
@parent XModalDialog
@symbol _
@remarks XWizard is a class to implement a dialog with multiple pages as
they are used for 'Wizards' or 'Assistants'.<P>
Usualy you create an empty dialog-template and set a XWizClientWindow as a start page.
The you define for the start page which page to display next (see XWizClientWindow::SetNextPage()),
you can decide at runtime, which page should be displayed. To start the wizard
call Init(). If the last page is reached
you perform the complete action your wizard is designed for.
*/


/*@ XWizard :: Init()
@group misc
@remarks Start the wizard
*/
void XWizard :: Init()
{
   if(start)
      start->Show();
   Show();
   Start();
}


/*@ XWizard :: ShowNextPage()
@group navigation
@remarks Show the next page which is specified in the current displayed XWizClientWindow.
@returns BOOL success
*/
BOOL XWizard :: ShowNextPage()
{
   if( current)
   {
      if(current->next)
      {
         current->Show(FALSE);
         current = current->next;
         SetClientSize();
         current->Show();
         current->SetFocus();
         return TRUE;
      } /* end if */
   }
   return FALSE;
}


void XWizard :: SetClientSize()
{
   XRect rect;
   current->GetSize(&rect);
   rect.SetX(5);
   rect.SetY(5);
   current->SetSize(&rect);
}


/*@ XWizard :: ShowPrevPage()
@group navigation
@remarks Show the previous page which is specified in the current displayed XWizClientWindow.
@returns BOOL success
*/
BOOL XWizard :: ShowPrevPage()
{
   if( current)
   {
      if(current->prev)
      {
         current->Show(FALSE);
         current = current->prev;
         SetClientSize();
         current->Show();
         current->SetFocus();
         return TRUE;
      } /* end if */
   }
   return FALSE;
}


/*@ XWizard :: XWizard(const XWindow*w, const ULONG id)
@group constructors/destructors
@remarks Create a wizard
@parameters
<t 'ø' c=2>
øXWindow* øowner
øULONG    øresource-id
</t>
*/
XWizard :: XWizard(const XWindow*w, const ULONG id): XModalDialog( id, w )
{
   current = start = NULL;
}

