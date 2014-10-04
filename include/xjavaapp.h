#ifndef __OOL_XJAVAAPP_H__
#define __OOL_XJAVAAPP_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XJavaApplet                           */
/*                       derived from: XControl                              */
/*                        last update: 02/97                                 */
/*                      programmed by: Thomas Bonk (thomas@ghecko.saar.de)   */
/*===========================================================================*/

#include <xstring.h>
#include <xjavaenv.h>
#include <xcontrol.h>



class _export_ XJavaApplet : public XControl
{
        private:
                JEAPPLETHND hApplet;
                USHORT      ID;
                XWindow*    Owner;

        public:
                //ctor/dtor
                         XJavaApplet(       XWindow* owner,
                                            XRect*   rect,
                                      const char*    documentURL,
                                      const char*    code,
                                            USHORT   id        = 0,
                                            int      parmCount = 0,
                                      const char**   parms     = 0  );
                virtual ~XJavaApplet();

                int Start( void );
                int Stop( void );
                int Destroy( void );
};

#endif

