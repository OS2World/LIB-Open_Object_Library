#include "xesem.h"
#include "xsemex.h"

/*@
@class XSemaphore
@type overview
@symbol _
@parent XObject
@remarks XSemaphore is the base class for XEventSemaphore, XMutexSemaphore
*/

/*@
@class XEventSemaphore
@parent XSemaphore
@type overview
@symbol _
*/

/*@ XEventSemaphore :: XEventSemaphore(char * name, BOOL shared, BOOL isSet)
@group constructors/destructors
@remarks   Create an event-semaphore
@parameters
<t 'ø' c=2>
øchar * name øthe name of the semaphore to create WITHOUT leading '\\SEM32\\'
øBOOL shared øTRUE=shared semaphore<BR>FALSE=no shared semaphore
øBOOL isSet  øTRUE=state is posted<BR>FALSE=state is not posted<BR>(default is FALSE)
</t>
@exceptions   If the method fails an exception of the type XSemaphoreException is thrown
*/
XEventSemaphore :: XEventSemaphore(const char * name, const BOOL shared, const BOOL isSet)
{
   XString n = "\\SEM32\\";
   n += name;
   hev = 0;
   ULONG attr = 0;
   if( shared )
      attr = DC_SEM_SHARED;
   ULONG rc = DosCreateEventSem( (PSZ) (char*) n, &hev, attr, isSet);
   if(rc)
      OOLSemThrow( "cannot create XEventSemaphore", rc);
}


/*@ XEventSemaphore :: Reset()
@group misc
@remarks   resets an event-semaphore
@returns ULONG count of registered clients of the semaphore
@exceptions   If the method fails an exception of the type XSemaphoreException is thrown
*/
ULONG XEventSemaphore :: Reset()
{
   ULONG count;
   ULONG rc = DosResetEventSem( hev, &count);
   if(rc)
      OOLSemThrow( "cannot reset XEventSemaphore", rc);
   return count;
}

/*@ XEventSemaphore :: Close()
@group misc
@remarks   close an event-semaphore
@exceptions   If the method fails an exception of the type XSemaphoreException is thrown
*/
void XEventSemaphore :: Close()
{
   ULONG rc = DosCloseEventSem(hev);
   if(rc)
      OOLSemThrow( "cannot close XEventSemaphore", rc);
   hev = 0;
}


/*@ XEventSemaphore :: XEventSemaphore(char * name)
@group constructors/destructors
@remarks   open an event-semaphore which was created by another process
@parameters
char * name   the name of the semaphore to open WITHOUT leading '\\SEM32\\'
@exceptions   If the method fails an exception of the type XSemaphoreException is thrown
*/
XEventSemaphore :: XEventSemaphore(const char * name)
{
   XString n = "\\SEM32\\";
   n += name;
   hev = 0;
   ULONG rc = DosOpenEventSem( (PSZ) (char*) n, &hev);
   if(rc)
      OOLSemThrow( "cannot open XEventSemaphore", rc);
}


/*@ XEventSemaphore :: Post()
@group misc
@remarks   Post an event. All semaphores which have opened this semaphore and are waiting gets post means
they are unlocked.
@parameters
@exceptions   If the method fails an exception of the type XSemaphoreException is thrown
*/
void XEventSemaphore :: Post()
{
   ULONG rc = DosPostEventSem( hev );
   if(rc)
      OOLSemThrow( "failed to post a XEventSemaphore", rc);
}


/*@ XEventSemaphore :: Wait( LONG timeOut)
@group misc
@remarks   Wait for a semaphore until it post or until timeout.
@parameters   <t 'ø' c=2>
   øLONG timeout   ø-1=wait endless<BR> 0 = return at once<BR> other=time to wait max. (in milliseconds)
   </t>
@exceptions   If the method fails an exception of the type XSemaphoreException is thrown
*/
void XEventSemaphore :: Wait( const LONG timeOut)
{
   ULONG rc = DosWaitEventSem( hev, timeOut);
   if(rc)
      OOLSemThrow( "failed to wait for XEventSemaphore", rc);
}


XEventSemaphore :: ~XEventSemaphore()
{
   if(hev)
      Close();
}
