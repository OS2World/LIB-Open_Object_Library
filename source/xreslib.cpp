#include "XRes.h"
#include "XResLib.h"
#include "XApp.h"
#include "XWindow.h"
#include "XString.h"
#include "XMsgBox.h"
#include "XIcon.h"
#include "xexcept.h"

XIcon :: ~XIcon()
{
   if (handle && loaded)
;//      WinFreeFileIcon(handle);
}


BOOL XIcon::Load(const char *p, const BOOL copy)
{
   loaded = TRUE;
   if (handle)
      WinFreeFileIcon(handle);
   handle = WinLoadFileIcon((PSZ) p, copy);
   return handle != NULLHANDLE ? TRUE : FALSE;
}


BOOL XIcon::Load(const XResource * r)
{
   handle = WinLoadPointer(HWND_DESKTOP, r->GetResourceLibrary()->GetModuleHandle(), r->GetID());
   return handle != NULLHANDLE ? TRUE : FALSE;
}


BOOL XIcon :: Load( const ULONG id)
{
   handle = WinLoadPointer(HWND_DESKTOP, XApplication::GetApplication()->GetResourceLibrary()->GetModuleHandle(), id);
   return handle != NULLHANDLE ? TRUE : FALSE;
}

/*@
@class XResource
@type overview
@symbol _
@remarks The class XResource describes resources used in an application. A resource may be an icon, a dialog template
a menubar etc. An resource is decribed in the class XResource with an ID and a library (XResourceLibrary).
The resources may reside in the EXE-file or in a resource-DLL (see XResouceLibrary for details).
*/


/*@ XResource::XResource()
@group constructors/destructors
@remarks Constructs a resource. Resources are defined by an ID and
a XResourceLibrary which contains the resource.
@parameters
<t 'ø' c=2>
øULONG øthe ID of the resource
øXResourceLibrary * øpointer to the library which contains the resource.
</t>
*/


/*@
@class XResourceLibrary
@parent XObject
@type overview
@symbol _
*/

/*@ XResourceLibrary :: XResourceLibrary(const char *path)
@group constructors/destructors
@remarks Constructs a resource library.
@parameters
char * path of the library without extension ".DLL".
                                         If the path is empty, a resource library will
                                         be created with the resources which are
                                         linked to the exe-file.
                                         If the file cannot be found an error ocures.
                                      Default is NULL.
@exceptions   If the method fails an exception of the type XException is thrown.
@updated _
*/
#include "xmsgbox.h"
XResourceLibrary :: XResourceLibrary(const char *path)
{
   char errorBuffer[100];

   moduleHandle = 0;
   LONG res;

   if (path)
   {
      if ((res = DosLoadModule((PSZ) errorBuffer, 100, (PSZ) path, &moduleHandle)) != 0)
      {
         XString s = "error loading resourcefile ";
         s += path;
         OOLThrow(s, res);
      } /* end if */
   }
}


/*@ XResourceLibrary::LoadString(XString * string, const unsigned long resId)
@group load resources
@remarks Loads a string out of the library.
@parameters
<t 'ø' c=2>
øXString * øvariable which will get the string
øULONG øthe ID of the string
</t>
@exceptions   If the method fails an exception of the type XException is thrown.
*/
BOOL XResourceLibrary::LoadString(XString * string, const unsigned long resId)
{
   char s[255];
   SHORT len;

   if ((len = WinLoadString( XApplication::GetApplication()->GetAnchorBlock(), moduleHandle, resId, 255, (PCH) s)) == 0)
      OOLThrow("error loading string from resources - string is empty or not avaible", -10);
   else
      *string = s;
   return TRUE;
}


PFNWP XResourceLibrary :: LoadFunction( char * name)
{
   return WinLoadProcedure( XApplication::GetApplication()->GetAnchorBlock(), moduleHandle, (PSZ) name);
}


BOOL XResourceLibrary :: UnLoadFunction( PFNWP pfnwp)
{
   return WinDeleteProcedure( XApplication::GetApplication()->GetAnchorBlock(), pfnwp);
}


HWND XResourceLibrary::LoadMenu(const XWindow * fr, const unsigned long id)
{
   LONG hwnd;

   if ((hwnd = WinLoadMenu(fr->GetHandle(), moduleHandle, id)) == 0)
      OOLThrow("error loading menu - menu not found", -10);

   return hwnd;
}


/*@ XResourceLibrary :: ~XResourceLibrary()
@group constructors/destructors
@remarks Destructs a resource library.
*/
XResourceLibrary :: ~XResourceLibrary()
{
   if (moduleHandle)
      DosFreeModule(moduleHandle);
}
