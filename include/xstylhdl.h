#ifndef __OOL_XSTYLEHANDLER_H__
#define __OOL_XSTYLEHANDLER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XStyleHandler                         */
/*                       derived from: XHandler                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xhandler.h"

#ifdef OOL_FOR_OS2_X86

   //event - idïs of changed window-style
   #define STY_FONT                      PP_FONTNAMESIZE
   #define STY_FOREGROUNDCOLOR           PP_FOREGROUNDCOLOR
   #define STY_BACKGROUNDCOLOR           PP_BACKGROUNDCOLOR

#endif


class _export_ XStyleHandler: public XHandler
{
   public:
      XStyleHandler( const XWindow * w): XHandler( OOL_STYLEHANDLER, w ) { ;}
      virtual BOOL HandleEvent( XEvent *) { return FALSE; }
};


#endif


