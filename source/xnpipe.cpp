#include "XNPipe.h"
#include "XcPipe.h"

/*@
@class XNamedPipeServer
@parent XIO
@type overview
@symbol _
*/


/*@
@class XNamedPipeClient
@parent XIO
@type overview
@symbol _
*/


/*@ XNamedPipeServer::GetState(void)
@group misc
@remarks Query the state of a pipeserver
@returns <t '�' c=2>
�LONG �state, possible values:<BR>
XNPIPE_STATE_DISCONNECTED<BR>
XNPIPE_STATE_LISTENING<BR>
XNPIPE_STATE_CONNECTED<BR>
XNPIPE_STATE_CLOSING<BR>
</t>
*/
LONG XNamedPipeServer::GetState(void)
{
   ULONG state, buffer, buffer2;
   AVAILDATA avail;

   DosPeekNPipe(handle, &buffer, 4, &buffer2, &avail, &state);
   return state;
}


LONG XNamedPipeClient::GetState(void)
{
   ULONG state, buffer, buffer2;
   AVAILDATA avail;

   DosPeekNPipe(handle, &buffer, 4, &buffer2, &avail, &state);
   return state;
}


/*@ XNamedPipeServer::Open(char *name, ULONG openMode, ULONG pipeMode, char pipeCount, LONG outSize, long inSize, ULONG timeOut)
@group open/close
@remarks Open a pipe, after you have opened it call Connect()
@parameters <t '�' c=2>
            �char *     �pipename, without leading '\\PIPE\\'
            �ULONG   �mode to open (see OS/2 docs):<BR>
XNPIPE_ACCESS_INBOUND<BR>
XNPIPE_ACCESS_OUTBOUND<BR>
XNPIPE_ACCESS_DUPLEX<BR>
XNPIPE_INHERIT<BR>
XNPIPE_NOINHERIT<BR>
XNPIPE_WRITEBEHIND<BR>
XNPIPE_NOWRITEBEHIND<BR>
            �ULONG   �mode of data-transfer:<BR>
XNPIPE_READMODE_BYTE<BR>
XNPIPE_READMODE_MESSAGE<BR>
XNPIPE_TYPE_BYTE<BR>
XNPIPE_TYPE_MESSAGE<BR>
XNPIPE_WAIT<BR>
XNPIPE_NOWAIT<BR>
            �char   �maximum instances of pipe-servers, -1 = unlimited (default is 1)
            �ULONG   �size of write-buffer
            �ULONG   �size of read-buffer
            �ULONG   �time to wait for clients
         </t>
*/
LONG XNamedPipeServer::Open(char *name, ULONG openMode, ULONG pipeMode, char pipeCount, LONG outSize, long inSize, ULONG timeOut)
{
   XString n = "\\PIPE\\";
   n += name;
   pipeMode |= pipeCount;
   ULONG rc = DosCreateNPipe((PSZ) (char*) n, &handle, openMode, pipeMode, outSize, inSize, timeOut);

   if (rc == 0)
      isOpen = TRUE;
   else
      handle = -1;
   return rc;
}


/*@ XNamedPipeServer::Connect(void)
@group open/close
@remarks Wait for a client
*/
LONG XNamedPipeServer::Connect(void)
{
   return DosConnectNPipe(handle);
}


/*@ XNamedPipeServer::DisConnect(void)
@group open/close
@remarks Frees a pipe when a client has stoped data-transfer ( if GetState() returns XNPIPE_STATE_CLOSING).
After DisConnect() you can call Connect() to wait for the next client or Close().
*/
LONG XNamedPipeServer::DisConnect(void)
{
   ULONG res = 0;

   if (isOpen)
      res = DosDisConnectNPipe(handle);
   return res;
}


/*@ XNamedPipeClient::WaitForServer(char *name, ULONG timeOut)
@group misc
@remarks Wait for a server
@parameters <t '�' c=2>
            �char * �name of the requested pipe (without leading '\\PIPE\\')
            �ULONG �how long to wait
            �char * �name of the server-maschine if the pipe is not on the local maschine<BR>(default is NULL)
            </t>
*/
LONG XNamedPipeClient::WaitForServer(char *name, ULONG timeOut, char * server)
{
   XString n;
   if(server)
   {
      n += "\\";
      n += server;
   }
   n += "\\PIPE\\";
   n += name;
   return DosWaitNPipe((PSZ) (char*) n, timeOut);
}


/*@ XNamedPipeClient::Open(char *name, ULONG modeopen)
@group open/close
@remarks Opens a connection, after it is open call WaitForServer()
@parameters <t '�' c=2>
            �char * �name of the requested pipe (without leading '\\PIPE\\'
            �ULONG �mode to open (see XFile::Open() for possible modes
            �char * �name of the server-maschine if the pipe is not on the local maschine<BR>(default is NULL)
            </t>
*/
LONG XNamedPipeClient::Open(char *name, ULONG modeopen, char * server)
{
   ULONG aktion;
   XString n;
   if(server)
   {
      n += "\\";
      n += server;
   }
   n += "\\PIPE\\";
   n += name;
   ULONG rc = DosOpen((PSZ) (char*) n, &handle, &aktion, 0, FILE_NORMAL, FILE_OPEN, modeopen, NULL);
   if( rc )
      handle = -1;
   return rc;
}

