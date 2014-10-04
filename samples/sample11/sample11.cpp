#include "sample11.h"

MyWindow::MyWindow() : XFrameWindow( ID_SAMPLE10, "Java Applet" ) 
{
        XRect rect( 100, 200, 510, 215 );

        pApplet = new XJavaApplet( this, &rect,
                                   "file:///./sample11.html",
                                   "SpreadSheet"                   );
        SetSize( &rect );
        SetClient( pApplet );
        pApplet->Start();
}


void main( int argc, char** argv )
{
	XJavaEnvironment * pJavaEnv = new XJavaEnvironment();
	pJavaEnv->GetMessageHandler()->ShowConsole();

	MyWindow * pMyWin = new MyWindow();

	XApplication::GetApplication()->Start();
	delete pJavaEnv;
}

