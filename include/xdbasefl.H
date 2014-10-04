#ifndef __XDBASEFIELD_H__
#define __XDBASEFIELD_H__


/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDBaseFile                            */
/*                       derived from: XObject                               */
/*                        last update: 5/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xobject.h"

#define XDBASE_FIELD_CHAR     'C'
#define XDBASE_FIELD_NUMERIC  'N'
#define XDBASE_FIELD_LOGIC    'L'
//#define XDBASE_FIELD_MEMO     'M'
#define XDBASE_FIELD_DATE     'D'

#pragma pack(1)

typedef struct
{
   char    name[11];       
   char    type;                         
   char*   data_ptr;
   UCHAR   length;                 
   UCHAR   dec_point;
} dbfField;

#pragma pack()

class _export_ XDBaseField: public XObject
{
      friend class XDBaseFile;
      friend class XDBaseRecord;
   private:
      dbfField field;
      XDBaseField(XFile * , ULONG& off);
      XDBaseField( char * name, unsigned char type, unsigned char length, unsigned char decPointer = 0);
      void Save(XFile * file);
      ULONG offset;
};



#endif
