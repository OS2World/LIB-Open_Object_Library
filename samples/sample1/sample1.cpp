/* required for memory observation */
#define DEBUG

/* include file */
#include "sample1.h"

#include <stdlib.h>
#include <string.h>
/* This is the code for sample1, a simple text-editor */

class MyMLE: public XMultiLineEdit
{
   public:
      MyAppWindow * owner;   //pointer to the parent
      MyMLE( MyAppWindow * w ): XMultiLineEdit( w, XRect(), MY_MLE, MLE_HORZSCROLL | MLE_VERTSCROLL | WIN_VISIBLE) { owner = w; }
      void DoControl(XControlEvent*);
};

/* we want to accept files wich are dropped on our window, so we need */
/* a handler wich can handle this messages for drag/drop. we need to derive */
/* a class from class XDragHandler and overwrite the method HandleEvent */
/* and the constructor */
class MyDragHandler: public XDragHandler
{
   public:
      MyDragHandler( XWindow * w): XDragHandler( w ) { ;}
      void HandleEvent(XDragEvent*);
};


/* here the method HandleEvent is overridden, so we can handle drag/drop events*/
void MyDragHandler :: HandleEvent ( XDragEvent * event)
{
   switch( event->GetEventID())
      {
         /* the mousepointer is over the MLE holding one or more objects */
         case DRG_DRAGOVER:
            {
               SHORT i;
               XDragItem item;
               /* ask for every object the type and format */
               for(i=0; i < event->GetDragItemCount(); i ++)
                 {
                    event->GetDragItem( &item, i);
                    if(item.VerifyItemType( DRG_M_FILE ) == FALSE)
                       {
                          /* no textfile, we will not accept these objects */
                          event->SetAcceptMode( DRG_NEVERDROP);
                          return;
                       }
                 }
               event->SetAcceptMode( DRG_DROP);       //ok, can be dropped
               event->SetOperation( DO_COPY);         //we will work with a copy
             }
             return;
         /* one or more object are dropped, because weïve tested the format (see above) */
         /* we donït have to test it here again, the object will be files */
         case DRG_DROP:
            {
               XDragItem item;
               XString name, container;
               SHORT i;

               /* query for every object the information we need */
               for(i=0; i < event->GetDragItemCount(); i++)
                  {
                     event->GetDragItem( &item, i);
                     item.Accept();                        //let the sender know that we accept the object

                     item.GetName( &name);               //query the filename
                     item.GetContainerName( &container); //query the directory
                     container+= name;                     //make a complete path
                                                         //get the parent window
                     MyAppWindow * appWin = (MyAppWindow*) ((MyMLE*) GetWindow())->owner;
                     appWin->LoadFile( container );        //load the file
                  }
            }
            return;
      }
}


/* here the mainwindow will be created */
MyAppWindow :: MyAppWindow( ULONG id ): XFrameWindow( id, "Sample1", XFrameWindow::defaultStyle | FRM_ICON | FRM_TASKLIST)
{

   loading = FALSE;

   XRect rec( 100, 100, 500, 350);
   SetSize( &rec );
   /* we create a MLE-control; because the MLE is used as client we dont want a border */
   /* and donït use the defaults (see <xmle.h>) */
   mle = new MyMLE( this );

   /*set backgroundcolor to white*/
   XColor col( COL_WHITE);
   mle->SetBackgroundColor( &col );

   /* the MLE is used as a new client-area, so sizing and moving is automaticaly done */
   SetClient( mle );

   /* we want a menubar, it is defined in the resources of the exe-file with the id IDM_MAIN */
   menu = new XMenuBar( this, IDM_MAIN);

   /* we put a string into the MLE */
   mle->SetText( "this is our MLE where you can edit your text...");

   /*we generate our drag-handler and attach it to the MLE */
   MyDragHandler * myDragHandler = new MyDragHandler( mle );

   /* setup our needed flags */
   saved = TRUE;
   loaded = FALSE;

   dlg = NULL;

   /*the mle gets the focus */
   mle->SetFocus();
}


class SearchDialog: public XModelessDialog
{
      XMultiLineEdit * mle;
      MyAppWindow * win;
      XMLESearch search;
   public:
      BOOL DoCommand( LONG );
      void DoControl( XControlEvent*);
      SearchDialog( XMultiLineEdit*, MyAppWindow * );
      ~SearchDialog();
};


SearchDialog :: ~SearchDialog()
{
   win->dlg = NULL;
}


void SearchDialog :: DoControl( XControlEvent * event )
{
   switch( event->GetEventID() )
     {
        case WIN_CHANGED:
           switch( event->GetWindowID())
              {
                 case CHECK_CASE:
                    {
                       XCheckBox * c = (XCheckBox*) event->GetWindow();
                       if( c->IsSelected())
                          search.SetCaseSensitive( FALSE );
                       else
                          search.SetCaseSensitive( TRUE );
                    }
                    break;
              }
            break;
     }
}


BOOL SearchDialog :: DoCommand( LONG com )
{
   switch( com )
     {
        case PUSH_CANCEL:
           break;
        case PUSH_REPLACE:
           {
              XString buffer;
              GetWindow( ENTRY_REPLACE )->GetText( &buffer );
              mle->InsertString( buffer );
            }
            return FALSE;
        case PUSH_SEARCH:
           {
              XString buffer;
              GetWindow( ENTRY_SEARCH )->GetText( &buffer );
              search.SetSearchItem( buffer );
              GetWindow( ENTRY_REPLACE )->GetText( &buffer );
              search.SetReplaceItem( buffer );
              LONG start, end;
              mle->GetSelection( start, end);
              if(start > end )
                 end = start;
              search.SetStartPoint( end );
              XCheckBox * c = (XCheckBox*) GetWindow( CHECK_REPLACE );
              if( c->IsSelected())
                 search.SetReplaceAll( TRUE );
              else
                 search.SetReplaceAll( FALSE );
              if( mle->Search( &search ) )
                 mle->Activate();
              else
                 XMessageBox( "pattern not found!", "Search", MB_OK|MB_ICONEXCLAMATION, this);
              return FALSE;
           }
     }
   return TRUE;
}


/*this is our search-dialog (very rudimental). We construct it out of the resources (see sample.dlg).*/
//SearchDialog :: SearchDialog ( XMultiLineEdit * m, XResource * r, MyAppWindow * w): XFrameWindow( r, "Search...", defaultDialogStyle | FRM_CENTER,  NULL, NULL, TRUE)
SearchDialog :: SearchDialog ( XMultiLineEdit * m, MyAppWindow * w): XModelessDialog( DIALOG_SEARCH, w, TRUE)
{
   mle = m;
   win = w;

   /*enable the checkbox "ignore case" */
   XCheckBox * c = (XCheckBox*) GetWindow( CHECK_CASE);
   c->Select();
   Show();
}


/*the destructor is called automaticaly when the window is close*/
MyAppWindow :: ~MyAppWindow()
{
   //if the search-dialog is open, destroy it
   if(dlg)
      delete dlg;
}


/* this is the overridden method DoCmmand from XFrameWindow. all commands */
/* from the menu (or toolbars in later samples) will be placed here. */
BOOL MyAppWindow :: DoCommand( LONG command )
{
   switch ( command )
      {
         //default clipboard actions
         case IDM_PASTE:
            mle->Paste();
            break;
         case IDM_COPY:
            mle->Copy();
            break;
         case IDM_CUT:
            mle->Cut();
            break;
         case IDM_CLEAR:
            mle->Clear();
            break;
         case IDM_FONT:
            {
               //select a new font for the MLE
               XFontDialog dlg( this );
               if( dlg.GetCommand() == USER_OK)
                  {
                     XString buffer;
                     LONG s = dlg.GetFontSize();
                     dlg.GetFontName( &buffer );
                     mle->SetFont(buffer, s );
                  }
            }
            break;
         case IDM_SEARCH:
            {
               //open a search dialog
               if( dlg ) //a dialog is already existing
                  dlg->Activate();
               else //create a new dialog
                  dlg = new SearchDialog( mle, this);
            }
            break;
         case IDM_OPEN:
           {
               /* display the filedialog defined by the system */
               /* set the file-suffix and title of the dialog */
               XFileDialog fileDlg(this, "*.TXT", NULL, NULL, FD_OPEN | FD_CENTER | FD_MULTIPLESEL);

               /* the user selected a file */
               if( fileDlg.GetCommand() == USER_OK)
                 {
                    XString fileName;
                    SHORT i, files = fileDlg.GetFileCount();  //how much files are selected?
                    for(i=0; i < files; i++)
                       {
                          fileDlg.GetFileName( &fileName, i );  //get filename and path for every file
                          LoadFile( fileName);                  //load the file
                       }
                 }
            }
            break;
         case IDM_CLOSE:
            /* the user selected close, so we try to close the window */
            /* the (overridden) method QueryForClose is called automaticaly (see below)*/
            delete this;
            break;
         case IDM_SAVE:
            SaveFile();
            break;
       }
   return TRUE;
}


/* when contents of window-controls change, we will get an event of the class */
/* XControlEvent where we can ask for information about the event and the sending */
/* window. in later examples the other types of events  will be shown */
void MyMLE :: DoControl( XControlEvent * event )
{
   //ask for the id of the sending window and the type of event
   if( event->GetEventID() == WIN_CHANGED && owner->loading == FALSE)
      {
         //sender is the MLE, event is that the text will be changed
         if( owner->saved == TRUE)
            {
               XString s;
               owner->GetText( &s );
               s += " - changed";
               owner->SetText( s );
               owner->saved = FALSE;
            }
       }
}


/* this is an overridden method of XFrameWindow, it is called if the user try to close the */
/* framewindow with the close-button or with ALT-F4. Return TRUE if the window can be closed, */
/* otherwise return FALSE */
BOOL MyAppWindow :: QueryForClose( void )
{
   if(saved == FALSE)    //text changed
      {
         XMessageBox msb( "Text changed - discard?", "Close Window", MB_YESNO|MB_WARNING|MB_MOVEABLE);
         if( msb.GetCommand() == MBID_NO)
            return FALSE;  //donït close the window
      }
  return TRUE;
}


/* load a file */
BOOL MyAppWindow :: LoadFile ( char * p)
{
   // is there already a file loaded or has the user entered some text?
   if(loaded == FALSE && saved == TRUE)
      {
         //no, we load the file in the window of the current thread
         XFile loadfile;

         /* now open the file, fail if given filename is not existing */
         /* open the file for read-access, dont allow any other programm to use the file while it is open*/
         if( loadfile.Open( p, XFILE_FAIL_IF_NEW | XFILE_OPEN_EXISTING, XFILE_SHARE_DENYWRITE | XFILE_READONLY ) == 0)
           {
              XString s;

              loading = TRUE;

              //how large is the file?
              XFileInfo info;
              loadfile.GetFileInfo( &info );
              LONG size = info.GetFileSize();

              //read the complete file
              loadfile.Read ( (PVOID) s.GetBuffer(info.GetFileSize() + 1), size);
              s.ReleaseBuffer( info.GetFileSize() );

              //set the XString content to the mle
              mle->SetText( s );
              //dontïforget to close the file
              loadfile.Close();
              loaded = TRUE;
              path = p;
              mle->SetFocus();
              GetText( &s );
              s+= " - ";
              s+= p;
              SetText( s );

              loading = FALSE;

              return TRUE;
            }
         else
            {
               XMessageBox( p, "couldnït open File!", MB_OK|MB_ERROR);
               return FALSE;
            }
      }
   else
     {
         //there is a file loaded, or the user has entered some text, so
         // we create a new window and load the file
//         XResource res( IDM_MAIN, ((MyApp*) GetProcess())->GetResourceLibrary());
         MyAppWindow * win = new MyAppWindow( IDM_MAIN );
         win->LoadFile(p);
         return TRUE;
     }
}


BOOL MyAppWindow :: SaveFile( void )
{
   XFile savefile;
   XString s;

   mle->GetText( &s );

   /* now open the file, create a new one if the given filename is not existing, otherwise overwrite */
   /* it. open the file for write-access, dont allow any other programm to use the file while it is open*/
   if( savefile.Open( path, XFILE_CREATE_IF_NEW | XFILE_REPLACE_EXISTING, XFILE_SHARE_DENYNONE | XFILE_WRITEONLY, s.GetLength()) == 0)
      {
         /* set the file-cursor to the beginning of the file */
         savefile.Seek( 0, FILE_BEGIN);

         /*save the string */
         savefile.Write ( (char*) s, s.GetLength());

         /* close the file */
         savefile.Close();
         saved = TRUE;
         return TRUE;
      }
   else
      {
         XMessageBox( "could not open file", "error", MB_OK| MB_ERROR);
         return FALSE;
      }
}


int main ( int argc, void ** argv)
{
   /* enable bounds-checking and memory observation */
   XMemoryCheck memObj;

   /* start the console if an argument is '-console' */
   XConsole::CheckConsole(argc, argv);

   /* store the arguments given */
   XApplication::GetApplication()->SetArguments( argc, argv);   //save the aguments given on the commandline
   try
   {
      if( XApplication::GetApplication()->GetArgumentCount() )
      {
         //start for every file given in the arguments a new window
         for( int i = 0; i < XApplication::GetApplication()->GetArgumentCount(); i++)
         {
            XString fileName;
            XApplication::GetApplication()->GetArgumentValue(i, &fileName);
            MyAppWindow * appWindow = new MyAppWindow( IDM_MAIN );  //create new framewindow (see above)
            appWindow->LoadFile(fileName);
         }
      }
      else
         MyAppWindow * appWindow = new MyAppWindow( IDM_MAIN );  //create new framewindow (see above)

      /* to check if bounds-checking works enable the following */
      /* lines and start the program with the '-console' argument: */
      /*
      char * s = (char*) malloc(10);
      strcpy(s, "1234567890");
      */
      XApplication::GetApplication()->Start();                    //let the application work
   }
   catch( XException& e)
   {
      e.ShowError();
   }
   return 0;
}
