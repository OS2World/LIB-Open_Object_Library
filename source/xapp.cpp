#include "xapp.h"
#include "xreslib.h"
#include "xrect.h"
#include "xframe.h"
#include "xfrmwnd.h"
#include "xstring.h"
#include "xmsgbox.h"
#include "xcountry.h"
#include "xexcept.h"
#include "xexit.h"

#include <stdlib.h>

/*@
@class XProcess
@parent XObject
@type overview
@symbol _
*/

//for emx only
#if !defined(__IBMCPP__) && !defined(__WATCOMC__)
void terminate(void)
{
}
#endif


void XProcess:: LeaveCriticSection()
{
   ULONG rc = DosExitCritSec();
   if(rc)
      OOLThrow( "Error in critical section", rc);
}


void XProcess:: EnterCriticSection()
{
   ULONG rc = DosEnterCritSec();
   if(rc)
      OOLThrow( "Error in critical section", rc);
}


/*@ XProcess :: ExecuteProg( const char * path, const char * args, const char * env, const ULONG flag, LONG * result)
@group misc
@remarks Executes a program
@parameters <t 'ø' c=2>
            øchar * path            øpath/file to execute
            øchar * args            øarguments (can be NULL)
            øchar * environment   øenviroment for the program (can be NULL)
            øULONG flags            øflags (see DosExecPgm)
            øLONG * buffer         øbuffer for return-code of the program
            </t>
@returns    LONG return-code       from the system
*/
LONG XProcess::ExecuteProg(const char *path, const char *args, const char *env, const ULONG flag, LONG * result)
{
   RESULTCODES rc;
   char error[100];
   XString buffer = path;

   char * set;
   SHORT len = strlen(path);
   if(args)
      len += strlen( args);
   len += 10;
   set = (char*) malloc( len );
   strcpy(set, path);

   char *s = set;

   s += strlen(path);
   *s = 0;
   s++;
   *s = ' ';
   s++;
   *s = 0;

   if (args)
      strcat(s, args);

   LONG ret = DosExecPgm(error, 100, flag, (PSZ) (char *) set, (PSZ) env, &rc, (PSZ) path);

   if(result)
   {
      if (ret)
         *result = ret;
      else
         *result = rc.codeResult;
   }
   free(set);
   return rc.codeTerminate;
}


/*@ XProcess :: ScanEnvironment( const char * valueToSearch, XString * buffer)
@group misc
@remarks Scan a value from the environment ( SET-entry of the config.sys )
@parameters <t 'ø' c=2>
            øchar * valueToSearch    øthe name of the value (eg. PATH)
            øXString * buffer        øbuffer to hold the data
            </t>
@returns    BOOL success
*/
BOOL XProcess::ScanEnvironment(const char *valueToSearch, XString * buffer)
{
   PSZ st = (PSZ) "";
   #if !defined( __IBMCPP__ ) & !defined( __WATCOMC__ )
   SHORT res = DosScanEnv((PSZ) valueToSearch, (unsigned char **) &st);
   #else
   SHORT res = DosScanEnv((PSZ) valueToSearch, (PCSZ *) &st);
   #endif
   if (res == 0)
      *buffer = (char *) st;
   return (res == 0) ? TRUE : FALSE;
}


/*@ XProcess :: Beep( const LONG f, const LONG d)
@group misc
@remarks Make some noise
@parameters <t 'ø' c=2>
           øLONG frequency               øfrequency in hertz
           øLONG duration                øduration in milliseconds
            </t>
*/
void XProcess::Beep(const LONG f, const LONG d)
{
   DosBeep(f, d);
}


/*@ XProcess :: Sleep( const LONG d)
@group misc
@remarks The current process is suspended for the given time
@parameters LONG duration                time to sleep in milliseconds
*/
void XProcess::Sleep(const LONG d)
{
   DosSleep(d);
}


/*@
@class XApplication
@parent XProcess
@type overview
@symbol _
@remarks XApplication initializes a stand-alone process and creates
a XResourceLibrary with the resources which are linked to the exe-file.
The XResourceLibrary can be accessed with GetResourceLibrary(). If youïve
set up your mainwindow, call Start() to run the application.<P>
An instance of XApplication is created when your programm starts, it can be accessed with XApplication::GetApplication().
<B>You must not create an instance of XApplication or a derived class.</B><P>
In your main-function you can create windows, dialog etc. Finished that you must call
XApplication::Start() so your application starts to work
@example
int main(int argc, void ** argv)
{
    //set the arguments for the case that they are used later
    XApplication::GetApplication()->SetArguments(argc, argv);

    //create a window
    //MyWindow * window = new MyWindow();

    //go!
    XApplication::GetApplication()->Start();
}
*/
class DummyApplication: public XApplication
{
   public:
      DummyApplication() {;}
};


DummyApplication oolApp;
XExitHandler * exitHandler = NULL;


/*@ XApplication :: SetExitHandler( XExitHandler * ex)
@group misc
@remarks Add an exit-handler to the application. The handlers method HandleEvent
is called when the user wants a shutdown of the system.
@parameters XExitHandler* the handler
*/
void XApplication :: SetExitHandler( XExitHandler * ex)
{
   exitHandler = ex;
}


XApplication :: XApplication()
{
   argCount = 0;
   argValues = NULL;
   windows = 0;
   frames = NULL;
   ex = FALSE;
   hab = WinInitialize(0);
   queue = WinCreateMsgQueue(hab, 100);
   resLib = new XResourceLibrary();
}


/*@ XApplication :: SetArguments( int argc, void ** argv)
@group arguments
@remarks Set the arguments which are received in the main-function. When they are set
you can query them with GetArgumentCount()/GetArgumentValue().
@parameters
<t 'ø' c=2>
øUSHORT    øCount of arguments given by main()
øvoid**   øPointer to arguments given by main()
</t>
*/
void XApplication :: SetArguments( int argc, void ** argv)
{
   if(argc > 1 && argv)
   {
      argCount = argc - 1;
      argValues = (XString**) malloc( argCount * sizeof(void*));
      for(int i = 1; i < argc; i++)
      {
          argValues[i-1] = new XString();
         *argValues[i-1] = (char*) argv[i];
      }
   }
}


/*@ XApplication :: SetResourceLibrary( const XResourceLibrary * r)
@group resources
@remarks Set a new XResourceLibrary as the actual library. All resources
will be loaded from the new library (except you use a XResource for loading).
@parameters XResourceLibrary * the new library
@example
XResourceLibrary * resourceLib;

//we use for every languag a different library
switch (language)
{
   case GERMAN:
      resourceLib = new XResourceLibrary( "ger.dll" );
      break;
   case ENGLISH:
      resourceLib = new XResourceLibrary( "eng.dll" );
      break;
   default:
      throw XException(...);
      break;
}

//we set the resource-library a the actual library for the application, all resorces
//will be loaded from this library
XApplication::GetApplication()->SetResourceLibrary( resourceLib );

//create the window with the selected library
MyAppWindow * window = new MyAppWindow();
*/
void XApplication :: SetResourceLibrary( const XResourceLibrary * r)
{
   resLib = (XResourceLibrary *) r;
}


/*@ XApplication :: GetArgumentCount()
@group arguments
@remarks Query the count of arguments that were set with SetArguments()
@returns SHORT value
*/

/*@ XApplication :: GetArgumentValue( USHORT index, XString * buffer)
@group arguments
@remarks Query an argument that was set with SetArguments()
@parameters
<t 'ø' c=2>
øUSHORT øzero-based index
øXString * øbuffer for the data
</t>
@returns BOOL success
*/
BOOL XApplication :: GetArgumentValue( USHORT index, XString * buffer)
{
   if( index <= argCount )
   {
      *buffer = *argValues[index];
      return TRUE;
   }
   else
      return FALSE;
}


/*@ XApplication :: ~XApplication()
@group constructors/destructors
@remarks Destructs the application and the resource library.
WARNING: to terminate an application, call Terminate(), never the destructor!
*/
XApplication :: ~XApplication()
{
}


/*@ XApplication :: GetResourceLibrary()
@remarks An application creates a resource library when the application is constructed
out of the resources which are binded to the exe-file. GetResourceLibrary returns
the pointer to this resource library.
@returns     XResourceLibrary *          The XResourceLibrary the application
                                         created when it starts up
*/


/*@ XApplication::GetApplication()
@remarks Returns a pointer to the one and only application-object.
@returns     XApplication *
*/
XApplication * XApplication::GetApplication()
{
   return &oolApp;
}


void XApplication::Clean(void)
{
   for (int i = 0; i < windows - 1; i++)
      delete frames[i];

   if (frames)
      free(frames);

   windows = 0;

   delete resLib;
   if(exitHandler)
      delete exitHandler;
   WinDestroyMsgQueue(queue);
   WinTerminate(hab);
}


/*@ XApplication :: Terminate( void )
@group initiate/terminate a process
@remarks With this method an application can be terminated. If you
call Terminate() the method QueryForQuit() will be called.
@fixed
*/
void XApplication::Terminate(void)
{
   ex = TRUE;
}


XProcess :: XProcess(void)
{
   tid = 0;
}


void XApplication :: AddWindow(const XWindow * w)
{
   windows += 1;
   frames = (XWindow **) realloc(frames, windows * sizeof(void *));
   frames[windows - 1] = (XWindow *) w;
}


void XApplication::RemoveWindow(const XWindow * w)
{
   SHORT i;
   BOOL swap = FALSE;
   HWND hwnd = w->GetHandle();

   for (i = 0; i < windows - 1; i++)
   {
      if (frames[i] == w)
         swap = TRUE;
      if (swap == TRUE)
         frames[i] = frames[i + 1];
   }
   windows -= 1;

   if (windows == 0)
      Terminate();
}


/*@ XApplication :: Start( void )
@group initiate/terminate a process
@remarks After a XApplication is constructed und you have
setup your code, call Start() to make the process work.
*/
void XApplication::Start(void)
{
   while (ex == FALSE)
   {
      WinGetMsg(hab, &messagequeue, 0, 0, 0);
      if (messagequeue.msg == WM_QUIT && exitHandler != NULL)
      {
         if (exitHandler->QueryForQuit() == TRUE)
            goto out;
         else
            WinCancelShutdown(queue, FALSE);
      }
      WinDispatchMsg(hab, &messagequeue);
   };
out:
   Clean();
}


/*@
@class XCountryInfo
@parent XObject
@type overview
@symbol _
*/

/*@ XCountryInfo :: XCountryInfo()
@group constructors/destructors
@remarks Query information about the country from the system
*/
XCountryInfo :: XCountryInfo()
{
   COUNTRYCODE countryCode = {0};
   ULONG infoLen = 0;

   DosQueryCtryInfo(sizeof(cInfo), &countryCode, &cInfo, &infoLen);
}


//Docs only

/*@ XCountryInfo::GetDateFormat()
@remarks  Query the format of a date value
@returns
<t 'ø' c=2>
ø0   øformat is mmddyy
ø1   øformat is ddmmyy
ø2   øformat is yymmdd
</t>
*/


/*@ XCountryInfo::GetCountry()
@remarks Query the country
@returns ID of the country:
<t 'ø' 2>
øAsian English                 ø099
øAustralia                     ø061
øBelgium                       ø032
øCanadian French               ø002
øCzechoslovakia                ø042
øDenmark                       ø045
øFinland                       ø358
øFrance                        ø033
øGermany                       ø049
øHungary                       ø036
øIceland                       ø354
øItaly                         ø039
øJapan                         ø081
øJapan  SAA                    ø081
øKorea                         ø082
øKorea  SAA                    ø082
øLatin America                 ø003
øNetherlands                   ø031
øNorway                        ø047
øPeople's Republic of China    ø086
øPoland                        ø048
øPortugal                      ø351
øSpain                         ø034
øSweden                        ø046
øSwitzerland                   ø041
øTaiwan                        ø088
øTurkey                        ø090
øUnited Kingdom                ø044
øUnited States                 ø001
øYugoslavia                    ø038
</t>
*/


/*@ XCountryInfo::XCountryInfo()
@remarks Constructor needed to query some information about the current country.
*/

/*@ XPoint::XPoint(LONG, LONG)
@remarks Construct a coordinate
@parameters LONG xPosition<BR>
LONG yPosition
*/

/*@
@class XPoint
@parent XObject
@type overview
@symbol _
*/

/*@ XPoint::GetX()
@remarks Query the x-position
@returns LONG xPosition
*/

/*@ XPoint::GetY()
@remarks Query the y-position
@returns LONG yPosition
*/

/*@ XPoint::SetX(LONG)
@remarks Set the x-position
@parameters LONG xPosition
*/

/*@ XPoint::SetY(LONG)
@remarks Set the y-position
@parameters LONG yPosition
*/


/*@
@class XRect
@parent XPoint
@type overview
@symbol _
*/

/*@ XRect::XRect(LONG, LONG, LONG, LONG)
@remarks Construct a rectangle
@parameters
<t 'ø' c=2>
øLONG øxPosition
øLONG øyPosition
øLONG øwidth
øLONG øheight
</t>
*/

/*@ XRect::SetHeight(LONG)
@remarks Set the height
@parameters LONG height
*/

/*@ XRect::SetWidth(LONG)
@remarks Set the width
@parameters LONG width
*/

/*@ XRect::GetWidth()
@remarks Query the width
@returns LONG width
*/

/*@ XRect::GetHeight()
@remarks Query the height
@returns LONG heigth
*/

/*@
@class XSize
@parent XObject
@type overview
@symbol _
*/

/*@ XSize::XSize(LONG width, LONG height)
@remarks Construct a size-description
@parameters
<t 'ø' c=2>
øLONG øwidth
øLONG øheight
</t>
*/

/*@ XSize::GetWidth()
@remarks Query the width
@returns LONG width
*/

/*@ XSize::GetHeight()
@remarks Query the height
@returns LONG heigth
*/

/*@ XSize::SetHeight(LONG)
@remarks Set the height
@parameters LONG height
*/

/*@ XSize::SetWidth(LONG)
@remarks Set the width
@parameters LONG width
*/


/*@
@class XCoordinateSystem
@parent XObject
@type overview
@symbol _
*/


/*@ XCoordinateSystem::Convert( XRect * rect, XWindow * window = NULL)
@remarks Map a coordinate to Windows/Java-Coordinates and back
@parameters
<t 'ø' c=2>
øXRect*  øinput/output coordinates. If the coordinates are in Warp-coordintates they are transformed to Windows/Java, otherwise they are transformt back
øXWindow* øWindow to calculate relative to. If NULL the coordinates are calculated relative to the Screen (default is NULL).
</t>
*/