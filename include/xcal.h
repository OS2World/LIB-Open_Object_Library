#ifndef __OOL_XCALENDAR_H__
#define __OOL_XCALENDAR_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XCalendarControl                      */
/*                       derived from: XUserWindow                           */
/*                        last update: 7/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xdate.h"
#include "XfrmWnd.h"
#include "xuserwnd.h"
#include "xtext.h"

class XGraphicDevice;


class _export_ XCalendarControl: public XUserWindow
{
   friend class CalendarHandler;
   private:
      XGraphicDevice * dev;
      XText ** dayList;
      UCHAR count;
      XDate date;
   public:
      XCalendarControl( const XWindow *, const XPoint& = XPoint(), const USHORT id = 0, const XDate& date=XDate());
      ~XCalendarControl();
      virtual void Draw( void );
      virtual void DaySelected( const UCHAR selectedDay ) {;}
      XText * GetDayObject(const UCHAR day) const;
};

#endif
