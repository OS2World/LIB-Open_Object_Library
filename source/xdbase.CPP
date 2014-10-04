#include "xdbasefi.h"
#include "xdbasefl.h"
#include "xdbaseex.h"
#include "xdbasere.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "XString.h"

/*@
@class XDBaseFile
@parent XObject
@type overview
@symbol _
@remarks XDBaseFile is a class to handle dBaseIII-databases
*/


/*@
@class XDBaseRecord
@parent XObject
@type overview
@symbol _
@remarks XDBaseRecord is a class to load save single record of a XDBaseFile
*/

/*@ XDBaseFile :: XDBaseFile( char * fileName, BOOL writeAccess, BOOL createNew)
@group   contructors/destructors
@remarks Create/Open a DBaseIII - file. The behaviour differs from the used parameters.
@parameters
<t 'ø' c=2>
øchar * fileName   øName of the file. A full qualified path can be nessacary.
øBOOL writeAccess   øTRUE=allow writing. The file will be locked for other applications for writing.<BR>
                   FALSE=read-only, other applications may write to the file.<BR>
                   (default is FALSE)
øBOOL createNew   øTRUE=create a new file. The file is not writen to disk until you call XDBaseFile::SaveHeader(). <BR>
                   FALSE=open a existing file.<BR>
                   (default is FALSE)
</t>
@exceptions If the function fails an exception of the type XDBaseException is thrown.
*/
XDBaseFile :: XDBaseFile( char * fileName, BOOL writeAccess, BOOL createNew)
{
   LONG rc;
   fields = NULL;
   fieldCount = 0;
   changed = FALSE;

   if(createNew)
   {
      head.version = 3;
      head.records = 0;
      head.recordSize = 0;
      head.headerSize = 0;
      return;
   }

   ULONG settings = OPEN_ACCESS_READONLY;
   if( writeAccess )
      settings = OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYWRITE;
   else
      settings |= OPEN_SHARE_DENYNONE;

   settings |= OPEN_FLAGS_RANDOMSEQUENTIAL;

   if( (rc = file.Open( fileName, XFILE_OPEN_EXISTING, settings)) == 0)
   {
      file.Seek(0);
      file.Read( &head, sizeof(head));
      char reserv[20];
      file.Read(reserv, 20);
      fieldCount = (head.headerSize-1) / 32 - 1;
      fields = (XDBaseField**) malloc( fieldCount * sizeof(void*));
      ULONG off = 0;
      for(int i = 0; i < fieldCount; i++)
      {
         fields[i] = new XDBaseField( &file, off );
      }
   }
   else
      OOLDbaseThrow("couldnït open file!", rc);
}


BOOL XDBaseFile :: RemoveSet(XDBaseRecord * record)
{
   if( record->index > 0)
   {
      file.Seek( (head.headerSize + 1) + (record->index-1) * head.recordSize, XFILE_BEGIN);
      record->buffer[head.recordSize-1] = '*';
      file.Write( record->buffer, head.recordSize );
   }
   record->deleted = TRUE;
   return TRUE;
}


BOOL XDBaseFile :: SaveSet(XDBaseRecord * record)
{
   BOOL isNew = FALSE;
   changed = TRUE;

   if( record->index == 0)
   {
       isNew = TRUE;
       head.records++;
      record->index = head.records;
   }
   file.Seek( head.headerSize + 1 + (record->index-1) * head.recordSize, XFILE_BEGIN);
   record->buffer[head.recordSize-1] = ' ';
   ULONG size = file.Write( record->buffer, head.recordSize );
   if( size != head.recordSize)
      throw XDBaseException( "error saving", -110);

   if(isNew)
   {
      file.Seek( 4 );
      file.Write( (LONG&) head.records );
      record->isNew = FALSE;
   }
   record->changed = FALSE;
   return TRUE;
}


BOOL XDBaseFile :: LoadSet( XDBaseRecord * record)
{
     if(record->index > head.records || record->index == 0)
      return FALSE;

   file.Seek( head.headerSize + 1 + (record->index-1) * head.recordSize, XFILE_BEGIN);

   ULONG size = file.Read( record->buffer, head.recordSize);
   if( size != head.recordSize)
      throw XDBaseException( "error loading", -111);

   if( record->buffer[head.recordSize-1] == '*')
      return FALSE;
   changed = FALSE;
   return TRUE;
}


/*@ XDBaseRecord :: XDBaseRecord( XDBaseFile * f, ULONG i)
@remarks Construct a record. A record is a single column of a XDBaseFile.
@parameters
<t 'ø' c=2>
øXDBaseFile *   ødatabase
øULONG         øzero based index of the record.<BR>
                0=create a new record<BR>
                other=index of a record to load
</t>
@exceptions If the function fails an exception of the type XDBaseException is thrown.
Usualy this exception is thrown when an index-value was given but no record with this index exists
or the record is marked as deleted.
@example
//In this example all records of a file will be read
//and are stored in a collection

    XDbaseFile file(...);

    list<XDBaseRecord*> recordList;
    for(int i=0; i < file.GetRecordCount(); i++)
    {
        try
        {
            XDBaseRecord * record = new XDBaseRecord(&file, i);
            recordList.push_back(record);
        }
        catch( XDBaseException e)
        {
            //the exception can be ignored here: the requested
            //record is removed from the file
        }
     }
*/
XDBaseRecord :: XDBaseRecord( XDBaseFile * f, ULONG i)
{
   index = i;
   owner = f;
   buffer = (char*) malloc( owner->head.recordSize );
   memset( buffer, 32, owner->head.recordSize);
   deleted = changed = FALSE;

   if( index )
   {
      if( owner->LoadSet( this ) == FALSE)
         OOLDbaseThrow( "couldnït load specified record", -100);
      isNew = FALSE;
   }
   else
      isNew = TRUE;
}


XDBaseRecord :: ~XDBaseRecord()
{
   free( buffer );
}


/*@ XDBaseRecord :: Save()
@remarks Save a record. If the record is new it will be attached at the
end of the file.
@returns BOOL success
*/
BOOL XDBaseRecord :: Save()
{
   return owner->SaveSet( this );
}


/*@ XDBaseRecord :: Remove()
@remarks Mark a record as deleted. The record cannot be loaded any more.
@returns BOOL success
*/
BOOL XDBaseRecord :: Remove()
{
   return owner->RemoveSet( this );
}


XDBaseFile :: ~XDBaseFile()
{
   if(changed)
   {
      XDate date;
      date.GetCurrentDate();

      char hDate[3];
      hDate[0] = date.GetYears() - 1900;
      hDate[1] = date.GetMonths();
      hDate[2] = date.GetDays();
      file.Seek( 1 );
      file.Write( hDate, 3);
   }
   file.Close();
   for(int i=0; i < fieldCount; i++)
      delete fields[i];
   free(fields);
}


XDBaseField :: XDBaseField(XFile * file, ULONG& off)
{
   file->Read(&field, sizeof(field));
   char fill[14];
   file->Read(fill, 14);
   offset = off;
   off += field.length;
}


void XDBaseField :: Save(XFile * file)
{
   file->Write(&field, sizeof(field));
   char fill[14];
   file->Write(fill, 14);
}


BOOL XDBaseFile :: GetFieldName(USHORT index, XString * string)
{
     if(index >= fieldCount)
      return FALSE;

   memcpy( string->GetBuffer(12), fields[index]->field.name, 11);
   string->ReleaseBuffer(11);
   string->Strip();
   return TRUE;
}


/*@ XDBaseRecord :: GetFieldData( USHORT i, XDate& date)
@remarks Query data of a record (date field)
@parameters
<t 'ø' c=2>
øUSHORT øzero based index of the field of interest
øXDate& øbuffer for the data
</t>
@returns BOOL success
*/
BOOL XDBaseRecord :: GetFieldData( USHORT i, XDate& date)
{
     if(i >= owner->fieldCount)
      return FALSE;

   if( owner->fields[i]->field.type == XDBASE_FIELD_DATE)
   {
      XString bu;
       memcpy( bu.GetBuffer(owner->fields[i]->field.length + 1), buffer + owner->fields[i]->offset, owner->fields[i]->field.length);
      bu.ReleaseBuffer( owner->fields[i]->field.length );
      long d = atol(bu.Left(4));
      date.SetYears( d );
      d = atol(bu.Mid(4, 2));
      date.SetMonths( d );
      d = atol(bu.Right(2));
      date.SetDays( d );
      return TRUE;
   }
   return FALSE;
}


/*@ XDBaseRecord :: GetFieldData(USHORT i, LONG& data)
@remarks Query data of a record (numeric field)
@parameters
<t 'ø' c=2>
øUSHORT øzero based index of the field of interest
øLONG& øbuffer for the data
</t>
@returns BOOL success
*/
BOOL XDBaseRecord :: GetFieldData(USHORT i, LONG& data)
{
     if(i >= owner->fieldCount)
      return FALSE;

   if( owner->fields[i]->field.type == XDBASE_FIELD_NUMERIC)
   {
      if( owner->fields[i]->field.dec_point== 0)
      {
         XString d;
         char * p = buffer;
         p += owner->fields[i]->offset;
          memcpy( d.GetBuffer(owner->fields[i]->field.length + 1), p, owner->fields[i]->field.length);
         d.ReleaseBuffer( owner->fields[i]->field.length );
           data = atol( (char*) d);
         return TRUE;
      }
   }
   return FALSE;
}


/*@ XDBaseRecord :: GetFieldData(USHORT i, double& data)
@remarks Query data of a record (numeric field)
@parameters
<t 'ø' c=2>
øUSHORT øzero based index of the field of interest
ødouble& øbuffer for the data
</t>
@returns BOOL success
*/
BOOL XDBaseRecord :: GetFieldData(USHORT i, double& data)
{
     if(i >= owner->fieldCount)
      return FALSE;

   if( owner->fields[i]->field.type == XDBASE_FIELD_NUMERIC)
   {
      if( owner->fields[i]->field.dec_point > 0)
      {
           char * end, * p = buffer;
         XString d;
         p += owner->fields[i]->offset;
          memcpy( d.GetBuffer(owner->fields[i]->field.length + 1), p, owner->fields[i]->field.length);
         d.ReleaseBuffer( owner->fields[i]->field.length );
           data = strtod( (char*) d, &end);
         return TRUE;
      }
   }
   return FALSE;
}


/*@ XDBaseRecord :: GetFieldData(USHORT i, XString& data)
@remarks Query data of a record (char field)
@parameters
<t 'ø' c=2>
øUSHORT øzero based index of the field of interest
øXString& øbuffer for the data
</t>
@returns BOOL success
*/
BOOL XDBaseRecord :: GetFieldData( USHORT i, XString& data)
{
     if(i >= owner->fieldCount)
      return FALSE;

//   if( fields[index]->type == XDBASE_FIELD_MEMO || fields[index]->type == XDBASE_FIELD_CHAR )
   {
       char * p = buffer;
      p += owner->fields[i]->offset;
       memcpy( data.GetBuffer(owner->fields[i]->field.length + 1), p, owner->fields[i]->field.length);
      data.ReleaseBuffer( owner->fields[i]->field.length );
      data.Strip();
      return TRUE;
   }
   return FALSE;
}


/*@ XDBaseRecord :: SetFieldData( USHORT i, XString& data)
@remarks Set data of a record (char field)
@parameters
<t 'ø' c=2>
øUSHORT øzero based index of the field of interest
øXString& ødata
</t>
@returns BOOL success
*/
BOOL XDBaseRecord :: SetFieldData( USHORT i, XString& data)
{
     if(i >= owner->fieldCount)
      return FALSE;

   if( owner->fields[i]->field.type == XDBASE_FIELD_CHAR )
   {
      char * p = buffer;
      p += owner->fields[i]->offset;
      memset( p, 32, owner->fields[i]->field.length);
       int len = data.GetLength();
      if(len > owner->fields[i]->field.length)
         len = owner->fields[i]->field.length;
      memcpy( p, (char*) data, len);
      changed = TRUE;
      return TRUE;
   }
   return FALSE;
}


/*@ XDBaseRecord :: SetFieldData( USHORT i, XDate& data)
@remarks Set data of a record (date field)
@parameters
<t 'ø' c=2>
øUSHORT øzero based index of the field of interest
øXDate& ødata
</t>
@returns BOOL success
*/
BOOL XDBaseRecord :: SetFieldData( USHORT i, XDate& date)
{
     if(i >= owner->fieldCount)
      return FALSE;

   if( owner->fields[i]->field.type == XDBASE_FIELD_DATE)
   {
       char * p = buffer;
      p += owner->fields[i]->offset;
      memset( p, 32, owner->fields[i]->field.length);
      char bu[12];
      sprintf( bu, "%.4i%.2i%.2i", date.GetYears(), date.GetMonths(), date.GetDays());
      memcpy( p, bu, 8);
      changed = TRUE;
      return TRUE;
   }
   return FALSE;
}


/*@ XDBaseRecord :: SetFieldData( USHORT i, LONG data)
@remarks Set data of a record (numeric field)
@parameters
<t 'ø' c=2>
øUSHORT øzero based index of the field of interest
øLONG ødata
</t>
@returns BOOL success
*/
BOOL XDBaseRecord :: SetFieldData(USHORT i, LONG data)
{
     if(i >= owner->fieldCount)
      return FALSE;
   if( owner->fields[i]->field.type == XDBASE_FIELD_NUMERIC)
   {
      if( owner->fields[i]->field.dec_point == 0)
      {
           char * p = buffer;
         p += owner->fields[i]->offset;
         char fString[20], bu[100];

         sprintf(fString, "%%.%ii", owner->fields[i]->field.length);
           int len = sprintf( bu, fString, data);
         memcpy( p, bu, len);
         changed = TRUE;
         return TRUE;
      }
   }
   return FALSE;
}


/*@ XDBaseRecord :: SetFieldData( USHORT i, BOOL data)
@remarks Set data of a record (logical field)
@parameters
<t 'ø' c=2>
øUSHORT øzero based index of the field of interest
øBOOL ødata
</t>
@returns BOOL success
*/
BOOL XDBaseRecord :: SetFieldData(USHORT i, BOOL data)
{
     if(i >= owner->fieldCount)
      return FALSE;
   if( owner->fields[i]->field.type == XDBASE_FIELD_LOGIC)
   {
        char * p = buffer;
      p += owner->fields[i]->offset;
      if(data)
         *p = 'Y';
      else
         *p = 'N';
      changed = TRUE;
      return TRUE;
   }
   return FALSE;
}


/*@ XDBaseRecord :: GetFieldData(USHORT i, BOOL& data)
@remarks Query data of a record (logical field)
@parameters
<t 'ø' c=2>
øUSHORT øzero based index of the field of interest
øBOOL& øbuffer for the data
</t>
@returns BOOL success
*/
BOOL XDBaseRecord :: GetFieldData(USHORT i, BOOL& data)
{
     if(i >= owner->fieldCount)
      return FALSE;
   if( owner->fields[i]->field.type == XDBASE_FIELD_LOGIC)
   {
        char * p = buffer;
      p += owner->fields[i]->offset;
      if( *p == 'Y' || *p == 'y' || *p == 'T' || *p == 't')
         data = TRUE;
      else
         data = FALSE;
      return TRUE;
   }
   return FALSE;
}


/*@ XDBaseRecord :: SetFieldData( USHORT i, double data)
@remarks Set data of a record (numeric field)
@parameters
<t 'ø' c=2>
øUSHORT øzero based index of the field of interest
ødouble ødata
</t>
@returns BOOL success
*/
BOOL XDBaseRecord :: SetFieldData(USHORT i, double data)
{
     if(i >= owner->fieldCount)
      return FALSE;

   if( owner->fields[i]->field.type == XDBASE_FIELD_NUMERIC)
   {
      if( owner->fields[i]->field.dec_point > 0)
      {
         char fString[10], bu[50], *p = buffer;
         p += owner->fields[i]->offset;
         sprintf( fString, "%%%i.%if", owner->fields[i]->field.length, owner->fields[i]->field.dec_point);
           int len = sprintf( bu, fString, data);
         memcpy( p, bu, len);
         changed = TRUE;
         return TRUE;
      }
   }
   return FALSE;
}


XDBaseField :: XDBaseField( char * n, unsigned char t, unsigned char l, unsigned char decPointer)
{
   if( t == XDBASE_FIELD_DATE )
      field.length = 8;
   else
      field.length = l;
   field.type = t;
   field.dec_point = decPointer;
   XString bu = n;
   if(bu.GetLength() > 10)
      bu = bu.Left(10);
   strncpy( field.name, (char*) bu, bu.GetLength());
}


void XDBaseFile :: AddField(char * name, UCHAR type, UCHAR length, UCHAR decPointer)
{
   fields = (XDBaseField**) realloc(fields, (fieldCount+1) * sizeof(void*));
   fields[fieldCount] = new XDBaseField(name, type, length, decPointer);
   ULONG offset = 0;
   for(int i = 0; i < fieldCount; i++)
      offset += fields[i]->field.length;
   fields[fieldCount]->offset = offset;
   fieldCount ++;
}


/*@ XDBaseFile :: AddCharacterField( char* name, UCHAR length)
@remarks Add a new character-field to the database (the database must be empty, store the database definition with SaveHeader())
@parameters
<t 'ø' c=2>
øchar* øname of the field
øUCHAR   ølength
</t>
*/
void XDBaseFile :: AddCharacterField( char* name, UCHAR length)
{
   AddField(name, XDBASE_FIELD_CHAR, length, 0);
}


/*@ XDBaseFile :: AddDateField( char * name)
@remarks Add a new date-field to the database (the database must be empty, store the database definition with SaveHeader())
@parameters
<t 'ø' c=2>
øchar* øname of the field
</t>
*/
void XDBaseFile :: AddDateField( char * name)
{
   AddField(name, XDBASE_FIELD_DATE, 8, 0);
}

/*@ XDBaseFile :: AddLogicField( char * name)
@remarks Add a new logic field to the database (the database must be empty, store the database definition with SaveHeader())
@parameters
<t 'ø' c=2>
øchar* øname of the field
</t>
*/
void XDBaseFile :: AddLogicField( char * name)
{
   AddField(name, XDBASE_FIELD_LOGIC, 1, 0);
}

/*@ XDBaseFile :: AddNumericField( char* name, UCHAR length, UCHAR decimal)
@remarks Add a new numeric field to the database (the database must be empty, store the database definition with SaveHeader())
@parameters
<t 'ø' c=2>
øchar* øname of the field
øUCHAR   ølength
øUCHAR   øposition of decimal point
</t>
*/
void XDBaseFile :: AddNumericField( char* name, UCHAR length, UCHAR decimal)
{
   AddField(name, XDBASE_FIELD_NUMERIC, length, decimal);
}



/*@ XDBaseFile :: SaveHeader(char * fileName)
@group   misc
@remarks Save the headerSize (dBaseIII-description) of the file. The current loaded file is closed and the
new file (if the filename differs from the original filename) is opend. <B>If a file exists, it will be overwriten!
The function resets the content, no records are saved!</B><P>
This function should only be called if you have used XDBaseFile::AddField() for inserting one or more new
Fields to the table (from this reason no records can be saved). You can use this function to create a copy
of the declaration of a dbaseIII-file:
<OL>
<LI>Open the file which contains the declaration and records:
<PRE>
     XDBaseFile file("orgfile.dbf");
</PRE>
<LI>You can add new fields to the declaration:
<PRE>
     file.AddField("DATE", XDBASE_FIELD_DATE, 8);
</PRE>
<LI>Save the declaration in a new file:
<PRE>
     file.SaveHeader( "newfile.dbf");
</PRE>
</OL>
@parameters   char * fileName
@exceptions If the function fails an exception of the type XDBaseException is thrown.
*/
void XDBaseFile :: SaveHeader(char * fileName)
{
   head.records = 0;
   head.recordSize = 0;
   head.headerSize = 33 + fieldCount * 32;
   changed = FALSE;

   for(int i=0; i < fieldCount; i++)
      head.recordSize += fields[i]->field.length;

   head.recordSize++;

   file.Close();

   LONG rc;
   if( (rc = file.Open( fileName, XFILE_REPLACE_EXISTING | XFILE_CREATE_IF_NEW, OPEN_FLAGS_RANDOMSEQUENTIAL | XFILE_READWRITE | OPEN_SHARE_DENYWRITE)) == 0)
   {
       XDate date;
      date.GetCurrentDate();

      head.date[0] = date.GetYears() - 1900;
      head.date[1] = date.GetMonths();
      head.date[2] = date.GetDays();

       file.Seek(0);
      file.Write( &head, sizeof(head));
      char reserv[20];
      memset(reserv, 32, 20);
      file.Write(reserv, 20);

      for(int i = 0; i < fieldCount; i++)
         fields[i]->Save(&file);
      char bu = 13;
      file.Write(bu);
   }
   else
      OOLDbaseThrow("couldnït open file!", rc);
}

//docs

/*@ XDBaseFile :: GetRecordCount()
@remarks Returns the count of records stored in the file INCLUDING
records which are marked as 'deleted'
@returns ULONG
*/

/*@ XDBaseFile :: GetFieldCount()
@remarks Returns the count of fields of each record
@returns USHORT
*/

/*@ XDBaseFile :: GetFieldName(USHORT index, XString *);
@remarks Returns a field-name
@parameters
<t 'ø' c=2>
øUSHORT øzero-based index of the field
øXString* øbuffer to get the name
*/

/*@ XDBaseFile :: GetFieldType(USHORT index)
@remarks Returns a fields data type
@parameters USHORT zero-based index of the field
@returns CHAR, possible values are
<UL compact>
<LI>XDBASE_FIELD_CHAR
<LI>XDBASE_FIELD_NUMERIC
<LI>XDBASE_FIELD_LOGIC
<LI>XDBASE_FIELD_DATE
</UL>
*/

/*@ XDBaseFile :: GetFieldLength(USHORT index)
@remarks Returns a fields size
@parameters USHORT zero-based index of the field
@returns UCHAR the size
*/

/*@ XDBaseFile :: GetFieldDecimalPoint(USHORT index)
@remarks Returns a fields dezimal point position
@parameters USHORT zero-based index of the field
@returns UCHAR position
*/

/*@ XDBaseRecord :: IsChanged()
@remarks Query if the record must be saved
@returns BOOL
*/

/*@ XDBaseRecord :: IsNew()
@remarks Query if the record is new
@returns BOOL
*/
