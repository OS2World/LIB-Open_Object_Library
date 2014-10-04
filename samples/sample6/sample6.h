/* sample6 shows the behavior of some controls like XValueSet, XSlider etc. */

#define INCL_OOL_WIN
#include "ool.h"

#include "resource.h"

class MyAppWindow: public XFrameWindow
{
      BOOL doorOpen;
      XBitmap * map;
      XUserButton * btn;
      XStaticText * text;
      XPie * pie;
   public:
      MyAppWindow( );
      ~MyAppWindow();
      void DoControl( XControlEvent*);
      void Draw( void );
      BOOL QueryForClose( void );
};

