#define INCL_OS2MM
#define INCL_MMIOOS2
#define INCL_MMIO_CODEC
#define INCL_MCIOS2
#define INCL_MACHDR

#include "XVideo.h"
#include "XFrmWnd.h"
#include "XMsgBox.h"
#include "XSound.h"
#include "XCntrEvn.h"
#include "xexcept.h"
#include "os2me.h"
#include "xcd.h"

#include <stdlib.h>

//defines for sound-class, it can handle *.mid and *.wav
#define MIDI 1
#define WAVE 2


XMediaWindow :: XMediaWindow(const XFrameWindow * res):XObjectWindow(res)
{
   mediaId = 0;
}


XMediaWindow :: ~XMediaWindow()
{
   if(mediaId)
      CloseDevice();
}


void XMediaWindow :: CloseDevice()
{
   _MCI_GENERIC_PARMS close;
   mciSendCommand( mediaId, MCI_CLOSE, MCI_WAIT, &close, 0);
   mediaId = 0;
}

/*@
@class XMediaWindow
@type overview
@symbol _
*/

/*@
@class XVideo
@parent XMediaWindow
@type overview
@symbol _
*/


/*@ XVideo :: XVideo(const XFrameWindow * ow)
@group constructors/destructors
@remarks Construct a video-window. To use this control you must invoke the multimedia-library!
@parameters XFrameWindow * owner    the owner of the video
*/
XVideo :: XVideo(const XFrameWindow * ow):XMediaWindow(ow)
{
   _MCI_OPEN_PARMS open;
   open.pszDeviceType = (PSZ) "digitalvideo";
   ULONG rc = mciSendCommand(0, MCI_OPEN, MCI_WAIT | MCI_OPEN_SHAREABLE, &open, 0);
   if( rc == 0 )
      mediaId = open.usDeviceID;
   else
      OOLThrow( "cannot open digita-video", rc);
}


/*@
@class XCDPlayer
@parent XMediaWindow
@type overview
@symbol _
*/

/*@ XCDPlayer::XCDPlayer( const XFrameWindow )
@group constructors/destructors
@remarks Opens the CD-Player. To play a record from the CD you must call ReadRecordList()
first, then call PlayRecord()!
@parameters XFrameWindow* owning window
@exceptions If an error ocures an exception of the type XException is thrown
*/
XCDPlayer :: XCDPlayer( const XFrameWindow * w ):XMediaWindow(w)
{
   count = 0;
   records = NULL;

   _MCI_OPEN_PARMS open;
   open.pszDeviceType = (PSZ) "cdaudio";
   ULONG rc = mciSendCommand(0, MCI_OPEN, MCI_WAIT | MCI_OPEN_SHAREABLE, &open, 0);
   if( rc == 0 )
      mediaId = open.usDeviceID;
   else
      OOLThrow( "cannot open cd-player", rc);

}


XCDPlayer :: ~XCDPlayer()
{
   if(records)
      free(records);
}


/*@ XCDPlayer :: ReadRecordList()
@group misc
@remarks Load the list of records on the current CD.
@exceptions If an error ocures an exception of the type XException is thrown
*/
void XCDPlayer :: ReadRecordList()
{
   if(records)
      free(records);
   records = NULL;
   count = 0;

   MCI_TOC_REC tokens[100];
   _MCI_TOC_PARMS params;

   memset( &tokens, 0, sizeof(tokens));
   params.pBuf = tokens;
   params.ulBufSize = sizeof(tokens);

   ULONG rc = mciSendCommand(mediaId, MCI_GETTOC, MCI_WAIT, &params, 0);

   if( rc == MCIERR_DEVICE_NOT_READY)
      OOLThrow( "device is not ready!", rc );
   else if( rc != 0)
      OOLThrow( "cannot get informations about cd-player", rc);

   while( tokens[count].TrackNum > 0 )
      count++;
   records = (cdRecord*) malloc( count * sizeof(cdRecord));
   for(int i = 0; i < count; i++)
   {
      records[i].startAddr = tokens[i].ulStartAddr;
      records[i].endAddr = tokens[i].ulEndAddr;
      records[i].trackNumber = tokens[i].TrackNum;
   }
}


/*@ XCDPlayer :: PlayRecord( USHORT record )
@group misc
@remarks Plays a record
@parameters USHORT zero based index
@returns ULONG errorcode
*/
ULONG XCDPlayer :: PlayRecord( const USHORT record )
{
   if( record < count )
      return Play( records[record].startAddr, records[record].endAddr );
   else
      return 0;
}


/*@ XCDPlayer::GetRecordLength( USHORT )
@group misc
@remarks Get the length of a record
@parameters USHORT zero based index
@returns ULONG time in milliseconds
*/
ULONG XCDPlayer :: GetRecordLength( const USHORT i)
{
   if( i < count )
      return (records[i].endAddr - records[i].startAddr) / 3;
   else
      return 0;
}


LONG XVideo::Restore(void) const
{
   _MCI_RESTORE_PARMS restore;
   mciSendCommand( mediaId, MCI_RESTORE, MCI_WAIT, &restore, 0);
   return 0;

}


/*@ XVideo::SetViewPort(const XFrameWindow * w)
@group misc
@remarks Set a framewindow as the viewport where videos are displayed
@parameters XFrameWindow * window   the window to show the video
@returns LONG resultcode
*/
LONG XVideo::SetViewPort(const XFrameWindow * w) const
{
   _MCI_VID_WINDOW_PARMS window;
   window.hwndDest = owner->GetHandle();
   mciSendCommand( mediaId, MCI_WINDOW, MCI_WAIT | MCI_DGV_WINDOW_HWND, &window, 0);
   return 0;
}


/*@ XMediaWindow::SetVolume(const USHORT volumePercent)
@group misc
@remarks Set volume
@parameters USHORT volume  volume in percent
@returns LONG resultcode
*/
LONG XMediaWindow::SetVolume(const USHORT volumePercent, const USHORT chanel)
{
   _MCI_SET_PARMS set;
   memset(&set, 0, sizeof(set));
   set.ulLevel = volumePercent;
   set.ulAudio = chanel;//MCI_SET_AUDIO_ALL
   return mciSendCommand( mediaId, MCI_SET, MCI_WAIT | MCI_SET_AUDIO, &set, 0);
}

/*@ XMediaWindow::SetDataFile(const char *p)
@group misc
@remarks Play a new file
@parameters char * the file to play
@returns LONG resultcode
*/
LONG XMediaWindow::SetDataFile(const char *p)
{
   _MCI_LOAD_PARMS load;
   load.pszElementName = (PSZ) p;
   return mciSendCommand( mediaId, MCI_LOAD, MCI_WAIT | MCI_OPEN_ELEMENT | MCI_READONLY, &load, 0);
}


/*@ XMediaWindow::Play(void)
@group play functions
@remarks Play the file loaded by SetDataFile
@parameters
<t 'ø' c=2>
øULONG øposition to start (default is 0 = current position)
øULONG øposition to end (default is 0, the whole file/chunk will be played)
</t>
@returns LONG resultcode
*/
LONG XMediaWindow::Play( ULONG from, ULONG to )
{
   ULONG code = MCI_NOTIFY;

   _MCI_PLAY_PARMS play;
   play.hwndCallback = owner->GetHandle();
   if( from )
   {
      code |= MCI_FROM;
      play.ulFrom = from;
   }
   if( to )
   {
      code |= MCI_TO;
      play.ulTo = to;
   }

   mciSendCommand( mediaId, MCI_PLAY, code, &play, MEDIA_PLAYED);
   return 0;
}


/*@ XMediaWindow::Pause(void)
@group play functions
@remarks Pause
@returns LONG resultcode
*/
LONG XMediaWindow::Pause(void)
{
   _MCI_GENERIC_PARMS pause;
   pause.hwndCallback = owner->GetHandle();
   return mciSendCommand( mediaId, MCI_PAUSE, MCI_NOTIFY, &pause, MEDIA_PAUSED);
}


/*@ XMediaWindow::Rewind(void)
@group play functions
@remarks Seek to the beginning
@returns LONG resultcode
*/
LONG XMediaWindow::Rewind(void)
{
   _MCI_SEEK_PARMS seek;
   seek.hwndCallback = owner->GetHandle();
   return mciSendCommand( mediaId, MCI_SEEK, MCI_NOTIFY | MCI_TO_START, &seek, MEDIA_REWINDED);
}


LONG XMediaWindow::Stop(void)
{
   _MCI_GENERIC_PARMS rewinded;
   rewinded.hwndCallback = owner->GetHandle();
   return mciSendCommand( mediaId, MCI_STOP, MCI_NOTIFY, &rewinded, MEDIA_STOPED);
}


LONG XMediaWindow::Acquire(void)
{
/**
   strcpy(commandString, "acquire ");
   strcat(commandString, idString);
   return SendMCIString();
***/
   return 0;
}


/*@
@class XSound
@parent XMediaWindow
@type overview
@symbol _
*/


LONG XSound::SetDataFile(const char *p)
{
   if (((strstr(p, ".MID") || strstr(p, ".mid")) && type != MIDI) || ((strstr(p, ".WAV") || strstr(p, ".wav")) && type != WAVE))
   {
      if (type != 0)
      {
         type = 0;
         CloseDevice();
      }

      if (p)
      {
         if (strstr(p, ".WAV"))
            type = WAVE;
         else if (strstr(p, ".MID"))
            type = MIDI;
      }
      if(type)
      {
         _MCI_OPEN_PARMS open;
         if(type == WAVE)
            open.pszDeviceType = (PSZ) "waveaudio";
         if(type == MIDI)
            open.pszDeviceType = (PSZ) "sequencer";

         ULONG rc = mciSendCommand(0, MCI_OPEN, MCI_WAIT | MCI_OPEN_SHAREABLE, &open, 0);
         if( rc == 0 )
            mediaId = open.usDeviceID;
         else
            OOLThrow( "cannot open audio", rc);
      }
   }
   return XMediaWindow::SetDataFile(p);
}


/*@ XSound :: ~XSound()
@group constructors/destructors
@remarks The destructor MUST be called before the message-loop of an aplication is
destroyed. Therefore overwrite QueryForClose() of the owning framewindow and call
the destructor of XSound in that function.
*/


/*@ XSound :: XSound(const XFrameWindow * w)
@group constructors/destructors
@remarks Construct a sound-device. XSound can play MIDI and WAV-files. To use this control you must invoke the multimedia-library!
@parameters XFrameWindow * owner    the owner of the device
*/


/*@ XVideo :: ~XVideo()
@group construtors/destructors
@remarks The destructor MUST be called before the message-loop of an aplication is
destroyed. Therefore overwrite QueryForClose() of the owning framewindow and call
the destructor of XVideo in that function.
*/

