#define INCL_DOSDEVIOCTL
#include "xcomport.h"

ULONG XDevice::Open(const char *filename, const ULONG modeopen, const ULONG modeAccess)
{
   ULONG aktion;

   return DosOpen((PSZ) filename, &handle, &aktion, 0, FILE_NORMAL, FILE_OPEN, modeopen|modeAccess, (PEAOP2) NULL);
}


ULONG XComPort::Open(ULONG port, const ULONG modeopen, const ULONG modeAccess)
{
    XString portString ="COM";
    portString += (LONG) port;
    return XDevice::Open((char*) portString, modeopen, modeAccess);
}


ULONG XComPort::SetBaudRate( const USHORT baud ) const
{
    const usBPS = baud;
    ULONG ulParmLen = 2;

    return DosDevIOCtl( handle,
                        IOCTL_ASYNC,
                        ASYNC_SETBAUDRATE,
                        (PULONG) &usBPS,
                        sizeof(usBPS),
                        &ulParmLen,
                        NULL,
                        0,
                        NULL);
}


UCHAR XComPort::GetStatus() const
{
     UCHAR   ucStatus;
     ULONG   ulStatusLen;

     DosDevIOCtl(handle,
                      IOCTL_ASYNC,
                      ASYNC_GETCOMMSTATUS,
                      NULL,
                      0,
                      NULL,
                      (PULONG) &ucStatus,
                      sizeof(ucStatus),
                      &ulStatusLen);
    return ucStatus;
}

