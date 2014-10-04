#define INCL_GRE_COLORTABLE
#include <xheaders.h>
#include <pmddim.h>
#include "xobject.h"
#include "xgamexc.h"
#include "xpal.h"


/*DOC
CLASS    XPalette
FUNCTION XPalette
GROUP    x-games/constructors/destructor
REMARKS  Default constructor for a palette object. Queries the WPS colors (0-9, 246-255) and stores
         them.
*/
XPalette::XPalette()
{
        int     i;

        // get HPS and HDC of the root window
        hpsDesktop = WinGetPS( HWND_DESKTOP );
        hdcDesktop = GpiQueryDevice( hpsDesktop );

        // get WPS colors
        GpiQueryRealColors( hpsDesktop, 0, 0, 256, (PLONG)aulWPSPalette );
        memcpy( &aulData[0], &aulWPSPalette[0], 256 * sizeof( ULONG ) );
} // XPalette::XPalette


/*DOC
CLASS    XPalette
FUNCTION ~XPalette
GROUP    x-games/constructors/destructor
REMARKS  Destructor.
*/


/*DOC
CLASS    XPalette
FUNCTION SetColors
GROUP    x-games/constructors/destructor
REMARKS  Sets colors for the palette. Please note that the colors 0-9 and 246-255 are used as 
         systems colors by the WPS; so don't override them. 
PARAMETERS       PULONG pulData                 Pointer to an array with the palette data.
                                                The format of the palette data is this:
                                                        (R << 16) + (G << 8) + B
                                                The high order byte must be 0.
           const ULONG  ulStart = 10            First entry to change
           const ULONG  ulNumEntries = 236      Number of entries to change
*/
void XPalette::SetColors( PULONG pulData, const ULONG ulStart, const ULONG ulNumEntries )
{
        memcpy( &aulData[ulStart], pulData, ulNumEntries * sizeof( ULONG ) );
} // XPalette::SetColors


/*DOC
CLASS    XPalette
FUNCTION Enable
GROUP    x-games/constructors/destructor
REMARKS  Enable this palette.
*/
void XPalette::Enable( BOOL flInvalidate )
{
        GpiCreateLogColorTable( hpsDesktop,
                                LCOL_PURECOLOR | LCOL_REALIZABLE,
                                LCOLF_CONSECRGB,
                                0, 256, (PLONG)aulData             );

        if( !GreRealizeColorTable( hdcDesktop ) )
                throw XGameException( "XPalette: unable to set physical palette",
                                      XGameException::XGERR_PALETTE               );

        if( flInvalidate )
                WinInvalidateRect( HWND_DESKTOP, (PRECTL)NULL, TRUE );
} // XPalette::Enable


/*DOC
CLASS    XPalette
FUNCTION Disable
GROUP    x-games/constructors/destructor
REMARKS  Disable this palette.
*/
void XPalette::Disable( BOOL flInvalidate )
{
        GpiCreateLogColorTable( hpsDesktop,
                                LCOL_PURECOLOR | LCOL_REALIZABLE,
                                LCOLF_CONSECRGB,
                                0, 256, (PLONG)aulWPSPalette       );

        if( !GreRealizeColorTable( hdcDesktop ) )
                throw XGameException( "XPalette: unable to set physical palette",
                                      XGameException::XGERR_PALETTE               );

        if( flInvalidate )
                WinInvalidateRect( HWND_DESKTOP, (PRECTL)NULL, TRUE );
} // XPalette::Disable

