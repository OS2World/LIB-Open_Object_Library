#include <xheaders.h>
#include "XJavaApp.h"

/*@
@class XJavaApplet
@parent XObject
@type overview
@symbol _
*/

/*@ XJavaApplet :: XJavaApplet(XWindow * owner, XRect * rect, const char *documentURL, const char *code, USHORT id, int parmCount, const char **parms)
@group constructors/destructors
@remarks The constructor of XJavaApplet initializes a Java applet.
@parameters       XWindow* owner
                 XRect*   rect
           const char*    documentUrl
           const char*    code
                 USHORT   id
                 int      parmCount
           const char**   parms
*/
XJavaApplet :: XJavaApplet(XWindow * owner, XRect * rect, const char *documentURL, const char *code, USHORT id, int parmCount, const char **parms):Owner(owner), ID(id), XControl(0)
{
   int rc;

   // get size of owner window
   XRect r;

   owner->GetSize(&r);
   long w = (rect ? rect->GetWidth() : r.GetWidth()), h = (rect ? rect->GetHeight() : r.GetHeight());

   // initialize applet
   rc = (int) jeAppletInit(&hApplet,
                     (JECONTEXTID) owner->GetHandle(),
                     (JENATIVEWND) owner->GetHandle(),
                     documentURL, code,
                     w, h,
                     parmCount, parms);
   if (JAVA_OK != rc)
   {
      XJavaEnvironment :: DisplayError("jeAppletInit", rc);
      return;
   }                     //if

   // initialize control
   winhandle = (HWND) jeHWNDFromHandle(hApplet);

   // Resize applet
   if (rect)
      SetSize(rect);
   else
   {
      XRect r;

      owner->GetSize(&r);
      SetSize(&r);
   }                     // else
}



/*@ XJavaApplet::~XJavaApplet()
@group constructors/destructors
@remarks The destructor of XJavaApplet destroys the a Java applet.
*/
XJavaApplet::~XJavaApplet()
{
   int rc;


   rc = Destroy();
   if (JAVA_OK != rc)
   {
      XJavaEnvironment::DisplayError("jeAppletDestroy", rc);
      return;
   }                     // if
}



/*@ XJavaApplet::Start(void)
@group java/misc
@remarks Starts the Java applet.
@returns An error code is returned; possible values are:
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
int XJavaApplet::Start(void)
{
   return (int) jeAppletStart(hApplet);
}



/*@ XJavaApplet::Stop(void)
@group java/misc
@remarks Stops the Java applet.
@returns An error code is returned; possible values are:
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
int XJavaApplet::Stop(void)
{
   return (int) jeAppletStop(hApplet);
}


/*@ XJavaApplet::Destroy(void)
@group java/misc
@remarks Destroys the Java applet.
@returns An error code is returned; possible values are:
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
int XJavaApplet::Destroy(void)
{
   int rc = (int) jeAppletDestroy(hApplet);

   if( JAVA_INVALID_APPLET == rc )
   rc = JAVA_OK;

   return rc;
}
