#include "xcons.h"
#include "xpmthr.h"
#include "xpipe.h"
#include "xrect.h"
#include <stdlib.h>
#include "xuserbtn.h"
#include "xbitmap.h"
#include "oolres.rh"
#include "xreslib.h"
XConsole * window;

#include<iostream.h>

void XConsole :: CheckConsole(int argc, void ** argv)
{
   XString arguments;
   BOOL console = FALSE;
   for(int i = 1; i < argc; i++)
   {
      XString s = (char*) argv[i];
      if(s == "-console")
      {
         console = TRUE;
      } /* end if */
      else
      {
         arguments += " ";
         arguments += s;
      }
   } /* end for */
   if( console )
   {
      window = new XConsole((char*) argv[0], arguments);
      XApplication::GetApplication()->Start();
      exit(0);
   } /* end if */
}


class ConsoleThread: public XPMThread
{
      XString file;
      XString command;
   public:
      PID pid;
      ConsoleThread( const char * c, const char * f): XPMThread(256000) { file = f; command = c; }
      void Init( void );
};


XConsole :: XConsole(char * f, char *  v):XFrameWindow( 100, "XConsole", XFrameWindow::defaultStyle | FRM_TASKLIST)
{
   titleBtn = NULL;
   window = this,
   mle = new XMultiLineEdit(this, XRect(), 0, MLE_HORZSCROLL|MLE_VERTSCROLL|WIN_VISIBLE, "", "8.Helv");

   SetClient(mle);
   XRect r (10,10,400,300);
   SetSize(&r);

   Show();

   HWND cons = QueryWindow (WIN_PARENT)->GetHandle ();
   titleBar = WinWindowFromID (cons, FID_TITLEBAR);
   SWP swp;
   WinQueryWindowPos (titleBar, &swp);
   WinSetWindowPos (titleBar, 0L, swp.x + swp.cy, swp.y, swp.cx - swp.cy, swp.cy, SWP_MOVE | SWP_SIZE);
   XFrameWindow * owner = (XFrameWindow*) WinQueryWindowULong( cons, 0);
   titleBtn = new XUserButton( owner, XRect(swp.x, swp.y, swp.cy, swp.cy), 9999, BS_NOPOINTERFOCUS | BS_NOBORDER | BS_NOCURSORSELECT);
   XBitmap bmp;
   XResourceLibrary lib("OOLRES");
   XResource res(IDR_RESTART, &lib);
   bmp.Load(&res);
   titleBtn->SetBitmap(&bmp);
   titleBtn->Show();
   thread = new ConsoleThread(fn = f, va = v);
   thread->Run();
}

void XConsole :: DoSize (XSize* s)
{
   SWP swp;
   WinQueryWindowPos (titleBar, &swp);
   WinSetWindowPos (titleBar, 0L, swp.x + swp.cy, swp.y, swp.cx - swp.cy, swp.cy, SWP_MOVE | SWP_SIZE);
   if(titleBtn)
      titleBtn->SetSize(swp.x, swp.y, swp.cy, swp.cy);//WinSetWindowPos (titleBtn, 0L, swp.x, swp.y, 0, 0, SWP_MOVE);
}

BOOL XConsole :: DoCommand (LONG msg) {
   switch (msg) {
   case 9999:
      if (NULL == thread) {
         thread = new ConsoleThread(fn, va);
         thread->Run();
      }
      break;
   }
   return TRUE;
}

void ConsoleThread :: Init( void )
{
   #define PIPESIZE 4096
   ULONG cbRead;
   char achBuf[PIPESIZE];
   HFILE hfSave = -1, hfSave2 = -1, hfNew = XPIPE_STDOUT, hfNew2=XPIPE_STDERROR;

   XString string = "XConsole (c)opyright Stefan von Brauk 1998\n\nStarting:  ";
   string += command;
   string += " ";
   string += file;
   string += "\r\n\r\n";

   window->mle->SetText(string);
   window->mle->ShowFirstChar();
   LONG l = window->mle->GetTextLength();
   window->mle->SelectText( l,l);

   //save stdout-handle
   XPipe::DuplicateHandle( XPIPE_STDOUT, hfSave);
   //save stderror-handle
   XPipe::DuplicateHandle( XPIPE_STDERROR, hfSave2);

   XPipe pipe;
   //open a pipe
   pipe.Open();
   //set the write-handle as stdout
   XPipe::DuplicateHandle( pipe.GetWriteHandle(), hfNew);

   //set the write-handle as stderror
   XPipe::DuplicateHandle( pipe.GetWriteHandle(), hfNew2);

   LONG result;

   ULONG li;

   if( command.Find( li, ".cmd") || command.Find( li, ".CMD"))
   {
      XString commandLine = "/C ";
      commandLine += command;
      commandLine += " ";
      commandLine += file;
      pid = XProcess::ExecuteProg( (char*) "CMD.EXE", (char*) commandLine, NULL, EXE_ASYNC, &result);
   }
   else
      pid = XProcess::ExecuteProg( command, (char*) file, NULL, EXEC_ASYNCRESULT, &result);

   //close write-handle
   XPipe::CloseHandle(pipe.GetWriteHandle() );

   //bring the saved handles back to stdout and stderror
   XPipe::DuplicateHandle( hfSave, hfNew);

   XPipe::DuplicateHandle( hfSave2, hfNew2);

   //close stdout/stderror
   XPipe::CloseHandle(hfSave);
   XPipe::CloseHandle(hfSave2);

   do
   {
      //read data
      cbRead = pipe.Read( achBuf, PIPESIZE);
      achBuf[cbRead] = 0;
      if(cbRead)
      {
         int l = window->mle->GetTextLength();
         window->mle->SelectText( l,l);
         window->mle->InsertString( (char*) achBuf );
      }
   } while(cbRead);

   pipe.Close();

   XString end = "\n\rAction complete, RC=";
   end += (int) result;

   window->mle->InsertString( end );

   window->thread = NULL;

   pid = 0;
   window->thread = NULL;
   Terminate();
}

