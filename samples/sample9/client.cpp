#define INCL_OOL_PROCESS
#include "ool.h"

#include <stdio.h>
#include <stdlib.h>


void main ( void)
{
   LONG res;

   //a client
   XNamedPipeClient client;

   //open a pipe
   if( (res = client.Open( "Sample9Pipe" )) != 0)
      {
         printf( "cannot open pipe, rc was: %i", res);
         exit(0);
      }

   //look for a server (max two seconds in this case)
   client.WaitForServer( "Sampl9Pipe", 2000);

   if( !client.GetState() == XNPIPE_STATE_CONNECTED)
      {
         printf( "not connected\n");
         exit(0);
      }


   //try to read data
   LONG i=0, buffer, result=0;
   printf("got server\n");

   while( 1 )
      {
         result = 0;
         printf( "Read data? (Y)es, (E)xit\n");
         int key = getchar();
         getchar();                       //remove carriage return
         if (key == 'E' || key == 'e')
            exit(0);
         if(key == 'Y' || key == 'y')
            {
               for(i=0; i < 100; i++)
                  {
                     client.Read( buffer );
                     result += buffer;
                  }
               printf( "data read: %li\n", result );
            }
      }

   //clean up
   client.Close();
}
