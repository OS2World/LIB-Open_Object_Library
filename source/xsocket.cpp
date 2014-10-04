#if defined( __WATCOMC__ )
#include <types.h> // TBO
#endif

#include "XSocket.h"
#if !defined( __IBMCPP__ ) & !defined( __WATCOMC__ )
   #include "io.h"
#endif

/*@
@class XSocket
@parent XIO
@type overview
@symbol _
*/


/*@
@class XClientSocket
@parent XSocket
@type overview
@symbol _
*/

/*@
@class XServerSocket
@parent XSocket
@type overview
@symbol _
*/

/*
 * these two functions are needed for Watcom compatability
 */
#ifdef __WATCOMC__
unsigned long _System lswap( unsigned long in )
{
        unsigned long out;

        swab( (char*)&in, (char*)&out, sizeof( unsigned long ) );
        return out;
}

unsigned short _System bswap( unsigned short in )
{
        unsigned short out;

        swab( (char*)&in, (char*)&out, sizeof( unsigned short ) );
        return out;
}
#endif



static BOOL flInit = FALSE;


/*@ XSocket::XSocket()
@group constructors/destructors
@remarks Constructor for the socket class; initializes the socket system
*/
XSocket::XSocket( void ) : phost( NULL ), XIO()
{
        if( !flInit )
        {
#if defined(__IBMCPP__) || defined(__WATCOMC__)
                if( 0 != sock_init() )
                        OOLThrow( "XSocket::XSocket() - error initializing sockets", sock_errno() );
#endif
                flInit = TRUE;
        } // if

        handle = 0;
} // Socket::Socket


/*@ XSocket::~XSocket()
@group constructors/destructors
@remarksdestructor for the socket class
*/
XSocket::~XSocket()
{
        Close();
}


/*@ XSocket::Open( int net, int typ, int y  )
@group Open/Close
@remarks opens a socket and throws an exception in case of an error.
@parameters
<t 'ø' c=2>
øint net    ø
øint typ    ø
øint y      ø
</t>
*/
void XSocket::Open( int net, int typ, int y )
{
        int x;

        if( -1 == (x = socket( net, typ, y )) )
#if defined(__IBMCPP__) || defined(__WATCOMC__)
                OOLThrow( "XSocket::Open() - error opening a socket", sock_errno() );
#else
                OOLThrow( "XSocket::Open() - error opening a socket",errno );
#endif
        handle = x;
} // XSocket::Open


/*@ XSocket::Close()
@group Open/Close
@remarks closes the socket and throws XException in case of an error.
 */
void XSocket::Close( void )
{
#if defined(__IBMCPP__) || defined(__WATCOMC__)
        if( 0 != soclose( handle ) )
                OOLThrow( "XSocket::Close() - error closing socket", sock_errno() );
#else
        if( 0 != close( handle ) )
                OOLThrow( "XSocket::Close() - error closing socket", errno );
#endif
} // XSocket::Close


/*@ XSocket::GetHostname()
@group Host
@remarks Get the standard hostname for the local host machine; throws an
                          exception in case of an error.
@returns XString
*/
XString XSocket::GetHostname( void )
{
        XString str;

        if( 0 != gethostname( str.GetBuffer( 100 ), 100 ) )
        {
                str.ReleaseBuffer();
#if defined(__IBMCPP__) || defined(__WATCOMC__)
                OOLThrow( "XSocket::GetHostname() - error getting local hostname", sock_errno() );
#else
                OOLThrow( "XSocket::GetHostname() - error getting local hostname",errno );
#endif
        } // if

        str.ReleaseBuffer();

        return str;
} // XSocket::GetHostname


/*@ XSocket::GetHostByName(const char*)
@group Host
@remarks sets information about a host specified by an Internet address;
                            throws an exception in case of an error
@parameters const char*
*/
void XSocket::GetHostByName( const char* hostname )
{
        struct hostent* p = gethostbyname( (char*)hostname );

        if( !p )
#if defined(__IBMCPP__) || defined(__WATCOMC__)
                OOLThrow( "XSocket::GetHostByName() - error getting host", sock_errno() );
#else
                OOLThrow( "XSocket::GetHostByName() - error getting host",errno );
#endif
        SetHostent( p );
} // XSocket::GetHostByName


/*@ XSocket::GetHostByAddr( const char* hostname, int addrfam )
@group Host
@remarks sets information about a host specified by an Internet address;
                           throws an exception in case of an error.
                           The difference to GetHostByName is that the host must be specified
                           as a 32-bit Internet address in network-byte order (eg. 192.32.4.10).
<t 'ø' c=2>
øconst char* hostname   ø
øint addr               ø
</t>
*/
void XSocket::GetHostByAddr( const char* hostname, int addrfam )
{
        struct hostent* p = gethostbyaddr( (char*)hostname, strlen( hostname ), addrfam );

        if( !p )
#if defined(__IBMCPP__) || defined(__WATCOMC__)
                OOLThrow( "XSocket::GetHostByAddr() - error getting host", sock_errno() );
#else
                OOLThrow( "XSocket::GetHostByAddr() - error getting host",errno );
#endif
        SetHostent( p );
} // XSocket::GetHostByAddr


/*@ XSocket::SetValidInAddress( ULONG in_addr, int addrfam )
@ group Host
@remarks Sets an address of a host whose request will be answered; other
                               hosts will be rejected.
@parameters
<t 'ø' c=2>
øULONG in_addr    ø
øint addrfam      ø
</t>
*/
void XSocket::SetInAddress( ULONG in_addr, int addrfam )
{
        name.sin_addr.s_addr = in_addr;
        name.sin_family = addrfam;
} // XSocket::SetInAddrType


/*@ XSocket::SetInAddress(int)
@group Adress
@remarks Sets the socket address.
@parameters int
*/
void XSocket::SetInAddress( int addrfam )
{

        memcpy( &name.sin_addr, phost->h_addr, phost->h_length );
        name.sin_family = addrfam;
} // XSocket::SetInAddress


/*@ XSocket::SetInPort(int)
@group Port
@remarks Sets the socket port.
@parameters int
*/
void XSocket::SetInPort( int port )
{
        name.sin_port = htons( port );
} // XSocket::SetInPort


/*@ XSocket::Write(const void*, ULONG)
@group Read/Write
@remarks Sends a buffer; throws an exception in case of an error
*/
ULONG XSocket::_Write( const void * buffer, const ULONG sizeofbuffer ) const
{
        int l = send( handle, (char*)buffer, sizeofbuffer, 0 );

        if( -1 == l )
#if defined(__IBMCPP__) || defined(__WATCOMC__)
                OOLThrow( "XSocket::Write() - error sending data", sock_errno() );
#else
                OOLThrow( "XSocket::Write() - error sending data",errno );
#endif
        return (ULONG)l;
} // XSocket::Write


/*@ XSocket::Read(void*, ULONG)
@group Read/Write
@remarks Receives data; throws an exception in case of an error.
                   Returns the number of bytes of the received data.
*/
ULONG XSocket::_Read( void * buffer, const ULONG sizeofbuffer )
{
        int l = recv( handle, (char*)buffer, sizeofbuffer, 0 );

        if( -1 == l )
#if defined(__IBMCPP__) || defined(__WATCOMC__)
                OOLThrow( "XSocket::Read() - error receiving data", sock_errno() );
#else
                OOLThrow( "XSocket::Read() - error receiving data",errno );
#endif
        return (ULONG)l;
} // XSocket::Read





/*@ XClientSocket::Connect()
@group Misc
@remarks Requests a connection to a remote host; throws an exception
                            in case of an error.
*/
void XClientSocket::Connect( void )
{
        if( 0 != connect( handle, (struct sockaddr*)&name, sizeof( name ) ) )
#if defined(__IBMCPP__) || defined(__WATCOMC__)
                OOLThrow( "XClientSocket::Connect() - error connecting to remote host", sock_errno() );
#else
                OOLThrow( "XClientSocket::Connect() - error connecting to remote host",errno );
#endif
} // XClientSocket::Connect





/*@ XServerSocket::Bind()
@group Misc
@remarks Binds a local name to the socket.
*/
void XServerSocket::Bind( void )
{
        if( 0 != bind( handle, (struct sockaddr*)&name, sizeof( name ) ) )
#if defined(__IBMCPP__) || defined(__WATCOMC__)
                OOLThrow( "XServerSocket::Bind() - error binding socket", sock_errno() );
#else
                OOLThrow( "XServerSocket::Bind() - error binding socket",errno );
#endif
} // XServerSocket:: Bind


/*@ XServerSocket::Listen()
@group Misc
@remarks Completes binding and creates a connection request queue for
                           incoming connection requests.
*/
void XServerSocket::Listen( int backlog )
{
        if( 0 != listen( handle, backlog ) )
#if defined(__IBMCPP__) || defined(__WATCOMC__)
                OOLThrow( "XServerSocket::Listen() - error", sock_errno() );
#else
                OOLThrow( "XServerSocket::Listen() - error",errno );
#endif
} // XServerSocket::Listen


/*@ XServerSocket::Accept()
@group Misc
@remarks Accepts a connection request from a remote host and returns a
                          socket where the server can read from and write to.
                           Please remember to delete the returned socket after usage!!
@returns XSocket*
*/
XSocket* XServerSocket::Accept( void )
{
        XSocket* soc = new XSocket();
        int     size = sizeof( soc->client );
        int     x    = accept( handle, &soc->client, &size );

        if( -1 == x )
        {
                delete soc;
#if defined(__IBMCPP__) || defined(__WATCOMC__)
                OOLThrow( "XServerSocket::Accept() - error accepting connection", sock_errno() );
#else
                OOLThrow( "XServerSocket::Accept() - error accepting connection", errno );
#endif
        } // if

        soc->SetSocket( x );
        return soc;
} // XServerSocket::Accept

