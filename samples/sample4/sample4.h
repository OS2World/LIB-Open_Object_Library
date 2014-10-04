/* the nly function of sample3 it to play videofiles, and the only thing to learn */
/* is how to use the multimedia-classes */

#define INCL_OOL_WIN
#include "ool.h"

#include "resource.h"


class MyAppWindow: public XFrameWindow
{
   public:
      MyAppWindow();
      ~MyAppWindow();
      void Draw();
};


#define CONT_LEFT  300
#define CONT_RIGHT 400
