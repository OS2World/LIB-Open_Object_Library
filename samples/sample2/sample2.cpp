#define INCL_OOL_WIN
#define INCL_OOL_DBASE
#include "ool.h"

#include <stdio.h>
#include <stdlib.h>

class MyAppWindow: public XFrameWindow
{
   public:
      MyAppWindow( char* );
};


MyAppWindow :: MyAppWindow( char * f ): XFrameWindow( (ULONG) 0, "Sample2", XFrameWindow::defaultStyle | FRM_TASKLIST )
{
   XRect rect;
   //setup the container
   XContainerControl * c = new XContainerControl(this, rect,0, 0, "8.Helv");
   XContainerInfo info( "", CO_DETAIL | CO_DETAILTITLES );
   c->SetInfo( &info);

   SetClient(c);
   XRect re( 100,100,500,400);
   SetSize( &re);
   try
   {
      if(f == NULL) //no filename given, create a new file
      {
         //create the file
         XDBaseFile file( "test.dbf", TRUE, TRUE);
         //add three columns
         file.AddCharacterField( "Name", 30);
         file.AddNumericField( "Number", 20);
         file.AddDateField( "GebDat");
         //save all
         file.SaveHeader( "test.dbf" );
         f = "test.dbf";
      }

      XDBaseFile file( (char*) f, TRUE);
      //add a new record
      XDBaseRecord record( &file);
      XString string("Test");
      XDate date;
      date.GetCurrentDate();
      //set the data
      record.SetFieldData( 2, date);
      record.SetFieldData( 0, string);
      record.SetFieldData( 1, (LONG) rand());
      //save it
      record.Save();

      int fieldCount = file.GetFieldCount(), records = file.GetRecordCount();

      XString title = f;
      title += " ";
      title += (LONG) records;
      title += " Record(s)";
      SetText( title );

      if( records > 100)
      {
         XString s = "The file contains ";
         s+= (LONG) records;
         s+= " records. Display all records?";
           XMessageBox m( (char*) s, "FrontLine - dBase III", MBOX_YESNO);
         if( m.GetCommand() == MDID_NO)
            delete this;
      }
      XContainerColumn * lastCol = NULL;
      // for each column in the DBase-file a column is created in the container
      for( int i = 0; i < fieldCount; i++)
      {
         LONG type= COL_HORZSEPARATOR;
         if( i < file.GetFieldCount() - 1)
            type |= COL_SEPARATOR;

         SHORT res = file.GetFieldType(i);

         if(res == XDBASE_FIELD_NUMERIC)
            type |= COL_RIGHT;
         else
            type |= COL_LEFT;

         if( res == XDBASE_FIELD_DATE)
            type |= COL_DATE;
         else
            type |= COL_STRING;

         XString  * ti = new XString();
         file.GetFieldName(i, ti);
         *ti += "(";
         char dummy[2];
         dummy[0] = file.GetFieldType(i);
         dummy[1] = 0;
         *ti += dummy;
         *ti += ")";

         XContainerColumn * col = new XContainerColumn( c, (char*) *ti, i, type, COL_LEFT | COL_FITITLEREADONLY | COL_HORZSEPARATOR | COL_TOP );
         c->InsertColumn( col, lastCol);
         lastCol = col;
      }
      c->UpdateColumns();
      c->EnableWindowUpdate(FALSE);
      //read alll records
      for(int j = 1; j <= file.GetRecordCount(); j++)
      {
         XContainerObject * obj = new XContainerObject(c, file.GetFieldCount());
         try
         {
            XDBaseRecord record( &file, j);
            for(int i = 0; i < fieldCount; i++)
            {
               if( file.GetFieldType(i) != XDBASE_FIELD_DATE)
               {
                  XString * s = new XString();
                  record.GetFieldData(i, *s);
                  obj->SetColumnData( i, (const char*) *s);
               }
               else
               {
                  XDate d;
                  record.GetFieldData(i, d);
                  obj->SetColumnData( i, &d);
               }
            }
            c->AddObject( obj );
         } /* end try */
         catch( XDBaseException e)  //if a record is marked as "deleted" an exception is thrown
         {

         } /* end catch */
      }
      c->EnableWindowUpdate();
   }
   catch( XDBaseException e)
   {
      XMessageBox( (char*) e.GetErrorMessage());
   }
   Activate();
}


main(int argc, char ** argv)
{
   try
   {
      MyAppWindow * win = new MyAppWindow( (char*) argv[1]);
      XApplication::GetApplication()->Start();
   }
   catch( XException e)
   {
      e.ShowError();
      exit(-1);
   }
   return 0;
}
