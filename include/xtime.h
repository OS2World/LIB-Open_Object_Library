#ifndef __OOL_XTIME_H__
#define __OOL_XTIME_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XTime                                 */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"
#include "string.h"


class XString;

typedef struct XTimeStructure{
   UCHAR hours;
   UCHAR minutes;
   UCHAR seconds;
   BOOL dayLightSave;
} XTimeStruct;


class _export_ XTime: public XObject
{
      friend class XContainerObject;
      friend class XIO;
      friend class XFileInfo;
   private:
      XTimeStructure t;
   public:
      XTime( const UCHAR hours=0, const UCHAR minutes=0, const UCHAR seconds=0);
      XTime(const XTime &ti) { memcpy(&t, &ti.t,sizeof(t));}
      virtual ~XTime() {;}
      SHORT Compare( const XTime*) const;
      void GetCurrentTime( void);
      UCHAR GetHours(void) const { return t.hours; }
      UCHAR GetMinutes(void) const { return t.minutes; }
      UCHAR GetSeconds(void) const { return t.seconds; }
      void Format( XString * buffer, const char * format) const ;
      BOOL IsLegalTime(void) const;
      void SetHours( const UCHAR hours ) { t.hours = hours; }
      void SetMinutes( const UCHAR minutes ) { t.minutes = minutes; }
      void SetSeconds( const UCHAR seconds ) { t.seconds = seconds; }
      XTime operator =(const XTime &ti) { memcpy(&t, &ti.t,sizeof(t)); return *this; }
      BOOL operator == (const XTime &ti) { return ( Compare( &ti ) == 0 ? TRUE : FALSE); }
      BOOL operator != (const XTime &ti) { return ( Compare( &ti ) != 0 ? TRUE : FALSE); }
      BOOL operator > (const XTime &ti) { return ( Compare( &ti ) > 0 ? TRUE : FALSE); }
      BOOL operator <(const XTime& ti) { return( Compare(&ti) < 0 ? TRUE : FALSE); }
};

#endif
