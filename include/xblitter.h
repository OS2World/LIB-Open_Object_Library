#if !defined( __XBLITTER_HPP__ )
#define __XBLITTER_HPP__

#include <xheaders.h>
#include "xobject.h"
#define  _MEERROR_H_
#include <mmioos2.h>
#include <dive.h>
#include <fourcc.h>
#include "xgamexc.h"
#include "xvirtscr.h"
#include "xpal.h"

/* EMX compatibility-hack */
#if !defined( __IBMCPP__ ) & !defined( __WATCOMC__ )
   #define mmioFOURCC( ch0, ch1, ch2, ch3 )                         \
                  ( (ULONG)(BYTE)(ch0) | ( (ULONG)(BYTE)(ch1) << 8 ) |    \
                  ( (ULONG)(BYTE)(ch2) << 16 ) | ( (ULONG)(BYTE)(ch3) << 24 ) )
#endif

class _export_ XBlitter : public XObject
{
        friend class XBlitterWindow;

        private:
                HDIVE     hDive;                        // handle to DIVE instance
                DIVE_CAPS DiveCaps;                     // DIVE capabilites
                FOURCC    fccFormats[100];              // supported color formats

                XVirtualScreen* pVScreen;               // pointer to the virtual screen object
                ULONG           ulBufferNumber;         // buffer number of the virtual screen

                XPalette*       pPalette;

        public:
                // ctor/dtor
                         XBlitter( void );
                virtual ~XBlitter();

                // associate/disassociate a virtual screen
                void AssociateVirtualScreen( XVirtualScreen* pVScr );
                void DisassociateVirtualScreen( void );

                // setup the blitter
                void Setup( PSETUP_BLITTER pSetupBlitter );

                // set palette
                void SetPalette( XPalette* xpal );

                // get the pointer to the virtual screen
                XVirtualScreen* GetVirtualScreen( void )   { return pVScreen; }

                // blit the virtual screen
                void Blit( void );
};

#endif
