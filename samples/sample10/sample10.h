#define INCL_OOL_WIN
#include "ool.h"


class MyAppWindow: public XScrollWindow
{
   public:
      MyAppWindow(char*);
      void Draw( void );
      BOOL DoCommand(LONG);
      XBitmap * bmp;
      void Load( char * filename);
      void DoSize( XSize*);
      void SetBitmapPos();
      virtual void VScroll( LONG );
      virtual void HScroll( LONG );
      void CalcBitmap();
      void InitMenu( XMenu *);
   private:
      XGraphicDevice * graphWindow, * graphMemory;
      BOOL init, loaded, stretch;
      XRect clientRect;
      XSize bmpSize;
};

