#include "sample15.h"

#include <stdlib.h>


class MyClient;
class MyMLE: public XMultiLineEdit
{
   public:
      MyClient * owner;   //pointer to the parent
      MyMLE( MyClient * w): XMultiLineEdit( (XWindow*) w, XRect(), 0, MLE_HORZSCROLL | MLE_VERTSCROLL | WIN_VISIBLE)
            { owner = w; }
      void DoControl(XControlEvent*);
      void FocusChanged(BOOL set); //overwrite function to inform the server that we have the focus
};


class MyClient: public XMDIClient
{
        friend class MyMLE;
                BOOL loading, saved;
                MyMLE * mle;
   public:
      //constructor for the mdi-client
      MyClient(XMDIServer * p, char * title=""):XMDIClient(p, IDM_MENU, title, XFrameWindow::defaultStyle )
               {
         mle = new MyMLE(this);    //create a multiline-edit
         SetClient(mle);          //set it as client-window
         saved = TRUE;
         loading = FALSE;
                        XRect r;
                        p->GetClientSize(&r);
                        SetSize( &r );
      }
      virtual void UpdateMenu( XMenuBar * );   //overwrite the method to update the menu
      void Load( char * );
};


// function to inform the server that we have the focus
void MyMLE :: FocusChanged(BOOL set)
{
   if(set) owner->FocusChanged(set);
}


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


void MyClient :: Load( char * p)
{
   XFile loadfile;
   loading = TRUE;

   /* now open the file, fail if given filename is not existing */
   /* open the file for read-access, dont allow any other programm to use the file while it is open*/
   if( loadfile.Open( p, XFILE_FAIL_IF_NEW | XFILE_OPEN_EXISTING, XFILE_SHARE_DENYWRITE | XFILE_READONLY ) == 0)
   {
      XString s;

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
      mle->SetFocus();
   }
   else
      XMessageBox( p, "couldnït open File!", MB_OK|MB_ERROR);
   loading = FALSE;
}


BOOL MyServer :: DoCommand( LONG command)
{
   switch( command )
      {
         //which menuitem was selected?
         case IDM_OPEN:
            {
               /*File/Open selected */
               /* display the file-dialog defined by the system */
               /* set the file-suffix and title of the dialog */
               XFileDialog fileDlg(this, "*.*", NULL, NULL, FD_OPEN | FD_CENTER | FD_MULTIPLESEL);

               /* the user selected a file */
               if( fileDlg.GetCommand() == USER_OK)
               {
                  for(int i = 0; i < fileDlg.GetFileCount(); i++)
                  {
                     MyClient * w = new MyClient( this );
                     XString name;
                     fileDlg.GetFileName(&name, i);
                     w->Load(name);
                     w->SetText(name);
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

         default:
            return XMDIServer::DoCommand(command);
      }
   return TRUE;
}


// when we have the focus we can here manipulate the menu of the MDI-Server
void MyClient :: UpdateMenu( XMenuBar * menu)
{
   menu->EnableItem(IDM_CLOSE);
   if( saved == FALSE)
   {
      menu->EnableItem(IDM_SAVE);
      menu->EnableItem(IDM_SAVEAS);
   }
   else
   {
      menu->EnableItem(IDM_SAVE, FALSE);
      menu->EnableItem(IDM_SAVEAS, FALSE);
   }
   menu->EnableItem(IDM_PRINT);
}


// no client-window is open, reset the menu
void MyServer :: SetEmptyMenu(XMenuBar * menu)
{
   menu->EnableItem(IDM_CLOSE, FALSE);
   menu->EnableItem(IDM_SAVE, FALSE);
   menu->EnableItem(IDM_SAVEAS, FALSE);
   menu->EnableItem(IDM_PRINT, FALSE);
}

void main ( int argc, void ** argv)
{
    XApplication::GetApplication()->SetArguments(argc, argv);

    MyServer * window = new MyServer();
    window->Init();

    for(int i = 0; i < XApplication::GetApplication()->GetArgumentCount(); i++)
    {
        XString buffer;
        XApplication::GetApplication()->GetArgumentValue(i, &buffer);
        MyClient * w = new MyClient( window, buffer);
        w->Load( buffer );
    }

    XApplication::GetApplication()->Start(); //dont forget to run the application
}

