#include <xheaders.h>
#include "xmsgbox.h"
#include "xjavaenv.h"
#include "xjvcmsgh.h"
#include "xjvsurlh.h"
#include "xjvsmsgh.h"
#include <list.h>



static XJavaEnvironment *pJEnv = NULL;


class handler_list
{
   private:
   typedef struct
   {
      XWindow *w;
      XObject *h;
   }   item;

       list < item * >handlers;

       public:
   // ctor/dtor
       handler_list(void)
   {
   }
      ~handler_list()
   {
   }

   // add a new handler to the list
   void add_handler(XWindow * w, XObject * h)
   {
      item *i = new item;

          i->w = w;
          i->h = h;
          handlers.push_back(i);
   }

   // remove a handler from the list
   void remove_handler(XWindow * w)
   {
      list < item * >::iterator i;

      for (i = handlers.begin(); i != handlers.end(); i++)
         if ((*i)->w == w)
         {
            handlers.erase(i);
            break;
         }               // if
   }

   // returns a pointer to a handler
   XObject *get_handler(JECONTEXTID id)
   {
      list < item * >::iterator i;

      for (i = handlers.begin(); i != handlers.end(); i++)
         if ((*i)->w->GetHandle() == id)
            return (*i)->h;

      return (XObject *) NULL;
   }
};


handler_list url_handlers;
handler_list smsg_handlers;


int APIENTRY myShowConsoleMsg(char *msg)
{
   XString str(msg);

   XJavaEnvironment :: Get()->GetMessageHandler()->ShowMessage(str);

   return 0;
}



void APIENTRY myShowURL(JECONTEXTID contextID, const char *url, const char *target)
{
   XString xurl(url), xtgt(target);
   XJavaShowURLHandler *hnd = (XJavaShowURLHandler *) url_handlers.get_handler(contextID);

   hnd->ShowURL(xurl, xtgt);
}



void APIENTRY myShowStatMsg(JECONTEXTID contextID, const char *msg)
{
   XString xmsg(msg);
   XJavaStatusMsgHandler *hnd = (XJavaStatusMsgHandler *) smsg_handlers.get_handler(contextID);

   hnd->ShowStatusMessage(xmsg);
}


/*@
@class XJavaEnvironment
@parent XObject
@type overview
@symbol _
*/


/*@ XJavaEnvironment::XJavaEnvironment(XJavaConsoleMessageHandler * phnd, int argc, char **argv)
@group constructors/destructors
@remarks The constructor of XJavaEnvironment initializes the Java virtual machines.
@parameters XJavaConsoleMessageHandler* phnd Pointer to a console message handler object;
                                            if NULL is passed, then a default message handler is beeing installed.
           int argc     Number of arguments that shall be passed to the
                        Java runtime system.
           char** argv  Array with strings containing the parameters; possible
                        parameters are:
                        -v|-verbose                                             turn on verbose mode
                        -debug                                                  enable remote Java debugging
                        -noasyncgc                                              don't allow asynchronous gc's
                        -verbosegc                                              print a message when gc occur
                        -ss<em>number</em>                                      set the C stack size of a process
                        -oss<em>number</em>                                     set the JAVA stack size of a process
                        -ms<em>number</em>                                      set the initial Java heap size
                        -mx<em>number</em>                                      set the maximum Java heap size
                        -classpath <em>directories separated by semicolons</em> list directories in which to look for classes
                        -prof                                                   output profiling data to ./java.prof
                        -verify                                                 verify all classes when read in
                        -verifyremote                                           verify classes read in over the network [default]
                        -noverify                                               do not verify any class
*/
XJavaEnvironment::XJavaEnvironment(XJavaConsoleMessageHandler * phnd, int argc, char **argv):XObject()
{
   int rc;
/*
   if (NULL != pJEnv)
   {
      XMessageBox("There already exists an Java environment "
               "in this application.");
      return;
   }                     // if
*/
   // initialize java runtime system
   if ((0 != argc) && (NULL != argv))
      rc = (int) jeInitJavaRuntimeEx(argc, argv);
   else
      rc = (int) jeInitJavaRuntime();
   if (JAVA_OK != rc)
   {
      DisplayError("jeInitJavaRuntime", rc);
      return;
   }

   // register console function
   if (phnd)
      rc = SetMessageHandler(phnd);
   else
      rc = SetMessageHandler(new XJavaConsoleMessageHandler());
/*        if( JAVA_OK != rc )
        {
                DisplayError( "jeShowConsoleFnPtr", rc );
                return;
        }*/

   pJEnv = this;
}



/*@ XJavaEnvironment :: ~XJavaEnvironment()
@group constructors/destructors
@remarks Constructor for the XJavaEnvironment class.
*/
XJavaEnvironment :: ~XJavaEnvironment()
{
   pJEnv = NULL;
}



/*@ XJavaEnvironment::DisplayError(char *msg, int rc)
@group java/misc
@remarks Opens a message box and displays an error message
@parameters int rc Error number; possible values are:
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
void XJavaEnvironment::DisplayError(char *msg, int rc)
{
   XString str;

   switch (rc)
   {
   case JAVA_OK:
      return;

   case JAVA_INVALID_APPLET:
      str = "Invalid Java applet.";
      break;

   case JAVA_INVALID_CONTEXT:
      str = "Invalid Java applet context.";
      break;

   case JAVA_INVALID_PARAMETER:
      str = "Invalid parameter passed to Java.";
      break;

   case JAVA_ALREADY_INITIALIZED:
      str = "Java runtime system is already initialized";
      break;

   case JAVA_OUT_OF_MEMORY:
      str = "Out of memory.";
      break;

   case JAVA_EXCEPTION_OCCURRED:
      str = "An Java exception occurred.";
      break;

   case JAVA_NOT_INITIALIZED:
      str = "Java runtime system not initialized.";
      break;

   case JAVA_INTERNAL_ERROR:
      str = "Internal Java error.";
      break;

   default:
      str += "Unknown Java error (";
      str += (XString) rc;
      str += ")";
   }                     // switch

   XMessageBox m((char *) str, msg);
}



/*@ XJavaEnvironment::Get(void)
@group java/misc
@remarks Queries the pointer to the actual Java environment object.
@returns XJavaEnvironment* pJEnv pointer to the actual Java environment object.
*/
XJavaEnvironment * XJavaEnvironment::Get(void)
{
   return pJEnv;
}



/*@ XJavaEnvironment::Iconify(XWindow * win)
@group java/misc
@remarks Notifies the applets of a window that the window has been minimized.
@parameters XWindow* pOwnerWnd - Owner window of the applets
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
int XJavaEnvironment::Iconify(XWindow * win)
{
   return (int) jeAppletsIconify((JECONTEXTID) win->GetHandle());
}



/*@ XJavaEnvironment::Uniconify(XWindow * win)
@group java/misc
@remarks Notifies the applets of a window that the window has been restored.
@parameters XWindow* pOwnerWnd - Owner window of the applets
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
int XJavaEnvironment::Uniconify(XWindow * win)
{
   return (int) jeAppletsUniconify((JECONTEXTID) win->GetHandle());
}




/*@ XJavaEnvironment::GetMessageHandler()
@group java/misc
@remarks Queries the pointer to the actual console message handler object.
@returns XJavaConsoleMessageHandler* pMsgHandler Pointer to the actual console message handler object.
*/




/*@ XJavaEnvironment::SetMessageHandler()
@group java/misc
@remarks Sets a new console message handler object. Please remember to delete the old
        message handler object if there is one.
@parameters XJavaConsoleMessageHandler* handler  pointer to the handler object
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
int XJavaEnvironment::SetMessageHandler(XJavaConsoleMessageHandler * phnd)
{
   int rc;

   pMsgHandler = phnd;
   rc = (int) jeRegisterShowConsole((jeShowConsoleFnPtr) & myShowConsoleMsg);

   return rc;
}


/*@ XJavaEnvironment::SetShowURLHandler(XWindow * owner, XJavaShowURLHandler * hnd)
@group java/misc
@remarks Sets a new console url show handler object. Please remember to delete the old
        handler object if there is one.
@parameters XWindow* wnd  pointer to the window object that holds an applet
           XJavaShowURLHandler* hnd   pointer to the handler object
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
int XJavaEnvironment::SetShowURLHandler(XWindow * owner, XJavaShowURLHandler * hnd)
{
   url_handlers.remove_handler(owner);
   url_handlers.add_handler(owner, hnd);
   return (int) jeRegisterShowDocument((jeShowDocumentFnPtr) & myShowURL);
}


/*@ XJavaEnvironment::GetShowURLHandler(XWindow * w)
@group java/misc
@parameters XWindow* owner Pointer to the window object to which the handler belongs to.
@remarks Queries the pointer to the actual url show handler object.
@returns Pointer to the actual url show handler object.
*/
XJavaShowURLHandler * XJavaEnvironment::GetShowURLHandler(XWindow * w)
{
   return (XJavaShowURLHandler *) url_handlers.get_handler(w->GetHandle());
}




/*@ XJavaEnvironment::SetStatusMsgHandler(XWindow * owner, XJavaStatusMsgHandler * hnd)
@group java/misc
@remarks Sets a new status message handler object. Please remember to delete the old
        handler object if there is one.
@parameters XWindow* wnd  pointer to the window object that holds an applet
           XJavaStatusMsgHandler* hnd   pointer to the handler object
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
int XJavaEnvironment::SetStatusMsgHandler(XWindow * owner, XJavaStatusMsgHandler * hnd)
{
   smsg_handlers.remove_handler(owner);
   smsg_handlers.add_handler(owner, hnd);
   return (int) jeRegisterShowStatus((jeShowStatusFnPtr) & myShowStatMsg);
}


/*@ XJavaEnvironment::GetStatusMsgHandler(XWindow * w)
@group java/misc
@parameters XWindow* owner Pointer to the window object to which the handler belongs to.
@remarks Queries the pointer to the actual status message handler object.
@returns Pointer to the actual status message handler.
*/
XJavaStatusMsgHandler * XJavaEnvironment::GetStatusMsgHandler(XWindow * w)
{
   return (XJavaStatusMsgHandler *) smsg_handlers.get_handler(w->GetHandle());
}


/*@ XJavaEnvironment::GetVersion(int *piMajor, int *piMinor)
@group java/misc
@remarks Queries the version of J-Empower.
@returns int* piMajor The major version number.
        int* piMinor The Minor version number.
        An error code is return; possible values are:
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
void XJavaEnvironment::GetVersion(int *piMajor, int *piMinor)
{
   jeVersion(piMajor, piMinor);
}


/*@ XJavaEnvironment::SetProperty(const char *key, const char *value)
@group java/misc
@remarks Sets a property value of the Java runtime system.
@parameters const char* key     Name of the key (refer to the Java documentation)
                 char* value   New value that the key has
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
int XJavaEnvironment::SetProperty(const char *key, const char *value)
{
   return (int) jeSetProperty(key, value);
}


/*@ XJavaEnvironment::GetProperty(const char *key, XString * value)
@group java/misc
@remarks Queries a property value from the Java runtime system.
@parameters const char* key     Name of the key (refer to the Java documentation)
                 char* value   Value that the key has
                 int   buflen  Length of the buffer for the value
                 int*  pvallen Real length of the key value
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
int XJavaEnvironment::GetProperty(const char *key, XString * value)
{
   int result, pvallen;

   result = jeGetProperty(key, value->GetBuffer(1024), 1024, &pvallen);
   value->ReleaseBuffer(pvallen);
   return result;
}

