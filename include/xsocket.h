
#if !defined( __XSOCKET_H__ )
#define __XSOCKET_H__

#if defined( __WATCOMC__ )
#include <types.h>
#endif

#include <stdlib.h>

#ifndef __IBMCPP__
#include <sys\types.h>
extern "C" {
   #include <sys\socket.h>
   #include <netinet\in.h>
   #include <netdb.h>
}
#endif


#include "xio.h"
#include "xstring.h"
#include "xexcept.h"



// TBO
#if defined( __IBMCPP__ )
#define INET

typedef int*  PINT;
typedef int   DTYP1;

#ifndef __off_t
  #define __off_t
  typedef long off_t;
#endif

typedef char *caddr_t;
typedef long daddr_t;

#define NIL   ((char*) 0)
#define PZERO 0

#define MAXHOSTNAMELEN 120
#ifndef MAXPATHLEN
   #define MAXPATHLEN CCHMAXPATH
#endif
#define MAXSOCKETS 2048


#define SOCBASEERR              10000

// OS/2 SOCKET API definitions of regular Microsoft C 6.0 error constants

#define SOCEPERM                (SOCBASEERR+1)             // Not owner
#define SOCESRCH                (SOCBASEERR+3)             // No such process
#define SOCEINTR                (SOCBASEERR+4)             // Interrupted system call
#define SOCENXIO                (SOCBASEERR+6)             // No such device or address
#define SOCEBADF                (SOCBASEERR+9)             // Bad file number
#define SOCEACCES               (SOCBASEERR+13)            // Permission denied
#define SOCEFAULT               (SOCBASEERR+14)            // Bad address
#define SOCEINVAL               (SOCBASEERR+22)            // Invalid argument
#define SOCEMFILE               (SOCBASEERR+24)            // Too many open files
#define SOCEPIPE                (SOCBASEERR+32)            // Broken pipe

#define SOCEOS2ERR              (SOCBASEERR+100)           // OS/2 Error

// OS/2 SOCKET API definitions of regular BSD error constants

#define SOCEWOULDBLOCK          (SOCBASEERR+35)            // Operation would block
#define SOCEINPROGRESS          (SOCBASEERR+36)            // Operation now in progress
#define SOCEALREADY             (SOCBASEERR+37)            // Operation already in progress
#define SOCENOTSOCK             (SOCBASEERR+38)            // Socket operation on non-socket
#define SOCEDESTADDRREQ         (SOCBASEERR+39)            // Destination address required
#define SOCEMSGSIZE             (SOCBASEERR+40)            // Message too long
#define SOCEPROTOTYPE           (SOCBASEERR+41)            // Protocol wrong type for socket
#define SOCENOPROTOOPT          (SOCBASEERR+42)            // Protocol not available
#define SOCEPROTONOSUPPORT      (SOCBASEERR+43)            // Protocol not supported
#define SOCESOCKTNOSUPPORT      (SOCBASEERR+44)            // Socket type not supported
#define SOCEOPNOTSUPP           (SOCBASEERR+45)            // Operation not supported on socket
#define SOCEPFNOSUPPORT         (SOCBASEERR+46)            // Protocol family not supported
#define SOCEAFNOSUPPORT         (SOCBASEERR+47)            // Address family not supported by protocol family
#define SOCEADDRINUSE           (SOCBASEERR+48)            // Address already in use
#define SOCEADDRNOTAVAIL        (SOCBASEERR+49)            // Can't assign requested address
#define SOCENETDOWN             (SOCBASEERR+50)            // Network is down
#define SOCENETUNREACH          (SOCBASEERR+51)            // Network is unreachable
#define SOCENETRESET            (SOCBASEERR+52)            // Network dropped connection on reset
#define SOCECONNABORTED         (SOCBASEERR+53)            // Software caused connection abort
#define SOCECONNRESET           (SOCBASEERR+54)            // Connection reset by peer
#define SOCENOBUFS              (SOCBASEERR+55)            // No buffer space available
#define SOCEISCONN              (SOCBASEERR+56)            // Socket is already connected
#define SOCENOTCONN             (SOCBASEERR+57)            // Socket is not connected
#define SOCESHUTDOWN            (SOCBASEERR+58)            // Can't send after socket shutdown
#define SOCETOOMANYREFS         (SOCBASEERR+59)            // Too many references: can't splice
#define SOCETIMEDOUT            (SOCBASEERR+60)            // Connection timed out
#define SOCECONNREFUSED         (SOCBASEERR+61)            // Connection refused
#define SOCELOOP                (SOCBASEERR+62)            // Too many levels of symbolic links
#define SOCENAMETOOLONG         (SOCBASEERR+63)            // File name too long
#define SOCEHOSTDOWN            (SOCBASEERR+64)            // Host is down
#define SOCEHOSTUNREACH         (SOCBASEERR+65)            // No route to host
#define SOCENOTEMPTY            (SOCBASEERR+66)            // Directory not empty

// OS/2 SOCKET API errors redefined as regular BSD error constants

#define EWOULDBLOCK             SOCEWOULDBLOCK
#define EINPROGRESS             SOCEINPROGRESS
#define EALREADY                SOCEALREADY
#define ENOTSOCK                SOCENOTSOCK
#define EDESTADDRREQ            SOCEDESTADDRREQ
#define EMSGSIZE                SOCEMSGSIZE
#define EPROTOTYPE              SOCEPROTOTYPE
#define ENOPROTOOPT             SOCENOPROTOOPT
#define EPROTONOSUPPORT         SOCEPROTONOSUPPORT
#define ESOCKTNOSUPPORT         SOCESOCKTNOSUPPORT
#define EOPNOTSUPP              SOCEOPNOTSUPP
#define EPFNOSUPPORT            SOCEPFNOSUPPORT
#define EAFNOSUPPORT            SOCEAFNOSUPPORT
#define EADDRINUSE              SOCEADDRINUSE
#define EADDRNOTAVAIL           SOCEADDRNOTAVAIL
#define ENETDOWN                SOCENETDOWN
#define ENETUNREACH             SOCENETUNREACH
#define ENETRESET               SOCENETRESET
#define ECONNABORTED            SOCECONNABORTED
#define ECONNRESET              SOCECONNRESET
#define ENOBUFS                 SOCENOBUFS
#define EISCONN                 SOCEISCONN
#define ENOTCONN                SOCENOTCONN
#define ESHUTDOWN               SOCESHUTDOWN
#define ETOOMANYREFS            SOCETOOMANYREFS
#define ETIMEDOUT               SOCETIMEDOUT
#define ECONNREFUSED            SOCECONNREFUSED
#define ELOOP                   SOCELOOP
#define ENAMETOOLONG            SOCENAMETOOLONG
#define EHOSTDOWN               SOCEHOSTDOWN
#define EHOSTUNREACH            SOCEHOSTUNREACH
#define ENOTEMPTY               SOCENOTEMPTY

// Address families.

#define AF_UNSPEC       0     // unspecified
#define AF_UNIX         1     // local to host (pipes, portals)
#define AF_INET         2     // internetwork: UDP, TCP, etc.
#define AF_IMPLINK      3     // arpanet imp addresses
#define AF_PUP          4     // pup protocols: e.g. BSP
#define AF_CHAOS        5     // mit CHAOS protocols
#define AF_NS           6     // XEROX NS protocols
#define AF_NBS          7     // nbs protocols
#define AF_ECMA         8     // european computer manufacturers
#define AF_DATAKIT      9     // datakit protocols
#define AF_CCITT        10    // CCITT protocols, X.25 etc
#define AF_SNA          11    // IBM SNA
#define AF_DECnet       12    // DECnet
#define AF_DLI          13    // Direct data link interface
#define AF_LAT          14    // LAT
#define AF_HYLINK       15    // NSC Hyperchannel
#define AF_APPLETALK    16    // Apple Talk
#define AF_NETBIO       17    // NetBIOS-style addresses
#define AF_VOICEVIEW    18    // VoiceView
#define AF_MAX          19

#define INVALID_SOCKET   (int)(~0)
#define SOCKET_ERROR      (-1)

// Definitions related to sockets: types, address families, options.

// Types

#define SOCK_STREAM     1       // stream socket
#define SOCK_DGRAM      2       // datagram socket
#define SOCK_RAW        3       // raw-protocol interface
#define SOCK_RDM        4       // reliably-delivered message
#define SOCK_SEQPACKET  5       // sequenced packet stream

// Option flags per-socket.

#define SO_DEBUG        0x0001          // turn on debugging info recording
#define SO_ACCEPTCONN   0x0002          // socket has had listen()
#define SO_REUSEADDR    0x0004          // allow local address reuse
#define SO_KEEPALIVE    0x0008          // keep connections alive
#define SO_DONTROUTE    0x0010          // just use interface addresses
#define SO_BROADCAST    0x0020          // permit sending of broadcast msgs
#define SO_USELOOPBACK  0x0040          // bypass hardware when possible
#define SO_LINGER       0x0080          // linger on close if data present
#define SO_OOBINLINE    0x0100          // leave received OOB data in line

// Additional options, not kept in so_options.

#define SO_SNDBUF       0x1001          // send buffer size
#define SO_RCVBUF       0x1002          // receive buffer size
#define SO_SNDLOWAT     0x1003          // send low-water mark
#define SO_RCVLOWAT     0x1004          // receive low-water mark
#define SO_SNDTIMEO     0x1005          // send timeout
#define SO_RCVTIMEO     0x1006          // receive timeout
#define SO_ERROR        0x1007          // get error status and clear
#define SO_TYPE         0x1008          // get socket type

// Structure used for manipulating linger option.

struct linger
{
 DTYP1  l_onoff;    // option on/off
 DTYP1  l_linger;   // linger time
};

// Level number for (get/set)sockopt() to apply to socket itself.

#define SOL_SOCKET  0xffff       // options for socket level

// Structure used by kernel to store most
// addresses.

struct sockaddr
{
 USHORT  sa_family;              // address family
 CHAR    sa_data[14];            // up to 14 bytes of direct address
};
typedef sockaddr* psockaddr;

// Structure used by kernel to pass protocol
// information in raw sockets.

struct sockproto
{
 USHORT  sp_family;              // address family
 USHORT  sp_protocol;            // protocol
};

// Protocol families, same as address families for now.

#define PF_UNSPEC       AF_UNSPEC
#define PF_UNIX         AF_UNIX
#define PF_INET         AF_INET
#define PF_IMPLINK      AF_IMPLINK
#define PF_PUP          AF_PUP
#define PF_CHAOS        AF_CHAOS
#define PF_NS           AF_NS
#define PF_NBS          AF_NBS
#define PF_ECMA         AF_ECMA
#define PF_DATAKIT      AF_DATAKIT
#define PF_CCITT        AF_CCITT
#define PF_SNA          AF_SNA
#define PF_DECnet       AF_DECnet
#define PF_DLI          AF_DLI
#define PF_LAT          AF_LAT
#define PF_HYLINK       AF_HYLINK
#define PF_APPLETALK    AF_APPLETALK
#define PF_MAX          AF_MAX

// Maximum queue length specifiable by listen.

#define SOMAXCONN       5

struct iovec
{
 PSZ   iov_base;
 int     iov_len;
};
typedef iovec* piovec;

struct uio
{
 piovec   uio_iov;
 int      uio_iovcnt;
 off_t    uio_offset;
 int      uio_segflg;
 int      uio_resid;
};
typedef uio*   pio;

// Message header for recvmsg and sendmsg calls.

struct msghdr
{
 PSZ      msg_name;               // optional address
 int      msg_namelen;            // size of address
 piovec   msg_iov;                // scatter/gather array
 int      msg_iovlen;             // # elements in msg_iov
 PSZ      msg_accrights;          // access rights sent/received
 int      msg_accrightslen;
};
typedef msghdr*   pmsghdr;

enum uio_rw
{
 UIO_READ,
 UIO_WRITE
};

#define FREAD  1
#define FWRITE 2

#define MSG_OOB         0x1        // process out-of-band data
#define MSG_PEEK        0x2        // peek at incoming message
#define MSG_DONTROUTE   0x4        // send without using routing tables
#define MSG_MAXIOVLEN   16

// Definition for bswap

#define htonl(x)     (lswap(x))
#define ntohl(x)     (lswap(x))
#define htons(x)     (bswap(x))
#define ntohs(x)     (bswap(x))
#define bzero(x,y)   memset((x),'\0',(y))
#define bcopy(x,y,z) memcpy((y),(x),(z))
#define bcmp(x,y,z)  memcmp((y),(x),(z))

//

#define IPPROTO_IP              0               // dummy for IP
#define IPPROTO_ICMP            1               // control message protocol
#define IPPROTO_GGP             3               // gateway^2 (deprecated)
#define IPPROTO_TCP             6               // tcp
#define IPPROTO_EGP             8               // exterior gateway protocol
#define IPPROTO_PUP             12              // pup
#define IPPROTO_UDP             17              // user datagram protocol
#define IPPROTO_IDP             22              // xns idp
#define IPPROTO_RAW             255             // raw IP packet
#define IPPROTO_MAX             256


// Ports < IPPORT_RESERVED are reserved for
// privileged processes (e.g. root).
// Ports > IPPORT_USERRESERVED are reserved
// for servers, not necessarily privileged.

#define IPPORT_RESERVED         1024
#define IPPORT_USERRESERVED     5000

// Link numbers

#define IMPLINK_IP              155
#define IMPLINK_LOWEXPER        156
#define IMPLINK_HIGHEXPER       158


// TBO
struct in_addr {
        unsigned long s_addr;
};


// Definitions of bits in internet address integers.
// On subnets, the decomposition of addresses to host and net parts
// is done according to subnet mask, not the masks here.

#define IN_CLASSA(i)            (((long)(i) & 0x80000000L) == 0)
#define IN_CLASSA_NET           0xff000000L
#define IN_CLASSA_NSHIFT        24
#define IN_CLASSA_HOST          0x00ffffffL
#define IN_CLASSA_MAX           128

#define IN_CLASSB(i)            (((long)(i) & 0xc0000000L) == 0x80000000L)
#define IN_CLASSB_NET           0xffff0000L
#define IN_CLASSB_NSHIFT        16
#define IN_CLASSB_HOST          0x0000ffffL
#define IN_CLASSB_MAX           65536L

#define IN_CLASSC(i)            (((long)(i) & 0xe0000000L) == 0xc0000000L)
#define IN_CLASSC_NET           0xffffff00L
#define IN_CLASSC_NSHIFT        8
#define IN_CLASSC_HOST          0x000000ffL

#define IN_CLASSD(i)            (((long)(i) & 0xf0000000L) == 0xe0000000L)
#define IN_MULTICAST(i)         IN_CLASSD(i)

#define IN_EXPERIMENTAL(i)      (((long)(i) & 0xe0000000L) == 0xe0000000L)
#define IN_BADCLASS(i)          (((long)(i) & 0xf0000000L) == 0xf0000000L)

#define INADDR_ANY              (ULONG)0x00000000L
#define INADDR_BROADCAST        (ULONG)0xffffffffL     // must be masked
#ifndef KERNEL
#define INADDR_NONE             0xffffffffL             // -1 return
#endif

#define IN_LOOPBACKNET          127                     // official!


struct sockaddr_in
{
 short    sin_family;
 USHORT   sin_port;
 in_addr  sin_addr;
 char     sin_zero[8];
};
typedef sockaddr_in* psockaddr_in;

#define SOCADDR_BINDSIZE 16

// Options for use with [gs]etsockopt at the IP level.

#define IP_OPTIONS      1  // set/get IP per-packet options


struct servent;
typedef servent*  pservent;

#define _MAXALIASES     35
#define _MAXADDRS       35
#define _MAXLINELEN     1024
#define _HOSTBUFSIZE    (BUFSIZ + 1)

// Structures returned by network
// data base library.  All addresses
// are supplied in host order, and
// returned in network order (suitable
// for use in system calls).

//typedef struct
struct hostent
{
 PSZ                h_name;       // official name of host
 PSZ*               h_aliases;    // alias list
 DTYP1              h_addrtype;   // host address type
 DTYP1              h_length;     // length of address
 PSZ*               h_addr_list;  // list of addresses from name server
 // TBO
#define h_addr  h_addr_list[0]  /* address, for backward compatiblity */
};// hostent, *phostend;
typedef hostent * phostent;


// Assumption here is that a network number
// fits in 32 bits -- probably a poor one.

struct netent
{
 PSZ    n_name;       // official name of net
 PSZ*   n_aliases;    // alias list
 DTYP1  n_addrtype;   // net address type
 ULONG  n_net;        // network #
};
typedef netent*  pnetent;

struct servent
{
 PSZ    s_name;       // official service name
 PSZ*   s_aliases;    // alias list
 DTYP1  s_port;       // port #
 PSZ    s_proto;      // protocol to use
};
typedef servent*  pservent;

struct protoent
{
 PSZ    p_name;       // official protocol name
 PSZ*   p_aliases;    // alias list
 DTYP1  p_proto;      // protocol #
};
typedef protoent*  pprotoent;

// After a successful call to gethostbyname_r()/gethostbyaddr_r(), the
// structure hostent_data will contain the data to which pointers in
// the hostent structure will point to.

struct hostent_data
{
 in_addr     host_addr;                     // host address pointer
 PSZ         h_addr_ptrs[_MAXADDRS + 1];    // host address
 char        hostaddr[_MAXADDRS];
 char        hostbuf[_HOSTBUFSIZE + 1];     // host data
 PSZ         host_aliases[_MAXALIASES];
 PSZ         host_addrs[2];
 FILE*       hostf;
 int         stayopen;                      // AIX addon
 ULONG       host_addresses[_MAXADDRS];     // As per defect 48367.
};                                          // Actual Addresses.
typedef hostent_data*   phostent_data;

struct servent_data     // should be considered opaque
{
 FILE*       serv_fp;
 char        line[_MAXLINELEN];
 PSZ         serv_aliases[_MAXALIASES];
 int         _serv_stayopen;
};
typedef servent_data*   pservent_data;


// Error return codes from gethostbyname() and gethostbyaddr()
// (left in extern int h_errno).

extern int h_errno;

#define HOST_NOT_FOUND  1          // Authoritative Answer Host not found
#define TRY_AGAIN       2          // Non-Authoritive Host not found, or SERVERFAIL
#define NO_RECOVERY     3          // Non recoverable errors, FORMERR, REFUSED, NOTIMP
#define NO_DATA         4          // Valid name, no data record of requested type
#define NO_ADDRESS      NO_DATA    // no address, look for MX record

#ifdef __cplusplus
  extern "C" {
#endif

ULONG     APIENTRY   inet_addr(const PSZ);
int       APIENTRY   accept(int, psockaddr, PINT);
int       APIENTRY   bind(int, psockaddr, int);
int       APIENTRY   connect(int, psockaddr, int);
int       APIENTRY   getpeername(int, psockaddr, PINT);
int       APIENTRY   getsockname(int, psockaddr, PINT);
int       APIENTRY   getsockopt(int, int, int, PSZ, PINT);
int       APIENTRY   listen(int, int);
int       APIENTRY   recv(int, PSZ, int, int);
int       APIENTRY   recvfrom(int, PSZ, int, int, psockaddr, PINT);
int       APIENTRY   send(int, PSZ, int, int);
int       APIENTRY   sendto(int, PSZ, int, int, psockaddr, int);
int       APIENTRY   shutdown(int, int);
int       APIENTRY   socket(int, int, int);
#ifndef BSD_SELECT
  int     APIENTRY   select(PINT, int, int, int, long);
#endif
int       APIENTRY   gethostid();
int       APIENTRY   recvmsg(int, pmsghdr, int);
int       APIENTRY   sendmsg(int, pmsghdr, int);
int       APIENTRY   setsockopt(int, int, int, PSZ, int);
int       APIENTRY   sock_init();
int       APIENTRY   sock_errno();
int       APIENTRY   psock_errno(PSZ);
int       APIENTRY   soclose(int);
int       APIENTRY   soabort(int);
int       APIENTRY   so_cancel(int);
int       APIENTRY   readv(int, piovec, int);
int       APIENTRY   writev(int, piovec, int);
int       APIENTRY   getinetversion(PSZ);
ULONG     APIENTRY   lswap(ULONG);
USHORT    APIENTRY   bswap(USHORT);
in_addr   APIENTRY   inet_makeaddr(ULONG, ULONG);
ULONG     APIENTRY   inet_network(PSZ);
PSZ       APIENTRY   inet_ntoa(in_addr);
ULONG     APIENTRY   inet_lnaof(in_addr);
ULONG     APIENTRY   inet_netof(in_addr);
int       APIENTRY   gethostbyname_r(PSZ, phostent, phostent_data);
int       APIENTRY   gethostbyaddr_r(PSZ, int, int, phostent, phostent_data);
int       APIENTRY   getservbyname_r(PSZ, PSZ, pservent, pservent_data);
int       APIENTRY   gethostname(PSZ, int );
phostent  APIENTRY   gethostbyname(PSZ);
phostent  APIENTRY   _gethtbyname(PSZ);
phostent  APIENTRY   gethostbyaddr(PSZ, int, int);
phostent  APIENTRY   _gethtbyaddr(PSZ, int, int);
pnetent   APIENTRY   getnetbyname(PSZ);
pnetent   APIENTRY   getnetbyaddr(ULONG, int);
pservent  APIENTRY   getservbyname( PSZ, PSZ);
pservent  APIENTRY   getservbyport( int, PSZ);
pservent  APIENTRY   getservent();
pprotoent APIENTRY   getprotobyname(PSZ);
pprotoent APIENTRY   getprotobynumber(int);
void      APIENTRY   sethostent(int);
phostent  APIENTRY   gethostent();
void      APIENTRY   endhostent();
void      APIENTRY   setnetent(int);
pnetent   APIENTRY   getnetent();
void      APIENTRY   endnetent();
void      APIENTRY   setprotoent(int);
pprotoent APIENTRY   getprotoent();
void      APIENTRY   endprotoent();
void      APIENTRY   setservent(int);
pservent  APIENTRY   getservent();
void      APIENTRY   endservent();


#ifdef __cplusplus
  }
#endif

// tbo
#endif // #if defined( __IBMCPP__ )


class _export_ XSocket : public XIO
{
        friend class XServerSocket;

        protected:
                struct sockaddr_in      name;
                struct sockaddr         client;
                struct hostent*         phost;

        public:
                // ctor/dtor
                         XSocket( void );
                virtual ~XSocket();

                void Open( int net = AF_INET, int typ = SOCK_STREAM, int y = 0 );
                void Close( void );

                int  GetSocket( void )          { return handle; }
                void SetSocket( int sock )      { handle = sock; }

                       void     SetHostent( struct hostent* ph )        { phost = ph; }
                struct hostent* GetHostent( void )                      { return phost; }

                XString GetHostname( void );
                void    GetHostByName( const char* hostname );
                void    GetHostByAddr( const char* hostname, int addrfam = AF_INET );

                void SetInAddress( int addrfam = AF_INET );
                void SetInAddress( ULONG in_addr /*= INADDR_ANY*/, int addrfam = AF_INET );
                void SetInPort( int port );

                ULONG _Read( void * buffer, const ULONG sizeofbuffer);
                ULONG _Write( const void * buffer, const ULONG sizeofbuffer) const;
};


class _export_ XClientSocket : public XSocket
{
        public:
                // ctor/dtor
                         XClientSocket( void ) : XSocket() {}
                virtual ~XClientSocket() {}

                void Connect( void );
};


class _export_ XServerSocket : public XSocket
{
        public:
                // ctor/dtor
                         XServerSocket( void ) : XSocket() {}
                virtual ~XServerSocket() {}

                void     Bind( void );
                void     Listen( int backlog = SOMAXCONN );
                XSocket* Accept( void );
};

#endif
