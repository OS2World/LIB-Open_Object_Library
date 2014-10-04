#include "xblitter.h"


/*DOC
CLASS    XBlitter
FUNCTION XBlitter
GROUP    x-games/constructors/destructor
REMARKS  Makes basic initialization of the blitter; throws a XGameException in case of an error.
*/
XBlitter::XBlitter( void ) : XObject()
{
        ULONG rc;

        // Get the screen capabilities
        DiveCaps.pFormatData    = fccFormats;
        DiveCaps.ulFormatLength = 120;
        DiveCaps.ulStructLen = sizeof( DIVE_CAPS );
        if( DiveQueryCaps ( &DiveCaps, DIVE_BUFFER_SCREEN ) )
                throw XGameException( "XBlitter: unable to query screen capabilities",
                                      XGameException::XGERR_SCREEN_CAPS                 );

        // we need a color depth of at least 8 bits per pixel
        if( DiveCaps.ulDepth < 8 )
                throw XGameException( "XBlitter: a color mode with at least 256 colors is required",
                                      XGameException::XGERR_COLOR_DEPTH                             );

        // now open a DIVE instance
        if( (rc = DiveOpen( &hDive, FALSE, NULL )) != 0 ) //()!=, svb
                throw XGameException( "XBlitter: unable to open a DIVE instance", rc );
} // XBlitter::XBlitter


/*DOC
CLASS    XBlitter
FUNCTION ~XBlitter
GROUP    x-games/constructors/destructor
REMARKS  Destroys the blitter.
*/
XBlitter::~XBlitter()
{
        DisassociateVirtualScreen();

        // close the DIVE instance
        DiveClose( hDive );
} // XBlitter::~XBlitter


/*DOC
CLASS    XBlitter
FUNCTION AssociateVirtualScreen
GROUP    x-games
REMARKS  Associates a virtual screen to the blitter; throws a XGameException in case of an error.
*/
void XBlitter::AssociateVirtualScreen( XVirtualScreen* pVScr )
{
        ULONG rc;

        DisassociateVirtualScreen();
        pVScreen = pVScr;

        rc = DiveAllocImageBuffer( hDive,
                                   &ulBufferNumber,
                                   FOURCC_LUT8,
                                   pVScreen->GetWidth(),
                                   pVScreen->GetHeight(),
                                   0, pVScreen->GetBuffer()    );
        if( rc )
                throw XGameException( "XBlitter: unable to associate virtual screen", rc );
} // XBlitter::AssociateVirtualScreen


/*DOC
CLASS    XBlitter
FUNCTION DisassociateVirtualScreen
GROUP    x-games
REMARKS  Disassociates the current virtual screen.
*/
void XBlitter::DisassociateVirtualScreen( void )
{
        if( pVScreen && ulBufferNumber )
        {
                DiveFreeImageBuffer( hDive, ulBufferNumber );
                pVScreen       = NULL;
                ulBufferNumber = 0;
        } // if
} // XBlitter::DisassociateVirtualScreen


/*DOC
CLASS    XBlitter
FUNCTION SetPalette
GROUP    x-games
REMARKS  Sets the Palette for this DIVE instance; throws a XGameException in case of an error.
PARAMETERS XPalette* xpal       pointer to a palette object
*/
void XBlitter::SetPalette( XPalette* xpal )
{
        if( DiveCaps.ulDepth > 8 )
                DiveSetSourcePalette( hDive, 0, 256, (PBYTE)xpal->aulData );
        else
        {
                //DiveSetSourcePalette( hDive, 0, 256, (PBYTE)xpal->aulData );
                //DiveSetDestinationPalette( hDive, 0, 256, (PBYTE)xpal->aulData );
                xpal->Enable();
        } // else

        pPalette = xpal;
} // XBlitter::SetSourcePalette


/*DOC
CLASS    XBlitter
FUNCTION Setup
GROUP    x-games
REMARKS  Setups the blitter with new values.
PARAMETERS const PSETUP_BLITTER pSetupBlitter  Pointer to the setup structure; if pSetupBlitter
                                               equals NULL, DiveSetupBlitter will also be called
                                               with NULL as parameter
*/
void XBlitter::Setup( PSETUP_BLITTER pSetupBlitter )
{
        if( NULL == pSetupBlitter )
                DiveSetupBlitter( hDive, NULL );
        else if( pVScreen )
        {
                ULONG rc;

                pSetupBlitter->fccSrcColorFormat = FOURCC_LUT8;
                pSetupBlitter->ulSrcWidth        = pVScreen->GetWidth();
                pSetupBlitter->ulSrcHeight       = pVScreen->GetHeight();
                pSetupBlitter->ulSrcPosX         = 0;
                pSetupBlitter->ulSrcPosY         = 0;

                if( (rc = DiveSetupBlitter( hDive, pSetupBlitter )) != 0 )//()!=, svb
                        throw XGameException( "XBlitter: unable to setup blitter", rc );
        } // else
} // XBlitter::Setup


/*DOC
CLASS    XBlitter
FUNCTION Blit
GROUP    x-games
REMARKS  Blitts the associated virtual screen; throws a XGameException in case of an error.
*/
void XBlitter::Blit( void )
{
        ULONG rc;

        if( pVScreen )
                if( (rc = DiveBlitImage( hDive, ulBufferNumber, DIVE_BUFFER_SCREEN )) != 0 )//()!=, svb
                        throw XGameException( "XBlitter: unable to blit", rc );
} // XBlitter::Blit


/*DOC
CLASS    XBlitter
FUNCTION GetVirtualScreen
GROUP    x-games
REMARKS  Returns the pointer to the associated virtual screen.
RETURNS  XVirtualScreen* pVScreen       pointer to the virtual screen
*/

