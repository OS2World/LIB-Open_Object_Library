#ifndef __OOL_XDEVICE_H__
#define __OOL_XDEVICE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDevice                               */
/*                       derived from: XIO                                   */
/*                        last update: 7/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xio.h"


class _export_ XDevice: public XIO
{
    public:
        XDevice() { };
        ~XDevice( ) { Close(); }
        ULONG Open( const char * deviceName, const ULONG modeForAcces = XFILE_READWRITE, const ULONG modeShare = XFILE_SHARE_DENYREADWRITE);
        ULONG ControlDevice();
        LONG ResetBuffer( void ) const { return DosResetBuffer(handle); }
};

#endif
