#include "xtime.h"
#include "xstring.h"
#include "time.h"
#include "xdate.h"

/*@
@class XDate
@parent XObject
@type overview
@symbol _
*/

/*@
@class XTime
@parent XObject
@type overview
@symbol _
*/

/*@ XTime::operator ==(const XTime &ti)
@group operators
@remarks Compare two XTimes
*/

/*@ XTime::operator !=(const XTime &ti)
@group operators
@remarks Compare two XTimes
*/


/*@ XTime::operator =(const XTime &ti)
@group operators
@remarks Copy the current time to another XTime instance
*/


/*@ XTime :: XTime(UCHAR hours, UCHAR minutes, UCHAR seconds)
@group constructors/desructors
@remarks Construct a XTime, if no parameters are used all values are zero, call GetCurrentTime() to get the current time.
@parameters: UCHAR hours
<t 'ø' c=2>
øUCHAR øminutes
øUCHAR øseconds
</t>
*/
XTime :: XTime( const UCHAR hours, const UCHAR minutes, const UCHAR seconds)
{
   t.hours = hours;
   t.minutes = minutes;
   t.seconds = seconds;
   t.dayLightSave = FALSE;
}


/*@ XTime::Compare(const XTime * ti)
@group misc
@remarks Compares the time stored with another time
@parameters: XTime * time
@returns  <t 'ø' c=2>
            øSHORT result      ø-1 the time stored is earlier, 0 the times are equal,1 the time stored is later
         </t>
*/
SHORT XTime::Compare(const XTime * ti) const
{
   if (t.hours < ti->t.hours)
      return -1;
   if (t.hours > ti->t.hours)
      return 1;
   if (t.minutes < ti->t.minutes)
      return -1;
   if (t.minutes > ti->t.minutes)
      return 1;
   if (t.seconds < ti->t.seconds)
      return -1;
   return (t.seconds > ti->t.seconds ? 1 : 0);
}


/*@ XTime::GetCurrentTime(void)
@group misc
@remarks Query the current time.
*/
void XTime::GetCurrentTime(void)
{
   tm *ti;
   time_t ltime;

   time(&ltime);
   ti = localtime(&ltime);

   t.hours = ti->tm_hour;
   t.minutes = ti->tm_min;
   t.seconds = ti->tm_sec;
   t.dayLightSave = ti->tm_isdst;
}


/*@ XTime::Format(XString * buffer, const char *format)
@group misc
@remarks Print the date formated in a string
@parameters
<t 'ø' c=2>
øXString * øbuffer which will hold data
øchar * østring which holds the format to use, see strftime() for valid format attributes
</t>
*/
void XTime::Format(XString * buffer, const char *format) const
{
   tm  td;

   td.tm_sec = t.seconds;
   td.tm_min = t.minutes;
   td.tm_hour = t.hours;
   td.tm_mday = 0;
   td.tm_mon = 0;
   td.tm_year = 0;
   td.tm_wday = 0;
   td.tm_yday = 0;
   td.tm_isdst = t.dayLightSave;

   size_t l = strftime(buffer->GetBuffer(512), 512, format, &td);

   buffer->ReleaseBuffer(l);
}


/*@ XTime::IsLegalTime(void)
@group misc
@remarks Query if the time is correct
@returns BOOL
*/
BOOL XTime :: IsLegalTime(void) const
{
   if( t.hours < 24 && t.minutes < 60 && t.seconds < 60)
      return TRUE;
   else
      return FALSE;
}


XDate :: XDate( const UCHAR day, const UCHAR month, const SHORT year, const UCHAR weekD, const USHORT dayOfYear)
{
   d.days = day;
   d.months = month;
   d.years = year;
   d.weekday = weekD;
   d.dayOfYear = dayOfYear;
}


/*@ XDate::Compare(const XDate * date)
@group misc
@remarks Compares the date with another
@parameters XDate* date to compare with
@returns  <t 'ø' c=2>
            øSHORT result      ø-1 the date stored is earlier, 0 the date are equal,1 the date stored is later
         </t>
*/
SHORT XDate::Compare(const XDate * date) const
{
   if (d.years < date->d.years)
      return -1;
   if (d.years > date->d.years)
      return 1;
   if (d.months < date->d.months)
      return -1;
   if (d.months > date->d.months)
      return 1;
   if (d.days < date->d.days)
      return -1;
   return (d.days > date->d.days ? 1 : 0);
}


/*@ XDate::GetCurrentDate(void)
@group misc
@remarks Query the current date from the system
*/
void XDate::GetCurrentDate(void)
{
   tm *t;
   time_t ltime;

   time(&ltime);
   t = localtime(&ltime);
   d.days = t->tm_mday;
   d.months = t->tm_mon + 1;
   d.years = t->tm_year + 1900;
   d.weekday = t->tm_wday;
   d.dayOfYear = t->tm_yday;
}


/*@ XDate::Format(XString * buffer, const char *format)
@group misc
@remarks Print the date into a string
@parameters
<t 'ø' c=2>
øXString* øbuffe which gets the date
øchar* øformat, see strftime() for details
</t>
*/
void XDate::Format(XString * buffer, const char *format) const
{
   tm  td;

   td.tm_sec = 0;
   td.tm_min = 0;
   td.tm_hour = 0;
   td.tm_mday = d.days;
   td.tm_mon = d.months - 1;
   td.tm_year = d.years - 1900;
   td.tm_wday = d.weekday;
   td.tm_yday = d.dayOfYear;
   td.tm_isdst = 0;

   size_t l = strftime(buffer->GetBuffer(512), 512, format, &td);

   buffer->ReleaseBuffer(l);
}


/*@ XDate :: IsLeapYear()
@group arithmetik
@remarks Query if a leap year is given
@returns BOOL
*/
BOOL XDate :: IsLeapYear() const
{
   if( d.years % 4 == 0 )
   {
      if( d.years % 100 == 0 && d.years % 400 != 0)
         return FALSE;
      return TRUE;
   }
   else
      return FALSE;
}


/*@ XDate :: GetTotalDaysOfYear()
@group arithmetik
@remarks returns the count of days of the current year
@returns USHORT count
*/
USHORT XDate :: GetTotalDaysOfYear() const
{
   return ( IsLeapYear() ? 366 : 365 );
}


SHORT daysInMonth[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};


/*@ XDate :: GetDayOfYear()
@group arithmetik
@remarks Returns the number of the day in the current year.
@returns USHORT number (1-366)
*/
USHORT XDate :: GetDayOfYear() const
{
   USHORT result = daysInMonth[d.months-1] + d.days;
   if( d.months > 2 && IsLeapYear())
      result++;
   return result;
}


/*@ XDate :: GetDaysOfMonth()
@group arithmetik
@remarks Returns the number of days in the cureent month
@returns UCHAR number (1-31)
*/
UCHAR XDate :: GetDaysOfMonth() const
{
   if(d.months == 2 && IsLeapYear())
      return 29;
   else
      return daysInMonth[d.months] - daysInMonth[d.months-1];
}


/*@ XDate :: GetTotalDays()
@group arithmetik
@remarks Returns the number of days
@returns ULONG days
*/
ULONG XDate :: GetTotalDays() const
{
   ULONG year = d.years - 1;
   return ( year * 365 + year / 4 - year / 100 + year / 400 + GetDayOfYear() - 1);
}


/*@ XDate :: GetWeekDay()
@group arithmetik
@remarks Returns the weekday of the current date.
@returns UCHAR weekday (0-6, 0=monday)
*/
UCHAR XDate :: GetWeekDay() const
{
   return GetTotalDays() % 7;
}


/*@ XDate :: GetDateFromDay( const USHORT days, const USHORT years)
@group arithmetik
@remarks Returns a date which is created from a number of days in a given year.
@parameters
<t 'ø' c=2>
øUSHORT ødays (1-366)
øUSHORT øyears
</t>
@returns XDate result
*/
XDate XDate :: GetDateFromDay( const USHORT day, const USHORT years)
{
   XDate date;
   SHORT i;

   date.SetYears( years );

   CHAR leapAdd = 0;
   if( date.IsLeapYear() && day >= 60 )
      leapAdd = 1;

   for( i = 12; i > 0; i--)
   {
      if( day > daysInMonth[i] + leapAdd)
         break;
   }

   date.SetMonths( i + 1);
   if(day == 60)
      leapAdd = 0;
   date.SetDays( day - daysInMonth[i] - leapAdd);

   return date;
}


/*@ XDate :: AddDays(const SHORT days)
@group arithmetik
@remarks Add/substract days to/from the current date.
@parameters SHORT days to add/substract (can be negative)
@returns BOOL result
*/
BOOL XDate :: AddDays(const SHORT days)
{
   ULONG total = GetTotalDays();
   total += (days + 1);
   if( total < 1)
      return FALSE;

   ULONG resYear = (total * 400) / 146097 + 1;

   XDate buffer(1,1,resYear);
   ULONG resDays = total - buffer.GetTotalDays();
   *this = GetDateFromDay( resDays, resYear );
   return TRUE;
}


/*@ XDate :: GetWeek()
@group arithmetik
@remarks Returns the number in the current year.
@returns USHORT weeknumber (1-53)
*/
USHORT XDate :: GetWeek() const
{
   XDate buffer(1,1,d.years);

   UCHAR weekday = buffer.GetWeekDay();
   USHORT days = GetDayOfYear();
   USHORT week = days / 7;
   if( weekday <= 3 )
      week ++;
   if(week == 0)
   {
      week = 52;
      if( weekday <= 3 )
         week ++;
   }
   else if( week == 53 && weekday <= 3)
      week = 1;

   return week;
}


/*@ XDate::GetWeekBegin()
@group arithmetik
@remarks Returns the date of the 1st day of the week
@returns XDate result
*/
XDate XDate::GetWeekBegin() const
{
   XDate date;

   date = *this;
   date.AddDays( -date.GetWeekDay());

   return date;
}


#define FRONLEICHNAM 1
#define ERSTERMAI 2
#define NEUJAHR 3
#define BUSSBETTAG 4
#define KARFREITAG 5
#define OSTERMONTAG 6
#define HIMMELFAHRT 7
#define PFINGSTMONTAG 8

USHORT XDate::IsCeremon()
{
   if( d.months == 5 && d.days == 1)
      return ERSTERMAI;

   if( d.months == 1 && d.days == 1)
      return NEUJAHR;

   XDate buffer(16, 11, d.years);
   CHAR weekDay = buffer.GetWeekDay();
   buffer.SetDays( 25 - weekDay );
   if( buffer.GetDays() > 22 )
      buffer.SetDays( buffer.GetDays() - 7);
   if(buffer == *this)
      return BUSSBETTAG;

   ULONG year1 = (d.years / 100) - (d.years / 400) + 4;
   ULONG year2 = year1 + 11 - ( d.years / 300 );
   ULONG r1 = ((d.years % 19) * 19 + year2) % 30;
   ULONG r2 = (((d.years % 4) * 2 + (4 * d.years) + (6 * r1) + year1) % 7) + r1 + 22;

   if(( r2 == 57) || ((r1 ==28) && (r2=56) && ((11*(year2+1) % 30) < 19 )))
      r2 -= 7;

   if( IsLeapYear())
      r2 += 14;
   else
      r2 += 13;

   XDate ascherMittwoch = GetDateFromDay( r2, d.years);
   buffer = ascherMittwoch;
   buffer.AddDays( 44 );
   if(buffer == *this)
      return KARFREITAG;
   buffer.AddDays( 3 );
   if(buffer == *this)
      return OSTERMONTAG;
   buffer.AddDays( 38 );
   if(buffer == *this)
      return HIMMELFAHRT;
   buffer.AddDays( 11 );
   if(buffer == *this)
      return PFINGSTMONTAG;
   buffer.AddDays( 10 );
   if(buffer == *this)
      return FRONLEICHNAM;

   return 0;
}


/*@ XDate :: IsLegalDate()
@group misc
@remarks Chcek if the date is coprrect
@returns BOOL
*/
BOOL XDate :: IsLegalDate() const
{
    if( d.months > 0 && d.months < 13)
    {
        if( d.days > 0 && d.days <= GetDaysOfMonth())
            return TRUE;
    }
    return FALSE;
}


/*@ XDate :: IsLegalDate()
@group misc
@remarks Chcek if the date is coprrect
@returns BOOL
*/


/*@ XDate :: GetDays()
@group get/set data
@remarks Get the day
@returns UCHAR
*/

/*@ XDate :: GetMonths()
@group get/set data
@remarks Get the month
@returns UCHAR
*/

/*@ XDate :: GetYears()
@group get/set data
@remarks Get the years
@returns USHORT
*/


/*@ XDate :: SetDays(UCHAR)
@group get/set data
@remarks Set the day
@parameters UCHAR days
*/

/*@ XDate :: SetMonths(UCHAR)
@group get/set data
@remarks Set the month
@parameters UCHAR months
*/

/*@ XDate :: SetYears(USHORT)
@group get/set data
@remarks Set the years
@parameters USHORT years
*/


/*@ XTime :: SetHours(UCHAR)
@group get/set data
@remarks Set the hours
@parameters UCHAR hours
*/

/*@ XTime :: SetMinutes(UCHAR)
@group get/set data
@remarks Set the minutes
@parameters UCHAR minutes
*/

/*@ XTime :: SetSeconds(UCHAR)
@group get/set data
@remarks Set the seconds
@parameters UCHAR seconds
*/

/*@ XTime :: GetHours()
@group get/set data
@remarks get the hours
@returns USHAR hours
*/


/*@ XTime :: GetMinutes()
@group get/set data
@remarks Get the minutes
@returns UCHAR minutes
*/

/*@ XTime :: GetSeconds()
@group get/set data
@remarks Get the seconds
@returns UCHAR seconds
*/
#include "xmsgbox.h"
BOOL XDate :: Set(const char * s, SHORT format)
{
   XString date = s;
   ULONG fo;
   char * search = NULL;
   if( date.Find( fo, "."))
      search = ".";
   else if(date.Find(fo, "-"))
      search = "-";
   else if(date.Find(fo, "/"))
      search = "/";
   else if(date.Find(fo, " "))
      search = " ";

   if(search == NULL)
      return FALSE;
   else
   {
      XString buffer = date.Left(fo);
      buffer.Strip();
      CHAR days = buffer.MakeLong();
      ULONG ne;

      if( date.Find(ne, search, fo + 1))
      {
         buffer = date.Mid(fo + 1, ne - fo - 1);
         buffer.Strip();
         CHAR months = buffer.MakeLong();
         buffer = date.Right( date.GetLength() - ne - 1);
         buffer.Strip();
         SHORT years = buffer.MakeLong();
         XMessageBox((char*) date.Right( date.GetLength() - ne - 1));
         if(years < 100)
         {
            if(years < 50)
               years += 2000;
            else
               years += 1900;
         }
         if( years < 1000)
            years += 1000;

         SetYears(years);
         if(format == 1)
         {
            SetDays(days);
            SetMonths(months);
         } /* end if */
         else
         {
            SetDays(months);
            SetMonths(days);
         }
      } /* end if */
      else
         return FALSE;
   } /* end if */
   return TRUE;
}