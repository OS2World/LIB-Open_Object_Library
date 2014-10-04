#ifndef __OOL_XJAVASHOWURLHND_H__
#define __OOL_XJAVASHOWURLHND_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XJavaShowURLHandler                   */
/*                       derived from: XObject                               */
/*                        last update: 01/97                                 */
/*                      programmed by: Thomas Bonk (thomas@ghecko.saar.de)   */
/*===========================================================================*/

#include <xobject.h>



class _export_ XJavaShowURLHandler : public XObject
{
        public:
                // ctor/dtor
                         XJavaShowURLHandler() : XObject() {}
                virtual ~XJavaShowURLHandler() {}

                virtual void ShowURL( XString& url, XString& target ) = 0;
};

#endif

