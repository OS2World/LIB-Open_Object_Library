#define DEBUG
#define INCL_OOL
#include "sample17.h"
#include <stdlib.h>
#include "dialog.h"


class MyWizard: public XWizard
{
   public:
      BOOL DoCommand(LONG);
      BOOL DoSysCommand( USHORT com);
      MyWizard(XWindow*, const XRect& rect=(200,200,300,300), char * test = "test");
};


BOOL MyWizard :: DoSysCommand( USHORT com)
{
   if(com == SC_CLOSE)
   {
      if( XShowMessage("Are you shure to exit the assistant?", MB_YESNO|MB_ICONQUESTION|MB_MOVEABLE, this) == MBID_NO)
         return FALSE;
      debugMessage("wizzard closed");
   } /* end if */
   return TRUE;
}


BOOL MyWizard :: DoCommand(LONG com)
{
   if(com == IDP_NEXT)
      ShowNextPage();
   else if(com == IDP_PREV)
      ShowPrevPage();
   else if(com == IDP_CREATE)
   {
      XMessageBox msb("Ready?", "MyAssistant", MB_YESNO|MB_ICONQUESTION|MB_MOVEABLE, this);
      if(msb.GetCommand() == MBID_YES)
         Close();
   }
   return FALSE;
}


MyWizard :: MyWizard(XWindow*w, const XRect& rect, char*): XWizard(w, 100)
{
   XWizClientWindow * w1 = new XWizClientWindow(this, DLG_START);
   SetStartPage(w1);
   XWizClientWindow * w2 = new XWizClientWindow(this, IDD_SET);
   w1->SetNextPage(w2);
   XWizClientWindow * w3 = new XWizClientWindow(this, IDD_FINISH);
   w2->SetNextPage(w3);
}


//create a window out of the resource-file, the last parameter tells the library to
//build the window with the ID IDM_MENU.
SampleWindow :: SampleWindow( ): XFrameWindow( IDM_MENU, "Sample", XFrameWindow::defaultStyle | FRM_ICON | FRM_TASKLIST | FRM_CENTER       | FRM_MENU | FRM_ACCELTABLE     )
{
   //Set background-color
   XColor color( COL_WHITE);
   SetBackgroundColor( &color );

   //Set size
   XRect rect( 100,100, 400, 300);
   SetSize(&rect);

   //create a helpinstance
   help = new XHelpInstance( "Sample17.hlp", "Sample - Help");
   //associate the help with this function
   help->AssociateWindow( this );

   MyWizard * w = new MyWizard(this);
   help->AssociateWindow( w );
   w->Init();

   Invalidate(TRUE);
}


SampleWindow :: ~SampleWindow()
{
   debugMessage("main window closed");
}


//draw the window-content
void SampleWindow :: Draw( void )
{
   //fill the background
   FillBackground( );
}


/* here the commands of the menu are posted */
BOOL SampleWindow :: DoCommand( LONG command)
{
   switch( command )
   {
      //which menuitem was selected?
      case IDM_OPEN:
           {
         /*File/Open selected */
         /* display the file-dialog defined by the system */
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
               //perfor your loading of the selected file(s) here
            }
         }
      }
      break;
      case IDM_SAVEAS:
           {
               /*File/Open selected */
               /* display the file-dialog defined by the system */
               /* set the file-suffix and title of the dialog */
               XFileDialog fileDlg(this, "*.TXT", NULL, NULL, FD_SAVEAS | FD_CENTER );

               /* the user selected a file */
               if( fileDlg.GetCommand() == USER_OK)
                 {
                    XString fileName;
                    fileDlg.GetFileName( &fileName);  //get filename and path where to save
                    //perfor your save_as code here
                 }
            }
            break;
         case IDM_HELP_HELP:
            help->ShowHelpForHelp();
            break;
         case IDM_HELP_INDEX:
            help->ShowHelpIndex();
            break;
         case IDM_HELP_GENERAL:
            help->ShowHelpForId( 100 );
            break;
      }
   return TRUE;
}


//here the control-events of our window-contents are posted
void SampleWindow :: DoControl( XControlEvent * event)
{
   switch( event->GetEventID())              //what type of event?
      {
         case WIN_CHANGED:                   //window-content changed!
            {
               switch( event->GetWindowID()) //which window posted the event?
                  {
                     default:
                        break;
                  }
            }
      }
}


int main ( int argc, void ** argv)
{
   //create the window
   try
   {
      XConsole::CheckConsole(argc, argv);
      debugMessage("start window init");
      SampleWindow * window = new SampleWindow( );
      debugMessage("end window init");
   } /* end try */
   catch( XException e)
   {
      e.ShowError();
   } /* end catch */
   char * s = (char*) malloc(20);
   XApplication::GetApplication()->Start();
}
