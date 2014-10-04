#define INCL_OOL_WIN
#define INCL_OOL_JAVA

#include "ool.h"
#include "resource.h"



// Main frame window
class MyWindow : public XFrameWindow
{
        private:
                XJavaApplet* pApplet;
        public:
                // ctor/dtor
                         MyWindow( );
};


