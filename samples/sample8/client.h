#define INCL_OOL_WIN
#include "ool.h"

#include "resource.h"

class MyDDE;

class MyAppWindow: public XModelessDialog
{
   public:
      MyAppWindow();
      ~MyAppWindow();
   private:
      MyDDE * dde;
};
