#ifndef __XCONSOLE_H__
#define __XCONSOLE_H__

#include "xfrmwnd.h"
#include "xmle.h"

class XUserButton;

// TBO
class _export_ XConsole: public XFrameWindow
{
   friend class ConsoleThread;
      ConsoleThread * thread;
      XMultiLineEdit * mle;
      XConsole(char * f, char * v);

      char *fn, *va;
      HWND titleBar;
      XUserButton * titleBtn;
   public:
      static void CheckConsole(int argc, void ** argv);
      BOOL DoCommand (LONG msg);
      void DoSize (XSize* s);
};

#define debugMessage(e) fputs( e, stderr); fputs("\n", stderr); fflush(stderr);

#endif