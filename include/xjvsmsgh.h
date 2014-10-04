#ifndef __OOL_XJAVASTATMSGHND_H__
#define __OOL_XJAVASTATMSGHND_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XJavaStatusMessageHandler             */
/*                       derived from: XObject                               */
/*                        last update: 02/97                                 */
/*                      programmed by: Thomas Bonk (thomas@ghecko.saar.de)   */
/*===========================================================================*/

#include <xobject.h>



class _export_ XJavaStatusMsgHandler : public XObject
{
        public:
                // ctor/dtor
                         XJavaStatusMsgHandler() : XObject() {}
                virtual ~XJavaStatusMsgHandler() {}

                virtual void ShowStatusMessage( XString& msg ) = 0;
};

#endif

