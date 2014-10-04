#ifndef __OOL_XBOX_H__
#define __OOL_XBOX_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XBox                                  */
/*                       derived from: XLine                                 */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xline.h"
#include "xpoint.h"

class XRect;

class _export_ XBox: public XLine
{
   private:
      char fill;
   public:
      XBox( const XPoint *, const XPoint *, const BOOL fill=TRUE);
      virtual ~XBox() {;}
      void Draw( XGraphicDevice * dev);
};


#endif
