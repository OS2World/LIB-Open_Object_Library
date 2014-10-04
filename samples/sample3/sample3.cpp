#include "sample3.h"

#include <stdlib.h>

SHORT volume = 50;


//this sample support bubble-help done like in sample2
class MyBubble: public XBubbleHelp
{
   public:
      MyBubble( XWindow*w);
      BOOL SetMsgText( ULONG windowID );
};


MyBubble :: MyBubble( XWindow*w): XBubbleHelp( w)
{
   XFont * font = new XFont(GetGraphicDevice(), "Tms Rmn", 9);
   GetTextObject()->SetFont( font );
}


//set the text for the controls
BOOL MyBubble :: SetMsgText( ULONG windowID )
{
   switch( windowID)
      {
         case IDM_PLAY:
            SetText( "Play a loaded file");
            break;
         case IDM_HALT:
            SetText("Stop playing a file");
            break;
         case IDM_BACK:
            SetText("Seek to the beginning of the file");
            break;
         case IDM_PLUS:
            SetText("more volume");
            break;
         case IDM_MIN:
            SetText( "less volume");
            break;
         case IDM_SEL:
            SetText( "load a file");
            break;
         default:
            return FALSE;
      }
   return TRUE;
}


MyAppWindow :: MyAppWindow( ): XFrameWindow( IDM_MAIN , "Sample3 - Videoplayer", XFrameWindow::defaultDialogStyle | FRM_TASKLIST | FRM_ICON)
{
   //we create a toolbar for the buttons at the top...
   XToolBar * toolBar = new XToolBar( this, TB_TOP);

   //now create pushbuttons with bitmaps, the bitmaps are in the resources of the exe
   XRect rec( 0, 0, 20, 20);

   back = new XPushButton( toolBar, rec, (USHORT) IDM_BACK, BU_BITMAP);
   play = new XPushButton( toolBar, rec, (USHORT) IDM_PLAY, BU_BITMAP);
   halt = new XPushButton( toolBar, rec, (USHORT) IDM_HALT, BU_BITMAP);
   mini = new XPushButton( toolBar, rec, (USHORT) IDM_MIN, BU_BITMAP);
   plus = new XPushButton( toolBar, rec, (USHORT) IDM_PLUS, BU_BITMAP);
   sel = new XPushButton( toolBar, rec, (USHORT) IDM_SEL, BU_BITMAP);

   //disable buttons
   play->Enable(FALSE);
   halt->Enable(FALSE);
   back->Enable(FALSE);

   //add the pushbuttons to the toolbar, the toolbar will use the size of the first button
   toolBar->AddWindow( play , FALSE, TRUE);
   toolBar->AddWindow( halt );
   toolBar->AddWindow( back );
   toolBar->AddWindow( plus, TRUE );
   toolBar->AddWindow( mini );
   toolBar->AddWindow( sel, TRUE );

   // set the size of the window and display it
   XColor col( COL_BLACK);
   SetBackgroundColor( &col);

   XRect rect( 150, 150, 280, 230);
   SetSize(&rect);

   Show();

   // know we generate a videoplayer and open it
   video = new XVideo ( this ); //this is the needed REIHENFOLGE

   // if you donït set the viewport like this, the default ULTIMOTION-window
   // will be opened, but our window will receive the media-control messages
   // (see below function DoControl() )
   if( video->SetViewPort( this) != 0)
      XMessageBox( "cannot set viewport");

   //create the bubblehelp
   MyBubble * bubble = new MyBubble( toolBar);

   Activate();
}


BOOL MyAppWindow :: QueryForClose( void )
{
   /* we MUST close the video and destroy it ourselves */
//   video->CloseDevice();
   delete video;
   return TRUE;
}


void MyAppWindow :: Draw( void )
{
   FillBackground( );
}


/* here the command of the toolbar are posted */
BOOL MyAppWindow :: DoCommand( LONG com)
{
  switch(com)
     {
         case IDM_SEL:
            {
               XFileDialog dlg( this, "c:\\mmos2\\movies\\*.AVI");
               if( dlg.GetCommand() == DID_OK)
                  {
                     XString s;
                     dlg.GetFileName( &s );
                     if( video->SetDataFile( (char*) s ) != 0)
                        XMessageBox( "cannot load video file");
                     play->Enable();
                  }
            }
            return TRUE;
         case IDM_PLAY:
            Activate();
            back->Enable();
            halt->Enable();
            play->Enable(FALSE);
            if( video->Play() != 0)
               XMessageBox( "cannot play video-device");
            return TRUE;
         case IDM_BACK:
            video->Rewind();
            return TRUE;
         case IDM_HALT:
            video->Pause();
            return TRUE;
         case IDM_PLUS:
            if( volume < 100)
               {
                  volume += 10;
                  if( video->SetVolume( volume ) != 0)
                     XMessageBox( "cannot set volume");
               }
            if(volume == 100)
               plus->Enable(FALSE);
            if(volume == 10)
               mini->Enable();
            return TRUE;
         case IDM_MIN:
            if( volume > 0)
               {
                  volume -= 10;
                  video->SetVolume( volume );
               }
            if(volume == 0)
               mini->Enable(FALSE);
            if(volume == 90)
               plus->Enable();
            return TRUE;
     }
   return FALSE;
}


void MyAppWindow :: DoControl( XControlEvent * event)
{
   //here the control-messages are posted, we only care about media-messages */
   switch( event->GetEventID())
      {
         case MEDIA_PLAYED: //end of file reached
            play->Enable(FALSE);
            halt->Enable(FALSE);
            return;
         case MEDIA_PAUSED: //video stopped
            halt->Enable(FALSE);
            play->Enable();
            return;
         case MEDIA_REWINDED: //file has been seeked to start
            halt->Enable(FALSE);
            back->Enable(FALSE);
            play->Enable();
            return;
      }
}


void main ( void)
{
   try
   {
      MyAppWindow * window = new MyAppWindow( );   //create new framewindow (see above)
      XApplication::GetApplication()->Start();      //let the application work
   }
   catch( XException e)
   {
      e.ShowError();
   }
}
