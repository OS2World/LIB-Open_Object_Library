#include "xvirtscr.h"


/*DOC
CLASS    XVirtualScreen
FUNCTION XVirtualScreen
GROUP    x-games/constrcutors/destructors
REMARKS  Creates a virtual screen. The only color format that is being supported is FOURCC_LUT8.    
*/
XVirtualScreen::XVirtualScreen( USHORT usWidth, USHORT usHeight ) : XObject()
{
        this->usWidth        = usWidth;
        this->usHeight       = usHeight;

        // allocate memory
        if( NULL == (pbBuffer = (BYTE*)new BYTE[usWidth * usHeight]) )
                throw XException( "XVirtualScreen: out of memory",
                                  XGameException::XGERR_OUT_OF_MEMORY );
} // XVirtualScreen::XVirtualScreen


/*DOC
CLASS    XVirtualScreen
FUNCTION ~XVirtualScreen
GROUP    x-games/constrcutors/destructors
REMARKS  Destructor of the virtual screen class.
*/
XVirtualScreen::~XVirtualScreen()
{
        delete pbBuffer;
} // XVirtualScreen::~XVirtualScreen


/*DOC
CLASS    XVirtualScreen
FUNCTION GetWidth
GROUP    x-games
REMARKS  Returns the width of the virtual screen.
RETURNS  USHORT usWidth
*/

/*DOC
CLASS    XVirtualScreen
FUNCTION GetHeight
GROUP    x-games
REMARKS  Returns the height of the virtual screen.
RETURNS  USHORT usHeight
*/

/*DOC
CLASS    XVirtualScreen
FUNCTION GetBuffer
GROUP    x-games
REMARKS  Returns the pointer to the buffer of the virtual screen.
RETURNS  BYTE* pbBuffer
*/

