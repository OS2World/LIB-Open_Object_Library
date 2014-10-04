#define INCL_OOL_WIN
#include "ool.h"

class MyWindow: public XFrameWindow
{
        public:
                void Draw();
                BOOL DoCommand(LONG);
                MyWindow();
};


MyWindow :: MyWindow( ): XFrameWindow( 100 , "Hello world!", XFrameWindow::defaultStyle|FRM_TASKLIST)
{
        //at first we define a color which is used to paint the background
        //we chose white as background color
        XColor color(COL_WHITE);
        SetBackgroundColor( &color );

        //create a text
        //the owner of the text is this window, show the text "Hello World"
        XStaticText * text = new XStaticText(this, "Hello world!", XRect( 100, 100, 150, 20));

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
        return TRUE;
}


void MyWindow :: Draw()
{
        //just fill the background
        FillBackground();
}

int main()
{
        //now we get a pointer to the
        //application object
        XApplication * currentApp = XApplication::GetApplication();

        //create the window
        MyWindow * myWin = new MyWindow();

        //let the application start
        currentApp->Start();

   return 0;
}

