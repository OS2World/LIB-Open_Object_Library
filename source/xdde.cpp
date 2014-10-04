#include "XWindow.h"
#include "XDDE.h"
#include "XDefHdl.h"

#include <stdlib.h>
#include <string.h>

#include "xprocess.h"

class ddehandler:public XDefaultHandler
{
      XDDE * dde;
   public:
      ddehandler(const XWindow * w, const XDDE * d):XDefaultHandler(w)
         {
            dde = (XDDE *) d;
         }
       BOOL HandleEvent(ULONG, void *, void *, LONG * retVal);
};



/*@
@class XDDE
@type overview
@symbol _
@remarks XDDE supports "dynamic data exchange". An instance of XDDE can be server or client,
it should be possible to create two or more XDDE�s for one window.
See OS/2 documentation for details.
*/


/*@ XDDE :: XDDE(const XWindow * handleFor, const BOOL s)
@group constructors/destructors
@remarks Constructs a XDDE instance
@parameters <t '�' c=2>
            �XWindow *      �the window to handle dde for. If you have set a control as a client
                                 for a framewindow with SetClient() you must give the control as the
                                 owner of the XDDE instance, not the frame window!
            �BOOL           �TRUE = server is generated, FALSE = client is generated
                                 (default is FALSE). If a server is generated XDDE will handle
                                 all events automaticaly, for clients you must call XDDE::Init()
                                 to get a connection to a server
            </t>
*/
XDDE :: XDDE(const XWindow * handleFor, const BOOL s)
{
   isServer = s;
   clients = 0;
   clientList = NULL;
   connected = FALSE;
   window = (XWindow *) handleFor;
   ack = 0;
   handler = new ddehandler(window, this);
}


/*@ XDDE::Init(char *, char*)
@group connection
@remarks Initiates a connection from a client to a server.
@parameters
<t '�' c=2>
�char *    �needed application name
�char *    �needed topic name
</t>
@returns BOOL success
*/
#include "xmsgbox.h"
BOOL XDDE::Init( char * app, char * topic)
{
   CONVCONTEXT ConvContext;

   memset(&ConvContext, 0, sizeof(CONVCONTEXT));
   ConvContext.cb = sizeof(CONVCONTEXT);
   if (!connected)
      {
         BOOL res = WinDdeInitiate(window->GetHandle(), (PSZ) app, (PSZ) topic, &ConvContext);
         return res;
      }
   else
      return TRUE;
}


/*@ XDDE::Terminate()
@group connection
@remarks Terminates the dde-connection
*/
void XDDE::Terminate()
{
   if (connected)
   {
      if (isServer)
      {
         int i;

         for (i = 0; i < clients; i++)
            if (clientList[i] != 0)
               WinDdePostMsg(clientList[i], window->GetHandle(), WM_DDE_TERMINATE, NULL, TRUE);
      }
      else
         WinDdePostMsg(server, window->GetHandle(), WM_DDE_TERMINATE, NULL, TRUE);
      connected = 0;
   }
}


/*@ XDDE :: ~XDDE()
@group constructors/destructors
@remarks The destructor of an instance is not called automaticaly, so
don�t forget to call this function. If a connection is established,
XDDE::Terminate() is called automaticaly
*/
XDDE :: ~XDDE()
{
   if (connected)
      Terminate();
   delete handler;
}


/*@ XDDE::AcceptConnection( char * app, char * topic, OOL_WINDOWHANDLE hwnd )
@group server functions
@remarks A server must allways overwrite XDDE::ConnectionRequested(). If that method
is called you can decide if you want to establish a connection. If you want to establish a
connection call this function with the needed parameters.
@parameters
<t '�' c=2>
�char *             �supported application-name
�char *             �supported topic-name
�OOL_WINDOWHANDLE �window handle of the client
</t>
@new
*/
void XDDE::AcceptConnection( char * app, char * topic, OOL_WINDOWHANDLE hwnd )
{
   CONVCONTEXT ConvContext;
   memset(&ConvContext, 0, sizeof(CONVCONTEXT));
   ConvContext.cb = sizeof(CONVCONTEXT);
   ConvContext.fsContext = 0;
   clients += 1;
   clientList = (ULONG *) realloc(clientList, clients * 4);
   clientList[clients - 1] = hwnd;
   connected = TRUE;

   WinDdeRespond( hwnd, window->GetHandle(), (PSZ) app, (PSZ) topic, &ConvContext);
}


BOOL ddehandler::HandleEvent(ULONG msg, MPARAM mp1, MPARAM mp2, LONG * retVal)
{
   switch (msg)
   {
      case WM_DDE_INITIATE:
      {
//         if (!dde->isServer)
//            break;
         PDDEINIT pDdeInit = (PDDEINIT) mp2;

         if((HWND) mp1 != dde->window->GetHandle())
         {
            SHORT i;

            for (i = 0; i < dde->clients; i++)
               if (dde->clientList[i] == (HWND) mp1)
               {
                  *retVal = FALSE;
                  return TRUE;
               }
            dde->ConnectionRequested( (char*) pDdeInit->pszAppName, (char*) pDdeInit->pszTopic, (HWND) mp1);
//            return TRUE;
         }
      }
break;
//      *retVal = FALSE;
//      return TRUE;
   case WM_DDE_INITIATEACK:
      {
         PDDEINIT pDdeInit = (PDDEINIT) mp2;
         if (((HWND) mp1 != dde->window->GetHandle()) && (!dde->connected))
         {
            if (dde->InitPossible((char *) pDdeInit->pszAppName, (char *) pDdeInit->pszTopic) == TRUE)
            {
               dde->server = (HWND) mp1;
               dde->connected = TRUE;
               dde->Connected((HWND) mp1);
               DosFreeMem(mp2);
            }
            else
               WinDdePostMsg((HWND) mp1, dde->window->GetHandle(), WM_DDE_TERMINATE, NULL, FALSE);
            *retVal = TRUE;
            return TRUE;
         }
      }
      break;
   case WM_DDE_REQUEST:
      {
         PDDESTRUCT pDdeStruct = (PDDESTRUCT) mp2;
         dde->DataRequested((char *) DDES_PSZITEMNAME(pDdeStruct), pDdeStruct->usFormat, (HWND) mp1);
      }
      break;
   case WM_DDE_EXECUTE:
      {
         PDDESTRUCT pDdeStruct = (PDDESTRUCT) mp2;
         if(dde->isServer)
            dde->DoCommand((char*) DDES_PABDATA(pDdeStruct));
      }
      break;
   case WM_DDE_ADVISE:
      {
         PDDESTRUCT pDdeStruct = (PDDESTRUCT) mp2;
         dde->AdviseRequested((char *) DDES_PSZITEMNAME(pDdeStruct), pDdeStruct->usFormat, (HWND) mp1);
      }
      break;
   case WM_DDE_UNADVISE:
      {
         dde->UnAdviseRequested((HWND) mp1);
      }
      break;
   case WM_DDE_DATA:
      {
         PDDESTRUCT pDdeStruct = (PDDESTRUCT) mp2;
         if (dde->server != (HWND) mp1)
         {
            *retVal = FALSE;
            return TRUE;
         }
         DosGetSharedMem(pDdeStruct, PAG_READ | PAG_WRITE);
         dde->DataReceived(pDdeStruct->cbData, DDES_PABDATA(pDdeStruct), (char *) DDES_PSZITEMNAME(pDdeStruct), pDdeStruct->usFormat);
      }
      break;
   case WM_DDE_ACK:
      {
         PDDESTRUCT pDdeStruct = (PDDESTRUCT) mp2;

         if (dde->server != (HWND) mp1)
         {
            *retVal = FALSE;
            return TRUE;
         }
         if (pDdeStruct)
         {
            dde->ack = pDdeStruct->fsStatus;
            DosFreeMem(pDdeStruct);
         }
         dde->Acknowledgement(dde->ack);
         *retVal = TRUE;
         return TRUE;
      }
      break;
   case WM_DDE_TERMINATE:
      {
         if (dde->connected)
         {
            WinDdePostMsg(dde->server, dde->window->GetHandle(), WM_DDE_TERMINATE, NULL, FALSE);
            if (dde->isServer)
            {
               BOOL connect = FALSE;
               int i;

               for (i = 0; i < dde->clients; i++)
               {
                  if (dde->clientList[i] == (HWND) mp1)
                     dde->clientList[i] = 0;
                  if (dde->clientList[i] != 0)
                  {
                     *retVal = TRUE;
                     connect = TRUE;
                   }
               }
               dde->connected = connect;
               if (connect == FALSE)
                  dde->DisConnected();
            }
            else
            {
               dde->connected = FALSE;
               dde->DisConnected();
            }
         }
      }
      break;
   }
   *retVal = FALSE;
   return FALSE;
}


/*@ XDDE::Advise(const char *itemName, const SHORT format)
@group data exchange
@remarks Calls the server to establish a continous data-exchange
@parameters
<t '�' c=2>
�char *  �the name of the requested item<BR>
�SHORT    �needed format of data
</t>
*/
BOOL XDDE::Advise(const char *itemName, const SHORT format)
{
   PDDESTRUCT pDdeStruct;

   if (!connected)
      return FALSE;

   pDdeStruct = MakeDDEMsg(format, itemName, NULL, 0);
   pDdeStruct->fsStatus |= DDE_FACKREQ;
   return WinDdePostMsg(server, window->GetHandle(), WM_DDE_ADVISE, pDdeStruct, TRUE);
}


/*@ XDDE::UnAdvise(const char *itemName)
@group data exchange
@remarks Calls the server to stop continous data-exchange
@parameters char * the name of the requested item
@returns BOOL success
*/
BOOL XDDE::UnAdvise(const char *itemName)
{
   PDDESTRUCT pDdeStruct;

   if (!connected)
      return FALSE;

   pDdeStruct = MakeDDEMsg(0, itemName, NULL, 0);   // DDEFMT_TEXT
   return WinDdePostMsg(server, window->GetHandle(), WM_DDE_UNADVISE, pDdeStruct, TRUE);
}


/*@ XDDE::GetData(const char *itemName, const SHORT format)
@group data exchange
@remarks Calls the server send data one time
@parameters
<t '�' c=2>
�char * �the name of the requested item
�SHORT �needed format of data
</t>
*/
BOOL XDDE::GetData(const char *itemName, const SHORT format)
{
   PDDESTRUCT pDdeStruct;

   if (!connected)
      return FALSE;

   pDdeStruct = MakeDDEMsg(format, itemName, NULL, 0);   // DDEFMT_TEXT
   return WinDdePostMsg(server, window->GetHandle(), WM_DDE_REQUEST, pDdeStruct, TRUE);
}


/*@ XDDE::SendCommand(const char *itemName, const char *theCommand)
@group data exchange
@remarks Send a command
@parameters
<t '�' c=2>
�char *       �the name of the requested item
�char *       �command to execute (max. 2000 chars)
</t>
*/
#define MACOMLENGTH 2000
BOOL XDDE::SendCommand(const char *itemName, const char *theCommand)
{
   char command[MACOMLENGTH];
   PDDESTRUCT pDdeStruct;

   if (!connected)
      return FALSE;

   ULONG length = strlen(theCommand);
   if( length >= MACOMLENGTH )
      length = MACOMLENGTH - 1;

   memcpy((char *) command, theCommand, length + 1);
   pDdeStruct = MakeDDEMsg(DDEFMT_TEXT, itemName, &command, sizeof(command));
   return WinDdePostMsg(server, window->GetHandle(), WM_DDE_EXECUTE, pDdeStruct, TRUE);
}


/*@ XDDE::PokeData(const OOL_WINDOWHANDLE handle, const char *itemName, const void *data, const ULONG s, const SHORT format)
@group data exchange
@remarks Poke data
@parameters <t '�' c=2>
            �OOL_WINDOWHANDLE     �handle of the receiver
            �char *                �the name of the requested item
            �void *                �buffer which holds data
            �ULONG                   �size of the buffer
            �SHORT                   �format of the data (default is DDEF_TEXT)
            </t>
*/
BOOL XDDE::PokeData(const OOL_WINDOWHANDLE handle, const char *itemName, const void *data, const ULONG s, const SHORT format)
{
   PDDESTRUCT pDdeStruct;
   ULONG size = s;

   if (!connected)
      return FALSE;

   if (size == 0)
   {
      if (format == DDEFMT_TEXT)
         size = strlen((char *) data) + 1;
      else
         return FALSE;
   }
   pDdeStruct = MakeDDEMsg(format, itemName, data, size);   // DDEFMT_TEXT
   return WinDdePostMsg(handle, window->GetHandle(), WM_DDE_POKE, pDdeStruct, TRUE);
}


/*@ XDDE::SendData(const OOL_WINDOWHANDLE client, const char *itemName, const void *data, const ULONG s, const SHORT format)
@group data exchange
@remarks Send data to a client
@parameters <t '�' c=2>
            �OOL_WINDOWHANDLE     �handle of the receiver
            �char *                �the name of the requested item
            �void *                �buffer which holds data
            �ULONG                �size of the buffer
            �SHORT                �format of the data (default is DDEF_TEXT)
            </t>
*/
BOOL XDDE::SendData(const OOL_WINDOWHANDLE client, const char *itemName, const void *data, const ULONG s, const SHORT format)
{
   PDDESTRUCT pDdeStruct;
   ULONG size = s;

   if (!connected)
      return FALSE;

   if (size == 0)
   {
      if (format == DDEFMT_TEXT)
         size = strlen((char *) data) + 1;
      else
         return FALSE;
   }
   pDdeStruct = MakeDDEMsg(format, itemName, data, size);   // DDEFMT_TEXT

   PID pid;
   TID tid;

   WinQueryWindowProcess(client, &pid, &tid);
   DosGiveSharedMem(&pDdeStruct, pid, PAG_READ | PAG_WRITE);

   return WinDdePostMsg(client, window->GetHandle(), WM_DDE_DATA, pDdeStruct, TRUE);
}


PDDESTRUCT XDDE::MakeDDEMsg(const USHORT usFormat, const char *itemName, const void *Data, const USHORT usDataSize)
{
   PDDESTRUCT pdde = (PDDESTRUCT) 0;
   ULONG rc = 0;
   ULONG usSegSize;
   ULONG totalsize;

   usSegSize = strlen(itemName) + 1;
   totalsize = sizeof(DDESTRUCT) + usDataSize + usSegSize;

   rc = DosAllocSharedMem((PPVOID) & pdde, (PSZ) 0, totalsize,
                     PAG_READ | PAG_WRITE | PAG_COMMIT | OBJ_TILE
                     | OBJ_GIVEABLE | OBJ_GETTABLE);

   if (rc)
      return ((PDDESTRUCT) 0);

   memset((PVOID) pdde, 0, (size_t) usSegSize);
   pdde->usFormat = usFormat;
   pdde->offszItemName = (USHORT) sizeof(DDESTRUCT);
   memcpy(DDES_PSZITEMNAME(pdde), itemName, usSegSize);
   pdde->offabData = (USHORT) sizeof(DDESTRUCT) + usSegSize;
   memcpy(DDES_PABDATA(pdde), Data, usDataSize);
   pdde->fsStatus = 0;
   pdde->cbData = (ULONG) totalsize;
   return pdde;
}


/////////////////docs only
/*@ XDDE::AdviseRequested()
@group application notification
@remarks Informs a server that an advise is requested. Override this
function if you need this information.
@parameters <t '�' c=2>
            �char *                �the name of the requested item
            �SHORT                �format of the data
            �OOL_WINDOWHANDLE     �handle of the client
            </t>
*/



/*@ XDDE::Connected()
@group application notification
@remarks Informs a server that a client has been connected. Override this
function if you need this information.
@parameters OOL_WINDOWHANDLE     handle of the client
*/

/*@ XDDE::ConnectionRequested( char * app, char * topic, OOL_WINDOWHANDLE handle)
@group Server functions
@remarks You must overwrite this method. It is called if a client wants to
establish a connection. If you want to response and establish the connection
call XDDE::AcceptConnection()
@parameters
<t '�' c=2>
�char * �required application-name
�char * �required topic-name
�OOL_WINDOWHANDLE �window-handle of the client
</t>
@new
*/

/*@ XDDE::DataReceived()
@group application notification
@remarks Informs that datas has been received. Override this
function if you need this information.
@parameters <t '�' c=2>
            �ULONG          �size of the data
            �void *             �buffer which holds the data
            �char *          �item name
            �SHORT          �format of the data
            </t>
*/

/*@ XDDE::DataRequested()
@group application notification
@remarks Informs a server that datas are needed. Override this
function if you need this information.
@parameters <t '�' c=2>
            �char *  �item name
            �SHORT    �format of the data
            �OOL_WINDOWHANDLE  �the clients handle
            </t>
*/

/*@ XDDE::DisConnected()
@group application notification
@remarks Informs that the xdde-instance has no more connections. Override this
function if you need this information.
*/


/*@ XDDE::QueryOwner()
@group misc
@remarks Returns a pointer to the owner window
@returns  XWindow *  the owner
*/


/*@ XDDE::IsConnected()
@group misc
@remarks Returns if a connection is established or not
@returns  BOOL connected
*/


/*@ XDDE::InitPossible(char*, char*)
@group connect
@remarks Informs the application that a server is ready for a DDE connect. Return TRUE for a connect, otherwise return FALSE.
On default this function returns allways TRUE, you must override this function to code the behaviour you want.
@parameters
<t '�' c=2>
�char *    �applivcation name of the possible server
�char *    �toppic-name that the possible server supports
</t>
@returns  BOOL connect
*/
