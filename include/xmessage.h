#ifndef __OOL_XMESSAGE_H__
#define __OOL_XMESSAGE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMessage                              */
/*                       derived from: XObject                               */
/*                        last update: 05/97                                 */
/*                      programmed by: Thomas Bonk (thomas@ghecko.saar.de)   */
/*===========================================================================*/

#include "xobject.h"
#include "xstring.h"

class _export_ XMessage : public XObject
{
        private:
                XString strFilename;

        public:
                     XMessage( const char* fileName ) : strFilename( fileName ) {}
                BOOL GetMessage( XString* pstrMsg, ULONG msgNum, ULONG varNum = 0, const char** varTexts = NULL );
};

#endif

