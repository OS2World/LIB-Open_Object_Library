#define INCL_OOL_WIN
#include "ool.h"

#include "resource.h"


class MyAppWindow: public XScrollWindow
{
   public:
     MyAppWindow( );
     ~MyAppWindow();
     BOOL DoCommand( LONG);
     void Draw( void );
};

