#define INCL_OOL_WIN
#include "ool.h"

class MyAppWindow: public XFrameWindow
{
   public:
      MyAppWindow();
      BOOL QueryForClose();
      void Draw( void );
      BOOL DoCommand(LONG);
   private:
      XToolBar * toolBar;
      XGraphicDevice * graphWindow, * graphMemory;
      XLayer layer;
};

