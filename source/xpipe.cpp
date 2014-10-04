#include "XPipe.h"


/*@
@class XPipe
@parent XIO
@type overview
@symbol _
*/

/*@
@class XPipe
@type overview
@symbol _
@remarks XPipe is a class which provides data-communication between related processes.
Therefore you redirect stdin/stdout/stderr to a pipes read- and/or write-handle(s) of the
server and client process. The client must be a client-process (see OS/2-docs for details).<BR>
To do <B>realy</B> client/server data-communication use DDE (class XDDE) or named pipes (XNamedPipeServer, XNamedPipeClient).<P>
@example
   ULONG hfSave, hfNew = XPIPE_STDERROR;

   //save stderror-handle
   XPipe::DuplicateHandle( XPIPE_STDERROR, hfSave);

   //create a pipe and open it
   XPipe pipe;
   pipe.Open();

   //set the write-handle from the pipe as stderror so the client will write to it
   XPipe::DuplicateHandle( pipe1.GetWriteHandle(), hfNew);

   //start the client-process here
   XProcess::ExecuteProg(.......);

   //close write-handle of the pipe
   XPipe::CloseHandle( pipe.GetWriteHandle());

   //bring the saved handde from stderror back
   XPipe::DuplicateHandle( hfSave, hfNew);

   //close stderror
   XPipe::CloseHandle(hfSave);

   ULONG cbRead;
   char buffer[XPIPE_DEFAULTSIZE];

   do
     {
         //read data like a file
         cbRead = pipe.Read( achBuf, XPIPE_DEFAULTSIZE);
     } while(cbRead);
*/


/*@ XPipe::CloseHandle()
@group open/close
@remarks Close a single handle.
@parameters ULONG handle   handle to close.
*/


/*@ XPipe::GetWriteHandle()
@group misc
@remarks Returns the write-handle from a pipe
@returns ULONG writeHandle
*/


/*@ XPipe::GetReadHandle()
@group misc
@remarks Returns the read-handle from a pipe
@returns ULONG readHandle
*/


/*@ XPipe::Open()
@group open/close
@remarks Opens a pipe. Afte it is open you can redirect stdin/stdout/stderr to one of its handles.
@parameters LONG buffersize    size of the buffer (default is 4096)
*/
LONG XPipe::Open(const LONG size)
{
   ULONG res = DosCreatePipe(&handle, &handle2, size);
   if( res )
   {
      handle = -1;
      handle2 = -1;
   }
   return res;
}


/*@ XPipe::DuplicateHandle(const ULONG handleFrom, ULONG & handleTo)
@group misc
@remarks Dupplicate a handle.
@returns ULONG handle
*/
ULONG XPipe::DuplicateHandle(const ULONG handleFrom, ULONG & handleTo)
{
   return DosDupHandle(handleFrom, &handleTo);
}


/*@ XPipe::Close()
@group open/close
@remarks Close the pipe.
*/
void XPipe::Close()
{
   if( handle2 != -1)
      DosClose(handle2);
   handle2 = -1;
   if( handle != -1)
      DosClose(handle);
   handle = -1;
}

