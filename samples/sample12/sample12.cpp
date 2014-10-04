#define INCL_OOL_WIN
#include "ool.h"
#include <stdio.h>

//this is the function which can be called from a rexx-script
REXXINTERFACE MyFunc( PUCHAR name, ULONG argc, PRXSTRING argv, PSZ* queue, PRXSTRING ret)
{
   //any arguments?
   if(argc > 0)
   {
       XString argument;
       //yep, print them
      for(int i = 0; i < argc;i++)
      {
           RXStringToXString( argv[i], argument);
         printf("MyFunc (C++), arguments[%i]: %s\n", i, (char*) argument);
      }
   }
   else
      printf("MyFunc (C++): no args\n");

   //return a string-value, it will be posted to our rexx-handler
   XString result = "NextCommand";
   XStringToPRXString( result, ret);
   return 0;
}


//a command-handler which will get commands from the rexx-script
REXXINTERFACE Handler(PRXSTRING commandString, PUSHORT flags, PRXSTRING returnString)
{
   XString command;

   //convert
   PRXStringToXString(commandString, command);

   //no commands, return
   if( command.IsEmpty())
      return 0;

   //which command?
   if(command == "INIT_VARS")
   {
      //gt a pointer to the used rexx-interface
       XRexxInterface * handler = XRexxInterface::GetHandler("Handler");
      if(handler == NULL)
      {
         printf("no handler found");
         return 0;
      }
       //set a varibale which can be used in the rexx-script
      handler->SetVar( "TEST_VAR", "1.0");
      printf( "Handler (C++): vars set\n");
   }
   else
      printf( "Handler (C++): donït know how to handle command:%s\n", (char*) command);

   //end
   *flags = RXSUBCOM_OK;
   return 0;
}


int main(int argc, void ** argv)
{
   XString returnString, arg= "123";
   SHORT returnValue;

   //register the function 'MyFunc' so it can be used from the rexx-script
   if( XRexxInterface::RegisterFunction( (char*) "MyFunc", (PFN) MyFunc) !=0 )
         printf( "cannot register function\n");

   //install a command-handler which will receive commands from the rexx-script
   XRexxInterface handler( "Handler", (PFN) Handler);

   //run the rexx-script
   LONG rc = handler.Execute( "test.mac", NULL, RXCOMMAND, &returnString, &returnValue, &arg );
printf("test\n");
   if(rc)
      printf("%i\n", rc);

   //end
   printf("done, returned: %s\n", (char*) returnString);
   return 0;
}
