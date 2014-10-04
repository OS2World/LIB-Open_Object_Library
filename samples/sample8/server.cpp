#include "server.h"

#include <stdlib.h>

SHORT volume = 50;

class MyDDE: public XDDE
{
   public:
      MyDDE( XWindow * w):XDDE(w, TRUE){clients=NULL;clientCount = 0;}
      void ConnectionRequested( char * app, char * topic, OOL_WINDOWHANDLE handle);
      void AdviseRequested(char * itemName, SHORT format, OOL_WINDOWHANDLE client);
      void DisConnected();
      void UnAdviseRequested( OOL_WINDOWHANDLE c);
      SHORT clientCount;
      OOL_WINDOWHANDLE * clients;
};


void MyDDE :: ConnectionRequested( char * app, char * topic, OOL_WINDOWHANDLE handle)
{
   if((strcmp("Sample8", app) == 0) && (strcmp("sampleTopic", topic) == 0))
   {
      AcceptConnection(app, topic, handle);
      GetOwner()->SetText("DDEServer - connected");
   }
}


void MyDDE :: UnAdviseRequested( OOL_WINDOWHANDLE c)
{
   int i;
   for(i=0; i < clientCount; i++)
      {
         if( clients[i] == c)
            clients[i] = 0;
      }
}


void MyDDE :: AdviseRequested(char * itemName, SHORT format, OOL_WINDOWHANDLE c)
{
   clientCount += 1;
   clients = (OOL_WINDOWHANDLE *) realloc( clients, clientCount * sizeof(OOL_WINDOWHANDLE));
   clients[clientCount -1] = c;
}



void MyDDE :: DisConnected( )
{
   GetOwner()->SetText("DDEServer - not connected");
}


MyAppWindow :: MyAppWindow( ): XModelessDialog( IDM_MAIN )//, "Sample8 - DDEServer", XFrameWindow::defaultDialogStyle | FRM_TASKLIST | FRM_CENTER, NULL, NULL, TRUE)
{
   Show();

   SetText("DDEServer - not connected");
   dde = new MyDDE(this );
   Activate();
}


MyAppWindow :: ~MyAppWindow()
{
   delete dde;
}


void MyAppWindow :: DoControl( XControlEvent * event)
{

   if( event->GetEventID() == WIN_KILLFOCUS)
      {
         char * item = NULL;
         switch( event->GetWindowID() )
            {
               case ENTRY_ONE:
                  item = "FIELD1";
                  break;
               case ENTRY_TWO:
                  item = "FIELD2";
                  break;
               case ENTRY_THREE:
                  item = "FIELD3";
                  break;
               case ENTRY_FOUR:
                  item = "FIELD4";
                  break;
            }
         if( item) //dde->IsConnected() &&
            {
               XString data;
               event->GetWindow()->GetText( &data );
               SHORT i;
               for(i=0; i < dde->clientCount; i++)
                  {
                     if( dde->clients[i] != 0)
                        dde->SendData( dde->clients[i], item, (char*) data, data.GetLength() + 1, 0);
                  }
            }
      }
}


void main ( void)
{
   MyAppWindow * window = new MyAppWindow();   //create new framewindow (see above)
   XApplication::GetApplication()->Start();    //let the application work
}
