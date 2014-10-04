#define INCL_OOL_WIN
#include "ool.h"


#include "resource.h"


class SampleWindow: public XFrameWindow
{
   public:
      SampleWindow();
      ~SampleWindow();
      BOOL DoCommand( LONG);
      void DoControl( XControlEvent*);
      void Draw( void );
      XHelpInstance * help;
};

