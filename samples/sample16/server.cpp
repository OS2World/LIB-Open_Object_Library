#include <fstream.h>
#include <xheaders.h>
#include XSocket_i

void main( int argc, char** argv )
{
        if( argc < 2 )
        {
                cout << "Usage: server <port number>" << endl;
                cout << "e.g.: server 1000" << endl;
        }
        else
        {
                XSocket*       pSocket;
                XServerSocket* pServerSocket;

                try
                {
                        XString message;
                        message.GetBuffer( 40 );

                        pServerSocket = new XServerSocket();
                        pServerSocket->Open();
                        pServerSocket->SetInAddress( (ULONG)INADDR_ANY );
                        pServerSocket->SetInPort( atoi( argv[1] ) );
                        pServerSocket->Bind();
                        pServerSocket->Listen();
                        cout << "Socket setup done. Waiting for connection..." << endl;

                        pSocket = pServerSocket->Accept();
                        cout << "Client connected. Reading data..." << endl;

                        pSocket->Read( message, 39 );
                        cout << "Received data is: " << message << endl;

                        cout << "Sending data back...";
                        XString s = "Test data received!";
                        pSocket->Write( s );
                        cout << "done." << endl;

                        delete pServerSocket;
                }
                catch( XException* x )
                {
                        x->PrintError();
                }
        }
}

