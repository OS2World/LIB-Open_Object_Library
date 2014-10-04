#if !defined( __XPALETTE_HPP__ )
#define __XPALETTE_HPP__

#include <xheaders.h>
#include "xobject.h"


//  Class Definition
class _export_ XPalette : public XObject
{
        friend class XBlitter;

        private:
                ULONG   aulData[256];
      ULONG   aulWPSPalette[256];

                HPS     hpsDesktop;
                HDC     hdcDesktop;

        public:
                // ctor/dtor
                         XPalette( void );
                virtual ~XPalette( void )       { WinReleasePS( hpsDesktop ); }

                // sets a range of colors
                void SetColors( PULONG pulData, const ULONG ulStart = 10, const ULONG ulNumEntries = 236 );

                // enable/disable palette
                void Enable( BOOL flInvalidate = TRUE );
                void Disable( BOOL flInvalidate = TRUE );
};

#endif
