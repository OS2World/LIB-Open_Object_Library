#include "XFileFi.h"
#include "xfileinf.h"

#include <string.h>


/*@
@class XFileFind
@parent XObject
@type overview
@symbol _
*/

/*@ XFileFind :: XFileFind(const char *m, const LONG o)
@group constructors/destructors
@remarks Construct a XFileFind-instance (do not forget to call the destructor)
@parameters char * mask for the files to find(e.g. "*.EXE" )
*/
XFileFind :: XFileFind(const char *m)
{
   init = TRUE;
   mask = m;
   dir = HDIR_SYSTEM;
}


/*@ XFileFind::Find(XString * string, XFileInfo*)
@group find files
@remarks After you have created an instance you can start to find files. Find()
copy the filename found to the given bufer and returns TRUE, it returns FALSE if no file is found.
@parameters
<t 'ø' c=2>
øXString * øbuffer to hold the filename
øXFileInfo*  øbuffer for file-info
</t>
@returns BOOL
@updated _
*/
BOOL XFileFind::Find(XString * string, XFileInfo*info)
{
   LONG res;
   ULONG count = 1;

   memset( &info->buffer, 0, sizeof(info->buffer));
   if (init)
   {
      res = DosFindFirst((PSZ) (char *) mask, &dir, FILE_NORMAL|FILE_HIDDEN|FILE_SYSTEM|FILE_DIRECTORY, &info->buffer, sizeof(info->buffer), &count, 1);
      init = FALSE;
   }
   else
      res = DosFindNext(dir, &info->buffer, sizeof(info->buffer), &count);
   if (res == 0)
   {
      memcpy(string->GetBuffer(info->buffer.cchName), info->buffer.achName, info->buffer.cchName);
      string->ReleaseBuffer( info->buffer.cchName);
      return TRUE;
   }
   else
      return FALSE;
}


/*@ XFileFind :: ~XFileFind()
@group constructors/destructors
@remarks Destroys a XFileFind-instance. Never forget to call the destructor!
*/
XFileFind :: ~XFileFind()
{
   DosFindClose(dir);
}

