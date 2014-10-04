#define INCL_OOL_WIN
#include "ool.h"

#include <stdlib.h>

#define ID_NOTEBOOK 200
#define ID_PAGE1    210
#define ID_OK       211

class MyWindow: public XFrameWindow
{
    public:
        BOOL DoCommand(LONG);
        MyWindow();
};


MyWindow :: MyWindow( ): XFrameWindow( 100 , "Hello world!", XFrameWindow::defaultStyle)
{
    //create a notebook-control
    XNoteBook * noteBook = new XNoteBook( this,                  // "this" is a pointer to the owner window
                                              XRect( 10,10,200,300), // position and size
                                              ID_NOTEBOOK,           // ID
                                              NB_TABBEDDIALOG|WIN_VISIBLE|NB_SOLIDBIND|NB_BACKPAGESBR|NB_SQUARETABS|NB_TABTEXTCENTER|NB_STATUSTEXTLEFT,
                                              "8.Helv");             // font to use
    // first we give the noebook a new color
    XColor grayColor(COL_PALEGRAY );
    noteBook->SetBackgroundColor( &grayColor );

    // we use the notebook in the client area so the notbook will be
    // resized automaticay when the frame window gets a new size
    SetClient( noteBook );

    // we create an empty notebook page....
    XNoteBookPage * page1 = new XNoteBookPage( noteBook,  // the owner of the page is the notebook-control
                                             ID_PAGE1);  // the ID of the resource
    // ...and set a title...
    page1->SetText( "Page 1" );

    // ...bring the page to the top...
    page1->SetTop();

    // ...and add a control to the page
    XPushButton * button = new XPushButton( page1,   //the page is the owner
                                            XRect( 60, 40, 70, 25),
                                            ID_OK,
                                            WIN_VISIBLE,
                                            "OK" );

    // now we add a second page...
    XNoteBookPage * page2 = new XNoteBookPage( noteBook,  // the owner of the page is the notebook-control
                                             ID_PAGE1);  // the ID of the resource
    // ...and give it a title
    page2->SetText( "Page 2" );

    // Set the position of this window
    // left-lower corner is 100,100, width is 300, height is 200
    XRect rect(100,100, 400, 300);
    SetSize(&rect);

    // make this window the active window
    Activate();
}


BOOL MyWindow :: DoCommand(LONG com)
{
    // at this point we dont care about commands
    return FALSE;
}


int main()
{
   try
   {
      // now we get a pointer to the
      // application object
      XApplication * currentApp = XApplication::GetApplication();

      //create the window
      MyWindow * myWin = new MyWindow();

      //let the application start
      currentApp->Start();
   }
   catch( XException& e)
   {
       e.ShowError();
       exit(-1);
   }
}

