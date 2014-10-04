#define INCL_OOL_WIN
#include "ool.h"

//class declaration
class MyWindow: public XFrameWindow
{
    public:
        void Draw();
        MyWindow();
};


class MyMouseHandler: public XMouseHandler
{
    public:
        MyMouseHandler( XFrameWindow * w): XMouseHandler(w) {;}
        BOOL HandleEvent(XMouseEvent * );
};


//in this method we can handle mouse-events
BOOL MyMouseHandler :: HandleEvent(XMouseEvent * event)
{
    switch( event->GetEventID())
    {
        case MOU_BTN1DOWN:
        {
            XProcess::Beep( 200, 200);
            char text[50];
            sprintf( text, "Button 1, x: %i, y: %i", event->p.GetX(), event->p.GetY());
            ((XFrameWindow *) GetWindow())->GetWindow(300)->SetText( text );
        }
        break;
        case MOU_BTN2DOWN:
        {
            XProcess::Beep( 800, 200);
            char text[50];
            sprintf( text, "Button 2, x: %i, y: %i", event->p.GetX(), event->p.GetY());
            ((XFrameWindow *) GetWindow())->GetWindow(300)->SetText( text );
        }
        break;
    }
    return TRUE;
}


MyWindow :: MyWindow( ): XFrameWindow( 100 , "Handling events!", XFrameWindow::defaultStyle)
{
    //at first we define a color which is used to paint the background
    //we chose white as background color
    XColor color(COL_WHITE);
    SetBackgroundColor( &color );

    //create a static text
    XStaticText * text = new XStaticText(this, "Hello world!", XRect( 20, 100, 200, 20), WIN_VISIBLE| TX_LEFT, 300);

    //now create our mouse-handler for this window
    MyMouseHandler * handler = new MyMouseHandler(this);

    //Set the position of this window
    //left-lower corner is 100,100, width is 300, height is 200
    XRect rect(100,100,300,230);
    SetSize(&rect);

    //make this window the active window
    Activate();
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
       e.ShowError();
   }

   return 0;
}

