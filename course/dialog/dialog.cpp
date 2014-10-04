#include "dialog.h"
#include "resource.h"

#include <stdlib.h>

//this is our modeless dialog class
class MyModelessDialog: public XModelessDialog
{
    public:
        MyModelessDialog( XWindow * w):XModelessDialog( IDM_MODELESSDIALOG, w)
                { GetWindow(PUSH_OK)->SetText("OK");}   //init the dialog
        BOOL DoCommand( LONG com );
};


BOOL MyModelessDialog :: DoCommand( LONG com )
{
    //a command was posted, destroy the dialog in every case in this sample
    return TRUE;
}


class MyModalDialog: public XModalDialog
{
    public:
        MyModalDialog( XWindow * w):XModalDialog( IDM_MODALDIALOG, w)
            { GetWindow(PUSH_OK)->SetText("OK");} //init the dialog
        BOOL DoCommand( LONG com );
};


BOOL MyModalDialog :: DoCommand( LONG com )
{
    //a command was posted, return TRUE to destroy the dialog in every case in this sample
    return TRUE;
}


MyAppWindow :: MyAppWindow( ): XFrameWindow( IDM_MENU, "Dialogs", XFrameWindow::defaultStyle | FRM_TASKLIST | FRM_CENTER | FRM_MENU)
{
    XColor color(COL_WHITE);
    SetBackgroundColor( &color );

    Activate();
}


MyAppWindow :: ~MyAppWindow()
{
    //Terminate the application
    XApplication::GetApplication()->Terminate();
}


/* here the commands of the menu are posted */
BOOL MyAppWindow :: DoCommand( LONG com)
{
    switch( com )
    {
        case IDM_MODELESS:   //show the modeless dialog
        {
            try
            {
                MyModelessDialog * dlg = new MyModelessDialog( this);
            }
            catch( XException e)
            {
                XMessageBox( e.GetErrorMessage());
            }
            break;
         }
        case IDM_MODAL:      //show the modeless dialog
        {
            try
            {
                MyModalDialog * dlg = new MyModalDialog( this);
                LONG result = dlg->Start();   //dont forget to make it modal, result
            }                                 //receives the command which destroyed the dialog
            catch( XException& e)
            {
                XMessageBox( e.GetErrorMessage());
            }
        }
        break;
    }
    return TRUE;
}


void MyAppWindow :: Draw( void )
{
   FillBackground();
}


void main ( void)
{
   try
   {
      MyAppWindow * window = new MyAppWindow();

      //Set the size
      XRect rec(200,200, 300, 200);
      window->SetSize( &rec );

      //let the application work
      XApplication::GetApplication()->Start();
   }
   catch( XException& e)
   {
      e.ShowError();
   }
}
