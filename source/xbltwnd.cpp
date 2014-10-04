#include "xbltwnd.h"


/*
 * NewWndProc() - this function handles messages that are specific to blitter windows
 */
MRESULT EXPENTRY NewWndProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
        XBlitterWindow* w = (XBlitterWindow*)WinQueryWindowPtr( hwnd, 0 );

         //obacht, svb
        if(w == NULL)
            return 0;

        if( WM_CLOSE == msg)
            w->Deactivate();

        // at first call old procedure
        MRESULT rc = (w->pOldWndProc)( hwnd, msg, mp1, mp2 );

        switch( msg )
        {
                case WM_VRNDISABLED:
                {
                        w->pBlitter->Setup( NULL );
                        return (MRESULT)FALSE;
                } // case WM_VRNDISABLED

                case WM_VRNENABLED:
                {
                        HPS             hps;
                        HRGN            hrgn;
                        RECTL           rcls[50];
                        SWP             swp;
                        POINTL          pointl;
                        SETUP_BLITTER   SetupBlitter;
                        RGNRECT         rgnCtl;

                        if( !(hps = WinGetPS( hwnd )) )
                                return (MRESULT) FALSE;

                        hrgn = GpiCreateRegion( hps, 0L, NULL );
                        if( hrgn )
                        {
                                // NOTE: If mp1 is zero, then this was just a move message.
                                // Illustrate the visible region on a WM_VRNENABLE.
                                WinQueryVisibleRegion( hwnd, hrgn );
                                rgnCtl.ircStart     = 0;
                                rgnCtl.crc          = 50;
                                rgnCtl.ulDirection  = 1;

                                // Get the all ORed rectangles
                                if( GpiQueryRegionRects( hps, hrgn, NULL, &rgnCtl, rcls ) )
                                {
                                        // Now find the window position and size, relative to parent.
                                        WinQueryWindowPos( hwnd, &swp );

                                        // Convert the point to offset from desktop lower left.
                                        pointl.x = swp.x;
                                        pointl.y = swp.y;
                                        WinMapWindowPoints ( hwnd, HWND_DESKTOP, &pointl, 1 );

                                        // Tell DIVE about the new settings.
                                        SetupBlitter.ulStructLen = sizeof ( SETUP_BLITTER );

                                        // these values will be set in XBlitter::Setup
                                        SetupBlitter.fccSrcColorFormat = 0;
                                        SetupBlitter.ulSrcWidth   = 0;
                                        SetupBlitter.ulSrcHeight  = 0;
                                        SetupBlitter.ulSrcPosX    = 0;
                                        SetupBlitter.ulSrcPosY    = 0;

                                        SetupBlitter.fInvert      = FALSE; // origin is always in the upper left corner
                                        SetupBlitter.ulDitherType = 0;

                                        SetupBlitter.fccDstColorFormat = FOURCC_SCRN;
                                        SetupBlitter.ulDstWidth        = swp.cx;
                                        SetupBlitter.ulDstHeight       = swp.cy;
                                        SetupBlitter.lDstPosX          = 0;
                                        SetupBlitter.lDstPosY          = 0;
                                        SetupBlitter.lScreenPosX       = pointl.x - 4;
                                        SetupBlitter.lScreenPosY       = pointl.y - 5;
                                        SetupBlitter.ulNumDstRects     = rgnCtl.crcReturned;
                                        SetupBlitter.pVisDstRects      = rcls;
                                        w->pBlitter->Setup( &SetupBlitter );
                                } // if
                                else
                                        w->pBlitter->Setup( NULL );

                                GpiDestroyRegion( hps, hrgn );
                                WinReleasePS( hps );
                        } // if
                        return (MRESULT)FALSE;
                } // case WM_VRNENABLED

/*                case WM_ACTIVATE:
                {
                        if( (BOOL)SHORT1FROMMP( mp1 ) )
                                w->Activate();
                        else
                                w->Deactivate();

                        return (MRESULT)FALSE;
                } // case WM_ACTIVATE*/
        } // switch

        return rc;
} // NewWndProc


/*DOC
CLASS XBlitterWindow
FUNCTION XBlitterWindow
GROUP x-games/constructors/destructors
REMARKS Construct a blitter-window
Note that destructors of windows are called automaticaly when a window is closed! (see ~XBlitterWindow).
This constructor throws a XGameException in case of an error.
PARAMETERS XResource * resource        a XResource contains two informations, an ID and a pointer
                                        to a XResourceLibrary. If you want to create a window out of
                                        a resourcefile you must specify the ID (otherwise it can be zero)
                                        and the XResourceLibrary which contains the window-resource.
                                        The window which is created always belongs to the process who
                                        owns the resource library, so if you work with multiple processes
                                        every process must have its own resource library.
            char * theTitle             The title of the window which is displayed in the titlebar
            XBlitter* blitter           Pointer to a blitter object
            ULONG style                 You can specify the style of the window with the following defines,
                                        which can be or-ed:

                                           FRM_TITLEBAR     the window gets a titlebar
                                           FRM_SYSMENU      the window gets the system menu
                                           FRM_MINBUTTON    the titlebar get a button to minimize the window
                                           FRM_MAXBUTTON    the titlebar get a button to maximize the window

                                           FRM_CENTER       the window is created in the midle of the workplace

                                           FRM_SIZEBORDER   the windowsize can be changed by the user
                                           FRM_DIALOGBORDER the window gets a thick border
                                           FRM_BORDER       the window gets a thin border

                                           FRM_TASKLIST     the window is displayed in the tasklist

                                           FRM_NOMOVEWITHOWNER  the window dontït move when the parent is moved
                                           FRM_ICON         the window get an icon wich is identified by theResourceID,
                                                            if the icon is not found in the resource-library, an error ocurses
                                           FRM_ACCELTABLE   an acceltable will be loaded from the resources with the windows id.
                                           FRM_SYSMODAL     the window is displayed system-modal
                                           FRM_SCREENALIGN
                                           FRM_MOUSEALIGN
                                           FRM_HIDEBUTTON
                                           FRM_HIDEMAX
                                           FRM_AUTOICON


                                        there are three static member-variables for default styles

                                           long defaultStyle          default setting for a framewindow
                                           long defaultClientStyle    default setting for windows wich are displayed as a clientwindow of a framewindow
                                           long defaultDialogStyle    default setting for windows wich are displayed as a dialog

                                        Default is defaultStyle.
            XRect * theRectangle        On default a window is created with length and hight of zero. Windows
                                        which are created with an resource template get the size of the template.
                                        Default is NULL.
                                        If theRectangle is specified, the window gets the size of it.
            XFrameWindow * parent       If parent is specified the window is a client of the parent. The
                                        behavior depends on the styles you have set.
                                        Default is NULL.
            BOOL buildFromResource      If this variable is set OOL try to build the window with a resource
                                        template which is identified by theResourceID. If the template is
                                        not found, an error ocurses.
                                        Default is FALSE.
            BOOL animate                Enable/disable animation on window creation. Default is FALSE
*/
XBlitterWindow::XBlitterWindow( const XResource * resource,
                                const char * title,
                                      XBlitter* pBlitter,
                                const ULONG createstyle,
                                const XRect& rec,
                                const XFrameWindow * parent,
                                const BOOL buildFromResources,
                                const BOOL animate                 )
        : XFrameWindow( resource, title, createstyle, rec, parent, buildFromResources, animate )
{
   Init(pBlitter);
} // XBlitterWindow::XBlitterWindow


XBlitterWindow::XBlitterWindow( const ULONG resource,
                                const char * title,
                                XBlitter* pBlitter,
                                const ULONG createstyle,
                                const XRect& rec,
                                const XFrameWindow * parent,
                                const BOOL buildFromResources,
                                const BOOL animate                 )
        : XFrameWindow( resource, title, createstyle, rec, parent, buildFromResources, animate )
{
   Init(pBlitter);
} // XBlitterWindow::XBlitterWindow


void XBlitterWindow::Init(XBlitter* pBlitter)
{
        this->pBlitter = pBlitter;

        // subclass the client window
        pOldWndProc = WinSubclassWindow( GetHandle(), NewWndProc );

        // Turn on visible region notification.
        WinSetVisibleRegionNotify( GetHandle(), TRUE );

        // Send an invalidation message to the client.
        WinPostMsg( GetHandle(), WM_VRNENABLED, 0L, 0L );
}


/*DOC
CLASS XBlitterWindow
FUNCTION ~XBlitterWindow
GROUP x-games/constructors/destructors
REMARKS Destructors of windows are called automaticaly when the window is closed.
The destructor of XBlitterWindow calls the destructor of every client window, menus and
toolbars. Also the destructors of handlers (derived classes of XHandler) which are
attached with it are called. If the last XFrameWindow of a process is closed, the process
will terminate.<BR>
If you want to close a window yourself, destruct the window with delete.
*/


/*DOC
CLASS XBlitterWindow
FUNCTION GetBlitter
GROUP x-games
REMARKS Returns a pointer to the blitter object that belongs to the frame object
RETURNS XBlitter* pBlitter  the blitter object
*/

