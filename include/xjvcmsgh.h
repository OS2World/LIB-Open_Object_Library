#ifndef __OOL_XJAVACONMSGHND_H__
#define __OOL_XJAVACONMSGHND_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XJavaConsoleMessageHandler            */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Thomas Bonk (thomas@ghecko.saar.de)   */
/*===========================================================================*/

#include <xobject.h>



class _export_ XJavaConsoleMessageHandler : public XObject
{
        public:
                // ctor/dtor
                         XJavaConsoleMessageHandler():XObject() {}
                virtual ~XJavaConsoleMessageHandler() {}

                virtual void ShowMessage( XString& ) {}
                virtual int  ShowConsole( void );
                virtual int  HideConsole( void );
};

#endif

