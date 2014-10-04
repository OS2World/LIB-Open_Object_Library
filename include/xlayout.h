#ifndef __OOL_XLAYOUT_H__
#define __OOL_XLAYOUT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XLayout                               */
/*                       derived from: XHandler                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xhandler.h"

class _export_ XLayout: public XHandler{
   public:
      XLayout( const XWindow * w): XHandler( OOL_LAYOUTHANDLER, (XWindow*) w ) { ;}
      virtual void Arrange(XSize*) {;}
      virtual void AddWindow(XWindow*) {;}
      virtual void RemoveWindow(XWindow*) {;}
};

#endif