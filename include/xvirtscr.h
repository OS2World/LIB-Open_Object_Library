#if !defined( __XVIRTUALSCREEN_HPP__ )
#define __XVIRTUALSCREEN_HPP__

#include <xheaders.h>
#include "xobject.h"
#include <mmioos2.h>
#include <dive.h>
#include <fourcc.h>
#include "xgamexc.h"


class _export_ XVirtualScreen : public XObject
{
        private:
                USHORT  usWidth, usHeight;
                BYTE*   pbBuffer;

        public:
                // ctor/dtor
                         XVirtualScreen( USHORT usWidth, USHORT usHeight );
                virtual ~XVirtualScreen();

                // return width and height
                inline USHORT GetWidth( void )  { return usWidth; }
                inline USHORT GetHeight( void ) { return usHeight; }

                // get pointer to the buffer
                inline BYTE* GetBuffer( void )  { return pbBuffer; }
};

#endif
