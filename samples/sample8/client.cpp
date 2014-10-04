#include "client.h"

#include <stdlib.h>


class MyDDE: public XDDE
{
   public:
      MyDDE( XWindow * w):XDDE(w){;}
      void DataReceived( ULONG size, void * data, char * itemName, SHORT format);
      void Connected( OOL_WINDOWHANDLE handle);
      void DisConnected( void );
};


void MyDDE :: Connected( OOL_WINDOWHANDLE handle)
{
   //init was succesful, we have a connect
   GetOwner()->SetText( "DDEClient - connected");

   //ask for data
   Advise();
}


void MyDDE :: DisConnected( void )
{
   GetOwner()->SetText( "DDEClient - not connected");
}


void MyDDE :: DataReceived( ULONG size, void * data, char * itemName, SHORT format)
{
   XString item = itemName;
   LONG id = 0;

   if(item == "FIELD1")
      id = ENTRY_ONE;
   if(item == "FIELD2")
      id = ENTRY_TWO;
   if(item == "FIELD3")
      id = ENTRY_THREE;
   if(item == "FIELD4")
      id = ENTRY_FOUR;

   if( id )
      {
         XModelessDialog * w = (XModelessDialog*) GetOwner();
         XWindow * entry = w->GetWindow( id );
         if( entry )
            entry->SetText( (char*) data );
      }
}


MyAppWindow :: MyAppWindow( ): XModelessDialog( IDM_MAIN )//, "Sample8 - DDEClient", XFrameWindow::defaultDialogStyle | FRM_TASKLIST | FRM_CENTER, NULL, NULL, TRUE)
{
   Show();

   Activate();

   //construct a dde
   dde = new MyDDE(this );

   //connect
   dde->Init("Sample8", "sampleTopic");
}


MyAppWindow :: ~MyAppWindow()
{
   dde->UnAdvise();
   delete dde;
}


void main ( void)
{
   MyAppWindow * win = new MyAppWindow();
   XApplication::GetApplication()->Start();               //let the application work
}
