#ifndef __OOL_XTABLE_H__
#define __OOL_XTABLE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XTable                                */
/*                       derived from: XObject                               */
/*                        last update: 11/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"
#include "xstring.h"
#include "xdate.h"
#include "xtime.h"

#include <sql.h>     
#include <sqlenv.h>  
#include <sqlda.h>

typedef struct Field
{
   unsigned char * data;
   short int ind;//*
//   USHORT len;
};


class _export_ XTable: public XObject{
   public:
      XTable( const char * name);
      ~XTable();
      void AddSet(void);
      int CreateIndex( char * id, char * columnList );
      int CloseCursor(void);
      static int Commit(void);
      static int Connect( char * dbName);
      static int DisConnect();
      static int DropIndex( char * id);
      static int DropTable( char * tab);
      static int DropView( char * view);
      int GetField( const char * fieldName, XString * );
      int GetField( const char * fieldName, long int& value);
      int GetField( const char * fieldName, XDate * value);
      int GetField( const char * fieldName, XTime * value);
      int GetField( const char * fieldName, double& value);
      int GetField( SHORT i, XString * );
      int GetField( SHORT i, long int& value);
      int GetField( SHORT i, XDate * value);
      int GetField( SHORT i, XTime * value);
      int GetField( SHORT i, double& value);
      int GetFieldIndex( const char * fieldName );
      char * GetName() { return tabName; }
      BOOL IsEOF ( void ) { return eoFile; }
      BOOL IsCursorOpen(void) { return cursorOpen; }
      int LockTable(BOOL exclusive = FALSE);
      int MoveNext( void );
      SHORT QueryFieldCount(void);
      LONG QueryFieldLength( SHORT index );
      LONG QueryFieldLength( SHORT index, SHORT& precision, SHORT& scale);
      BOOL QueryFieldName( XString * buffer, SHORT index);
      SHORT QueryFieldType( SHORT index);
      static int Rollback(void);
      int SaveSet( void );
      int Select(const char * whereList, char * order=NULL, char * group=NULL, BOOL update=FALSE);
      int SetField( const char * fieldName, long int value, BOOL isNull = FALSE);
      int SetField( const char * fieldName, char * value, BOOL isNull = FALSE);
   private: 
      static int Drop(char* t, char*n);
      BOOL cursorOpen, insertFlag;
      struct sqlca sqlca;
      sqlda * sql;
      char * tabName; 
      BOOL eoFile;
//      SHORT myCursor;
      Field * fields;
      XString sqlString;
};

#endif
