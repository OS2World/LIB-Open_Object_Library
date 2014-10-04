#ifndef __OOL_XTIMESL_H__
#define __OOL_XTIMESL_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XTimeSlice                            */
/*                       derived from: XChart                                */
/*                        last update: 3/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "XChart.h"
#include "XLine.h"

class _export_ XTimeSlice: public XChart
{
      XLine ** lines;
      double maxValue;
      XColor color;
      LONG width;
   public:
      XTimeSlice( const XWindow * w, const XRect& rec=XRect(), const LONG id=0, const LONG st = 0, const char* t = NULL);
      void Calculate( void );
      void SetLineWidth(const LONG w);
      void SetLineColor( const XColor * col);
      void SetMaxValue( const double val) { maxValue = val; }
      void SetItemCount( const USHORT count );
};

#endif
