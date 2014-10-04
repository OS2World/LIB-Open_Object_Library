#include "xexcept.h"
#include "xsemex.h"
#include "xstring.h"
#include "xmsgbox.h"


/*@
@class XException
@type overview
@symbol _
@remarks The OOL throws exceptions in form of this class. You can catch this exceptions and exceptions
derived from this class in the usual way.<P>
To throw exceptions yourself you may use the macro OOLThrow with exception-text and errorcode, eg<BR>
<PRE>OOLThrow("an exception", 100)</PRE><BR>
For derived exception-classes this macro is redefined.
*/


/*@ XException :: GetErrorMessage()
@group Get information
@remarks   returns a char pointer with the error text
@returns char * errorText
*/


/*@ XException :: GetErrorCode()
@group Get information
@remarks   returns the error-code
@returns LONG theCode
*/

/*@ XException :: ShowError( XWindow * w)
@group Show information
@remarks   Displays a messagebox with information about the exception. This function should only be
used with PM-applications.
@parameters   XWindow * owner   owner window, can be NULL.
*/
void XException :: ShowError( const XWindow * w) const
{
   XString m = "Exception message:\n";
   m += msg + "\n\nErrorcode: ";
   m += (long) code;
   m += "\n\nFile: " + file + "\nLine: ";
   m += (long) line;
   XMessageBox( (char*) m, "An Exception occured!", MBOX_OK|MBOX_ERROR, w);
}


/*@ XException :: PrintError()
@group Show information
@remarks   Displays information about the exception on the commandline. This function should only be
used with VIO-applications.
*/
void XException :: PrintError(void) const
{
   XString m = "Exception message:\n";
   m += msg + "\n\nErrorcode: ";
   m += (long) code;
   m += "\n\nFile: " + file + "\nLine: ";
   m += (long) line;
   printf( "\n%s\n", (char*) m);
}

