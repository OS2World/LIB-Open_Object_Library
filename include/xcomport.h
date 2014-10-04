#ifndef __OOL_XCOMPORT_H__
#define __OOL_XCOMPORT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XComPort                              */
/*                       derived from: XDevice                               */
/*                        last update: 7/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xdevice.h"


class _export_ XComPort: public XDevice
{
    public:
        XComPort() { };
//////        ~XComPort( ) { Close(); }
        ULONG Open( ULONG port, const ULONG modeForAcces = XFILE_READWRITE, const ULONG modeShare = XFILE_SHARE_DENYREADWRITE);
        ULONG SetBaudRate( const USHORT baud ) const;
        UCHAR GetStatus() const ; 
};

#endif
