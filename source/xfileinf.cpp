#include "xfileinf.h"
#include "XDate.h"
#include "XTime.h"


/*@
@class XFileInfo
@parent XObject
@type overview
@symbol _
*/

/*@ XFileInfo::IsReadonly()
@group file attributes
@remarks Query if the file is readonly
@returns BOOL
*/

/*@ XFileInfo::IsDirectory()
@group file attributes
@remarks Query if the file is a directory
@returns BOOL
*/

/*@ XFileInfo::IsHidden()
@group file attributes
@remarks Query if the file is hidden
@returns BOOL
*/

/*@ XFileInfo::IsSystem()
@group file attributes
@remarks Query if the file is a system-file
@returns BOOL
*/

/*@ XFileInfo::IsArchived()
@group file attributes
@remarks Query if the archived-bit is on
@returns BOOL
*/

/*@ XFileInfo::GetCreationDate(XDate * date)
@group get information
@remarks Query the date when the file was created.
@parameters  XDate * buffer    buffer to hold the data
*/
void XFileInfo::GetCreationDate(XDate * date) const
{
   date->d.days = buffer.fdateCreation.day;
   date->d.months = buffer.fdateCreation.month;
   date->d.years = buffer.fdateCreation.year + 1980;
}


void XFileInfo::SetCreationDate( const XDate * date)
{
   buffer.fdateCreation.day = date->d.days;
   buffer.fdateCreation.month = date->d.months;
   buffer.fdateCreation.year = date->d.years - 1980;
}


void XFileInfo::SetCreationTime( const XTime * time)
{
   buffer.ftimeCreation.twosecs = time->t.seconds / 2;
   buffer.ftimeCreation.minutes = time->t.minutes;
   buffer.ftimeCreation.hours = time->t.hours;
}

void XFileInfo::SetLastWriteDate( const XDate * date)
{
   buffer.fdateLastWrite.day = date->d.days;
   buffer.fdateLastWrite.month = date->d.months;
   buffer.fdateLastWrite.year = date->d.years - 1980;
}


void XFileInfo::SetLastWriteTime( const XTime * time)
{
   buffer.ftimeLastWrite.twosecs = time->t.seconds / 2;
   buffer.ftimeLastWrite.minutes = time->t.minutes;
   buffer.ftimeLastWrite.hours = time->t.hours;
}


/*@ XFileInfo::GetCreationTime(XTime * time)
@group get information
@remarks Query the time when the file was created.
@parameters  XTime * buffer    buffer to hold the data
*/
void XFileInfo::GetCreationTime(XTime * time) const
{
   time->t.seconds = buffer.ftimeCreation.twosecs * 2;
   time->t.minutes = buffer.ftimeCreation.minutes;
   time->t.hours = buffer.ftimeCreation.hours;
}


/*@ XFileInfo::GetLastWriteDate(XDate * date)
@group get information
@remarks Query the date of last write-access
@parameters  XDate * buffer    buffer to hold the data
*/
void XFileInfo::GetLastWriteDate(XDate * date) const
{
   date->d.days = buffer.fdateLastWrite.day;
   date->d.months = buffer.fdateLastWrite.month;
   date->d.years = buffer.fdateLastWrite.year + 1980;
}


/*@ XFileInfo::GetLastWriteTime(XTime * time)
@group get information
@remarks Query the time of last write-access
@parameters  XTime * buffer    buffer to hold the data
*/
void XFileInfo::GetLastWriteTime(XTime * time)  const
{
   time->t.seconds = buffer.ftimeLastWrite.twosecs * 2;
   time->t.minutes = buffer.ftimeLastWrite.minutes;
   time->t.hours = buffer.ftimeLastWrite.hours;
}


/*@ XFileInfo::GetFileSize(void)
@group get information
@remarks Query the size of the file.
@returns ULONG size
*/
ULONG XFileInfo::GetFileSize(void) const
{
   return buffer.cbFile;
}


/*@ XFileInfo::GetFileAttributes(void)
@group file attributes
@remarks Query the files attributes
@returns LONG attributes (can be or-ed):
<UL>
<LI>XFILE_IS_NORMAL
<LI>XFILE_IS_HIDDEN
<LI>XFILE_IS_SYSTEM
<LI>XFILE_IS_DIRECTORY
<LI>XFILE_IS_ARCHIVED
<LI>XFILE_IS_READONLY
</UL>
*/
LONG XFileInfo::GetFileAttributes(void) const
{
   return buffer.attrFile;
}


/*@ XFileInfo::SetFileAttributes( const LONG attr)
@group file attributes
@remarks Set the files attributes
@parameters LONG attributes, can be or-ed:
<UL>
<LI>XFILE_IS_NORMAL
<LI>XFILE_IS_HIDDEN
<LI>XFILE_IS_SYSTEM
<LI>XFILE_IS_DIRECTORY
<LI>XFILE_IS_ARCHIVED
<LI>XFILE_IS_READONLY
</UL>
*/
void XFileInfo::SetFileAttributes( const LONG attr)
{
   buffer.attrFile = attr;
}
