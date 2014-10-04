#include "xmusem.h"
#include "xsemex.h"


/*@
@class XMutexSemaphore
@parent XSemaphore
@type overview
@symbol _
*/

/*@ XMutexSemaphore :: XMutexSemaphore(char * name, BOOL shared, BOOL owned)
@group constructors/destructors
@remarks Create a XMutexSemaphore
@parameters
<t 'ø' c=2>
øchar * name         øname of the semaphore WITHOUT leading '\\SEM32'
øBOOL shared         øif shared or not
øBOOL owned            øTRUE=initial state is owned<BR>FALSE=initial state is not owned
</t>
@exceptions   If the method fails an exception of the type XSemaphoreException is thrown
*/
XMutexSemaphore :: XMutexSemaphore(char * name, BOOL shared, BOOL owned)
{
   XString n = "\\SEM32\\";
   n += name;
   hev = 0;
   LONG attr = 0;
   if(shared)
      attr |= DC_SEM_SHARED;
   ULONG rc = DosCreateMutexSem( (PSZ) (char*) n, &hev, attr, owned);
   if( rc )
      OOLSemThrow( "Cannot create XMutexSemaphore", rc);
}


XMutexSemaphore :: ~XMutexSemaphore()
{
   if(hev)
      Close();
}


/*@ XMutexSemaphore :: Close()
@group misc
@remarks   Crlose a XMutexSemaphore
@exceptions   If the method fails an exception of the type XSemaphoreException is thrown
*/
void XMutexSemaphore :: Close()
{
   ULONG rc = DosCloseMutexSem( hev);
   if( rc )
      OOLSemThrow( "Cannot close XMutexSemaphore", rc);
   hev = 0;
}


/*@ XMutexSemaphore :: XMutexSemaphore(char * name)
@group constructors/destructors
@remarks   Open a XMutexSemaphore
@parameters char * name      name of the semaphore WITHOUT leading '\\SEM32'
@exceptions   If the method fails an exception of the type XSemaphoreException is thrown
*/
XMutexSemaphore :: XMutexSemaphore(char * name)
{
   XString n = "\\SEM32\\";
   n += name;
   hev = 0;
   ULONG rc = DosOpenMutexSem( (PSZ) (char*) n, &hev);
   if( rc )
      OOLSemThrow( "Cannot open XMutexSemaphore", rc);
}


/*@ XMutexSemaphore :: Release()
@group misc
@remarks   Release a XMutexSemaphore, the next semaphore get access.
@exceptions   If the method fails an exception of the type XSemaphoreException is thrown
*/
void XMutexSemaphore :: Release()
{
   ULONG rc = DosReleaseMutexSem( hev);
   if( rc )
      OOLSemThrow( "XMutexSemaphore::Release failed", rc);
}


/*@ XMutexSemaphore :: Request(LONG timeOut)
@group misc
@remarks   Request a semaphore.
@parameters   <t 'ø' c=2>
   øLONG timeout   ø-1=wait endless<BR> 0 = return at once<BR> other=time to wait max. (in milliseconds)
   </t>
@exceptions   If the method fails an exception of the type XSemaphoreException is thrown
*/
void XMutexSemaphore :: Request(LONG timeOut)
{
   ULONG rc = DosRequestMutexSem( hev, timeOut);
   if( rc )
      OOLSemThrow( "XMutexSemaphore::Request failed", rc);
}

