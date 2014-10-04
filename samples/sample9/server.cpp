#define INCL_OOL_PROCESS
#include "ool.h"

#include <process.h>
#include <stdlib.h>
#include <stdio.h>

XString semName;

//we create a thread
class MyServer: public XThread
{
      XNamedPipeServer server;
      int number;
   public:
      MyServer(int nr);
      void Start( );
      virtual void Init(void);
};


//run the thread
MyServer :: MyServer(int n)
{
   number = n;
   Run();
}


//method is called automaticaly from the thread
void MyServer::Init(void)
{
   LONG res;
   if( (res = server.Open( "Sample9Pipe", XNPIPE_ACCESS_DUPLEX, XNPIPE_WAIT|XNPIPE_TYPE_BYTE|XNPIPE_READMODE_BYTE, 2 )) != 0)
   {
      printf( "cannot open server: %i", res);
      exit(0);
   }

   //now enter the loop for message-handling
   Start();
}


//start this server
void MyServer :: Start( )
{
   printf("server %i started, id = %i\n", number, number * 10);
   LONG res;

   //wait for a connect
   server.Connect();

   while(1)
   {
      //client is killed?
      if( server.GetState() == XNPIPE_STATE_CLOSING)
      {
         //clear connection
         server.DisConnect();
         //wait for the next client
         if( (res = server.Connect()) != 0)
         {
            printf( "cannot connect: %i id=%i", res, number*10);
            exit(0);
         }
      }

      //we have a client
      if( server.GetState() == XNPIPE_STATE_CONNECTED)
      {
         //write some data
         LONG i;
         for(i=0; i < 10000; i++)
            server.Write( i + number*10);
         server.ResetBuffer();
      }
   }
}


void main ( void)
{
   LONG res;
   printf( "press Ctrl+C to stop the server\n");

   //in this case we start two servers
   try
   {

      MyServer * server1 = new MyServer(1);
      MyServer * server2 = new MyServer(2);

      //wait until the user kills this process
      //Sleep(0) gives control to other threads/processes
      while(1)
         XProcess::Sleep(0);
   }
   catch( XException& e)
   {
      e.PrintError();
   }
}
