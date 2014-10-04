/*
 * Fire - a sample program for the XGame library
 * Copyright (C) 1997 by Thomas Bonk (thomas@ghecko.saar.de).
 * All rights reserved.
 * Fire.hpp - class declarations
 */
#if !defined( __FIRE_HPP__ )
#define __FIRE_HPP__

#include <stdlib.h>
#include <xheaders.h>
#define INCL_OOL
#include "ool.h"


class FlameThread : public XThread
{
        private:
                XBlitter* pBlitter;
                XPalette* pPal;
                PBYTE     pBuffer;

                void PrepPal( void );
                void ScrollUp( void );
                void NewLine( void );

        public:
                // ctor
                FlameThread( XBlitter* pblt ) : pBlitter( pblt ), XThread() {}

                void Init( void );
};


class FlameWindow : public XBlitterWindow
{
        public:
                FlameWindow( const ULONG resource,
                             const char * title,
                                   XBlitter* pBlitter,
                             const ULONG     blitting,
                             const ULONG createstyle = defaultStyle,
                             const XRect& rec=XRect()  ) : XBlitterWindow( resource, title, pBlitter, createstyle, rec ) {}//blitting,
                virtual void Activate( void ) const {}
                virtual void Deactivate( void ) {}
};

#endif
