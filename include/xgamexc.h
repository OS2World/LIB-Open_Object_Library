/*
 * XGame - a game programmer's library for OS/2; XGame is an extension to the OOL.
 * Copyright (C) 1997 by Thomas Bonk (thomas@ghecko.saar.de). All rights reserved.
 * XGameException.hpp - declaration of a exception class
 */
#if !defined( __XGAMEEXCEPTION_HPP__ )
#define __XGAMEEXCEPTION_HPP__

#include <xheaders.h>
#include "xexcept.h"


class _export_ XGameException : public XException
{
        public:
                // error codes
                enum {
                        XGERR_OK                  =  0,
                        XGERR_OUT_OF_MEMORY       = -1,
                        XGERR_SCREEN_CAPS         = -2,
                        XGERR_COLOR_DEPTH         = -3,
                        XGERR_BAD_PALFORMAT       = -4,
                        XGERR_BAD_BLITMETHOD      = -5,
                        XGERR_PALETTE             = -6
                };

                // ctor
                XGameException( char* msg, LONG code ) : XException( msg, code ) {}
};

#endif
