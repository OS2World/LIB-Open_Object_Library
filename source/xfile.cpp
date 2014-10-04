#include "XFile.h"
#include "XFileInf.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*@
@class XIO
@parent XObject
@type overview
@symbol _
*/

/*@
@class XFile
@parent XIO
@type overview
@symbol _
*/

ULONG XFile :: SetMaxFiles( ULONG handles )
{
   return DosSetMaxFH(handles);
}


/*@ XFile::GetFileInfo(XFileInfo * info)
@group misc
@remarks Query information about files. To access information the
file must be open and XFILE_SHARE_DENYWRITE must be set!
@parameters XFileInfo * pointer to an instance of XFileInfo
@returns    LONG                    result of the operatingsystem
*/
LONG XFile::GetFileInfo(XFileInfo * info)
{
   FILESTATUS3 buffer;

   LONG res = DosQueryFileInfo(handle, 1, &buffer, sizeof(buffer));
   if(res == 0)
   {
      char * p = (char*) &info->buffer;
      p += 4;
      memcpy( p, &buffer, sizeof(buffer));
   }
   return res;
}


LONG XFile::GetPathInfo( const char * path, XFileInfo * info)
{
   FILESTATUS3 buffer;

   LONG res = DosQueryPathInfo( (PSZ) path, 1, &buffer, sizeof(buffer));
   if(res == 0)
   {
      char * p = (char*) &info->buffer;
      p += 4;
      memcpy( p, &buffer, sizeof(buffer));
   }
   return res;
}


void XEAList :: ClearList()
{
   for(int i = 0; i <count; i++)
      delete list[i];
   if(list)
      free (list);
   count = 0;
   list = NULL;
}


XEA :: ~XEA()
{
   if(value)
      free(value);
}


/*@ XFile::GetEAList ( char*, XEAList* )
@group EAs
@parameters
<t 'ø' c=2>
øchar * fileName  øname of the file
øXEAList* list    øbuffer of type XEAList
</t>
@returns ULONG result
@remarks Read all EAs of a file.
*/
ULONG XFile :: GetEAList( const char * path, XEAList * eaList)
{
   UCHAR buffer[500] = {0};
   ULONG ulEnumCnt = 0;
   FEA2 *ptr = NULL;
   ULONG  ulTemp = 0;
   ULONG rc;
   ulEnumCnt = (ULONG)-1;

   rc = DosEnumAttribute(ENUMEA_REFTYPE_PATH, (PSZ) (char*) path, 1L, (PVOID)&buffer, sizeof(buffer), &ulEnumCnt, ENUMEA_LEVEL_NO_VALUE);

   if (rc == 0)
   {
      eaList->ClearList();
      eaList->count = ulEnumCnt;
      eaList->list = (XEA**) malloc( ulEnumCnt * sizeof(void*));
      ptr = (FEA2 *)buffer;
      for ( int i = 0; i < ulEnumCnt; i++)
      {
         eaList->list[i] = new XEA();
         eaList->list[i]->name = ptr->szName;
         ulTemp = ptr->oNextEntryOffset + (ULONG)ptr;
         ptr = (FEA2 *)ulTemp;
      }
   }
   else
      return rc;

   for( int i = 0; i < ulEnumCnt; i++)
   {
      rc = XFile::GetEA( path, eaList->list[i], eaList->list[i]->name);
      if(rc)
         return rc;
   }
   return 0;
}


/*@ XFile::SetEAList ( char*, XEAList* )
@group EAs
@parameters
<t 'ø' c=2>
øchar* filename   øname of the file
øXEAList* list    ølist of EAs to save
</t>
@returns ULONG result
@remarks Save a list of EAs to a file
*/
ULONG XFile :: SetEAList( const char * path, const XEAList * list)
{
   for(int i = 0; i < list->GetEACount(); i++)
   {
      ULONG rc = SetEA( path, list->GetEA(i));
      if(rc)
         return rc;
   }
   return 0;
}


/*@ XFile::GetEA ( char* fileName, XEA*ea, char* eaName)
@group EAs
@parameters
<t 'ø' c=2>
øchar * fileName øname of the file
øXEA * ea        øbuffer to a XEA
øchar * eaName   øname of the EA to read
</t>
@returns ULONG result
@remarks Read a single EA-entry of a file
*/
ULONG XFile :: GetEA( const char * path, XEA * ea, const char * name)
{
   EAOP2 eaop;
   eaop.fpGEA2List = (GEA2LIST*)new char[500];
   eaop.fpFEA2List = (FEA2LIST*)new char[65536];

   eaop.oError = 0;
   eaop.fpGEA2List->list[0].oNextEntryOffset = 0;
   eaop.fpGEA2List->list[0].cbName = (BYTE) strlen(name);
   strcpy(eaop.fpGEA2List->list[0].szName, name);

   eaop.fpGEA2List->cbList = strlen(name)
    + sizeof(eaop.fpGEA2List->list[0].oNextEntryOffset)
    + sizeof(eaop.fpGEA2List->list[0].cbName)
    + sizeof(eaop.fpGEA2List->cbList);
   if (eaop.fpGEA2List->cbList % 4)
      eaop.fpGEA2List->cbList+= 4-(eaop.fpGEA2List->cbList % 4);

   eaop.fpFEA2List->cbList = 65536;

   ULONG rc = DosQueryPathInfo((PCSZ) path, FIL_QUERYEASFROMLIST, &eaop, sizeof(eaop));
   if (rc)
   {
      delete eaop.fpGEA2List;
      delete eaop.fpFEA2List;
      return rc;
   }
   ea->name = eaop.fpFEA2List->list[0].szName;
   ea->type = eaop.fpFEA2List->list[0].fEA;
   char * p = eaop.fpFEA2List->list[0].szName + eaop.fpFEA2List->list[0].cbName + 1;
   ea->valueSize = eaop.fpFEA2List->list[0].cbValue;
   if( ea->valueSize)
   {
      ea->value = malloc(eaop.fpFEA2List->list[0].cbValue);
      memcpy( ea->value, p, eaop.fpFEA2List->list[0].cbValue);
   }
   delete eaop.fpGEA2List;
   delete eaop.fpFEA2List;
   return 0;
}


/*@ XFile::SetEA ( char*, XEA*)
@group EAs
@parameters
<t 'ø' c=2>
øchar * fileame   øname of the file
øXEA* ea          øbuffer with EA to save
</t>
@returns ULONG
@remarks Save an EA. If the EA exists it will be replaced otherwise added to the EA-list of the file.
<p>An EA can also removed from the file's EA-list when you set valueSize to zero of the EA to remove.
*/
ULONG XFile :: SetEA( const char * path, const XEA * ea)
{
   EAOP2 eaop;
   memset(&eaop, 0, sizeof(eaop));
   eaop.fpFEA2List = (FEA2LIST*)new char[65536];

   eaop.fpFEA2List->list[0].fEA = ea->type;
   eaop.fpFEA2List->list[0].oNextEntryOffset = 0;
   eaop.fpFEA2List->list[0].cbValue = ea->valueSize;
   eaop.fpFEA2List->list[0].cbName = ea->name.GetLength();
   char * x = eaop.fpFEA2List->list[0].szName + eaop.fpFEA2List->list[0].cbName+1;
   eaop.fpFEA2List->cbList = eaop.fpFEA2List->list[0].cbValue + x-(char*)eaop.fpFEA2List;
   if (eaop.fpFEA2List->cbList % 4)
      eaop.fpFEA2List->cbList+= 4-(eaop.fpFEA2List->cbList % 4);

   strcpy(eaop.fpFEA2List->list[0].szName, (char*) ea->name);
   char * p = eaop.fpFEA2List->list[0].szName + eaop.fpFEA2List->list[0].cbName + 1;
   if( ea->valueSize)
      memcpy( p, ea->value, ea->valueSize);

   ULONG rc = DosSetPathInfo((PCSZ) path, FIL_QUERYEASIZE, &eaop, sizeof(eaop), DSPI_WRTTHRU);
   delete eaop.fpGEA2List;
   delete eaop.fpFEA2List;
   return rc;
}

/*@ XFile::SetFileInfo(XFileInfo * info)
@group misc
@remarks Set file-information. To access information the
file must be open with XFILE_READONLY and XFILE_SHARE_DENYWRITE must be set!
@parameters XFileInfo * pointer to an instance of XFileInfo
@returns    LONG                    result of the operatingsystem
*/
LONG XFile::SetFileInfo( const XFileInfo * info)
{
   FILESTATUS3 buffer;

   char * p = (char*) &info->buffer;
   p += 4;
   memcpy( &buffer, p, sizeof(buffer));

   return DosSetFileInfo(handle, FIL_STANDARD, &buffer, sizeof(buffer));
}


LONG XFile::SetPathInfo( const char * path, const XFileInfo * info)
{
   FILESTATUS3 buffer;

   char * p = (char*) &info->buffer;
   p += 4;
   memcpy( &buffer, p, sizeof(buffer));

   return DosSetPathInfo( (PSZ) path, FIL_STANDARD, &buffer, sizeof(buffer), DSPI_WRTTHRU);
}


/*@ XFile::Open(const char *filename, const ULONG modeifexist, const ULONG modeopen, const ULONG size, const EAOP2 * ealist)
@group open/close
@remarks Open a file
@parameters <t 'ø' c=2>
            øchar * path                  øthe path of the file
            øULONG modeForOpen            øhow to open, possible values are:
<BR>
XFILE_REPLACE_EXISTING override existing filea
<BR>
XFILE_OPEN_EXISTING open if file exists
<BR>
XFILE_FAIL_EXISTING cancel if the file exists
<BR>
XFILE_FAIL_IF_NEW cancel if the file doesnït exist
<BR>
XFILE_CREATE_IF_NEW  create a new file if it doesnït exist
<BR>
                                         (can be or-ed)
            øULONG accessMode             ømode for access, possible values are:
<BR>
XFILE_READONLY
<BR>
XFILE_WRITEONLY
<BR>
XFILE_READWRITE
<BR>
                                         (can be or-ed)
            øULONG shareMode             ømode for file-sharing, possible values are:
<BR>
XFILE_SHARE_DENYREAD
<BR>
XFILE_SHARE_DENYWRITE
<BR>
XFILE_SHARE_DENYREADWRITE
<BR>
XFILE_SHARE_DENYNONE
<BR>
                                         (can be or-ed)
            øULONG size                   øsize to open (only if a file is created, default is 0)
            øEAOP2 * eaList               ølist with extended attributes (default is NULL)
            </t>
@returns    ULONG                        result returned by the OS
@updated _
@also XEA, XEAList, XPipe, XNamedPipe, XSocket
*/
ULONG XFile::Open(const char *filename, const ULONG modeifexist, const ULONG modeopen, const ULONG modeAccess, const ULONG size, const EAOP2 * ealist)
{
   ULONG aktion, res;

   res = DosOpen((PSZ) filename, &handle, &aktion, size, 0, modeifexist, modeopen|modeAccess, (PEAOP2) ealist);
   if( res )
   {
      handle = -1;
      isOpen = FALSE;
   } /* end if */
   else
      isOpen = TRUE;
   return res;
}


/*@ XFile::IsDriveAvaible(const char drive)
@group disks
@remarks Query if a drive is avaible or not
@parameters char drive     the drive (A, B, ....)
@returns    BOOL result
*/
BOOL XFile::IsDriveAvaible(const char drive)
{
   SHORT number = drive - 'A';
   ULONG u1 = 0, u2 = 0;

   DosQueryCurrentDisk(&u1, &u2);
   return ((u2 << (31 - number) >> 31) ? TRUE : FALSE);
}


/*@ XFile::GetCurrentDirectory(XString * buffer)
@group directorys
@remarks Query the current directory
@parameters XString * buffer to hold the data
*/
void XFile::GetCurrentDirectory(XString * buffer)
{
   ULONG r = 512;

   DosQueryCurrentDir(0, (PCH) buffer->GetBuffer(512), &r);
   buffer->ReleaseBuffer();
}


/*@ XFile::SetDefaultDisk(const char drive)
@group disks
@remarks Set the dault disk
@parameters
<t 'ø' c=2>
øchar øthe drive (A, B, ....)
</t>
*/
BOOL XFile::SetDefaultDisk(const char drive)
{
   char d = toupper(drive);
   USHORT number = d - 'A' + 1;

   if (DosSetDefaultDisk(number) == 0)
      return TRUE;
   else
      return FALSE;
}


/*@ XFile::GetCurrentDisk(char &buffer)
@group disks
@remarks Query the current disk
@parameters char&  buffer to hold data (will contain A,B,..)
*/
void XFile::GetCurrentDisk(char &buffer)
{
   ULONG u1, u2;

   DosQueryCurrentDisk(&u1, &u2);
   buffer = (char) u1 + 'A' - 1;
}


/*@ XFile::CreateDirectory(const char *path)
@group directorys
@remarks Creates a directory
@parameters char * path of the directory to create
*/
ULONG XFile::CreateDirectory(const char *path)
{
   return DosCreateDir((PSZ) path, NULL);
}


/*@ XFile::DeleteDirectory(const char *path)
@group directorys
@remarks Delete a directory
@parameters char * path of the directory to delete
*/
ULONG XFile::DeleteDirectory(const char *path)
{
   return DosDeleteDir((PSZ) path);
}

////////////////docs only
/*@ XFileDialog::GetCommand()
@group misc
@remarks Returns the action of the user
@returns    SHORT                        result: USER_OK or USER_CANCEL
*/


/*@ XFileDialog::GetFileCount()
@group misc
@remarks Returns the number of files selected
@returns    SHORT                        count of files
*/

/*@ XIO::Read(PVOID, ULONG)
@group I/O
@definition Read(PVOID, ULONG)
@remarks Read from I/O
@parameters PVOID buffer                 pointer to memory
<BR>
            ULONG size                   count of bytes to read
@returns    ULONG                        count of bytes which are read
*/


/*@ XIO::Read( XString&, ULONG)
@group I/O
@definiton Read( XString&, ULONG)
@remarks Read from I/O
@parameters XString&                 string-buffer
<BR>
            ULONG size                   count of bytes to read
@returns    ULONG                        count of bytes which are read
@fixed
*/


/*@ XIO::Read(LONG&)
@group I/O
@definiton Read(LONG&)
@remarks Read from I/O
@parameters LONG&            buffer
@returns    ULONG                        count of bytes which are read
*/

/*@ XIO::Read(SHORT&)
@definiton Read(SHORT&)
@group I/O
@remarks Read from I/O
@parameters SHORT&            buffer
@returns    ULONG                        count of bytes which are read
*/


/*@ XIO::Read(CHAR&)
@definiton Read(CHAR&)
@group I/O
@remarks Read from I/O
@parameters CHAR&            buffer
@returns    ULONG                        count of bytes which are read
*/


/*@ XIO::Read(XTime&)
@definiton Read(XTime&)
@group I/O
@remarks Read from I/O
@parameters XTime&            buffer
@returns    ULONG                        count of bytes which are read
*/


/*@ XIO::Read(XDate&)
@definiton Read(XDate&)
@group I/O
@remarks Read from I/O
@parameters XDate&            buffer
@returns    ULONG                        count of bytes which are read
*/


/*@ XIO::Write(PVOID, ULONG)
@group I/O
@remarks Write to I/O
@parameters PVOID buffer                 pointer to memory
<BR>
            ULONG size                   count of bytes to write
@returns    ULONG                        count of bytes which are written
*/

/*@ XIO::Write(XString&)
@group I/O
@remarks Writes a string to I/O including terminating NULL.
@parameters XString&            string
@returns    ULONG                        count of bytes which are written
*/

/*@ XIO::Write(LONG)
@group I/O
@remarks Write LONG to I/O
@parameters LONG             data
@returns    ULONG                        count of bytes which are written
*/

/*@ XIO::Write(short)
@group I/O
@remarks Write SHORT to I/O
@parameters SHORT         data
@returns    ULONG                        count of bytes which are written
*/

/*@ XIO::Write(char)
@group I/O
@remarks Write CHAR to I/O
@parameters CHAR         data
@returns    ULONG                        count of bytes which are written
*/

/*@ XIO::Write(XTime&)
@group I/O
@remarks Write a time to I/O
@parameters XTime&         data in OOL-format
@returns    ULONG                        count of bytes which are written
*/


/*@ XIO::Write(XDate&)
@group I/O
@remarks Write a date to I/O
@parameters XDate&         data in OOL-format
@returns    ULONG                        count of bytes which are written
*/


/*@ XIO::Close()
@group open/close
@remarks Close I/O
*/


/*@ XIO::GetPointerPos()
@group misc
@remarks Returns the position of the file-pointer relative to the beginning of I/O. Before calling
this function you must call XIO::Seek()! (e.g. Seek(0, XFILE_CURRENT) )
@returns    ULONG                        the position
*/


/*@ XIO::Seek()
@group misc
@remarks Seek in the file
@parameters <t 'ø' c=2>
            øULONG position               øposition to seek to relative to relPos
            øULONG relPos                 øposition relative to:
<BR>
XFILE_BEGIN  relative to the beginning of the file (default)
<BR>
XFILE_CURRENT relative to current position
<BR>
XFILE_END       relative to the end of the file
            </t>
@returns    ULONG                        count of bytes which are read
*/


/*@ XFile::Remove()
@group misc
@remarks Delete a file
@parameters char * fileName   name of the file to delete (including path if nessecary)
@returns    BOOL success
*/


/*@ XFile::Copy()
@group misc
@remarks Copy a file
@parameters char * from    file to copy (including path if nessecary)<BR>
            char * to      destination (including path if nessecary)
@returns    ULONG result   returncode from the OS.
*/


/*@ XFile::Move()
@group misc
@remarks Move a file
@parameters char * from    file to copy (including path if nessecary)<BR>
            char * to      destination (including path if nessecary). If the same directory is given,
                           Move() renames the file.
@returns    ULONG result   returncode from the OS.
*/


/*@ XFile::ResetBuffer()
@group misc
@remarks Wait until all writen datas are physicaly saved
@returns    ULONG result   returncode from the OS.
*/


