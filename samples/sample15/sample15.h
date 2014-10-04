#define INCL_OOL
#include "ool.h"

#include "resource.h"

class XMenuBar;

class MyServer: public XMDIServer
{
   public:
      MyServer():XMDIServer(IDM_MENU, "Sample 15: MDI Application", XFrameWindow::defaultStyle | FRM_ICON | FRM_TASKLIST | FRM_CENTER | FRM_MENU | FRM_ACCELTABLE)
      {
         XRect rect(100, 100, 500, 400);
         SetSize( &rect);
      }
      void SetEmptyMenu(XMenuBar*);
      BOOL DoCommand( LONG command);
};

