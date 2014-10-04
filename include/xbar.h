#ifndef __OOL_XBAR_H__
#define __OOL_XBAR_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XBar                                  */
/*                       derived from: XChart                                */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "XChart.h"
#include "XBox.h"

class _export_ XBar: public XChart
{
      XBox ** box;
   public:
      XBar( const XWindow * w, const XRect& rec=XRect(), const LONG id=0, const LONG st = 0, const char* t = NULL);
      void Calculate( void );
      void GetItemColor( const USHORT index, XColor * );
      void SetItemColor( const USHORT index, const XColor *);
      void SetItemCount( const USHORT count );
};

#endif
