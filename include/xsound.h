#ifndef __OOL_XSOUND_H__
#define __OOL_XSOUND_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XSound                                */
/*                       derived from: XMediaWindow                          */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xmedia.h"

class XFrameWindow;

class _export_ XSound: public XMediaWindow{
   private:
      USHORT type;
   public:
      XSound( const XFrameWindow * w): XMediaWindow(w) {;}
//      LONG OpenDevice( const char * p);
      LONG SetDataFile( const char * p);
};


#endif
