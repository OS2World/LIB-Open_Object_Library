#if !defined( __XBLITTERWINDOW_HPP__ )
#define __XBLITTERWINDOW_HPP__

#include <xheaders.h>
#include "xfrmwnd.h"
#include "xpal.h"
#include "xblitter.h"


class _export_ XBlitterWindow : public XFrameWindow
{
        friend MRESULT EXPENTRY NewWndProc( HWND, ULONG, MPARAM, MPARAM );

        private:
                PFNWP                   pOldWndProc;            // pointer to the old window proc

                XBlitter*               pBlitter;               // pointer to the blitter object

                inline void Activate( void )   const { pBlitter->pPalette->Enable(); }
                inline void Deactivate( void )  { pBlitter->pPalette->Disable(); }
                void Init(XBlitter* pBlitter);

        public:
                // ctor / dtor
                         XBlitterWindow( const XResource * resource,
                                         const char * title,
                                               XBlitter* pBlitter,
                                         const ULONG createstyle = defaultStyle,
                                         const XRect& rec = XRect(),
                                         const XFrameWindow * parent=NULL,
                                         const BOOL buildFromResources = FALSE,
                                         const BOOL animate = FALSE                 );
                         XBlitterWindow( const ULONG,
                                         const char * title,
                                               XBlitter* pBlitter,
                                         const ULONG createstyle = defaultStyle,
                                         const XRect& rec = XRect(),
                                         const XFrameWindow * parent=NULL,
                                         const BOOL buildFromResources = FALSE,
                                         const BOOL animate = FALSE                 );
                virtual ~XBlitterWindow()       {}

                // get a pointer to the blitter object
                XBlitter* GetBlitter( void )    { return pBlitter; }
};

#endif
