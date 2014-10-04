#define INCL_OOL_WIN
#include "ool.h"


class MyWindow: public XFrameWindow
{
   public:
      void Draw();
      BOOL DoCommand(LONG);
      MyWindow();
};


MyWindow :: MyWindow( ): XFrameWindow( 100 , "Hello world!", XFrameWindow::defaultStyle)
{
   XContainerControl * container = new XContainerControl( this );

   SetClient( container );

   //Set the position of this window
   //left-lower corner is 100,100, width is 300, height is 200
   XRect rect(100,100,300,200);
   SetSize(&rect);

   //set the container in detail-mode
   XContainerInfo info;
   info.SetAttributes( CO_DETAIL | CO_DETAILTITLES);
   container->SetInfo( &info );

   XContainerColumn * col1 = new XContainerColumn( container,   //the owner
                                                "Column 1",  //title of the column
                                                0,           //first column
                                                COL_HORZSEPARATOR | COL_STRING | COL_SEPARATOR, COL_LEFT | COL_FITITLEREADONLY | COL_HORZSEPARATOR | COL_TOP ); //settings
   //insert the column
   container->InsertColumn( col1 );

   XContainerColumn * col2 = new XContainerColumn( container,   //owner
                                                "Column 2",  //title
                                                1,           //second column
                                                COL_SEPARATOR | COL_HORZSEPARATOR | COL_STRING, COL_LEFT | COL_FITITLEREADONLY | COL_HORZSEPARATOR | COL_TOP );

   //insert the second column behind the first one
   container->InsertColumn( col2, col1 );

   //draw the container columns
   container->UpdateColumns();

   //now insert some objects
   XContainerObject * obj1 = new XContainerObject(container, 2);
   obj1->SetColumnData( 0, "text1");
   obj1->SetColumnData( 1, "text2");
   container->AddObject(obj1);

   XContainerObject * obj2 = new XContainerObject(container, 2);
   obj2->SetColumnData( 0, "text1");
   obj2->SetColumnData( 1, "text2");
   container->AddObject(obj2);

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

