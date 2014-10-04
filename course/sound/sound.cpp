#define INCL_OOL_WIN
#define INCL_OOL_MM
#include "ool.h"

#include "resource.h"


class MyWindow: public XFrameWindow
{
      XToolBar * toolBar;
      XSound * sound;
   public:
      BOOL DoCommand(LONG);
      void DoControl( XControlEvent *);
      MyWindow();
      ~MyWindow() { delete sound; }
};


MyWindow :: MyWindow( ): XFrameWindow( 100 , "Hello world!", FRM_BORDER|FRM_TASKLIST|FRM_TITLEBAR|FRM_SYSMENU|WIN_VISIBLE, XRect(100,100,156,57))
{
   toolBar = new XToolBar(this, TB_TOP, 0, 34);

   XRect rec(0,0,32,24);
   XPushButton * back = new XPushButton( toolBar, rec, (USHORT) IDP_BACK, BU_BITMAP | WIN_VISIBLE);
   XPushButton * play = new XPushButton( toolBar, rec, (USHORT) IDP_PLAY, BU_BITMAP);
   XPushButton * halt = new XPushButton( toolBar, rec, (USHORT) IDP_STOP, BU_BITMAP);
   XPushButton * sel = new XPushButton( toolBar, rec, (USHORT) IDP_OPEN, BU_BITMAP);

   //disable buttons
   play->Enable(FALSE);
   halt->Enable(FALSE);
   back->Enable(FALSE);

   //add the pushbuttons to the toolbar, the toolbar will use the size of the first button
   toolBar->AddWindow( play );
   toolBar->AddWindow( halt );
   toolBar->AddWindow( back );
   toolBar->AddWindow( sel, TRUE );

   sound = new XSound(this);

   //make this window the active window
   Activate();
}


BOOL MyWindow :: DoCommand(LONG command)
{
   switch(command)
   {
      case IDP_PLAY:
         sound->Play();
         toolBar->GetWindow( IDP_STOP )->Enable();
         toolBar->GetWindow( IDP_PLAY )->Enable( FALSE );
         break;
      case IDP_STOP:
         if( sound->Stop() )
            XMessageBox("error");
         toolBar->GetWindow( IDP_STOP )->Enable( FALSE );
         toolBar->GetWindow( IDP_BACK )->Enable();
         toolBar->GetWindow( IDP_PLAY )->Enable();
         break;
      case IDP_BACK:
         if( sound->Rewind() )
            XMessageBox("error");
         toolBar->GetWindow( IDP_STOP )->Enable( FALSE );
         toolBar->GetWindow( IDP_PLAY )->Enable();
         break;
      case IDP_OPEN:
         {
            XFileDialog dlg( this, "c:\\mmos2\\sounds\\");
            if( dlg.GetCommand() == DID_OK)
            {
               XString s;
               dlg.GetFileName( &s );
               if( sound->SetDataFile( (char*) s ) != 0)
               {
                  XMessageBox( "cannot load sound file");
                  return TRUE;
               }
               toolBar->GetWindow( IDP_PLAY )->Enable();
               toolBar->GetWindow( IDP_STOP )->Enable( FALSE );
               toolBar->GetWindow( IDP_BACK )->Enable( FALSE );
            }
         }
         break;
      default:
         return FALSE;
   }
   return TRUE;
}


void MyWindow :: DoControl( XControlEvent * event)
{
   switch( event->GetEventID())
   {
      case MEDIA_PLAYED:
         toolBar->GetWindow( IDP_BACK )->Enable();
         toolBar->GetWindow( IDP_STOP )->Enable( FALSE );
         break;
      case MEDIA_REWINDED:
         toolBar->GetWindow( IDP_BACK )->Enable( FALSE );
         break;
      default:
         break;
   }
}


int main()
{
   try
   {
      //now we get a pointer to the
      //application object
      XApplication * currentApp = XApplication::GetApplication();

      //create the window
      MyWindow * myWin = new MyWindow();

      //let the application start
      currentApp->Start();
   }
   catch( XException& e)
   {
      e.ShowError();
   }

   return 0;
}

