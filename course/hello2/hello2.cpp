#define INCL_OOL_WIN
#include "ool.h"


class MyWindow: public XFrameWindow
{
        public:
                void Draw();
                BOOL DoCommand(LONG);
                void DoSize(XSize *);
                MyWindow();
};


MyWindow :: MyWindow( ): XFrameWindow( 100 , "Hello world!", XFrameWindow::defaultStyle)
{
        //at first we define a color which is used to paint the background
        //we chose white as background color
        XColor color(COL_WHITE);
        SetBackgroundColor( &color );

        //create a text
        //the owner of the text is this window, show the text "Hello World"
        XStaticText * text = new XStaticText(this, "Hello world!", XRect( 100, 100, 150, 20), TX_CENTER, 300);

        //Set the position of this window
        //left-lower corner is 100,100, width is 300, height is 200
        XRect rect(100,100,300,200);
        SetSize(&rect);

        //make this window the active window
        Activate();
}


BOOL MyWindow :: DoCommand(LONG com)
{
        //at this point we dont care about commands
        return FALSE;
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
                newSize.SetX( size->GetWidth() / 2 - 75 );
                newSize.SetY( size->GetHeight() / 2 - 10 );
                newSize.SetWidth( 150 );
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

