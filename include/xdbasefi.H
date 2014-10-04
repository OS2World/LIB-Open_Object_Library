#ifndef __XDBASEFILE_H__
#define __XDBASEFILE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDBaseFile                            */
/*                       derived from: XObject                               */
/*                        last update: 5/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xfile.h"
#include "xdbasefl.h"

#pragma pack(1)

typedef struct 
{
   char version; 
   char date[3];
   ULONG records;
   USHORT headerSize;
   USHORT recordSize;
} dbHeader;

#pragma pack()

class _export_ XDBaseFile: public XObject
{
   friend class XDBaseRecord;
   private:
      XFile file;
      dbHeader head;
      XDBaseField ** fields;
      USHORT fieldCount;
      BOOL changed;
      BOOL LoadSet( XDBaseRecord *);
      BOOL RemoveSet( XDBaseRecord* );
      BOOL SaveSet( XDBaseRecord *);
      void AddField(char * name, UCHAR type, UCHAR length, UCHAR decPointer = 0);
   public:
      XDBaseFile( char * file, BOOL writeAccess = TRUE, BOOL createNew = FALSE);
      ~XDBaseFile();
      void AddCharacterField( char* name, UCHAR length);
      void AddDateField( char * name);
      void AddLogicField( char * name);
      void AddNumericField( char* name, UCHAR length, UCHAR decimal = 0);
      ULONG GetRecordCount() {return head.records;}
      USHORT GetFieldCount() {return fieldCount;}
      BOOL GetFieldName(USHORT index, XString *);
      char GetFieldType(USHORT index) 
                           { if(index >= fieldCount) return -1; return fields[index]->field.type; }
      unsigned char GetFieldLength(USHORT index)
                           { if(index >= fieldCount) return -1; return fields[index]->field.length; }
      unsigned char GetFieldDecimalPoint(USHORT index)
                           { if(index >= fieldCount) return -1; return fields[index]->field.dec_point; }
      void SaveHeader(char * fileName);
};

#endif
