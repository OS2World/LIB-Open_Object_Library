#define INCL_OOL_WIN
#include "ool.h"

//some defines for control-windows we will create
#define IDE_ENTRYNAME 200
#define IDC_LISTNAME  201

//class declaration
class MyWindow: public XFrameWindow
{
        public:
                void Draw();
                void DoControl(XControlEvent *);
                MyWindow();
};


MyWindow :: MyWindow( ): XFrameWindow( 100 , "Handling events!", XFrameWindow::defaultStyle)
{
    //at first we define a color which is used to paint the background
    //we chose white as background color
    XColor color(COL_WHITE);
    SetBackgroundColor( &color );

    //Set the position of this window
    //left-lower corner is 100,100, width is 300, height is 200
    XRect rect(100,100,300,230);
    SetSize(&rect);

    //now we create some control windows so events can be posted
    //at first a combobox
    XComboBox * combo = new XComboBox( this, XRect( 20, 100, 200, 90), IDC_LISTNAME, CB_DROPDOWNLIST | WIN_VISIBLE);
    //insert some rows
    combo->InsertItem( "Line 1");
    combo->InsertItem( "Line 2");
    combo->InsertItem( "Line 3");
    combo->InsertItem( "Line 4");
    combo->InsertItem( "Line 5");

    //create a simple entry field
    XEntryField * entry = new XEntryField( this, XRect( 20, 60, 100, 20), IDE_ENTRYNAME, ES_MARGIN | EN_LEFT | WIN_VISIBLE);

    //make this window the active window
    Activate();
}


void MyWindow :: DoControl(XControlEvent * event )
{
    switch( event->GetEventID())
    {
        case WIN_CHANGED:
            if( event->GetWindowID() == IDE_ENTRYNAME)  // in this case we are only interested
            {                                           // for the window with the ID IDE_ENTRYNAME
                XString buffer;
                event->GetWindow()->GetText( &buffer ); // here we read the new text of the window
                                                        // do something with the text
            }
            break;
        case WIN_ENTER:
            if( event->GetWindowID() == IDC_LISTNAME)   // in this case we are only inerested
            {                                           // for a combobox with the ID IDL_LISTNAME
                XString buffer;                         // WARNING: the following typecast ist only
                                                        // allowed if you are shure, that the window
                                                        // IS a combobox
                ((XComboBox*) event->GetWindow())->GetText( &buffer );
                GetWindow( IDE_ENTRYNAME )->SetText(buffer);
            }
            break;
    }
}


void MyWindow :: Draw()
{
        //just fill the background
        FillBackground();
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
       e.PrintError();
   }

   return 0;
}

