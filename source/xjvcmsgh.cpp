#include <xheaders.h>
#include "XJavaEnv.h"
#include "XJvCMsgH.h"


/*@ XJavaConsoleMessageHandler::ShowMessage()
@group java/misc
@remarks Whenever the Java runtime system passes a string to the console, this
        function is beeing called. If you want to process console messages, you
        have to override this function.
@parameters XString& msg The console message
*/


/*@ XJavaConsoleMessageHandler::ShowConsole(void)
@group java/misc
@remarks Makes the Java console visible.
@returns An error code is return; possible values are:
                  <b>JAVA_OK</b>                  no error
                  <b>JAVA_INVALID_APPLET</b>      inavlid applet code given
                  <b>JAVA_INVALID_CONTEXT</b>     inavlid applet context given
                  <b>JAVA_INVALID_PARAMETER</b>   invalid parameter
                  <b>JAVA_ALREADY_INITIALIZED</b> Java runtime system already initialized
                  <b>JAVA_OUT_OF_MEMORY</b>       out of memory
                  <b>JAVA_EXCEPTION_OCCURRED</b>  Java exception occurred
                  <b>JAVA_NOT_INITIALIZED</b>     Java runtime system not initialized
                  <b>JAVA_INTERNAL_ERROR</b>      internal Java error
*/
int XJavaConsoleMessageHandler::ShowConsole(void)
{
	return (int) jeConsoleShow(TRUE);
}



/*@ XJavaConsoleMessageHandler::HideConsole(void)
@group java/misc
@remarks Makes the Java console invisible.
@returns An error code is return; possible values are:
                  <b>JAVA_OK</b>                  no error
                  <b>JAVA_INVALID_APPLET</b>      inavlid applet code given
                  <b>JAVA_INVALID_CONTEXT</b>     inavlid applet context given
                  <b>JAVA_INVALID_PARAMETER</b>   invalid parameter
                  <b>JAVA_ALREADY_INITIALIZED</b> Java runtime system already initialized
                  <b>JAVA_OUT_OF_MEMORY</b>       out of memory
                  <b>JAVA_EXCEPTION_OCCURRED</b>  Java exception occurred
                  <b>JAVA_NOT_INITIALIZED</b>     Java runtime system not initialized
                  <b>JAVA_INTERNAL_ERROR</b>      internal Java error
*/
int XJavaConsoleMessageHandler::HideConsole(void)
{
	return (int) jeConsoleShow(FALSE);
}
