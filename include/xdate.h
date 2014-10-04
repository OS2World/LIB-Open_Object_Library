#ifndef __OOL_XDATE_H__
#define __OOL_XDATE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDate                                 */
/*                       derived from: XObject                               */
/*                        last update: 1/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xobject.h"
#include "string.h"

typedef struct XDateStructure{
   UCHAR days;
   UCHAR months;
   USHORT years;
   UCHAR weekday;
   USHORT dayOfYear;
} XDateStruct;


class XString;


class _export_ XDate: public XObject
{
      friend class XContainerObject;
      friend class XIO;
      friend class XFileInfo;
   private:
      XDateStructure d;
   public:
      XDate( const UCHAR day=0, const UCHAR month=0, const SHORT year=0, const UCHAR weekd=0, const USHORT dayOfYear=0);
      XDate( const XDate& da) { memcpy(&d, &da.d, sizeof(d));}
      virtual ~XDate() {;}
      BOOL AddDays(const SHORT days);
      SHORT Compare(const XDate*d) const;
      void GetCurrentDate( void);
      static XDate GetDateFromDay( const USHORT days, const USHORT years);
      UCHAR GetDays( void ) const { return d.days; }
      UCHAR GetDaysOfMonth() const;
      USHORT GetDayOfYear(void) const ;//{ return d.dayOfYear; }
      UCHAR GetMonths( void ) const { return d.months; }
      ULONG GetTotalDays() const;
      USHORT GetTotalDaysOfYear() const;
      USHORT GetWeek() const;
      XDate GetWeekBegin() const;
      UCHAR GetWeekDay() const;
      USHORT GetYears( void ) const { return d.years; }
      void Format( XString * buffer, const char * format) const;
      USHORT XDate::IsCeremon();
      BOOL IsLeapYear() const;
      BOOL IsLegalDate() const;
      void SetDays( const UCHAR days) { d.days = days; }
      void SetMonths( const UCHAR months) { d.months = months; }
      void SetWeekDay( const UCHAR weekd) { d.weekday = weekd; }
      void SetYears( const USHORT years) { d.years = years; }
      BOOL Set(const char * s, SHORT format);
      XDate operator = (const XDate& da) { memcpy(&d, &da.d, sizeof(d)); return *this; }
      BOOL operator == (const XDate &da) { return ( Compare( &da ) == 0 ? TRUE : FALSE); }
      BOOL operator != (const XDate &da) { return ( Compare( &da ) != 0 ? TRUE : FALSE); }
      BOOL operator > (const XDate &da) { return ( Compare( &da ) > 0 ? TRUE : FALSE); }
      BOOL operator < (const XDate& da) { return( Compare(&da) < 0 ? TRUE : FALSE); }
};


#endif
