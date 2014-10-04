#define INCL_OOL_WIN
#include "ool.h"

#include "resource.h"

class MyWindow: public XFrameWindow
{
        public:
                void Draw();
                BOOL DoCommand(LONG);
                void DoSize(XSize *);
                MyWindow();
};


MyWindow :: MyWindow( ): XFrameWindow( IDM_MENU , "Hello world!", FRM_MENU | XFrameWindow::defaultStyle)
{
        //at first we define a color which is used to paint the background
        //we chose white as background color
        XColor color(COL_WHITE);
        SetBackgroundColor( &color );

        //create a text
        //the owner of the text is this window, show the text "Hello World"
        XStaticText * text = new XStaticText(this, "Hello world!", XRect( 100, 100, 200, 20), TX_CENTER, 300);

        //create a toolbar
        XToolBar * toolBar = new XToolBar( this );

        //now two buttons are created and attached to the toolbar
        XPushButton * btnOpen = new XPushButton( toolBar, XRect( 0,0,45,25), IDM_FILE_OPEN, WIN_VISIBLE, "Open", "8.Helv");
        toolBar->AddWindow( btnOpen, FALSE, TRUE );

        XPushButton * btnClose = new XPushButton( toolBar, XRect( 0,0,45,25), IDM_FILE_CLOSE, WIN_VISIBLE, "Close", "8.Helv");
        toolBar->AddWindow( btnClose );

        //Set the position of this window
        //left-lower corner is 100,100, width is 300, height is 200
        XRect rect(100,100,300,200);
        SetSize(&rect);

        //make this window the active window
        Activate();
}


BOOL MyWindow :: DoCommand(LONG command)
{
    //now look which command is send
    switch( command )
    {
        case IDM_FILE_OPEN:
            //handle the command to open a file
            GetWindow(300)->SetText( "File - open selected!");
            break;
        case IDM_FILE_CLOSE:
            //handle the command to close a file
            GetWindow(300)->SetText( "File - close selected!");
            break;
        case IDM_SELECT_ALL:
            //handle the command to close a file
            GetWindow(300)->SetText( "Select - all selected!");
            break;
        case IDM_SELECT_NONE:
            //handle the command to close a file
            GetWindow(300)->SetText( "Select - none selected!");
            break;
        default:
            return FALSE;   //show the library that we did not handle the command
    }
    return TRUE; //show the library that we have handled the command
}


void MyWindow :: Draw()
{
    //just fill the background
    FillBackground();
}


void MyWindow :: DoSize(XSize * size )
{
    XWindow * text = GetWindow(300);
    if(text) //is there a text object?
    {
        //calculate the new size/position of the text object
        XRect newSize;
        newSize.SetX( size->GetWidth() / 2 - 100 );
        newSize.SetY( size->GetHeight() / 2 - 10 );
        newSize.SetWidth( 200 );
        newSize.SetHeight( 20 );
        text->SetSize( &newSize );
    }
}


int main()
{
   try
   {
      //now we get a pointer to the
      //application object
      XApplication * currentApp = XApplication::GetApplication();

      //create the window
      MyWindow * myWin = new MyWindow();

      //let the application start
      currentApp->Start();
   }
   catch( XException& e)
   {
       e.ShowError();
   }

   return 0;
}

