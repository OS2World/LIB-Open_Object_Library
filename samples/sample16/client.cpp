#include <fstream.h>
#include <xheaders.h>
#include XSocket_i

void main( int argc, char** argv )
{
        if( argc < 3 )
        {
                cout << "Usage: client <hostname> <port number>" << endl;
                cout << "e.g.: client localhost 1000" << endl;
        }
        else
        {
                XClientSocket* pClientSocket;

                try
                {
                        XString message;
                        message.GetBuffer( 40 );

                        cout << "Connecting to server...";
                        pClientSocket = new XClientSocket();
                        pClientSocket->Open();
                        pClientSocket->GetHostByName( argv[1] );
                        pClientSocket->SetInAddress();
                        pClientSocket->SetInPort( atoi( argv[2] ) );
                        pClientSocket->Connect();
                        cout << "done." << endl;

                        cout << "Sending data...";
                        XString s = "This is a test!";
                        pClientSocket->XIO::Write( s );
                        cout << "done." << endl;

                        cout << "Received data: ";
                        pClientSocket->Read( message, 39 );
                        cout << message << endl;

                        delete pClientSocket;
                }
                catch( XException* x )
                {
                        x->PrintError();
                }
        }
}

