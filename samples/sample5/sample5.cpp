#include "sample5.h"

#include <stdlib.h>

//here a window is created out of the resource-DLL, the last parameter tells the library
//to build the window with the ID given in XResource. .
MyAppWindow :: MyAppWindow( ): XScrollWindow( WIN_MAIN, "Sample5 - Resource DLL", XFrameWindow::defaultStyle | FRM_TASKLIST | FRM_CENTER,  NULL, NULL, TRUE )
//MyAppWindow :: MyAppWindow( ): XScrollWindow( WIN_MAIN, "Sample5 - Resource DLL", XFrameWindow::defaultStyle | FRM_TASKLIST | FRM_CENTER )
{
   //we add a menubar to the window, the resources are in the resource-DLL
   XMenuBar * menu = new XMenuBar( this, IDM_MENU);

   //add scroller to the window
   AddVertScroller();
   AddHorzScroller();

   //Activate the window
   Activate();
}


MyAppWindow :: ~MyAppWindow()
{
}


/* here the commands of the menu and the pushbuttons are posted */
BOOL MyAppWindow :: DoCommand( LONG com)
{
   switch( com )
      {
         //the two menu-items of interest, we terminate the application in every case
         case IDM_CANCEL:
         case IDM_OK:
         //also push-buttons post a command if they are pressed
         case PUSH_OK:
         case PUSH_CANCEL:
            XApplication::GetApplication()->Terminate();
            break;
      }
   return TRUE;
}


//here the control-events of our window-contents are posted
void MyAppWindow :: DoControl( XControlEvent * event)
{
   switch( event->GetEventID()) //what type of event?
      {
         case WIN_CHANGED:      //window-content changed
            {
               switch( event->GetWindowID()) //which window posted the event?
                  {
                     case RADIO_MALE:        //the radio-button MALE is clicked
                        //add your code here to handle the event
                        break;
                     case RADIO_FEMALE:      //the radio-button FEMALE is clicked
                        //add your code here to handle the event
                        break;
                     default:
                        break;        //entry-field, ignored in this sample
                  }
            }
      }
}


void main ( void)
{
   try
   {
      //we create a resource-DLL (sample5.dll), it contains the resources for the window
      XResourceLibrary * resourceLib = new XResourceLibrary( "sample5" );

      //we set the resource-library a the actual library for the application, all resources
      //are loaded from this library
      XApplication::GetApplication()->SetResourceLibrary( resourceLib );

      //create the window
      MyAppWindow * window = new MyAppWindow();

      //because we have added a menubar and scrollers, we must resize the window
      //but we cannot do it in the construcor!
      XRect rec;

      window->GetSize( &rec );

      rec.SetHeight( rec.GetHeight() + 40);
      rec.SetWidth( rec.GetWidth() + 15);

      window->SetSize( &rec );
      XApplication::GetApplication()->Start();               //let the application work
   }
   catch( XException e)
   {
      e.ShowError();
   }
}
