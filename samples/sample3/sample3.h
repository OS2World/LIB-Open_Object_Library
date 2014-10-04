/* the nly function of sample3 it to play videofiles, and the only thing to learn */
/* is how to use the multimedia-classes */

#define INCL_OOL
#include "ool.h"

#include "resource.h"


class MyAppWindow: public XFrameWindow
{
   public:
      MyAppWindow( );
      BOOL DoCommand( LONG);
      void DoControl( XControlEvent*);
      void Draw( void );
      BOOL QueryForClose( void );
   private:
      XPushButton * back, *halt, *play, *plus, *mini, *sel;
      XVideo * video;
};


